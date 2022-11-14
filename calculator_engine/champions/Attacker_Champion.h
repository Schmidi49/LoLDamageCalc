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

        void slot_apply_slow(const double &slow);

        virtual void set_Defender(Defender_Champion* defender);

    protected:
        Defender_Champion* m_Defender{nullptr};
        volatile bool m_Defender_set{false};

        double m_cur_slow{0.0};
    };
}


#endif //LOLDAMAGECALC_ATTACKER_CHAMPION_H
