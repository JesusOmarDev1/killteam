#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "structures.h"
#include "functions.cpp"

using namespace std;

// Variables globales de listas
ListaOperativos listaOperativos;
ListaEquipamiento listaEquipamiento;
ListaRosters listaRosters;

extern void inicializarListaOperativos(ListaOperativos *lista);
extern void inicializarListaEquipamiento(ListaEquipamiento *lista);
extern void inicializarListaRosters(ListaRosters *lista);
extern bool cargarOperativos(ListaOperativos *lista);
extern bool cargarEquipamiento(ListaEquipamiento *lista);
extern bool cargarRosters(ListaRosters *lista);
extern bool guardarOperativos(ListaOperativos *lista);
extern bool guardarEquipamiento(ListaEquipamiento *lista);
extern bool guardarRosters(ListaRosters *lista);
extern void liberarListaOperativos(ListaOperativos *lista);
extern void liberarListaEquipamiento(ListaEquipamiento *lista);
extern void liberarListaRosters(ListaRosters *lista);
extern void precargarEquipamientoUniversal(ListaEquipamiento *lista);
extern void precargarEquipamientoFaccion(ListaEquipamiento *lista);
extern void mostrarEquipamientoPorFaccion(ListaEquipamiento *lista, int faccionId);
extern void inicializarEquipamientoCompleto(ListaEquipamiento *lista);
extern bool asignarEquipamientoARoster(DatosRoster *roster, int idxOperativo, int idEquipamiento);
extern void mostrarEquipamientoDeRoster(ListaEquipamiento *catalogo, DatosRoster *roster);
extern void precargarCatalogoOperativos(ListaOperativos *lista);
extern void mostrarOperativosPorFaccion(ListaOperativos *lista, int faccionId);
extern int obtenerComposicionFaccion(int faccionId);
extern bool validarComposicionRoster(int faccionId, int cantidadOperativos, int tieneLider);
extern void mostrarNombreFaccion(int faccionId);
extern int calcularPuntosRoster(ListaOperativos *catalogo, DatosRoster *roster);
extern void mostrarResumenRoster(ListaOperativos *catalogo, ListaEquipamiento *equipos, DatosRoster *roster);
extern void inicializarCatalogoCompleto(ListaOperativos *lista);
extern NodoOperativo *buscarOperativoPorId(ListaOperativos *lista, int id);
extern NodoEquipamiento *buscarEquipamientoPorId(ListaEquipamiento *lista, int id);
extern void agregarRoster(ListaRosters *lista, DatosRoster datos);
extern void mostrarRostersGuardados(ListaOperativos *catalogo, ListaRosters *lista);
extern void verDetalleRoster(ListaOperativos *catalogo, ListaEquipamiento *equipos, ListaRosters *lista, int idRoster);
extern bool eliminarRosterCompleto(ListaRosters *lista, int idRoster);
extern void reconstruirListaRostersDesdeArchivo(ListaRosters *lista);
extern NodoRoster *buscarRosterPorId(ListaRosters *lista, int id);
extern void menuBusquedaAvanzada(ListaOperativos *lista);
extern void capturarOperativoPorTeclado(ListaOperativos *lista);
extern void menuEditarOperativo(ListaOperativos *lista);
extern void menuEliminarOperativo(ListaOperativos *lista);
extern void exportarRosterATexto(ListaOperativos *catalogo, ListaEquipamiento *equipos, ListaRosters *lista, int idRoster);
extern void menuEditarRoster(ListaOperativos *catalogo, ListaEquipamiento *equipos, ListaRosters *lista);
extern void limpiarBuffer();
extern int leerEntero(const char *mensaje);
extern int leerEnteroEnRango(const char *mensaje, int minimo, int maximo);
extern void leerCadenaSegura(const char *mensaje, char *destino, int tamanoMaximo);
extern bool confirmarAccion(const char *mensaje);
extern bool validarIdOperativoExiste(ListaOperativos *lista, int id);
extern bool validarIdEquipamientoExiste(ListaEquipamiento *lista, int id);
extern void limpiarPantalla();
extern void lineaDecorativa(int ancho);
extern void lineaSimple(int ancho);
extern void mostrarTitulo(const char *titulo);
extern void mostrarHeaderKillTeam();
extern void mostrarMensajeExito(const char *mensaje);
extern void mostrarMensajeError(const char *mensaje);
extern void mostrarMensajeAdvertencia(const char *mensaje);
extern void pausarPantalla();
extern void mostrarSubtitulo(const char *subtitulo);

void menuListasGuardadas();

void mostrarMenuPrincipal()
{
    mostrarHeaderKillTeam();
    mostrarTitulo("MENU PRINCIPAL");
    cout << "  [1] Catalogo de Operativos" << endl;
    cout << "  [2] Equipamiento" << endl;
    cout << "  [3] Roster Builder" << endl;
    cout << "  [4] Listas Guardadas" << endl;
    cout << "  [5] Busqueda Avanzada" << endl;
    cout << "  [0] Salir" << endl;
    lineaDecorativa(40);
}

