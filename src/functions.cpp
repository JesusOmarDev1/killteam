#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "structures.h"
#include "interface.cpp"

using namespace std;

// ============================================
// ARCHIVOS BINARIOS
// ============================================
#define ARCHIVO_UNIDADES "unidades.dat"
#define ARCHIVO_EQUIPAMIENTO "equipamiento.dat"
#define ARCHIVO_LISTAS "listas.dat"

// ============================================
// LISTAS ENLAZADAS SIMPLES
// ============================================

void inicializarListaOperativos(ListaOperativos *lista)
{
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->cantidad = 0;
    lista->siguienteId = 1;
}

void inicializarListaEquipamiento(ListaEquipamiento *lista)
{
    lista->cabeza = NULL;
    lista->cantidad = 0;
}

void inicializarListaRosters(ListaRosters *lista)
{
    lista->cabeza = NULL;
    lista->cantidad = 0;
}

// ============================================
// CREAR NODOS
// ============================================

NodoOperativo *crearNodoOperativo(DatosOperativo datos)
{
    NodoOperativo *nuevo = (NodoOperativo *)malloc(sizeof(NodoOperativo));
    if (nuevo == NULL)
    {
        cout << "Error: No se pudo asignar memoria." << endl;
        return NULL;
    }
    nuevo->siguiente = NULL;
    nuevo->datos = datos;
    return nuevo;
}

NodoEquipamiento *crearNodoEquipamiento(DatosEquipamiento datos)
{
    NodoEquipamiento *nuevo = (NodoEquipamiento *)malloc(sizeof(NodoEquipamiento));
    if (nuevo == NULL)
    {
        cout << "Error: No se pudo asignar memoria." << endl;
        return NULL;
    }
    nuevo->siguiente = NULL;
    nuevo->datos = datos;
    return nuevo;
}

NodoRoster *crearNodoRoster(DatosRoster datos)
{
    NodoRoster *nuevo = (NodoRoster *)malloc(sizeof(NodoRoster));
    if (nuevo == NULL)
    {
        cout << "Error: No se pudo asignar memoria." << endl;
        return NULL;
    }
    nuevo->siguiente = NULL;
    nuevo->datos = datos;
    return nuevo;
}

// ============================================
// AGREGAR A LISTAS
// ============================================

void agregarOperativo(ListaOperativos *lista, DatosOperativo datos)
{
    NodoOperativo *nuevo = crearNodoOperativo(datos);
    if (nuevo == NULL)
        return;

    if (lista->cabeza == NULL)
    {
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    }
    else
    {
        lista->cola->siguiente = nuevo;
        lista->cola = nuevo;
    }
    lista->cantidad++;
    lista->siguienteId++;
}

