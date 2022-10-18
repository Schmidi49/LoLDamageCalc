/******************************************************************************
 * @file Jinx.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (26.09.2022)
 */

#include "include/Jinx.h"



namespace LDC::champions{
    Attacker_Jinx::Attacker_Jinx(engine_signal_system* ess, const std::string &name, const int &lvl) :
        Attacker_Champion(ess, name, lvl){
        if(!m_champion_data.empty()) {
            std::cout << "reading Jinx champion specifics" << std::endl;
            if (!m_champion_data.contains("spell_w")) {
                std::cerr << "no \"spell_w\" specified" << std::endl;
            } else {
                delete m_jinx_spell_w;
                m_jinx_spell_w = new Generic_Damage_Spell("attacker_jinx_w", m_champion_data["spell_w"], this,
                                                          m_Defender, m_ess);
            }

            func_spell_w = [SpellPtr = m_jinx_spell_w]
                    (const bool &crit, const bool &enhanced, const int &instance) {
                SpellPtr->execute_spell(crit, enhanced, instance);
            };

            if (!m_champion_data.contains("spell_e")) {
                std::cerr << "no \"spell_e\" specified" << std::endl;
            } else {
                delete m_jinx_spell_e;
                m_jinx_spell_e = new Generic_Damage_Spell("attacker_jinx_e", m_champion_data["spell_e"], this,
                                                          m_Defender, m_ess);
            }

            func_spell_e = [SpellPtr = m_jinx_spell_e]
                    (const bool &crit, const bool &enhanced, const int &instance) {
                SpellPtr->execute_spell(crit, enhanced, instance);
            };
        }
    }

    Attacker_Jinx::~Attacker_Jinx() {
        m_jinx_spell_w = nullptr;
        m_jinx_spell_e = nullptr;
        m_jinx_spell_r = nullptr;
    }
}
