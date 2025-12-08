#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Categoria {
private:
    int id;
    char nombre[30];

public:
    Categoria(){
        id=0;
        nombre[0]='\0';
    };
    Categoria(int i, const char n[]){
        id=i;
        strcpy(nombre, n);
    }
    

    // Setters 
    void setId(int i){
        id=i;
    }
    void setNombre(const char n[]){
        strcpy(nombre, n);
    }

    // Getters
    int getId() const{
        return id;
    }
    char* getNombre(){
        return nombre;
    }

    // Sobrecarga 
    friend ostream& operator<<(ostream& out, const Categoria& c){
        out<<"ID: "<< c.id<< " Nombre: " << c.nombre;
        return out;
    }
};

class ArchivoCategorias{
private:
    const char* nombreArchivo = "categorias.dat";

    public:
    void agregarCategoria();
    void mostrarCategorias();
};

void ArchivoCategorias::agregarCategoria(){
    ofstream archivo(nombreArchivo, ios::app | ios::binary);
    if(!archivo){
        cout<<"No se puede abrir el archivo.\n";
        return;
    }

    cout<<"Ingresa el ID de la categoria (-1 para terminar): ";
    int id; cin>>id;

    while(id != -1){
        Categoria c; c.setId(id);

        char nombre[30];
        cout<<"Nombre: "; cin.ignore(); cin.getline(nombre,30); c.setNombre(nombre);

        archivo.write(reinterpret_cast<char*>(&c), sizeof(Categoria));
        cout<<"\nCategoria agregada.\n";
        cout<<"\nNuevo ID (-1 para terminar): "; cin>>id;
    }
    archivo.close();
}

void ArchivoCategorias::mostrarCategorias(){
    ifstream archivo(nombreArchivo, ios::binary);
    if(!archivo){
        cout<<"No se pudo abrir el archivo.\n";
        return;
    }

    Categoria c;
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Categoria))){
        cout<<c<<endl;
    }

    archivo.close();
}

