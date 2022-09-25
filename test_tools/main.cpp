/******************************************************************************
 * @file main.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#include <iostream>

#include <Base_Champion.h>
#include <engine_signal_system.hpp>

void exec_auto_attack(bool crit){std::cout << "auto did " << (crit ? "crit!" : "not crit.") << std::endl;}

int main(int argc, char* argv[]){
    std::cout << "Hello Test" << std::endl;
    LDC::champion::Base_Champion a("GenericChamp", 15);
    std::cout << "Champion created" << std::endl;
    std::cout << "Champion HP at level 2: "<< a.test() << std::endl;

    LDC::engine_signal_system ess;
    ess.actio_auto_attack.connect(exec_auto_attack);

    std::cout << "Doing auto attacks: " << std::endl;
    ess.actio_auto_attack(true);
    ess.actio_auto_attack(true);
    ess.actio_auto_attack(false);
    ess.actio_auto_attack(true);
}