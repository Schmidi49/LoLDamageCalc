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

namespace LDC::champions {
    class Defender_Champion : public Base_Champion{
    public:
        Defender_Champion(engine_signal_system* ess, const std::string &name, const int &lvl = 1);
        ~Defender_Champion();

        //TODO set flag for set pointer
        void set_Attacker(Attacker_Champion* attacker);

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

    public:
        double get_max_health(){return *m_current_stats->hp();};
        double get_mis_health(){return m_missing_health;};
        double get_cur_health(){return (*m_current_stats->hp() - m_missing_health);};

    protected:
        virtual void slot_take_damage(const LDC::Damage& dmg, const LDC::DamageAtributes& atrb);

    protected:
        Attacker_Champion* m_Attacker{nullptr};

        double m_missing_health{0.0};
    };
}


#endif //LOLDAMAGECALC_DEFENDER_CHAMPION_H
