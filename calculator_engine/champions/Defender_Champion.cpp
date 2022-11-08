/******************************************************************************
 * @file Defender_Champion.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#include <iostream>

#include "Defender_Champion.h"

namespace LDC::champions {

    Defender_Champion::Defender_Champion(engine_signal_system *ess, const std::string &name, const int &lvl) :
        Base_Champion(ess, name, lvl){
        m_connections.push_back(
                m_ess->attacker.deal_damage.connect(
                std::bind(&Defender_Champion::slot_take_damage, this, std::placeholders::_1)));

        m_connections.push_back(m_ess->defender.auto_attack.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(!m_Attacker_set){
                std::cerr << "no attacker specified" << std::endl;
                return;
            }
            func_auto_attack(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->defender.passive.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(!m_Attacker_set){
                std::cerr << "no attacker specified" << std::endl;
                return;
            }
            func_passive(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->defender.spell_q.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(!m_Attacker_set){
                std::cerr << "no attacker specified" << std::endl;
                return;
            }
            func_spell_q(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->defender.spell_w.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(!m_Attacker_set){
                std::cerr << "no attacker specified" << std::endl;
                return;
            }
            func_spell_w(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->defender.spell_e.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(!m_Attacker_set){
                std::cerr << "no attacker specified" << std::endl;
                return;
            }
            func_spell_e(crit, enhanced, instance);
        }));
        m_connections.push_back(m_ess->defender.spell_r.connect([&](const bool &crit, const bool &enhanced, const int &instance){
            if(!m_Attacker_set){
                std::cerr << "no attacker specified" << std::endl;
                return;
            }
            func_spell_r(crit, enhanced, instance);
        }));
    }

    Defender_Champion::~Defender_Champion() {
        m_Attacker = nullptr;
    }

    void Defender_Champion::set_Attacker(Attacker_Champion *attacker) {
        if(attacker) {
            m_Attacker = attacker;
            m_Attacker_set = true;
        }
        else {
            m_Attacker_set = false;
            throw std::invalid_argument("Pointer towards attacker is invalid");
        }

    }

    void Defender_Champion::slot_take_damage(const LDC::Damage& dmg) {
        std::cout << "Damage taken: " << dmg << std::endl;
        m_missing_health += dmg.dmg;
        if(m_missing_health > *m_current_stats->hp()){
            m_missing_health = *m_current_stats->hp();
            m_ess->defender.death();
        }
    }
}
