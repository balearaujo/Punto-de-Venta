#ifndef ARCHIVOUSUARIOS_H
#define ARCHIVOUSUARIOS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "Usuario.h"
using namespace std;

class ArchivoUsuarios { //clase para usuarios
private:
    const char* nombreArchivo = "usuarios.dat"; //archivo

public:

    void crearArchivo() { //crea el archivo
        fstream archivo(nombreArchivo, ios::binary | ios::in);

        if (archivo.good()) {
            archivo.close();
            return; // ya existe
        }

        archivo.close();
        ofstream out(nombreArchivo, ios::binary);

        Usuario admin;
        admin.setDatos(1, "ADMINISTRADOR", "admin", "admin123", 1); //usuario por defecto
        out.write(reinterpret_cast<char*>(&admin), sizeof(Usuario));

        out.close();

        cout << "\nArchivo de usuarios creado. Usuario admin por defecto:\n";
        cout << "Usuario: admin | Contrasenia: admin123\n";
    }

    Usuario* login() { //login del usuario
        static Usuario u;
        ifstream archivo(nombreArchivo, ios::binary);

        if (!archivo) {
            cout << "ERROR: No existe el archivo de usuarios.\n";
            return nullptr;
        }
        //escribe usuario y contraseña
        char user[50], pass[20];
        cout << "\nUsuario: ";
        cin >> user;
        cout << "Contrasena: ";
        cin >> pass;

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) { //lee el archivo
            if (strcmp(u.getUsuario(), user) == 0 &&
                strcmp(u.getContrasenia(), pass) == 0 && 
                u.estaActivo()) { //revisa q las claves sean correctas y que este activo

                archivo.close();
                return &u;
            }
        }

        archivo.close();
        return nullptr;
    }

    void agregarUsuario() { //agrega el usuario
        ofstream archivo(nombreArchivo, ios::app | ios::binary);
        if (!archivo) {
            cout << "No se pudo abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        int id, tipo;
        char nombre[50], user[50], pass[20];
        //ingresa los datos del usuario
        cout << "\nID del usuario: ";
        cin >> id;
        cin.ignore();
        cout << "Nombre: "; cin.getline(nombre, 50);
        cout << "Usuario: "; cin.getline(user, 50);
        cout << "Contrasena: "; cin.getline(pass, 20);
        cout << "Tipo (1=Admin, 2=Cajero): ";
        cin >> tipo;

        u.setDatos(id, nombre, user, pass, tipo); //los pone en usuario

        archivo.write(reinterpret_cast<char*>(&u), sizeof(Usuario));
        archivo.close();

        cout << "Usuario agregado correctamente.\n";
    }

    void mostrarUsuarios() { //muestra los usuarios
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo) {
            cout << "No se puede abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        cout << "\n=== LISTA DE USUARIOS ===\n";

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) { //recorre el archivo
            u.imprimir(); //imprime
        }

        archivo.close();
    }

    bool buscarUsuario(int id, Usuario& outUser, long& pos) { //busca los usuarios
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo) return false;

        Usuario u;
        pos = 0;

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) { //lee archivo
            if (u.getId() == id) { //compara id
                outUser = u;
                archivo.close();
                return true;
            }
            pos += sizeof(Usuario);
        }

        archivo.close();
        return false;
    }

    void desactivarUsuario(int id) { //desactiva un usuario
        fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);

        if (!archivo) {
            cout << "No se puede abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        long pos = archivo.tellg();

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            if (u.getId() == id) {
                u.setActivo(0); //cambia su estado

                archivo.seekp(pos); //busca posicion
                archivo.write(reinterpret_cast<char*>(&u), sizeof(Usuario));

                cout << "Usuario desactivado.\n";
                archivo.close();
                return;
            }

            pos = archivo.tellg();
        }

        cout << "No se encontró el usuario.\n";
        archivo.close();
    }

    void cambiarContrasena(int id) { //cambia la contraseña
        fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);

        if (!archivo) {
            cout << "No se puede abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        long pos = archivo.tellg();

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            if (u.getId() == id) { //compara ids
                //rescribe contraseña
                char pass[20];
                cout << "Nueva contraseña: ";
                cin >> pass;

                u.setContrasena(pass); //guarda contraseña

                archivo.seekp(pos);
                archivo.write(reinterpret_cast<char*>(&u), sizeof(Usuario)); //escibe en el archivo

                cout << "Contrasenia actualizada\n";
                archivo.close();
                return;
            }
            pos = archivo.tellg();
        }

        cout << "Usuario no encontrado\n";
        archivo.close();
    }

};

#endif
