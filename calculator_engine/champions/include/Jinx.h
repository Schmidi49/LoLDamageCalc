/******************************************************************************
 * @file Jinx.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (26.09.2022)
 */

#ifndef LOLDAMAGECALC_JINX_H
#define LOLDAMAGECALC_JINX_H

#include <Attacker_Champion.h>
#include <Defender_Champion.h>
#include <Generic_Damage_Spell.h>

namespace LDC::champions{
    class Attacker_Jinx : public Attacker_Champion{
    public:
        Attacker_Jinx(engine_signal_system* ess, const std::string &name, const int &lvl = 1);
        ~Attacker_Jinx();

        //TODO q does not trigger spellblade

    private:
        bool m_q_stance{false};

        Generic_Damage_Spell* m_jinx_spell_w{nullptr};
        Generic_Damage_Spell* m_jinx_spell_e{nullptr};
        Generic_Damage_Spell* m_jinx_spell_r{nullptr};
    };
}

#endif //LOLDAMAGECALC_JINX_H
