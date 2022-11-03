/******************************************************************************
 * @file Base_Champion.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#ifndef LOLDAMAGECALC_BASE_CHAMPION_H
#define LOLDAMAGECALC_BASE_CHAMPION_H

#include <string>

#include <Stats.hpp>
#include <engine_signal_system.hpp>
#include <environment.h>

#include <json.hpp>


namespace LDC::champions {
    using json_types = nlohmann::json_abi_v3_11_2::detail::value_t;

    class Base_Champion {
    public:
        Base_Champion(engine_signal_system* ess, const std::string &name, const int &lvl = 1);

        ~Base_Champion();

        Stats<double>* get_current_stats(){return m_current_stats;};

        bool set_lvl(const int&lvl);
        virtual bool set_spell_lvl_q(const int&lvl);
        virtual bool set_spell_lvl_w(const int&lvl);
        virtual bool set_spell_lvl_e(const int&lvl);
        virtual bool set_spell_lvl_r(const int&lvl);
        bool reset_levels();

        int get_lvl() const;
        virtual int get_spell_lvl_q();
        virtual int get_spell_lvl_w();
        virtual int get_spell_lvl_e();
        virtual int get_spell_lvl_r();

    private:
        void read_champion_base_stats();

    protected:
        Stats<double>* calc_current_stats();

        static bool level_check(const int& champ_lvl, const int&q_lvl, const int&w_lvl, const int&e_lvl, const int&r_lvl);


    protected:
        std::string m_name;
        int m_champ_lvl;
        nlohmann::json m_champion_data;
        std::vector<boost::signals2::connection> m_connections;

        bool m_read_json_good{false};

        Stats<ChampionBaseStat>* m_base_stats{new Stats<ChampionBaseStat>};
        Stats<double>* m_current_stats{new Stats<double>};

        bool m_uses_mana{false};
        DamageType m_adaptive_type{physical};
        bool m_ranged_champion{false};

        bool m_attack_speed_cap{true};

        engine_signal_system* m_ess;
    };

    class Attacker_Champion;

    class Defender_Champion;
}


#endif //LOLDAMAGECALC_BASE_CHAMPION_H
