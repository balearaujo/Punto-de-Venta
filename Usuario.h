#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <cstring>
using namespace std;


class Usuario { //clase de usuario
protected:
    //atributos
    int id;
    char nombre[50];
    char usuario[50];
    char contrasenia[20];
    int tipo;
    int activo;

public:
    //constructores
    Usuario() {
        id=0;
        nombre[0]='\0';
        usuario[0]='\0';
        contrasenia[0]='\0';
        tipo=2;
        activo=1;
    }

    void setDatos(int _id, const char n[], const char u[], const char c[],int t){ //setter
    id= _id;
    strcpy(nombre,n);
    strcpy(usuario, u);
    strcpy(contrasenia,c);
    tipo=t;
    activo=1;
    }
    //getter
    int getId() const { return id; }
    const char* getNombre() const{ return nombre; }
    const char* getUsuario() const{ return usuario; }
    const char* getContrasenia()const{return contrasenia;}
    int getTipo()const{return tipo;}
    int estaActivo()const {return activo==1;}
    //setters
    void setContrasena(const char* c) { strcpy(contrasenia, c); }
    void setActivo(int a){activo=a; }

    void imprimir () const { //impresion
        cout <<"\nID: "<<id
            <<"\nNombre: "<<nombre
            <<"\nUsuario: "<<usuario
            <<"\nTipo: "<<(tipo==1? "Administrador" : "Cajero")
            <<"\nEstado: "<<(activo==1? "Activo":"Inactivo")
            <<"\n---------------------------\n";
    }
};

//herencia multiple
class Administrador:public Usuario{ //usuario de tipo administrador
    public: 
    Administrador(const Usuario& u){
        setDatos(u.getId(), u.getNombre(), u.getUsuario(), u.getContrasenia(), 1);
    }
};

class Cajero:public Usuario{ //usuario de tipo cajero
    public: 
    Cajero(const Usuario& u){
        setDatos(u.getId(), u.getNombre(), u.getUsuario(), u.getContrasenia(), 2);
    }
};



#endif