void agregarEquipamiento(ListaEquipamiento *lista, DatosEquipamiento datos)
{
    NodoEquipamiento *nuevo = crearNodoEquipamiento(datos);
    if (nuevo == NULL)
        return;

    if (lista->cabeza == NULL)
    {
        lista->cabeza = nuevo;
    }
    else
    {
        NodoEquipamiento *actual = lista->cabeza;
        while (actual->siguiente != NULL)
        {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    lista->cantidad++;
}

void agregarRoster(ListaRosters *lista, DatosRoster datos)
{
    NodoRoster *nuevo = crearNodoRoster(datos);
    if (nuevo == NULL)
        return;

    if (lista->cabeza == NULL)
    {
        lista->cabeza = nuevo;
    }
    else
    {
        NodoRoster *actual = lista->cabeza;
        while (actual->siguiente != NULL)
        {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    lista->cantidad++;
}

// ============================================
// BUSCAR POR ID
// ============================================

NodoOperativo *buscarOperativoPorId(ListaOperativos *lista, int id)
{
    NodoOperativo *actual = lista->cabeza;
    while (actual != NULL)
    {
        if (actual->datos.id == id)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

NodoEquipamiento *buscarEquipamientoPorId(ListaEquipamiento *lista, int id)
{
    NodoEquipamiento *actual = lista->cabeza;
    while (actual != NULL)
    {
        if (actual->datos.id == id)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

NodoRoster *buscarRosterPorId(ListaRosters *lista, int id)
{
    NodoRoster *actual = lista->cabeza;
    while (actual != NULL)
    {
        if (actual->datos.id == id)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

// ============================================
// PERSISTENCIA BINARIA - GUARDAR (fwrite)
// ============================================

bool guardarOperativos(ListaOperativos *lista)
{
    FILE *archivo = fopen(ARCHIVO_UNIDADES, "wb");
    if (archivo == NULL)
    {
        cout << "Error: No se pudo abrir " << ARCHIVO_UNIDADES << endl;
        return false;
    }

    NodoOperativo *actual = lista->cabeza;
    while (actual != NULL)
    {
        fwrite(&(actual->datos), sizeof(DatosOperativo), 1, archivo);
        actual = actual->siguiente;
    }

    fclose(archivo);
    return true;
}

bool guardarEquipamiento(ListaEquipamiento *lista)
{
    FILE *archivo = fopen(ARCHIVO_EQUIPAMIENTO, "wb");
    if (archivo == NULL)
    {
        cout << "Error: No se pudo abrir " << ARCHIVO_EQUIPAMIENTO << endl;
        return false;
    }

    NodoEquipamiento *actual = lista->cabeza;
    while (actual != NULL)
    {
        fwrite(&(actual->datos), sizeof(DatosEquipamiento), 1, archivo);
        actual = actual->siguiente;
    }

    fclose(archivo);
    return true;
}

bool guardarRosters(ListaRosters *lista)
{
    FILE *archivo = fopen(ARCHIVO_LISTAS, "wb");
    if (archivo == NULL)
    {
        cout << "Error: No se pudo abrir " << ARCHIVO_LISTAS << endl;
        return false;
    }

    NodoRoster *actual = lista->cabeza;
    while (actual != NULL)
    {
        fwrite(&(actual->datos), sizeof(DatosRoster), 1, archivo);
        actual = actual->siguiente;
    }

    fclose(archivo);
    return true;
}

// ============================================
// PERSISTENCIA BINARIA - CARGAR (fread)
// ============================================

bool cargarOperativos(ListaOperativos *lista)
{
    FILE *archivo = fopen(ARCHIVO_UNIDADES, "rb");
    if (archivo == NULL)
    {
        return false;
    }

    DatosOperativo temp;
    while (fread(&temp, sizeof(DatosOperativo), 1, archivo) == 1)
    {
        agregarOperativoOrdenado(lista, temp);
    }

    fclose(archivo);
    return true;
}

bool cargarEquipamiento(ListaEquipamiento *lista)
{
    FILE *archivo = fopen(ARCHIVO_EQUIPAMIENTO, "rb");
    if (archivo == NULL)
    {
        return false;
    }

    DatosEquipamiento temp;
    while (fread(&temp, sizeof(DatosEquipamiento), 1, archivo) == 1)
    {
        agregarEquipamiento(lista, temp);
    }

    fclose(archivo);
    return true;
}

bool cargarRosters(ListaRosters *lista)
{
    FILE *archivo = fopen(ARCHIVO_LISTAS, "rb");
    if (archivo == NULL)
    {
        return false;
    }

    DatosRoster temp;
    while (fread(&temp, sizeof(DatosRoster), 1, archivo) == 1)
    {
        agregarRoster(lista, temp);
    }

    fclose(archivo);
    return true;
}

// ============================================
// EDICION DIRECTA POR OFFSET (EXTRA)
// ============================================

bool editarOperativoPorOffset(int offset, DatosOperativo nuevosDatos)
{
    FILE *archivo = fopen(ARCHIVO_UNIDADES, "r+b");
    if (archivo == NULL)
    {
        return false;
    }

    fseek(archivo, offset * sizeof(DatosOperativo), SEEK_SET);
    fwrite(&nuevosDatos, sizeof(DatosOperativo), 1, archivo);
    fclose(archivo);
    return true;
}

// ============================================
// ELIMINAR (reconstruye archivo)
// ============================================

bool eliminarOperativoDeArchivo(int id)
{
    FILE *original = fopen(ARCHIVO_UNIDADES, "rb");
    if (original == NULL)
        return false;

    FILE *temporal = fopen("temp.dat", "wb");
    if (temporal == NULL)
    {
        fclose(original);
        return false;
    }

    DatosOperativo temp;
    while (fread(&temp, sizeof(DatosOperativo), 1, original) == 1)
    {
        if (temp.id != id)
        {
            fwrite(&temp, sizeof(DatosOperativo), 1, temporal);
        }
    }

    fclose(original);
    fclose(temporal);

    remove(ARCHIVO_UNIDADES);
    rename("temp.dat", ARCHIVO_UNIDADES);
    return true;
}

// ============================================
// LIBERAR MEMORIA (free)
// ============================================

void liberarListaOperativos(ListaOperativos *lista)
{
    NodoOperativo *actual = lista->cabeza;
    while (actual != NULL)
    {
        NodoOperativo *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->cantidad = 0;
}

void liberarListaEquipamiento(ListaEquipamiento *lista)
{
    NodoEquipamiento *actual = lista->cabeza;
    while (actual != NULL)
    {
        NodoEquipamiento *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    lista->cabeza = NULL;
    lista->cantidad = 0;
}

void liberarListaRosters(ListaRosters *lista)
{
    NodoRoster *actual = lista->cabeza;
    while (actual != NULL)
    {
        NodoRoster *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    lista->cabeza = NULL;
    lista->cantidad = 0;
}

// ============================================
// SISTEMA DE EQUIPAMIENTO
// ============================================

void crearDatosEquipamiento(DatosEquipamiento *eq, int id, int faccionId, const char *nombre, const char *descripcion)
{
    eq->id = id;
    eq->faccionId = faccionId;
    strncpy(eq->nombre, nombre, TAM_NOMBRE - 1);
    eq->nombre[TAM_NOMBRE - 1] = '\0';
    strncpy(eq->descripcion, descripcion, TAM_DESCRIPCION - 1);
    eq->descripcion[TAM_DESCRIPCION - 1] = '\0';
}

void precargarEquipamientoUniversal(ListaEquipamiento *lista)
{
    DatosEquipamiento eq;

    crearDatosEquipamiento(&eq, 1, -1, "Deposito de Municion", "Reabastecimiento: repetir dado de ataque al disparar");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 2, -1, "Alambre de Cuchillas", "Terreno Expuesto/Obstructor, cruzar suma 2 pulgadas");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 3, -1, "Dispositivo de Comunicaciones", "Suma 3 pulgadas a reglas de APOYO");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 4, -1, "Minas", "D3+3 dano al estar en alcance de control");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 5, -1, "Barricadas Ligeras", "Terreno Ligero");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 6, -1, "Barricada Pesada", "Terreno Pesado");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 7, -1, "Barricada Portatil", "Protector (+1 Salv), Portatil, Mover con Barricada (1AP)");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 8, -1, "Escaleras", "Trepar: distancia vertical cuenta como 1 pulgada");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 9, -1, "Granada de Utilidad", "Selecciona 2: Humo (ocultar) / Aturdidora (restar APL)");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 10, -1, "Granada Explosiva", "Fragmentacion (4A,4+,2/4,Explosion 2 pulg,Saturar) / Antiblindaje (4A,4+,4/5,Perf 1,Saturar)");
    agregarEquipamiento(lista, eq);
}

void precargarEquipamientoFaccion(ListaEquipamiento *lista)
{
    DatosEquipamiento eq;

    // Angeles de la Muerte (faccionId = 0)
    crearDatosEquipamiento(&eq, 101, ID_FAC_ANGELES, "Sellos de Pureza", "Ignora modificadores negativos a salvacion");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 102, ID_FAC_ANGELES, "Reliquias de Capitulo", "Mejora una regla de capitol en +1");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 103, ID_FAC_ANGELES, "Escudos Inclinados", "+1 defensa contra ataques de rango");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 104, ID_FAC_ANGELES, "Auspex", "Revela operativos ocultos a 6 pulgadas");
    agregarEquipamiento(lista, eq);

    // Kommandos (faccionId = 1)
    crearDatosEquipamiento(&eq, 201, ID_FAC_KOMMANDOS, "Choppas", "+1 ataque en combate cuerpo a cuerpo");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 202, ID_FAC_KOMMANDOS, "Arpon", "Ataque de rango 6 pulg, 3+, 3/4");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 203, ID_FAC_KOMMANDOS, "Dinamita", "Explosion 2 pulg, D6 de dano, un solo uso");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 204, ID_FAC_KOMMANDOS, "Culatas Plegables", "+1 movimiento, ignora terreno dificil");
    agregarEquipamiento(lista, eq);

    // Mandrakes (faccionId = 2)
    crearDatosEquipamiento(&eq, 301, ID_FAC_MANDRAKES, "Cadenas Garfio", "GolpeALMA: +1 dano contra objetivos heridos");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 302, ID_FAC_MANDRAKES, "Glifo de Sombra", "Reubicarse dentro de sombra al activar");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 303, ID_FAC_MANDRAKES, "Gema de Alma", "Recupera 1 herida al eliminar un enemigo");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 304, ID_FAC_MANDRAKES, "Dardos de Hueso", "Veneno: resta 1 APL del objetivo");
    agregarEquipamiento(lista, eq);

    // Void-Dancer Troupe (faccionId = 3)
    crearDatosEquipamiento(&eq, 401, ID_FAC_TROUPE, "Talisman de Hueso Espectral", "Ignora Perforante 1 vez por turno");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 402, ID_FAC_TROUPE, "Municion Toxina Chirriadora", "+1 dano critico, rango +2 pulg");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 403, ID_FAC_TROUPE, "Mascara Mortuoria", "Terror: enemigos a 3 pulg -1 impacto");
    agregarEquipamiento(lista, eq);

    crearDatosEquipamiento(&eq, 404, ID_FAC_TROUPE, "Mascara del Suplente", "Intercambiar posicion con aliado a 6 pulg");
    agregarEquipamiento(lista, eq);
}

void mostrarEquipamientoPorFaccion(ListaEquipamiento *lista, int faccionId)
{
    NodoEquipamiento *actual = lista->cabeza;
    bool hay = false;

    while (actual != NULL)
    {
        if (actual->datos.faccionId == faccionId || actual->datos.faccionId == -1)
        {
            if (!hay)
            {
                cout << "\n=== EQUIPAMIENTO ";
                if (faccionId == -1)
                    cout << "UNIVERSAL";
                else if (faccionId == ID_FAC_ANGELES)
                    cout << "ANGELES DE LA MUERTE";
                else if (faccionId == ID_FAC_KOMMANDOS)
                    cout << "KOMMANDOS";
                else if (faccionId == ID_FAC_MANDRAKES)
                    cout << "MANDRAKES";
                else if (faccionId == ID_FAC_TROUPE)
                    cout << "TROPA DANZANTE VACIA";
                cout << " ===" << endl;
                hay = true;
            }
            cout << "[" << actual->datos.id << "] " << actual->datos.nombre << endl;
            cout << "    " << actual->datos.descripcion << endl;
        }
        actual = actual->siguiente;
    }

    if (!hay)
    {
        cout << "No hay equipamiento disponible." << endl;
    }
}

bool asignarEquipamientoARoster(DatosRoster *roster, int idxOperativo, int idEquipamiento)
{
    if (idxOperativo < 0 || idxOperativo >= MAX_OPERATIVOS_POR_ROSTER)
        return false;

    roster->idsEquipamientoPorOperativo[idxOperativo] = idEquipamiento;
    roster->cantidadEquipamiento++;
    return true;
}

void mostrarEquipamientoDeRoster(ListaEquipamiento *catalogo, DatosRoster *roster)
{
    if (roster->cantidadEquipamiento == 0)
    {
        cout << "Sin equipamiento asignado." << endl;
        return;
    }

    cout << "\n=== EQUIPAMIENTO DE LA LISTA ===" << endl;
    for (int i = 0; i < roster->cantidadOperativos; i++)
    {
        if (roster->idsEquipamientoPorOperativo[i] != -1)
        {
            NodoEquipamiento *eq = buscarEquipamientoPorId(catalogo, roster->idsEquipamientoPorOperativo[i]);
            if (eq != NULL)
            {
                cout << "- " << eq->datos.nombre;
                if (eq->datos.faccionId == -1)
                    cout << " [Universal]";
                else
                    cout << " [Faccion]";
                cout << endl;
            }
        }
    }
}

