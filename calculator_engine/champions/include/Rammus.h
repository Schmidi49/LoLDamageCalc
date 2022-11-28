/******************************************************************************
 * @file Jinx.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (26.09.2022)
 */

#ifndef LOLDAMAGECALC_RAMMUS_H
#define LOLDAMAGECALC_RAMMUS_H

#include <Attacker_Champion.h>
#include <Defender_Champion.h>
#include <Generic_Damage_Spell.h>

namespace LDC::champions{
    class Attacker_Rammus : public Attacker_Champion{

    };

    class Defender_Rammus : public Defender_Champion{
    public:
        Defender_Rammus(engine_signal_system *ess, const std::string &name, const int &lvl = 1);
        ~Defender_Rammus() = default;

    public:
        int get_spell_lvl_q() override{return m_q_lvl;};
        int get_spell_lvl_w() override{return m_w_lvl;};
        int get_spell_lvl_e() override{return m_e_lvl;};
        int get_spell_lvl_r() override{return m_r_lvl;};

        bool set_spell_lvl_q(const int&lvl) override;
        bool set_spell_lvl_w(const int&lvl) override;
        bool set_spell_lvl_e(const int&lvl) override;
        bool set_spell_lvl_r(const int&lvl) override;

    private:
        void execute_spell_q(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_w(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_e(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_r(const bool &crit, const bool &enhanced, const int &instance);

    private:
        int m_q_lvl{0};
        int m_w_lvl{0};
        int m_e_lvl{0};
        int m_r_lvl{0};
    };
}

#endif //LOLDAMAGECALC_RAMMUS_H