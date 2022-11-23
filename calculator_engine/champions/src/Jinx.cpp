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

            //---------------- read passive ----------------
            if(!read_single_float("/passive/as", m_passive_raw_as) || !read_single_float("/passive/ms", m_passive_raw_ms)){
                m_setup_incomplete = true;
                std::cerr << "read passive failed" << std::endl;
            }

            //---------------- read spell q ----------------
            if (!m_champion_data.contains("spell_q")) {
                m_setup_incomplete = true;
                std::cerr << "no spell_q specified" << std::endl;
            }
            else {
                int extracted_q_max_level;
                if(!read_single_int("/spell_q/max_lvl", extracted_q_max_level)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q max_lvl failed" << std::endl;
                }
                else if (m_q_max_lvl != extracted_q_max_level) {
                    m_setup_incomplete = true;
                    std::cerr << "spell_q can not be a other level than 5" << std::endl;
                }

                if(!read_single_float("/spell_q/cost", m_advanced_aa_cost)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q cost failed" << std::endl;
                }
                if(!read_single_float("/spell_q/cd", m_q_cd)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q cd failed" << std::endl;
                }
                if(!read_single_float("/spell_q/ad_modify", m_advanced_aa_ad_modify)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q ad_modify failed" << std::endl;
                }
                if(!read_single_float("/spell_q/ad_modify", m_advanced_aa_ad_modify)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q ad_modify failed" << std::endl;
                }
                if(!read_single_float("/spell_q/as_modify", m_advanced_aa_as_bonus)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q as_modify failed" << std::endl;
                }
                if(!read_array_float("/spell_q/bonus_as_stacks", m_aa_raw_as_stacks, m_q_max_lvl)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_q bonus_as_stacks failed" << std::endl;
                }
            }

            //---------------- read spell w ----------------
            if (!m_champion_data.contains("spell_w")) {
                m_setup_incomplete = true;
                std::cerr << "no spell_w specified" << std::endl;
            } else {
                delete m_jinx_spell_w;
                m_jinx_spell_w = new Generic_Damage_Spell("attacker_jinx_w", m_champion_data["spell_w"], this,
                                                          m_Defender, m_ess);
                m_setup_incomplete |= m_jinx_spell_w->setup_incomplete();
            }

            func_spell_w = [SpellPtr = m_jinx_spell_w]
                    (const bool &crit, const bool &enhanced, const int &instance) {
                SpellPtr->execute_spell(crit, enhanced, instance);
            };

            //---------------- read spell e ----------------
            if (!m_champion_data.contains("spell_e")) {
                m_setup_incomplete = true;
                std::cerr << "no \"spell_e\" specified" << std::endl;
            } else {
                delete m_jinx_spell_e;
                m_jinx_spell_e = new Generic_Damage_Spell("attacker_jinx_e", m_champion_data["spell_e"], this,
                                                          m_Defender, m_ess);
                m_setup_incomplete |= m_jinx_spell_e->setup_incomplete();
            }

            func_spell_e = [SpellPtr = m_jinx_spell_e]
                    (const bool &crit, const bool &enhanced, const int &instance) {
                SpellPtr->execute_spell(crit, enhanced, instance);
            };

            //---------------- read spell r ----------------
            if (!m_champion_data.contains("spell_r")) {
                m_setup_incomplete = true;
                std::cerr << "no \"spell_r\" specified" << std::endl;
            } else {
                delete m_jinx_spell_r;
                m_jinx_spell_r = new Generic_Damage_Spell("attacker_jinx_r", m_champion_data["spell_r"], this,
                                                          m_Defender, m_ess);
                m_setup_incomplete |= m_jinx_spell_r->setup_incomplete();

                double raw_range[2];
                if(!read_array_float("/spell_r/range", raw_range, 2)){
                    m_setup_incomplete = true;
                    std::cerr << "read spell_r range failed" << std::endl;
                }
                else{
                    m_r_range_min = raw_range[0];
                    m_r_range_max = raw_range[1];
                }
            }

            //---------------- defining none generic damage functions ----------------
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
        else{
            m_setup_incomplete = true;
            std::cerr << "creating jinx failed due to missing champion data" << std::endl;
        }
    }

    Attacker_Jinx::~Attacker_Jinx() {
        m_jinx_spell_w = nullptr;
        m_jinx_spell_e = nullptr;
        m_jinx_spell_r = nullptr;
    }

    bool Attacker_Jinx::set_spell_lvl_q(const int &lvl) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return false;
        }
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

        //reset as modifiers from q stances
        if(!m_q_stance && m_q_lvl > 0){
            double reduced_as = 0.0;
            if(m_aa_cur_as_stacks > 0){
                reduced_as -= m_aa_raw_as_stacks[m_q_lvl - 1];
            }
            if(m_aa_cur_as_stacks > 1){
                reduced_as -= m_aa_raw_as_stacks[m_q_lvl - 1] * 0.5 * (m_aa_cur_as_stacks - 1);
            }
            m_stats_bonus_flat->add("as", reduced_as);
        }

        m_q_lvl = lvl;

        //reapply as modifiers from q stances
        if(!m_q_stance && m_q_lvl > 0){
            double enhanced_as = 0.0;
            if(m_aa_cur_as_stacks > 0){
                enhanced_as += m_aa_raw_as_stacks[m_q_lvl - 1];
            }
            if(m_aa_cur_as_stacks > 1){
                enhanced_as += m_aa_raw_as_stacks[m_q_lvl - 1] * 0.5 * (m_aa_cur_as_stacks - 1);
            }
            m_stats_bonus_flat->add("as", enhanced_as);
        }

        calc_current_stats();
        return true;
    }

    bool Attacker_Jinx::set_spell_lvl_w(const int &lvl) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return false;
        }
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
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return false;
        }
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
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return false;
        }
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

    void Attacker_Jinx::execute_auto_attack(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Defender_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }
        if(m_q_stance){
            if(!use_mana(m_advanced_aa_cost)){
                std::cerr << "not enough mana to use advanced auto attack" << std::endl;
                return;
            }

            Damage dmg(*m_stats_current->ad() * m_advanced_aa_ad_modify);
            dmg.type = DamageType::physical;
            dmg.tag = DamageTag::basic;
            dmg.projectile = true;
            dmg.spellshieldAffected = false;
            dmg.cc = false;

            if(crit){
                dmg.crit = true;
                dmg.dmg *= 1.75;
            }

            dmg += -(m_ess->defender.get_premit_dmg_red(dmg));
            dmg *= m_ess->defender.get_dmg_mod(dmg);
            dmg += -(m_ess->defender.get_postmit_dmg_red(dmg));

            m_ess->attacker.deal_damage(dmg);
            m_ess->attacker.execute_onhit();
        }
        // if(!m_q_stance)
        else {
            Damage dmg(*m_stats_current->ad());
            dmg.type = DamageType::physical;
            dmg.tag = DamageTag::basic;
            dmg.projectile = true;
            dmg.spellshieldAffected = false;
            dmg.cc = false;

            if(crit){
                dmg.crit = true;
                dmg.dmg *= 1.75;
            }

            dmg += -(m_ess->defender.get_premit_dmg_red(dmg));
            dmg *= m_ess->defender.get_dmg_mod(dmg);
            dmg += -(m_ess->defender.get_postmit_dmg_red(dmg));

            m_ess->attacker.deal_damage(dmg);
            m_ess->attacker.execute_onhit();

            if(m_q_lvl > 0) {
                if (m_aa_cur_as_stacks < 3) {
                    if(m_aa_cur_as_stacks == 0){
                        m_stats_bonus_flat->add("as", m_aa_raw_as_stacks[m_q_lvl-1]);
                    }
                    else{
                        double temp_as = m_aa_raw_as_stacks[m_q_lvl-1] * 0.5;
                        m_stats_bonus_flat->add("as", temp_as);
                    }
                    calc_current_stats();
                    m_aa_cur_as_stacks++;
                }
            }
        }
    }

    void Attacker_Jinx::execute_passive(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Defender_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }
        //TODO check stats of jinx passive ingame
        //illegal input
        if(enhanced && (instance < 0 || instance > 100)){
            std::cerr << "illegal instance " << instance << " for passive selected" << std::endl;
            return;
        }

        //reset old bonus
        m_passive_cur_as = -m_passive_cur_as;
        m_passive_cur_ms = 1.0 / m_passive_cur_ms;
        m_stats_bonus_flat->add("as", m_passive_cur_as);
        m_stats_bonus_percentage->mult("ms",m_passive_cur_ms);

        //save new bonus
        //use passive at max when not enhanced
        if(!enhanced){
            if(m_passive_cur_as == 0.0){
                add_attackspeed_cap_disable();
            }
            m_passive_cur_as = m_passive_raw_as;
            m_passive_cur_ms = 1.0 + m_passive_raw_ms;
        }
        //use passive at a certain percentage
        else {
            //save new bonus
            if(m_passive_cur_as != 0.0 && instance == 0){
                remove_attackspeed_cap_disable();
                m_passive_cur_as = 0.0;
                m_passive_cur_ms = 1.0;
            }
            else {
                m_passive_cur_as = m_passive_raw_as * instance / 100.0;
                m_passive_cur_ms = 1.0 * (m_passive_raw_ms * instance / 100.0);
            }
        }
        //set new bonus
        m_stats_bonus_flat->add("as", m_passive_cur_as);
        m_stats_bonus_percentage->mult("ms", m_passive_cur_ms);
        calc_current_stats();
    }

    void Attacker_Jinx::execute_spell_q(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Defender_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }
        if(m_q_lvl > 0) {
            //reset as modifiers from q stances
            if(m_q_stance){
                double temp_as = -m_advanced_aa_as_bonus;
                m_stats_bonus_flat->add("as", temp_as);
            }
            else{
                double temp_as = 0.0;
                if(m_aa_cur_as_stacks > 0){
                    temp_as -= m_aa_raw_as_stacks[m_q_lvl - 1];
                }
                if(m_aa_cur_as_stacks > 1){
                    temp_as -= m_aa_raw_as_stacks[m_q_lvl - 1] * 0.5 * (m_aa_cur_as_stacks - 1);
                }
                temp_as += m_advanced_aa_as_bonus;

                m_stats_bonus_flat->add("as", temp_as);
                m_aa_cur_as_stacks = 0;
            }
            calc_current_stats();
            m_q_stance ^= true;
        }
        else{
            std::cerr << "attacker_jinx_r is not leveled" << std::endl;
        }
    }

    void Attacker_Jinx::execute_spell_r(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Defender_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }
        if(get_spell_lvl_r() == 0){
            std::cerr << "attacker_jinx_r is not leveled" << std::endl;
            return;
        }
        if(!use_mana(m_jinx_spell_r->get_spell_cost())){
            std::cerr << "not enough mana to use attacker_jinx_r" << std::endl;
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
            std::cerr << "Pointer towards defender is invalid" << std::endl;
        }
    }

    Defender_Jinx::Defender_Jinx(engine_signal_system *ess, const std::string &name, const int &lvl) :
    Defender_Champion(ess, name, lvl) {
        m_connections.push_back(m_ess->attacker.get_slows.connect(
                std::bind(&Defender_Jinx::slot_spell_w_slow, this)));


        //---------------- read spell stats ----------------
        if(!read_array_float("/spell_w/slow", m_w_raw_slows, std::size(m_w_raw_slows))){
            m_setup_incomplete = true;
            std::cerr << "read spell_w slow failed" << std::endl;
        }
        if(!read_array_float("/spell_w/cost", m_w_raw_cost, m_w_max_lvl)){
            m_setup_incomplete = true;
            std::cerr << "read spell_w cost failed" << std::endl;
        }
        if(!read_single_float("/spell_e/cost", m_e_cost)){
            m_setup_incomplete = true;
            std::cerr << "read spell_e cost failed" << std::endl;
        }

        //---------------- set functions ----------------
        func_auto_attack = [&](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "defender_jinx_auto_attack has no own effect" << std::endl;
        };

        func_passive = [&](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "defender_jinx_passive has no own effect" << std::endl;
        };

        func_spell_q = [&](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "defender_jinx_q has no own effect" << std::endl;
        };

        func_spell_w = [&](const bool &crit, const bool &enhanced, const int &instance){
            execute_spell_w(crit, enhanced,instance);
        };

        func_spell_e = [&](const bool &crit, const bool &enhanced, const int &instance){
            execute_spell_e(crit, enhanced,instance);
        };

        func_spell_r = [&](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "defender_jinx_r has no own effect" << std::endl;
        };
    }

    bool Defender_Jinx::set_spell_lvl_q(const int &lvl) {
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

    bool Defender_Jinx::set_spell_lvl_w(const int &lvl) {
        if(!level_check(m_champ_lvl,
                        m_q_lvl,
                        lvl,
                        get_spell_lvl_e(),
                        get_spell_lvl_r())){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        m_w_lvl = lvl;
        return true;
    }

    bool Defender_Jinx::set_spell_lvl_e(const int &lvl) {
        if(!level_check(m_champ_lvl,
                        m_q_lvl,
                        get_spell_lvl_w(),
                        lvl,
                        get_spell_lvl_r())){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        m_e_lvl = lvl;
        return true;
    }

    bool Defender_Jinx::set_spell_lvl_r(const int &lvl) {
        if(!level_check(m_champ_lvl,
                        m_q_lvl,
                        get_spell_lvl_w(),
                        get_spell_lvl_e(),
                        lvl)){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        m_r_lvl = lvl;
        return true;
    }

    double Defender_Jinx::slot_spell_w_slow() {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return 0.0;
        }
        if(!m_Attacker_set){
            std::cerr << "no defender specified" << std::endl;
            return 0.0;
        }
        return m_w_cur_slow;
    }

    void Defender_Jinx::execute_spell_w(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Attacker_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }
        if(get_spell_lvl_w() == 0){
            std::cerr << "defender_jinx_w is not leveled" << std::endl;
            return;
        }
        if(!use_mana(m_w_raw_cost[m_w_lvl])){
            std::cerr << "not enough mana to use defender_jinx_w" << std::endl;
            return;
        }

        if(enhanced){
            m_w_cur_slow = 0.0;
        }
        else if(m_w_lvl > 0 && m_w_lvl <= m_w_max_lvl){
            m_w_cur_slow = m_w_raw_slows[m_w_lvl - 1];
        }
        else{
            std::cerr << "internal error: defender_jinx m_w_lvl is not in the legal range!" << std::endl;
            return;
        }
        m_ess->defender.apply_slow();
    }

    void Defender_Jinx::execute_spell_e(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Attacker_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }
        if(get_spell_lvl_e() == 0){
            std::cerr << "defender_jinx_e is not leveled" << std::endl;
            return;
        }
        if(!use_mana(m_e_cost)){
            std::cerr << "not enough mana to use defender_jinx_e" << std::endl;
            return;
        }
        m_ess->defender.apply_hard_cc();
    }
}
