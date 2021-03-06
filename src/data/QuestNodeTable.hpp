/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU  General  Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU  General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _QUEST_NODE_TABLE_HPP_
#define _QUEST_NODE_TABLE_HPP_

#include <string>
#include <unordered_map>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "types.hpp"
#include "globals.hpp"

class LuaScript;

struct NodeStruct {
    std::string entrypoint;
    std::shared_ptr<LuaScript> script;
};

class QuestNodeTable {
private:
    template <typename Key>
    using Table = std::unordered_multimap<Key, NodeStruct>;
    template <typename Key>
    using TableIterator = typename Table<Key>::const_iterator;

    static std::unique_ptr<QuestNodeTable> instance;
    Table<TYPE_OF_ITEM_ID> itemNodes;
    Table<unsigned int> npcNodes;
    Table<unsigned int> monsterNodes;
    Table<position> triggerNodes;

public:
    QuestNodeTable();

    template <typename Key>
    using TableRange = std::pair<TableIterator<Key>, TableIterator<Key>>;

    static QuestNodeTable &getInstance();
    void reload();
    TableRange<TYPE_OF_ITEM_ID> getItemNodes();
    TableRange<unsigned int> getNpcNodes();
    TableRange<unsigned int> getMonsterNodes();
    TableRange<position> getTriggerNodes();

private:
    void readQuest(boost::filesystem::ifstream &questFile, boost::filesystem::path &questPath);
    void clear();

    QuestNodeTable(const QuestNodeTable &) = delete;
    QuestNodeTable &operator=(const QuestNodeTable &) = delete;
};

#endif

