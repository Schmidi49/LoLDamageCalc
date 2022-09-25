/******************************************************************************
 * @file Attacker_Champion.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#ifndef LOLDAMAGECALC_ATTACKER_CHAMPION_H
#define LOLDAMAGECALC_ATTACKER_CHAMPION_H

#include <Base_Champion.h>
#include <Defender_Champion.h>


namespace LDC::champion {
    class Attacker_Champion : public Base_Champion  {
    public:
        Attacker_Champion(engine_signal_system* ess, const std::string &name, const int &lvl = 1);
        ~Attacker_Champion();

        void set_Defender(Defender_Champion* defender);

    protected:
        virtual void execute_auto_attack(const bool &crit);
        virtual void execute_passive(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_q(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_w(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_e(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_r(const bool &crit, const bool &enhanced, const int &instance);

    protected:
        Defender_Champion* m_Defender{nullptr};
    };
}


#endif //LOLDAMAGECALC_ATTACKER_CHAMPION_H
