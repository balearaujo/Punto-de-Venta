#include <iostream>
#include "ArchivoProductos.h"
#include "Ticket.h"
#include "Varitas.h"
#include "Venta.h"
#include "Cliente.h"
#include "Proveedor.h"
using namespace std;


int main() {
    ArchivoProductos archivo;
    ArchivoProveedores ArchivoProv;
    ArchivoClientes archivoClientes;

    archivo.CrearArchivo();
    ArchivoProv.crearArchivo();

    int opcion;
    do {
        cout<<"\n---- MENU PRINCIPAL ----\n";
        cout<<"1. Agregar producto"<<endl;
        cout<<"2. Mostrar productos"<<endl;
        cout<<"3. Buscar producto"<<endl;
        cout<<"4. Modificar producto"<<endl;
        cout<<"5. Eliminar producto"<<endl;
        cout<<"6. Registrar venta"<<endl;
        cout<<"7. Añadir Cliente"<<endl;
        cout<<"8. Mostrar Clientes"<<endl;
        cout<<"9. Buscar Cliente"<<endl;
        cout<<"10. Modificar Cliente"<<endl;
        cout<<"11. Salir"<<endl;
        cout<<"12. Lista de proveedores "<<endl;
        cout<<"13. Agregar proveedor"<<endl;
        cout<<"Selecciona una opcion: "<<endl; 
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
                int codigo, cantidad;
                char continuar;
                Ticket total; // acumula subtotales

                do {
                    cout << "Codigo del producto: ";
                    cin >> codigo;
                    cout << "Cantidad a vender: ";
                    cin >> cantidad;

                    Ticket venta = archivo.registrarVenta(codigo, cantidad);

                    if (venta.getSubtotal() > 0)
                        total = total + venta;

                    cout << "¿Vender otro producto? (s/n): ";
                    cin >> continuar;

                } while (continuar == 's' || continuar == 'S');

                // Aquí ya tienes el total acumulado
                cout << "\nTOTAL DE LA VENTA (SUBTOTAL): $" << total.getSubtotal() << "\n";

                // Crear ticket final
                VentaFinal vf;
                vf.generarFechaHora();
                vf.pedirMetodoPago();
                vf.calcularTotales(total.getSubtotal());
                vf.imprimirTicket();

            } break;
            case 11: cout << "\nSaliendo del programa...\n";
                break;
            
            case 7: {
                ArchivoClientes archivoClientes;
                archivoClientes.agregarCliente();
            } break;
            case 8: {
                ArchivoClientes archivoClientes;
                archivoClientes.mostrarClientes();
            } break;
            case 9: {
                ArchivoClientes archivoClientes;
                int id; cout << "ID del cliente: "; cin >> id;
                archivoClientes.buscarCliente(id);
            } break;

            case 10: {
                ArchivoClientes archivoClientes;
                int id; cout << "ID del cliente a modificar: "; cin >> id;
                archivoClientes.modificarCliente(id);
            } break;

            case 12:{
                ArchivoProv.mostrarProveedores();
                break;
            }

            case 13:{
                ArchivoProv.agregarProveedor();
                break;
            }

            default: cout << "Opcion invalida.\n";
        }

    } while(opcion != 7);

    return 0;
}
