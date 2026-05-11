#include "../util/structures.h"

// Valida que el total de armas sea <= MAX_WEAPONS
bool validate_total_weapons(OperativeNode *op)
{
    return op->weaponCount <= MAX_WEAPONS;
}

// Valida que keywords sea <= MAX_KEYWORDS
bool validate_keyword_count(int count)
{
    return count <= MAX_KEYWORDS;
}