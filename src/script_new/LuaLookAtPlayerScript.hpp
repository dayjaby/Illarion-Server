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

#ifndef LUALOOKATPLAYERSCRIPT_HH
#define LUALOOKATPLAYERSCRIPT_HH

#include "script/LuaScript.hpp"

class Character;

class LuaLookAtPlayerScript : public LuaScript {
	public:
		LuaLookAtPlayerScript(const std::string &filename) { }
		LuaLookAtPlayerScript(const LuaLookAtPlayerScript&) = delete;
		LuaLookAtPlayerScript& operator=(const LuaLookAtPlayerScript&) = delete;

		void lookAtPlayer(Character *source, Character *target, unsigned char mode) { }
};

#endif
