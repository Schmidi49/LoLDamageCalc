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
                                               Defender_Champion *defender, engine_signal_system *ess) {
        m_spell_name = spell_name;
        m_defender = defender;
        m_ess = ess;

        read_json(setup_json);
    }

    Generic_Damage_Spell::~Generic_Damage_Spell() {
        m_defender = nullptr;
        m_ess = nullptr;
    }

    bool Generic_Damage_Spell::set_lvl(const int &lvl) {
        if(lvl == 0){
            m_cur_lvl = 0;

            //TODO stuff
        }
        else if(lvl <= m_max_lvl && lvl > 0){
            m_cur_lvl = lvl;

            //TODO stuff
        }
        else{
            std::cerr << "passed level is not in the valid range for this spell" << std::endl;
            return false;
        }
        return true;
    }

    bool Generic_Damage_Spell::read_json(const nlohmann::json &setup_json) {
        // read damage type, necessary
        if(setup_json["damage_type"].is_null()){
            std::cerr << "did not find a damage type, aborting" << std::endl;
            return false;
        }
        else if(!setup_json["damage_type"].is_string()){
            std::cerr << "damage type was not a string" << std::endl;
            return false;
        }
        m_dmg_type = to_DamageType(setup_json["damage_type"]);
        if(m_dmg_type == DamageType::undefined){
            std::cerr << "undefined Damage Type" << std::endl;
            return false;
        }

        //read max level, if not specified default value 5 is used
        if(!setup_json["max_lvl"].is_null()){
            if(setup_json["damage_type"].is_number_integer())
                m_max_lvl = setup_json["damage_type"];
            else
                std::cerr << "max_lvl is not a integer" << std::endl;
        }
        else
            std::cout << "max_lvl not specified, using default of 5" << std::endl;

        //read can crit, if not specified default value false is used
        if(!setup_json["can_crit"].is_null()){
            if(setup_json["can_crit"].is_boolean())
                m_can_crit = setup_json["can_crit"];
            else
                std::cerr << "can_crit is not a boolean" << std::endl;
        }
        else
            std::cout << "max_lvl not specified, default false" << std::endl;

        //read apply onhit, if not specified default value false is used
        if(!setup_json["applys_onhit"].is_null()){
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
                                        std::list<int> *raw_data) {
        if(setup_json[attribute].is_null())
            std::cout << "not specified: " << attribute << std::endl;
        else if(setup_json[attribute].is_number_integer())
            raw_data = new std::list<int>{setup_json[attribute]};
        else if(setup_json[attribute].type() == json_types::array){
            if(setup_json[attribute].empty()) {
                std::cerr << attribute << " was empty" << std::endl;
                return false;
            }
            else if(setup_json[attribute].size() == 1)
                raw_data = new std::list<int>{setup_json[attribute][0]};
            else if(setup_json[attribute].size() == m_max_lvl)
                raw_data = new std::list<int>{setup_json[attribute]};
            else {
                std::cerr << attribute << " has the wrong size" << std::endl;
                return false;
            }
        }
        return true;
    }

    bool Generic_Damage_Spell::read_stat(const nlohmann::json &setup_json, const std::string &attribute,
                                        std::list<double> *raw_data) {
        if(setup_json[attribute].is_null())
            std::cout << "not specified: " << attribute << std::endl;
        else if(setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_float())
            raw_data = new std::list<double>{setup_json[attribute]};
        else if(setup_json[attribute].type() == json_types::array){
            if(setup_json[attribute].empty()) {
                std::cerr << attribute << " was empty" << std::endl;
                return false;
            }
            else if(setup_json[attribute].size() == 1)
                raw_data = new std::list<double>{setup_json[attribute][0]};
            else if(setup_json[attribute].size() == m_max_lvl)
                raw_data = new std::list<double>{setup_json[attribute]};
            else {
                std::cerr << attribute << " has the wrong size" << std::endl;
                return false;
            }
        }
        return true;
    }
}
