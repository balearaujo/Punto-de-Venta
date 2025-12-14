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

ArchivoProductos archivo;
ArchivoProveedores ArchivoProv;
ArchivoClientes archivoClientes;
ArchivoVentas ArchVentas;
ArchivoUsuarios archivoUsuarios;
ArchivoGastos ArchGas;

void menu();
void ayuda();
void VentasMenu(); 
void OpcionesdeCarrito();
void ProductosMenu();
void Modificador();
void ClienteMenu();
void ProveedoresMenu();
void UsuariosMenu();
void ReportesMenu();
void Exportables();
void ConsultasMenu();
void AdministacionMenu();


Usuario* u = nullptr;
Carrito* carrito = nullptr;
bool esAdmin=false;

void titulo(const char* t){
    system("cls");
    cout << "====================================\n";
    cout << " " << t << "\n";
    cout << "====================================\n";
}

void pausa(){
    cout << "\n";
    system("pause");
}

int leerEnteroSeguro(){
    int x;
    while(!(cin >> x)){
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Ingrese un numero: ";
    }
    return x;
}

float leerFloatSeguro(){
    float x;
    while(!(cin >> x)){
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Ingrese un numero valido: ";
    }
    return x;
}

char leerCharSN(){
    char c;
    cin >> c;
    while(c!='s' && c!='S' && c!='n' && c!='N'){
        cout << "Ingrese solo (s/n): ";
        cin >> c;
    }
    return c;
}


int main() {
    archivo.CrearArchivo();
    ArchivoProv.crearArchivo();
    ArchVentas.CrearArchivo();
    archivoUsuarios.crearArchivo();

    //login
    int tipo;
    do {
        titulo("SISTEMA DE PUNTO DE VENTA");
        cout << "1. Iniciar sesion\n";
        cout << "2. Registrar nuevo usuario\n";
        cout << "> ";
        cin >> tipo;

        if(tipo == 2){
            archivoUsuarios.agregarUsuario();
            cout << "\nUsuario registrado\n";
            pausa();
        }
        else if(tipo != 1){
            cout << "\nOpcion invalida\n";
            pausa();
        }

    } while (tipo != 1);

    titulo("INICIO DE SESION");
    do {
        u = archivoUsuarios.login();

        if (u == nullptr) {
            cout << "\nDatos incorrectos\n";
            pausa();
            titulo("INICIO DE SESION");
        }

    } while (u == nullptr);

    
    esAdmin = (u->getTipo() == 1);
    carrito = new Carrito(u->getId());


    titulo("BIENVENIDO");
    cout << "Usuario: " << u->getNombre() << endl;
    cout << "Tipo: " << (esAdmin ? "Administrador" : "Cajero") << endl;
    pausa();

    menu();

    return 0;
}


void menu(){
    int opcion;
    do{
        titulo("MENU PRINCIPAL");
        cout<<"1. Ventas\n";
        cout<<"2. Producto\n";
        if (esAdmin){
        cout<<"3. Cliente\n";
        cout<<"4. Proveedores\n";
        cout<<"5. Usuarios\n";
        cout<<"6. Reportes\n";
        cout<<"7. Consultas\n";
        cout<<"8. Administracion\n";
        
        }
        cout<<"9. Ayuda\n";
        cout<<"0. Salir\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1: VentasMenu(); break;
            case 2: ProductosMenu(); break;
            case 3: 
            if (!esAdmin) {cout<<"Acceso denegado"; pausa(); break;}
            ClienteMenu(); break;
            case 4: 
            if (!esAdmin) {cout<<"Acceso denegado";  pausa(); break;}
            ProveedoresMenu(); break;
            case 5:
            if (!esAdmin) {cout<<"Acceso denegado";  pausa(); break;}
            UsuariosMenu(); break;
            case 6: 
            if (!esAdmin) {cout<<"Acceso denegado";  pausa(); break;}
            ReportesMenu(); break;
            case 7:
            if (!esAdmin) {cout<<"Acceso denegado";  pausa(); break;}
            ConsultasMenu(); break;
            case 8: 
            if (!esAdmin) {cout<<"Acceso denegado";  pausa(); break;}
            AdministacionMenu(); break;

            case 9:
            ayuda(); break;

            case 0: cout<<"Saliendo..."; break;
            default: cout<<"Opcion invalida"; pausa(); break; 
        }

    }while (opcion!=0);
}

