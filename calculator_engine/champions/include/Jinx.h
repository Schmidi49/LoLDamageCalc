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

        const static int m_q_max_lvl{5};
        double m_q_cd{0.0};
        bool m_q_stance{false};
        int m_q_lvl{0};

        double m_advanced_aa_cost{0.0};
        double m_advanced_aa_ad_modify{1.0};
        double m_advanced_aa_as_bonus{0.0};
        int m_aa_cur_as_stacks{0};
        double m_aa_raw_as_stacks[m_q_max_lvl]{0};

        double m_r_range_max{100.0};
        double m_r_range_min{10.0};

        Generic_Damage_Spell* m_jinx_spell_w{nullptr};
        Generic_Damage_Spell* m_jinx_spell_e{nullptr};
        Generic_Damage_Spell* m_jinx_spell_r{nullptr};
    };

    class Defender_Jinx : public Defender_Champion {
    public:
        Defender_Jinx(engine_signal_system *ess, const std::string &name, const int &lvl = 1);
        ~Defender_Jinx() = default;

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
        void execute_spell_w(const bool &crit, const bool &enhanced, const int &instance);
        void execute_spell_e(const bool &crit, const bool &enhanced, const int &instance);

        double slot_spell_w_slow();

    private:
        int m_q_lvl{0};
        int m_w_lvl{0};
        int m_e_lvl{0};
        int m_r_lvl{0};

        const static int m_w_max_lvl{5};

        double m_w_cur_slow={0.0};
        double m_w_raw_slows[m_w_max_lvl]{0.0};

        double m_w_raw_cost[m_w_max_lvl]{0.0};
        double m_e_cost{0.0};
    };
}

#endif //LOLDAMAGECALC_JINX_H
