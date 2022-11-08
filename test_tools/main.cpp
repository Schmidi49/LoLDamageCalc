/******************************************************************************
 * @file main.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#include <iostream>

#include <Jinx.h>
#include <engine_signal_system.hpp>

int main(int argc, char* argv[]){
    std::cout << "Hello Test" << std::endl;
    LDC::engine_signal_system ess;

    LDC::champions::Attacker_Jinx jinx(&ess, "Jinx");
    std::cout << "Jinx created" << std::endl;
    std::cout << "set champ lvl: " << jinx.set_lvl(5) << std::endl;
    std::cout << "set w lvl: "<< std::endl;
    jinx.set_spell_lvl_w(3);
    std::cout << "set e lvl: "<< std::endl;
    jinx.set_spell_lvl_e(0);

    LDC::champions::Defender_Champion d(&ess, "GenericChamp");
    std::cout << "Defender created" << std::endl;

    jinx.set_Defender(&d);
    d.set_Attacker(&jinx);

    std::cout << "Doing spells/attacks: " << std::endl;
    ess.attacker.auto_attack(true, false, 0);
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.spell_q(true, false, 0);
    ess.attacker.spell_w(false, true, 0);
    ess.attacker.spell_e(false, true, 76);
    ess.attacker.spell_r(false, false, 0);
}