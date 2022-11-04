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
#include <vector>

#include <Base_Champion.h>
#include <Attacker_Champion.h>
#include <engine_signal_system.hpp>
#include <environment.h>

namespace LDC::champions {
    class Generic_Damage_Spell {
    public:
        Generic_Damage_Spell(const std::string& spell_name, const nlohmann::json& setup_json,
                             Attacker_Champion* attacker, Defender_Champion* defender, engine_signal_system* ess);
        ~Generic_Damage_Spell();

        void set_Defender(Defender_Champion* defender){m_defender = defender;};

        void set_ess(engine_signal_system* ess){m_ess = ess;};

        bool set_lvl(const int& lvl);

        int get_lvl() const {return m_cur_lvl;};

        //pre mitigation!!!!!!
        Damage calculate_damage(const bool &crit, const bool &enhanced, const int &instance);

        bool execute_spell(const bool &crit, const bool &enhanced, const int &instance);

    private:
        bool read_json(const nlohmann::json& setup_json);
        bool read_stat(const nlohmann::json& setup_json, const std::string& attribute, std::vector<int>* raw_data);
        bool read_stat(const nlohmann::json& setup_json, const std::string& attribute, std::vector<double>* raw_data);

    private:
        engine_signal_system* m_ess{nullptr};
        Defender_Champion* m_defender{nullptr};
        Attacker_Champion* m_attacker{nullptr};
        std::string m_spell_name;

        int m_max_lvl{5};
        int m_cur_lvl{1};

        DamageType m_dmg_type{DamageType::magic};
        bool m_can_crit{false};
        bool m_applys_onhit{false};
        bool m_aoe{false};
        bool m_projectile{false};
        bool m_spellshieldAffected{true};

        //TODO maybe rework structure of cur/raw stats into a combined dict
        double m_cur_ad{0.0};
        double m_cur_ap{0.0};
        int m_cur_base_dmg{0};
        int m_cur_spell_cost{0};
        double m_cur_cd{0.0};
        double m_cur_max_health{0.0};
        double m_cur_mis_health{0.0};
        double m_cur_cur_health{0.0};

        std::vector<double> m_raw_ad{};
        std::vector<double> m_raw_ap{};
        std::vector<int> m_raw_base_dmg{};
        std::vector<int> m_raw_spell_cost{};
        std::vector<double> m_raw_cd{};
        std::vector<double> m_raw_max_health{};
        std::vector<double> m_raw_mis_health{};
        std::vector<double> m_raw_cur_health{};
    };
}


#endif //LOLDAMAGECALC_GENERIC_DAMGAGE_SPELL_H
