#include <iostream>
#include "ArchivoProductos.h"
#include "Ticket.h"
#include "Varitas.h"
using namespace std;

int main() {
    ArchivoProductos archivo;
    archivo.CrearArchivo();
    int opcion;
    do {
        cout<<"\n---- MENU PRINCIPAL ----\n";
        cout<<"1. Agregar producto"<<endl;
        cout<<"2. Mostrar productos"<<endl;
        cout<<"3. Buscar producto"<<endl;
        cout<<"4. Modificar producto"<<endl;
        cout<<"5. Eliminar producto"<<endl;
        cout<<"6. Registrar venta"<<endl;
        cout<<"7. Salir"<<endl;
        cout<<"Selecciona una opcion: "; 
        cin >> opcion;

        switch(opcion) {
            case 1: archivo.agregarProducto();
                break;
            case 2: archivo.mostrarProductos();
                break;
            case 3: { int c; cout << "Codigo del producto: "; cin >> c; archivo.buscarProducto(c);}
                break;
            case 4: { int c; cout << "Codigo del producto: "; cin >> c; archivo.modificarProducto(c);} 
                break;
            case 5: { int c; cout << "Codigo del producto: "; cin >> c; archivo.eliminarProducto(c);}
                break;
            case 6: {
                int codigo, cantidad; char continuar;
                Ticket total;
                do {
                    cout << "Codigo del producto: "; cin >> codigo;
                    cout << "Cantidad a vender: "; cin >> cantidad;
                    Ticket venta = archivo.registrarVenta(codigo, cantidad);
                    if(venta.getSubtotal()>0) total = total + venta;
                    cout << "¿Vender otro producto? (s/n): "; cin >> continuar;
                } while(continuar=='s' || continuar=='S');
                cout << "\n TOTAL DE LA VENTA \n" << total << "\n\n";
            } break;
            case 7: cout << "\nSaliendo del programa...\n";
                break;
            default: cout << "Opcion invalida.\n";
        }

    } while(opcion != 7);

    return 0;
}
