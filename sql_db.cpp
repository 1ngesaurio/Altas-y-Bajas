#include "sql_db.h"
#include <iostream>
using std::cout;
using std::endl;

sql_db :: sql_db (char *servidor, char *usuario, char *password, char *database)
{
    this->servidor = servidor;
    this->usuario = usuario;
    this->password = password;
    this->database = database;
    
   // Intentar iniciar MySQL:
   if(!(objDatos = mysql_init(0))) {
      // Imposible crear el objeto objDatos
      cout << "ERROR: imposible crear el objeto objDatos." << endl;
      getchar();
      exit(1);
   }

   // Debe existir un usuario "root" con clave de acceso "clave"
   if(!mysql_real_connect(objDatos, this->servidor, this->usuario, this->password, this->database, MYSQL_PORT, NULL, 0)) {
      // No se puede conectar con el servidor en el puerto especificado.
      cout << "Imposible conectar con servidor mysql en el puerto " 
           << MYSQL_PORT << " Error: " << mysql_error(objDatos) << endl;
      mysql_close(objDatos);
      rewind(stdin);
      getchar();
      exit(2);
   }
}

sql_db :: ~sql_db()
{
   this->sql_close();
}

void sql_db :: sql_close()
{   
   mysql_close(this->objDatos);
}

MYSQL_RES * sql_db :: ejecutar_sql(char *sql)
{
   if(mysql_ping(objDatos)) {
      cout << "Error: conexión imposible" << endl;
      this->sql_close();
   }
             
   if(mysql_query(objDatos, sql)) {
      // Error al realizar la consulta:
      cout << "ERROR: " <<  mysql_error(objDatos) << endl;
      this->sql_close();
      rewind(stdin);
      getchar();
      exit(2);
   }

   return mysql_store_result(objDatos);
}

int sql_db :: numero_filas(MYSQL_RES *res)
{
    return mysql_num_rows(res);
}

int sql_db :: filas_afectadas()
{
    return mysql_affected_rows(this->objDatos);
}

int sql_db :: numero_columnas(MYSQL_RES *res)
{
    return mysql_num_fields(res);
}

const char * sql_db :: nombre_columna(MYSQL_RES *res, int indice)
{
    return mysql_fetch_field_direct(res, indice)->name;
}

int sql_db :: longitud_columna(MYSQL_RES *res, int indice)
{
    return mysql_fetch_field_direct(res, indice)->length;
}

const char * sql_db :: valor_defecto_columna(MYSQL_RES *res, int indice)
{
    MYSQL_FIELD *col = mysql_fetch_field_direct(res, indice);
    return col->def ? col->def : "NULL";
}

MYSQL_ROW  sql_db :: obtener_fila(MYSQL_RES *res)
{
    return mysql_fetch_row(res);
}

void sql_db :: fijar_fila(MYSQL_RES *res, int indice)
{
     mysql_data_seek(res, indice);
}

unsigned long sql_db :: ultimo_id()
{
    return (unsigned long )mysql_insert_id(this->objDatos);
}
