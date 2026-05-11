#include <iostream>
#include <iomanip>
#include <string>
#include "../util/structures.h"
#include "../util/operativeslist.h"

using namespace std;

// ===========================
// FUNCIONES AUXILIARES
// ===========================

void initializeList(OperativeList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->nextId = 1;
}

bool isEmpty(OperativeList *list)
{
    return (list->head == NULL);
}

OperativeNode* getNodeAt(OperativeList *list, int position)
{
    if (isEmpty(list))
        return NULL;

    if (position < 0 || position >= list->size)
        return NULL;

    OperativeNode *current = list->head;
    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }
    return current;
}

int countMatches(OperativeList *list, string name)
{
    int count = 0;
    OperativeNode *current = list->head;

    while (current != NULL)
    {
        if (current->name == name)
            count++;
        current = current->next;
    }
    return count;
}

// ===========================
// AGREGAR OPERATIVO
// ===========================
int addOperative(OperativeList *list, int position)
{
    if (list->size >= MAX_OPS)
    {
        cout << "\n  ERROR: Limite de operativos alcanzado (" << MAX_OPS << ").\n";
        return 0;
    }

    // Crear nuevo nodo
    OperativeNode *newNode = new OperativeNode;
    newNode->id = list->nextId++;

    // Capturar datos del operativo
    cout << "\n=== CAPTURAR OPERATIVO (ID: " << newNode->id << ") ===\n";
    capture_operative_data(newNode);
    newNode->next = NULL;

    // Lista vacia
    if (isEmpty(list))
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        // Agregar al INICIO
        if (position == 0)
        {
            newNode->next = list->head;
            list->head = newNode;
        }
        // Agregar al FINAL
        else if (position == 1)
        {
            list->tail->next = newNode;
            list->tail = newNode;
        }
        // Agregar al CENTRO
        else
        {
            int center = list->size / 2;
            OperativeNode *prev = getNodeAt(list, center - 1);
            newNode->next = prev->next;
            prev->next = newNode;
        }
    }

    list->size++;
    cout << "\n  Operativo agregado exitosamente.\n";
    return 1;
}

// ===========================
// MOSTRAR TODOS LOS OPERATIVOS
// ===========================
void displayAllOperatives(OperativeList *list)
{
    if (isEmpty(list))
    {
        cout << "\n  No hay operativos registrados.\n";
        return;
    }

    cout << "\n============================================\n";
    cout << "       LISTA DE OPERATIVOS (" << list->size << ")\n";
    cout << "============================================\n";

    OperativeNode *current = list->head;
    while (current != NULL)
    {
        display_operative(current);
        current = current->next;
    }
}

// ===========================
// BUSCAR OPERATIVO POR NOMBRE
// ===========================
void searchOperative(OperativeList *list, string name)
{
    if (isEmpty(list))
    {
        cout << "\n  No hay operativos registrados.\n";
        return;
    }

    OperativeNode *current = list->head;
    int found = 0;

    cout << "\n=== RESULTADOS DE BUSQUEDA: \"" << name << "\" ===\n";

    while (current != NULL)
    {
        if (current->name == name)
        {
            found++;
            cout << "\n--- Operativo #" << found << " ---\n";
            display_operative(current);
        }
        current = current->next;
    }

    if (found == 0)
    {
        cout << "\n  No se encontro ningun operativo con ese nombre.\n";
        cout << "  Desea agregarlo? (1=Si, 0=No): ";
        int option;
        cin >> option;
        cin.ignore();

        if (option == 1)
        {
            addOperative(list, 1);  // Agregar al final
        }
    }
    else
    {
        cout << "\n  Se encontraron " << found << " resultado(s).\n";
    }
}

// ===========================
// MOSTRAR POR FACCION
// ===========================
void displayByFaction(OperativeList *list, string faction)
{
    if (isEmpty(list))
    {
        cout << "\n  No hay operativos registrados.\n";
        return;
    }

    OperativeNode *current = list->head;
    int count = 0;

    cout << "\n=== OPERATIVOS DE FACCION: \"" << faction << "\" ===\n";

    while (current != NULL)
    {
        if (current->faction == faction)
        {
            count++;
            display_operative(current);
        }
        current = current->next;
    }

    if (count == 0)
        cout << "\n  No hay operativos de esta faccion.\n";
    else
        cout << "\n  Total: " << count << " operativo(s).\n";
}

