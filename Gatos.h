#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Gasto {
private:
    char fecha[11];     
    char concepto[50];
    float monto;

public:
    Gasto() {
        fecha[0]= '\0';
        concepto[0] = '\0';
        monto = 0;
    }

    void setDatos(const char f[], const char c[], float m) {
        strcpy(fecha, f);
        strcpy(concepto, c);
        monto = m;
    }

    const char* getFecha() const { return fecha; }
    const char* getConcepto() const { return concepto; }
    float getMonto() const { return monto; }

    void imprimir() const {
        cout << fecha << " | " << concepto << " | $" << monto << endl;
    }
};

class ArchivoGastos {
private:
    const char* nombre = "gastos.dat";

public:
    void crearArchivo() {
        fstream f(nombre, ios::app | ios::binary);
        f.close();
    }

    void registrarGasto() {
        Gasto g;
        char fecha[11], concepto[50];
        float monto;

        cout << "Fecha (dd/mm/aaaa): ";
        cin >> fecha;
        cin.ignore();
        cout << "Concepto: ";
        cin.getline(concepto, 50);
        cout << "Monto: ";
        cin >> monto;

        g.setDatos(fecha, concepto, monto);

        ofstream out(nombre, ios::binary | ios::app);
        out.write((char*)&g, sizeof(Gasto));
        out.close();

        cout << "Gasto registrado correctamente.\n";
    }

    float totalGastosPorDia(const char* fechaBuscada) {
        ifstream in(nombre, ios::binary);
        Gasto g;
        float total = 0;

        while (in.read((char*)&g, sizeof(Gasto))) {
            if (strcmp(g.getFecha(), fechaBuscada) == 0) {
                total += g.getMonto();
            }
        }

        in.close();
        return total;
    }
};
