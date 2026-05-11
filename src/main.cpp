#include <iostream>
#include <string>
#include "../lib/ascii-art/Ascii.h"
#include "../util/structures.h"
#include "../util/operativeslist.h"

using namespace std;

void welcome();
void display_menu();
void handle_add_operative(OperativeList *list);
void handle_search_operative(OperativeList *list);
void handle_display_by_faction(OperativeList *list);
void handle_modify_operative(OperativeList *list);
void handle_delete_operative(OperativeList *list);

int main()
{
   OperativeList team = {};
   initializeList(&team);

   welcome();

   int option;

   do
   {
      display_menu();
      cout << "  Seleccione una opcion: ";
      cin >> option;
      cin.ignore();

      switch (option)
      {
      case 1:
         handle_add_operative(&team);
         break;

      case 2:
         displayAllOperatives(&team);
         break;

      case 3:
         handle_search_operative(&team);
         break;

      case 4:
         handle_display_by_faction(&team);
         break;

      case 5:
         handle_modify_operative(&team);
         break;

      case 6:
         handle_delete_operative(&team);
         break;

      case 0:
         cout << "\n  Saliendo...\n";
         break;

      default:
         cout << "\n  Opcion invalida. Intente de nuevo.\n";
         break;
      }

   } while (option != 0);

   cout << "\n  ¡Gracias por usar Kill Team - Fire Team Editor!\n\n";
   return 0;
}

void display_menu()
{
   cout << "\n";
   cout << "====================================\n";
   cout << "      KILL TEAM - MENU PRINCIPAL\n";
   cout << "====================================\n";
   cout << "  1. Agregar operativo (inicio/fin/centro)\n";
   cout << "  2. Mostrar todos los operativos\n";
   cout << "  3. Buscar operativo por nombre\n";
   cout << "  4. Mostrar operativos por faccion\n";
   cout << "  5. Modificar operativo\n";
   cout << "  6. Eliminar operativo\n";
   cout << "  0. Salir\n";
   cout << "====================================\n";
}

void welcome()
{
   ascii::Ascii font(ascii::FontName::sevenstar);
   font.print("KILLTEAM");
   cout << "\n";
}

void handle_add_operative(OperativeList *list)
{
   cout << "\n=== AGREGAR OPERATIVO ===\n";
   cout << "  Donde desea agregarlo?\n";
   cout << "  0 = INICIO de la lista\n";
   cout << "  1 = FINAL de la lista\n";
   cout << "  2 = CENTRO de la lista\n";
   cout << "  Opcion: ";

   int position;
   cin >> position;
   cin.ignore();

   if (position < 0 || position > 2)
   {
      cout << "\n  Posicion invalida. Se agregara al final.\n";
      position = 1;
   }

   addOperative(list, position);
}

void handle_search_operative(OperativeList *list)
{
   cout << "\n=== BUSCAR OPERATIVO ===\n";
   cout << "  Ingrese nombre a buscar: ";
   string name;
   getline(cin, name);

   searchOperative(list, name);
}

void handle_display_by_faction(OperativeList *list)
{
   cout << "\n=== MOSTRAR POR FACCION ===\n";
   cout << "  Ingrese faccion: ";
   string faction;
   getline(cin, faction);

   displayByFaction(list, faction);
}

void handle_modify_operative(OperativeList *list)
{
   cout << "\n=== MODIFICAR OPERATIVO ===\n";
   cout << "  Ingrese nombre del operativo a modificar: ";
   string name;
   getline(cin, name);

   modifyOperative(list, name);
}

void handle_delete_operative(OperativeList *list)
{
   cout << "\n=== ELIMINAR OPERATIVO ===\n";
   cout << "  Ingrese nombre del operativo a eliminar: ";
   string name;
   getline(cin, name);

   deleteOperative(list, name);
}