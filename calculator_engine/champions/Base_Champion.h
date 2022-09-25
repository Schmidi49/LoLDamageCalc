/******************************************************************************
 * @file Base_Champion.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#ifndef LOLDAMAGECALC_BASE_CHAMPION_H
#define LOLDAMAGECALC_BASE_CHAMPION_H

#include <string>

#include <json.hpp>
#include <engine_signal_system.hpp>

namespace LDC::champion {
    class ChampionBaseStat{
    public:
        double at_level(int lvl) const {return m_base + (lvl - 1) * m_growth;};
        double get_base() const {return m_base;};
        double get_growth() const {return m_growth;};

        void set_base(double base){m_base = base;};
        void set_growth(double growth){m_growth = growth;};

    private:
        double m_base;
        double m_growth;
    };

    template <typename T>class Stats{
    public:
        Stats() = default;
        ~Stats() {
            for(auto it : m_stats)
                delete it.second;
        }

        auto find(const std::string& id){return m_stats.find(id);};
        T* at(const std::string& id){return m_stats.at(id);};

        auto begin(){return m_stats.begin();};
        auto end(){return m_stats.end();};

    private:
        std::map<std::string, T*> m_stats{
            {"hp", new T()},
            {"mana", new T()},
            {"armor", new T()},
            {"ad", new T()},
            {"mr", new T()}
        };

    public:
        ChampionBaseStat* hp = m_stats.at("hp");
        ChampionBaseStat* mana= m_stats.at("mana");
        ChampionBaseStat* armor = m_stats.at("armor");
        ChampionBaseStat* ad = m_stats.at("ad");
        ChampionBaseStat* mr = m_stats.at("mr");
    };

    class Base_Champion {
    public:
        Base_Champion(engine_signal_system* ess, const std::string &name, const int &lvl = 1);

        ~Base_Champion();

        //TODO rm test
        double test(int lvl){return m_base_stats->hp->at_level(lvl);};


    private:
        Stats<ChampionBaseStat>* getChampionBasesStats();

    protected:
        std::string m_name;
        int m_level;

        Stats<ChampionBaseStat>* m_base_stats;

        engine_signal_system* m_ess;
    };
}


#endif //LOLDAMAGECALC_BASE_CHAMPION_H
