/******************************************************************************
 * @file Jinx.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (26.09.2022)
 */

#ifndef LOLDAMAGECALC_JINX_H
#define LOLDAMAGECALC_JINX_H

#include <vector>

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
        int get_spell_lvl_r() override{return m_jinx_spell_r->get_lvl();};

        bool set_spell_lvl_q(const int&lvl) override;
        bool set_spell_lvl_w(const int&lvl) override;
        bool set_spell_lvl_e(const int&lvl) override;
        bool set_spell_lvl_r(const int&lvl) override;

        void set_Defender(Defender_Champion* defender) override;

    private:
        void execute_auto_attack(const bool &crit, const bool &enhanced, const int &instance);
        void execute_passive(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_q(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_r(const bool &crit, const bool &enhanced, const int &instance);

    private:
        double m_passive_raw_as{0.0};
        double m_passive_raw_ms{1.0};
        double m_passive_cur_as{0.0};
        double m_passive_cur_ms{0.0};

        double m_advanced_aa_cost{0.0};
        double m_advanced_aa_ad_modify{1.0};
        double m_advanced_aa_as_bonus{0.0};
        int m_aa_cur_as_stacks{0};
        double m_aa_raw_as_stacks[5]{0.0,0.0,0.0,0.0,0.0};

        const int m_q_max_level{5};
        double m_q_cd{0.0};
        bool m_q_stance{false};
        int m_q_lvl{0};

        double m_r_range_max{100.0};
        double m_r_range_min{10.0};

        Generic_Damage_Spell* m_jinx_spell_w{nullptr};
        Generic_Damage_Spell* m_jinx_spell_e{nullptr};
        Generic_Damage_Spell* m_jinx_spell_r{nullptr};
    };
}

#endif //LOLDAMAGECALC_JINX_H
