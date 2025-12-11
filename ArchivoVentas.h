#ifndef ARCHIVOVENTAS_H
#define ARCHIVOVENTAS_H

#include <fstream>
#include <iostream>
#include "Varitas.h"
#include "Venta.h"
#include "ArchivoProductos.h"
using namespace std;


class ArchivoVentas {
private:
    const char* nombreArchivo = "ventas.dat";

public:
    bool registrarVenta(ArchivoProductos &ap);
    void mostrarVentas();
    bool buscarVenta(int folio);

    void reportePorDia(const char* dia);
    void reportePorProducto(int codigo);
    void reporteTotales();
};


bool ArchivoVentas::registrarVenta(ArchivoProductos &ap){
    Venta venta;           
    venta.generarFolio();
    venta.generarFechaHora();

    int codigo, cantidad;
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        cout << "\nCodigo del producto: ";
        cin >> codigo;

        Varitas p;
        long pos;

        if (!ap.obtenerProducto(codigo, p, pos)) {
            cout << "ERROR: Producto no encontrado\n";
            return false;
        }

        cout<< "Cantidad: ";
        cin>> cantidad;

        if (cantidad <= 0) {
            cout << "Cantidad invalida\n";
            return false;
        }

        if (cantidad > p.getExistencia()) {
            cout << "ERROR: el Stock es insuficiente\n";
            return false;
        }

        venta.agregarDetalle(p.getCodigo(),p.getNombre(),p.getPrecio(),cantidad);
        
        p.setExistencia(p.getExistencia()-cantidad);
        
        fstream prod("productos.dat", ios::in| ios::out| ios::binary);
        prod.seekp(pos);
        prod.write(reinterpret_cast<char*>(&p), sizeof(Varitas));
        prod.close();

        cout<<"¿Desea agregar otro producto? (s/n)";
        cin>>continuar;
    }

    if (venta.getNumDetalles() == 0) {
        cout << "Venta cancelada, no se agregaron productos\n";
        return false;
    }

        venta.calcularTotales();

        ofstream archivo(nombreArchivo,ios::app|ios::binary);
        if(!archivo){
            cout<<"Error!! No se puede abrir el archivo ventas.dat\n";
            return false;
        }

        venta.guardarEnArchivo(archivo);
        archivo.close();

        cout<<"\nVenta registrada correctamente.\n";
        venta.imprimirTicket();
        return true;
    }
    

void ArchivoVentas::mostrarVentas() {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir ventas.dat\n";
        return;
    }

    Venta venta;
    while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
       venta.imprimirTicket();
    }
}

bool ArchivoVentas::buscarVenta(int folio) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return false;

    Venta venta;
   while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
        if (venta.getFolio() == folio) {
            cout << "\n=== VENTA ENCONTRADA ===\n";
            venta.imprimirTicket();
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

#endif
