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
#include "Carrito.h"
#include "VentaPendiente.h"
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
    Carrito carrito(u->getId());

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
        cout << "4. Registrar venta al carrito\n";
        cout << "5. Mostrar carrito\n";
        cout << "6. Confirmar carrito\n";

        if (esAdmin) {
            cout << "7. Agregar producto\n";
            cout << "8. Modificar producto\n";
            cout << "9. Eliminar producto\n";
            cout << "10. Añadir Cliente\n";
            cout << "11. Mostrar Clientes\n";
            cout << "12. Buscar Cliente\n";
            cout << "13. Modificar Cliente\n";
            cout << "14. Lista de proveedores\n";
            cout << "15. Agregar proveedor\n";
            cout << "16. Administrar usuarios (listar)\n";
            cout << "17. Agregar usuario\n";
            cout << "18. Mostrar ventas historicas\n";
            cout << "19. Reporte por producto\n";
            cout << "20 no m sale el 17//ACTUALIZACION; CREO Q ESTA REPETIDO EN EL PUNYO 20\n";
            cout << "21. Consulta por IdProveedor\n";
            cout << "22. Consulta por categoria\n";
            cout << "23. Consulta producto vendido por Fecha\n";
            cout << "24. Consulta por cliente\n";
            cout << "25. Generar reporte de ventas y ganancias por dia en txt\n";
            cout << "26. Registrar detalle de gastos\n";
            cout << "27. Corte del dia\n";
            cout << "28. Exportar el historial de clientes\n";
            cout << "29. Exportar el historial de proveedores\n";
            cout << "30. Top 10 de productos.\n";
            cout << "31. Ajustar precios por porcentaje";
            cout << "32. Aplicar mayoreo";
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

            case 4:{
                if (esAdmin) {
                        cout << "Acceso denegado.\n";
                        break;
                }

                int id_cliente;
                cout << "ID del cliente:\n";
                cin >> id_cliente;

                if (archivoClientes.buscarCliente(id_cliente) != 1) {
                    cout << "Cliente no valido\n";
                    break;
                }

                VentaPendiente vp;
                vp.generarFolio();
                vp.generarFechaHora();
                vp.setUsuario(u->getId());

                char continuar;
                do {
                    ArchVentas.registrarVentaPendiente(archivo, vp, id_cliente);
                    cout << "Agregar otro producto (s/n): ";
                    cin >> continuar;
                } while (continuar == 's' || continuar == 'S');

                vp.calcularTotales();
                carrito.agregarVenta(vp);

                cout << "Venta agregada al carrito correctamente.\n";
                } break;

            case 5:
                if (esAdmin) {
                cout << "Acceso denegado.\n";
                break;
                }
                carrito.mostrarCarrito();
                break;
            
            case 6:
                if(esAdmin){
                    cout<<"Acceso denegado.\n";
                    break;
                }
                carrito.confirmarTodas(ArchVentas);
                break;

            // ----------------- SOLO ADMIN -----------------

            case 7:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivo.agregarProducto();
                break;

            case 8:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int c;
                    cout << "Codigo del producto: ";
                    cin >> c;
                    archivo.modificarProducto(c);
                }
                break;

            case 9:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int c;
                    cout << "Codigo del producto: ";
                    cin >> c;
                    archivo.eliminarProducto(c);
                }
                break;

            case 10:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivoClientes.agregarCliente();
                break;

            case 11:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivoClientes.mostrarClientes();
                break;

            case 12:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int id;
                    cout << "ID del cliente: ";
                    cin >> id;
                    archivoClientes.buscarCliente(id);
                }
                break;

            case 13:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                {
                    int id;
                    cout << "ID del cliente a modificar: ";
                    cin >> id;
                    archivoClientes.modificarCliente(id);
                }
                break;

            case 14:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                ArchivoProv.mostrarProveedores();
                break;

            case 15:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                ArchivoProv.agregarProveedor();
                break;

            case 16:
                if (!esAdmin) { cout << "Acceso denegado.\n"; break; }
                archivoUsuarios.mostrarUsuarios();
                break;
            
            case 17:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                archivoUsuarios.agregarUsuario();
                break;

            case 18:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                ArchVentas.reporteTotales();
                break;
                
            case 19:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                cout<<"Producto a buscar:";
                int idP; cin>>idP;
                ArchVentas.reportePorProducto(idP);
                break;

            case 20:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char fecha[20];
                cout <<"Ingresa la fecha a consultar (DD/MM/AA): ";
                cin >> fecha;
                ArchVentas.reportePorDia(fecha);
                break;

            case 21:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                int idProv; cout<<"Ingresa Id del Proveedor para buscar sus productos: "; cin>>idProv;
                archivo.consulraporIdProv(idProv);
                break;

            case 22:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char cat[20]; cout<<"Ingresa la categoria a buscar: "; cin>>cat;
                archivo.consultaporCategoria(cat);
                break;

            case 23:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char Fech[20]; cout<<"Ingresa la fecha a buscar DD/MM/AAAA: "; cin>>Fech;
                ArchVentas.consultarPorFecha(Fech);
                break;
            
            case 24:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                int id_cliente;
                cout << "ID del cliente:\n";
                cin >> id_cliente;

                if(archivoClientes.buscarCliente(id_cliente)!=1){
                    cout << "No fue posible continuar el proceso por falta de un cliente\n";
                    break;
                }

                ArchVentas.mostrarVentasCliente(id_cliente);

                break;

            case 25:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                char fechat[20]; cout<<"Generar .txt por fecha: "; cin>>fechat;
                //este lo q hace es hacerlo un .txt, sale? o sea no lo imprime como q lo guarda, si quieres pruebalo, lo encuentras en output
                ArchVentas.reportePorDiaTxt(fechat);
                break;

            case 26:
                ArchGas.registrarGasto();
                break;

            case 27:
                char corte[11]; cout<<"Fecha del corte: "; cin>>corte;
                ArchVentas.corteDiario(corte);
                break;

            case 28:
                ArchVentas.HistorialClienteTxt(); break;

            case 29:
                ArchivoProv.historialProveedoresTxt(archivo); break;

            case 30:
                ArchVentas.Top10productos(); break;
            case 31:
                if (!esAdmin){
                    cout<<"Acceso denegado.\n"; break;
                }
                float porc;
                cout<<"Porcentaje(+  aumenta, -disminuye): ";
                cin>>porc;
                archivo.ajustarPrecioPorcentaje(porc);
                break;
            case 32:
                if(!esAdmin){
                    cout<<"Acceso denegado.\n";break;
                }
                char cat[30];
                float desc;

                cout<<"Categoria: ";
                cin>>cat;
                cout<<"Descuento de mayoreo (%): ";
                cin>>desc;
                archivo.aplicarMayoreoCategoria(cat, desc);
                break;
                case 0:
                cout << "\nCerrando sesión... \n";
                break;

            default:
                cout << "Opción inválida.\n";
        }

    } while (opcion != 0);

    return 0;
}
