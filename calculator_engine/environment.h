/******************************************************************************
 * @file environment.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (25.09.2022)
 */

#ifndef LOLDAMAGECALC_ENVIRONMENT_H
#define LOLDAMAGECALC_ENVIRONMENT_H

#include <cstring>
#include <iostream>

namespace LDC {
    enum DamageType{
        physical,
        magic,
        trueDmg
    };

    std::string to_string(const DamageType& dt);

    DamageType to_DamageType(const std::string& str);

    struct Damage{
        double physical{0.0};
        double magic{0.0};
        double trueDmg{0.0};
    };

    std::string to_string(const Damage &dmg);

    struct DamageAtributes{
        bool basicAttack{false};
    };

    class Environment {

    };

} // LDC

inline std::ostream &  operator<<(std::ostream &os, const LDC::DamageType& dt)
{
    os << LDC::to_string(dt);
    return os;
}

inline std::ostream &  operator<<(std::ostream &os, const LDC::Damage& dmg)
{
    os << LDC::to_string(dmg);
    return os;
}

#endif //LOLDAMAGECALC_ENVIRONMENT_H
