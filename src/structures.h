#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <ctime>

// ============================================
// CONSTANTES DEL SISTEMA
// ============================================

// Tamanios de strings
#define TAM_NOMBRE 50
#define TAM_FACCION 30
#define TAM_ARQUETIPO 30
#define TAM_DESCRIPCION 200
#define TAM_TIPO_ARMA 20
#define TAM_KEYWORD 20
#define TAM_HABILIDAD 100
#define TAM_REGLA_ARMA 50

// Limites de colecciones
#define MAX_ARMAS 6
#define MAX_KEYWORDS 5
#define MAX_REGLAS_ARMA 3
#define MAX_HABILIDADES 5
#define MAX_OPS 20
#define MAX_EQUIPAMIENTO_POR_LISTA 4
#define MAX_EQUIPAMIENTO_POR_OPERATIVO 1
#define MAX_OPERATIVOS_POR_ROSTER 10

// IDs de facciones
#define ID_FAC_ANGELES 0
#define ID_FAC_KOMMANDOS 1
#define ID_FAC_MANDRAKES 2
#define ID_FAC_TROUPE 3

// Composiciones de kill team por faccion
#define COMP_ANGELES 6
#define COMP_KOMMANDOS 10
#define COMP_MANDRAKES 9
#define COMP_TROUPE 8

// ============================================
// ENUMERACIONES
// ============================================

typedef enum
{
    ARMA_RANGO,
    ARMA_MELEE
} TipoArma;

// ============================================
// ESTRUCTURA ARMA
// ============================================
typedef struct
{
    int ataque;
    int impacto;
    int dano;
    int danoCritico;
    int alcance;
    int cantidadReglas;
    int amenaza;
    TipoArma tipo;
    char nombre[TAM_NOMBRE];
    char reglas[MAX_REGLAS_ARMA][TAM_REGLA_ARMA];
} Arma;

// ============================================
// DATOS DEL OPERATIVO
// ============================================
typedef struct
{
    int id;
    int movimiento;
    int apl;
    int salvacion;
    int heridas;
    int defensa;
    int cantidadArmas;
    int cantidadKeywords;
    int cantidadHabilidades;
    int esLider;
    int faccionId;
    int puntos;
    int idEquipamiento; // -1 = sin equipamiento, >0 = ID del equipamiento
    char nombre[TAM_NOMBRE];
    char faccion[TAM_FACCION];
    char arquetipo[TAM_ARQUETIPO];
    char keywords[MAX_KEYWORDS][TAM_KEYWORD];
    char habilidades[MAX_HABILIDADES][TAM_HABILIDAD];
    Arma armas[MAX_ARMAS];
} DatosOperativo;

// ============================================
// NODO OPERATIVO
// Para lista enlazada en memoria (con puntero)
// ============================================
typedef struct NodoOperativo
{
    struct NodoOperativo *siguiente;
    DatosOperativo datos;
} NodoOperativo;

// ============================================
// LISTA DE OPERATIVOS
// ============================================
typedef struct
{
    NodoOperativo *cabeza;
    NodoOperativo *cola;
    int cantidad;
    int siguienteId;
} ListaOperativos;

// ============================================
// EQUIPAMIENTO
// ============================================
typedef struct
{
    int id;
    int faccionId; // -1 = universal, 0-3 = faccion
    char nombre[TAM_NOMBRE];
    char descripcion[TAM_DESCRIPCION];
} DatosEquipamiento;

typedef struct NodoEquipamiento
{
    struct NodoEquipamiento *siguiente;
    DatosEquipamiento datos;
} NodoEquipamiento;

typedef struct
{
    NodoEquipamiento *cabeza;
    int cantidad;
} ListaEquipamiento;

// ============================================
// ROSTER / LISTA DE EQUIPO ARMADA
// Guarda solo IDs de operativos y equipamiento
// ============================================
typedef struct
{
    int id;
    int faccionId;
    int cantidadOperativos;
    int cantidadEquipamiento;
    time_t fechaCreacion;
    char nombre[TAM_NOMBRE];
    int idsOperativos[MAX_OPERATIVOS_POR_ROSTER];
    int idsEquipamientoPorOperativo[MAX_OPERATIVOS_POR_ROSTER]; // -1 = sin equipamiento
} DatosRoster;

typedef struct NodoRoster
{
    struct NodoRoster *siguiente;
    DatosRoster datos;
} NodoRoster;

typedef struct
{
    NodoRoster *cabeza;
    int cantidad;
} ListaRosters;

// ============================================
// PROTOTIPOS
// ============================================
void agregarOperativoOrdenado(ListaOperativos *lista, DatosOperativo datos);

#endif
