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

namespace LDC::champion {
    Base_Champion::Base_Champion(engine_signal_system* ess, const std::string &name, const int &lvl) {
        m_name = name;
        m_level = lvl;

        m_base_stats = getChampionBasesStats();

        if(ess)
            m_ess = ess;
        else
            throw std::invalid_argument("Pointer towards engine signal system is invalid");
    }


    Base_Champion::~Base_Champion() {
        delete m_base_stats;
        m_ess = nullptr;
    }

    Stats<ChampionBaseStat>* Base_Champion::getChampionBasesStats() {
        const std::string jsonFile = std::string(REPO_DIR) + std::string(json_dir) + m_name + ".json";
        Stats<ChampionBaseStat>* importedStats{new Stats<ChampionBaseStat>()};

        std::ifstream f(jsonFile);
        if(f.good()){
            json data = json::parse(f);
            for(auto & it : *importedStats){
                const auto& newBase = data["stats"][it.first]["base"];
                const auto& newGrowth = data["stats"][it.first]["growth"];
                if(newBase != nullptr && newGrowth != nullptr) {
                    it.second->set_base(newBase);
                    it.second->set_growth(newGrowth);
                    continue;
                }
                if(newBase == nullptr)
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.first << "][\"base\"]" << std::endl;
                if(newGrowth == nullptr)
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.first << "][\"growth\"]" << std::endl;
            }
        }else{
            std::cerr << "json File:\"" << jsonFile << "\" bad" << std::endl;
        }

        return importedStats;
    }
}
