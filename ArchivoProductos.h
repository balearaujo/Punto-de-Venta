#ifndef ARCHIVOPRODUCTOS_H
#define ARCHIVOPRODUCTOS_H
#include "Varitas.h"
#include "Venta.h"
#include "Ticket.h"
#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

class ArchivoProductos { //clase archivos
private: //archivo
    const char* nombreArchivo = "productos.dat";
public: //metodos
    void CrearArchivo();
    void agregarProducto();
    void mostrarProductos();
    bool buscarProducto(int codigo, bool print);
    bool obtenerProducto (int codigo, Varitas &p, long &posicion);
    void modificarProducto(int codigo);
    void eliminarProducto(int codigo);
    bool consultaporCategoria(char * cat);
    bool consulraporIdProv(int IdPr);
    bool consultaporFecha(char* f);
    void ajustarPrecioPorcentaje(float porcentaje);
    void aplicarMayoreoCategoria(const char* categoria, float descuento); 
};

void ArchivoProductos::CrearArchivo() { //crear archivo binario
    fstream archivo(nombreArchivo, ios::app | ios::binary);
    if (!archivo) cout << "No se pudo crear el archivo.\n"; //validar que abra bien
    archivo.close();
}

void ArchivoProductos::agregarProducto() {
    ofstream archivo(nombreArchivo, ios::app | ios::binary);
    if (!archivo) { cout << "No se puede abrir el archivo.\n"; return; } //validar apertura

    cout << "Ingresa el codigo del producto (-1 para terminar): ";
    int code; cin >> code; //ingresa el codigo

    while (code != -1) {

        if (buscarProducto(code, false)) { //revisa que no este duplicado
            cout << "ERROR: Ya existe un producto con ese código.\n";
            cout << "Ingresa otro código (-1 para terminar): ";
            cin >> code; //pide otro codigo
            continue; 
        }

        Varitas v; //crea varitas

        //ingresa los datos a la clase
        v.setCodigo(code);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        char nombre[30];
        cout << "Nombre: ";
        cin.getline(nombre, 30);
        v.setNombre(nombre);

        float precio;
        cout << "Precio: ";
        cin >> precio;
        v.setPrecio(precio);

        int existencia;
        cout << "Existencia: ";
        cin >> existencia;
        v.setExistencia(existencia);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        char categoria[30];
        cout << "Categoria: ";
        cin.getline(categoria, 30);
        v.setCategoria(categoria);

        int idProv;
        cout<<"ID Proveedor(0 si no aplica)";
        cin>>idProv;
        v.setIdProveedor(idProv);

        archivo.write(reinterpret_cast<char*>(&v), sizeof(Varitas)); //los escribe en el archivo
        if (!archivo) cout <<"Error al escribir el producto en archivo \n ";
        else cout << "\nProducto agregado.\n";

        cout << "\nNuevo codigo (-1 para terminar): "; //pregunta si agragar mas
        cin >> code;
    }

    archivo.close();
}

void ArchivoProductos::mostrarProductos() { //muestar productos
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return; } //valida

    
    Varitas v;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) { //recorre el archivo
        cout << v << endl; //imprime su contenido
    }

    archivo.close();
}

bool ArchivoProductos::buscarProducto(int codigo, bool print=true) { //busca el producto
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; } //valida apertura

    Varitas v;
    bool encontrado = false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) { //recorre el archivo
        if(v.getCodigo() == codigo) { //compara con el codigo a buscar
            if(print) cout << "\n=== PRODUCTO ENCONTRADO ===\n" << v << endl; //imprime si fue encontrado
            encontrado = true; break;
        }
    }
    if(!encontrado && print) cout << "\nProducto con codigo " << codigo << " no encontrado.\n";
    archivo.close();
    return encontrado;
}

bool ArchivoProductos::consultaporCategoria(char * cat){ //busca por categoria
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo){ //valida apertura
        cout<<"No se pudo abrir el archivo\n"; return false;
    }

    Varitas v;
    bool encontrado=false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))){ //recorre el archivo
        if(strcmp(v.getCategoria(), cat)==0){ //compara con la catedoria
            cout<<"Producto de categoria encontrado"<<v<<endl;
            encontrado=true;
        }
    }
    if(!encontrado) cout << "\nProducto de categoria " << cat << " no encontrado.\n";
    archivo.close();
    return encontrado;
}


