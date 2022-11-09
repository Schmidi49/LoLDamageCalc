/******************************************************************************
 * @file Base_Champion.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#include "Base_Champion.h"

#include <iostream>
#include <fstream>
#include <exception>

#include <json.hpp>

using json = nlohmann::json;

constexpr char json_dir[] = "/calculator_engine/champions/stat_json/";

namespace LDC::champions {
    Base_Champion::Base_Champion(engine_signal_system* ess, const std::string &name, const int &lvl) {
        m_name = name;
        m_champ_lvl = lvl;

        read_champion_base_stats();
        //write multiplication of 1.0 into the percentage bonus stats, or else every stat will be 0
        for(const auto &it : *m_stats_bonus_percentage){
            m_stats_bonus_percentage->set(it.first, new double(1.0));
        }
        calc_current_stats();

        if(ess)
            m_ess = ess;
        else
            throw std::invalid_argument("Pointer towards engine signal system is invalid");

        func_auto_attack = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "Auto attack: " << (crit ? "did crit" : "did not crit") << std::endl;
        };
        func_passive = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "Passive: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_q = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "Q: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_w = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "W: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_e = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "E: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
        func_spell_r = [](const bool &crit, const bool &enhanced, const int &instance){
            std::cout << "R: " << (crit ? "did crit, " : "did not crit, ") ;
            std::cout << (enhanced ? "is enhanced, " : "is not enhanced, ");
            std::cout << "Instance: " << instance << std::endl;
        };
    }


    Base_Champion::~Base_Champion() {
        m_ess = nullptr;
        for(auto it: m_connections)
            it.disconnect();
    }

    Stats<double> *Base_Champion::calc_current_stats() {
        for(const auto& it : *m_stats_current) {
            if(it.first != "as")
                m_stats_current->set(it.first, new double((m_stats_base->at(it.first)->at_level(m_champ_lvl)
                                                           + *m_stats_bonus_flat->at(it.first))
                                                          * *m_stats_bonus_percentage->at(it.first)));
            else {
                //attackspeed has its own formula: base+(bonus*ratio)
                //note that there is no percentage bonus attack speed
                m_stats_current->set(it.first, new double(m_stats_base->at(it.first)->get_base()
                                                          + ((m_stats_base->at(it.first)->growth_at_level(m_champ_lvl)
                                                              + *m_stats_bonus_flat->at(it.first))
                                                             * m_as_ratio)));
                if(*m_stats_bonus_percentage->as() != 1.0)//warning
                    std::cout << "percentage bonus attackspeed does not exist and is therefore ignored" << std::endl;
                if(*m_stats_current->as() > 2.5 && m_attack_speed_cap_disables == 0)
                    m_stats_current->set("as", new double(2.5));
            }
        }
        m_stats_current->set("b_ad", new double(*m_stats_current->ad() - m_stats_base->ad()->at_level(m_champ_lvl)));
        return m_stats_current;
    }

    bool Base_Champion::level_check(const int &champ_lvl, const int &q_lvl, const int &w_lvl, const int &e_lvl,
                                    const int &r_lvl) {
        if(champ_lvl < (q_lvl + w_lvl + e_lvl + r_lvl)){
            std::cerr << "combined spell levels too high" << std::endl;
            return false;
        }
        switch(r_lvl){
            case 0: break;
            case 1:
                if(champ_lvl < 6){
                    std::cerr << "r_lvl too high" << std::endl;
                    return false;
                }
                break;
            case 2:
                if(champ_lvl < 11){
                    std::cerr << "r_lvl too high" << std::endl;
                    return false;
                }
                break;
            case 3:
                if(champ_lvl < 16){
                    std::cerr << "r_lvl too high" << std::endl;
                    return false;
                }
                break;
            default: std::cerr << "illegal r_lvl specified" << std::endl; return false;
        }

        if((champ_lvl/2) + 1 < q_lvl){
            std::cerr << "q_lvl too high" << std::endl;
            return false;
        }
        if((champ_lvl/2) + 1 < w_lvl){
            std::cerr << "w_lvl too high" << std::endl;
            return false;
        }
        if((champ_lvl/2) + 1 < e_lvl){
            std::cerr << "e_lvl too high" << std::endl;
            return false;
        }

        std::cout << "levels specified are legal" << std::endl;
        return true;
    }

    bool Base_Champion::reset_levels() {
        m_champ_lvl = 18; //for safety, so there is no random fail when resetting the spell levels
        if(set_spell_lvl_q(0) ||
           set_spell_lvl_w(0) ||
           set_spell_lvl_e(0) ||
           set_spell_lvl_r(0)){
            std::cerr << "unexpected failure when resetting levels" << std::endl;
            m_champ_lvl = 1;
            return false;
        }
        m_champ_lvl = 1;
        return true;
    }

    bool Base_Champion::set_lvl(const int &lvl) {
        if (lvl < 0 || lvl > 18){
            std::cerr << "illegal level: " << lvl << std::endl;
            return false;
        }
        if(!level_check(lvl,
                        get_spell_lvl_q(),
                        get_spell_lvl_w(),
                        get_spell_lvl_e(),
                        get_spell_lvl_r())){
            std::cerr << "selected champion level is not compatible with the current spell levels" << std::endl;
            return false;
        }

        m_champ_lvl = lvl;
        calc_current_stats();
        return true;
    }

    int Base_Champion::get_lvl() const {
        return m_champ_lvl;
    }

    void Base_Champion::read_champion_base_stats() {
        //TODO cleanup type check (no need to check if number_int && number:float)
        const std::string jsonFile = std::string(REPO_DIR) + std::string(json_dir) + m_name + ".json";
        std::ifstream f(jsonFile);
        if(f.good()){
            m_champion_data = json::parse(f);

            for(json::iterator it = m_champion_data["stats"].begin(); it != m_champion_data["stats"].end(); ++it){
                ChampionBaseStat* tempStat{new ChampionBaseStat};
                const auto& newBase = m_champion_data["stats"][it.key()]["base"];
                const auto& newGrowth = m_champion_data["stats"][it.key()]["growth"];

                if (newBase.is_null() || (!newBase.is_number_unsigned() && !newBase.is_number_float()))
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"base\"]" << std::endl;
                else
                    tempStat->set_base(newBase);
                if (newGrowth.is_null() || (!newGrowth.is_number_unsigned() && !newGrowth.is_number_float()))
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"growth\"]" << std::endl;
                else
                    tempStat->set_growth(newGrowth);

                if(it.key() == "as"){
                    const auto& asRatio = m_champion_data["stats"]["as"]["ratio"];
                    if (asRatio.is_null())
                        m_as_ratio = tempStat->get_base();
                    else if (!asRatio.is_number_unsigned() && !asRatio.is_number_float())
                        std::cerr << "Data From json invalid: [\"stats\"][\"as\"][\"growth\"]" << std::endl;
                    else
                        m_as_ratio = asRatio;
                }

                m_stats_base->set(it.key(), tempStat);
                m_read_json_good = true;
            }

            if(m_champion_data["adaptive_type"].is_null())
                std::cerr << "no adaptive type specified, using physical instead" << std::endl;
            else if(!m_champion_data["adaptive_type"].is_string())
                std::cerr << "adaptive type is not a string" << std::endl;
            else if(m_champion_data["adaptive_type"] == "physical")
                m_adaptive_type =physical;
            else if(m_champion_data["adaptive_type"] == "magic")
                m_adaptive_type = magic;
            else
                std::cerr << "incorrect adaptive type specified" << std::endl;

            if(m_champion_data["resource_mana"].is_null())
                std::cerr << "no resource mana indicator specified, assuming champion does use mana" << std::endl;
            else if(!m_champion_data["resource_mana"].is_boolean())
                std::cerr << "resource mana indicator is not a boolean" << std::endl;
            else
                m_uses_mana = m_champion_data["resource_mana"];

            if(m_champion_data["ranged"].is_null())
                std::cerr << "no ranged indicator specified, assuming champion does use mana" << std::endl;
            else if(!m_champion_data["ranged"].is_boolean())
                std::cerr << "resource ranged is not a boolean" << std::endl;
            else
                m_ranged_champion = m_champion_data["ranged"];
        }else{
            std::cerr << "json File:\"" << jsonFile << "\" bad" << std::endl;
        }
    }

    bool Base_Champion::set_spell_lvl_q(const int &lvl) {
        std::cerr << "set_spell_lvl_q is not implemented" << std::endl;
        return false;
    }

    bool Base_Champion::set_spell_lvl_w(const int &lvl) {
        std::cerr << "set_spell_lvl_w is not implemented" << std::endl;
        return false;
    }

    bool Base_Champion::set_spell_lvl_e(const int &lvl) {
        std::cerr << "set_spell_lvl_e is not implemented" << std::endl;
        return false;
    }

    bool Base_Champion::set_spell_lvl_r(const int &lvl) {
        std::cerr << "set_spell_lvl_r is not implemented" << std::endl;
        return false;
    }

    int Base_Champion::get_spell_lvl_q() {
        std::cout << "get_spell_lvl_q is not implemented" << std::endl;
        return 0;
    }

    int Base_Champion::get_spell_lvl_w() {
        std::cout << "get_spell_lvl_w is not implemented" << std::endl;
        return 0;
    }

    int Base_Champion::get_spell_lvl_e() {
        std::cout << "get_spell_lvl_e is not implemented" << std::endl;
        return 0;
    }

    int Base_Champion::get_spell_lvl_r() {
        std::cout << "get_spell_lvl_r is not implemented" << std::endl;
        return 0;
    }

    bool Base_Champion::use_mana(double mana_to_use) {
        if(!m_uses_mana){
            std::cerr << "manaless champion can not consume mana" << std::endl;
            return false;
        }
        if(mana_to_use > (*m_stats_current->mana() - m_missing_mana)){
            std::cout << "not enough mana" << std::endl;
            return false;
        }
        else if(mana_to_use < 0){
            m_missing_mana -= mana_to_use;
            if(m_missing_mana < 0)
                m_missing_mana = 0;
            return true;
        }
        else{
            m_missing_mana += mana_to_use;
            return true;
        }
    }

    void Base_Champion::add_attackspeed_cap_disable() {
        m_attack_speed_cap_disables++;
    }

    void Base_Champion::remove_attackspeed_cap_disable() {
        m_attack_speed_cap_disables--;
        if(m_attack_speed_cap_disables < 0){
            std::cerr << "m_attack_speed_cap_disables fell below 0" << std::endl;
            m_attack_speed_cap_disables = 0;
        }
    }
}
