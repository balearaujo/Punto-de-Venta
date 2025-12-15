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

//archivos
ArchivoProductos archivo;
ArchivoProveedores ArchivoProv;
ArchivoClientes archivoClientes;
ArchivoVentas ArchVentas;
ArchivoUsuarios archivoUsuarios;
ArchivoGastos ArchGas;

//funciones
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


Usuario* u = nullptr; //puntero a usuario
Carrito* carrito = nullptr; //puntero a carrito
bool esAdmin=false; //booleano si usuario es admin

void titulo(const char* t){ //impresion de titulo bonito
    system("cls");
    cout << "====================================\n";
    cout << " " << t << "\n";
    cout << "====================================\n";
}

void pausa(){ //pausa antes de borrar
    cout << "\n";
    system("pause");
}

int leerEnteroSeguro(){ //validación
    int x;
    while(!(cin >> x)){
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Ingrese un numero: ";
    }
    return x;
}

float leerFloatSeguro(){ //validacion
    float x;
    while(!(cin >> x)){
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Ingrese un numero valido: ";
    }
    return x;
}

char leerCharSN(){ //validacion
    char c;
    cin >> c;
    while(c!='s' && c!='S' && c!='n' && c!='N'){
        cout << "Ingrese solo (s/n): ";
        cin >> c;
    }
    return c;
}


int main() {
    //creacion de los archivos
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
        //opciones de registro
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
        u = archivoUsuarios.login(); //guarda el login

        if (u == nullptr) {
            cout << "\nDatos incorrectos\n";
            pausa();
            titulo("INICIO DE SESION");
        }

    } while (u == nullptr);

    
    esAdmin = (u->getTipo() == 1); //guarda si el usuario es administrador
    carrito = new Carrito(u->getId());


    titulo("BIENVENIDO");
    cout << "Usuario: " << u->getNombre() << endl; //obtiene el nombre
    cout << "Tipo: " << (esAdmin ? "Administrador" : "Cajero") << endl; //imprime el rol
    pausa();

    menu();

    return 0;
}


