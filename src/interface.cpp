#include <iostream>
#include <cstring>

using namespace std;

void limpiarPantalla()
{
#ifdef _WIN32
   system("cls");
#else
   system("clear");
#endif
}

void lineaDecorativa(int ancho)
{
   for (int i = 0; i < ancho; i++)
   {
      cout << "=";
   }
   cout << endl;
}

void lineaSimple(int ancho)
{
   for (int i = 0; i < ancho; i++)
   {
      cout << "-";
   }
   cout << endl;
}

void mostrarTitulo(const char *titulo)
{
   int ancho = 40;
   cout << endl;
   lineaDecorativa(ancho);
   int espacios = (ancho - strlen(titulo)) / 2;
   for (int i = 0; i < espacios; i++)
      cout << " ";
   cout << titulo << endl;
   lineaDecorativa(ancho);
}

void mostrarHeaderKillTeam()
{
   limpiarPantalla();
   cout << endl;
   lineaDecorativa(40);
   cout << "    KILL TEAM" << endl;
   lineaDecorativa(40);
   cout << endl;
}

void mostrarMensajeExito(const char *mensaje)
{
   cout << "\n  [OK] " << mensaje << endl;
}

void mostrarMensajeError(const char *mensaje)
{
   cout << "\n  [ERROR] " << mensaje << endl;
}

void mostrarMensajeAdvertencia(const char *mensaje)
{
   cout << "\n  [!] " << mensaje << endl;
}

void pausarPantalla()
{
   cout << "\n  Presione Enter para continuar...";
   cin.get();
}

void mostrarSubtitulo(const char *subtitulo)
{
   cout << "\n  --- " << subtitulo << " ---" << endl;
}