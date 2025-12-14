#ifndef CARRITO_H
#define CARRITO_H
#include "VentaPendiente.h"
#include "ArchivoVentas.h"
#include <iostream>
using namespace std;


class Carrito {
private:
    int idCarrito;
    VentaPendiente ventas[10];
    int numVentas;

public:
    Carrito(int id = 0) {
        idCarrito = id;
        numVentas = 0;
    }

    int getId() const { return idCarrito; }

    bool agregarVenta(const VentaPendiente& v) {
        if (numVentas >= 10) return false;
        ventas[numVentas++] = v;
        return true;
    }

    void mostrarCarrito() {
        cout << "\n===== CARRITO #" << idCarrito << " =====\n";
        for (int i = 0; i < numVentas; i++) {
            cout << "\nVenta " << i + 1 << endl;
            ventas[i].mostrarResumen();
        }
    }

    void confirmarTodas(ArchivoVentas& av) {
        ofstream archivo("ventas.dat", ios::app | ios::binary);

        for (int i = 0; i < numVentas; i++) {
            ventas[i].calcularTotales();
            ventas[i].confirmar();
            ventas[i].guardarEnArchivo(archivo);
        }

        archivo.close();
        numVentas = 0;

        cout << "\nTodas las ventas del carrito fueron confirmadas.\n";
    }

    bool estaVacio() const {
        return numVentas == 0;
    }
};

#endif
