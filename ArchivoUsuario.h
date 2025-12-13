#ifndef ARCHIVOUSUARIOS_H
#define ARCHIVOUSUARIOS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "Usuario.h"
using namespace std;

class ArchivoUsuarios {
private:
    const char* nombreArchivo = "usuarios.dat";

public:

    void crearArchivo() {
        fstream archivo(nombreArchivo, ios::binary | ios::in);

        if (archivo.good()) {
            archivo.close();
            return; // ya existe
        }

        archivo.close();
        ofstream out(nombreArchivo, ios::binary);

        Usuario admin;
        admin.setDatos(1, "ADMINISTRADOR", "admin", "admin123", 1);
        out.write(reinterpret_cast<char*>(&admin), sizeof(Usuario));

        out.close();

        cout << "\nArchivo de usuarios creado. Usuario admin por defecto:\n";
        cout << "Usuario: admin | Contrasenia: admin123\n";
    }

    Usuario* login() {
        static Usuario u;
        ifstream archivo(nombreArchivo, ios::binary);

        if (!archivo) {
            cout << "ERROR: No existe el archivo de usuarios.\n";
            return nullptr;
        }

        char user[50], pass[20];
        cout << "\nUsuario: ";
        cin >> user;
        cout << "Contrasena: ";
        cin >> pass;

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            if (strcmp(u.getUsuario(), user) == 0 &&
                strcmp(u.getContrasenia(), pass) == 0 &&
                u.estaActivo()) {

                archivo.close();
                return &u;
            }
        }

        archivo.close();
        return nullptr;
    }

    // ------------------------------------------------------
    //  AGREGAR USUARIO
    // ------------------------------------------------------
    void agregarUsuario() {
        ofstream archivo(nombreArchivo, ios::app | ios::binary);
        if (!archivo) {
            cout << "No se pudo abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        int id, tipo;
        char nombre[50], user[50], pass[20];

        cout << "\nID del usuario: ";
        cin >> id;
        cin.ignore();
        cout << "Nombre: "; cin.getline(nombre, 50);
        cout << "Usuario: "; cin.getline(user, 50);
        cout << "Contrasena: "; cin.getline(pass, 20);
        cout << "Tipo (1=Admin, 2=Cajero): ";
        cin >> tipo;

        u.setDatos(id, nombre, user, pass, tipo);

        archivo.write(reinterpret_cast<char*>(&u), sizeof(Usuario));
        archivo.close();

        cout << "Usuario agregado correctamente.\n";
    }

    // ------------------------------------------------------
    //  MOSTRAR TODOS
    // ------------------------------------------------------
    void mostrarUsuarios() {
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo) {
            cout << "No se puede abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        cout << "\n=== LISTA DE USUARIOS ===\n";

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            u.imprimir();
        }

        archivo.close();
    }

    // ------------------------------------------------------
    //  BUSCAR USUARIO
    // ------------------------------------------------------
    bool buscarUsuario(int id, Usuario& outUser, long& pos) {
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo) return false;

        Usuario u;
        pos = 0;

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            if (u.getId() == id) {
                outUser = u;
                archivo.close();
                return true;
            }
            pos += sizeof(Usuario);
        }

        archivo.close();
        return false;
    }

    // ------------------------------------------------------
    //  DESACTIVAR USUARIO
    // ------------------------------------------------------
    void desactivarUsuario(int id) {
        fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);

        if (!archivo) {
            cout << "No se puede abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        long pos = archivo.tellg();

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            if (u.getId() == id) {
                u.setActivo(0);

                archivo.seekp(pos);
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

    // ------------------------------------------------------
    //  CAMBIAR CONTRASEÑA
    // ------------------------------------------------------
    void cambiarContrasena(int id) {
        fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);

        if (!archivo) {
            cout << "No se puede abrir usuarios.dat\n";
            return;
        }

        Usuario u;
        long pos = archivo.tellg();

        while (archivo.read(reinterpret_cast<char*>(&u), sizeof(Usuario))) {
            if (u.getId() == id) {

                char pass[20];
                cout << "Nueva contraseña: ";
                cin >> pass;

                u.setContrasena(pass);

                archivo.seekp(pos);
                archivo.write(reinterpret_cast<char*>(&u), sizeof(Usuario));

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