void VentasMenu(){
    int opcion;

    do{
        titulo("MENU DE VENTAS");
        cout<<"\n1. Realizar ventas\n";
        cout<<"2. Opciones de carrito\n";
        cout<<"0. Regresar a menu principal\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1: {
                char continuar;
                Ticket total;

                int id_cliente;

                cout << "ID del cliente:\n";
                id_cliente=leerEnteroSeguro();

                if(archivoClientes.buscarCliente(id_cliente)!=1){
                    cout << "Cliente no registrado\n";
                    archivoClientes.agregarCliente(); 
                    pausa(); 
                }

                do {
                    ArchVentas.registrarVenta(archivo,id_cliente);

                    cout << "¿Vender otro producto? (s/n): ";
                    continuar = leerCharSN();

                } while (continuar == 's' || continuar == 'S');

                Venta vf;
                vf.generarFechaHora();
                vf.calcularTotales();
                vf.imprimirTicket();
            } pausa(); break;
            
            case 2: OpcionesdeCarrito(); break;
            case 0: return;
            default: cout<<"Opcion invalida";  pausa();
            
        }
    }while (opcion!=0);
}

void OpcionesdeCarrito(){
    int opcion;
    do{
        titulo("OPCIONES DE CARRITO");
        cout << "\n1. Registrar venta al carrito\n";
        cout << "2. Mostrar carrito\n";
        cout << "3. Confirmar carrito\n";
        cout<<"0. Regresar a Menu de Ventas\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:{
                int id_cliente;
                cout << "ID del cliente:\n";
                id_cliente=leerEnteroSeguro();

                if (archivoClientes.buscarCliente(id_cliente) != 1) {
                    cout << "Cliente no registrado\n";
                    archivoClientes.agregarCliente(); pausa();
                }

                VentaPendiente vp;
                vp.generarFolio();
                vp.generarFechaHora();
                vp.setUsuario(u->getId());

                char continuar;
                do {
                    ArchVentas.registrarVentaPendiente(archivo, vp, id_cliente);
                    cout << "Agregar otro producto (s/n): ";
                    continuar = leerCharSN();
                } while (continuar == 's' || continuar == 'S');

                vp.calcularTotales();
                carrito->agregarVenta(vp);

                cout << "Venta agregada al carrito correctamente.\n";
                } pausa(); break;

            case 2: 
                carrito->mostrarCarrito();pausa(); break; 
            case 3:
                carrito->confirmarTodas(ArchVentas);pausa(); break; 
            case 0: return;
            default: cout<<"Opcion invalida"; break;
            
        }

    }while (opcion!=0);
}
void ProductosMenu(){
    int opcion;

    do{
        titulo("MENU PRODUCTOS");
        cout << "\n1. Mostrar productos\n";
        cout << "2. Buscar producto\n";
        if (esAdmin){
        cout << "3. Agregar producto\n";
        cout << "4. Modificar producto\n";
        cout << "5. Eliminar producto\n";
        }
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                archivo.mostrarProductos(); pausa();
                break;

            case 2: {
                int c;
                cout << "Codigo del producto: ";
                c=leerEnteroSeguro();
                archivo.buscarProducto(c); pausa();
            } break;

            case 3:
            if (!esAdmin) {cout<<"Acceso denegado"; pausa(); break;}
                archivo.agregarProducto(); pausa();
                break;

            case 4:{
                if (!esAdmin) {cout<<"Acceso denegado"; pausa(); break;}
                Modificador();
                }
                break;

            case 5:{
                if (!esAdmin) {cout<<"Acceso denegado"; pausa(); break;}
                int c;
                cout << "Codigo del producto: "; c=leerEnteroSeguro();
                archivo.eliminarProducto(c); pausa();
                }
                break;
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
                
        }

    }while (opcion!=0);
}

