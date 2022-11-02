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
    public:
        int get_spell_lvl_q() override{return m_q_lvl;};
        int get_spell_lvl_w() override{return m_jinx_spell_w->get_lvl();};
        int get_spell_lvl_e() override{return m_jinx_spell_e->get_lvl();};
        //TODO add r as spell
        //int get_spell_lvl_r() override{return m_jinx_spell_r->get_lvl();};

        bool set_spell_lvl_q(const int&lvl) override;
        bool set_spell_lvl_w(const int&lvl) override;
        bool set_spell_lvl_e(const int&lvl) override;
        bool set_spell_lvl_r(const int&lvl) override;

    private:
        bool m_q_stance{false};

        int m_q_lvl{0};

        Generic_Damage_Spell* m_jinx_spell_w{nullptr};
        Generic_Damage_Spell* m_jinx_spell_e{nullptr};
        Generic_Damage_Spell* m_jinx_spell_r{nullptr};
    };
}

#endif //LOLDAMAGECALC_JINX_H
