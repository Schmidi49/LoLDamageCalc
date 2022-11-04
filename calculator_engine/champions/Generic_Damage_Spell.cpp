/******************************************************************************
 * @file Generic_Damage_Spell.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (22.09.2022)
 */

#include <iostream>

#include "Generic_Damage_Spell.h"

namespace LDC::champions {
    Generic_Damage_Spell::Generic_Damage_Spell(const std::string& spell_name, const nlohmann::json& setup_json,
                                               Attacker_Champion* attacker, Defender_Champion *defender, engine_signal_system *ess) {
        m_spell_name = spell_name;
        m_attacker = attacker;
        m_defender = defender;
        m_ess = ess;

        if(!read_json(setup_json)){
            throw std::runtime_error("reading json data failed critically!");
        }

        set_lvl(0);
    }

    Generic_Damage_Spell::~Generic_Damage_Spell() {
        m_defender = nullptr;
        m_ess = nullptr;
    }

    bool Generic_Damage_Spell::set_lvl(const int &lvl) {
        if(lvl == 0){
            m_cur_lvl = 0;

            m_cur_ad=0.0;
            m_cur_ap=0.;
            m_cur_base_dmg=0;
            m_cur_spell_cost=0;
            m_cur_cd=0.0;
            m_cur_max_health=0.0;
            m_cur_mis_health=0.0;
            m_cur_cur_health=0.0;
        }
        else if(lvl <= m_max_lvl && lvl > 0){
            m_cur_lvl = lvl;

            if(m_raw_ad.empty())
                m_cur_ad = 0.0;
            else if(m_raw_ad.size() == 1)
                m_cur_ad = m_raw_ad[0];
            else
                m_cur_ad = m_raw_ad[lvl - 1];

            if(m_raw_ap.empty())
                m_cur_ap = 0.0;
            else if(m_raw_ap.size() == 1)
                m_cur_ap = m_raw_ap[0];
            else
                m_cur_ap = m_raw_ap[lvl - 1];

            if(m_raw_base_dmg.empty())
                m_cur_base_dmg = 0;
            else if(m_raw_base_dmg.size() == 1)
                m_cur_base_dmg = m_raw_base_dmg[0];
            else
                m_cur_base_dmg = m_raw_base_dmg[lvl - 1];

            if(m_raw_spell_cost.empty())
                m_cur_spell_cost = 0;
            else if(m_raw_spell_cost.size() == 1)
                m_cur_spell_cost = m_raw_spell_cost[0];
            else
                m_cur_spell_cost = m_raw_spell_cost[lvl - 1];

            if(m_raw_cd.empty())
                m_cur_cd = 0.0;
            else if(m_raw_cd.size() == 1)
                m_cur_cd = m_raw_cd[0];
            else
                m_cur_cd = m_raw_cd[lvl - 1];

            if(m_raw_mis_health.empty())
                m_cur_mis_health = 0.0;
            else if(m_raw_mis_health.size() == 1)
                m_cur_mis_health = m_raw_mis_health[0];
            else
                m_cur_mis_health = m_raw_mis_health[lvl - 1];

            if(m_raw_cur_health.empty())
                m_cur_cur_health = 0.0;
            else if(m_raw_cur_health.size() == 1)
                m_cur_cur_health = m_raw_cur_health[0];
            else
                m_cur_cur_health = m_raw_cur_health[lvl - 1];

            if(m_raw_max_health.empty())
                m_cur_max_health = 0.0;
            else if(m_raw_max_health.size() == 1)
                m_cur_max_health = m_raw_max_health[0];
            else
                m_cur_max_health = m_raw_max_health[lvl - 1];
        }
        else{
            std::cerr << "passed level is not in the valid range for this spell" << std::endl;
            return false;
        }
        return true;
    }

