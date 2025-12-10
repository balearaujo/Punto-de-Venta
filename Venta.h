#ifndef VENTA_H
#define VENTA_H

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

class VentaFinal {
private:
    char fecha[20];
    char hora[20];
    char metodoPago[20];
    float subtotal;
    float iva;
    float total;

public:
    VentaFinal() {
        strcpy(fecha, ""); 
        strcpy(hora, "");
        strcpy(metodoPago, "");
        subtotal = iva = total = 0;
    }

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

    void calcularTotales(float sub) {
        subtotal = sub;
        iva = subtotal * 0.16;
        total = subtotal + iva;
    }

    void pedirMetodoPago() {
        cout << "Metodo de pago (EFECTIVO/TARJETA): ";
        cin >> metodoPago;
    }

    void imprimirTicket() {
        cout << "\n=====================================\n";
        cout << "           TICKET DE COMPRA\n";
        cout << "=====================================\n";
        cout << "Fecha: " << fecha << "\n";
        cout << "Hora:  " << hora << "\n";
        cout << "Metodo de pago: " << metodoPago << "\n";
        cout << "-------------------------------------\n";
        cout << "Subtotal: $" << subtotal << "\n";
        cout << "IVA (16%): $" << iva << "\n";
        cout << "TOTAL A PAGAR: $" << total << "\n";
        cout << "-------------------------------------\n";
        cout << "Gracias por su compra :3\n";
        cout << "=====================================\n\n";
    }

};

#endif
