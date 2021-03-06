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

#include "db/SchemaHelper.hpp"
#include "Config.hpp"

using namespace Database;

std::string SchemaHelper::serverSchema;
std::string SchemaHelper::accountSchema;

void SchemaHelper::setSchemata() {
    serverSchema = std::string("\"") + (Config::instance().postgres_schema_server()) + "\"";
    accountSchema = std::string("\"") + (Config::instance().postgres_schema_account()) + "\"";
}

const std::string &SchemaHelper::getServerSchema() {
    return serverSchema;
}

const std::string &SchemaHelper::getAccountSchema() {
    return accountSchema;
}
