/******************************************************************************
 * @file environment.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (25.09.2022)
 */

#include "environment.h"

namespace LDC {
    std::string to_string(DamageType dt){
        switch (dt){
            case physical:
                return "physical";
                break;
            case magic:
                return "magic";
                break;
            case trueDmg:
                return "trueDmg";
                break;
        }
    }

    DamageType to_DamageType(const std::string& str){
        if(str == "physical")
            return physical;
        else if(str == "magic")
            return magic;
        else if(str == "trueDmg")
            return trueDmg;
        else
            throw std::invalid_argument("invalid Damage Type");
    }

    std::string to_string(const Damage& dmg) {
        return "physical:" + std::to_string(dmg.physical) +
               ",magic:" + std::to_string(dmg.magic) +
               ",true:" + std::to_string(dmg.trueDmg);
    }
} // LDC