void Modificador(){
    int opcion;

    do{
        titulo("OPCIONES DE MODIFICACION");
        cout<<"\n1. Modificar producto completo\n";
        cout<<"2. Ajustar precios por porcentaje\n";
        cout<<"3. Aplicar mayoreo\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:{
                    int c;
                    cout << "Codigo del producto: ";
                    cin >> c;
                    archivo.modificarProducto(c); pausa();
                }
            break;
            
            case 2:
                float porc;
                cout<<"Porcentaje(+  aumenta, -disminuye): ";
                porc = leerFloatSeguro();
                archivo.ajustarPrecioPorcentaje(porc); pausa();
                break;
            case 3: {
                char cat[30];
                float desc;

                cout<<"Categoria: ";
                cin>>cat;
                cout<<"Descuento de mayoreo (%): ";
                desc = leerFloatSeguro();

                archivo.aplicarMayoreoCategoria(cat, desc); pausa();
                break;
            }
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ClienteMenu(){
    int opcion;

    do{
        titulo("MANEJO DE CLIENTES");
        cout << "\n1. Añadir Cliente\n";
        cout << "2. Mostrar Clientes\n";
        cout << "3. Buscar Cliente\n";
        cout << "4. Modificar Cliente\n";
        cout<<"5. Generar historial en .txt\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                archivoClientes.agregarCliente(); pausa();
                break;

            case 2:
                archivoClientes.mostrarClientes(); pausa();
                break;

            case 3:
                {
                    int id;
                    cout << "ID del cliente: ";
                    id = leerEnteroSeguro();
                    archivoClientes.buscarCliente(id); pausa();
                }
                break;

            case 4:
                {
                    int id;
                    cout << "ID del cliente a modificar: ";
                    id = leerEnteroSeguro();
                    archivoClientes.modificarCliente(id); pausa();
                }
                break;

            case 5:
                ArchVentas.HistorialClienteTxt(); pausa(); break; 

            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ProveedoresMenu(){
    int opcion;

    do{
        titulo("MANEJO DE PROVEEDORES");
        cout << "\n1. Lista de proveedores\n";
        cout << "2. Agregar proveedor\n";
        cout<<"3. Generar historial en .txt\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                ArchivoProv.mostrarProveedores(); pausa();
                break;

            case 2:
                ArchivoProv.agregarProveedor(); pausa();
                break;
            case 3:
                ArchVentas.HistorialClienteTxt(); pausa(); break;
            
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}
void UsuariosMenu(){
    int opcion;

    do{
        titulo("MANEJO DE USUARIOS");
        cout << "\n1. Administrar usuarios (listar)\n";
        cout << "2. Agregar usuario\n";
        cout<<"3. Historial en .txt\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                archivoUsuarios.mostrarUsuarios(); pausa();
                break;
            case 2:
                archivoUsuarios.agregarUsuario(); pausa();
                break;
            case 3:
                int idUsuario;
                char fechaInicio[11], fechaFin[11];

                cout << "ID del usuario: ";
                idUsuario = leerEnteroSeguro();

                cout << "Fecha inicio (YYYY-MM-DD): ";
                cin >> fechaInicio;

                cout << "Fecha fin (YYYY-MM-DD): ";
                cin >> fechaFin;

                ArchVentas.historialPorUsuarioSemana(idUsuario, fechaInicio, fechaFin); pausa(); break; 
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ReportesMenu(){
    int opcion;

    do{
        titulo("OPCIONES DE REPORTE");
        cout << "\n1. Mostrar ventas historicas\n";
        cout << "2. Reporte por producto\n";
        cout<<"3. Historial por usuario\n";
        cout<<"4. Exportables a .txt\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
            ArchVentas.reporteTotales(); pausa();
            break;
            case 2:
                cout<<"Producto a buscar:";
                int idP; cin>>idP;
                ArchVentas.reportePorProducto(idP); pausa();
                break;
            case 3:
                int idUsuario;
                char fechaInicio[11], fechaFin[11];

                cout << "ID del usuario: ";
                idUsuario = leerEnteroSeguro();

                cout << "Fecha inicio (YYYY-MM-DD): ";
                cin >> fechaInicio;

                cout << "Fecha fin (YYYY-MM-DD): ";
                cin >> fechaFin;

                ArchVentas.historialPorUsuarioSemana(idUsuario, fechaInicio, fechaFin); pausa(); break;

            case 4: Exportables(); break;
            case 0: return;
            default: cout<<"Opcion invalida"; pausa(); 
        }

    }while (opcion!=0);
}

void Exportables(){
    int opcion;

    do{
        titulo("EXPORTABLES A .TXT");
        cout << "\n1. Generar reporte de ventas y ganancias por dia en txt\n";
        cout << "2. Corte del dia\n";
        cout << "3. Exportar el historial de clientes\n";
        cout << "4. Exportar el historial de proveedores\n";
        cout << "5. Top 10 de productos.\n";
        cout<<"6. Movimientos de caja\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                char fechat[20]; cout<<"Generar .txt por fecha: "; cin>>fechat;
                //este lo q hace es hacerlo un .txt, sale? o sea no lo imprime como q lo guarda, si quieres pruebalo, lo encuentras en output
                ArchVentas.reportePorDiaTxt(fechat); pausa();
                break;

            case 2:
                char corte[11]; cout<<"Fecha del corte: "; cin>>corte;
                ArchVentas.corteDiario(corte); pausa();
                break;

            case 3:
                ArchVentas.HistorialClienteTxt(); pausa(); break;

            case 4:
                ArchivoProv.historialProveedoresTxt(archivo); pausa();break;

            case 5:
                ArchVentas.Top10productos(); pausa(); break;

            case 6:
                char fec[11]; cout<<"Ingresa la fecha para mostar los movimientos de caja: "; cin>>fec;
                ArchVentas.movimientosCaja(fec); cout<<"Ahora visible en .txt";pausa(); break;
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ConsultasMenu(){
    int opcion;
    
    do{
        titulo("CONSULTAS A MENU");
        cout << "\n1. Consulta por IdProveedor\n";
        cout << "2. Consulta por categoria\n";
        cout << "3. Consulta producto vendido por Fecha\n";
        cout << "4. Consulta por cliente\n";
        cout<<"0. Regresar\n"; 
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                int idProv; cout<<"Ingresa Id del Proveedor para buscar sus productos: "; cin>>idProv;
                archivo.consulraporIdProv(idProv); pausa();
                break;

            case 2:
                char cat[20]; cout<<"Ingresa la categoria a buscar: "; cin>>cat;
                archivo.consultaporCategoria(cat); pausa();
                break;

            case 3:
                char Fech[20]; cout<<"Ingresa la fecha a buscar DD/MM/AAAA: "; cin>>Fech;
                ArchVentas.consultarPorFecha(Fech); pausa();
                break;
            
            case 4:
                int id_cliente;
                cout << "ID del cliente:\n";
                id_cliente = leerEnteroSeguro();

                if(archivoClientes.buscarCliente(id_cliente)!=1){
                    cout << "No fue posible continuar el proceso por falta de un cliente\n"; pausa();
                    break;
                }

                ArchVentas.mostrarVentasCliente(id_cliente); pausa();
                break; 

            case 0: return;
            default: cout<<"Opcion invalida"; pausa(); break;
        }

    }while (opcion!=0);
}
void AdministacionMenu(){
    int opcion;

    do{
        titulo("MENU DE ADMINISTRACION$");
        cout << "\n1. Generar reporte de ventas y ganancias por dia en txt\n";
        cout << "2. Registrar detalle de gastos\n";
        cout << "3. Corte del dia\n";
        cout<<"4. Movimientos de caja\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1:
                char fechat[20]; cout<<"Generar .txt por fecha: "; cin>>fechat;
                //este lo q hace es hacerlo un .txt, sale? o sea no lo imprime como q lo guarda, si quieres pruebalo, lo encuentras en output
                ArchVentas.reportePorDiaTxt(fechat); pausa();
                break;

            case 2:
                ArchGas.registrarGasto(); pausa();
                break;

            case 3:
                char corte[11]; cout<<"Fecha del corte: "; cin>>corte;
                ArchVentas.corteDiario(corte); pausa();
                break;
            
            case 4:
                char fec[11]; cout<<"Ingresa la fecha para mostar los movimientos de caja: "; cin>>fec;
                ArchVentas.movimientosCaja(fec); cout<<"Ahora visible en .txt"; pausa(); break;
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}


void ayuda(){
    titulo("AYUDA DEL SISTEMA");

    cout<<"Este sistema fue disenado para facilitar la administracion\n";
    cout<<"de un punto de venta, administrando elementos como ventas,\n";
    cout<<"productos, clientes y usuarios.\n\n";

    cout<<"FUNCIONES PRINCIPALES:\n\n";

    cout<<"- Ventas:\n";
    cout<<"  Registro de ventas directas, manejo de carrito y\n";
    cout<<"  confirmacion de ventas simultaneas.\n\n";

    cout<<"- Productos:\n";
    cout<<"  Consulta, alta, modificacion y eliminacion de productos.\n";
    cout<<"  Algunas opciones estan restringidas a administradores.\n\n";

    cout<<"FUNCIONES DIRIGIDAS A ADMINISTRADORES: \n\n";
    cout<<"- Clientes:\n";
    cout<<"  Registro, consulta y modificacion de clientes,\n";
    cout<<"  asi como generacion de historiales.\n\n";

    cout<<"- Proveedores:\n";
    cout<<"  Administracion de proveedores y reportes de historial.\n\n";

    cout<<"- Usuarios:\n";
    cout<<"  Control de usuarios, roles y consulta de actividades.\n\n";

    cout<<"- Reportes y Exportables:\n";
    cout<<"  Generacion de reportes, cortes del dia y archivos .txt.\n\n";

    cout<<"- Consultas:\n";
    cout<<"  Busquedas por categoria, proveedor, cliente o fecha.\n\n";

    cout<<"CONTROL DE ACCESO:\n";
    cout<<"El sistema cuenta con autenticacion por usuario y rol.\n";
    cout<<"Las opciones disponibles dependen del tipo de usuario.\n\n";

    cout<<"NOTA:\n";
    cout<<"Los archivos exportables se guardan automaticamente\n";
    cout<<"en la carpeta del proyecto.\n";

    pausa();
}
