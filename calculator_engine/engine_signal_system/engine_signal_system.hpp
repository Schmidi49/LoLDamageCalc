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

    public:
        signal<void(const bool &crit)> actio_auto_attack;

        signal<void(const bool &crit, const bool &enhancement, const int &index)> actio_passive;

        signal<void(const bool &crit, const bool &enhancement, const int &index)> actio_spell_q;
        signal<void(const bool &crit, const bool &enhancement, const int &index)> actio_spell_w;
        signal<void(const bool &crit, const bool &enhancement, const int &index)> actio_spell_e;
        signal<void(const bool &crit, const bool &enhancement, const int &index)> actio_spell_r;
    };
}

#endif //LOLDAMAGECALC_ENGINE_SIGNAL_SYSTEM_H
