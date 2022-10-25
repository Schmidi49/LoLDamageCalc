/******************************************************************************
 * @file environment.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (25.09.2022)
 */

#include <boost/optional.hpp>
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

    Damage::Damage(const double &p, const double &m, const double &t) {
        if(p >= 0.0 && m >= 0.0 && t >= 0.0) {
            physical = p;
            magic = m;
            trueDmg = t;
        }
        else
            throw std::invalid_argument("any damage can not be negative");
    }

    double Damage::get(const DamageType &dt) const {
        switch (dt) {
            case DamageType::physical:
                return physical;
            case DamageType::magic:
                return magic;
            case DamageType::trueDmg:
                return trueDmg;
        }
    }

    void Damage::set(const DamageType &dt, const double &dmg) {
        if(dmg >= 0.0) {
            switch (dt) {
                case DamageType::physical:
                    physical = dmg;
                    break;
                case DamageType::magic:
                    magic = dmg;
                    break;
                case DamageType::trueDmg:
                    trueDmg = dmg;
                    break;
            }
        }
        else
            throw std::invalid_argument("any damage can not be negative");
   }

    Damage &Damage::operator+=(Damage const &dmg) {
        physical += dmg.physical;
        magic += dmg.magic;
        trueDmg += dmg.trueDmg;
        return *this;
    }

    Damage &Damage::operator+=(Damage &&dmg) {
        physical += dmg.physical;
        magic += dmg.magic;
        trueDmg += dmg.trueDmg;
        return *this;
    }

    Damage Damage::operator+(const Damage &dmg) {
        Damage result(*this);
        result += dmg;
        return result;
    }

    Damage Damage::operator+(Damage &&dmg) {
        Damage result(*this);
        result += std::move(dmg);
        return result;
    }
} // LDC