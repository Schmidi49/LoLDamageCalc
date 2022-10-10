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
            json data = json::parse(f);

            for(json::iterator it = data["stats"].begin(); it != data["stats"].end(); ++it){
                ChampionBaseStat* tempStat{new ChampionBaseStat};
                const auto& newBase = data["stats"][it.key()]["base"];
                const auto& newGrowth = data["stats"][it.key()]["growth"];

                if (newBase == json_types::null || (newBase.type() != json_types::number_unsigned && newBase.type() != json_types::number_float))
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"base\"]" << std::endl;
                else
                    tempStat->set_base(newBase);
                if (newGrowth == json_types::null || (newGrowth.type() != json_types::number_unsigned && newGrowth.type() != json_types::number_float))
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"growth\"]" << std::endl;
                else
                    tempStat->set_growth(newGrowth);

                if(it.key() == "as"){
                    const auto& asRatio = data["stats"][it.key()]["ratio"];
                    if (newGrowth == json_types::null)
                        m_base_stats->as_ratio = tempStat->get_growth();
                    else if (newGrowth.type() != json_types::number_unsigned && newGrowth.type() != json_types::number_float)
                        std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"growth\"]" << std::endl;
                    else
                        m_base_stats->as_ratio = asRatio;
                }

                m_base_stats->set(it.key(), tempStat);
                m_read_json_good = true;
            }

            if(data["adaptive_type"] == json_types::null)
                std::cerr << "no adaptive type specified, using physical instead" << std::endl;
            else if(data["adaptive_type"].type() != json_types::string)
                std::cerr << "adaptive type is not a string" << std::endl;
            else if(data["adaptive_type"] == "physical")
                m_adaptive_type =physical;
            else if(data["adaptive_type"] == "magic")
                m_adaptive_type = magic;
            else
                std::cerr << "incorrect adaptive type specified" << std::endl;

            if(data["resource_mana"] == json_types::null)
                std::cerr << "no resource mana indicator specified, assuming champion does use mana" << std::endl;
            else if(data["resource_mana"].type() != json_types::boolean)
                std::cerr << "resource mana indicator is not a boolean" << std::endl;
            else
                m_uses_mana = data["resource_mana"];

            if(data["ranged"] == json_types::null)
                std::cerr << "no ranged indicator specified, assuming champion does use mana" << std::endl;
            else if(data["ranged"].type() != json_types::boolean)
                std::cerr << "resource ranged is not a boolean" << std::endl;
            else
                m_ranged_champion = data["ranged"];
        }else{
            std::cerr << "json File:\"" << jsonFile << "\" bad" << std::endl;
        }
        
    }
}
