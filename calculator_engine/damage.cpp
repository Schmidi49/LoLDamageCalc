/******************************************************************************
 * @file Damage.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (05.11.2022)
 */

#include "damage.h"

#include <boost/optional.hpp>

namespace LDC {
    std::string to_string(const DamageType& dt){
        switch (dt){
            case physical:
                return "physical";
            case magic:
                return "magic";
            case trueDmg:
                return "trueDmg";
        }
        return "undefined";
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

    std::string to_string(DamageTag dt){
        switch (dt){
            case defaultDmg:
                return "defaultDmg";
            case raw:
                return "raw";
            case proc:
                return "proc";
            case reactive:
                return "reactive";
            case basic:
                return "basic";
            case spell:
                return "spell";
            case area:
                return "area";
            case persitent:
                return "persitent";
            case persitentArea:
                return "persitentArea";
            case pet:
                return "pet";
        }
        return "undefined";
    }

    DamageTag to_DamageTag(const std::string& str){
        if(str == "defaultDmg")
            return defaultDmg;
        else if(str == "raw")
            return raw;
        else if(str == "proc")
            return proc;
        else if(str == "reactive")
            return reactive;
        else if(str == "basic")
            return basic;
        else if(str == "spell")
            return spell;
        else if(str == "area")
            return area;
        else if(str == "persitent")
            return persitent;
        else if(str == "persitentArea")
            return persitentArea;
        else if(str == "pet")
            return pet;
        else
            throw std::invalid_argument("invalid Damage Tag");
    }

    std::string to_string(const Damage& dmg) {
        return to_string(dmg.type) + ": " + std::to_string(dmg.dmg);
    }

    std::string to_string(const DamageDealt& dmg) {
        return "physical:" + std::to_string(dmg.physical) +
               ",magic:" + std::to_string(dmg.magic) +
               ",true:" + std::to_string(dmg.trueDmg);
    }

    DamageDealt::DamageDealt(const double &p, const double &m, const double &t) {
        if(p >= 0.0 && m >= 0.0 && t >= 0.0) {
            physical = p;
            magic = m;
            trueDmg = t;
        }
        else
            throw std::invalid_argument("any damage can not be negative");
    }

    double DamageDealt::get(const DamageType &dt) const {
        switch (dt) {
            case DamageType::physical:
                return physical;
            case DamageType::magic:
                return magic;
            case DamageType::trueDmg:
                return trueDmg;
        }
        throw std::runtime_error("undefined DamageType recieved");
    }

    void DamageDealt::set(const DamageType &dt, const double &dmg) {
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

    DamageDealt &DamageDealt::operator+=(DamageDealt const &dmg) {
        physical += dmg.physical;
        magic += dmg.magic;
        trueDmg += dmg.trueDmg;
        return *this;
    }
    DamageDealt &DamageDealt::operator+=(DamageDealt &&dmg) {
        physical += dmg.physical;
        magic += dmg.magic;
        trueDmg += dmg.trueDmg;
        return *this;
    }
    DamageDealt DamageDealt::operator+(const DamageDealt &dmg) {
        DamageDealt result(*this);
        result += dmg;
        return result;
    }
    DamageDealt DamageDealt::operator+(DamageDealt &&dmg) {
        DamageDealt result(*this);
        result += std::move(dmg);
        return result;
    }


} // LDC