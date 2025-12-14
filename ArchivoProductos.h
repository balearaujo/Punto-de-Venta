#ifndef ARCHIVOPRODUCTOS_H
#define ARCHIVOPRODUCTOS_H
#include "Varitas.h"
#include "Venta.h"
#include "Ticket.h"
#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

class ArchivoProductos {
private:
    const char* nombreArchivo = "productos.dat";
public:
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

void ArchivoProductos::CrearArchivo() {
    fstream archivo(nombreArchivo, ios::app | ios::binary);
    if (!archivo) cout << "No se pudo crear el archivo.\n";
    archivo.close();
}

void ArchivoProductos::agregarProducto() {
    ofstream archivo(nombreArchivo, ios::app | ios::binary);
    if (!archivo) { cout << "No se puede abrir el archivo.\n"; return; }

    cout << "Ingresa el codigo del producto (-1 para terminar): ";
    int code; cin >> code;

    while (code != -1) {

        if (buscarProducto(code, false)) {
            cout << "ERROR: Ya existe un producto con ese código.\n";
            cout << "Ingresa otro código (-1 para terminar): ";
            cin >> code;
            continue; // ← ahora sí existe el while
        }

        Varitas v;
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

        archivo.write(reinterpret_cast<char*>(&v), sizeof(Varitas));
        if (!archivo) cout <<"Error al escribir el producto en archivo \n ";
        else cout << "\nProducto agregado.\n";

        cout << "\nNuevo codigo (-1 para terminar): ";
        cin >> code;
    }

    archivo.close();
}

void ArchivoProductos::mostrarProductos() {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return; }

    
    Varitas v;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        cout << v << endl;
    }

    archivo.close();
}

bool ArchivoProductos::buscarProducto(int codigo, bool print=true) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; }

    Varitas v;
    bool encontrado = false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() == codigo) {
            if(print) cout << "\n=== PRODUCTO ENCONTRADO ===\n" << v << endl;
            encontrado = true; break;
        }
    }
    if(!encontrado && print) cout << "\nProducto con codigo " << codigo << " no encontrado.\n";
    archivo.close();
    return encontrado;
}

bool ArchivoProductos::consultaporCategoria(char * cat){
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo){
        cout<<"No se pudo abrir el archivo\n"; return false;
    }

    Varitas v;
    bool encontrado=false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))){
        if(strcmp(v.getCategoria(), cat)==0){
            cout<<"Producto de categoria encontrado"<<v<<endl;
            encontrado=true;
        }
    }
    if(!encontrado) cout << "\nProducto de categoria " << cat << " no encontrado.\n";
    archivo.close();
    return encontrado;
}


bool ArchivoProductos::consulraporIdProv(int IdPr) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; }

    Varitas v;
    bool encontrado = false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getIdProveedor() == IdPr) {
        cout << "\n=== PRODUCTO ENCONTRADO ===\n" << v << endl;
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "\nProducto de " << IdPr << " no encontrado.\n";
    archivo.close();
    return encontrado;
}

//falta implementar por usuario, tampoco supe como

bool ArchivoProductos::obtenerProducto(int codigo, Varitas &p,long &posicion){
    ifstream archivo (nombreArchivo, ios::binary);
    if (!archivo){
        cout<<"No se puede abrir el archivo\n";
        return false;
    }

    Varitas v;
    bool encontrado= false;
    while (archivo.read(reinterpret_cast<char*>(&v),sizeof(Varitas))){
        std::streampos afterRead=archivo.tellg();
        long startPos=static_cast<long> (afterRead)-static_cast<long>(sizeof(Varitas));
        if (v.getCodigo()==codigo){
            p=v;
            posicion=startPos;
            encontrado=true;
            break;
        }
    }
    archivo.close();
    return encontrado;
}

