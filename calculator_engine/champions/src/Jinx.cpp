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
}
