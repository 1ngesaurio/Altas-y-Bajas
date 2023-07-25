//Erickson tacam 1490-20-7748
//Desarrollado en Dev c++

#include "sql_db.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

// Definición de estructura  
struct persona{ 
   int codigo; 
   char nombre[40]; 
   int edad; 
   char especialidad[40]; 
};

typedef struct persona Persona;

// Objeto global
sql_db obj_mysql("localhost", "root", "1ngesaurio9", "Municipalidad");

// Prototipos de metodos
void ingresar(), insertar(Persona info); 
void buscar(), modificar(), actualizar(Persona info), eliminar();
void ordenar(), mostrar(); 
void asignardpi();
char menu(void); void pausa();

int main(int argc, char *argv[])
{
   system("CLS"); 
   char op; 

   for(;;) 
   { 
     fflush(stdin);
     switch(op=menu()) 
     { 
        case '1': 
           ingresar(); 
           pausa(); 
           break; 
        case '2': 
           buscar(); 
           pausa();
           break; 
        case '3': 
           modificar(); 
           pausa(); 
           break; 
        case '4': 
           eliminar(); 
           pausa(); 
           break; 
        case '5': 
           ordenar(); 
           pausa();
           break; 
        case '6': 
           mostrar(); 
           pausa(); 
           break; 
        case '7': 
        	
           break; 
        default : 
           cout << "Opcion no valida"; 
           pausa();
           break; 
     } 
     cout << "\t\t\t Gracias vuelva pronto :)/ "<<endl;
     if(op=='7') break; 
     system("CLS"); 
   } 

   return 0;
}

void ingresar() 
{ 
   Persona info; 

   cout << "Ingrese nombre: ";
   cin >> info.nombre;
   cout << "Ingrese edad: ";
   cin >> info.edad;
   cout << "Ingrese especialidad: ";
   cin >> info.especialidad;   
   insertar(info);
   cout << "Persona ingresada exitosamente" << endl;
}

void insertar(Persona info) 
{  
   char *consulta;
   char sentencia[] = "INSERT INTO persona(nombre, edad, especialidad) VALUES(\'%s\', %d, \'%s\')";
   
   consulta = new char[strlen(sentencia)+sizeof(Persona)-sizeof(int)];   
   sprintf(consulta, sentencia, info.nombre, info.edad, info.especialidad);
   
   obj_mysql.ejecutar_sql(consulta);
} 

void buscar()
{
   char nombre[20];
   char *consulta;
   char sentencia[] =  "SELECT * FROM persona WHERE nombre=\'%s\'";
   MYSQL_RES *res;
   MYSQL_ROW  row;   
   
   cout << "Ingrese nombre a buscar: ";
   cin >> nombre;
   
   consulta = new char[strlen(sentencia)+strlen(nombre)];   
   sprintf(consulta, sentencia, nombre);
   res = obj_mysql.ejecutar_sql(consulta);
   
   if(obj_mysql.filas_afectadas()>0)
   {
       row = obj_mysql.obtener_fila(res);
	   cout << "Codigo: " << row[0] << endl;
	   cout << "Nombre: " << row[1] << endl;	   
	   cout << "Edad: " << row[2] << endl;
	   cout << "Especialidad: " << row[3] << endl;       	   
   }
   else
      cout << "Persona no encontrada" << endl;
}

void modificar()
{
   char nombre[20];
   char *consulta;
   char *temp;
   char sentencia[] =  "SELECT * FROM persona WHERE nombre=\'%s\'";
   MYSQL_RES *res;
   MYSQL_ROW  row;   
   Persona info;
   
   cout << "Ingrese nombre a modificar: ";
   cin >> nombre;
   
   strcpy(nombre, info.nombre);
   consulta = new char[strlen(sentencia)+strlen(nombre)];   
   sprintf(consulta, sentencia, nombre);
   res = obj_mysql.ejecutar_sql(consulta);

   if(obj_mysql.filas_afectadas()>0)
   {
       row = obj_mysql.obtener_fila(res);
       sprintf(temp, "%s", row[0]);
       info.codigo = atoi(temp);
       Persona info;
       cout << "Ingrese nombre: ";
       cin >> info.nombre;
       cout << "Ingrese edad: ";
       cin >> info.edad;
       cout << "Ingrese especialidad: ";
       cin >> info.especialidad;         
       actualizar(info);
       cout << "Datos actualizados exitosamente" << endl;       
   }
   else
      cout << "Persona no encontrada" << endl;
} 

