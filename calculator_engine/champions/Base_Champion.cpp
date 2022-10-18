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
        m_level = lvl;

        getChampionBasesStats();

        if(ess)
            m_ess = ess;
        else
            throw std::invalid_argument("Pointer towards engine signal system is invalid");
    }


    Base_Champion::~Base_Champion() {
        m_ess = nullptr;
    }

    Stats<double> *Base_Champion::get_current_stats() {
        //IMOPRTANT!!!!! care for attackspeed, its base+(bonus*ratio)
        for(const auto& it : *m_current_stats) {
            if(it.first != "as")
                m_current_stats->set(it.first, new double(m_base_stats->at(it.first)->at_level(m_level)));
            else {
                m_current_stats->set(it.first, new double(m_base_stats->at(it.first)->get_base()
                                                          + (m_base_stats->at(it.first)->growth_at_level(m_level) *
                                                             m_base_stats->as_ratio)));
                if(*m_current_stats->as() > 2.5)
                    m_current_stats->set("as", new double(2.5));
            }
        }
        return m_current_stats;
    }

    void Base_Champion::getChampionBasesStats() {
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
                    const auto& asRatio = m_champion_data["stats"][it.key()]["ratio"];
                    if (asRatio.is_null())
                        m_base_stats->as_ratio = tempStat->get_growth();
                    else if (!asRatio.is_number_unsigned() && !asRatio.is_number_float())
                        std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"growth\"]" << std::endl;
                    else
                        m_base_stats->as_ratio = asRatio;
                }

                m_base_stats->set(it.key(), tempStat);
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
}
