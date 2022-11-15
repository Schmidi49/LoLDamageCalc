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
            if (!m_champion_data.contains("passive")) {
                m_setup_incomplete = true;
                std::cerr << "no \"passive\" specified" << std::endl;
            }
            else if (!m_champion_data["passive"].contains("as") || !m_champion_data["passive"].contains("ms")) {
                m_setup_incomplete = true;
                std::cerr << "\"passive\" has missing information" << std::endl;
            } else {
                if(m_champion_data["passive"]["as"].is_number()){
                    m_passive_raw_as = m_champion_data["passive"]["as"];
                }
                else{
                    m_setup_incomplete = true;
                    std::cerr << "passive as is not a number" << std::endl;
                }
                if(m_champion_data["passive"]["ms"].is_number()){
                    m_passive_raw_ms = m_champion_data["passive"]["ms"];
                }
                else{
                    m_setup_incomplete = true;
                    std::cerr << "passive ms is not a number" << std::endl;
                }
            }

            //---------------- read spell q ----------------
            if (!m_champion_data.contains("spell_q")) {
                m_setup_incomplete = true;
                std::cerr << "no \"spell_q\" specified" << std::endl;
            } else {
                if(m_champion_data["spell_q"].contains("max_lvl")){
                    if(m_champion_data["spell_q"]["max_lvl"].is_number_integer()) {
                        if (m_q_max_level != m_champion_data["spell_q"]["max_lvl"]) {
                            m_setup_incomplete = true;
                            std::cerr << "spell_q can not be a other level than 5" << std::endl;
                        }
                    }
                    else {
                        m_setup_incomplete = true;
                        std::cerr << "spell_q max_lvl is not a integer" << std::endl;
                    }
                }
                else {
                    m_setup_incomplete = true;
                    std::cerr << "spell_q max_lvl not specified" << std::endl;
                }

                if(m_champion_data["spell_q"].contains("cost")){
                    if(m_champion_data["spell_q"]["cost"].is_number())
                        m_advanced_aa_cost = m_champion_data["spell_q"]["cost"];
                    else {
                        m_setup_incomplete = true;
                        std::cerr << "spell_q cost is not a number" << std::endl;
                    }
                }
                else {
                    m_setup_incomplete = true;
                    std::cerr << "spell_q cost not specified" << std::endl;
                }

                if(m_champion_data["spell_q"].contains("cd")){
                    if(m_champion_data["spell_q"]["cd"].is_number())
                        m_q_cd = m_champion_data["spell_q"]["cd"];
                    else {
                        m_setup_incomplete = true;
                        std::cerr << "spell_q cd is not a number" << std::endl;
                    }
                }
                else {
                    m_setup_incomplete = true;
                    std::cerr << "spell_q cd not specified" << std::endl;
                }

                if(m_champion_data["spell_q"].contains("ad_modify")){
                    if(m_champion_data["spell_q"]["ad_modify"].is_number())
                        m_advanced_aa_ad_modify = m_champion_data["spell_q"]["ad_modify"];
                    else {
                        m_setup_incomplete = true;
                        std::cerr << "advanced_aa ad_modify is not a number" << std::endl;
                    }
                }
                else {
                    m_setup_incomplete = true;
                    std::cerr << "advanced_aa ad_modify not specified" << std::endl;
                }

                if(m_champion_data["spell_q"].contains("as_modify")){
                    if(m_champion_data["spell_q"]["as_modify"].is_number())
                        m_advanced_aa_as_bonus = m_champion_data["spell_q"]["as_modify"];
                    else {
                        m_setup_incomplete = true;
                        std::cerr << "advanced_aa as_modify is not a number" << std::endl;
                    }
                }
                else {
                    m_setup_incomplete = true;
                    std::cerr << "advanced_aa as_modify not specified" << std::endl;
                }


                if(!m_champion_data["spell_q"].contains("bonus_as_stacks")) {
                    m_setup_incomplete = true;
                    std::cerr << "spell_q bonus_as_stacks not specified" << std::endl;
                }
                else if(m_champion_data["spell_q"]["bonus_as_stacks"].is_array()){
                    if(m_champion_data["spell_q"]["bonus_as_stacks"].size() == m_q_max_level) {
                        for (int i = 0; i < m_q_max_level; i++) {
                            if (m_champion_data["spell_q"]["bonus_as_stacks"][i].is_number()) {
                                m_aa_raw_as_stacks[i] = m_champion_data["spell_q"]["bonus_as_stacks"][i];
                            } else {
                                m_setup_incomplete = true;
                                std::cerr << "entry " << i << " of spell_q bonus_as_stacks is not an integer" << std::endl;
                            }
                        }
                    }
                    else {
                        m_setup_incomplete = true;
                        std::cerr << "spell_q bonus_as_stacks has the wrong size" << std::endl;
                    }
                }
                else{
                    m_setup_incomplete = true;
                    std::cerr << "spell_q bonus_as_stacks is not an array" << std::endl;
                }
            }

            //---------------- read spell w ----------------
            if (!m_champion_data.contains("spell_w")) {
                m_setup_incomplete = true;
                std::cerr << "no \"spell_w\" specified" << std::endl;
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

                if(!m_champion_data["spell_r"].contains("range")){
                    m_setup_incomplete = true;
                    std::cerr << "did not find a range for jinx r, aborting" << std::endl;
                }
                else if(!m_champion_data["spell_r"]["range"].is_array()){
                    m_setup_incomplete = true;
                    std::cerr << "range is not a filed" << std::endl;
                }
                else if(m_champion_data["spell_r"]["range"].size() != 2){
                    m_setup_incomplete = true;
                    std::cerr << "field range has the wrong size" << std::endl;
                }
                else if(!m_champion_data["spell_r"]["range"][0].is_number() ||
                        !m_champion_data["spell_r"]["range"][1].is_number()){
                    m_setup_incomplete = true;
                    std::cerr << " range has the wrong type" << std::endl;
                }
                else{
                    m_r_range_min = m_champion_data["spell_r"]["range"][0];
                    m_r_range_max = m_champion_data["spell_r"]["range"][1];
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


        //---------------- read w slow ----------------
        if(!m_champion_data["spell_w"].contains("slow")) {
            m_setup_incomplete = true;
            std::cerr << "spell_q slow not specified" << std::endl;
        }
        else if(m_champion_data["spell_w"]["slow"].is_array()){
            if(m_champion_data["spell_w"]["slow"].size() == m_w_max_lvl) {
                for (int i = 0; i < m_w_max_lvl; i++) {
                    if (m_champion_data["spell_w"]["slow"][i].is_number()) {
                        m_spell_w_raw_slows[i] = m_champion_data["spell_w"]["slow"][i];
                    } else {
                        m_setup_incomplete = true;
                        std::cerr << "entry " << i << " of spell_w slow is not a number" << std::endl;
                    }
                }
            }
            else {
                m_setup_incomplete = true;
                std::cerr << "spell_w slow has the wrong size" << std::endl;
            }
        }
        else{
            m_setup_incomplete = true;
            std::cerr << "spell_w slow is not an array" << std::endl;
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
            std::cout << "defender_jinx_q has no own effect" << std::endl;
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
        return m_spell_w_cur_slow;
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
        if(enhanced){
            m_spell_w_cur_slow = 0.0;
        }
        else if(m_w_lvl > 0 && m_w_lvl <= m_w_max_lvl){
            m_spell_w_cur_slow = m_spell_w_raw_slows[m_w_lvl - 1];
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
        m_ess->defender.apply_hard_cc();
    }
}
