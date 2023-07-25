#ifndef SQLDB_H
#define SQLDB_H

#include <windows.h>
#include "C:\Program Files\MySQL\MySQL Server 8.0\include\mysql.h"
#include "C:\Program Files\MySQL\MySQL Server 8.0\include\mysqld_error.h"

class sql_db {
	private:
            char *servidor, *usuario, *password, *database;            
            MYSQL *objDatos;
                  
	public:
		sql_db(char *servidor, char *usuario, char *password, char *database);
		~sql_db();
		void sql_close();		
		MYSQL_RES * ejecutar_sql(char *sql);
		int numero_filas(MYSQL_RES *res);
		int filas_afectadas();
		int numero_columnas(MYSQL_RES *res);
		const char *nombre_columna(MYSQL_RES *res, int indice);
	    int longitud_columna(MYSQL_RES *res, int indice);
	    const char *valor_defecto_columna(MYSQL_RES *res, int indice);
	    MYSQL_ROW obtener_fila(MYSQL_RES *res);
	    void fijar_fila(MYSQL_RES *res, int indice);
	    unsigned long ultimo_id();
};

#endif
