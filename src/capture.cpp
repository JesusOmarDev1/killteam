#include <iostream>
#include "../util/structures.h"

using namespace std;

// ===========================
// CAPTURA DE ARMA
// ===========================
void capture_weapon(Weapon *weapon)
{
    int typeChoice;

    cout << "    Tipo de Arma (0 = RANGO, 1 = CUERPO A CUERPO): ";
    cin >> typeChoice;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        typeChoice = 0;
    }
    weapon->type = (typeChoice == 0) ? RANGE : MELEE;

    cout << "    Nombre del arma: ";
    cin.ignore();
    getline(cin, weapon->name);

    if (weapon->type == RANGE)
    {
        cout << "    Rango (pulgadas): ";
        cin >> weapon->range;
    }
    else
    {
        weapon->range = 0;
    }

    cout << "    Ataques (A): ";
    cin >> weapon->attacks;

    cout << "    Para impactar (3 = 3+, 4 = 4+): ";
    cin >> weapon->hit;

    cout << "    Dano normal (D): ";
    cin >> weapon->damage;

    cout << "    Dano critico (Dc, 0 si no tiene): ";
    cin >> weapon->criticalDamage;

    weapon->threat = calculate_threat(weapon->attacks, weapon->damage, weapon->criticalDamage);

    cout << "    Reglas especiales (max " << MAX_WEAPON_RULES << ", -1 para terminar):\n";
    weapon->ruleCount = 0;

    for (int i = 0; i < MAX_WEAPON_RULES; i++)
    {
        cout << "    Regla " << (i + 1) << ": ";
        cin >> weapon->rules[i];

        if (weapon->rules[i] == "-1")
        {
            weapon->rules[i] = "";
            break;
        }
        weapon->ruleCount++;
    }
}

// ===========================
// CAPTURA DE DATOS DEL OPERATIVO
// ===========================
void capture_operative_data(OperativeNode *op)
{
    cout << "  Nombre: ";
    getline(cin, op->name);

    cout << "  Faccion: ";
    getline(cin, op->faction);

    cout << "  Palabras clave (max " << MAX_KEYWORDS << ", -1 para terminar):\n";
    op->keywordCount = 0;

    for (int i = 0; i < MAX_KEYWORDS; i++)
    {
        cout << "    Palabra clave " << (i + 1) << ": ";
        getline(cin, op->keywords[i]);

        if (op->keywords[i] == "-1")
        {
            op->keywords[i] = "";
            break;
        }
        op->keywordCount++;
    }

    cout << "  Arquetipo: ";
    getline(cin, op->archetype);

    cout << "  Estadisticas:\n";
    cout << "    Movimiento (M): ";
    cin >> op->movement;

    cout << "    APL: ";
    cin >> op->apl;

    cout << "    Salvacion (3 = 3+, 4 = 4+): ";
    cin >> op->save;

    cout << "    Heridas (W): ";
    cin >> op->wounds;

    cout << "  Armas (max " << MAX_WEAPONS << "):\n";
    cout << "    Cuantas armas tiene? ";
    cin >> op->weaponCount;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        op->weaponCount = 0;
    }

    if (op->weaponCount < 0 || op->weaponCount > MAX_WEAPONS)
    {
        op->weaponCount = (op->weaponCount > MAX_WEAPONS) ? MAX_WEAPONS : 0;
        cout << "    Limitado a " << MAX_WEAPONS << " armas.\n";
    }

    cin.ignore();

    for (int i = 0; i < op->weaponCount; i++)
    {
        cout << "\n    --- Arma " << (i + 1) << " ---\n";
        capture_weapon(&op->weapons[i]);
    }

    op->abilityCount = 0;

    if (!validate_total_weapons(op))
    {
        cout << "  ERROR: Total de armas excede el maximo.\n";
    }
}