#ifndef ARCHIVOPRODUCTOS_H
#define ARCHIVOPRODUCTOS_H
#include "Varitas.h"
#include "Ticket.h"
#include <fstream>
#include <iostream>
using namespace std;
#include <limits>

class ArchivoProductos {
private:
    const char* nombreArchivo = "productos.dat";
public:
    void CrearArchivo();
    void agregarProducto();
    void mostrarProductos();
    bool buscarProducto(int codigo);
    void modificarProducto(int codigo);
    void eliminarProducto(int codigo);
    Ticket registrarVenta(int codigo, int cantidad);
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

    while(code != -1) {
        Varitas v; v.setCodigo(code);

        char nombre[30];
        cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.getline(nombre,30); v.setNombre(nombre);

        float precio; cout << "Precio: "; cin >> precio; v.setPrecio(precio);

        int existencia; cout << "Existencia: "; cin >> existencia; v.setExistencia(existencia);
        char categoria[30];
        cout << "Categoria: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.getline(categoria,30); v.setCategoria(categoria);

        archivo.write(reinterpret_cast<char*>(&v), sizeof(Varitas));
        cout << "\nProducto agregado.\n";
        cout << "\nNuevo codigo (-1 para terminar): "; cin >> code;
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

bool ArchivoProductos::buscarProducto(int codigo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; }

    Varitas v;
    bool encontrado = false;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() == codigo) {
            cout << "\n=== PRODUCTO ENCONTRADO ===\n" << v << endl;
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "\nProducto con codigo " << codigo << " no encontrado.\n";
    archivo.close();
    return encontrado;
}

void ArchivoProductos::modificarProducto(int codigo) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if(!archivo) { cout << "No se puede abrir el archivo.\n"; return; }

    Varitas v;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() == codigo) {
            cout << "Producto encontrado:\n" << v << endl;

            float nuevoPrecio;
            cout << "Nuevo precio: "; cin >> nuevoPrecio; v.setPrecio(nuevoPrecio);

            archivo.seekp(-sizeof(Varitas), ios::cur);
            archivo.write(reinterpret_cast<char*>(&v), sizeof(Varitas));
            cout << "Producto modificado.\n"; break;
        }
    }
    archivo.close();
}

void ArchivoProductos::eliminarProducto(int codigo) {
    ifstream archivoLectura(nombreArchivo, ios::binary);
    ofstream archivoTemp("temp.dat", ios::binary);

    Varitas v;
    while(archivoLectura.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() != codigo)
            archivoTemp.write(reinterpret_cast<char*>(&v), sizeof(Varitas));
    }

    archivoLectura.close(); archivoTemp.close();
    remove(nombreArchivo);
    rename("temp.dat", nombreArchivo);

    cout << "Producto eliminado si existía.\n";
}

Ticket ArchivoProductos::registrarVenta(int codigo, int cantidad) {
    Ticket temp;
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if(!archivo) { cout << "No se puede abrir el archivo.\n"; return temp; }

    Varitas v;
    while(archivo.read(reinterpret_cast<char*>(&v), sizeof(Varitas))) {
        if(v.getCodigo() == codigo) {
            if(cantidad > v.getExistencia()) {
                cout << "No hay suficiente existencia.\n"; return temp;
            }

            v -= cantidad;
            float subtotal = v.getPrecio() * cantidad;
            Ticket t(codigo, cantidad, subtotal);
            temp = t; 

            archivo.seekp(-static_cast<int>(sizeof(Varitas)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&v), sizeof(Varitas));
            archivo.flush(); // asegura que se escriba inmediatamente

            cout << "\nVenta registrada:\n" << t << endl;
            break;
        }
    }

    archivo.close();
    return temp;
}

#endif
