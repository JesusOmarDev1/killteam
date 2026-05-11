#include <iostream>
#include <string>
#include "../lib/ascii-art/Ascii.h"

using namespace std;

int main() {

   welcome();

   return 0;
}

void welcome() {
   ascii::Ascii font(ascii::FontName::sevenstar);
   font.print("KILLTEAM");
}