void ArchivoProductos::modificarProducto(int codigo) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo) {
        cout << "No se puede abrir el archivo.\n";
        return;
    }

    Varitas v;
    
    while (archivo.read(reinterpret_cast<char*>(&v),sizeof(Varitas))) {
        long pos = static_cast<long>(archivo.tellg()) - static_cast<long>(sizeof(Varitas));              

    
        if (v.getCodigo() == codigo) {
            cout << "Producto encontrado:\n"<< v;          

            float nuevoPrecio;
            cout<<"\nNuevo precio: ";
            cin >> nuevoPrecio;
            v.setPrecio(nuevoPrecio);
            
            int nuevaExistencia;
            cout<<"Nueva exixstencia (ingrea -1 para mantener): ";
            cin >>nuevaExistencia;
            if(nuevaExistencia >=0) v.setExistencia(nuevaExistencia);


            archivo.clear();
            archivo.seekp(pos, ios::beg); // 2) Regresas al inicio del registro
            archivo.write(reinterpret_cast<char*>(&v),sizeof(Varitas)); // 3) Sobrescribes

            if(!archivo) cout <<"Error al escribir la modificacion\n";
            cout << "Producto modificado\n";
            archivo.close();
            return;
        }
    }

    cout << "Producto no encontrado.\n";
    archivo.close();
}

void ArchivoProductos::eliminarProducto(int codigo) {
    ifstream archivoLectura(nombreArchivo, ios::binary);
    if(!archivoLectura){
        cout<<"No se puede abrir el archivo para lectura\n";
        return;
    }

    ofstream archivoTemp("temp.dat", ios::binary);
    if(!archivoTemp){
        cout<<"No se puede creal el archivo temporal";
        archivoLectura.close();
        return;
    }

    Varitas v;
    bool encontrado=false;
    while(archivoLectura.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() != codigo)
            archivoTemp.write(reinterpret_cast<char*>(&v), sizeof(Varitas));
        else
            encontrado=true;
    }

    archivoLectura.close(); archivoTemp.close();

   if (remove(nombreArchivo)!=0){
    cout<<"Error al eliminar el archivo original\n";
    return;
   }
   if (rename("temp.dat", nombreArchivo)!=0){
    cout<<"Error al nombrar el archivo temporal\n";
    return;
   };
   if(encontrado) cout << "Producto eliminado\n";
   else cout<<"Producto no encontrado, ningun registro se ha eliminado";
}

void ArchivoProductos::ajustarPrecioPorcentaje(float porcentaje){
    fstream archivo(nombreArchivo,ios::in |ios::out| ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo de productos :(\n";
        return;
    }
    Varitas p;
    long pos;
    float factor=1+porcentaje/100.0;

    while (archivo.read((char*)&p, sizeof(Varitas))) {
        pos=archivo.tellg()- sizeof(Varitas);
        float precioNuevo= p.getPrecio()*factor;
        if (precioNuevo<0) precioNuevo=0;
        p.setPrecio(precioNuevo);
        archivo.seekp(pos);
        archivo.write((char*)&p,sizeof(Varitas));
        archivo.flush();
    }
    archivo.close();
    cout<<"\nPrecios ajustados en"<<porcentaje<<"%correctamente.\n"; 
}

void ArchivoProductos::aplicarMayoreoCategoria(const char* categoria,float descuento){
    fstream archivo(nombreArchivo, ios::in|ios::out|ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo de productos :(\n";
        return;
    }
    Varitas p;
    long pos;
    float factor=1-descuento/100.0;
    while (archivo.read((char*)&p, sizeof(Varitas))) {
        if (strcmp(p.getCategoria(), categoria)==0){
        pos=archivo.tellg()-sizeof(Varitas);
        float nuevoPrecio= p.getPrecio()*factor;
        if (nuevoPrecio<0) nuevoPrecio=0;

        p.setPrecio(nuevoPrecio);
        archivo.seekp(pos);
        archivo.write((char*)&p,sizeof(Varitas));
        archivo.flush();
        }
    }
    archivo.close();
    cout<<"\nMayoreo aplicado a la categoria: "<<categoria<<"\n"; 


}

#endif