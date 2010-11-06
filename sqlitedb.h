#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <stdlib.h>
#include <sqlite3.h>

extern sqlite3 *sqlitedb;

void start_sqlite_db();
double get_cache_estoque(requisicao_estoque *req);
void reset_cache_estoque(char *codi_psv);

#endif
