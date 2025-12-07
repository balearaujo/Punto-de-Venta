#ifndef TICKET_H
#define TICKET_H

#include <iostream>
using namespace std;

class Ticket {
private:
    int codigo;
    int cantidad;
    float subtotal;

public:
    Ticket();
    Ticket(int, int, float);

    int getCodigo() const;
    int getCantidad() const;
    float getSubtotal() const;

    void setCodigo(int);
    void setCantidad(int);
    void setSubtotal(float);

    Ticket operator+(const Ticket&) const;

    friend ostream& operator<<(ostream&, const Ticket&);
};

Ticket::Ticket() { codigo = 0; cantidad = 0; subtotal = 0.0; }
Ticket::Ticket(int c, int cant, float sub) { codigo = c; cantidad = cant; subtotal = sub; }

int Ticket::getCodigo()const {
    return codigo;
}
int Ticket::getCantidad()const {
    return cantidad;
}
float Ticket::getSubtotal()const {
    return subtotal;
}

void Ticket::setCodigo(int c) {
    codigo = c;
}
void Ticket::setCantidad(int cant) {
    cantidad = cant;
}
void Ticket::setSubtotal(float sub) { 
    subtotal = sub;
}

Ticket Ticket::operator+(const Ticket& t) const {
    Ticket temp;
    temp.subtotal = subtotal + t.subtotal;
    return temp;
}

ostream& operator<<(ostream& out, const Ticket& t) {
    out << "Código: " << t.codigo
        << " | Cantidad: " << t.cantidad
        << " | Subtotal: $" << t.subtotal;
    return out;
}

#endif