    bool Generic_Damage_Spell::read_json(const nlohmann::json &setup_json) {
        // read damage type, necessary
        if(!setup_json.contains("damage_type")){
            std::cerr << "did not find a damage type, aborting" << std::endl;
            return false;
        }
        else if(!setup_json["damage_type"].is_string()){
            std::cerr << "damage type was not a string" << std::endl;
            return false;
        }
        try{
            m_dmg_type = to_DamageType(setup_json["damage_type"]);
        }
        catch (std::invalid_argument& e){
            std::cerr << e.what() << std::endl;
            return false;
        }



        //read max level, if not specified default value 5 is used
        if(setup_json.contains("max_lvl")){
            if(setup_json["max_lvl"].is_number_integer())
                m_max_lvl = setup_json["max_lvl"];
            else
                std::cerr << "max_lvl is not a integer" << std::endl;
        }
        else
            std::cout << "max_lvl not specified, using default of 5" << std::endl;

        //read can crit, if not specified default value false is used
        if(setup_json.contains("can_crit")){
            if(setup_json["can_crit"].is_boolean())
                m_can_crit = setup_json["can_crit"];
            else
                std::cerr << "can_crit is not a boolean" << std::endl;
        }
        else
            std::cout << "can_crit not specified, default false" << std::endl;

        //read aoe, if not specified default value false is used
        if(setup_json.contains("aoe")){
            if(setup_json["aoe"].is_boolean())
                m_aoe = setup_json["aoe"];
            else
                std::cerr << "aoe is not a boolean" << std::endl;
        }
        else
            std::cout << "aoe not specified, default false" << std::endl;

        //read aoe, if not specified default value false is used
        if(setup_json.contains("projectile")){
            if(setup_json["projectile"].is_boolean())
                m_projectile = setup_json["projectile"];
            else
                std::cerr << "projectile is not a boolean" << std::endl;
        }
        else
            std::cout << "projectile not specified, default false" << std::endl;

        //read aoe, if not specified default value false is used
        if(setup_json.contains("spellshieldAffected")){
            if(setup_json["spellshieldAffected"].is_boolean())
                m_spellshieldAffected = setup_json["spellshieldAffected"];
            else
                std::cerr << "spellshieldAffected is not a boolean" << std::endl;
        }
        else
            std::cout << "spellshieldAffected not specified, default true" << std::endl;

        //read apply onhit, if not specified default value false is used
        if(setup_json.contains("applys_onhit")){
            if(setup_json["applys_onhit"].is_boolean())
                m_applys_onhit = setup_json["onhit"];
            else
                std::cerr << "applys_onhit is not a boolean" << std::endl;
        }
        else
            std::cout << "applys_onhit not specified, default false" << std::endl;

        return
        read_stat(setup_json, "ad_scale", &m_raw_ad) &&
        read_stat(setup_json, "ap_scale", &m_raw_ap) &&
        read_stat(setup_json, "base_damage", &m_raw_base_dmg) &&
        read_stat(setup_json, "cost", &m_raw_spell_cost) &&
        read_stat(setup_json, "cd", &m_raw_cd) &&
        read_stat(setup_json, "max_hp_damage", &m_raw_max_health) &&
        read_stat(setup_json, "mis_hp_damage", &m_raw_mis_health) &&
        read_stat(setup_json, "cur_hp_damage", &m_raw_cur_health);
    }

    bool Generic_Damage_Spell::read_stat(const nlohmann::json &setup_json, const std::string &attribute,
                                        std::vector<int> *raw_data) {
        if(!setup_json.contains(attribute))
            std::cout << "not specified: " << attribute << std::endl;
        else if(setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_unsigned())
            raw_data = new std::vector<int>{setup_json[attribute]};
        else if(setup_json[attribute].is_array()){
            if(setup_json[attribute].empty()) {
                std::cerr << attribute << " was empty" << std::endl;
                return false;
            }
            else if(setup_json[attribute].size() == 1) {
                delete raw_data;
                if (setup_json[attribute].is_number_integer())
                    raw_data = new std::vector<int>{setup_json[attribute][0]};
                else {
                    std::cerr << "single entry of " << attribute << " is not an integer" << std::endl;
                    return false;
                }
            }
            else if(setup_json[attribute].size() == m_max_lvl) {
                raw_data->clear();
                for (int i = 0; i < m_max_lvl; i++) {
                    if (setup_json[attribute][i].is_number_integer()) {
                        raw_data->push_back(setup_json[attribute][i]);
                    } else {
                        std::cerr << "entry " << i << " of " << attribute << " is not an integer" << std::endl;
                        return false;
                    }
                }
            }
            else {
                std::cerr << attribute << " has the wrong size" << std::endl;
                return false;
            }
        }
        return true;
    }

