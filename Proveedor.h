#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Proveedor{
    //atributos
    private:
    int id;
    char nombre[50];
    char telefono[20];

    public:
    //constructores
    Proveedor(){
        id=0;
        strcpy(nombre, "");
        strcpy(telefono, "");
    }

    Proveedor(int i, char n[], char t[]){
        id=i;
        strcpy(nombre, n);
        strcpy(telefono, t);
    }

    Proveedor(const Proveedor &p){
        id=p.id;
        strcpy(nombre, p.nombre);
        strcpy(telefono, p.telefono);
    }
    //setters
    void setId(int i){id=i;}
    void setNombre(char n[]){strcpy(nombre, n);}
    void setTelefono(char t[]){strcpy(telefono, t);}

    //getters
    int getId() {return id;}
    char* getNombre() {return nombre;}
    char* getTelefono(){return telefono;}
    //sobrecarga de operador
    friend istream& operator>>(istream& in, Proveedor& p) {
        cout << "ID: "; in >> p.id;
        cout << "Nombre: "; in.ignore(); in.getline(p.nombre, 50);
        cout << "Telefono: "; in.getline(p.telefono, 20);
        return in;
    }
    // Sobrecarga de salida
    friend ostream& operator<<(ostream& out, const Proveedor& p) {
        out << "ID: " << p.id
            << " | Nombre: " << p.nombre
            << " | Telefono: " << p.telefono;
        return out;
    } 
};

class ArchivoProveedores { //clase de archivo
private:
    const char* nombreArchivo = "proveedores.dat";

public:
    void crearArchivo() { //crea archivo
        fstream archivo(nombreArchivo, ios::app | ios::binary);
        if(!archivo) cout << "No se pudo crear el archivo.\n"; //valida apertura 
        archivo.close();
    }

    void agregarProveedor() { //agrega proveedor
        ofstream archivo(nombreArchivo, ios::app | ios::binary);
        if(!archivo) { cout << "No se puede abrir el archivo.\n"; return; }

        Proveedor p;
        cin >> p;
        archivo.write(reinterpret_cast<char*>(&p), sizeof(Proveedor)); //escribe el archivo
        cout << "Proveedor agregado.\n";
        archivo.close();
    }

    void mostrarProveedores() { //muestra los provedores
        ifstream archivo(nombreArchivo, ios::binary);
        if(!archivo) { cout << "No se pudo abrir el archivo.\n"; return; }

        Proveedor p;
        while(archivo.read(reinterpret_cast<char*>(&p), sizeof(Proveedor))) { //recorre archivp
            cout << p << endl; //imprime
        }
        archivo.close();
    }

    bool buscarProveedor(int id) { //busca proveedor
        ifstream archivo(nombreArchivo, ios::binary);
        if(!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; }

        Proveedor p;
        bool encontrado = false;
        while(archivo.read(reinterpret_cast<char*>(&p), sizeof(Proveedor))) { //recorre el archivo
            if(p.getId() == id) { //compara el id
                cout << "Proveedor encontrado: " << p << endl;
                encontrado = true; break; //revisa si fue encontrado
            }
        }
        archivo.close();
        return encontrado;
    }

    void eliminarProveedor(int id) { //se elimina el proveedor
        ifstream archivoLectura(nombreArchivo, ios::binary);
        ofstream archivoTemp("temp.dat", ios::binary); //archivo temporal

        Proveedor p;
        while(archivoLectura.read(reinterpret_cast<char*>(&p), sizeof(Proveedor))) {
            if(p.getId() != id)
                archivoTemp.write(reinterpret_cast<char*>(&p), sizeof(Proveedor)); //sino es el id a borrar se reescribe
        }

        archivoLectura.close();
        archivoTemp.close();
        remove(nombreArchivo); //se borra el archivo principal
        rename("temp.dat", nombreArchivo); //se renombra el temporal

        cout << "Proveedor eliminado si existía.\n";
    }

    void historialProveedoresTxt(ArchivoProductos &ap) { //historial de proveedores
        ifstream ventas("ventas.dat", ios::binary);
        ofstream txt("historial_proveedores.txt"); //archivo de texto

        if (!ventas || !txt) {
            cout << "Error al abrir archivos\n";
            return;
        }

        struct Historial { //campos del historial
            int idProv;
            int productos;
            float total;
        };

        Historial hist[50]; //arreglo con el historial
        int n = 0;

        Venta v;

        while (ventas.read((char*)&v, sizeof(Venta))) { //lee las ventas

            DetalleVenta* d = v.getDetalles();

            for (int i = 0; i < v.getNumDetalles(); i++) {

                Varitas p;
                long pos;

                if (!ap.obtenerProducto(d[i].getCodigo(), p, pos))
                    continue;

                int idProv = p.getIdProveedor();
                float subtotal = d[i].getSubtotal();

                bool encontrado = false;

                for (int j = 0; j < n; j++) {
                    if (hist[j].idProv == idProv) {
                        hist[j].productos += d[i].getCantidad();
                        hist[j].total += subtotal;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado) {
                    hist[n].idProv = idProv;
                    hist[n].productos = d[i].getCantidad();
                    hist[n].total = subtotal;
                    n++;
                }
            }
        }

        txt << "===== HISTORIAL DE PROVEEDORES =====\n\n";
        //imprime historial de proveedores
        for (int i = 0; i < n; i++) {
            txt << "Proveedor ID: " << hist[i].idProv << "\n";
            txt << "Productos vendidos: " << hist[i].productos << "\n";
            txt << "Total generado: $" << hist[i].total << "\n";
            txt << "-----------------------------\n";
        }

        ventas.close();
        txt.close();

        cout << "Historial de proveedores generado ✔\n";
    }

};
