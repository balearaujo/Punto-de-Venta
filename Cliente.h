#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Cliente { //clase cliente
private:
    //atributos
    int id;
    char nombre[50];
    char direccion[100];
    char telefono[15];
    int puntos;

public:
    //constructores
    Cliente(){
        id=0;
        strcpy(nombre, "");
        strcpy(direccion, "");
        strcpy(telefono, "");
        puntos=0;
    }

    Cliente(int i, const char n[], const char d[], const char t[]){
        id=i;
        strcpy(nombre, n);
        strcpy(direccion, d);
        strcpy(telefono, t);
        puntos=0;
    }

    Cliente(const Cliente& c){
        id=c.id;
        strcpy(nombre, c.nombre);
        strcpy(direccion, c.direccion); 
        strcpy(telefono, c.telefono);
    }

    void operator=(const Cliente& c){ //sonbrecarga de operadores
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
    void setPuntos(int p) { puntos=p;}
    // Getters
    int getId() const{ return id; }
    char* getNombre(){ return nombre; }
    char* getDireccion(){ return direccion; }
    char* getTelefono(){ return telefono; }
    void agregarPuntos(int p){puntos +=p;}

    void mostrarCliente() { 
        cout << "ID: " << id << "\nNombre: " << nombre << "\nDireccion: " << direccion << "\nTelefono: " << telefono << "\npuntos: "<<puntos<<endl;
        if (puntos>=50) cout<<"--------Cliente frecuente--------\n";
    }
};

class ArchivoClientes { //clase de archivos
private:
    const char* nombreArchivo="clientes.dat";
public:
    //metodos
    void agregarCliente();
    void mostrarClientes();
    int buscarCliente(int id);
    int contarClientes();
    void modificarCliente(int id);
    void sumarPuntos(int idCliente, float totalCompra);
};

void ArchivoClientes::agregarCliente(){ //agrega cliente
    ofstream archivo(nombreArchivo, ios::app | ios::binary);
    if(!archivo){
        cout<<"No se puede abrir el archivo.\n";
        return;
    }
    Cliente c;
    cout<<"Ingresa el ID del cliente (-1 para terminar): ";
    int id; cin>>id;
    while(id!=-1){
        //pide los datos
        c.setId(id);
        char nombre[50];
        cout<<"Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.getline(nombre,50); c.setNombre(nombre);
        char direccion[100];
        cout<<"Direccion: "; cin.getline(direccion,100); c.setDireccion(direccion);
        char telefono[15];
        cout<<"Telefono: "; cin.getline(telefono,15); c.setTelefono(telefono);
        archivo.write(reinterpret_cast<char*>(&c), sizeof(Cliente)); //escribe en el archivo
        cout<<"\nCliente agregado.\n";
        cout<<"\nNuevo ID (-1 para terminar): "; cin>>id;
    }
    archivo.close();
}

void ArchivoClientes::mostrarClientes(){ //muestra los clientes
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return;
    }
    Cliente c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){ //leee archivo
        c.mostrarCliente(); //imprime
        cout << "-----------------------" << endl;
    }
    archivo.close();
}

int ArchivoClientes::buscarCliente(int id){ //busca cliente
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return -1;
    }
    Cliente c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){ //lee archivo
        if(c.getId()==id){ //encuentra cliente
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

int ArchivoClientes::contarClientes(){ //cuenta los clientes
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return 0;
    }
    archivo.seekg(0, ios::end);
    int size = archivo.tellg(); //tamaño
    archivo.close();
    return size / sizeof(Cliente);
}

void ArchivoClientes::modificarCliente(int id){ //modifica los clientes
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if(!archivo){
        cout<<"No se puede abrir el archivo.\n";
        return;
    }
    Cliente c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){
        if(c.getId()==id){ //busca el id
            cout<<"Cliente encontrado:\n";
            c.mostrarCliente(); //muestra datos anteriores
            //pide nuevos datos
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

void ArchivoClientes::sumarPuntos(int idCliente, float totalCompra){ //suma puntos si es frecuente
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if(!archivo) return;

    Cliente c;
    while(archivo.read((char*)&c, sizeof(Cliente))){
        if(c.getId() == idCliente){ //busca el id
            int puntosGanados = totalCompra / 10; // 1 punto por cada 10pesos
            c.agregarPuntos(puntosGanados);

            archivo.seekp(-sizeof(Cliente), ios::cur); //encuentra la posicion
            archivo.write((char*)&c, sizeof(Cliente));
            break;
        }
    }
    archivo.close();
}