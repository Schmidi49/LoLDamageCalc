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

        read_json(setup_json);

        set_lvl(0);
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

        //read can crit, if not specified default value false is used
        if(setup_json.contains("aoe")){
            if(setup_json["aoe"].is_boolean())
                m_aoe = setup_json["aoe"];
            else
                std::cerr << "aoe is not a boolean" << std::endl;
        }
        else
            std::cout << "aoe not specified, default false" << std::endl;

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
                                        std::list<int> *raw_data) {
        if(!setup_json.contains(attribute))
            std::cout << "not specified: " << attribute << std::endl;
        else if(setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_unsigned())
            raw_data = new std::list<int>{setup_json[attribute]};
        else if(setup_json[attribute].is_array()){
            if(setup_json[attribute].empty()) {
                std::cerr << attribute << " was empty" << std::endl;
                return false;
            }
            else if(setup_json[attribute].size() == 1) {
                delete raw_data;
                if (setup_json[attribute].is_number_integer())
                    raw_data = new std::list<int>{setup_json[attribute][0]};
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
                                        std::list<double> *raw_data) {
        if(!setup_json.contains(attribute))
            std::cout << "not specified: " << attribute << std::endl;
        else if(setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_float())
            raw_data = new std::list<double>{setup_json[attribute]};
        else if(setup_json[attribute].is_array()){
            if(setup_json[attribute].empty()) {
                std::cerr << attribute << " was empty" << std::endl;
                return false;
            }
            else if(setup_json[attribute].size() == 1) {
                delete raw_data;
                if (setup_json[attribute].is_number_integer() || setup_json[attribute].is_number_float() )
                    raw_data = new std::list<double>{setup_json[attribute][0]};
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

    void Generic_Damage_Spell::execute_spell(const bool &crit, const bool &enhanced, const int &instance) {
        std::cout << "spell pressed: " << m_spell_name << std::endl;
    }
}
