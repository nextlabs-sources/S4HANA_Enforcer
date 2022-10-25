#ifndef TALK_WITH_HDB_H
#define TALK_WITH_HDB_H

#include "SQLDBC.h"

/*
 * these function is hooked, call xxxx_old to the original function
 * */
extern "C" {
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*Prepare_2_Fun)(SQLDBC::SQLDBC_PreparedStatement *self,
                             const char *sql,
                             const SQLDBC_StringEncoding encoding);

SQLDBC_Retcode prepare_2_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql,
                           const SQLDBC_StringEncoding encoding);
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*Prepare_3_Fun)(SQLDBC::SQLDBC_PreparedStatement *self,
                             const char *sql,
                             const SQLDBC_Length sqlLength,
                             const SQLDBC_StringEncoding encoding);

extern Prepare_3_Fun prepare_3_old;
SQLDBC_Retcode prepare_3_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql,
                           const SQLDBC_Length 	sqlLength,
                           const SQLDBC_StringEncoding encoding);
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*PreparedStat_SetBindingType_Fun)(SQLDBC::SQLDBC_PreparedStatement *self,
                                                  SQLDBC_size_t size);

extern PreparedStat_SetBindingType_Fun SetBindingType_old;
SQLDBC_Retcode SetBindingType_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                SQLDBC_size_t size);

////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*ExecuteItabFun)(SQLDBC::SQLDBC_PreparedStatement *self,
                                 void*,
                                 bool);

extern ExecuteItabFun execute_itab_old;
SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p,
                                bool b);
////////////////////////////////////////////////////////////////////
typedef
void* (*GetItabReaderFun)(SQLDBC::SQLDBC_PreparedStatement *self);

extern GetItabReaderFun get_itab_reader_old;
void* get_itab_reader_new(SQLDBC::SQLDBC_PreparedStatement *self);
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*BindParameterFun) (SQLDBC::SQLDBC_PreparedStatement *self,
                                    const SQLDBC_UInt2     Index,
                                    const SQLDBC_HostType  Type,
                                    void                  *paramAddr,
                                    SQLDBC_Length         *LengthIndicator,
                                    const SQLDBC_Length    Size,
                                    const SQLDBC_Bool      Terminate);
extern BindParameterFun bind_parameter_old;
SQLDBC_Retcode bind_parameter_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                  const SQLDBC_UInt4     Index,
                                  const SQLDBC_HostType  Type,
                                  void                  *paramAddr,
                                  SQLDBC_Length         *LengthIndicator,
                                  const SQLDBC_Length    Size,
                                  const SQLDBC_Bool      Terminate);
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*BindParameterAddrFun) (SQLDBC::SQLDBC_PreparedStatement *self,
                                        const SQLDBC_UInt2 Index, 
                                        const SQLDBC_HostType Type, 
                                        void *paramAddr, 
                                        SQLDBC_Length *LengthIndicator, 
                                        const SQLDBC_Length Size, 
                                        const SQLDBC_Bool Terminate);

extern BindParameterAddrFun bind_parameter_addr_old;
SQLDBC_Retcode bind_parameter_addr_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                        const SQLDBC_UInt2 Index, 
                                        const SQLDBC_HostType Type, 
                                        void *paramAddr, 
                                        SQLDBC_Length *LengthIndicator, 
                                        const SQLDBC_Length Size, 
                                        const SQLDBC_Bool Terminate);
///////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode (*ExecuteFun)(SQLDBC::SQLDBC_PreparedStatement *self);
extern ExecuteFun execute_old;
SQLDBC_Retcode execute_new(SQLDBC::SQLDBC_PreparedStatement *self);
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Int4 (*GetRowsAffectedFun)(SQLDBC::SQLDBC_Statement *self);

extern GetRowsAffectedFun get_rows_affected_old;
SQLDBC_Int4 get_rows_affected_new(SQLDBC::SQLDBC_Statement *self);
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC::SQLDBC_ResultSet *(*GetResultSetFun)(SQLDBC::SQLDBC_Statement *self);
extern GetResultSetFun get_result_set_old;
SQLDBC::SQLDBC_ResultSet *get_result_set_new(SQLDBC::SQLDBC_Statement *self);
/////////////////////////////////////////////////////////////////////////////

typedef void(*SQLDBC_Connection_ReleaseStatement_Prepared_Fun)(SQLDBC::SQLDBC_Connection* self, SQLDBC::SQLDBC_PreparedStatement *stat);
extern SQLDBC_Connection_ReleaseStatement_Prepared_Fun SQLDBC_Connection_ReleaseStatement_Prepared_Old;
void SQLDBC_Connection_ReleaseStatement_Prepared_New(SQLDBC::SQLDBC_Connection* self, SQLDBC::SQLDBC_PreparedStatement *stat);
/////////////////////////////////////////////////////////////////////////////

