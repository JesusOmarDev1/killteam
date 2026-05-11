#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <string>
#include <cstring>

// ===========================
// LIMITES DEL SISTEMA
// ===========================
#define MAX_WEAPONS 6      // Maximo de armas que un operativo puede equipar
#define MAX_KEYWORDS 5     // Maximo de palabras clave que un operativo puede tener
#define MAX_WEAPON_RULES 3 // Maximo de reglas especiales que un arma puede tener
#define MAX_ABILITIES 5    // Maximo de habilidades especiales que un operativo puede tener
#define MAX_OPS 20         // Maximo de operativos en memoria (nodos en lista)

// ===========================
// TIPOS DE ARMA
// ===========================
typedef enum
{
    RANGE, // 0 - Arma de rango
    MELEE  // 1 - Arma cuerpo a cuerpo
} WeaponType;

// ===========================
// ESTRUCTURA ARMA
// ===========================
typedef struct
{
    std::string name;
    WeaponType type;                     // Rango o cuerpo a cuerpo
    int attacks;                         // Numero de ataques (A)
    int hit;                             // Valor para impactar (3 = 3+)
    int damage;                          // Danio normal (D)
    int criticalDamage;                  // Danio critico (Dc), 0 si no tiene
    std::string rules[MAX_WEAPON_RULES]; // Reglas especiales
    int ruleCount;                       // Cantidad de reglas especiales
    int threat;                          // Amenaza calculada
    int range;                           // Solo para armas de rango (0 para melee)
} Weapon;

// ===========================
// NODO DE LISTA ENLAZADA
// ===========================
typedef struct OperativeNode
{
    int id;                               // ID unico del operativo
    std::string name;                     // Nombre del operativo
    std::string faction;                  // Faccion del operativo
    std::string keywords[MAX_KEYWORDS];   // Palabras clave
    int keywordCount;                     // Cantidad de palabras clave
    std::string archetype;                // Arquetipo
    int movement;                         // Movimiento (M)
    int apl;                              // APL
    int save;                             // Salvacion (S)
    int wounds;                           // Heridas (W)
    Weapon weapons[MAX_WEAPONS];          // Armas del operativo
    int weaponCount;                      // Cantidad de armas
    std::string abilities[MAX_ABILITIES]; // Habilidades especiales
    int abilityCount;                     // Cantidad de habilidades
    struct OperativeNode *next;           // Puntero al siguiente nodo
} OperativeNode;

// ===========================
// LISTA SIMPLEMENTE ENLAZADA
// ===========================
typedef struct
{
    OperativeNode *head; // Primer nodo
    OperativeNode *tail; // Ultimo nodo
    int size;            // Cantidad de nodos
    int nextId;          // Siguiente ID a asignar
} OperativeList;

// ===========================
// DECLARACION DE FUNCIONES
// ===========================

// Listas Enlazadas
void initializeList(OperativeList *list);
bool isEmpty(OperativeList *list);
int addOperative(OperativeList *list, int position);
void displayAllOperatives(OperativeList *list);
void searchOperative(OperativeList *list, std::string name);
void displayByFaction(OperativeList *list, std::string faction);
void modifyOperative(OperativeList *list, std::string name);
void deleteOperative(OperativeList *list, std::string name);
OperativeNode *getNodeAt(OperativeList *list, int position);
int countMatches(OperativeList *list, std::string name);
void displayOperativeCompact(OperativeNode *op);

// Captura de operativo
void capture_operative_data(OperativeNode *op);

// Visualizacion de salida
void display_operative(OperativeNode *op);
void display_weapons(OperativeNode *op);

// Calculos
int calculate_threat(int attacks, int damage, int criticalDamage);

// Validaciones
bool validate_total_weapons(OperativeNode *op);
bool validate_keyword_count(int count);

#endif