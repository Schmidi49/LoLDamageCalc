/******************************************************************************
 * @file Stats.h
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (26.09.2022)
 */

#ifndef LOLDAMAGECALC_STATS_HPP
#define LOLDAMAGECALC_STATS_HPP

#include <map>
#include <stdexcept>

namespace LDC::champions{
    class ChampionBaseStat{
    public:
        double at_level(int lvl) const {return m_base + m_growth * (lvl - 1) * (0.7025 + 0.0175 * (lvl - 1));};
        double growth_at_level(int lvl) const {return m_growth * (lvl - 1) * (0.7025 + 0.0175 * (lvl - 1));};
        double get_base() const {return m_base;};
        double get_growth() const {return m_growth;};

        void set_base(double base){m_base = base;};
        void set_growth(double growth){m_growth = growth;};

    private:
        double m_base{0.0};
        double m_growth{0.0};
    };

    template <typename T>class Stats{
    public:
        Stats() = default;
        ~Stats() {
            for(auto it : m_stats) {
                delete it.second; it.second = nullptr;}
        }

        auto find(const std::string& id){return m_stats.find(id);};
        T* at(const std::string& id){
            try{
                return m_stats.at(id);
            }catch(std::out_of_range &e){
                //TODO find fix for awkward implementation
                //return &T{};
                return m_pointer_to_null;
            }
        };
        void set(const std::string& id, T* newT){delete m_stats[id]; m_stats[id] = newT;};
        void add(const std::string& id, T &newT){*m_stats[id] += newT;};
        void mult(const std::string& id, T &newT){*m_stats[id] *= newT;}
        void clear(){for(auto it : m_stats) {delete it.second;it.second = new T;}};
        bool check(){return m_stats.count() == CURRENTLY_IMPLEMENTED_STATS;};

        auto begin(){return m_stats.begin();};
        auto end(){return m_stats.end();};

    private:
        static constexpr int CURRENTLY_IMPLEMENTED_STATS = 6;
        std::map<std::string, T*> m_stats{
                {"hp", new T()},
                {"mana", new T()},
                {"armor", new T()},
                {"ad", new T()},
                {"ap", new T()},
                {"as", new T()},
                {"mr", new T()},
                {"ms", new T()}
        };
        T* m_pointer_to_null = new T{};

    public:
        T* hp() {return m_stats.at("hp");};
        T* mana() {return m_stats.at("mana");};
        T* armor () {return m_stats.at("armor");};
        T* ad () {return m_stats.at("ad");};
        T* ap () {return m_stats.at("ap");};
        T* as () {return m_stats.at("as");};
        T* mr () {return m_stats.at("mr");};
        T* ms () {return m_stats.at("mr");};
        double as_ratio{1.0};
    };
}


#endif //LOLDAMAGECALC_STATS_HPP
