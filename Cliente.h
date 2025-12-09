#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Cliente {
private:
    int id;
    char nombre[50];
    char direccion[100];
    char telefono[15];

public:
    Cliente(){
        id=0;
        strcpy(nombre, "");
        strcpy(direccion, "");
        strcpy(telefono, "");
    }

    Cliente(int i, const char n[], const char d[], const char t[]){
        id=i;
        strcpy(nombre, n);
        strcpy(direccion, d);
        strcpy(telefono, t);
    }

    Cliente(const Cliente& c){
        id=c.id;
        strcpy(nombre, c.nombre);
        strcpy(direccion, c.direccion); 
        strcpy(telefono, c.telefono);
    }

    void operator=(const Cliente& c){
        if (this!=& c) {
            id=c.id;
            strcpy(nombre, c.nombre);
            strcpy(direccion, c.direccion); 
            strcpy(telefono, c.telefono);
        }
    }

    // Setters
    void setId(int i){ id=i; }
    void setNombre(const char n[]){ strcpy(nombre, n); }
    void setDireccion(const char d[]){ strcpy(direccion, d); }
    void setTelefono(const char t[]){ strcpy(telefono, t); }

    // Getters
    int getId() const{ return id; }
    char* getNombre(){ return nombre; }
    char* getDireccion(){ return direccion; }
    char* getTelefono(){ return telefono; }

    virtual void mostrarCliente() { //metodo virtual para polimorfismo
        cout << "ID: " << id << "\nNombre: " << nombre << "\nDireccion: " << direccion << "\nTelefono: " << telefono << endl;
    }
};

class ClienteFrecuente : public Cliente {
    int puntosAcumulados;
public:
    ClienteFrecuente() : Cliente() {
        puntosAcumulados=0;
    }

    ClienteFrecuente(int i, const char n[], const char d[], const char t[], int puntos) 
        : Cliente(i, n, d, t) {
        puntosAcumulados=puntos;
    }

    void setPuntosAcumulados(int puntos) {
        puntosAcumulados= puntos;
    }

    int getPuntosAcumulados() const {
        return puntosAcumulados;
    }

    void mostrarCliente() override { //Ejemplo de polimorfismo
        Cliente::mostrarCliente();
        cout<<"Puntos Acumulados: "<< puntosAcumulados << endl;
    }
};

class ArchivoClientes {
private:
    const char* nombreArchivo="clientes.dat";
public:
    void agregarCliente();
    void mostrarClientes();
    int buscarCliente(int id);
    int contarClientes();
    void modificarCliente(int id);
};

void ArchivoClientes::agregarCliente(){
    ofstream archivo(nombreArchivo, ios::app | ios::binary);
    if(!archivo){
        cout<<"No se puede abrir el archivo.\n";
        return;
    }
    Cliente c;
    cout<<"Ingresa el ID del cliente (-1 para terminar): ";
    int id; cin>>id;
    while(id!=-1){
        c.setId(id);
        char nombre[50];
        cout<<"Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.getline(nombre,50); c.setNombre(nombre);
        char direccion[100];
        cout<<"Direccion: "; cin.getline(direccion,100); c.setDireccion(direccion);
        char telefono[15];
        cout<<"Telefono: "; cin.getline(telefono,15); c.setTelefono(telefono);
        archivo.write(reinterpret_cast<char*>(&c), sizeof(Cliente));
        cout<<"\nCliente agregado.\n";
        cout<<"\nNuevo ID (-1 para terminar): "; cin>>id;
    }
    archivo.close();
}

void ArchivoClientes::mostrarClientes(){
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return;
    }
    Cliente c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){
        c.mostrarCliente();
        cout << "-----------------------" << endl;
    }
    archivo.close();
}

int ArchivoClientes::buscarCliente(int id){
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return -1;
    }
    Cliente c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){
        if(c.getId()==id){
            cout<<"\n=== CLIENTE ENCONTRADO ===\n";
            c.mostrarCliente();
            archivo.close();
            return 1;
        }
    }
    cout<<"\nCliente con ID "<<id<<" no encontrado.\n";
    archivo.close();
    return 0;
}

int ArchivoClientes::contarClientes(){
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return 0;
    }
    archivo.seekg(0, ios::end);
    int size = archivo.tellg();
    archivo.close();
    return size / sizeof(Cliente);
}

void ArchivoClientes::modificarCliente(int id){
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if(!archivo){
        cout<<"No se puede abrir el archivo.\n";
        return;
    }
    Cliente c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){
        if(c.getId()==id){
            cout<<"Cliente encontrado:\n";
            c.mostrarCliente();
            char nombre[50];
            cout<<"Nuevo nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.getline(nombre,50); c.setNombre(nombre);
            char direccion[100];
            cout<<"Nueva direccion: "; cin.getline(direccion,100); c.setDireccion(direccion);
            char telefono[15];
            cout<<"Nuevo telefono: "; cin.getline(telefono,15); c.setTelefono(telefono);
            archivo.seekp(-static_cast<int>(sizeof(Cliente)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&c), sizeof(Cliente));
            cout<<"Cliente modificado.\n";
            archivo.close();
            return;
        }
    }
    cout<<"Cliente con ID "<<id<<" no encontrado.\n";
    archivo.close();
}