void inicializarEquipamientoCompleto(ListaEquipamiento *lista)
{
    precargarEquipamientoUniversal(lista);
    precargarEquipamientoFaccion(lista);
    guardarEquipamiento(lista);
}

// ============================================
// ROSTER BUILDER CON REGLAS REALES
// ============================================

void agregarArmaAOperativo(DatosOperativo *op, const char *nombre, TipoArma tipo,
                           int ataque, int impacto, int dano, int danoCritico,
                           int alcance, int amenaza)
{
    if (op->cantidadArmas >= MAX_ARMAS)
    {
        cout << "Advertencia: operativo " << op->nombre << " ya tiene maximo de armas." << endl;
        return;
    }

    Arma *a = &op->armas[op->cantidadArmas];
    strncpy(a->nombre, nombre, TAM_NOMBRE - 1);
    a->nombre[TAM_NOMBRE - 1] = '\0';
    a->tipo = tipo;
    a->ataque = ataque;
    a->impacto = impacto;
    a->dano = dano;
    a->danoCritico = danoCritico;
    a->alcance = alcance;
    a->amenaza = amenaza;
    a->cantidadReglas = 0;

    op->cantidadArmas++;
}

void crearDatosOperativo(DatosOperativo *op, int id, const char *nombre, const char *faccion, int faccionId, int esLider, int mov, int apl, int salv, int heridas, int defensa, int puntos, const char *arquetipo)
{
    op->id = id;
    op->faccionId = faccionId;
    op->esLider = esLider;
    op->movimiento = mov;
    op->apl = apl;
    op->salvacion = salv;
    op->heridas = heridas;
    op->defensa = defensa;
    op->puntos = puntos;
    op->cantidadArmas = 0;
    op->cantidadKeywords = 0;
    op->cantidadHabilidades = 0;
    op->idEquipamiento = -1;

    strncpy(op->nombre, nombre, TAM_NOMBRE - 1);
    op->nombre[TAM_NOMBRE - 1] = '\0';
    strncpy(op->faccion, faccion, TAM_FACCION - 1);
    op->faccion[TAM_FACCION - 1] = '\0';
    strncpy(op->arquetipo, arquetipo, TAM_ARQUETIPO - 1);
    op->arquetipo[TAM_ARQUETIPO - 1] = '\0';
}

