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

#ifndef __LUA_BINDING_HELPER
#define __LUA_BINDING_HELPER

#include "Character.hpp"

#include <memory>
#include "globals.hpp"

struct lua_State;

typedef int(*lua_function)(struct lua_State*);

// returns Character::informScriptMediumPriority if informtype is invalid
Character::informType check_informtype(int informtype);

void luaH_register_struct(struct lua_State* state, const char* tablename, const char* basename = nullptr);
void luaH_register_class(struct lua_State* state, const char* tablename, const char* basename = nullptr);
void luaH_getmethods(struct lua_State* state, const char* tablename);
void luaH_setmethods(struct lua_State* state, const char* tablename);
void luaH_register_function(struct lua_State* state, const char* name, lua_function func);
void luaH_pushint(struct lua_State *l, const std::string &key, const int &value);

// needs a positive index to work
std::shared_ptr<script_data_exchangemap> luaH_convert_to_map(struct lua_State* state, int index);

#endif