void mostrarMenuEquipamiento()
{
    mostrarTitulo("EQUIPAMIENTO");
    cout << "  [1] Ver equipamiento Universal" << endl;
    cout << "  [2] Ver equipamiento Angeles de la Muerte" << endl;
    cout << "  [3] Ver equipamiento Kommandos" << endl;
    cout << "  [4] Ver equipamiento Mandrakes" << endl;
    cout << "  [5] Ver equipamiento Tropa Danzante Vacia" << endl;
    cout << "  [0] Volver" << endl;
    lineaDecorativa(40);
}

void menuEquipamiento()
{
    int op;
    do
    {
        mostrarMenuEquipamiento();
        op = leerEnteroEnRango("", 0, 5);
        switch (op)
        {
        case 1:
            mostrarEquipamientoPorFaccion(&listaEquipamiento, -1);
            break;
        case 2:
            mostrarEquipamientoPorFaccion(&listaEquipamiento, ID_FAC_ANGELES);
            break;
        case 3:
            mostrarEquipamientoPorFaccion(&listaEquipamiento, ID_FAC_KOMMANDOS);
            break;
        case 4:
            mostrarEquipamientoPorFaccion(&listaEquipamiento, ID_FAC_MANDRAKES);
            break;
        case 5:
            mostrarEquipamientoPorFaccion(&listaEquipamiento, ID_FAC_TROUPE);
            break;
        case 0:
            break;
        default:
            cout << "Opcion invalida." << endl;
        }
    } while (op != 0);
}

void menuCatalogoOperativos()
{
    int op;
    do
    {
        mostrarTitulo("CATALOGO DE OPERATIVOS");
        cout << "  [1] Ver operativos por faccion" << endl;
        cout << "  [2] Capturar nueva unidad" << endl;
        cout << "  [3] Editar unidad existente" << endl;
        cout << "  [4] Eliminar unidad existente" << endl;
        cout << "  [0] Volver" << endl;
        lineaDecorativa(40);

        op = leerEnteroEnRango("Opcion: ", 0, 4);

        switch (op)
        {
        case 1:
        {
            mostrarTitulo("VER OPERATIVOS");
            cout << "  Seleccione faccion:" << endl;
            cout << "  [1] Angeles de la Muerte (" << COMP_ANGELES << " ops)" << endl;
            cout << "  [2] Kommandos (" << COMP_KOMMANDOS << " ops)" << endl;
            cout << "  [3] Mandrakes (" << COMP_MANDRAKES << " ops)" << endl;
            cout << "  [4] Tropa Danzante Vacia (" << COMP_TROUPE << " ops)" << endl;
            cout << "  [0] Volver" << endl;
            lineaDecorativa(40);

            int fac = leerEnteroEnRango("Opcion: ", 0, 4);
            if (fac >= 1 && fac <= 4)
            {
                mostrarOperativosPorFaccion(&listaOperativos, fac - 1);
                pausarPantalla();
            }
            break;
        }
        case 2:
            capturarOperativoPorTeclado(&listaOperativos);
            pausarPantalla();
            break;
        case 3:
            menuEditarOperativo(&listaOperativos);
            pausarPantalla();
            break;
        case 4:
            menuEliminarOperativo(&listaOperativos);
            pausarPantalla();
            break;
        case 0:
            break;
        }
    } while (op != 0);
}

