//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.


#ifndef _CHARACTERCONTAINER_HPP_
#define _CHARACTERCONTAINER_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <boost/lexical_cast.hpp>
#include "utility.hpp"
#include "constants.hpp"


struct PositionComparison {
    bool operator()(const position& pos1, const position& pos2) const {
        if(pos1.x == pos2.x) {
            if(pos1.y == pos2.y) {
                return pos1.z < pos2.z;
            } else {
                return pos1.y < pos2.y;
            }
        } else {
            return pos1.x < pos2.x;
        }
    }
};

template <class T>
class CharacterContainer {
public:
    typedef T* pointer;

private:
    typedef std::function<void(pointer)> for_each_type;
    typedef void(T::*for_each_member_type)();
    typedef typename std::unordered_map<TYPE_OF_CHARACTER_ID, pointer> container_type;
    typedef typename std::map<position, TYPE_OF_CHARACTER_ID,PositionComparison> position_to_id_type;
    position_to_id_type position_to_id;
    container_type container;

    bool getPosition(TYPE_OF_CHARACTER_ID id,position& pos) {
        auto i = container.find(id);
        if(i!=container.end()) {
            pos = (i->second)->getPosition();
            return true;
        } else {
            return false;
        }
    }

    bool getCharacterID(const position& pos,TYPE_OF_CHARACTER_ID& id) {
        auto i = position_to_id.find(pos);
        if(i!=position_to_id.end()) {
            id = i->second;
            return true;
        } else {
            return false;
        }
    }


    auto projection_x_axis(const position& pos, int r) const -> iterator_range<position_to_id_type::const_iterator> {
        return {{position_to_id.lower_bound(position(pos.x-r-1,0,0)),position_to_id.upper_bound(position(pos.x+r+1,0,0))}};
    }

public:
    bool empty() const {
        return container.empty();
    }

    auto size() const -> decltype(container.size()) {
        return container.size();
    }

    void insert(pointer p) {
        container.emplace(p->getId(), p);
        position_to_id[p->getPosition()] = p->getId();
    }

    pointer find(const std::string &name) const;
    pointer find(TYPE_OF_CHARACTER_ID id) const;
    pointer find(const position &pos) const;
    void update(TYPE_OF_CHARACTER_ID id, const position& old, const position& current);
    bool erase(TYPE_OF_CHARACTER_ID id);
    void clear() {
        container.clear();
    }

    std::vector<pointer> findAllCharactersInRangeOf(const position &pos, int distancemetric) const;
    std::vector<pointer> findAllCharactersInMaxRangeOf(const position &pos, int distancemetric) const;
    std::vector<pointer> findAllCharactersInScreen(const position &pos) const;
    std::vector<pointer> findAllAliveCharactersInRangeOf(const position &pos, int distancemetric) const;
    std::vector<pointer> findAllAliveCharactersInRangeOfOnSameMap(const position &pos, int distancemetric) const;
    bool findAllCharactersWithXInRangeOf(short int startx, short int endx, std::vector<pointer> &ret) const;

    void for_each(const for_each_type &function) {
        for (const auto &key_value : container) {
            function(key_value.second);
        }
    }

    void for_each(const for_each_type &function) const {
        for (const auto &key_value : container) {
            function(key_value.second);
        }
    }

    void for_each(const for_each_member_type &function) {
        for (const auto &key_value : container) {
            (key_value.second->*function)();
        }
    }
};


template <class T>
auto CharacterContainer<T>::find(const std::string &text) const -> pointer {
    try {
        auto id = boost::lexical_cast<TYPE_OF_CHARACTER_ID>(text);
        return find(id);
    } catch (boost::bad_lexical_cast &) {
        for (const auto &character : container) {
            if (comparestrings_nocase(character.second->getName(), text)) {
                return character.second;
            }
        }
    }

    return nullptr;
}


template <class T>
auto CharacterContainer<T>::find(TYPE_OF_CHARACTER_ID id) const -> pointer {
    const auto it = container.find(id);

    if (it != container.end()) {
        return it->second;
    }

    return nullptr;
}


template <class T>
auto CharacterContainer<T>::find(const position &pos) const -> pointer {
    const auto i = position_to_id.find(pos);
    if(i!=position_to_id.end()) {
        return find(i->second);
    }
    return nullptr;
}

template <class T>
void CharacterContainer<T>::update(TYPE_OF_CHARACTER_ID id, const position& old, const position& current) {
    const auto i = position_to_id.find(old);
    if(i!=position_to_id.end() && i->second==id) {
        position_to_id.erase(i);
    }
    position_to_id.insert(std::make_pair(current,id));
}



