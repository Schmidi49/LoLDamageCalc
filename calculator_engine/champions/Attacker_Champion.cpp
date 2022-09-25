/******************************************************************************
 * @file Attacker_Champion.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#include <iostream>


#include "Attacker_Champion.h"


namespace LDC::champion{
    Attacker_Champion::Attacker_Champion(engine_signal_system* ess, const std::string &name, const int &lvl) :
        Base_Champion(ess, name, lvl){
        m_ess->attacker.auto_attack.connect(std::bind(&Attacker_Champion::execute_auto_attack, this, std::placeholders::_1));
        m_ess->attacker.passive.connect(std::bind(&Attacker_Champion::execute_passive, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->attacker.spell_q.connect(std::bind(&Attacker_Champion::execute_spell_q, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->attacker.spell_w.connect(std::bind(&Attacker_Champion::execute_spell_w, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->attacker.spell_e.connect(std::bind(&Attacker_Champion::execute_spell_e, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_ess->attacker.spell_r.connect(std::bind(&Attacker_Champion::execute_spell_r, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    Attacker_Champion::~Attacker_Champion() {
        m_Defender = nullptr;
    }

    void Attacker_Champion::set_Defender(Defender_Champion* defender) {
        if(defender)
            m_Defender = defender;
        else
            throw std::invalid_argument("Pointer towards defender is invalid");
    }

    void Attacker_Champion::execute_auto_attack(const bool & crit) {
        std::cout << "Auto attack: " << (crit ? "did crit" : "did not crit") << std::endl;
    }

    void Attacker_Champion::execute_passive(const bool &crit, const bool &enhanced, const int &instance) {
        std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
        std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
        std::cout << "Instance: " << instance << std::endl;
    }

    void Attacker_Champion::execute_spell_q(const bool &crit, const bool &enhanced, const int &instance) {
        std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
        std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
        std::cout << "Instance: " << instance << std::endl;
    }

    void Attacker_Champion::execute_spell_w(const bool &crit, const bool &enhanced, const int &instance) {
        std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
        std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
        std::cout << "Instance: " << instance << std::endl;
    }

    void Attacker_Champion::execute_spell_e(const bool &crit, const bool &enhanced, const int &instance) {
        std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
        std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
        std::cout << "Instance: " << instance << std::endl;
    }

    void Attacker_Champion::execute_spell_r(const bool &crit, const bool &enhanced, const int &instance) {
        std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
        std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
        std::cout << "Instance: " << instance << std::endl;
    }


}

