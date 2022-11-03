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


namespace LDC::champions {
    class Attacker_Champion : public Base_Champion  {
    public:
        Attacker_Champion(engine_signal_system* ess, const std::string &name, const int &lvl = 1);
        ~Attacker_Champion();

        //TODO set flag for set pointer
        void set_Defender(Defender_Champion* defender);

    private:
        virtual void execute_auto_attack(const bool &crit);
        virtual void execute_passive(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_q(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_w(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_e(const bool &crit, const bool &enhanced, const int &instance);
        virtual void execute_spell_r(const bool &crit, const bool &enhanced, const int &instance);

    protected:
        std::function<void(const bool &crit)> func_auto_attack;
        std::function<void(const bool &crit, const bool &enhanced, const int &instance)> func_passive;
        std::function<void(const bool &crit, const bool &enhanced, const int &instance)> func_spell_q;
        std::function<void(const bool &crit, const bool &enhanced, const int &instance)> func_spell_w;
        std::function<void(const bool &crit, const bool &enhanced, const int &instance)> func_spell_e;
        std::function<void(const bool &crit, const bool &enhanced, const int &instance)> func_spell_r;



    protected:
        Defender_Champion* m_Defender{nullptr};
        volatile bool m_Defender_set{false};
    };
}


#endif //LOLDAMAGECALC_ATTACKER_CHAMPION_H
