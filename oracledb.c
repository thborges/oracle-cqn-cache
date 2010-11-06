
#include "config.h"

OCI_Connection *oracledb = NULL;

void check_gerror(GError *error) {
	if (error) {
		perror(error->message);
		exit(1);
	}
}

void raise_ocierror() {
	OCI_Error *err = OCI_GetLastError();
	if (err) {
		perror(OCI_ErrorGetString(err));
		exit(1);
	}
}

void check_ociliberrorb(int b) {
	if (!b) raise_ocierror();
}

void check_ociliberror(void *p) {
	if (!p) raise_ocierror();
}

void start_oracle_db() {
  
	OCI_Initialize(NULL, NULL, OCI_ENV_CONTEXT | OCI_ENV_THREADED | OCI_ENV_EVENTS);
  
	GKeyFile *keyfile = NULL;
	GError *error = NULL;
	char *database, *user, *passwd;

	keyfile = g_key_file_new();
	check_gerror(error);

	g_key_file_load_from_file(keyfile, "sagrcache.conf", G_KEY_FILE_NONE, &error);
	check_gerror(error);
	
	database = g_key_file_get_string(keyfile, "oracle", "database", &error);
	check_gerror(error);

	user = g_key_file_get_string(keyfile, "oracle", "user", &error);
	check_gerror(error);

	passwd = g_key_file_get_string(keyfile, "oracle", "passwd", &error);
	check_gerror(error);

	oracledb = OCI_CreateConnection(database, user, passwd, OCI_SESSION_DEFAULT);
	check_ociliberror(oracledb);

	fprintf(stdout, "Conectado com oracle.\n");


}

double get_estoque(requisicao_estoque *req) {

	double saldo;
	OCI_Statement *stmt = OCI_StatementCreate(oracledb);
	check_ociliberror(stmt);

	OCI_Prepare(stmt, "select sum(SALD_CTR) from table(saldo_inicial_tipoest(:emp, :est, :prod, :data, 'S', null, 0))");
	OCI_BindShort(stmt, ":emp", &req->empresa);
	OCI_BindShort(stmt, ":est", &req->controle);
	OCI_BindString(stmt, ":prod", req->produto, 0);
	OCI_Date *d = OCI_DateCreate(NULL);
	OCI_DateSetDate(d, req->data_ano, req->data_mes, req->data_dia);
	OCI_BindDate(stmt, ":data", d);
	
	check_ociliberrorb(OCI_Execute(stmt));
	OCI_Resultset *rs = OCI_GetResultset(stmt);
	check_ociliberror(rs);

	if (OCI_FetchNext(rs)) {
		saldo = OCI_GetDouble(rs, 1);
	}
	else {
		saldo = 0.0;
	}

	OCI_FreeStatement(stmt);
	return saldo;
}