    bool Generic_Damage_Spell::read_stat(const nlohmann::json &setup_json, const std::string &attribute,
                                        std::vector<double> *raw_data) {
        if(!setup_json.contains(attribute))
            std::cout << "not specified: " << attribute << std::endl;
        else if(setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_float())
            raw_data = new std::vector<double>{setup_json[attribute]};
        else if(setup_json[attribute].is_array()){
            if(setup_json[attribute].empty()) {
                std::cerr << attribute << " was empty" << std::endl;
                return false;
            }
            else if(setup_json[attribute].size() == 1) {
                delete raw_data;
                if (setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_float() )
                    raw_data = new std::vector<double>{setup_json[attribute][0]};
                else {
                    std::cerr << "single entry of " << attribute << " is not an integer" << std::endl;
                    return false;
                }
            }
            else if(setup_json[attribute].size() == m_max_lvl) {
                raw_data->clear();
                for (int i = 0; i < m_max_lvl; i++) {
                    if (setup_json[attribute][i].is_number_integer() || setup_json[attribute][i].is_number_float()) {
                        raw_data->push_back(setup_json[attribute][i]);
                    } else {
                        std::cerr << "entry " << i << " of " << attribute << " is not an integer" << std::endl;
                        return false;
                    }
                }
            }
            else {
                std::cerr << attribute << " has the wrong size" << std::endl;
                return false;
            }
        }
        return true;
    }

    Damage Generic_Damage_Spell::calculate_damage(const bool &crit, const bool &enhanced, const int &instance) {
        Damage dmg;
        double own_damage = m_cur_base_dmg;
        const auto& stats_attacker = m_attacker->get_current_stats();
        own_damage += m_cur_ad * *stats_attacker->ad();
        own_damage += m_cur_ap * *stats_attacker->ap();

        if(m_cur_cur_health != 0.0 )
            own_damage += m_cur_cur_health * m_defender->get_cur_health();
        if(m_cur_mis_health != 0.0 )
            own_damage += m_cur_mis_health * m_defender->get_mis_health();
        if(m_cur_max_health != 0.0 )
            own_damage += m_cur_max_health * m_defender->get_max_health();

        //TODO crit multiplier/rate yet to be implemented, mode of operation!!
        if(m_can_crit && crit)
            own_damage *= 1.75;

        dmg.set(m_dmg_type, own_damage);

        if(m_applys_onhit)
            dmg += *m_ess->attacker.get_onhit();

        return dmg;
    }

    bool Generic_Damage_Spell::execute_spell(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_cur_lvl == 0){
            std::cerr << m_spell_name << " is not leveled" << std::endl;
            return false;
        }
        if(m_cur_spell_cost > 0){
            if(!m_attacker->use_mana(m_cur_spell_cost)) {
                std::cerr << "spell could not be executed due to mana cost" << std::endl;
                return false;
            }
        }
        std::cout << "spell pressed: " << m_spell_name << std::endl;
        //TODO spellcost, cd, evtl healing etc
        DamageAtributes da = DamageAtributes();
        da.crit = crit && m_can_crit;
        da.onhit = m_applys_onhit;
        da.ActiveSpell = true;
        da.omnivamp = true;
        da.aoe = m_aoe;
        da.projectile = m_projectile;
        da.spellshieldAffected = m_spellshieldAffected;
        m_ess->attacker.deal_damage(calculate_damage(crit, enhanced, instance), da);
        return true;
    }
}
