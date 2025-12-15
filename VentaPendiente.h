#ifndef VENTAPENDIENTE_H
#define VENTAPENDIENTE_H
#include "Venta.h"
#include <iostream>
using namespace std;


class VentaPendiente : public Venta { //clase heredada de venta
private:
    bool confirmada; //atributo

public:
    VentaPendiente() : Venta() { //constructor
        confirmada = false;
    }

    void confirmar() {
        confirmada = true; //revisa que este confirmada
    }

    bool estaConfirmada() const {
        return confirmada;
    }

    void mostrarResumen() const { //muestra el resumen
        cout << "\n--- VENTA PENDIENTE ---\n";
        cout << "Cliente ID: " << getCliente() << endl;
        cout << "Productos: " << getCantidadTotalProductos() << endl;
        cout << "Subtotal: $" << getSubtotal() << endl;
    }
};

#endif