void menu(){
    int opcion; //opciones principales
    do{
        titulo("MENU PRINCIPAL");
        cout<<"1. Ventas\n";
        cout<<"2. Producto\n";
        if (esAdmin){ //opciones solo visibles para admin
        cout<<"3. Cliente\n";
        cout<<"4. Proveedores\n";
        cout<<"5. Usuarios\n";
        cout<<"6. Reportes\n";
        cout<<"7. Consultas\n";
        cout<<"8. Administracion\n";
        
        }
        cout<<"9. Ayuda\n";
        cout<<"0. Salir\n";
        cout<<"> "; opcion = leerEnteroSeguro(); //validacion

        switch(opcion){ //aqui llama otros menus
            case 1: VentasMenu(); break;
            case 2: ProductosMenu(); break;
            case 3: //aqui llama a menus solo accesibles para administradores
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

void VentasMenu(){ //realizacion de ventas
    int opcion;

    do{
        titulo("MENU DE VENTAS");
        cout<<"\n1. Realizar ventas\n";
        cout<<"2. Opciones de carrito\n"; //llama a opciones de ventas simultaneas
        cout<<"0. Regresar a menu principal\n";
        cout<<"> "; opcion = leerEnteroSeguro();

        switch(opcion){
            case 1: {
                char continuar;
                Ticket total;

                int id_cliente;

                cout << "ID del cliente:\n";
                id_cliente=leerEnteroSeguro();

                if(archivoClientes.buscarCliente(id_cliente)!=1){ //revisa si el cliente ya esta registrado
                    cout << "Cliente no registrado\n";
                    archivoClientes.agregarCliente();  //se agrega cliente
                    pausa(); 
                }

                do {
                    ArchVentas.registrarVenta(archivo,id_cliente);

                    cout << "¿Vender otro producto? (s/n): ";
                    continuar = leerCharSN();

                } while (continuar == 's' || continuar == 'S'); //condicion para vender otro

                Venta vf;
                vf.generarFechaHora(); //genera hora
                vf.calcularTotales(); //calcula totales
                vf.imprimirTicket(); //imprime tickets
            } pausa(); break;
            
            case 2: OpcionesdeCarrito(); break;
            case 0: return;
            default: cout<<"Opcion invalida";  pausa();
            
        }
    }while (opcion!=0);
}

void OpcionesdeCarrito(){ //opciones para venta simultanea
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

                if (archivoClientes.buscarCliente(id_cliente) != 1) { //pregunta si el cliente esta registrado
                    cout << "Cliente no registrado\n"; //si el cliente no esta registado lo agrefa
                    archivoClientes.agregarCliente(); pausa();
                }

                VentaPendiente vp; //entra a venta pendiente
                vp.generarFolio(); //genera folio
                vp.generarFechaHora(); //genera fecha
                vp.setUsuario(u->getId()); //guarda usuario

                char continuar;
                do {
                    ArchVentas.registrarVentaPendiente(archivo, vp, id_cliente);
                    cout << "Agregar otro producto (s/n): ";
                    continuar = leerCharSN();
                } while (continuar == 's' || continuar == 'S');

                vp.calcularTotales();//calcula el total
                carrito->agregarVenta(vp); //se agrefa la venta al carrito

                cout << "Venta agregada al carrito correctamente.\n";
                } pausa(); break;

            case 2: 
                carrito->mostrarCarrito();pausa(); break; //se muestra el carrito
            case 3:
                carrito->confirmarTodas(ArchVentas);pausa(); break; //confirma las venta
            case 0: return;
            default: cout<<"Opcion invalida"; break;
            
        }

    }while (opcion!=0);
}
void ProductosMenu(){ //menu con opciones para los productos
    int opcion;

    do{
        titulo("MENU PRODUCTOS"); //llamada a titulo bonito
        cout << "\n1. Mostrar productos\n";
        cout << "2. Buscar producto\n";
        if (esAdmin){
        cout << "3. Agregar producto\n";
        cout << "4. Modificar producto\n";
        cout << "5. Eliminar producto\n";
        }
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro(); //validacion de entero

        switch(opcion){
            case 1:
                archivo.mostrarProductos(); pausa(); //muestra producto
                break;

            case 2: {
                int c;
                cout << "Codigo del producto: ";
                c=leerEnteroSeguro(); //las entradas tienen validacion
                archivo.buscarProducto(c); pausa(); //busca producto
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
                archivo.eliminarProducto(c); pausa(); //elimina
                }
                break;
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
                
        }

    }while (opcion!=0);
}

