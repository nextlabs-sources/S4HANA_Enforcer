//
// Created by jeff on 2020/1/14.
//

#ifndef HOOK_EMS4H_DATATYPE_H
#define HOOK_EMS4H_DATATYPE_H


enum EM_SAP_DataType{
    EM_SAP_DATE,
    EM_SAP_TIME,
    EM_SAP_SECONDDATE,
    EM_SAP_TIMESTAMP,

    EM_SAP_TINYINT,
    EM_SAP_SMALLINT,
    EM_SAP_INTEGER,
    EM_SAP_BIGINT,
    EM_SAP_SMALLDECIMAL,
    EM_SAP_DECIMAL,
    EM_SAP_REAL,
    EM_SAP_DOUBLE,

    EM_SAP_BOOLEAN,

    EM_SAP_VARCHAR,
    EM_SAP_NVARCHAR,
    EM_SAP_ALPHANUM,
    EM_SAP_SHORTTEXT,

    //nonsupport
            EM_SAP_VARBINARY,
    EM_SAP_BLOB,
    EM_SAP_CLOB,
    EM_SAP_NCLOB,
    EM_SAP_TEXT,
    EM_SAP_ARRAY,
    EM_SAP_ST_GEOMETRY,
    EM_SAP_ST_POINT,

    //
    EM_SAP_OTHER
};

#endif //HOOK_EMS4H_DATATYPE_H
