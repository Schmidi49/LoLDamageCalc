/******************************************************************************
 * @file Damage.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (05.11.2022)
 */

#ifndef LOLDAMAGECALC_DAMAGE_H
#define LOLDAMAGECALC_DAMAGE_H

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

    enum DamageTag{
        defaultDmg,
        raw,
        proc,
        reactive,
        basic,
        spell,
        area,
        persitent,
        persitentArea,
        pet
    };

    std::string to_string(const DamageTag& dt);

    DamageTag to_DamageTag(const std::string& str);

    class Damage{
    public:
        Damage() = default;
        Damage(double d){dmg = d;};

        Damage & operator+=(double const & dmg);
        Damage & operator+=(double && dmg);
        Damage operator+(double const & dmg);
        Damage operator+(double && dmg);

        Damage & operator*=(double const & mult);
        Damage & operator*=(double && mult);
        Damage operator*(double const & mult);
        Damage operator*(double && mult);

        double dmg{0.0};
        DamageType type;
        DamageTag tag;
        bool crit{false};
        bool cc{false};
        bool projectile{false};
        bool spellshieldAffected{false};
    };

    std::string to_string(const Damage& dt);

    class DamageDealt{
    public:
        DamageDealt() = default;
        DamageDealt(const double& p, const double& m, const double& t);

        double get(const DamageType& dt) const;

        void set(const DamageType& dt, const double& dmg);

        DamageDealt & operator+=(DamageDealt const & dmg);
        DamageDealt & operator+=(DamageDealt && dmg);
        DamageDealt operator+(DamageDealt const & dmg);
        DamageDealt operator+(DamageDealt && dmg);
    public:
        double physical{0.0};
        double magic{0.0};
        double trueDmg{0.0};
    };

    std::string to_string(const DamageDealt &dmg);
};

inline std::ostream &  operator<<(std::ostream &os, const LDC::DamageType& dt)
{
    os << LDC::to_string(dt);
    return os;
}

inline std::ostream &  operator<<(std::ostream &os, const LDC::DamageTag& dt)
{
    os << LDC::to_string(dt);
    return os;
}

inline std::ostream &  operator<<(std::ostream &os, const LDC::Damage& dmg)
{
    os << LDC::to_string(dmg);
    return os;
}

inline std::ostream &  operator<<(std::ostream &os, const LDC::DamageDealt& dd)
{
    os << LDC::to_string(dd);
    return os;
}
#endif //LOLDAMAGECALC_DAMAGE_H
