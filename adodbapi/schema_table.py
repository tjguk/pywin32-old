from __future__ import division
from __future__ import absolute_import
from __future__ import print_function

"""call using an open ADO connection --> list of table names"""
from . import adodbapi

def names(connection_object):
    ado = connection_object.adoConn
    schema = ado.OpenSchema(20) # constant = adSchemaTables

    tables = []
    while not schema.EOF:
        name = adodbapi.getIndexedValue(schema.Fields,'TABLE_NAME').Value
        tables.append(name)
        schema.MoveNext()
    del schema
    return tables
