#include "archivo.h"
#include <iostream>
#include <list>
#include <limits>
#include <cctype>

// ==================================================
// 🔒 Funciones de validación
// ==================================================

// Verifica que una cadena sea numérica
bool esNumero(const std::string &str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Pide una opción numérica válida (solo números del 1 al 3)
std::string leerOpcionMenu() {
    std::string entrada;
    bool valido = false;

    while (!valido) {
        std::cin >> entrada;

        if (esNumero(entrada) && (entrada == "1" || entrada == "2" || entrada == "3")) {
            valido = true;
        } else {
            std::cout << "Entrada invalida. Intente nuevamente: ";
            // Limpia el buffer en caso de error
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return entrada;
}

// ==================================================
// 🧠 Lógica principal (idéntica a la tuya)
// ==================================================
int main() {
    bool menu_mayor = true;
    std::string respuesta_menu_mayor;

    std::list<Router> topologia;

    while (menu_mayor) {
        std::cout << "BIENVENIDO\n" << std::endl;
        std::cout << "\t1. Generar routers aleatorios" << std::endl;
        std::cout << "\t2. Leer routers desde un archivo" << std::endl;
        std::cout << "\t3. Salir del sistema\n" << std::endl;
        std::cout << "Seleccione una opcion: ";

        // 🔹 Validación de la entrada
        respuesta_menu_mayor = leerOpcionMenu();

        if (respuesta_menu_mayor == "1") {
            generar_routers_Aleatorios(topologia);
            menus(topologia);
        } else if (respuesta_menu_mayor == "2") {
            bool segundo_menu = leer_red_archivo(topologia);
            if (segundo_menu) {
                menus(topologia);
            }
        } else if (respuesta_menu_mayor == "3") {
            menu_mayor = false;
        }
    }

    return 0;
}
