#include <iostream>
#include "ArchivoProductos.h"
#include "Ticket.h"
#include "Varitas.h"
#include "Venta.h"
#include "Cliente.h"
#include "Proveedor.h"
#include "ArchivoVentas.h"
#include "ArchivoUsuario.h"
#include "Usuario.h"
using namespace std;

int main() {

    ArchivoProductos archivo;
    ArchivoProveedores ArchivoProv;
    ArchivoClientes archivoClientes;
    ArchivoVentas ArchVentas;
    ArchivoUsuarios archivoUsuarios;
    ArchivoGastos ArchGas;

    archivo.CrearArchivo();
    ArchivoProv.crearArchivo();
    ArchVentas.CrearArchivo();
    archivoUsuarios.crearArchivo();

    Usuario* u = nullptr;

    do {
            
        cout << "\n ====== Wasap ==== \n";
        cout << "Agregar usuario (0) / iniciar secion (1)\n";

        int tipo;
        cin >> tipo;
        if(tipo==0){
            archivoUsuarios.agregarUsuario();
            cout << "Listo\n";
            break;
        } else if(tipo!=1){
            cout << "Opción invalida\n";
        } else break;
    } while (true);

    cout << "\n======= INICIO DE SESION =======\n";
    do {
        u = archivoUsuarios.login();
        if (u == nullptr) {
            cout << "Usuario o contrasenia incorrectos. Intenta de nuevo.\n";
        }
    } while (u == nullptr);

    bool esAdmin = (u->getTipo() == 1);

    cout << "\nBienvenido, " << u->getNombre()
        << " (" << (esAdmin ? "Administrador" : "Cajero") << ")\n";

    // -------------------------------
    //           MENU
    // -------------------------------

    int opcion;
    do {
        cout << "\n------ MENU PRINCIPAL ------\n";
        cout << "1. Registrar venta\n";
        cout << "2. Mostrar productos\n";
        cout << "3. Buscar producto\n";

        if (esAdmin) {
            cout << "4. Agregar producto\n";
            cout << "5. Modificar producto\n";
            cout << "6. Eliminar producto\n";
            cout << "7. Añadir Cliente\n";
            cout << "8. Mostrar Clientes\n";
            cout << "9. Buscar Cliente\n";
            cout << "10. Modificar Cliente\n";
            cout << "11. Lista de proveedores\n";
            cout << "12. Agregar proveedor\n";
            cout << "13. Administrar usuarios (listar)\n";
            cout<<"14. Agregar usuario\n";
            cout<<"15. Mostrar ventas historicas\n";
            cout<<"16. Reporte por producto\n";
            cout<<"Aun no m sale el 17//ACTUALIZACION; CREO Q ESTA REPETIDO EN EL PUNYO 20\n";
            cout<<"18. Consulta por IdProveedor\n";
            cout<<"19. Consulta por categoria\n";
            cout<<"20. Consulta producto vendido por Fecha\n";
            cout << "21. Consulta por cliente\n";
            cout<<"22. Generar reporte de ventas y ganancias por dia en txt\n";
            cout<<"23. Registrar detalle de gastos\n";
            cout<<"24. Corte del dia\n";
            cout<<"25. Exportar el historial de clientes\n";
            cout<<"26. Exportar el historial de proveedores\n";
            cout<<"27. Top 10 de productos.\n";
        }

        cout << "0. Salir\n";
        cout << "Selecciona una opcion: ";
        cin >> opcion;


        switch (opcion) {

            case 1: {
                int codigo, cantidad;
                char continuar;
                Ticket total;

                int id_cliente;

                cout << "ID del cliente:\n";
                cin >> id_cliente;

                if(archivoClientes.buscarCliente(id_cliente)!=1){
                    cout << "No fue posible continuar el proceso por falta de un cliente\n";
                    break;
                }

                do {
                    ArchVentas.registrarVenta(archivo,id_cliente);

                    cout << "¿Vender otro producto? (s/n): ";
                    cin >> continuar;

                } while (continuar == 's' || continuar == 'S');

                Venta vf;
                vf.generarFechaHora();
                vf.calcularTotales();
                vf.imprimirTicket();
            } break;

            case 2:
                archivo.mostrarProductos();
                break;

            case 3: {
                int c;
                cout << "Codigo del producto: ";
                cin >> c;
                archivo.buscarProducto(c);
            } break;

            // ----------------- SOLO ADMIN -----------------

            case 4:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivo.agregarProducto();
                break;

            case 5:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int c;
                    cout << "Codigo del producto: ";
                    cin >> c;
                    archivo.modificarProducto(c);
                }
                break;

            case 6:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int c;
                    cout << "Codigo del producto: ";
                    cin >> c;
                    archivo.eliminarProducto(c);
                }
                break;

            case 7:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivoClientes.agregarCliente();
                break;

            case 8:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivoClientes.mostrarClientes();
                break;

            case 9:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int id;
                    cout << "ID del cliente: ";
                    cin >> id;
                    archivoClientes.buscarCliente(id);
                }
                break;

            case 10:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int id;
                    cout << "ID del cliente a modificar: ";
                    cin >> id;
                    archivoClientes.modificarCliente(id);
                }
                break;

            case 11:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                ArchivoProv.mostrarProveedores();
                break;

            case 12:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                ArchivoProv.agregarProveedor();
                break;

            case 13:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivoUsuarios.mostrarUsuarios();
                break;
            
            case 14:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                archivoUsuarios.agregarUsuario();
                break;

            case 15:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                ArchVentas.reporteTotales();
                break;
                
            case 16:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                cout<<"Producto a buscar:";
                int idP; cin>>idP;
                ArchVentas.reportePorProducto(idP);
                break;
            case 17:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char fecha[20];
                cout <<"Ingresa la fecha a consultar (DD/MM/AA): ";
                cin >> fecha;
                ArchVentas.reportePorDia(fecha);
                break;

            case 18:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                int idProv; cout<<"Ingresa Id del Proveedor para buscar sus productos: "; cin>>idProv;
                archivo.consulraporIdProv(idProv);
                break;

            case 19:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char cat[20]; cout<<"Ingresa la categoria a buscar: "; cin>>cat;
                archivo.consultaporCategoria(cat);
                break;

            case 20:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char Fech[20]; cout<<"Ingresa la fecha a buscar DD/MM/AAAA: "; cin>>Fech;
                ArchVentas.consultarPorFecha(Fech);
                break;
            
            case 21:
                //Hacer cosas de admin y eso

                int id_cliente;
                cout << "ID del cliente:\n";
                cin >> id_cliente;

                if(archivoClientes.buscarCliente(id_cliente)!=1){
                    cout << "No fue posible continuar el proceso por falta de un cliente\n";
                    break;
                }

                ArchVentas.mostrarVentasCliente(id_cliente);

                break;

            case 22:
                char fechat[20]; cout<<"Generar .txt por fecha: "; cin>>fechat;
                //este lo q hace es hacerlo un .txt, sale? o sea no lo imprime como q lo guarda, si quieres pruebalo, lo encuentras en output
                ArchVentas.reportePorDiaTxt(fechat);
                break;

            case 23:
                ArchGas.registrarGasto();
                break;

            case 24:
                char corte[11]; cout<<"Fecha del corte: "; cin>>corte;
                ArchVentas.corteDiario(corte);
                break;

            case 25:
                ArchVentas.HistorialClienteTxt(); break;

            case 26:
                ArchivoProv.historialProveedoresTxt(archivo); break;

            case 27:
                ArchVentas.Top10productos(); break;
            case 0:
                cout << "\nCerrando sesión... \n";
                break;

            default:
                cout << "Opción inválida.\n";
        }

    } while (opcion != 0);

    return 0;
}
