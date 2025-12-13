#ifndef VENTAPENDIENTE_H
#define VENTAPENDIENTE_H

#include "Venta.h"

class VentaPendiente : public Venta {
private:
    bool confirmada;

public:
    VentaPendiente() : Venta() {
        confirmada = false;
    }

    void confirmar() {
        confirmada = true;
    }

    bool estaConfirmada() const {
        return confirmada;
    }

    void mostrarResumen() {
        cout << "\n--- VENTA PENDIENTE ---\n";
        cout << "Cliente ID: " << getCliente() << endl;
        cout << "Productos: " << getCantidadTotalProductos() << endl;
        cout << "Subtotal: $" << getSubtotal() << endl;
    }
};

#endif
