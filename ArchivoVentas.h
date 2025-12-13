#ifndef ARCHIVOVENTAS_H
#define ARCHIVOVENTAS_H

#include <fstream>
#include <iostream>
#include "Varitas.h"
#include "Venta.h"
#include "ArchivoProductos.h"
#include "Gatos.h"
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
    void reportePorDiaTxt(const char *dia);
    void corteDiario(const char* fecha);
    void HistorialClienteTxt();
    void Top10productos();
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

        venta.agregarDetalle(p.getCodigo(),p.getNombre(),p.getPrecio(), p.getCosto(), cantidad, id_cliente);
        
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
    float totalDia= 0;
    float gananciaDia=0;
    int cantidadProductos=0;
    bool hayVentas=false;

    cout << "\n=== REPORTE DE VENTAS DEL DIA " << dia << " ===\n";

    while (archivo.read(reinterpret_cast<char*>(&venta), sizeof(Venta))) {
        if (strcmp(venta.getFecha(), dia) == 0) {
            venta.imprimirTicket();
            totalDia += venta.getTotal();
            gananciaDia+=venta.getTotal();
            cantidadProductos += venta.getCantidadTotalProductos();
            hayVentas=true;
        }
    }

    if(!hayVentas)
        cout<<"No se encontraron ventas para la fecha"<<dia<<"\n";
    else{
        cout << "\nTOTAL VENDIDO EN EL DIA: $" << totalDia << "\n";
        cout<<gananciaDia<<gananciaDia<<totalDia<<endl;
        cout<<"TOTAL DE PRODUCTOS VENDIDOS EN EL DIA:"<<cantidadProductos<<"\n";
    }
        archivo.close();
}

void ArchivoVentas::reportePorDiaTxt(const char *dia){
    ifstream archivo(nombreArchivo, ios::binary);
    ofstream txt("reporte_dia.txt");

    if(!archivo || !txt){
        cout << "Error al abrir archivos\n";
        return;
    }

    Venta venta;
    float total = 0, ganancia = 0;
    bool hay = false;

    txt << "REPORTE DE VENTAS DEL DIA " << dia << "\n\n";

    while(archivo.read((char*)&venta, sizeof(Venta))){
        if(strcmp(venta.getFecha(), dia) == 0){
            hay = true;
            txt << "Folio: " << venta.getFolio()
                << " Total: $" << venta.getTotal()
                << " Ganancia: $" << venta.getGanancia() << "\n";
            total += venta.getTotal();
            ganancia += venta.getGanancia();
        }
    }

    if(!hay)
        txt << "No hubo ventas este dia\n";
    else{
        txt << "\nTOTAL VENDIDO: $" << total << endl;
        txt << "GANANCIA TOTAL: $" << ganancia << endl;
    }

    archivo.close();
    txt.close();

    cout << "Reporte TXT generado correctamente.\n";
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

void ArchivoVentas::corteDiario(const char* fecha){
    ifstream ventas(nombreArchivo, ios::binary);

    if (!ventas) {
        cout << "No se pudo abrir ventas.dat\n";
        return;
    }

    float ingresos=0; Venta v; float gastos, dinero;

    while (ventas.read((char*)&v, sizeof(Venta))) {
        if (strcmp(v.getFecha(), fecha) == 0) {
            ingresos += v.getTotal();
        }
    }
    ventas.close();

    ArchivoGastos ArchGas;
    gastos=ArchGas.totalGastosPorDia(fecha);

    dinero=ingresos-gastos;

    ofstream txt("corte_diario.txt");
    txt << "===== CORTE DIARIO =====\n";
    txt << "Fecha: " << fecha << "\n\n";
    txt << "Ingresos por ventas: $" << ingresos << "\n";
    txt << "Gastos del dia: $" << gastos << "\n";
    txt << "----------------------------\n";
    txt << "SALDO FINAL: $" << dinero << "\n";
    txt.close();

    cout << "\nCORTE DIARIO GENERADO\n";
    cout << "Ingresos: $" << ingresos << endl;
    cout << "Gastos: $" << gastos << endl;
    cout << "Saldo: $" << dinero << endl;
}

void ArchivoVentas::HistorialClienteTxt() {
        ifstream ventas(nombreArchivo, ios::binary);
        ofstream txt("historial_clientes.txt");


        if (!ventas || !txt) {
            cout << "Error al abrir archivos\n";
            return;
        }

        struct Historial {
            int id;
            float total;
            int compras;
        };

        Historial hist[100];
        int n = 0;

        Venta v;

        while (ventas.read((char*)&v, sizeof(Venta))) {
            int id = v.getCliente();
            bool encontrado = false;

            for (int i = 0; i < n; i++) {
                if (hist[i].id == id) {
                    hist[i].total += v.getTotal();
                    hist[i].compras++;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                hist[n].id = id;
                hist[n].total = v.getTotal();
                hist[n].compras = 1;
                n++;
            }
        }

        txt << "===== HISTORIAL DE CLIENTES =====\n\n";

        for (int i = 0; i < n; i++) {
            txt << "Cliente ID: " << hist[i].id << "\n";
            txt << "Compras realizadas: " << hist[i].compras << "\n";
            txt << "Total gastado: $" << hist[i].total << "\n";
            txt << "------------------------------\n";
        }

        ventas.close();
        txt.close();

        cout << "Historial de clientes exportado a historial_clientes.txt\n";
    }

void ArchivoVentas::Top10productos() {
    ifstream ventas(nombreArchivo, ios::binary);
    ofstream txt("top10_productos.txt");

    if (!ventas || !txt) {
        cout << "Error al abrir archivos\n";
        return;
    }

    struct Top {
        int codigo;
        char nombre[50];
        int cantidad;
        float total;
    };

    Top lista[200];
    int n = 0;

    Venta v;

    while (ventas.read((char*)&v, sizeof(Venta))) {
        DetalleVenta* d = v.getDetalles();

        for (int i = 0; i < v.getNumDetalles(); i++) {
            int cod = d[i].getCodigo();
            bool existe = false;

            for (int j = 0; j < n; j++) {
                if (lista[j].codigo == cod) {
                    lista[j].cantidad += d[i].getCantidad();
                    lista[j].total += d[i].getSubtotal();
                    existe = true;
                    break;
                }
            }

            if (!existe) {
                lista[n].codigo = cod;
                strcpy(lista[n].nombre, d[i].getNombre());
                lista[n].cantidad = d[i].getCantidad();
                lista[n].total = d[i].getSubtotal();
                n++;
            }
        }
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (lista[j].cantidad > lista[i].cantidad) {
                Top temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }
    }

    txt << "===== TOP 10 PRODUCTOS MAS VENDIDOS =====\n\n";

    int limite = (n < 10) ? n : 10;

    for (int i = 0; i < limite; i++) {
        txt << i + 1 << ") Codigo: " << lista[i].codigo << " | "
            << lista[i].nombre << "\n";
        txt << "Cantidad vendida: " << lista[i].cantidad << "\n";
        txt << "Total generado: $" << lista[i].total << "\n";
        txt << "----------------------------\n";
    }

    ventas.close();
    txt.close();

    cout << "Top 10 generado correctamente\n";
}

#endif
