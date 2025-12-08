#ifndef VARITAS_H
#define VARITAS_H
#include <limits>
#include <iostream>
#include <cstring>
using namespace std;

class Varitas {
private:
    int codigo;
    char nombre[30];
    float precio;
    int existencia;
    char categoria[30];

public:
    Varitas();
    Varitas(int, const char[], float, int, const char[]);
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

    void setCategoria(const char cat[]){ strcpy(categoria, cat); }
    char* getCategoria(){ return categoria; }
};

inline Varitas::Varitas() {
    codigo = 0;
    strcpy(nombre, "");
    precio = 0.0;
    existencia = 0;
    strcpy(categoria, "");
}

inline Varitas::Varitas(int c, const char n[], float p, int e, const char cat[]) {
    codigo = c;
    strcpy(nombre, n);
    precio = p;
    existencia = e;
    strcpy(categoria, cat);
}

inline Varitas::Varitas(const Varitas& v) {
    codigo = v.codigo;
    strcpy(nombre, v.nombre);
    precio = v.precio;
    existencia = v.existencia;
    strcpy(categoria, v.categoria);
}

inline void Varitas::operator=(const Varitas& v) {
    if (this != &v) {
        codigo = v.codigo;
        strcpy(nombre, v.nombre);
        precio = v.precio;
        existencia = v.existencia;
        strcpy(categoria, v.categoria);
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
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    in.getline(v.nombre, 30);
    cout << "Precio: ";
    in >> v.precio;
    cout << "Existencia: ";
    in >> v.existencia;
    cout << "Categoria: ";
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    in.getline(v.categoria, 30);
    return in;
}

// Sobrecarga de salida
inline ostream& operator<<(ostream& out, const Varitas& v) {
    out << "Codigo: " << v.codigo
        << " | Nombre: " << v.nombre
        << " | Precio: $" << v.precio
        << " | Existencia: " << v.existencia<< " | Categoria: " << v.categoria;
    return out;
}

#endif
