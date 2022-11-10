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

        virtual void set_Attacker(Attacker_Champion* attacker);

    protected:
        double slot_res_dmg_mod(LDC::Damage& dmg);
        virtual void slot_take_damage(const LDC::Damage& dmg);

    protected:
        Attacker_Champion* m_Attacker{nullptr};
        volatile bool m_Attacker_set{false};

        bool m_is_dead{false};
    };
}


#endif //LOLDAMAGECALC_DEFENDER_CHAMPION_H
