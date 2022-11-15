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
        template<typename T>
        struct add_dmg_reduction{
            typedef T result_type;

            template<typename InputIterator>
            T operator()(InputIterator first, InputIterator last) const
            {
                if(first == last ) return T();
                T sum = *first++;
                while (first != last) {
                    sum += *first++;
                }

                return sum;
            }
        };

        template<typename T>
        struct mult_dmg_mod{
            typedef T result_type;

            template<typename InputIterator>
            T operator()(InputIterator first, InputIterator last) const
            {
                if(first == last ) return T{1};
                T sum = *first++;
                while (first != last) {
                    sum *= *first++;
                }

                return sum;
            }
        };

        // combiner which returns the maximum value returned by all slots
        template<typename T>
        struct maximum_slow
        {
            typedef T result_type;

            template<typename InputIterator>
            T operator()(InputIterator first, InputIterator last) const
            {
                // If there are no slots to call, just return the
                // default-constructed value
                if(first == last ) return T();
                T max_value = T();
                while (first != last) {
                    if (max_value < *first && *first >= 0 && *first < 1)
                        max_value = *first;
                    ++first;
                }

                return max_value;
            }
        };

        struct champ_active_struct{
            boost::signals2::signal<void(const bool &crit, const bool &enhanced, const int &instance)> auto_attack;

            boost::signals2::signal<void(const bool &crit, const bool &enhanced, const int &instance)> passive;

            boost::signals2::signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_q;
            boost::signals2::signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_w;
            boost::signals2::signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_e;
            boost::signals2::signal<void(const bool &crit, const bool &enhanced, const int &instance)> spell_r;
        };

        struct attacker_struct : public champ_active_struct{
            boost::signals2::signal<double (), maximum_slow<double>> get_slows;
            boost::signals2::signal<void()> execute_onhit;
            boost::signals2::signal<void(const LDC::Damage& dmg)> deal_damage;
        };

        struct defender_struct : public champ_active_struct{
            boost::signals2::signal<double (LDC::Damage& dmg), add_dmg_reduction<double>> get_premit_dmg_red;
            boost::signals2::signal<double (LDC::Damage& dmg), add_dmg_reduction<double>> get_postmit_dmg_red;
            boost::signals2::signal<double (LDC::Damage& dmg), mult_dmg_mod<double>> get_dmg_mod;
            boost::signals2::signal<void ()> apply_hard_cc;
            boost::signals2::signal<void ()> apply_slow;
            boost::signals2::signal<void()> death;
        };
    public:
        attacker_struct attacker;
        defender_struct defender;
    };



}


#endif //LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
