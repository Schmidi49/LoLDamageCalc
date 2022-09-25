/******************************************************************************
 * @file main.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#include <iostream>

#include <Attacker_Champion.h>
#include <engine_signal_system.hpp>

int main(int argc, char* argv[]){
    std::cout << "Hello Test" << std::endl;
    LDC::engine_signal_system ess;
    LDC::champion::Attacker_Champion a(&ess, "GenericChamp");
    std::cout << "Champion created" << std::endl;
    std::cout << "Champion HP at level 2: "<< a.test(2) << std::endl;

    std::cout << "Doing spells/attacks: " << std::endl;
    ess.attacker.auto_attack(true);
    ess.attacker.auto_attack(false);
    ess.attacker.spell_q(true, false, 0);
    ess.attacker.spell_w(false, true, 0);
    ess.attacker.spell_w(false, true, 76);
    ess.attacker.spell_w(false, false, 0);
}