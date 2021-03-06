#include "libpq-fe.h"
#include "PGDatabase.h" 

int PGDatabase::Connect( const PGConnInfo& dbConnInfo ) {
	m_pConnect= PQsetdbLogin(dbConnInfo.pghost, dbConnInfo.pgport,
		dbConnInfo.pgoptions, dbConnInfo.pgtty, dbConnInfo.dbName, 
		dbConnInfo.login, dbConnInfo.passwd);
	if (PQstatus(m_pConnect) != CONNECTION_OK) {
		fprintf(stderr, "Connection to database failed: %s",
			PQerrorMessage(m_pConnect));
	    PQfinish(m_pConnect);

		return 1;
	}

	return 0;
}

int PGDatabase::DisConnect() {
	PQfinish(m_pConnect); 
	m_pConnect= NULL;

	return 0;
}

bool PGDatabase::SendQuery( const char* strQuery){ 
	int retval = PQsendQuery(m_pConnect, strQuery);
	if (retval == 0) {
		fprintf(stderr, "Send Query to database failed: %s",
			PQerrorMessage(m_pConnect));
	}
	return retval != 0;
}

PGRecordset* PGDatabase::GetRecordset() { 
	PGresult* pResult= PQgetResult(m_pConnect);

	PGRecordset *pRecord = NULL;
	if (pResult != NULL) {
		pRecord = new PGRecordset;
		pRecord->Create(pResult);
	}

	return pRecord;
}

void PGDatabase::BeginTransaction() {
	PQexec(m_pConnect, "begin transaction");
}

void PGDatabase::Commit() {
	PQexec(m_pConnect, "commit"); 
}
