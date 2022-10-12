/******************************************************************************
 * @file Generic_Damage_Spell.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#ifndef LOLDAMAGECALC_GENERIC_DAMGAGE_SPELL_H
#define LOLDAMAGECALC_GENERIC_DAMGAGE_SPELL_H

#include <cstring>
#include <json.hpp>
#include <list>

#include <Defender_Champion.h>
#include <engine_signal_system.hpp>
#include <environment.h>

namespace LDC::champions {
    class Generic_Damage_Spell {
    public:
        Generic_Damage_Spell(const std::string& spell_name, const nlohmann::json& setup_json,
                             Defender_Champion* defender, engine_signal_system* ess);
        ~Generic_Damage_Spell();

        void set_Defender(Defender_Champion* defender){m_defender = defender;};

        void set_ess(engine_signal_system* ess){m_ess = ess;};

        bool set_lvl(const int& lvl);

        double calculate_damage(const bool &crit, const bool &enhanced, const int &instance);

        double execute_spell(const bool &crit, const bool &enhanced, const int &instance);

    private:
        bool read_json(const nlohmann::json& setup_json);
        bool read_stat(const nlohmann::json& setup_json, const std::string& attribute, std::list<int>* raw_data);
        bool read_stat(const nlohmann::json& setup_json, const std::string& attribute, std::list<double>* raw_data);

    private:
        engine_signal_system* m_ess{nullptr};
        Defender_Champion* m_defender{nullptr};
        std::string m_spell_name;

        unsigned int m_max_lvl{5};
        unsigned int m_cur_lvl{1};

        DamageType m_dmg_type{DamageType::undefined};
        bool m_can_crit{false};
        bool m_applys_onhit{false};

        double m_cur_ad{0};
        double m_cur_ap{0};
        int m_cur_base_dmg{0};
        int m_cur_spell_cost{0};
        double m_cur_cd{0};
        double m_cur_max_health{0};
        double m_cur_mis_health{0};
        double m_cur_cur_health{0};

        std::list<double> m_raw_ad{0};
        std::list<double> m_raw_ap{0};
        std::list<int> m_raw_base_dmg{0};
        std::list<int> m_raw_spell_cost{0};
        std::list<double> m_raw_cd{0};
        std::list<double> m_raw_max_health{0};
        std::list<double> m_raw_mis_health{0};
        std::list<double> m_raw_cur_health{0};
    };
}


#endif //LOLDAMAGECALC_GENERIC_DAMGAGE_SPELL_H
