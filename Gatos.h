#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Gasto {
private:
//atributos
    char fecha[11];     
    char concepto[50];
    float monto;

public:
    //constructor
    Gasto() {
        fecha[0]= '\0';
        concepto[0] = '\0';
        monto = 0;
    }
    //setter
    void setDatos(const char f[], const char c[], float m) {
        strcpy(fecha, f);
        strcpy(concepto, c);
        monto = m;
    }
    //getters
    const char* getFecha() const { return fecha; }
    const char* getConcepto() const { return concepto; }
    float getMonto() const { return monto; }

    void imprimir() const { //imprime
        cout << fecha << " | " << concepto << " | $" << monto << endl;
    }
};

class ArchivoGastos { //clase de archivos
private:
    const char* nombre = "gastos.dat";

public:
    void crearArchivo() { //crea archivos
        fstream f(nombre, ios::app | ios::binary);
        f.close();
    }

    void registrarGasto() { //registra los datos
        Gasto g;
        char fecha[11], concepto[50];
        float monto;
        //pide los datos
        cout << "Fecha (dd/mm/aaaa): ";
        cin >> fecha;
        cin.ignore();
        cout << "Concepto: ";
        cin.getline(concepto, 50);
        cout << "Monto: ";
        cin >> monto;

        g.setDatos(fecha, concepto, monto); //guarda datos

        ofstream out(nombre, ios::binary | ios::app);
        out.write((char*)&g, sizeof(Gasto)); //los escribe en el arhivo
        out.close();

        cout << "Gasto registrado correctamente.\n";
    }

    float totalGastosPorDia(const char* fechaBuscada) { //total de gastos
        ifstream in(nombre, ios::binary);
        Gasto g;
        float total = 0;

        while (in.read((char*)&g, sizeof(Gasto))) {
            if (strcmp(g.getFecha(), fechaBuscada) == 0) { //busca la fecha
                total += g.getMonto(); //suma los gastos
            }
        }

        in.close();
        return total;
    }
};