template <class T>
bool CharacterContainer<T>::erase(TYPE_OF_CHARACTER_ID id) {
    position pos;
    if(getPosition(id,pos)) {
        if(position_to_id[pos]==id) position_to_id.erase(pos);
    }
    return container.erase(id) > 0;
}


template <class T>
auto CharacterContainer<T>::findAllCharactersInRangeOf(const position &pos, int distancemetric) const -> std::vector<pointer> {
    std::vector<pointer> temp;
    auto candidates = projection_x_axis(pos,distancemetric);
    for(auto& c : candidates) {
        const position& p = c.first;
        TYPE_OF_CHARACTER_ID id = c.second;
        short int dx = p.x - pos.x;
        short int dy = p.y - pos.y;
        short int dz = p.z - pos.z;
        if((abs(dx) + abs(dy) <= distancemetric) && (-RANGEDOWN <= dz) && (dz <= RANGEUP)) {
            if(auto character=find(id)) temp.push_back(character);
        }
    };
    return temp;
}


template <class T>
auto CharacterContainer<T>::findAllCharactersInScreen(const position &pos) const -> std::vector<pointer> {
    std::vector<pointer> temp;
    const int MAX_SCREEN_RANGE = 30;
    auto candidates = projection_x_axis(pos,MAX_SCREEN_RANGE);
    for(auto& c : candidates) {
        const position& p = c.first;
        TYPE_OF_CHARACTER_ID id = c.second;
        short int dx = p.x - pos.x;
        short int dy = p.y - pos.y;
        short int dz = p.z - pos.z;
        if(auto character=find(id)) {
            if((abs(dx) + abs(dy) <= character->getScreenRange()) && (-RANGEDOWN <= dz) && (dz <= RANGEUP)) {
                temp.push_back(character);
            }
        }
    };
    return temp;
}

template <class T>
auto CharacterContainer<T>::findAllCharactersInMaxRangeOf(const position &pos, int distancemetric) const -> std::vector<pointer> {
    std::vector<pointer> temp;
    auto candidates = projection_x_axis(pos,distancemetric);
    for(auto& c : candidates) {
        const position& p = c.first;
        TYPE_OF_CHARACTER_ID id = c.second;
        short int dx = p.x - pos.x;
        short int dy = p.y - pos.y;
        short int dz = p.z - pos.z;
        if(abs(dx) <= distancemetric && abs(dy) <= distancemetric && (-RANGEDOWN <= dz) && (dz <= RANGEUP)) {
            if(auto character=find(id)) temp.push_back(character);
        }
    };
    return temp;
}


template <class T>
auto CharacterContainer<T>::findAllAliveCharactersInRangeOf(const position &pos, int distancemetric) const -> std::vector<pointer> {
    std::vector<pointer> temp;
    auto candidates = projection_x_axis(pos,distancemetric);
    for(auto& c : candidates) {
        const position& p = c.first;
        TYPE_OF_CHARACTER_ID id = c.second;
        short int dx = p.x - pos.x;
        short int dy = p.y - pos.y;
        short int dz = p.z - pos.z;
        if(((abs(dx) + abs(dy) <= distancemetric) || (distancemetric==1 && abs(dx)==1 && abs(dy)==1)) && (-RANGEDOWN <= dz) && (dz <= RANGEUP)) {
            if(auto character=find(id))
                if(character->isAlive())
                    temp.push_back(character);
        }
    };
    return temp;
}

template <class T>
auto CharacterContainer<T>::findAllAliveCharactersInRangeOfOnSameMap(const position &pos, int distancemetric) const -> std::vector<pointer> {
    std::vector<pointer> temp;
    auto candidates = projection_x_axis(pos,distancemetric);
    for(auto& c : candidates) {
        const position& p = c.first;
        TYPE_OF_CHARACTER_ID id = c.second;
        short int dx = p.x - pos.x;
        short int dy = p.y - pos.y;
        short int dz = p.z - pos.z;
        if(((abs(dx) + abs(dy) <= distancemetric) || (distancemetric==1 && abs(dx)==1 && abs(dy)==1)) && (-RANGEDOWN <= dz) && (dz <= RANGEUP)) {
            if(auto character=find(id))
                if(character->isAlive())
                    temp.push_back(character);
        }
    };
    return temp;}


template <class T>
bool CharacterContainer<T>::findAllCharactersWithXInRangeOf(short int startx, short int endx, std::vector<pointer> &ret) const {
    bool found_one = false;
    int r = (endx-startx)/2+1;
    int x = startx + (endx-startx)/2;
    auto candidates = projection_x_axis(position(x,0,0),r);
    for(auto& c : candidates) {
        const position& p = c.first;
        TYPE_OF_CHARACTER_ID id = c.second;
        if ((p.x >= startx) && (p.x <= endx)) {
            if(auto character=find(id))
                ret.push_back(character);
        }
    };
    return found_one;
}

#endif
