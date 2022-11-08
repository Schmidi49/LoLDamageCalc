/******************************************************************************
 * @file Jinx.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (26.09.2022)
 */

#include "include/Jinx.h"



namespace LDC::champions{
    Attacker_Jinx::Attacker_Jinx(engine_signal_system* ess, const std::string &name, const int &lvl) :
        Attacker_Champion(ess, name, lvl){
        if(!m_champion_data.empty()) {
            std::cout << "reading Jinx champion specifics" << std::endl;
            if (!m_champion_data.contains("spell_w")) {
                std::cerr << "no \"spell_w\" specified" << std::endl;
                throw std::runtime_error("no spell_w specified in json data!");
            } else {
                delete m_jinx_spell_w;
                m_jinx_spell_w = new Generic_Damage_Spell("attacker_jinx_w", m_champion_data["spell_w"], this,
                                                          m_Defender, m_ess);
            }

            func_spell_w = [SpellPtr = m_jinx_spell_w]
                    (const bool &crit, const bool &enhanced, const int &instance) {
                SpellPtr->execute_spell(crit, enhanced, instance);
            };

            if (!m_champion_data.contains("spell_e")) {
                std::cerr << "no \"spell_e\" specified" << std::endl;
                throw std::runtime_error("no spell_e specified in json data!");
            } else {
                delete m_jinx_spell_e;
                m_jinx_spell_e = new Generic_Damage_Spell("attacker_jinx_e", m_champion_data["spell_e"], this,
                                                          m_Defender, m_ess);
            }

            func_spell_e = [SpellPtr = m_jinx_spell_e]
                    (const bool &crit, const bool &enhanced, const int &instance) {
                SpellPtr->execute_spell(crit, enhanced, instance);
            };

            if (!m_champion_data.contains("spell_r")) {
                std::cerr << "no \"spell_r\" specified" << std::endl;
                throw std::runtime_error("no spell_r specified in json data!");
            } else {
                delete m_jinx_spell_r;
                m_jinx_spell_r = new Generic_Damage_Spell("attacker_jinx_r", m_champion_data["spell_r"], this,
                                                          m_Defender, m_ess);
                if(!m_champion_data["spell_r"].contains("range")){
                    std::cerr << "did not find a range, aborting" << std::endl;
                    throw std::runtime_error("reading json data failed critically!");
                }
                else if(!m_champion_data["spell_r"]["range"].is_array()){
                    std::cerr << "damage type was not a string" << std::endl;
                    throw std::runtime_error("reading json data failed critically!");
                }
                else if(m_champion_data["spell_r"]["range"].size() != 2){
                    std::cerr << "field range has the wrong size" << std::endl;
                    throw std::runtime_error("reading json data failed critically!");
                }
                else if(!m_champion_data["spell_r"]["range"][0].is_number() ||
                        !m_champion_data["spell_r"]["range"][1].is_number()){
                    std::cerr << " range has the wrong type" << std::endl;
                    throw std::runtime_error("reading json data failed critically!");
                }
                else{
                    m_r_range_min = m_champion_data["spell_r"]["range"][0];
                    m_r_range_max = m_champion_data["spell_r"]["range"][1];
                }
            }

            func_auto_attack = [&](const bool &crit, const bool &enhanced, const int &instance){
                execute_auto_attack(crit, enhanced, instance);
            };

            func_passive = [&](const bool &crit, const bool &enhanced, const int &instance){
                execute_passive(crit, enhanced, instance);
            };

            func_spell_q = [&](const bool &crit, const bool &enhanced, const int &instance){
                execute_spell_q(crit, enhanced, instance);
            };

            func_spell_r = [&](const bool &crit, const bool &enhanced, const int &instance){
                execute_spell_r(crit, enhanced, instance);
            };
        }
    }

    Attacker_Jinx::~Attacker_Jinx() {
        m_jinx_spell_w = nullptr;
        m_jinx_spell_e = nullptr;
        m_jinx_spell_r = nullptr;
    }

    bool Attacker_Jinx::set_spell_lvl_q(const int &lvl) {
        if (lvl < 0 || lvl > 5){
            std::cerr << "illegal level: " << lvl << std::endl;
            return false;
        }
        if(!level_check(m_champ_lvl,
                        lvl,
                        get_spell_lvl_w(),
                        get_spell_lvl_e(),
                        get_spell_lvl_r())){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        m_q_lvl = lvl;
        return true;
    }

    bool Attacker_Jinx::set_spell_lvl_w(const int &lvl) {
        if(!level_check(m_champ_lvl,
                        m_q_lvl,
                        lvl,
                        get_spell_lvl_e(),
                        get_spell_lvl_r())){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        return m_jinx_spell_w->set_lvl(lvl);
    }

    bool Attacker_Jinx::set_spell_lvl_e(const int &lvl) {
        if(!level_check(m_champ_lvl,
                        m_q_lvl,
                        get_spell_lvl_w(),
                        lvl,
                        get_spell_lvl_r())){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        return m_jinx_spell_e->set_lvl(lvl);
    }

    bool Attacker_Jinx::set_spell_lvl_r(const int &lvl) {
        if(!level_check(m_champ_lvl,
                        m_q_lvl,
                        get_spell_lvl_w(),
                        get_spell_lvl_e(),
                        lvl)){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        return m_jinx_spell_r->set_lvl(lvl);
    }

    void Attacker_Jinx::execute_spell_q(const bool &crit, const bool &enhanced, const int &instance) {
        m_q_stance ^= true;
    }

    void Attacker_Jinx::execute_spell_r(const bool &crit, const bool &enhanced, const int &instance) {
        if(get_spell_lvl_r() == 0){
            std::cerr << "attacker_jinx_r is not leveled" << std::endl;
            return;
        }

        double rocket_dmg;
        if(instance == 0){
            rocket_dmg = m_r_range_max;
        }
        else if(instance >= m_r_range_min && instance <= m_r_range_max){
            rocket_dmg = instance;
        }
        else{
            std::cerr << "invalid damage range for rocket, valid range: ["
            << m_r_range_min << "," << m_r_range_max << "]" << std::endl;
            return;
        }

        if(m_jinx_spell_r->get_spell_cost() > 0){
            if(!use_mana(m_jinx_spell_r->get_spell_cost())) {
                std::cerr << "spell could not be executed due to mana cost" << std::endl;
                return;
            }
        }
        std::cout << "spell pressed: " << "attacker_jinx_r" << std::endl;

        Damage dmg{m_jinx_spell_r->calculate_damage(crit, enhanced, instance)};
        dmg *= (rocket_dmg / 100.0);

        dmg += -(m_ess->defender.get_premit_dmg_red(dmg));
        dmg *= m_ess->defender.get_dmg_mod(dmg);
        dmg += -(m_ess->defender.get_postmit_dmg_red(dmg));

        m_ess->attacker.deal_damage(dmg);
    }

    void Attacker_Jinx::execute_passive(const bool &crit, const bool &enhanced, const int &instance) {

    }

    void Attacker_Jinx::set_Defender(Defender_Champion *defender) {
        if(defender) {
            m_Defender = defender;
            m_jinx_spell_w->set_Defender(defender);
            m_jinx_spell_e->set_Defender(defender);
            m_jinx_spell_r->set_Defender(defender);
            m_Defender_set = true;
        }
        else {
            m_Defender_set = false;
            throw std::invalid_argument("Pointer towards defender is invalid");
        }
    }
}
