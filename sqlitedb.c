
#include "config.h"

sqlite3 *sqlitedb = NULL;

void check_sqlite_error(int rc) {
	if (rc != 0) {
		perror(sqlite3_errmsg(sqlitedb));
		exit(1);
	}
}

void start_sqlite_db() {
	int rc;
	check_sqlite_error(sqlite3_open(":memory:", &sqlitedb));

	rc = sqlite3_exec(sqlitedb, 
		"create table sini(produto TEXT, empresa INT,\
		 data INT, controle INT, saldo DOUBLE, \
		 constraint pk_sini primary key (produto, empresa, \
		     data, controle));", NULL, 0, NULL);
	check_sqlite_error(rc);
}

int get_data_as_int(requisicao_estoque *req) {
	// 20101128 = 20.101.128
	return req->data_ano * 10000 +
	       req->data_mes * 100 +
	       req->data_dia;
}

void set_cache_estoque(requisicao_estoque *req, const double saldo) {

	sqlite3_stmt *updstmt = NULL;
	check_sqlite_error(sqlite3_prepare(sqlitedb, 
		"insert into sini(empresa,produto,data,controle,saldo) values (?,?,?,?,?)", -1, &updstmt, NULL));

	check_sqlite_error(sqlite3_bind_int(updstmt, 1, req->empresa));
	check_sqlite_error(sqlite3_bind_text(updstmt, 2, req->produto, 0, NULL));
	check_sqlite_error(sqlite3_bind_int(updstmt, 3, get_data_as_int(req)));
	check_sqlite_error(sqlite3_bind_int(updstmt, 4, req->controle));
	check_sqlite_error(sqlite3_bind_double(updstmt, 5, saldo));

	sqlite3_step(updstmt);
	sqlite3_finalize(updstmt);
}

double get_cache_estoque(requisicao_estoque *req) {
	sqlite3_stmt *selstmt = NULL;
	int rc;
	double saldo;
	
	check_sqlite_error(sqlite3_prepare(sqlitedb, 
		"select saldo from sini \
		 where empresa = ? \
		   and produto = ? \
		   and data = ? \
		   and controle = ?", -1, &selstmt, NULL));

	sqlite3_bind_int(selstmt, 1, req->empresa);
	sqlite3_bind_text(selstmt, 2, req->produto, 0, NULL);
	sqlite3_bind_int(selstmt, 3, get_data_as_int(req));
	sqlite3_bind_int(selstmt, 4, req->controle);

	rc = sqlite3_step(selstmt);
	if (rc == SQLITE_ROW)
		saldo = sqlite3_column_double(selstmt, 0);
	else {
		saldo = get_estoque(req);
		set_cache_estoque(req, saldo);
	}

	sqlite3_finalize(selstmt);
	return saldo;
}

void reset_cache_estoque(char *codi_psv) {

	sqlite3_stmt *updstmt = NULL;
	check_sqlite_error(sqlite3_prepare(sqlitedb, 
		"delete from sini where produto = ?", -1, &updstmt, NULL));

	check_sqlite_error(sqlite3_bind_text(updstmt, 1, codi_psv, 0, NULL));

	sqlite3_step(updstmt);
	sqlite3_finalize(updstmt);
}

