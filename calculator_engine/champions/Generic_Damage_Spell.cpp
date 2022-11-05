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

            for(auto it: m_scalings)
                it.second->cur = 0.0;

        }
        else if(lvl <= m_max_lvl && lvl > 0){
            m_cur_lvl = lvl;

            for(auto it: m_scalings){
                if(it.second->raw.empty())
                    it.second->cur = 0.0;
                else if(it.second->raw.size() == 1)
                    it.second->cur = it.second->raw[0];
                else
                    it.second->cur = it.second->raw[lvl - 1];
            }
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

        // read damage tag, necessary
        if(!setup_json.contains("damage_tag")){
            std::cerr << "did not find a damage tag, aborting" << std::endl;
            return false;
        }
        else if(!setup_json["damage_tag"].is_string()){
            std::cerr << "damage tag was not a string" << std::endl;
            return false;
        }
        try{
            m_dmg_tag = to_DamageTag(setup_json["damage_tag"]);
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

        //read projectile, if not specified default value false is used
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

        //read applies_onhit, if not specified default value false is used
        if(setup_json.contains("applies_onhit")){
            if(setup_json["applies_onhit"].is_boolean())
                m_applies_onhit = setup_json["applies_onhit"];
            else
                std::cerr << "applies_onhit is not a boolean" << std::endl;
        }
        else
            std::cout << "applies_onhit not specified, default false" << std::endl;

        //read applies_cc, if not specified default value false is used
        if(setup_json.contains("applies_cc")){
            if(setup_json["applies_cc"].is_boolean())
                m_applies_cc = setup_json["applies_cc"];
            else
                std::cerr << "applies_cc is not a boolean" << std::endl;
        }
        else
            std::cout << "applies_cc not specified, default false" << std::endl;

        return
        read_stat(setup_json, "ad_scale", &m_scalings.ad()->raw) &&
        read_stat(setup_json, "ap_scale", &m_scalings.ap()->raw) &&
        read_stat(setup_json, "base_damage", &m_scalings.base_dmg()->raw) &&
        read_stat(setup_json, "cost", &m_scalings.spell_cost()->raw) &&
        read_stat(setup_json, "cd", &m_scalings.cd()->raw) &&
        read_stat(setup_json, "max_hp_damage", &m_scalings.max_health()->raw) &&
        read_stat(setup_json, "mis_hp_damage", &m_scalings.mis_health()->raw) &&
        read_stat(setup_json, "cur_hp_damage", &m_scalings.cur_health()->raw);
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

    double Generic_Damage_Spell::calculate_damage(const bool &crit, const bool &enhanced, const int &instance) {
        double dmg = m_scalings.base_dmg()->cur;
        const auto& stats_attacker = m_attacker->get_current_stats();
        dmg += m_scalings.ad()->cur * *stats_attacker->ad();
        dmg += m_scalings.ap()->cur * *stats_attacker->ap();

        if(m_scalings.cur_health()->cur != 0.0 )
            dmg += m_scalings.cur_health()->cur * m_defender->get_cur_health();
        if(m_scalings.mis_health()->cur != 0.0 )
            dmg += m_scalings.mis_health()->cur * m_defender->get_mis_health();
        if(m_scalings.max_health()->cur != 0.0 )
            dmg += m_scalings.max_health()->cur * m_defender->get_max_health();

        //TODO crit multiplier/rate yet to be implemented, mode of operation!!
        if(m_can_crit && crit)
            dmg *= 1.75;

        return dmg;
    }

    bool Generic_Damage_Spell::execute_spell(const bool &crit, const bool &enhanced, const int &instance) {
        if(m_cur_lvl == 0){
            std::cerr << m_spell_name << " is not leveled" << std::endl;
            return false;
        }
        if(m_scalings.spell_cost()->cur > 0){
            if(!m_attacker->use_mana(m_scalings.spell_cost()->cur)) {
                std::cerr << "spell could not be executed due to mana cost" << std::endl;
                return false;
            }
        }
        std::cout << "spell pressed: " << m_spell_name << std::endl;
        Damage dmg{calculate_damage(crit, enhanced, instance)};
        dmg.type = m_dmg_type;
        dmg.tag = m_dmg_tag;
        dmg.crit = crit && m_can_crit;
        dmg.projectile = m_projectile;
        dmg.spellshieldAffected = m_spellshieldAffected;
        dmg.cc = m_applies_cc;
        m_ess->attacker.deal_damage(dmg);
        if(m_applies_onhit)
            m_ess->attacker.execute_onhit();
        return true;
    }
}
