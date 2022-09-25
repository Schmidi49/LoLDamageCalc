/******************************************************************************
 * @file engine_signal_system.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (25.09.2022)
 */

#ifndef LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
#define LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H

#include <boost/signals2.hpp>

namespace LDC{
    template <typename funcsig>
    using signal = boost::signals2::signal<funcsig>;

    struct champ_actives{
        signal<void(const bool &crit)> auto_attack;

        signal<void(const bool &crit, const bool &enhanced, const int &instance)> passive;

        signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_q;
        signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_w;
        signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_e;
        signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_r;
    };

    class engine_signal_system{
    public:
        champ_actives attacker;
        champ_actives defender;
    };
}

#endif //LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
