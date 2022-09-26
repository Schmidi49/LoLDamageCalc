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
        m_ess = nullptr;
    }

    Stats<double> *Base_Champion::get_current_stats() {
        for(const auto& it : *m_current_stats)
            m_current_stats->set(it.first, new double(m_base_stats->at(it.first)->at_level(m_level)));
        return m_current_stats;
    }

    Stats<ChampionBaseStat>* Base_Champion::getChampionBasesStats() {
        using t = nlohmann::json_abi_v3_11_2::detail::value_t;
        const std::string jsonFile = std::string(REPO_DIR) + std::string(json_dir) + m_name + ".json";
        Stats<ChampionBaseStat>* importedStats{new Stats<ChampionBaseStat>()};


        std::ifstream f(jsonFile);
        if(f.good()){
            json data = json::parse(f);

            for(json::iterator it = data["stats"].begin(); it != data["stats"].end(); ++it){
                ChampionBaseStat* tempStat{new ChampionBaseStat};
                const auto& newBase = data["stats"][it.key()]["base"];
                const auto& newGrowth = data["stats"][it.key()]["growth"];

                if (newBase == t::null || (newBase.type() != t::number_unsigned && newBase.type() != t::number_float))
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"base\"]" << std::endl;
                else
                    tempStat->set_base(newBase);
                if (newGrowth == t::null || (newGrowth.type() != t::number_unsigned && newGrowth.type() != t::number_float))
                    std::cerr << "Data From json invalid: [\"stats\"][" << it.key() << "][\"growth\"]" << std::endl;
                else
                    tempStat->set_growth(newGrowth);
                importedStats->set(it.key(), tempStat);
            }
        }else{
            std::cerr << "json File:\"" << jsonFile << "\" bad" << std::endl;
        }

        return importedStats;
    }
}
