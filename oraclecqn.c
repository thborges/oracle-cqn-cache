
#include "config.h"

OCI_Statement *stmtevt = NULL;
OCI_Subscription *subcqn = NULL;

void start_oracle_cqn() {

	stmtevt = OCI_StatementCreate(oracledb);
	check_ociliberror(stmtevt);

	subcqn = OCI_SubscriptionRegister(oracledb, "sagrcache", OCI_CNT_ALL, cqn_event_handler, 9754, 0);
	check_ociliberror(subcqn);

	OCI_Statement *stmt = OCI_StatementCreate(oracledb);
	check_ociliberror(stmt);
	
	char* sqls[] = {
	  "select demi_not, codi_psv, qtde_ino from inota",
	  "select drec_nfe, codi_psv, quan_inf from infentra",
	  NULL
	  };
	  
	int i = 0;
	do {
		  check_ociliberrorb(OCI_Prepare(stmt, sqls[i]));
		  check_ociliberrorb(OCI_SubscriptionAddStatement(subcqn, stmt));
	} while (sqls[++i] != NULL);
}

void cqn_event_handler(OCI_Event *event) {

	unsigned int type = OCI_EventGetType(event);
	unsigned int op = OCI_EventGetOperation(event);
	const char *rowid = OCI_EventGetRowid(event);
	const char *tabela = OCI_EventGetObject(event);

	const char* sqlfmt = "select codi_psv from %s where rowid = :rid";
	char sql[512];
	sprintf(sql, sqlfmt, OCI_EventGetObject(event));
	check_ociliberrorb(OCI_Prepare(stmtevt, sql));
	check_ociliberrorb(OCI_BindString(stmtevt, ":rid", (char*)rowid, 0)); 
	check_ociliberrorb(OCI_Execute(stmtevt));
	
	OCI_Resultset *rs = OCI_GetResultset(stmtevt);
	check_ociliberror(rs);
	OCI_FetchNext(rs);
	const char *codi_psv = OCI_GetString(rs, 1);

	fprintf(stdout, "Tabela: %s Row: %s codi_psv: %s Op: %d Type: %d\n", tabela, rowid, codi_psv, op, type);

}

