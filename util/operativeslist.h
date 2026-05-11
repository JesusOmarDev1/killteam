#ifndef OPERATIVESLIST_H
#define OPERATIVESLIST_H

// ===========================
// FUNCIONES DE LISTA ENLAZADA
// ===========================

// Inicializar la lista
void initializeList(OperativeList *list);

// Verificar si la lista esta vacia
bool isEmpty(OperativeList *list);

// Agregar operativo (position: 0=inicio, 1=fin, 2=centro)
int addOperative(OperativeList *list, int position);

// Mostrar todos los operativos
void displayAllOperatives(OperativeList *list);

// Buscar operativo por nombre
void searchOperative(OperativeList *list, std::string name);

// Mostrar operativos por faccion
void displayByFaction(OperativeList *list, std::string faction);

// Modificar operativo por nombre
void modifyOperative(OperativeList *list, std::string name);

// Eliminar operativo por nombre
void deleteOperative(OperativeList *list, std::string name);

// ===========================
// FUNCIONES AUXILIARES
// ===========================

// Obtener nodo en una posicion
OperativeNode* getNodeAt(OperativeList *list, int position);

// Contar cuantas veces aparece un nombre
int countMatches(OperativeList *list, std::string name);

// Mostrar un operativo de forma compacta (para选择了iones)
void displayOperativeCompact(OperativeNode *op);

#endif