void actualizar(Persona info)
{
   char nombre[20];
   char *consulta;
   char sentencia[] = "UPDATE persona SET nombre=\'%s\', edad=%d, =\'%s\' WHERE id=%d";
   
   strcpy(nombre, info.nombre);
   consulta = new char[strlen(sentencia)+strlen(nombre)];   
   sprintf(consulta, sentencia, info.nombre, info.edad, info.especialidad, info.codigo);
   obj_mysql.ejecutar_sql(consulta);
} 

void eliminar() 
{ 
   char nombre[20];
   char *consulta;
   char *temp;
   
   char sentencia_buscar[] = "SELECT * FROM persona WHERE nombre=\'%s\'";
   char sentencia_eliminar[] = "DELETE FROM persona WHERE id=%d";
   MYSQL_RES *res;
   MYSQL_ROW  row;   
   Persona info;
   
   cout << "Ingrese nombre a eliminar: ";
   cin >> nombre;

   consulta = new char[strlen(sentencia_buscar)+strlen(nombre)];   
   sprintf(consulta, sentencia_buscar, nombre);
   res = obj_mysql.ejecutar_sql(consulta);

   if(obj_mysql.filas_afectadas()>0)
   {
       row = obj_mysql.obtener_fila(res);
       temp = new char[strlen(row[0])];
       sprintf(temp, "%s", row[0]);
       int id = atoi(temp);
       consulta = new char[strlen(sentencia_eliminar)+sizeof(int)];   
       sprintf(consulta, sentencia_eliminar, id);
       obj_mysql.ejecutar_sql(consulta);
       cout << "Datos eliminados exitosamente" << endl;       
   }
   else
      cout << "Persona no encontrada" << endl;
} 

void ordenar() 
{   
   MYSQL_RES *res;
   MYSQL_ROW row;
   int filas, columnas;
   
   res = obj_mysql.ejecutar_sql("SELECT * FROM persona ORDER BY nombre");
   filas = obj_mysql.numero_filas(res);
   columnas = obj_mysql.numero_columnas(res);
   
   cout << "--- Listado de Personas ("  << filas << ") ---" << endl; 
   
   for(int j=0; j<columnas; j++)
       cout << obj_mysql.nombre_columna(res, j) <<  " - ";
   cout << endl;

   while(row = obj_mysql.obtener_fila(res)) 
   {
       for(int j=0; j<columnas; j++)
       {
           cout << row[j] << ", ";
       }
       cout << endl;
   }    
   cout << "Se han ordenado los registros por: nombre" << endl; 
} 

void mostrar() 
{ 
   MYSQL_RES *res;
   MYSQL_ROW row;
   int filas, columnas;
   
   res = obj_mysql.ejecutar_sql("SELECT * FROM persona");
   filas = obj_mysql.numero_filas(res);
   columnas = obj_mysql.numero_columnas(res);
   
   cout << "--- Listado de Personas ("  << filas << ") ---" << endl; 
   
   for(int j=0; j<columnas; j++)
       cout << obj_mysql.nombre_columna(res, j) <<  " - ";
   cout << endl;

   while(row = obj_mysql.obtener_fila(res)) 
   {
       for(int j=0; j<columnas; j++)
       {
           cout << row[j] << ", ";
       }
       cout << endl;
   }   
} 


char menu(void) 
{ 
   char c; 
   cout << "\n\n\t\t\t *Menu de Opciones base de datos persona*" << endl;
        cout << "\t\t\t  ------------------------------------" << endl;
        cout << "\n\t1. Ingresar" << endl;
        cout << "\t2. Buscar" << endl;
        cout << "\t3. Modificar" << endl;
        cout << "\t4. Eliminar" << endl;
        cout << "\t5. Ordenar" << endl;
        cout << "\t6. Mostrar" << endl;
        cout << "\t7. SALIR" << endl;
        
        cout << "\n\tIngrese una opcion: ";
   c=getchar(); 
   cout << endl; 
   return(c); 
}

void pausa()
{
   cout << endl;
   system("PAUSE");
   cout << endl;
}

