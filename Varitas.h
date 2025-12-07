#ifndef VARITAS_H
#define VARITAS_H

#include <iostream>
#include <cstring>
using namespace std;

class Varitas {
private:
    int codigo;
    char nombre[30];
    float precio;
    int existencia;

public:
    Varitas();
    Varitas(int, const char[], float, int);
    Varitas(const Varitas&);
    void operator=(const Varitas&);

    // Setters y getters
    void setCodigo(int);
    void setNombre(const char[]);
    void setPrecio(float);
    void setExistencia(int);

    int getCodigo() const;
    char* getNombre();
    float getPrecio() const;
    int getExistencia() const;

    // Sobrecarga de operadores
    bool operator==(const Varitas&) const;
    Varitas& operator+=(int);
    Varitas& operator-=(int);

    friend istream& operator>>(istream&, Varitas&);
    friend ostream& operator<<(ostream&, const Varitas&);
};

inline Varitas::Varitas() {
    codigo = 0;
    nombre[0] = '\0';
    precio = 0.0;
    existencia = 0;
}

inline Varitas::Varitas(int c, const char n[], float p, int e) {
    codigo = c;
    strcpy(nombre, n);
    precio = p;
    existencia = e;
}

inline Varitas::Varitas(const Varitas& v) {
    codigo = v.codigo;
    strcpy(nombre, v.nombre);
    precio = v.precio;
    existencia = v.existencia;
}

inline void Varitas::operator=(const Varitas& v) {
    if (this != &v) {
        codigo = v.codigo;
        strcpy(nombre, v.nombre);
        precio = v.precio;
        existencia = v.existencia;
    }
}

// Setters
inline void Varitas::setCodigo(int c) { codigo = c; }
inline void Varitas::setNombre(const char n[]) { strcpy(nombre, n); }
inline void Varitas::setPrecio(float p) { precio = p; }
inline void Varitas::setExistencia(int e) { existencia = e; }

// Getters
inline int Varitas::getCodigo() const { return codigo; }
inline char* Varitas::getNombre() { return nombre; }
inline float Varitas::getPrecio() const { return precio; }
inline int Varitas::getExistencia() const { return existencia; }

// Operadores
inline bool Varitas::operator==(const Varitas& v) const {
    return codigo == v.codigo;
}

inline Varitas& Varitas::operator+=(int cant) {
    existencia += cant;
    return *this;
}

inline Varitas& Varitas::operator-=(int cant) {
    existencia -= cant;
    if (existencia < 0) existencia = 0;
    return *this;
}

// Sobrecarga de entrada
inline istream& operator>>(istream& in, Varitas& v) {
    cout << "Codigo: ";
    in >> v.codigo;
    cout << "Nombre: ";
    in.ignore();
    in.getline(v.nombre, 30);
    cout << "Precio: ";
    in >> v.precio;
    cout << "Existencia: ";
    in >> v.existencia;
    return in;
}

// Sobrecarga de salida
inline ostream& operator<<(ostream& out, const Varitas& v) {
    out << "Codigo: " << v.codigo
        << " | Nombre: " << v.nombre
        << " | Precio: $" << v.precio
        << " | Existencia: " << v.existencia;
    return out;
}

#endif