void Modificador(){ //opciones para modificar producto
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
                    archivo.modificarProducto(c); pausa(); //modifica el producto en precio y existencia
                }
            break;
            
            case 2:
                float porc;
                cout<<"Porcentaje(+  aumenta, -disminuye): ";
                porc = leerFloatSeguro(); //valida el flotante
                archivo.ajustarPrecioPorcentaje(porc); pausa(); //ajusta los precios
                break;
            case 3: {
                char cat[30];
                float desc;

                cout<<"Categoria: ";
                cin>>cat;
                cout<<"Descuento de mayoreo (%): ";
                desc = leerFloatSeguro(); //validacion de flotante

                archivo.aplicarMayoreoCategoria(cat, desc); pausa(); //aplica descuento a toda una categoria
                break;
            }
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ClienteMenu(){ //opciones de manejo cliente
    int opcion;

    do{
        titulo("MANEJO DE CLIENTES");
        cout << "\n1. Añadir Cliente\n";
        cout << "2. Mostrar Clientes\n";
        cout << "3. Buscar Cliente\n";
        cout << "4. Modificar Cliente\n";
        cout<<"5. Generar historial en .txt\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro(); //lee  entero 

        switch(opcion){
            case 1:
                archivoClientes.agregarCliente(); pausa(); //agrega cliente
                break;

            case 2:
                archivoClientes.mostrarClientes(); pausa(); //muestra cliente
                break;

            case 3:
                {
                    int id;
                    cout << "ID del cliente: ";
                    id = leerEnteroSeguro();
                    archivoClientes.buscarCliente(id); pausa(); //busca cliente
                }
                break;

            case 4:
                {
                    int id;
                    cout << "ID del cliente a modificar: ";
                    id = leerEnteroSeguro();
                    archivoClientes.modificarCliente(id); pausa(); //modifica cliente
                }
                break;

            case 5:
                ArchVentas.HistorialClienteTxt(); pausa(); break; //muestra historial de cliente

            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ProveedoresMenu(){ //menu para manejo de proveedores
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
                ArchivoProv.mostrarProveedores(); pausa(); //mostrar
                break;

            case 2:
                ArchivoProv.agregarProveedor(); pausa(); //agregar
                break;
            case 3:
                ArchVentas.HistorialClienteTxt(); pausa(); break; //historial
            
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}
void UsuariosMenu(){ //menu de para manejo de usuaurios
    int opcion;

    do{
        titulo("MANEJO DE USUARIOS");
        cout << "\n1. Administrar usuarios (listar)\n";
        cout << "2. Agregar usuario\n";
        cout<<"3. Historial en .txt\n";
        cout<<"0. Regresar\n";
        cout<<"> "; opcion = leerEnteroSeguro(); //validacion

        switch(opcion){
            case 1:
                archivoUsuarios.mostrarUsuarios(); pausa(); //muestra usuario
                break;
            case 2:
                archivoUsuarios.agregarUsuario(); pausa(); //agrega
                break;
            case 3:
                int idUsuario;
                char fechaInicio[11], fechaFin[11];

                cout << "ID del usuario: ";
                idUsuario = leerEnteroSeguro();

                cout << "Fecha inicio (DD-MM-YYYY): "; //fecha de inicio con su formato
                cin >> fechaInicio;

                cout << "Fecha fin (DD-MM-AAAA): "; //fecha fin con formato
                cin >> fechaFin;

                ArchVentas.historialPorUsuarioSemana(idUsuario, fechaInicio, fechaFin); pausa(); break; //historial de usuario por semana
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ReportesMenu(){ //opciones de reporte
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
            ArchVentas.reporteTotales(); pausa(); //reporte total
            break;
            case 2:
                cout<<"Producto a buscar:";
                int idP; cin>>idP;
                ArchVentas.reportePorProducto(idP); pausa(); //reporte por producto
                break;
            case 3:
                int idUsuario;
                char fechaInicio[11], fechaFin[11];

                cout << "ID del usuario: ";
                idUsuario = leerEnteroSeguro(); //validacion

                cout << "Fecha inicio (YYYY-MM-DD): ";
                cin >> fechaInicio;

                cout << "Fecha fin (YYYY-MM-DD): ";
                cin >> fechaFin;

                ArchVentas.historialPorUsuarioSemana(idUsuario, fechaInicio, fechaFin); pausa(); break; //por usuario

            case 4: Exportables(); break;
            case 0: return;
            default: cout<<"Opcion invalida"; pausa(); 
        }

    }while (opcion!=0);
}

void Exportables(){ //exportables
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
                ArchVentas.movimientosCaja(fec); cout<<"Ahora visible en .txt";pausa(); break; //moviemientos de caja
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}

void ConsultasMenu(){ //consultas por clasificacion
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

                if(archivoClientes.buscarCliente(id_cliente)!=1){ //revisar si el cliente esta registrado
                    cout << "No fue posible continuar el proceso por falta de un cliente\n"; pausa();
                    break;
                }

                ArchVentas.mostrarVentasCliente(id_cliente); pausa(); //mostrar ventas
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
                ArchGas.registrarGasto(); pausa(); //registrar gastos
                break;

            case 3:
                char corte[11]; cout<<"Fecha del corte: "; cin>>corte;
                ArchVentas.corteDiario(corte); pausa(); //corte diario
                break;
            
            case 4:
                char fec[11]; cout<<"Ingresa la fecha para mostar los movimientos de caja: "; cin>>fec;
                ArchVentas.movimientosCaja(fec); cout<<"Ahora visible en .txt"; pausa(); break; //movimientos de caja
            case 0: return;
            default: cout<<"Opcion invalida"; pausa();
        }

    }while (opcion!=0);
}


void ayuda(){ //funcion de ayuda en texto
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
