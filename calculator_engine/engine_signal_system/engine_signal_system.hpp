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
    class engine_signal_system{
    private:
        template <typename funcsig>
        using signal = boost::signals2::signal<funcsig>;

        struct champ_active_struct{
            signal<void(const bool &crit)> auto_attack;

            signal<void(const bool &crit, const bool &enhanced, const int &instance)> passive;

            signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_q;
            signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_w;
            signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_e;
            signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_r;
        };

        struct attacker_struct : public champ_active_struct{
            signal<void()> apply_onhit;
        };

        struct defender_struct : public champ_active_struct{

        };
    public:
        attacker_struct attacker;
        defender_struct defender;
    };
}

#endif //LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
