/******************************************************************************
 * @file Defender_Champion.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#ifndef LOLDAMAGECALC_DEFENDER_CHAMPION_H
#define LOLDAMAGECALC_DEFENDER_CHAMPION_H

namespace LDC::champion {
    class Defender_Champion {
        void execute_auto_attack(const bool &crit);
        void execute_passive(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_q(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_w(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_e(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_r(const bool &crit, const bool &enhanced, const int &instance);
    };
}


#endif //LOLDAMAGECALC_DEFENDER_CHAMPION_H