void precargarCatalogoOperativos(ListaOperativos *lista)
{
    DatosOperativo op;
    int id = 1;

    // ============================================
    // ANGELES DE LA MUERTE (6: 1 lider + 5)
    // ============================================
    crearDatosOperativo(&op, id++, "Capitan", "Angeles de la Muerte", ID_FAC_ANGELES, 1, 3, 3, 3, 14, 3, 70, "Lider");
    agregarArmaAOperativo(&op, "Rifle de Asalto de Capitan", ARMA_RANGO, 4, 3, 3, 4, 24, 1);
    agregarArmaAOperativo(&op, "Punial de Combate", ARMA_MELEE, 4, 2, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Sargento Intercesor", "Angeles de la Muerte", ID_FAC_ANGELES, 0, 3, 2, 3, 13, 3, 20, "Subalterno");
    agregarArmaAOperativo(&op, "Rifle de Asalto", ARMA_RANGO, 4, 3, 3, 4, 24, 1);
    agregarArmaAOperativo(&op, "Punial de Combate", ARMA_MELEE, 3, 3, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Intercesor", "Angeles de la Muerte", ID_FAC_ANGELES, 0, 3, 2, 3, 13, 3, 18, "Linea");
    agregarArmaAOperativo(&op, "Rifle de Asalto", ARMA_RANGO, 4, 3, 3, 4, 24, 1);
    agregarArmaAOperativo(&op, "Punial de Combate", ARMA_MELEE, 3, 3, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Intercesor", "Angeles de la Muerte", ID_FAC_ANGELES, 0, 3, 2, 3, 13, 3, 18, "Linea");
    agregarArmaAOperativo(&op, "Rifle de Asalto", ARMA_RANGO, 4, 3, 3, 4, 24, 1);
    agregarArmaAOperativo(&op, "Punial de Combate", ARMA_MELEE, 3, 3, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Intercesor de Asalto Granadero", "Angeles de la Muerte", ID_FAC_ANGELES, 0, 3, 2, 3, 13, 3, 22, "Especialista");
    agregarArmaAOperativo(&op, "Rifle con Lanzagranadas", ARMA_RANGO, 4, 3, 3, 4, 24, 2);
    agregarArmaAOperativo(&op, "Punial de Combate", ARMA_MELEE, 3, 3, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Francotirador Eliminator", "Angeles de la Muerte", ID_FAC_ANGELES, 0, 3, 2, 3, 12, 3, 25, "Francotirador");
    agregarArmaAOperativo(&op, "Rifle Francotirador", ARMA_RANGO, 4, 2, 4, 5, 36, 1);
    agregarArmaAOperativo(&op, "Punial de Combate", ARMA_MELEE, 3, 3, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    // ============================================
    // KOMMANDOS (10: 1 lider + 9)
    // ============================================
    crearDatosOperativo(&op, id++, "Boss Nob", "Kommandos", ID_FAC_KOMMANDOS, 1, 3, 3, 5, 14, 4, 60, "Lider");
    agregarArmaAOperativo(&op, "Choppa", ARMA_MELEE, 5, 3, 4, 5, 0, 1);
    agregarArmaAOperativo(&op, "Slugga", ARMA_RANGO, 4, 4, 3, 4, 12, 1);
    agregarOperativoOrdenado(lista, op);

    for (int i = 0; i < 6; i++)
    {
        crearDatosOperativo(&op, id++, "Kommando Boy", "Kommandos", ID_FAC_KOMMANDOS, 0, 3, 2, 5, 10, 4, 12, "Linea");
        agregarArmaAOperativo(&op, "Shoota", ARMA_RANGO, 4, 4, 3, 4, 18, 1);
        agregarArmaAOperativo(&op, "Choppa", ARMA_MELEE, 3, 4, 3, 4, 0, 1);
        agregarOperativoOrdenado(lista, op);
    }

    crearDatosOperativo(&op, id++, "Squig Bomba", "Kommandos", ID_FAC_KOMMANDOS, 0, 4, 2, 6, 8, 5, 15, "Especialista");
    agregarArmaAOperativo(&op, "Bomba de Squig", ARMA_RANGO, 1, 4, 5, 6, 6, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Rompedor Breacha", "Kommandos", ID_FAC_KOMMANDOS, 0, 3, 2, 5, 12, 4, 18, "Asalto");
    agregarArmaAOperativo(&op, "Breacha", ARMA_MELEE, 4, 3, 4, 5, 0, 2);
    agregarArmaAOperativo(&op, "Slugga", ARMA_RANGO, 4, 4, 3, 4, 12, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Snipa Boy", "Kommandos", ID_FAC_KOMMANDOS, 0, 3, 2, 5, 10, 4, 16, "Francotirador");
    agregarArmaAOperativo(&op, "Snipa", ARMA_RANGO, 4, 3, 4, 5, 30, 1);
    agregarArmaAOperativo(&op, "Choppa", ARMA_MELEE, 3, 4, 3, 4, 0, 1);
    agregarOperativoOrdenado(lista, op);

    // ============================================
    // MANDRAKES (9: 1 lider + 8)
    // ============================================
    crearDatosOperativo(&op, id++, "Aterrorizador Nocturno", "Mandrakes", ID_FAC_MANDRAKES, 1, 3, 3, 4, 12, 4, 55, "Lider");
    agregarArmaAOperativo(&op, "Hoja de Sombra", ARMA_MELEE, 5, 3, 4, 5, 0, 1);
    agregarArmaAOperativo(&op, "Dardos de Hueso", ARMA_RANGO, 3, 3, 3, 4, 12, 1);
    agregarOperativoOrdenado(lista, op);

    for (int i = 0; i < 2; i++)
    {
        crearDatosOperativo(&op, id++, "Terror Nocturno", "Mandrakes", ID_FAC_MANDRAKES, 0, 3, 2, 4, 10, 4, 16, "Linea");
        agregarArmaAOperativo(&op, "Hoja de Sombra", ARMA_MELEE, 4, 3, 3, 4, 0, 1);
        agregarOperativoOrdenado(lista, op);
    }

    for (int i = 0; i < 2; i++)
    {
        crearDatosOperativo(&op, id++, "Abismal", "Mandrakes", ID_FAC_MANDRAKES, 0, 3, 2, 4, 10, 4, 16, "Linea");
        agregarArmaAOperativo(&op, "Hoja de Sombra", ARMA_MELEE, 4, 3, 3, 4, 0, 1);
        agregarOperativoOrdenado(lista, op);
    }

    for (int i = 0; i < 2; i++)
    {
        crearDatosOperativo(&op, id++, "Elegido de la Carne", "Mandrakes", ID_FAC_MANDRAKES, 0, 3, 2, 4, 10, 4, 18, "Asalto");
        agregarArmaAOperativo(&op, "Garras de Abismo", ARMA_MELEE, 5, 3, 4, 5, 0, 1);
        agregarOperativoOrdenado(lista, op);
    }

    for (int i = 0; i < 2; i++)
    {
        crearDatosOperativo(&op, id++, "Tejedor de Sombras", "Mandrakes", ID_FAC_MANDRAKES, 0, 3, 2, 4, 8, 4, 20, "Especialista");
        agregarArmaAOperativo(&op, "Garras de Abismo", ARMA_MELEE, 4, 3, 3, 4, 0, 1);
        agregarArmaAOperativo(&op, "Dardos de Hueso", ARMA_RANGO, 3, 3, 3, 4, 12, 1);
        agregarOperativoOrdenado(lista, op);
    }

    // ============================================
    // TROPA DANZANTE VACIA (8: 1 lider + 7)
    // ============================================
    crearDatosOperativo(&op, id++, "Actor Principal", "Tropa Danzante Vacia", ID_FAC_TROUPE, 1, 3, 3, 6, 10, 4, 50, "Lider");
    agregarArmaAOperativo(&op, "Espada Danzante", ARMA_MELEE, 5, 2, 4, 5, 0, 1);
    agregarArmaAOperativo(&op, "Pistola Shuriken", ARMA_RANGO, 4, 3, 3, 4, 12, 1);
    agregarOperativoOrdenado(lista, op);

    for (int i = 0; i < 2; i++)
    {
        crearDatosOperativo(&op, id++, "Vidente Sombra", "Tropa Danzante Vacia", ID_FAC_TROUPE, 0, 3, 2, 6, 9, 4, 18, "Psiquico");
        agregarArmaAOperativo(&op, "Baston de Sombra", ARMA_MELEE, 4, 3, 3, 4, 0, 1);
        agregarOperativoOrdenado(lista, op);
    }

    for (int i = 0; i < 3; i++)
    {
        crearDatosOperativo(&op, id++, "Bailarin", "Tropa Danzante Vacia", ID_FAC_TROUPE, 0, 3, 2, 6, 9, 4, 16, "Linea");
        agregarArmaAOperativo(&op, "Espada Danzante", ARMA_MELEE, 4, 3, 3, 4, 0, 1);
        agregarArmaAOperativo(&op, "Pistola Shuriken", ARMA_RANGO, 4, 3, 3, 4, 12, 1);
        agregarOperativoOrdenado(lista, op);
    }

    crearDatosOperativo(&op, id++, "Bufon Mortal", "Tropa Danzante Vacia", ID_FAC_TROUPE, 0, 3, 2, 6, 10, 4, 22, "Especialista");
    agregarArmaAOperativo(&op, "Beso de la Muerte", ARMA_MELEE, 4, 2, 3, 4, 0, 1);
    agregarArmaAOperativo(&op, "Pistola Shuriken", ARMA_RANGO, 4, 3, 3, 4, 12, 1);
    agregarOperativoOrdenado(lista, op);

    crearDatosOperativo(&op, id++, "Solitario", "Tropa Danzante Vacia", ID_FAC_TROUPE, 0, 4, 3, 6, 11, 4, 28, "Asesino");
    agregarArmaAOperativo(&op, "Aguijon de Sombra", ARMA_MELEE, 5, 2, 4, 5, 0, 1);
    agregarArmaAOperativo(&op, "Pistola Shuriken", ARMA_RANGO, 4, 3, 4, 5, 12, 1);
    agregarOperativoOrdenado(lista, op);

    guardarOperativos(lista);
}

void mostrarOperativosPorFaccion(ListaOperativos *lista, int faccionId)
{
    NodoOperativo *actual = lista->cabeza;
    bool hay = false;

    while (actual != NULL)
    {
        if (actual->datos.faccionId == faccionId)
        {
            if (!hay)
            {
                cout << "\n=== OPERATIVOS DISPONIBLES ===" << endl;
                hay = true;
            }
            cout << "[" << actual->datos.id << "] " << actual->datos.nombre;
            if (actual->datos.esLider)
                cout << " **LIDER**";
            cout << " (" << actual->datos.puntos << " pts)" << endl;
        }
        actual = actual->siguiente;
    }

    if (!hay)
    {
        cout << "No hay operativos disponibles para esta faccion." << endl;
    }
}

int obtenerComposicionFaccion(int faccionId)
{
    switch (faccionId)
    {
    case ID_FAC_ANGELES:
        return COMP_ANGELES;
    case ID_FAC_KOMMANDOS:
        return COMP_KOMMANDOS;
    case ID_FAC_MANDRAKES:
        return COMP_MANDRAKES;
    case ID_FAC_TROUPE:
        return COMP_TROUPE;
    default:
        return 0;
    }
}

bool validarComposicionRoster(int faccionId, int cantidadOperativos, int tieneLider)
{
    int requerido = obtenerComposicionFaccion(faccionId);

    if (cantidadOperativos != requerido)
    {
        cout << "Error: La faccion requiere exactamente " << requerido << " operativos." << endl;
        return false;
    }

    if (!tieneLider)
    {
        cout << "Error: Debe incluir exactamente 1 LIDER." << endl;
        return false;
    }

    return true;
}

void mostrarNombreFaccion(int faccionId)
{
    switch (faccionId)
    {
    case ID_FAC_ANGELES:
        cout << "Angeles de la Muerte";
        break;
    case ID_FAC_KOMMANDOS:
        cout << "Kommandos";
        break;
    case ID_FAC_MANDRAKES:
        cout << "Mandrakes";
        break;
    case ID_FAC_TROUPE:
        cout << "Tropa Danzante Vacia";
        break;
    default:
        cout << "Desconocida";
    }
}

int calcularPuntosRoster(ListaOperativos *catalogo, DatosRoster *roster)
{
    int total = 0;
    for (int i = 0; i < roster->cantidadOperativos; i++)
    {
        NodoOperativo *op = buscarOperativoPorId(catalogo, roster->idsOperativos[i]);
        if (op != NULL)
        {
            total += op->datos.puntos;
        }
    }
    return total;
}

void mostrarResumenRoster(ListaOperativos *catalogo, ListaEquipamiento *equipos, DatosRoster *roster)
{
    cout << "\n================================" << endl;
    cout << "     RESUMEN DEL ROSTER         " << endl;
    cout << "================================" << endl;

    // Validacion defensiva: si roster es NULL, solo mostrar encabezado
    if (roster == NULL)
        return;

    cout << "Nombre: " << roster->nombre << endl;
    cout << "Faccion: ";
    mostrarNombreFaccion(roster->faccionId);
    cout << endl;
    cout << "Operativos: " << roster->cantidadOperativos << "/" << obtenerComposicionFaccion(roster->faccionId) << endl;
    cout << "Puntos totales: " << calcularPuntosRoster(catalogo, roster) << endl;

    cout << "\n--- Operativos ---" << endl;
    int totalEquipamiento = 0;
    for (int i = 0; i < roster->cantidadOperativos; i++)
    {
        NodoOperativo *op = buscarOperativoPorId(catalogo, roster->idsOperativos[i]);
        if (op != NULL)
        {
            cout << "- " << op->datos.nombre;
            if (op->datos.esLider)
                cout << " [LIDER]";
            cout << " (" << op->datos.puntos << " pts)" << endl;

            // Mostrar equipamiento del operativo (desde el roster)
            if (roster->idsEquipamientoPorOperativo[i] != -1)
            {
                NodoEquipamiento *eq = buscarEquipamientoPorId(equipos, roster->idsEquipamientoPorOperativo[i]);
                if (eq != NULL)
                {
                    cout << "    Eq: " << eq->datos.nombre;
                    if (eq->datos.faccionId == -1)
                        cout << " [Universal]";
                    else
                        cout << " [Faccion]";
                    cout << endl;
                    totalEquipamiento++;
                }
            }

            // Mostrar armas del operativo
            for (int j = 0; j < op->datos.cantidadArmas; j++)
            {
                cout << "    Arma: " << op->datos.armas[j].nombre;
                if (op->datos.armas[j].tipo == ARMA_RANGO)
                    cout << " (Rango " << op->datos.armas[j].alcance << "\")";
                else
                    cout << " (Melee)";
                cout << endl;
            }
        }
    }

    cout << "\nEquipamiento total: " << totalEquipamiento << "/" << MAX_EQUIPAMIENTO_POR_LISTA << endl;
}

void inicializarCatalogoCompleto(ListaOperativos *lista)
{
    precargarCatalogoOperativos(lista);
    guardarOperativos(lista);
}

// ============================================
// GESTION DE LISTAS GUARDADAS
// ============================================

void formatearFecha(time_t fecha, char *buffer, int tamanoBuffer)
{
    struct tm *tiempoLocal = localtime(&fecha);
    strftime(buffer, tamanoBuffer, "%d/%m/%Y %H:%M", tiempoLocal);
}

void mostrarRostersGuardados(ListaOperativos *catalogo, ListaRosters *lista)
{
    if (lista->cabeza == NULL)
    {
        cout << "\nNo hay listas guardadas." << endl;
        return;
    }

    cout << "\n=== LISTAS GUARDADAS ===" << endl;
    NodoRoster *actual = lista->cabeza;
    while (actual != NULL)
    {
        char fechaFormateada[20];
        formatearFecha(actual->datos.fechaCreacion, fechaFormateada, 20);

        cout << "[" << actual->datos.id << "] " << actual->datos.nombre;
        cout << " | ";
        mostrarNombreFaccion(actual->datos.faccionId);
        cout << " | " << actual->datos.cantidadOperativos << " ops";
        cout << " | " << calcularPuntosRoster(catalogo, &(actual->datos)) << " pts";
        cout << " | " << fechaFormateada << endl;
        actual = actual->siguiente;
    }
}

bool buscarRosterPorNombre(ListaRosters *lista, const char *nombre, DatosRoster *resultado)
{
    NodoRoster *actual = lista->cabeza;
    while (actual != NULL)
    {
        if (strstr(actual->datos.nombre, nombre) != NULL)
        {
            *resultado = actual->datos;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool eliminarRosterDeArchivo(int idRoster)
{
    FILE *original = fopen(ARCHIVO_LISTAS, "rb");
    if (original == NULL)
        return false;

    FILE *temporal = fopen("temp_listas.dat", "wb");
    if (temporal == NULL)
    {
        fclose(original);
        return false;
    }

    DatosRoster temp;
    while (fread(&temp, sizeof(DatosRoster), 1, original) == 1)
    {
        if (temp.id != idRoster)
        {
            fwrite(&temp, sizeof(DatosRoster), 1, temporal);
        }
    }

    fclose(original);
    fclose(temporal);

    remove(ARCHIVO_LISTAS);
    rename("temp_listas.dat", ARCHIVO_LISTAS);
    return true;
}

void eliminarRosterDeLista(ListaRosters *lista, int idRoster)
{
    NodoRoster *actual = lista->cabeza;
    NodoRoster *anterior = NULL;

    while (actual != NULL)
    {
        if (actual->datos.id == idRoster)
        {
            if (anterior == NULL)
            {
                lista->cabeza = actual->siguiente;
            }
            else
            {
                anterior->siguiente = actual->siguiente;
            }
            free(actual);
            lista->cantidad--;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

bool eliminarRosterCompleto(ListaRosters *lista, int idRoster)
{
    eliminarRosterDeLista(lista, idRoster);
    return eliminarRosterDeArchivo(idRoster);
}

void verDetalleRoster(ListaOperativos *catalogo, ListaEquipamiento *equipos, ListaRosters *lista, int idRoster)
{
    NodoRoster *nodo = buscarRosterPorId(lista, idRoster);
    if (nodo == NULL)
    {
        cout << "Lista no encontrada." << endl;
        return;
    }

    char fechaFormateada[20];
    formatearFecha(nodo->datos.fechaCreacion, fechaFormateada, 20);

    cout << "\n=== DETALLE DE LISTA ===" << endl;
    cout << "ID: " << nodo->datos.id << endl;
    cout << "Nombre: " << nodo->datos.nombre << endl;
    cout << "Faccion: ";
    mostrarNombreFaccion(nodo->datos.faccionId);
    cout << endl;
    cout << "Fecha: " << fechaFormateada << endl;

    mostrarResumenRoster(catalogo, equipos, &(nodo->datos));
}

void reconstruirListaRostersDesdeArchivo(ListaRosters *lista)
{
    liberarListaRosters(lista);
    inicializarListaRosters(lista);
    cargarRosters(lista);
}

// ============================================
// BUSQUEDA AVANZADA
// ============================================

void buscarOperativosPorNombre(ListaOperativos *lista, const char *busqueda)
{
    NodoOperativo *actual = lista->cabeza;
    bool hay = false;

    cout << "\n=== RESULTADOS DE BUSQUEDA ===" << endl;
    cout << "Buscando: \"" << busqueda << "\"" << endl;

    while (actual != NULL)
    {
        if (strstr(actual->datos.nombre, busqueda) != NULL)
        {
            cout << "[" << actual->datos.id << "] " << actual->datos.nombre;
            cout << " | ";
            mostrarNombreFaccion(actual->datos.faccionId);
            cout << " | " << actual->datos.puntos << " pts";
            if (actual->datos.esLider)
                cout << " [LIDER]";
            cout << endl;
            hay = true;
        }
        actual = actual->siguiente;
    }

    if (!hay)
    {
        cout << "No se encontraron operativos con ese nombre." << endl;
    }
}

void buscarOperativosPorFaccion(ListaOperativos *lista, int faccionId)
{
    NodoOperativo *actual = lista->cabeza;
    bool hay = false;

    cout << "\n=== OPERATIVOS DE ";
    mostrarNombreFaccion(faccionId);
    cout << " ===" << endl;

    while (actual != NULL)
    {
        if (actual->datos.faccionId == faccionId)
        {
            cout << "[" << actual->datos.id << "] " << actual->datos.nombre;
            cout << " | " << actual->datos.arquetipo;
            cout << " | " << actual->datos.puntos << " pts";
            if (actual->datos.esLider)
                cout << " [LIDER]";
            cout << endl;
            hay = true;
        }
        actual = actual->siguiente;
    }

    if (!hay)
    {
        cout << "No hay operativos para esta faccion." << endl;
    }
}

void buscarOperativosConArmaRango(ListaOperativos *lista)
{
    NodoOperativo *actual = lista->cabeza;
    bool hay = false;

    cout << "\n=== OPERATIVOS CON ARMA DE RANGO ===" << endl;

    while (actual != NULL)
    {
        bool tieneArmaRango = false;
        for (int i = 0; i < actual->datos.cantidadArmas; i++)
        {
            if (actual->datos.armas[i].tipo == ARMA_RANGO)
            {
                tieneArmaRango = true;
                break;
            }
        }

        if (tieneArmaRango)
        {
            cout << "[" << actual->datos.id << "] " << actual->datos.nombre;
            cout << " | ";
            mostrarNombreFaccion(actual->datos.faccionId);
            cout << endl;

            for (int i = 0; i < actual->datos.cantidadArmas; i++)
            {
                if (actual->datos.armas[i].tipo == ARMA_RANGO)
                {
                    cout << "    - " << actual->datos.armas[i].nombre;
                    cout << " (" << actual->datos.armas[i].alcance << "\")";
                    cout << " A" << actual->datos.armas[i].ataque;
                    cout << " " << actual->datos.armas[i].impacto << "+";
                    cout << " D" << actual->datos.armas[i].dano;
                    cout << "/" << actual->datos.armas[i].danoCritico;
                    cout << endl;
                }
            }
            hay = true;
        }
        actual = actual->siguiente;
    }

    if (!hay)
    {
        cout << "No se encontraron operativos con arma de rango." << endl;
    }
}

void menuBusquedaAvanzada(ListaOperativos *lista)
{
    int opcion;
    do
    {
        cout << "\n=== BUSQUEDA AVANZADA ===" << endl;
        cout << "1. Buscar por nombre" << endl;
        cout << "2. Buscar por faccion" << endl;
        cout << "3. Buscar operativos con arma de rango" << endl;
        cout << "0. Volver" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
        {
            char busqueda[TAM_NOMBRE];
            cout << "Ingrese nombre a buscar: ";
            cin.ignore();
            cin.getline(busqueda, TAM_NOMBRE);
            buscarOperativosPorNombre(lista, busqueda);
            break;
        }
        case 2:
        {
            cout << "Seleccione faccion:" << endl;
            cout << "1. Angeles de la Muerte" << endl;
            cout << "2. Kommandos" << endl;
            cout << "3. Mandrakes" << endl;
            cout << "4. Tropa Danzante Vacia" << endl;
            cout << "Opcion: ";
            int faccion;
            cin >> faccion;
            if (faccion >= 1 && faccion <= 4)
            {
                buscarOperativosPorFaccion(lista, faccion - 1);
            }
            else
            {
                cout << "Opcion invalida." << endl;
            }
            break;
        }
        case 3:
            buscarOperativosConArmaRango(lista);
            break;
        case 0:
            break;
        default:
            cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);
}

// ============================================
// ORDENAMIENTO ALFABETICO
// Insercion ordenada sobre lista enlazada
// ============================================

int compararNombres(const char *nombre1, const char *nombre2)
{
    return strcmp(nombre1, nombre2);
}

void agregarOperativoOrdenado(ListaOperativos *lista, DatosOperativo datos)
{
    NodoOperativo *nuevo = crearNodoOperativo(datos);
    if (nuevo == NULL)
        return;

    // Caso 1: lista vacia o nuevo va al principio
    if (lista->cabeza == NULL || compararNombres(nuevo->datos.nombre, lista->cabeza->datos.nombre) < 0)
    {
        nuevo->siguiente = lista->cabeza;
        lista->cabeza = nuevo;
        if (lista->cola == NULL)
        {
            lista->cola = nuevo;
        }
        lista->cantidad++;
        lista->siguienteId++;
        return;
    }

    // Caso 2: buscar posicion de insercion
    NodoOperativo *actual = lista->cabeza;
    while (actual->siguiente != NULL && compararNombres(actual->siguiente->datos.nombre, nuevo->datos.nombre) < 0)
    {
        actual = actual->siguiente;
    }

    // Insertar despues de 'actual'
    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    if (nuevo->siguiente == NULL)
    {
        lista->cola = nuevo;
    }
    lista->cantidad++;
    lista->siguienteId++;
}

void ordenarListaOperativos(ListaOperativos *lista)
{
    if (lista->cabeza == NULL || lista->cabeza->siguiente == NULL)
    {
        return; // 0 o 1 elemento, ya esta ordenada
    }

    // Algoritmo: insertion sort para lista enlazada
    NodoOperativo *ordenada = NULL;
    NodoOperativo *actual = lista->cabeza;

    while (actual != NULL)
    {
        NodoOperativo *siguiente = actual->siguiente;

        // Insertar 'actual' en la lista ordenada
        if (ordenada == NULL || compararNombres(actual->datos.nombre, ordenada->datos.nombre) < 0)
        {
            actual->siguiente = ordenada;
            ordenada = actual;
        }
        else
        {
            NodoOperativo *temp = ordenada;
            while (temp->siguiente != NULL && compararNombres(temp->siguiente->datos.nombre, actual->datos.nombre) < 0)
            {
                temp = temp->siguiente;
            }
            actual->siguiente = temp->siguiente;
            temp->siguiente = actual;
        }

        actual = siguiente;
    }

    lista->cabeza = ordenada;

    // Actualizar cola
    NodoOperativo *temp = lista->cabeza;
    while (temp->siguiente != NULL)
    {
        temp = temp->siguiente;
    }
    lista->cola = temp;
}

// ============================================
// VALIDACIONES ROBUSTAS
// ============================================

void limpiarBuffer()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

bool esNumeroValido(const char *cadena)
{
    if (cadena == NULL || strlen(cadena) == 0)
        return false;
    for (size_t i = 0; i < strlen(cadena); i++)
    {
        if (!isdigit(cadena[i]) && !(i == 0 && cadena[i] == '-'))
        {
            return false;
        }
    }
    return true;
}

int leerEntero(const char *mensaje)
{
    char entrada[50];
    int valor;

    while (true)
    {
        cout << mensaje;
        cin.getline(entrada, 50);

        if (!esNumeroValido(entrada))
        {
            cout << "Error: Debe ingresar un numero valido." << endl;
            continue;
        }

        valor = atoi(entrada);
        return valor;
    }
}

int leerEnteroEnRango(const char *mensaje, int minimo, int maximo)
{
    int valor;
    while (true)
    {
        valor = leerEntero(mensaje);
        if (valor >= minimo && valor <= maximo)
        {
            return valor;
        }
        cout << "Error: El valor debe estar entre " << minimo << " y " << maximo << "." << endl;
    }
}

void leerCadenaSegura(const char *mensaje, char *destino, int tamanoMaximo)
{
    cout << mensaje;
    cin.getline(destino, tamanoMaximo);

    // Si se excedio el buffer, limpiar resto
    if (cin.fail())
    {
        limpiarBuffer();
        destino[tamanoMaximo - 1] = '\0';
        cout << "Advertencia: El texto fue truncado a " << (tamanoMaximo - 1) << " caracteres." << endl;
    }
}

bool confirmarAccion(const char *mensaje)
{
    char respuesta[10];
    cout << mensaje << " (s/n): ";
    cin.getline(respuesta, 10);
    return (respuesta[0] == 's' || respuesta[0] == 'S');
}

bool validarIdOperativoExiste(ListaOperativos *lista, int id)
{
    NodoOperativo *op = buscarOperativoPorId(lista, id);
    if (op == NULL)
    {
        cout << "Error: No existe un operativo con ese ID." << endl;
        return false;
    }
    return true;
}

bool validarIdEquipamientoExiste(ListaEquipamiento *lista, int id)
{
    NodoEquipamiento *eq = buscarEquipamientoPorId(lista, id);
    if (eq == NULL)
    {
        cout << "Error: No existe un equipamiento con ese ID." << endl;
        return false;
    }
    return true;
}

// ============================================
// CAPTURA DE OPERATIVOS POR TECLADO (R1)
// ============================================

void capturarArmasOperativo(DatosOperativo *op)
{
    cout << "\n--- Captura de Armas ---" << endl;
    cout << "Cantidad de armas (0-" << MAX_ARMAS << "): ";
    int cantidad = leerEnteroEnRango("", 0, MAX_ARMAS);

    for (int i = 0; i < cantidad; i++)
    {
        cout << "\nArma " << (i + 1) << ":" << endl;
        char nombreArma[TAM_NOMBRE];
        leerCadenaSegura("Nombre: ", nombreArma, TAM_NOMBRE);

        cout << "Tipo: [0] Rango | [1] Melee" << endl;
        int tipo = leerEnteroEnRango("Opcion: ", 0, 1);

        int ataque = leerEntero("Ataque: ");
        int impacto = leerEntero("Impacto (ej: 3 para 3+): ");
        int dano = leerEntero("Dano: ");
        int danoCritico = leerEntero("Dano Critico: ");
        int alcance = (tipo == 0) ? leerEntero("Alcance (pulgadas): ") : 0;
        int amenaza = leerEntero("Amenaza: ");

        agregarArmaAOperativo(op, nombreArma, (TipoArma)tipo, ataque, impacto, dano, danoCritico, alcance, amenaza);
    }
}

void capturarOperativoPorTeclado(ListaOperativos *lista)
{
    DatosOperativo op;
    memset(&op, 0, sizeof(DatosOperativo));

    mostrarTitulo("CAPTURA DE NUEVA UNIDAD");

    op.id = lista->siguienteId;
    op.cantidadArmas = 0;
    op.cantidadKeywords = 0;
    op.cantidadHabilidades = 0;
    op.idEquipamiento = -1;

    char nombre[TAM_NOMBRE];
    leerCadenaSegura("Nombre del operativo: ", nombre, TAM_NOMBRE);
    strncpy(op.nombre, nombre, TAM_NOMBRE - 1);
    op.nombre[TAM_NOMBRE - 1] = '\0';

    cout << "\nSeleccione faccion:" << endl;
    cout << "[0] Angeles de la Muerte" << endl;
    cout << "[1] Kommandos" << endl;
    cout << "[2] Mandrakes" << endl;
    cout << "[3] Tropa Danzante Vacia" << endl;
    op.faccionId = leerEnteroEnRango("Opcion: ", 0, 3);

    switch (op.faccionId)
    {
    case ID_FAC_ANGELES:
        strncpy(op.faccion, "Angeles de la Muerte", TAM_FACCION - 1);
        break;
    case ID_FAC_KOMMANDOS:
        strncpy(op.faccion, "Kommandos", TAM_FACCION - 1);
        break;
    case ID_FAC_MANDRAKES:
        strncpy(op.faccion, "Mandrakes", TAM_FACCION - 1);
        break;
    case ID_FAC_TROUPE:
        strncpy(op.faccion, "Tropa Danzante Vacia", TAM_FACCION - 1);
        break;
    }
    op.faccion[TAM_FACCION - 1] = '\0';

    char arquetipo[TAM_ARQUETIPO];
    leerCadenaSegura("Arquetipo (Lider, Linea, Especialista, etc.): ", arquetipo, TAM_ARQUETIPO);
    strncpy(op.arquetipo, arquetipo, TAM_ARQUETIPO - 1);
    op.arquetipo[TAM_ARQUETIPO - 1] = '\0';

    cout << "\nEs lider? (s/n): ";
    char lider[10];
    cin.getline(lider, 10);
    op.esLider = (lider[0] == 's' || lider[0] == 'S') ? 1 : 0;

    op.movimiento = leerEntero("Movimiento: ");
    op.apl = leerEntero("APL: ");
    op.salvacion = leerEntero("Salvacion: ");
    op.heridas = leerEntero("Heridas: ");
    op.defensa = leerEntero("Defensa: ");
    op.puntos = leerEntero("Puntos: ");

    capturarArmasOperativo(&op);

    mostrarResumenRoster(NULL, NULL, NULL); // Solo para mostrar titulo
    cout << "Nombre: " << op.nombre << endl;
    cout << "Faccion: " << op.faccion << endl;
    cout << "Arquetipo: " << op.arquetipo << endl;
    cout << "Stats: M" << op.movimiento << " APL" << op.apl << " SV" << op.salvacion
         << " W" << op.heridas << " DF" << op.defensa << " pts" << op.puntos << endl;
    cout << "Armas: " << op.cantidadArmas << endl;

    if (confirmarAccion("Confirmar alta"))
    {
        agregarOperativoOrdenado(lista, op);
        guardarOperativos(lista);
        mostrarMensajeExito("Operativo capturado y guardado.");
    }
    else
    {
        mostrarMensajeAdvertencia("Captura cancelada.");
    }
}

// ============================================
// EDICION Y ELIMINACION DE UNIDADES (R12)
// ============================================

void mostrarOperativosParaEdicion(ListaOperativos *lista)
{
    cout << "\n=== OPERATIVOS DEL CATALOGO ===" << endl;
    NodoOperativo *actual = lista->cabeza;
    while (actual != NULL)
    {
        cout << "[" << actual->datos.id << "] " << actual->datos.nombre;
        cout << " | " << actual->datos.arquetipo;
        cout << " | " << actual->datos.puntos << " pts";
        if (actual->datos.esLider)
            cout << " [LIDER]";
        cout << endl;
        actual = actual->siguiente;
    }
}

bool editarOperativoEnArchivoPorId(int id, DatosOperativo nuevosDatos)
{
    FILE *archivo = fopen(ARCHIVO_UNIDADES, "r+b");
    if (archivo == NULL)
    {
        return false;
    }

    DatosOperativo temp;
    bool encontrado = false;
    while (fread(&temp, sizeof(DatosOperativo), 1, archivo) == 1)
    {
        if (temp.id == id)
        {
            fseek(archivo, -(long)sizeof(DatosOperativo), SEEK_CUR);
            fwrite(&nuevosDatos, sizeof(DatosOperativo), 1, archivo);
            encontrado = true;
            break;
        }
    }

    fclose(archivo);
    return encontrado;
}

void menuEditarOperativo(ListaOperativos *lista)
{
    mostrarTitulo("EDITAR UNIDAD");
    mostrarOperativosParaEdicion(lista);

    int id = leerEntero("Ingrese ID de la unidad a editar (0=cancelar): ");
    if (id == 0)
        return;

    NodoOperativo *nodo = buscarOperativoPorId(lista, id);
    if (nodo == NULL)
    {
        mostrarMensajeError("ID no encontrado.");
        return;
    }

    DatosOperativo op = nodo->datos;
    bool modificado = false;

    cout << "\nDatos actuales:" << endl;
    cout << "Nombre: " << op.nombre << endl;
    cout << "Faccion: " << op.faccion << endl;
    cout << "Arquetipo: " << op.arquetipo << endl;
    cout << "Puntos: " << op.puntos << endl;

    if (confirmarAccion("Cambiar nombre"))
    {
        leerCadenaSegura("Nuevo nombre: ", op.nombre, TAM_NOMBRE);
        modificado = true;
    }

    if (confirmarAccion("Cambiar arquetipo"))
    {
        leerCadenaSegura("Nuevo arquetipo: ", op.arquetipo, TAM_ARQUETIPO);
        modificado = true;
    }

    if (confirmarAccion("Cambiar puntos"))
    {
        op.puntos = leerEntero("Nuevos puntos: ");
        modificado = true;
    }

    if (confirmarAccion("Cambiar stats"))
    {
        op.movimiento = leerEntero("Movimiento: ");
        op.apl = leerEntero("APL: ");
        op.salvacion = leerEntero("Salvacion: ");
        op.heridas = leerEntero("Heridas: ");
        op.defensa = leerEntero("Defensa: ");
        modificado = true;
    }

    if (modificado)
    {
        nodo->datos = op;

        // Guardar en archivo binario directamente (EXTRA)
        if (editarOperativoEnArchivoPorId(id, op))
        {
            mostrarMensajeExito("Unidad editada y guardada en archivo.");
        }
        else
        {
            // Fallback: reescribir todo
            guardarOperativos(lista);
            mostrarMensajeExito("Unidad editada (archivo reescrito).");
        }
    }
    else
    {
        mostrarMensajeAdvertencia("Sin cambios.");
    }
}

void menuEliminarOperativo(ListaOperativos *lista)
{
    mostrarTitulo("ELIMINAR UNIDAD");
    mostrarOperativosParaEdicion(lista);

    int id = leerEntero("Ingrese ID de la unidad a eliminar (0=cancelar): ");
    if (id == 0)
        return;

    NodoOperativo *nodo = buscarOperativoPorId(lista, id);
    if (nodo == NULL)
    {
        mostrarMensajeError("ID no encontrado.");
        return;
    }

    cout << "Unidad: " << nodo->datos.nombre << " (" << nodo->datos.arquetipo << ")" << endl;
    if (!confirmarAccion("Confirmar eliminacion"))
    {
        mostrarMensajeAdvertencia("Eliminacion cancelada.");
        return;
    }

    // Eliminar de la lista enlazada
    if (lista->cabeza == nodo)
    {
        lista->cabeza = nodo->siguiente;
        if (lista->cola == nodo)
            lista->cola = NULL;
    }
    else
    {
        NodoOperativo *actual = lista->cabeza;
        while (actual != NULL && actual->siguiente != nodo)
        {
            actual = actual->siguiente;
        }
        if (actual != NULL)
        {
            actual->siguiente = nodo->siguiente;
            if (lista->cola == nodo)
                lista->cola = actual;
        }
    }
    free(nodo);
    lista->cantidad--;

    // Reconstruir archivo
    if (eliminarOperativoDeArchivo(id))
    {
        mostrarMensajeExito("Unidad eliminada.");
    }
    else
    {
        guardarOperativos(lista);
        mostrarMensajeExito("Unidad eliminada (archivo reconstruido).");
    }
}

// ============================================
// EXPORTAR A .TXT (R9)
// ============================================

void exportarRosterATexto(ListaOperativos *catalogo, ListaEquipamiento *equipos, ListaRosters *lista, int idRoster)
{
    NodoRoster *nodo = buscarRosterPorId(lista, idRoster);
    if (nodo == NULL)
    {
        mostrarMensajeError("Lista no encontrada.");
        return;
    }

    char nombreArchivo[100];
    sprintf(nombreArchivo, "roster_%d_%s.txt", idRoster, nodo->datos.nombre);
    // Limpiar nombre de archivo
    for (int i = 0; nombreArchivo[i]; i++)
    {
        if (nombreArchivo[i] == ' ' || nombreArchivo[i] == '/')
            nombreArchivo[i] = '_';
    }

    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        mostrarMensajeError("No se pudo crear el archivo.");
        return;
    }

    char fechaFormateada[30];
    formatearFecha(nodo->datos.fechaCreacion, fechaFormateada, 30);

    fprintf(archivo, "===================================\n");
    fprintf(archivo, "  KILL TEAM - LISTA DE EJERCITO\n");
    fprintf(archivo, "===================================\n");
    fprintf(archivo, "Nombre: %s\n", nodo->datos.nombre);

    fprintf(archivo, "Faccion: ");
    switch (nodo->datos.faccionId)
    {
    case ID_FAC_ANGELES:
        fprintf(archivo, "Angeles de la Muerte");
        break;
    case ID_FAC_KOMMANDOS:
        fprintf(archivo, "Kommandos");
        break;
    case ID_FAC_MANDRAKES:
        fprintf(archivo, "Mandrakes");
        break;
    case ID_FAC_TROUPE:
        fprintf(archivo, "Tropa Danzante Vacia");
        break;
    }
    fprintf(archivo, "\n");

    fprintf(archivo, "Fecha: %s\n", fechaFormateada);
    fprintf(archivo, "Operativos: %d/%d\n", nodo->datos.cantidadOperativos, obtenerComposicionFaccion(nodo->datos.faccionId));
    fprintf(archivo, "Puntos totales: %d\n", calcularPuntosRoster(catalogo, &(nodo->datos)));
    fprintf(archivo, "-----------------------------------\n");
    fprintf(archivo, "OPERATIVOS:\n");
    fprintf(archivo, "-----------------------------------\n");

    int totalEquipamiento = 0;
    for (int i = 0; i < nodo->datos.cantidadOperativos; i++)
    {
        NodoOperativo *op = buscarOperativoPorId(catalogo, nodo->datos.idsOperativos[i]);
        if (op != NULL)
        {
            fprintf(archivo, "[%d] %s", i + 1, op->datos.nombre);
            if (op->datos.esLider)
                fprintf(archivo, " [LIDER]");
            fprintf(archivo, " - %d pts\n", op->datos.puntos);

            if (nodo->datos.idsEquipamientoPorOperativo[i] != -1)
            {
                NodoEquipamiento *eq = buscarEquipamientoPorId(equipos, nodo->datos.idsEquipamientoPorOperativo[i]);
                if (eq != NULL)
                {
                    fprintf(archivo, "    Equipamiento: %s", eq->datos.nombre);
                    if (eq->datos.faccionId == -1)
                        fprintf(archivo, " [Universal]");
                    else
                        fprintf(archivo, " [Faccion]");
                    fprintf(archivo, "\n");
                    totalEquipamiento++;
                }
            }

            for (int j = 0; j < op->datos.cantidadArmas; j++)
            {
                fprintf(archivo, "    Arma: %s", op->datos.armas[j].nombre);
                if (op->datos.armas[j].tipo == ARMA_RANGO)
                    fprintf(archivo, " (Rango %d\")", op->datos.armas[j].alcance);
                else
                    fprintf(archivo, " (Melee)");
                fprintf(archivo, "\n");
            }
            fprintf(archivo, "\n");
        }
    }

    fprintf(archivo, "-----------------------------------\n");
    fprintf(archivo, "Equipamiento total: %d/%d\n", totalEquipamiento, MAX_EQUIPAMIENTO_POR_LISTA);
    fprintf(archivo, "Puntos totales: %d\n", calcularPuntosRoster(catalogo, &(nodo->datos)));
    fprintf(archivo, "===================================\n");

    fclose(archivo);
    mostrarMensajeExito("Lista exportada a .txt");
    cout << "Archivo: " << nombreArchivo << endl;
}

// ============================================
// EDITAR ROSTER (INTERCAMBIAR UNIDADES) (R10)
// ============================================

void menuEditarRoster(ListaOperativos *catalogo, ListaEquipamiento *equipos, ListaRosters *lista)
{
    mostrarTitulo("EDITAR LISTA");
    mostrarRostersGuardados(catalogo, lista);

    if (lista->cantidad == 0)
        return;

    int idRoster = leerEntero("Ingrese ID de la lista a editar (0=cancelar): ");
    if (idRoster == 0)
        return;

    NodoRoster *nodo = buscarRosterPorId(lista, idRoster);
    if (nodo == NULL)
    {
        mostrarMensajeError("Lista no encontrada.");
        return;
    }

    DatosRoster *roster = &(nodo->datos);
    int faccionId = roster->faccionId;
    int maxOps = obtenerComposicionFaccion(faccionId);

    cout << "\nOperativos actuales:" << endl;
    for (int i = 0; i < roster->cantidadOperativos; i++)
    {
        NodoOperativo *op = buscarOperativoPorId(catalogo, roster->idsOperativos[i]);
        if (op != NULL)
        {
            cout << "[" << i << "] " << op->datos.nombre;
            if (op->datos.esLider)
                cout << " [LIDER]";
            cout << endl;
        }
    }

    int idxQuitar = leerEnteroEnRango("Indice del operativo a quitar: ", 0, roster->cantidadOperativos - 1);
    NodoOperativo *opQuitar = buscarOperativoPorId(catalogo, roster->idsOperativos[idxQuitar]);

    if (opQuitar->datos.esLider)
    {
        // Verificar que haya otro lider
        int otrosLideres = 0;
        for (int i = 0; i < roster->cantidadOperativos; i++)
        {
            if (i == idxQuitar)
                continue;
            NodoOperativo *op = buscarOperativoPorId(catalogo, roster->idsOperativos[i]);
            if (op != NULL && op->datos.esLider)
                otrosLideres++;
        }
        if (otrosLideres == 0)
        {
            mostrarMensajeError("No puede quitar el unico LIDER. Agregue otro primero.");
            return;
        }
    }

    cout << "\nOperativos disponibles de la faccion:" << endl;
    mostrarOperativosPorFaccion(catalogo, faccionId);

    int idNuevo = leerEntero("ID del nuevo operativo (0=cancelar): ");
    if (idNuevo == 0)
        return;

    NodoOperativo *opNuevo = buscarOperativoPorId(catalogo, idNuevo);
    if (opNuevo == NULL)
    {
        mostrarMensajeError("ID invalido.");
        return;
    }

    if (opNuevo->datos.faccionId != faccionId)
    {
        mostrarMensajeError("Ese operativo no pertenece a la faccion de la lista.");
        return;
    }

    // Verificar que no este repetido (excepto el que vamos a quitar)
    for (int i = 0; i < roster->cantidadOperativos; i++)
    {
        if (i == idxQuitar)
            continue;
        if (roster->idsOperativos[i] == idNuevo)
        {
            mostrarMensajeError("Operativo ya en la lista.");
            return;
        }
    }

    // Validar lider
    if (opNuevo->datos.esLider && opQuitar->datos.esLider == 0)
    {
        // Ya hay un lider, no puede haber dos
        int yaTieneLider = 0;
        for (int i = 0; i < roster->cantidadOperativos; i++)
        {
            if (i == idxQuitar)
                continue;
            NodoOperativo *op = buscarOperativoPorId(catalogo, roster->idsOperativos[i]);
            if (op != NULL && op->datos.esLider)
            {
                yaTieneLider = 1;
                break;
            }
        }
        if (yaTieneLider)
        {
            mostrarMensajeError("La lista ya tiene un LIDER.");
            return;
        }
    }

    // Hacer el cambio
    roster->idsOperativos[idxQuitar] = idNuevo;
    roster->idsEquipamientoPorOperativo[idxQuitar] = -1; // Resetear equipamiento

    if (guardarRosters(lista))
    {
        mostrarMensajeExito("Lista actualizada.");
    }
    else
    {
        mostrarMensajeError("Error al guardar cambios.");
    }
}
