#ifndef VENTA_H
#define VENTA_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
using namespace std;

class DetalleVenta { //clase con los detalles de la venta
private:
    //atributo
    int codigo;
    char nombre[50];
    int cantidad;
    float precio;
    float costo;
    float subtotal;

public:
    //cnstructores
    DetalleVenta() {
        codigo=0;
        nombre[0]='\0';
        cantidad=0;
        precio=0;
        costo=0;
        subtotal = 0;
    }
    //setters
    void setDatos(int c, const char n[],float p, float cost, int cant){
        codigo=c;
        strcpy(nombre,n);
        precio=p;
        costo=cost;
        cantidad=cant;
        subtotal=precio * cantidad;
    }
    //getters
    int getCodigo() const {return codigo; }
    const char* getNombre() const {return nombre;}
    int getCantidad() const {return cantidad; }
    int getPrecio() const {return precio;}
    float getSubtotal() const {return subtotal; }
    float getCosto() const {return costo; }
    float getGanancia() const {return (precio-costo)*cantidad;}
    
    void imprimir (){ //imprime
        cout<<codigo<< "|"
            <<nombre<< "| Cantidad: "<<cantidad
            <<"|$" <<precio<< "c/u | Subtotal: $" <<subtotal<<"\n";
    }
};


class Venta { //clase venta
private:
    //atrbutos
    int folio;
    int id_cliente;
    int id_usuario;

    static int ultimoFolio;
    char fecha[25];
    char hora[25];

    DetalleVenta detalles[50];
    int numDetalles;

    float subtotal;
    float iva;
    float total;
    char metodoPago[20];

public:
    //constructor
    Venta(){
        folio=0;
        id_cliente=0;
        id_usuario=0;
        numDetalles=0;
        subtotal= iva= total=0;
        fecha[0]= hora[0]='\0';
        metodoPago[0]='\0';
    }
    public:
    //metodos getters y setters
    void setCliente(int id) {id_cliente = id;}
    int getCliente() const {return id_cliente;}

    void setUsuario(int idu){id_usuario=idu;}
    int getUsuario()const {return id_usuario;}
    const char* getFecha() const {return fecha;}
    const char* getHora()const{return hora;}
    DetalleVenta* getDetalles() {return detalles;}
    const char* getMetodoPago() const {return metodoPago;}
    //metodos
    void generarFolio(){ folio=++ultimoFolio;}
    int getFolio()const{ return folio;}
    void setFolio(int f) { folio=f;}
    void setMetodoPago(const char mp[]){strcpy(metodoPago, mp);};

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

    bool agregarDetalle (int cod, const char nom[], float precio, float costo, int cant, int id_client){
        if(numDetalles >=50){ return false;}
        if(cant<=0){ return false;} //valida cantidades

        detalles[numDetalles].setDatos(cod, nom, precio, costo, cant); //agrega los detalles
        numDetalles++; //incrementa el numero de detalles
        id_cliente = id_client;
        return true;
    }
    
    void calcularTotales() { //calcula los totales
        subtotal = 0;
        for(int i=0;  i<numDetalles; i++)
            subtotal += detalles[i].getSubtotal();
        iva = subtotal * 0.16; //agrega el 16 del iva
        total = subtotal + iva; //agrega al total el iva
    }
    float getTotal() const { return total; }
    float getSubtotal() const { return subtotal; }
    int getNumDetalles() const { return numDetalles; } 
    int getCantidadTotalProductos()const{ //cantidad total de productos los suma
        int totalCant=0;
        for (int i=0; i<numDetalles; i++)
            totalCant += detalles[i].getCantidad(); //suma
        return totalCant;
    }

    void imprimirTicket() { //imprime el ticket
        cout << "\n=====================================\n";
        cout << "           TICKET DE COMPRA\n";
        cout << "=====================================\n";
        cout << "Fecha: " << fecha << "\n";
        cout << "Hora:  " << hora << "\n";
        for (int i=0; i<numDetalles; i++)
        detalles[i].imprimir();
        cout << "-------------------------------------\n";
        cout<<"Metodo de pago: "<<metodoPago<<"\n";
        cout << "Subtotal: $" << subtotal << "\n";
        cout << "IVA (16%): $" << iva << "\n";
        cout << "TOTAL A PAGAR: $" << total << "\n";
        cout << "-------------------------------------\n";
        cout << "Gracias por su compra :) \n";
        cout << "=====================================\n\n";
    }

    float getGanancia()const{//obtiene las ganacias
        float ganancia=0;
        for (int i=0; i<numDetalles; i++){
            ganancia+=detalles[i].getGanancia();
        }
        return ganancia;
    }

    void guardarEnArchivo(ofstream &out) { //guarda em archivo
        out.write((char*)this,sizeof(*this)); //escribe
    }

    void leerDeArchivo(ifstream&in){ //lee en archivo
        in.read((char*)this, sizeof(*this)); //escribe
    }
};
int Venta::ultimoFolio=0;

#endif