// ===========================
// MODIFICAR OPERATIVO
// ===========================
void modifyOperative(OperativeList *list, string name)
{
    if (isEmpty(list))
    {
        cout << "\n  No hay operativos registrados.\n";
        return;
    }

    int matches = countMatches(list, name);

    if (matches == 0)
    {
        cout << "\n  No se encontro ningun operativo con el nombre: " << name << "\n";
        return;
    }

    // Si hay un solo resultado
    if (matches == 1)
    {
        OperativeNode *current = list->head;
        while (current != NULL)
        {
            if (current->name == name)
            {
                cout << "\n=== MODIFICAR OPERATIVO #" << current->id << " ===\n";
                cout << "  Nombre actual: " << current->name << "\n";
                cout << "  Ingrese nuevo nombre (Enter para mantener): ";
                string newName;
                getline(cin, newName);
                if (!newName.empty())
                    current->name = newName;

                cout << "\n  Faccion actual: " << current->faction << "\n";
                cout << "  Ingrese nueva faccion (Enter para mantener): ";
                string newFaction;
                getline(cin, newFaction);
                if (!newFaction.empty())
                    current->faction = newFaction;

                cout << "\n  Movimiento actual: " << current->movement << "\n";
                cout << "  Nuevo movimiento: ";
                int newMov;
                if (cin >> newMov)
                    current->movement = newMov;
                cin.ignore();

                cout << "\n  APL actual: " << current->apl << "\n";
                cout << "  Nuevo APL: ";
                int newApl;
                if (cin >> newApl)
                    current->apl = newApl;
                cin.ignore();

                cout << "\n  Salvacion actual: " << current->save << "\n";
                cout << "  Nueva salvacion: ";
                int newSave;
                if (cin >> newSave)
                    current->save = newSave;
                cin.ignore();

                cout << "\n  Heridas actuales: " << current->wounds << "\n";
                cout << "  Nuevas heridas: ";
                int newWounds;
                if (cin >> newWounds)
                    current->wounds = newWounds;
                cin.ignore();

                cout << "\n  Operativo modificado exitosamente.\n";
                return;
            }
            current = current->next;
        }
    }
    // Si hay multiples resultados
    else
    {
        cout << "\n  Se encontraron " << matches << " operativos con el nombre: " << name << "\n";
        cout << "  Seleccione cual desea modificar:\n";

        OperativeNode *current = list->head;
        int index = 1;
        OperativeNode *options[20];

        while (current != NULL)
        {
            if (current->name == name)
            {
                options[index] = current;
                cout << "  " << index << ". ID=" << current->id << " | Faccion: " << current->faction << "\n";
                index++;
            }
            current = current->next;
        }

        cout << "  Opcion: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice >= 1 && choice <= matches)
        {
            OperativeNode *op = options[choice];
            cout << "\n=== MODIFICAR OPERATIVO #" << op->id << " ===\n";

            cout << "  Nuevo nombre: ";
            string newName;
            getline(cin, newName);
            if (!newName.empty())
                op->name = newName;

            cout << "  Nueva faccion: ";
            string newFaction;
            getline(cin, newFaction);
            if (!newFaction.empty())
                op->faction = newFaction;

            cout << "  Nuevo movimiento: ";
            int newMov;
            if (cin >> newMov)
                op->movement = newMov;
            cin.ignore();

            cout << "  Nuevo APL: ";
            int newApl;
            if (cin >> newApl)
                op->apl = newApl;
            cin.ignore();

            cout << "  Nueva salvacion: ";
            int newSave;
            if (cin >> newSave)
                op->save = newSave;
            cin.ignore();

            cout << "  Nuevas heridas: ";
            int newWounds;
            if (cin >> newWounds)
                op->wounds = newWounds;
            cin.ignore();

            cout << "\n  Operativo modificado exitosamente.\n";
        }
        else
        {
            cout << "  Opcion invalida.\n";
        }
    }
}

// ===========================
// ELIMINAR OPERATIVO
// ===========================
void deleteOperative(OperativeList *list, string name)
{
    if (isEmpty(list))
    {
        cout << "\n  No hay operativos registrados.\n";
        return;
    }

    int matches = countMatches(list, name);

    if (matches == 0)
    {
        cout << "\n  No se encontro ningun operativo con el nombre: " << name << "\n";
        return;
    }

    // Si hay un solo resultado
    if (matches == 1)
    {
        OperativeNode *current = list->head;
        OperativeNode *prev = NULL;

        while (current != NULL)
        {
            if (current->name == name)
            {
                // Es el unico nodo
                if (list->size == 1)
                {
                    list->head = NULL;
                    list->tail = NULL;
                }
                // Es el primer nodo
                else if (current == list->head)
                {
                    list->head = current->next;
                }
                // Es el ultimo nodo
                else if (current == list->tail)
                {
                    list->tail = prev;
                    prev->next = NULL;
                }
                // Es un nodo del medio
                else
                {
                    prev->next = current->next;
                }

                delete current;
                list->size--;
                cout << "\n  Operativo eliminado exitosamente.\n";
                return;
            }
            prev = current;
            current = current->next;
        }
    }
    // Si hay multiples resultados
    else
    {
        cout << "\n  Se encontraron " << matches << " operativos con el nombre: " << name << "\n";
        cout << "  Seleccione cual desea eliminar:\n";

        OperativeNode *current = list->head;
        int index = 1;
        OperativeNode *options[20];

        while (current != NULL)
        {
            if (current->name == name)
            {
                options[index] = current;
                cout << "  " << index << ". ID=" << current->id << " | Faccion: " << current->faction << "\n";
                index++;
            }
            current = current->next;
        }

        cout << "  Opcion: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice >= 1 && choice <= matches)
        {
            OperativeNode *toDelete = options[choice];
            OperativeNode *current = list->head;
            OperativeNode *prev = NULL;

            while (current != NULL)
            {
                if (current == toDelete)
                {
                    if (list->size == 1)
                    {
                        list->head = NULL;
                        list->tail = NULL;
                    }
                    else if (current == list->head)
                    {
                        list->head = current->next;
                    }
                    else if (current == list->tail)
                    {
                        list->tail = prev;
                        prev->next = NULL;
                    }
                    else
                    {
                        prev->next = current->next;
                    }

                    delete current;
                    list->size--;
                    cout << "\n  Operativo eliminado exitosamente.\n";
                    return;
                }
                prev = current;
                current = current->next;
            }
        }
        else
        {
            cout << "  Opcion invalida.\n";
        }
    }
}

// ===========================
// MOSTRAR OPERATIVO COMPACTO
// ===========================
void displayOperativeCompact(OperativeNode *op)
{
    cout << "  [" << op->id << "] " << op->name
         << " | " << op->faction
         << " | M:" << op->movement
         << " APL:" << op->apl
         << " Sv:" << op->save << "+"
         << " W:" << op->wounds << "\n";
}