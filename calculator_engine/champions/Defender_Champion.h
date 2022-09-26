/******************************************************************************
 * @file Defender_Champion.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#ifndef LOLDAMAGECALC_DEFENDER_CHAMPION_H
#define LOLDAMAGECALC_DEFENDER_CHAMPION_H

#include <Base_Champion.h>
#include <Attacker_Champion.h>

namespace LDC::champion {
    class Defender_Champion : public Base_Champion{
    public:
        Defender_Champion(engine_signal_system* ess, const std::string &name, const int &lvl = 1);
        ~Defender_Champion();

        void set_Attacker(Base_Champion* attacker);

    protected:
        virtual void execute_auto_attack(const bool &crit);
        virtual void execute_passive(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_q(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_w(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_e(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_r(const bool &crit, const bool &enhanced, const int &instance);

    protected:
        Base_Champion* m_Attacker{nullptr};
    };
}


#endif //LOLDAMAGECALC_DEFENDER_CHAMPION_H
