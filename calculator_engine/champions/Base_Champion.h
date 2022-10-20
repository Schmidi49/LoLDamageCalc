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

        //TODO rm test
        double test(int lvl){return m_base_stats->hp()->at_level(lvl);};


    private:
        void read_champion_base_stats();

    protected:
        bool set_lvl(const int&lvl);

        Stats<double>* calc_current_stats();

    protected:
        std::string m_name;
        int m_level;
        nlohmann::json m_champion_data;

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
