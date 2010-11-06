#ifndef ORACLEDB_H
#define ORACLEDB_H

#include <glib.h>
#include <stdlib.h>
#include <ocilib.h>

extern OCI_Connection *oracledb;

void start_oracle_db();
void check_ociliberrorb(int b);
void check_ociliberror(void *p);
double get_estoque(requisicao_estoque *req);

#endif

