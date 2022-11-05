/******************************************************************************
 * @file engine_signal_system.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (25.09.2022)
 */

#ifndef LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
#define LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H

#include <boost/signals2.hpp>
#include <damage.h>

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
            signal<void()> execute_onhit;
            signal<void(const LDC::Damage& dmg)> deal_damage;
        };

        struct defender_struct : public champ_active_struct{
            signal<void()> death;
        };
    public:
        attacker_struct attacker;
        defender_struct defender;
    };

    /*
    template<typename T>
    struct add_onhits{
        typedef T result_type;

        template<typename InputIterator>
        T operator()(InputIterator first, InputIterator last) const
        {
            if(first == last ) return T();
            T sum = *first++;
            while (first != last) {
                sum += first++;
            }

            return sum;
        }
    };*/
}


#endif //LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
