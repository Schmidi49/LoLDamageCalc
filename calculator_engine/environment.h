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
        undefined,
        physical,
        magic,
        truedmg
    };

    std::string to_string(DamageType dt);

    DamageType to_DamageType(const std::string& str);


    class damage{
    public:
        int physical;
        int magic;
        int trueDmg;
    };

    class environment {

    };

} // LDC

inline std::ostream &  operator<<(std::ostream &os, const LDC::DamageType& dt)
{
    os << LDC::to_string(dt);
    return os;
}

#endif //LOLDAMAGECALC_ENVIRONMENT_H
