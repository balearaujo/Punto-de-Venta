#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Proveedor{
    private:
    int id;
    char nombre[50];
    char telefono[20];

    public:
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

    void setId(int i){id=i;}
    void setNombre(char n[]){strcpy(nombre, n);}
    void setTelefono(char t[]){strcpy(telefono, t);}

    int getId() {return id;}
    char* getNombre() {return nombre;}
    char* getTelefono(){return telefono;}

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

class ArchivoProveedores {
private:
    const char* nombreArchivo = "proveedores.dat";

public:
    void crearArchivo() {
        fstream archivo(nombreArchivo, ios::app | ios::binary);
        if(!archivo) cout << "No se pudo crear el archivo.\n";
        archivo.close();
    }

    void agregarProveedor() {
        ofstream archivo(nombreArchivo, ios::app | ios::binary);
        if(!archivo) { cout << "No se puede abrir el archivo.\n"; return; }

        Proveedor p;
        cin >> p;
        archivo.write(reinterpret_cast<char*>(&p), sizeof(Proveedor));
        cout << "Proveedor agregado.\n";
        archivo.close();
    }

    void mostrarProveedores() {
        ifstream archivo(nombreArchivo, ios::binary);
        if(!archivo) { cout << "No se pudo abrir el archivo.\n"; return; }

        Proveedor p;
        while(archivo.read(reinterpret_cast<char*>(&p), sizeof(Proveedor))) {
            cout << p << endl;
        }
        archivo.close();
    }

    bool buscarProveedor(int id) {
        ifstream archivo(nombreArchivo, ios::binary);
        if(!archivo) { cout << "No se pudo abrir el archivo.\n"; return false; }

        Proveedor p;
        bool encontrado = false;
        while(archivo.read(reinterpret_cast<char*>(&p), sizeof(Proveedor))) {
            if(p.getId() == id) {
                cout << "Proveedor encontrado: " << p << endl;
                encontrado = true; break;
            }
        }
        archivo.close();
        return encontrado;
    }

    void eliminarProveedor(int id) {
        ifstream archivoLectura(nombreArchivo, ios::binary);
        ofstream archivoTemp("temp.dat", ios::binary);

        Proveedor p;
        while(archivoLectura.read(reinterpret_cast<char*>(&p), sizeof(Proveedor))) {
            if(p.getId() != id)
                archivoTemp.write(reinterpret_cast<char*>(&p), sizeof(Proveedor));
        }

        archivoLectura.close();
        archivoTemp.close();
        remove(nombreArchivo);
        rename("temp.dat", nombreArchivo);

        cout << "Proveedor eliminado si existía.\n";
    }

    void historialProveedoresTxt(ArchivoProductos &ap) {
        ifstream ventas("ventas.dat", ios::binary);
        ofstream txt("historial_proveedores.txt");

        if (!ventas || !txt) {
            cout << "Error al abrir archivos\n";
            return;
        }

        struct Historial {
            int idProv;
            int productos;
            float total;
        };

        Historial hist[50];
        int n = 0;

        Venta v;

        while (ventas.read((char*)&v, sizeof(Venta))) {

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
