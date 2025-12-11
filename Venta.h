#ifndef VENTA_H
#define VENTA_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
using namespace std;

class DetalleVenta {
private:
    int codigo;
    char nombre[50];
    int cantidad;
    float precio;
    float subtotal;
   

public:
    DetalleVenta() {
        codigo=0;
        nombre[0]='\0';
        cantidad=0;
        precio=0;
        subtotal = 0;
    }

    void setDatos(int c, const char n[],float p, int cant){
        codigo=c;
        strcpy(nombre,n);
        precio=p;
        cantidad=cant;
        subtotal=precio * cantidad;
    }

    int getCodigo() const {return codigo; }
    const char* getNombre() const {return nombre;}
    int getCantidad() const {return cantidad; }
    int getPrecio() const {return precio;}
    float getSubtotal() const {return subtotal; }

    void imprimir (){
        cout<<codigo<< "|"
            <<nombre<< "| Cantidad: "<<cantidad
            <<"|$" <<precio<< "c/u | Subtotal: $" <<subtotal<<"\n";
    }
};


class Venta {
private:
    int folio;
    static int ultimoFolio;
    char fecha[25];
    char hora[25];

    DetalleVenta detalles[50];
    int numDetalles;

    float subtotal;
    float iva;
    float total;

public:
    Venta(){
        folio=0;
        numDetalles=0;
        subtotal= iva= total=0;
        fecha[0]= hora[0]='\0';
    }

    
    const char* getFecha() const {return fecha;}
    const char* getHora()const{return hora;}
    DetalleVenta* getDetalles() {return detalles;}

    void generarFolio(){ folio=++ultimoFolio;}
    int getFolio()const{ return folio;}
    void setFolio(int f) { folio=f;}

    void generarFechaHora() {
        time_t now = time(0);
        tm *t = localtime(&now);

        // Fecha simple tipo 12/02/2025
        fecha[0] = (t->tm_mday / 10) + '0';
        fecha[1] = (t->tm_mday % 10) + '0';
        fecha[2] = '/';
        fecha[3] = ((t->tm_mon + 1) / 10) + '0';
        fecha[4] = ((t->tm_mon + 1) % 10) + '0';
        fecha[5] = '/';
        fecha[6] = (t->tm_year + 1900) / 1000 % 10 + '0';
        fecha[7] = (t->tm_year + 1900) / 100 % 10 + '0';
        fecha[8] = (t->tm_year + 1900) / 10 % 10 + '0';
        fecha[9] = (t->tm_year + 1900) % 10 + '0';
        fecha[10] = '\0';

        // Hora tipo 14:08:55
        hora[0] = (t->tm_hour / 10) + '0';
        hora[1] = (t->tm_hour % 10) + '0';
        hora[2] = ':';
        hora[3] = (t->tm_min / 10) + '0';
        hora[4] = (t->tm_min % 10) + '0';
        hora[5] = ':';
        hora[6] = (t->tm_sec / 10) + '0';
        hora[7] = (t->tm_sec % 10) + '0';
        hora[8] = '\0';
    }

    bool agregarDetalle (int cod, const char nom[], float precio, int cant){
        if(numDetalles >=50){ return false;}
        if(cant<=0){ return false;}

        detalles[numDetalles].setDatos(cod, nom, precio, cant);
        numDetalles++;
        return true;
    }
    
    void calcularTotales() {
        subtotal = 0;
        for(int i=0;  i<numDetalles; i++)
            subtotal += detalles[i].getSubtotal();
        iva = subtotal * 0.16;
        total = subtotal + iva;
    }
    float getTotal() const { return total; }
    float getSubtotal() const { return subtotal; }
    int getNumDetalles() const { return numDetalles; } 

    void imprimirTicket() {
        cout << "\n=====================================\n";
        cout << "           TICKET DE COMPRA\n";
        cout << "=====================================\n";
        cout << "Fecha: " << fecha << "\n";
        cout << "Hora:  " << hora << "\n";
        for (int i=0; i<numDetalles; i++)
        detalles[i].imprimir();
        cout << "-------------------------------------\n";
        cout << "Subtotal: $" << subtotal << "\n";
        cout << "IVA (16%): $" << iva << "\n";
        cout << "TOTAL A PAGAR: $" << total << "\n";
        cout << "-------------------------------------\n";
        cout << "Gracias por su compra :) \n";
        cout << "=====================================\n\n";
    }

    void guardarEnArchivo(ofstream &out) {
        out.write((char*)this,sizeof(*this));
    }

    void leerDeArchivo(ifstream&in){
        in.read((char*)this, sizeof(*this));
    }
};
int Venta::ultimoFolio=0;

#endif
