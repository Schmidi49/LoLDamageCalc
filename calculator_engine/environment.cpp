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
            case undefined:
                return "undefined";
                break;
            case physical:
                return "physical";
                break;
            case magic:
                return "magic";
                break;
            case truedmg:
                return "truedmg";
                break;
            default:
                return "undefined";
                break;
        }
    }

    DamageType to_DamageType(const std::string& str){
        if(str == "physical")
            return physical;
        else if(str == "magic")
            return magic;
        else if(str == "truedmg")
            return truedmg;
        else
            return undefined;
    }

} // LDC