void rosterBuilder()
{
    mostrarTitulo("ROSTER BUILDER");
    cout << "  Seleccione faccion para su lista:" << endl;
    cout << "  [1] Angeles de la Muerte (" << COMP_ANGELES << " operativos)" << endl;
    cout << "  [2] Kommandos (" << COMP_KOMMANDOS << " operativos)" << endl;
    cout << "  [3] Mandrakes (" << COMP_MANDRAKES << " operativos)" << endl;
    cout << "  [4] Tropa Danzante Vacia (" << COMP_TROUPE << " operativos)" << endl;
    cout << "  [0] Cancelar" << endl;
    lineaDecorativa(40);

    int faccionSeleccionada = leerEnteroEnRango("Opcion: ", 0, 4);

    if (faccionSeleccionada == 0)
    {
        mostrarMensajeAdvertencia("Operacion cancelada.");
        return;
    }

    int faccionId = faccionSeleccionada - 1;
    int maxOps = obtenerComposicionFaccion(faccionId);

    cout << "\nFaccion: ";
    mostrarNombreFaccion(faccionId);
    cout << endl;
    cout << "Requiere exactamente " << maxOps << " operativos (incluyendo 1 LIDER)." << endl;

    // Mostrar operativos disponibles
    mostrarOperativosPorFaccion(&listaOperativos, faccionId);

    DatosRoster nuevoRoster;
    nuevoRoster.faccionId = faccionId;
    nuevoRoster.cantidadOperativos = 0;
    nuevoRoster.cantidadEquipamiento = 0;
    nuevoRoster.id = listaRosters.cantidad + 1;
    nuevoRoster.fechaCreacion = time(NULL);

    // Inicializar equipamiento a -1
    for (int i = 0; i < MAX_OPERATIVOS_POR_ROSTER; i++)
    {
        nuevoRoster.idsEquipamientoPorOperativo[i] = -1;
    }

    leerCadenaSegura("Ingrese nombre de la lista: ", nuevoRoster.nombre, TAM_NOMBRE);

    // Seleccionar operativos
    int tieneLider = 0;
    while (nuevoRoster.cantidadOperativos < maxOps)
    {
        char mensaje[100];
        sprintf(mensaje, "Seleccione operativo %d/%d (ID, 0=terminar): ", nuevoRoster.cantidadOperativos + 1, maxOps);
        int idOp = leerEntero(mensaje);

        if (idOp == 0)
            break;

        NodoOperativo *op = buscarOperativoPorId(&listaOperativos, idOp);
        if (op == NULL)
        {
            mostrarMensajeError("ID invalido.");
            continue;
        }

        if (op->datos.faccionId != faccionId)
        {
            mostrarMensajeError("Ese operativo no pertenece a la faccion seleccionada.");
            continue;
        }

        // Verificar que no este repetido
        bool repetido = false;
        for (int i = 0; i < nuevoRoster.cantidadOperativos; i++)
        {
            if (nuevoRoster.idsOperativos[i] == idOp)
            {
                repetido = true;
                break;
            }
        }
        if (repetido)
        {
            mostrarMensajeError("Operativo ya seleccionado.");
            continue;
        }

        if (op->datos.esLider)
        {
            if (tieneLider)
            {
                mostrarMensajeError("Solo puede haber 1 LIDER.");
                continue;
            }
            tieneLider = 1;
        }

        nuevoRoster.idsOperativos[nuevoRoster.cantidadOperativos] = idOp;
        nuevoRoster.cantidadOperativos++;
        mostrarMensajeExito(op->datos.nombre);
    }

    // Validar composicion
    if (!validarComposicionRoster(faccionId, nuevoRoster.cantidadOperativos, tieneLider))
    {
        cout << "Lista invalida. No se guardo." << endl;
        return;
    }

    // Asignar equipamiento por operativo
    mostrarSubtitulo("ASIGNAR EQUIPAMIENTO");
    cout << "  Maximo " << MAX_EQUIPAMIENTO_POR_LISTA << " equipamientos (1 por operativo)." << endl;

    int equipamientoAsignado = 0;
    for (int i = 0; i < nuevoRoster.cantidadOperativos && equipamientoAsignado < MAX_EQUIPAMIENTO_POR_LISTA; i++)
    {
        NodoOperativo *op = buscarOperativoPorId(&listaOperativos, nuevoRoster.idsOperativos[i]);
        if (op == NULL)
            continue;

        cout << "\nOperativo: " << op->datos.nombre;
        if (op->datos.esLider)
            cout << " [LIDER]";
        cout << endl;

        if (!confirmarAccion("Asignar equipamiento a este operativo"))
            continue;

        mostrarEquipamientoPorFaccion(&listaEquipamiento, faccionId);

        int idEq = leerEntero("Seleccione equipamiento (ID, 0=ninguno): ");
        if (idEq == 0)
            continue;

        NodoEquipamiento *eq = buscarEquipamientoPorId(&listaEquipamiento, idEq);
        if (eq == NULL)
        {
            mostrarMensajeError("ID invalido.");
            i--; // Repetir este operativo
            continue;
        }

        // Validar que sea universal o de la faccion
        if (eq->datos.faccionId != -1 && eq->datos.faccionId != faccionId)
        {
            mostrarMensajeError("Ese equipamiento no es valido para esta faccion.");
            i--; // Repetir este operativo
            continue;
        }

        // Guardar en el roster: posicion i corresponde al operativo i
        nuevoRoster.idsEquipamientoPorOperativo[i] = idEq;
        equipamientoAsignado++;
        mostrarMensajeExito(eq->datos.nombre);
    }

    // Limpiar equipamiento no asignado
    for (int j = nuevoRoster.cantidadOperativos; j < MAX_OPERATIVOS_POR_ROSTER; j++)
    {
        nuevoRoster.idsEquipamientoPorOperativo[j] = -1;
    }

    nuevoRoster.cantidadEquipamiento = equipamientoAsignado;

    // Mostrar resumen y confirmar
    mostrarResumenRoster(&listaOperativos, &listaEquipamiento, &nuevoRoster);

    if (confirmarAccion("Guardar esta lista"))
    {
        agregarRoster(&listaRosters, nuevoRoster);
        guardarRosters(&listaRosters);
        mostrarMensajeExito("Lista guardada exitosamente!");
    }
    else
    {
        mostrarMensajeAdvertencia("Lista descartada.");
    }
}

