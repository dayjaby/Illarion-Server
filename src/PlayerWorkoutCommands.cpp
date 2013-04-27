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


#include "World.hpp"

#include "Player.hpp"
#include "tuningConstants.hpp"

#include "netinterface/NetInterface.hpp"
#include "netinterface/protocol/ClientCommands.hpp"


void Player::workoutCommands() {
    if (!canAct()) {
        return;
    }

#ifdef _PLAYER_AUTO_SAVE_
    checkSave();
#endif

    ClientCommandPointer cmd = Connection->getCommand();

    if (cmd) {
        cmd->performAction(this);
    } else if (IsAlive()) {
        if (getAttackMode() && canFight()) {
            //cp->ltAction->abortAction();
            World::get()->characterAttacks(this);
        }
    }

    cmd.reset();
}

