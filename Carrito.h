#ifndef CARRITO_H
#define CARRITO_H
#include "VentaPendiente.h"
#include "ArchivoVentas.h"
#include <iostream>
using namespace std;


class Carrito { //clase carrito
private: //datos
    int idCarrito;
    VentaPendiente ventas[10];
    int numVentas;

public: 

    Carrito(int id = 0) { //constructor
        idCarrito = id;
        numVentas = 0;
    }

    int getId() const { return idCarrito; } //getter

    bool agregarVenta(const VentaPendiente& v) { //metodo para agregar venta
        if (numVentas >= 10) return false;
        ventas[numVentas++] = v;
        return true;
    }

    void mostrarCarrito() {
        cout << "\n===== CARRITO #" << idCarrito << " =====\n";
        for (int i = 0; i < numVentas; i++) {
            cout << "\nVenta " << i + 1 << endl;
            ventas[i].mostrarResumen(); //imprime carrito
        }
    }

    void confirmarTodas(ArchivoVentas& av) { //confirma las ventas
        ofstream archivo("ventas.dat", ios::app | ios::binary);

        for (int i = 0; i < numVentas; i++) { //calcula totales
            ventas[i].calcularTotales();
            ventas[i].confirmar();
            ventas[i].guardarEnArchivo(archivo);
        }

        archivo.close();
        numVentas = 0;

        cout << "\nTodas las ventas del carrito fueron confirmadas.\n";
    }

    bool estaVacio() const { //revisa si esta vacio
        return numVentas == 0;
    }
};

#endif
