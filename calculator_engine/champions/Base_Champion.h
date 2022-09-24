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

namespace LDC::champion {
    class Stat{
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

    class Stats{
    public:
        Stats() = default;
        ~Stats() = default;

        auto find(const std::string& id){return m_stats.find(id);};
        Stat* at(const std::string& id){return m_stats.at(id);};

        auto begin(){return m_stats.begin();};
        auto end(){return m_stats.end();};

    private:
        std::map<std::string, Stat*> m_stats{
            {"hp", new Stat()},
            {"mana", new Stat()},
            {"armor", new Stat()},
            {"ad", new Stat()},
            {"mr", new Stat()}
        };

    public:
        Stat* hp = m_stats.at("hp");
        Stat* mana= m_stats.at("mana");
        Stat* armor = m_stats.at("armor");
        Stat* ad = m_stats.at("ad");
        Stat* mr = m_stats.at("mr");
    };



    class Base_Champion {
    public:
        explicit Base_Champion(const std::string &name);
        Base_Champion(const std::string &name, const int &lvl);

        ~Base_Champion() = default;

        double test(){return m_base_stats->hp->at_level(2);};


    private:
        Stats* getStats();

    private:
        std::string m_name;
        int m_level;

        Stats* m_base_stats;
    };
}


#endif //LOLDAMAGECALC_BASE_CHAMPION_H
