#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <cstring>
using namespace std;


class Usuario {
protected:
    int id;
    char nombre[50];
    char usuario[50];
    char contrasenia[20];
    int tipo;
    int activo;

public:
    Usuario() {
        id=0;
        nombre[0]='\0';
        usuario[0]='\0';
        contrasenia[0]='\0';
        tipo=2;
        activo=1;
    }

    void setDatos(int _id, const char n[], const char u[], const char c[],int t){
    id= _id;
    strcpy(nombre,n);
    strcpy(usuario, u);
    strcpy(contrasenia,c);
    tipo=t;
    activo=1;
    }

    int getId() const { return id; }
    const char* getNombre() const{ return nombre; }
    const char* getUsuario() const{ return usuario; }
    const char* getContrasenia()const{return contrasenia;}
    int getTipo()const{return tipo;}
    int estaActivo()const {return activo==1;}

    void setContrasena(const char* c) { strcpy(contrasenia, c); }
    void setActivo(int a){activo=a; }

    void imprimir () const {
        cout <<"\nID: "<<id
            <<"\nNombre: "<<nombre
            <<"\nUsuario: "<<usuario
            <<"\nTipo: "<<(tipo==1? "Administrador" : "Cajero")
            <<"\nEstado: "<<(activo==1? "Activo":"Inactivo")
            <<"\n---------------------------\n";
    }
};


#endif