bool ArchivoProductos::consulraporIdProv(int IdPr) { //consulta por ID del proveedor
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; } //valida apertura

    Varitas v;
    bool encontrado = false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) { //recorre el archivo
        if(v.getIdProveedor() == IdPr) {
        cout << "\n=== PRODUCTO ENCONTRADO ===\n" << v << endl; //encunetra producto
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "\nProducto de " << IdPr << " no encontrado.\n";
    archivo.close();
    return encontrado;
}


bool ArchivoProductos::obtenerProducto(int codigo, Varitas &p,long &posicion){ //para obtener el producto
    ifstream archivo (nombreArchivo, ios::binary);
    if (!archivo){ //valida apertura
        cout<<"No se puede abrir el archivo\n";
        return false;
    }

    Varitas v;
    bool encontrado= false;
    while (archivo.read(reinterpret_cast<char*>(&v),sizeof(Varitas))){
        std::streampos afterRead=archivo.tellg();
        long startPos=static_cast<long> (afterRead)-static_cast<long>(sizeof(Varitas));
        if (v.getCodigo()==codigo){ //busca por codigo
            p=v;
            posicion=startPos; //lo posiciona
            encontrado=true;
            break;
        }
    }
    archivo.close();
    return encontrado;
}

void ArchivoProductos::modificarProducto(int codigo) {//modifica el producto
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo) {
        cout << "No se puede abrir el archivo.\n";
        return;
    }

    Varitas v;
    
    while (archivo.read(reinterpret_cast<char*>(&v),sizeof(Varitas))) { //recorre el archivo
        long pos = static_cast<long>(archivo.tellg()) - static_cast<long>(sizeof(Varitas));              

    
        if (v.getCodigo() == codigo) { //compara el codigo
            cout << "Producto encontrado:\n"<< v;          

            //rescribe los datos
            float nuevoPrecio;
            cout<<"\nNuevo precio: ";
            cin >> nuevoPrecio;
            v.setPrecio(nuevoPrecio);
            
            int nuevaExistencia;
            cout<<"Nueva exixstencia (ingrea -1 para mantener): ";
            cin >>nuevaExistencia;
            if(nuevaExistencia >=0) v.setExistencia(nuevaExistencia);


            archivo.clear();
            archivo.seekp(pos, ios::beg); // Regresas al inicio del registro
            archivo.write(reinterpret_cast<char*>(&v),sizeof(Varitas)); // Sobrescribes

            if(!archivo) cout <<"Error al escribir la modificacion\n";
            cout << "Producto modificado\n";
            archivo.close();
            return;
        }
    }

    cout << "Producto no encontrado.\n";
    archivo.close();
}

void ArchivoProductos::eliminarProducto(int codigo) { //eliminar producto
    ifstream archivoLectura(nombreArchivo, ios::binary);
    if(!archivoLectura){ //validar apertura
        cout<<"No se puede abrir el archivo para lectura\n";
        return;
    }

    ofstream archivoTemp("temp.dat", ios::binary); //archivo temporal
    if(!archivoTemp){
        cout<<"No se puede creal el archivo temporal";
        archivoLectura.close(); 
        return;
    }

    Varitas v;
    bool encontrado=false;
    while(archivoLectura.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() != codigo) //revisa si el codigo a borrar es diferente
            archivoTemp.write(reinterpret_cast<char*>(&v), sizeof(Varitas)); //si es distinto lo escribe en temporal
        else
            encontrado=true;
    }

    archivoLectura.close(); archivoTemp.close(); //cierra archivos

   if (remove(nombreArchivo)!=0){ //elimina archivo temporal
    cout<<"Error al eliminar el archivo original\n";
    return;
   }
   if (rename("temp.dat", nombreArchivo)!=0){ //renombra el archivo al nombre original
    cout<<"Error al nombrar el archivo temporal\n";
    return;
   };
   if(encontrado) cout << "Producto eliminado\n";
   else cout<<"Producto no encontrado, ningun registro se ha eliminado";
}

void ArchivoProductos::ajustarPrecioPorcentaje(float porcentaje){ //ajusta el precio por porcentajo
    fstream archivo(nombreArchivo,ios::in |ios::out| ios::binary); 
    if(!archivo){
        cout<<"No se pudo abrir el archivo de productos :(\n";
        return;
    }
    Varitas p;
    long pos;
    float factor=1+porcentaje/100.0; //operacion para porcentaje

    while (archivo.read((char*)&p, sizeof(Varitas))) {
        pos=archivo.tellg()- sizeof(Varitas);
        float precioNuevo= p.getPrecio()*factor; //multiplica por factor
        if (precioNuevo<0) precioNuevo=0;
        p.setPrecio(precioNuevo); //pone precio nuevo
        archivo.seekp(pos); //encuentra posicion
        archivo.write((char*)&p,sizeof(Varitas));
        archivo.flush();
    }
    archivo.close();
    cout<<"\nPrecios ajustados en"<<porcentaje<<"%correctamente.\n"; 
}

void ArchivoProductos::aplicarMayoreoCategoria(const char* categoria,float descuento){ //aplica descuento de mayoria a categoria
    fstream archivo(nombreArchivo, ios::in|ios::out|ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo de productos :(\n"; //valida entrada
        return;
    }
    Varitas p;
    long pos;
    float factor=1-descuento/100.0; //calcula el factor
    while (archivo.read((char*)&p, sizeof(Varitas))) {
        if (strcmp(p.getCategoria(), categoria)==0){
        pos=archivo.tellg()-sizeof(Varitas);
        float nuevoPrecio= p.getPrecio()*factor; //aplica el precio a categoria
        if (nuevoPrecio<0) nuevoPrecio=0;

        p.setPrecio(nuevoPrecio);
        archivo.seekp(pos); //encuentra posicion
        archivo.write((char*)&p,sizeof(Varitas)); //la escribe
        archivo.flush();
        }
    }
    archivo.close();
    cout<<"\nMayoreo aplicado a la categoria: "<<categoria<<"\n"; 


}

#endif