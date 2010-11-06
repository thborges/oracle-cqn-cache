
#ifndef ORACLECQN_H
#define ORACLECQN_H

#include <stdio.h>
#include <stdlib.h>
#include <ocilib.h>

#include "oracledb.h"

extern OCI_Statement *stmtevt;
extern OCI_Subscription *subcqn;

void cqn_event_handler(OCI_Event *event);
void start_oracle_cqn();

#endif
