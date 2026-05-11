#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_WEAPONS 6      // Número máximo de armas que un operativo puede equipar
#define MAX_KEYWORDS 5     // Número máximo de palabras clave que un operativo puede tener
#define MAX_WEAPON_RULES 3 // Número máximo de reglas especiales que un arma puede tener
#define MAX_ABILITIES 5    // Número máximo de habilidades especiales que un operativo puede tener
#define MAX_OPS_FIRETEAM 6 // Número máximo de operativos en un fireteam

// Estructuras para representar operativos y armas en el juego
typedef enum
{
    RANGE,
    MELEE
} WeaponType;

typedef struct
{
    char name[50];
    WeaponType type;                  // Rango o cuerpo a cuerpo
    int attacks;                      // Número de ataques (A)
    int hit;                          // Valor necesario para impactar (3 = 3+)
    int damage;                       // Daño normal (D)
    int criticalDamage;               // Daño crítico (Dc), 0 si no tiene
    char rules[MAX_WEAPON_RULES][30]; // Reglas especiales
    int ruleCount;                    // Número de reglas especiales
} Weapon;

typedef struct
{
    char name[50];                      // Nombre del operativo
    char keywords[MAX_KEYWORDS][30];    // Palabras clave (ej. "Infiltration", "Recon", etc.)
    int keywordCount;                   // Número de palabras clave
    char archetype[20];                 // Arquetipo (ej. "Assault", "Support", etc.)
    int movement;                       // Movimiento (M)
    int apl;                            // Limitación de puntos de acción (APL)
    int save;                           // Salvación (S) (3 = 3+)
    int wounds;                         // Heridas (W)
    Weapon weapons[MAX_WEAPONS];        // Armas del operativo
    int weaponCount;                    // Número de armas equipadas
    char abilities[MAX_ABILITIES][100]; // Habilidades especiales del operativo (ej. "Stealth", "Overwatch", etc.)
    int abilityCount;                   // Número de habilidades especiales
} Operative;

typedef struct
{
    char teamName[50];                      // Nombre del fireteam
    char faction[30];                       // Facción a la que pertenece el fireteam (ej. "PanOceania", "Yu Jing", etc.)
    Operative operatives[MAX_OPS_FIRETEAM]; // Operativos en el fireteam
    int operativeCount;                     // Número de operativos en el fireteam
} FireTeam;

// Estructura para representar un nodo en una lista enlazada
typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

#endif