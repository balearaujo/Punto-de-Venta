#ifndef VARITAS_H
#define VARITAS_H

#include "Producto.h"
#include <iostream>
#include <cstring>
using namespace std;

class Varitas : public Producto {
private:
    char categoria[30];
    int idProveedor;

public:
    Varitas() : Producto(), idProveedor(0) {
        categoria[0] = '\0';
    }

    Varitas(int c, const char n[], float p, int e, const char cat[], int prov)
        : Producto(c, n, p, e), idProveedor(prov) {
        strcpy(categoria, cat);
    }

    // Setters
    void setCategoria(const char cat[]) { strcpy(categoria, cat); }
    void setIdProveedor(int prov) { idProveedor = prov; }

    // Getters
    char* getCategoria() { return categoria; }
    int getIdProveedor() const { return idProveedor; }

    // Operadores
    Varitas& operator+=(int cant) { existencia += cant; return *this; }
    Varitas& operator-=(int cant) { existencia -= cant; if (existencia < 0) existencia = 0; return *this; }
    bool operator==(const Varitas& v) const { return codigo == v.codigo; }

    // Entrada
    friend istream& operator>>(istream& in, Varitas& v) {
        cout << "Codigo: "; in >> v.codigo;
        cout << "Nombre: "; in.ignore(); in.getline(v.nombre, 30);
        cout << "Precio: "; in >> v.precio;
        cout << "Existencia: "; in >> v.existencia;
        cout << "Categoria: "; in.ignore(); in.getline(v.categoria, 30);
        cout << "ID Proveedor: "; in >> v.idProveedor;
        return in;
    }

    // Salida
    friend ostream& operator<<(ostream& out, const Varitas& v) {
        v.mostrar(out);  // Llama a Producto::mostrar
        out << " | Categoria: " << v.categoria
            << " | Proveedor: " << v.idProveedor;
        return out;
    }
};

#endif