void menuListasGuardadas()
{
    int op;
    do
    {
        mostrarTitulo("LISTAS GUARDADAS");
        cout << "  [1] Ver todas las listas" << endl;
        cout << "  [2] Ver detalle de lista" << endl;
        cout << "  [3] Editar lista (intercambiar unidades)" << endl;
        cout << "  [4] Eliminar lista" << endl;
        cout << "  [5] Exportar lista a .txt" << endl;
        cout << "  [0] Volver" << endl;
        lineaDecorativa(40);
        op = leerEnteroEnRango("Opcion: ", 0, 5);

        switch (op)
        {
        case 1:
            mostrarRostersGuardados(&listaOperativos, &listaRosters);
            pausarPantalla();
            break;
        case 2:
        {
            mostrarRostersGuardados(&listaOperativos, &listaRosters);
            if (listaRosters.cantidad > 0)
            {
                int id = leerEntero("Ingrese ID de la lista: ");
                verDetalleRoster(&listaOperativos, &listaEquipamiento, &listaRosters, id);
                pausarPantalla();
            }
            break;
        }
        case 3:
            menuEditarRoster(&listaOperativos, &listaEquipamiento, &listaRosters);
            pausarPantalla();
            break;
        case 4:
        {
            mostrarRostersGuardados(&listaOperativos, &listaRosters);
            if (listaRosters.cantidad > 0)
            {
                int id = leerEntero("Ingrese ID de la lista a eliminar: ");
                if (confirmarAccion("Esta seguro de que desea eliminar esta lista"))
                {
                    if (eliminarRosterCompleto(&listaRosters, id))
                    {
                        mostrarMensajeExito("Lista eliminada exitosamente.");
                        reconstruirListaRostersDesdeArchivo(&listaRosters);
                    }
                    else
                    {
                        mostrarMensajeError("Error al eliminar la lista.");
                    }
                }
                else
                {
                    mostrarMensajeAdvertencia("Eliminacion cancelada.");
                }
            }
            pausarPantalla();
            break;
        }
        case 5:
        {
            mostrarRostersGuardados(&listaOperativos, &listaRosters);
            if (listaRosters.cantidad > 0)
            {
                int id = leerEntero("Ingrese ID de la lista a exportar: ");
                exportarRosterATexto(&listaOperativos, &listaEquipamiento, &listaRosters, id);
                pausarPantalla();
            }
            break;
        }
        case 0:
            break;
        }
    } while (op != 0);
}

int main()
{
    // Inicializar listas
    inicializarListaOperativos(&listaOperativos);
    inicializarListaEquipamiento(&listaEquipamiento);
    inicializarListaRosters(&listaRosters);

    // Cargar datos si existen
    bool opsCargados = cargarOperativos(&listaOperativos);
    bool equipCargado = cargarEquipamiento(&listaEquipamiento);
    cargarRosters(&listaRosters);

    // Si no hay datos, precargar catalogos
    if (!opsCargados || listaOperativos.cantidad == 0)
    {
        mostrarMensajeExito("Precargando catalogo de operativos...");
        inicializarCatalogoCompleto(&listaOperativos);
    }

    if (!equipCargado || listaEquipamiento.cantidad == 0)
    {
        mostrarMensajeExito("Precargando equipamiento...");
        inicializarEquipamientoCompleto(&listaEquipamiento);
    }

    int opcion;
    do
    {
        mostrarMenuPrincipal();
        opcion = leerEnteroEnRango("", 0, 5);

        switch (opcion)
        {
        case 1:
            menuCatalogoOperativos();
            break;
        case 2:
            menuEquipamiento();
            break;
        case 3:
            rosterBuilder();
            pausarPantalla();
            break;
        case 4:
            menuListasGuardadas();
            break;
        case 5:
            menuBusquedaAvanzada(&listaOperativos);
            pausarPantalla();
            break;
        case 0:
            mostrarSubtitulo("GUARDANDO DATOS");
            guardarOperativos(&listaOperativos);
            guardarEquipamiento(&listaEquipamiento);
            guardarRosters(&listaRosters);
            mostrarMensajeExito("Datos guardados. Hasta la proxima!");
            break;
        default:
            mostrarMensajeError("Opcion invalida.");
        }
    } while (opcion != 0);

    // Liberar memoria
    liberarListaOperativos(&listaOperativos);
    liberarListaEquipamiento(&listaEquipamiento);
    liberarListaRosters(&listaRosters);

    return 0;
}
