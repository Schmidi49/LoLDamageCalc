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
        m_ess->attacker.deal_damage.connect(
                std::bind(&Defender_Champion::slot_take_damage, this, std::placeholders::_1, std::placeholders::_2));

        func_auto_attack = [](const bool &crit){
            std::cout << "Auto attack: " << (crit ? "did crit" : "did not crit") << std::endl;
        };
        func_passive = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_q = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "Q: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_w = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "W: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_e = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "E: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_r = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "R: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };

        m_ess->defender.auto_attack.connect(std::bind(&Defender_Champion::execute_auto_attack, this, std::placeholders::_1));
        m_ess->defender.passive.connect(std::bind(&Defender_Champion::execute_passive, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->defender.spell_q.connect(std::bind(&Defender_Champion::execute_spell_q, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->defender.spell_w.connect(std::bind(&Defender_Champion::execute_spell_w, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->defender.spell_e.connect(std::bind(&Defender_Champion::execute_spell_e, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->defender.spell_r.connect(std::bind(&Defender_Champion::execute_spell_r, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    Defender_Champion::~Defender_Champion() {
        m_Attacker = nullptr;
    }

    void Defender_Champion::set_Attacker(Attacker_Champion *attacker) {
        if(attacker)
            m_Attacker = attacker;
        else
            throw std::invalid_argument("Pointer towards attacker is invalid");
    }

    void Defender_Champion::execute_auto_attack(const bool & crit) {
        func_auto_attack(crit);
    }

    void Defender_Champion::execute_passive(const bool &crit, const bool &enhanced, const int &instance) {
        func_passive(crit, enhanced, instance);
    }

    void Defender_Champion::execute_spell_q(const bool &crit, const bool &enhanced, const int &instance) {
        func_spell_q(crit, enhanced, instance);
    }

    void Defender_Champion::execute_spell_w(const bool &crit, const bool &enhanced, const int &instance) {
        func_spell_w(crit, enhanced, instance);
    }

    void Defender_Champion::execute_spell_e(const bool &crit, const bool &enhanced, const int &instance) {
        func_spell_e(crit, enhanced, instance);
    }

    void Defender_Champion::execute_spell_r(const bool &crit, const bool &enhanced, const int &instance) {
        func_spell_r(crit, enhanced, instance);
    }

    void Defender_Champion::slot_take_damage(const LDC::Damage& dmg, const LDC::DamageAtributes& atrb) {
        std::cout << "Damage taken: " << dmg << std::endl;
        //do stuff ex: dmg reduction
        m_missing_health += dmg.physical + dmg.magic + dmg.trueDmg;
        if(m_missing_health > *m_current_stats->hp()){
            m_missing_health = *m_current_stats->hp();
            m_ess->defender.death();
        }
    }
}
