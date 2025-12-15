#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <iostream>
#include <cstring>
using namespace std;

class Producto {
protected:
    //atributos
    int codigo;
    char nombre[30];
    float precio;
    int existencia;

public:
    //constructores
    Producto() : codigo(0), precio(0.0), existencia(0) {
        nombre[0] = '\0';
    }

    Producto(int c, const char n[], float p, int e) : codigo(c), precio(p), existencia(e) {
        strcpy(nombre, n);
    }

    // Setters
    void setCodigo(int c) { codigo = c; }
    void setNombre(const char n[]) { strcpy(nombre, n); }
    void setPrecio(float p) { precio = p; }
    void setExistencia(int e) { existencia = e; }

    // Getters
    int getCodigo() const { return codigo; }
    char* getNombre() { return nombre; }
    float getPrecio() const { return precio; }
    int getExistencia() const { return existencia; }

    void actualizarCantidad(int cant) { //actualiza la cantidad
        if (cant <= existencia)
            existencia -= cant;
        else
            cout << "No hay suficiente inventario\n";
    }

    void mostrar(ostream& out) const { //muestra
        out << "Codigo: " << codigo
            << " | Nombre: " << nombre
            << " | Precio: $" << precio
            << " | Existencia: " << existencia;
    }

    friend ostream& operator<<(ostream& out, const Producto& p) { //sobrecarga de operadores
        p.mostrar(out);
        return out;
    }
};

#endif
