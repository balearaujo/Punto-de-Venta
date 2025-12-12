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
    void CrearArchivo();
    bool registrarVenta(ArchivoProductos &ap,int id_cliente);
    void mostrarVentas();
    void mostrarVentasCliente(int id_cliente);
    bool buscarVenta(int folio);

    void reportePorDia(const char* dia);
    void reportePorProducto(int codigo);
    void reporteTotales();
    bool consultarPorFecha(const char* fechaBuscada);

};


void ArchivoVentas::CrearArchivo() {
    fstream archivo(nombreArchivo, ios::app | ios::binary);
    if (!archivo) cout << "No se pudo crear el archivo.\n";
    archivo.close();
}

bool ArchivoVentas::registrarVenta(ArchivoProductos &ap, int id_cliente){
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

        venta.agregarDetalle(p.getCodigo(),p.getNombre(),p.getPrecio(),cantidad,id_cliente);
        
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
        cout << "\nMetodo de pago (1 = Efectivo, 2 = Tarjeta): ";
        int metodo;
        cin >> metodo;

    if (metodo == 1)
        venta.setMetodoPago("Efectivo");
    else if (metodo == 2)
        venta.setMetodoPago("Tarjeta");
    else {
    cout << "Metodo invalido, se cancela la venta.\n";
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


void ArchivoVentas::mostrarVentasCliente(int id_cliente) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir ventas.dat\n";
        return;
    }

    Venta venta;
    bool compro = false;
    int total=0;

    while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
        if(venta.getCliente()!=id_cliente)continue;
        compro = true;
        total += venta.getTotal();
        venta.imprimirTicket();
    }
    if(compro == false){
        cout << "El cliente no ha registrado ninguna venta\n";
    } else {
        cout << "El total es: " << total <<"\n";
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

void ArchivoVentas::reportePorDia(const char* dia) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir ventas.dat\n";
        return;
    }

    Venta venta;
    float totalDia = 0;
    int cantidadProductos=0;
    bool hayVentas=false;

    cout << "\n=== REPORTE DE VENTAS DEL DIA " << dia << " ===\n";

    while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
        if (strcmp(venta.getFecha(), dia) == 0) {
            venta.imprimirTicket();
            totalDia += venta.getTotal();
            cantidadProductos += venta.getCantidadTotalProductos();
            hayVentas=true;
        }
    }

    if(!hayVentas)
        cout<<"No se encontraron ventas para la fecha"<<dia<<"\n";
    else{
        cout << "\nTOTAL VENDIDO EN EL DIA: $" << totalDia << "\n";
        cout<<"TOTAL DE PRODUCTOS VENDIDOS EN EL DIA:"<<cantidadProductos<<"\n";
    }
        archivo.close();
}

void ArchivoVentas::reportePorProducto(int codigo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir ventas.dat\n";
        return;
    }

    Venta venta;
    int totalCantidad = 0;
    float totalDinero = 0;

    cout << "\n=== REPORTE DEL PRODUCTO " << codigo << " ===\n";

    while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
        DetalleVenta* d = venta.getDetalles();
        for (int i = 0; i < venta.getNumDetalles(); i++) {
            if (d[i].getCodigo() == codigo) {
                d[i].imprimir();
                totalCantidad += d[i].getCantidad();
                totalDinero += d[i].getSubtotal();
            }
        }
    }

    cout << "\nTOTAL UNIDADES VENDIDAS: " << totalCantidad;
    cout << "\nTOTAL GENERADO: $" << totalDinero << "\n";

    archivo.close();
}

void ArchivoVentas::reporteTotales() {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir ventas.dat\n";
        return;
    }

    Venta venta;
    float totalGeneral = 0;

    cout << "\n=== REPORTE GENERAL DE VENTAS ===\n";

    while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
        venta.imprimirTicket();
        totalGeneral += venta.getTotal();
    }

    cout << "\nTOTAL GENERADO POR TODAS LAS VENTAS: $" << totalGeneral << "\n";

    archivo.close();
}

bool ArchivoVentas::consultarPorFecha(const char* fechaBuscada) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir el archivo de ventas.\n";
        return false;
    }

    Venta v;
    bool encontrado = false;

    while (archivo.read((char*)&v, sizeof(Venta))) {

        if (strcmp(v.getFecha(), fechaBuscada) == 0) {

            cout << "\n=== VENTA ENCONTRADA ===\n";
            cout << "Folio: " << v.getFolio() << endl;
            cout << "Total: $" << v.getTotal() << endl;
            v.imprimirTicket();  

            encontrado = true;
        }
    }

    if (!encontrado)
        cout << "\nNo hay ventas en la fecha " << fechaBuscada << endl;

    archivo.close();
    return encontrado;
}
#endif
