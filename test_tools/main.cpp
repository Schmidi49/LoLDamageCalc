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
    std::cout << "set champ lvl: " << std::endl;
    jinx.set_lvl(7);
    std::cout << "set q lvl: "<< std::endl;
    jinx.set_spell_lvl_q(4);
    std::cout << "set w lvl: "<< std::endl;
    jinx.set_spell_lvl_w(1);
    std::cout << "set e lvl: "<< std::endl;
    jinx.set_spell_lvl_e(1);
    std::cout << "set r lvl: "<< std::endl;
    jinx.set_spell_lvl_r(1);

    LDC::champions::Defender_Champion d(&ess, "GenericChamp");
    std::cout << "Defender created" << std::endl;

    jinx.set_Defender(&d);
    d.set_Attacker(&jinx);

    //test routine for jinx
    ess.attacker.auto_attack.connect([&](const bool &crit, const bool &enhanced, const int &instance){
        std::cout << "as: " << *jinx.get_ptr_current_stats()->as() << std::endl;
    });
    ess.attacker.spell_q.connect([&](const bool &crit, const bool &enhanced, const int &instance){
        std::cout << "as: " << *jinx.get_ptr_current_stats()->as() << std::endl;
    });
    ess.attacker.passive.connect([&](const bool &crit, const bool &enhanced, const int &instance){
        std::cout << "as: " << *jinx.get_ptr_current_stats()->as() << std::endl;
    });

    std::cout << "initial as: " << *jinx.get_ptr_current_stats()->as() << std::endl;
    std::cout << "Doing attacks: " << std::endl;
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.auto_attack(true, false, 0);
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.spell_q(false, false, 0);
    ess.attacker.auto_attack(true, false, 0);
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.spell_q(false, false, 0);
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.passive(false, false, 0);
    ess.attacker.auto_attack(false, false, 0);
    ess.attacker.spell_q(false, false, 0);
    ess.attacker.auto_attack(true, false, 0);
    ess.attacker.passive(false, true, 0);

    std::cout << "Doing spells: " << std::endl;
    ess.attacker.spell_w(false, false, 0);
    ess.attacker.spell_e(false, false, 0);
    ess.attacker.spell_r(false, false, 0);
    ess.attacker.spell_r(false, false, 50);
    ess.attacker.spell_r(false, false, 100);

}