typedef SQLDBC_Retcode(*SQLDBC_Connection_Commit_Fun)(SQLDBC::SQLDBC_Connection* self);
extern SQLDBC_Connection_Commit_Fun SQLDBC_Connection_Commit_Old;
SQLDBC_Retcode SQLDBC_Connection_Commit_New(SQLDBC::SQLDBC_Connection* self);
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC::SQLDBC_Connection*(*SQLDBC_Statement_getConnection_Fun)(SQLDBC::SQLDBC_Statement* self);
extern SQLDBC_Statement_getConnection_Fun SQLDBC_Statement_getConnection_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC::SQLDBC_PreparedStatement*(*SQLDBC_Connection_createPreparedStatement_Fun)(SQLDBC::SQLDBC_Connection* self);
extern SQLDBC_Connection_createPreparedStatement_Fun SQLDBC_Connection_createPreparedStatement_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC::SQLDBC_Statement*(*SQLDBC_Connection_createStatement_Fun)(SQLDBC::SQLDBC_Connection* self);
extern SQLDBC_Connection_createStatement_Fun SQLDBC_Connection_createStatement_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC::SQLDBC_ErrorHndl &(*SQLDBC_ConnectionItem_error_Fun)(SQLDBC::SQLDBC_ConnectionItem* self);
extern SQLDBC_ConnectionItem_error_Fun SQLDBC_ConnectionItem_error_Old;
/////////////////////////////////////////////////////////////////////////////
typedef const char*(*SQLDBC_ErrorHndl_getErrorText_Fun)(SQLDBC::SQLDBC_ErrorHndl* self);
extern SQLDBC_ErrorHndl_getErrorText_Fun SQLDBC_ErrorHndl_getErrorText_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode(*SQLDBC_ResultSet_next_Fun)(SQLDBC::SQLDBC_ResultSet* self);
extern SQLDBC_ResultSet_next_Fun SQLDBC_ResultSet_next_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode(*SQLDBC_Statement_execute_Fun)(SQLDBC::SQLDBC_Statement* self,const char* sql);
extern SQLDBC_Statement_execute_Fun SQLDBC_Statement_execute_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode(*SQLDBC_ResultSet_getObject_Fun)(SQLDBC::SQLDBC_ResultSet* self,
                                                        const SQLDBC_Int4 Index,
                                                        const SQLDBC_HostType Type,
                                                        void * 	paramAddr,
                                                        SQLDBC_Length * 	LengthIndicator,
                                                        const SQLDBC_Length 	Size,
                                                        const SQLDBC_Bool 	Terminate); 	

extern SQLDBC_ResultSet_getObject_Fun SQLDBC_ResultSet_getObject_Old;
/////////////////////////////////////////////////////////////////////////////
typedef void(*SQLDBC_Connection_releaseStatement_Fun)(SQLDBC::SQLDBC_Connection* self, SQLDBC::SQLDBC_Statement *stmt ); 	
extern SQLDBC_Connection_releaseStatement_Fun SQLDBC_Connection_releaseStatement_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Int4(*SQLDBC_ResultSet_getResultCount_Fun)(SQLDBC::SQLDBC_ResultSet* self); 	
extern SQLDBC_ResultSet_getResultCount_Fun SQLDBC_ResultSet_getResultCount_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode(*SQLDBC_PreparedStatement_clearParameters_Fun)(SQLDBC::SQLDBC_PreparedStatement* self); 	
extern SQLDBC_PreparedStatement_clearParameters_Fun SQLDBC_PreparedStatement_clearParameters_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Int2(*SQLDBC_ResultSetMetaData_getColumnCount_Fun)(SQLDBC::SQLDBC_ResultSetMetaData* self); 	
extern SQLDBC_ResultSetMetaData_getColumnCount_Fun SQLDBC_ResultSetMetaData_getColumnCount_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode(*SQLDBC_ResultSetMetaData_getColumnLabel_Fun)(SQLDBC::SQLDBC_ResultSetMetaData* self,
                                                                    SQLDBC_Int2 	column,
                                                                    char * 	buffer,
                                                                    const SQLDBC_StringEncoding 	encoding,
                                                                    const SQLDBC_Length 	bufferSize,
                                                                    SQLDBC_Length * 	bufferLength); 	
extern SQLDBC_ResultSetMetaData_getColumnLabel_Fun SQLDBC_ResultSetMetaData_getColumnLabel_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC::SQLDBC_ResultSetMetaData *(*SQLDBC_ResultSet_getResultSetMetaData_Fun)(SQLDBC::SQLDBC_ResultSet* self); 	
extern SQLDBC_ResultSet_getResultSetMetaData_Fun SQLDBC_ResultSet_getResultSetMetaData_Old;
/////////////////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode(*SQLDBC_PreparedStatement_setBatchSize_Fun)(SQLDBC::SQLDBC_PreparedStatement* self, SQLDBC_UInt4 rowarraysize); 	
extern SQLDBC_PreparedStatement_setBatchSize_Fun SQLDBC_PreparedStatement_setBatchSize_Old;
SQLDBC_Retcode SQLDBC_PreparedStatement_setBatchSize_New(SQLDBC::SQLDBC_PreparedStatement* self, SQLDBC_UInt4 rowarraysize);
/////////////////////////////////////////////////////////////////////////////

}

#endif