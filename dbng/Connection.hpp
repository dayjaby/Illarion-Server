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

#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>

namespace Database {
class Connection;

typedef Connection *PConnection;

class Connection {
private:
    /* The libpgxx representation of the connection to the database. */
    pqxx::connection *internalConnection;

    pqxx::transaction<> *transaction;

public:
    ~Connection(void);
    
    void beginTransaction(void);
    void commitTransaction(void);
    void rollbackTransaction(void);

    
private:
    Connection(void);
    Connection(const Connection &org);
    Connection(pqxx::connection *connection);

    friend class ConnectionManager;
};
}

#endif // _CONNECTION_HPP_
