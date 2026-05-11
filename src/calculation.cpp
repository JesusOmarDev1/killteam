#include "../util/structures.h"

// Calcula la amenaza de un arma usando la formula:
// Amenaza = A * D + (A/6) * (Dc - D)
int calculate_threat(int attacks, int damage, int criticalDamage)
{
   int normalDamage = damage;
   int critDamage = criticalDamage;
   int difference = critDamage - normalDamage;

   return attacks * normalDamage + (attacks / 6) * difference;
}