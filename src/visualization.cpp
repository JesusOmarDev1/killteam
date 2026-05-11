#include <iostream>
#include <iomanip>
#include "../util/structures.h"

using namespace std;

// =============================================
// CONSTANTES DE FORMATO
// =============================================
const int COL_NAME     = 16;
const int COL_TYPE     = 5;
const int COL_RNG      = 5;
const int COL_A        = 3;
const int COL_HIT      = 5;
const int COL_D        = 4;
const int COL_DC       = 4;
const int COL_THREAT   = 7;

// =============================================
// ARMAS
// =============================================
void display_weapons(OperativeNode *op)
{
    cout << "\n  --- ARMAS ---\n";

    if (op->weaponCount > 0)
    {
        // Encabezados
        cout << "  " << left << setw(COL_NAME) << "NOMBRE"
             << setw(COL_TYPE + 1) << "TIPO"
             << setw(COL_RNG + 1) << "RNG"
             << setw(COL_A + 1) << "A"
             << setw(COL_HIT + 1) << "HIT"
             << setw(COL_D + 1) << "D"
             << setw(COL_DC + 1) << "Dc"
             << "AMENAZA\n";

        // Separador
        cout << "  ";
        cout << setfill('-') << setw(COL_NAME) << ""
             << setw(COL_TYPE + 1) << ""
             << setw(COL_RNG + 1) << ""
             << setw(COL_A + 1) << ""
             << setw(COL_HIT + 1) << ""
             << setw(COL_D + 1) << ""
             << setw(COL_DC + 1) << ""
             << setw(COL_THREAT) << "";
        cout << setfill(' ') << "\n";

        // Filas de armas
        for (int i = 0; i < op->weaponCount; i++)
        {
            Weapon *w = &op->weapons[i];
            string typeStr = (w->type == RANGE) ? "RNG" : "MEL";
            string rangeStr = (w->type == RANGE) ? to_string(w->range) + "\"" : "-";
            string dcStr = (w->criticalDamage > 0) ? to_string(w->criticalDamage) : "-";

            cout << "  " << left << setw(COL_NAME) << w->name
                 << setw(COL_TYPE + 1) << typeStr
                 << setw(COL_RNG + 1) << rangeStr
                 << right << setw(COL_A + 1) << w->attacks
                 << setw(COL_HIT + 1) << w->hit << "+"
                 << setw(COL_D + 1) << w->damage
                 << setw(COL_DC + 1) << dcStr
                 << setw(COL_THREAT) << w->threat << "\n";
        }

        cout << "\n  Total armas: " << op->weaponCount << "/" << MAX_WEAPONS << "\n";
    }
    else
    {
        cout << "  (Sin armas)\n";
    }
}

// =============================================
// DATACARD (STATLINE)
// =============================================
void display_statline(OperativeNode *op)
{
    cout << "  +-----------------+\n";
    cout << "  | " << left << setw(6) << "M"
         << setw(5) << "APL"
         << setw(6) << "SAVE"
         << setw(4) << "W" << " |\n";
    cout << "  +-----------------+\n";

    cout << "  | " << right << setw(3) << op->movement << "\""
         << setw(4) << op->apl
         << setw(5) << op->save << "+"
         << setw(4) << op->wounds << " |\n";

    cout << "  +-----------------+\n";
}

// =============================================
// UN OPERATIVO COMPLETO
// =============================================
void display_operative(OperativeNode *op)
{
    cout << "\n";
    cout << "####################################\n";
    cout << "# ID: " << op->id << "\n";
    cout << "# " << left << setw(32) << op->name << "#\n";
    cout << "####################################\n";

    cout << "  Faccion: " << op->faction << "\n";

    cout << "  Palabras clave: ";
    for (int i = 0; i < op->keywordCount; i++)
    {
        if (i > 0) cout << ", ";
        cout << op->keywords[i];
    }
    cout << "\n";

    cout << "  Arquetipo: " << op->archetype << "\n";

    display_statline(op);

    display_weapons(op);
}