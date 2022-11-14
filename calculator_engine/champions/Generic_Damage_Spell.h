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
#include <damage.h>

namespace LDC::champions {
    struct single_scaling{
        double cur{0};
        std::vector<double> raw{};
    };

    class Scalings{
    public:
        Scalings() = default;
        ~Scalings() {
            for(auto it : m_scalings) {
                delete it.second; it.second = nullptr;}
        }

        auto find(const std::string& id){return m_scalings.find(id);};
        single_scaling* at(const std::string& id){return m_scalings.at(id);};
        void set(const std::string& id, single_scaling* newT){delete m_scalings[id]; m_scalings[id] = newT;};
        void clear(){for(auto it : m_scalings) {delete it.second;it.second = new single_scaling;}};

        auto begin(){return m_scalings.begin();};
        auto end(){return m_scalings.end();};

    private:
        std::map<std::string, single_scaling*> m_scalings{
                {"ad", new single_scaling()},
                {"ap", new single_scaling()},
                {"b_ad", new single_scaling()},
                {"base_dmg", new single_scaling()},
                {"spell_cost", new single_scaling()},
                {"cd", new single_scaling()},
                {"max_health", new single_scaling()},
                {"mis_health", new single_scaling()},
                {"cur_health", new single_scaling()},
        };

    public:
        single_scaling* ad() {return m_scalings.at("ad");};
        single_scaling* ap() {return m_scalings.at("ap");};
        single_scaling* b_ad () {return m_scalings.at("b_ad");};
        single_scaling* base_dmg () {return m_scalings.at("base_dmg");};
        single_scaling* spell_cost () {return m_scalings.at("spell_cost");};
        single_scaling* cd () {return m_scalings.at("cd");};
        single_scaling* max_health () {return m_scalings.at("max_health");};
        single_scaling* mis_health () {return m_scalings.at("mis_health");};
        single_scaling* cur_health () {return m_scalings.at("cur_health");};
    };

    class Generic_Damage_Spell {
    public:
        Generic_Damage_Spell(const std::string& spell_name, const nlohmann::json& setup_json,
                             Attacker_Champion* attacker, Defender_Champion* defender, engine_signal_system* ess);
        ~Generic_Damage_Spell();

        void set_Defender(Defender_Champion* defender){m_defender = defender;};

        void set_ess(engine_signal_system* ess){m_ess = ess;};

        bool set_lvl(const int& lvl);

        int get_lvl() const {return m_cur_lvl;};

        double get_spell_cost(){return m_scalings.spell_cost()->cur;};

        bool setup_incomplete(){return m_setup_incomplete;};

        //pre mitigation!!!!!!
        Damage calculate_damage(const bool &crit, const bool &enhanced, const int &instance);

        bool execute_spell(const bool &crit, const bool &enhanced, const int &instance);

    private:
        bool read_json(const nlohmann::json& setup_json);
        bool read_stat(const nlohmann::json& setup_json, const std::string& attribute, std::vector<double>* raw_data);

    private:
        engine_signal_system* m_ess{nullptr};
        Defender_Champion* m_defender{nullptr};
        Attacker_Champion* m_attacker{nullptr};
        std::string m_spell_name;

        bool m_setup_incomplete{false};

        int m_max_lvl{5};
        int m_cur_lvl{1};

        DamageType m_dmg_type{DamageType::magic};
        DamageTag m_dmg_tag{defaultDmg};
        bool m_can_crit{false};
        bool m_projectile{false};
        bool m_spellshieldAffected{true};
        bool m_applies_cc{false};
        bool m_applies_onhit{false};

        Scalings m_scalings;
    };
}


#endif //LOLDAMAGECALC_GENERIC_DAMGAGE_SPELL_H
