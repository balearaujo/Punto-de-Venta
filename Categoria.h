#include <iostream>
#include <cstring>
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

