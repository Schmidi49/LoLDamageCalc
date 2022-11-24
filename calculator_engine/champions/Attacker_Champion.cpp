/******************************************************************************
 * @file Attacker_Champion.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#include <iostream>


#include "Attacker_Champion.h"


namespace LDC::champions{
    Attacker_Champion::Attacker_Champion(engine_signal_system* ess, const std::string &name, const int &lvl) :
        Base_Champion(ess, name, lvl){
        m_connections.push_back(m_ess->defender.apply_slow.connect(
                std::bind(&Attacker_Champion::slot_apply_slow, this)));

        m_connections.push_back(m_ess->attacker.auto_attack.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(m_setup_incomplete){
                std::cerr << "champion not set up correctly" << std::endl;
                return;
            }
            if(!m_Defender_set){
                std::cerr << "no defender specified" << std::endl;
                return;
            }
            func_auto_attack(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->attacker.passive.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(m_setup_incomplete){
                std::cerr << "champion not set up correctly" << std::endl;
                return;
            }
            if(!m_Defender_set){
                std::cerr << "no defender specified" << std::endl;
                return;
            }
            func_passive(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->attacker.spell_q.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(m_setup_incomplete){
                std::cerr << "champion not set up correctly" << std::endl;
                return;
            }
            if(!m_Defender_set){
                std::cerr << "no defender specified" << std::endl;
                return;
            }
            func_spell_q(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->attacker.spell_w.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(m_setup_incomplete){
                std::cerr << "champion not set up correctly" << std::endl;
                return;
            }
            if(!m_Defender_set){
                std::cerr << "no defender specified" << std::endl;
                return;
            }
            func_spell_w(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->attacker.spell_e.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(m_setup_incomplete){
                std::cerr << "champion not set up correctly" << std::endl;
                return;
            }
            if(!m_Defender_set){
                std::cerr << "no defender specified" << std::endl;
                return;
            }
            func_spell_e(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->attacker.spell_r.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(m_setup_incomplete){
                std::cerr << "champion not set up correctly" << std::endl;
                return;
            }
            if(!m_Defender_set){
                std::cerr << "no defender specified" << std::endl;
                return;
            }
            func_spell_r(crit, enhanced, instance);
        }));
    }

    Attacker_Champion::~Attacker_Champion() {
        m_Defender = nullptr;
    }

    void Attacker_Champion::set_Defender(Defender_Champion* defender) {
        if(defender) {
            m_Defender = defender;
            m_Defender_set = true;
        }
        else {
            m_Defender_set = false;
            std::cerr << "Pointer towards defender is invalid" << std::endl;
        }
    }

    void Attacker_Champion::slot_apply_slow() {
        m_cur_slow = 1.0 / m_cur_slow;
        m_stats_bonus_percentage->mult("ms", m_cur_slow);
        m_cur_slow = 1.0 - m_ess->attacker.get_slows();
        m_stats_bonus_percentage->mult("ms", m_cur_slow);
    }

    void Attacker_Champion::execute_effectless_spell(const std::string &spell_name) {
        if(m_setup_incomplete){
            std::cerr << "champion not set up correctly" << std::endl;
            return;
        }
        if(!m_Defender_set){
            std::cerr << "no defender specified" << std::endl;
            return;
        }

        std::cout << spell_name << " has no own effect" << std::endl;
    }
}

