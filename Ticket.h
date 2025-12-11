#ifndef TICKET_H
#define TICKET_H

#include <iostream>
#include <cstring>
using namespace std;

class Ticket {
private:
    int codigo;
    char nombre[50];
    int cantidad;
    float precioUnit; 
    float subtotal;

    
public:
    Ticket();
    Ticket(int, const char[],int, float);

    inline int getCodigo() const;
    inline int getCantidad() const;
    inline float getPrecioUnit() const;
    inline float getSubtotal() const;

    inline void setCodigo(int);
    inline void setCantidad(int);
    inline void setPrecioUnit(float);
    inline void setSubtotal(float);

    Ticket operator+(const Ticket&) const;

    friend ostream& operator<<(ostream&, const Ticket&);
};

inline Ticket::Ticket() {
    codigo = 0;
    strcpy(nombre, "");
    cantidad = 0;
    precioUnit=0;
    subtotal = 0.0;
}

inline Ticket::Ticket(int c, const char n[],int cant, float pUnit){
    codigo = c;
    strcpy(nombre,n);
    cantidad = cant;
    precioUnit=pUnit;
    subtotal =cant* pUnit;
}

inline int Ticket::getCodigo()const { return codigo; }
inline int Ticket::getCantidad()const {return cantidad; }
inline float Ticket::getPrecioUnit() const { return precioUnit; }
inline float Ticket::getSubtotal()const { return subtotal; }

inline void Ticket::setCodigo(int c) {codigo = c; }
inline void Ticket::setCantidad(int cant) {cantidad = cant;}
inline void Ticket::setPrecioUnit(float pUnit) { precioUnit = pUnit; }
inline void Ticket::setSubtotal(float sub) { subtotal = sub;}

Ticket Ticket::operator+(const Ticket& t) const {
    Ticket temp;
    temp.subtotal = subtotal + t.subtotal;
    return temp;
}

ostream& operator<<(ostream& out, const Ticket& t) {
    out << "Código: " << t.codigo
        << " | Nombre: " << t.nombre
        << " | Cantidad: " << t.cantidad
        << " | Precio unitario: $" << t.precioUnit
        << " | Subtotal: $" << t.subtotal;
    return out;
}

#endif
