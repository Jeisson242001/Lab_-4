#include "archivo.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <string>
#include "router.h"



// ------------------------
// Verificar existencia de archivo
// ------------------------
bool comprobarLectura(const std::string& nombredelarchivo){
    std::ifstream archivo(nombredelarchivo);
    if(!archivo){
        std::cerr << "\nNo se puede abrir el archivo. No se encuentra.\n" << std::endl;
        return false;
    }else{
        archivo.close();
        std::cout << "\nVerificacion exitosa\n" << std::endl;
        return true;
    }
}

// ------------------------
// Leer topología desde archivo
// ------------------------
bool leer_red_archivo(std::list<Router>& topologia){
    std::string nombre_archivo;
    std::cout<<"Ingrese el nombre del archivo a leer (sin extension): ";
    std::cin>> nombre_archivo;
    nombre_archivo+= ".txt";
    if(!comprobarLectura(nombre_archivo)) return false;

    std::ifstream archivo(nombre_archivo);
    if(!archivo.is_open()){
        std::cerr<<"Error abriendo el archivo despues de la verificacion.\n";
        return false;
    }

    topologia.clear();
    std::string linea;

    // Crear routers
    while(std::getline(archivo, linea)){
        if(linea.empty()) continue;
        char id = linea[0];
        if(std::isalpha(static_cast<unsigned char>(id)))
            topologia.emplace_back(id);
    }

    // Parsear conexiones
    archivo.clear();
    archivo.seekg(0);
    while(std::getline(archivo, linea)){
        if(linea.empty()) continue;
        char id_fuente = linea[0];
        std::string rest = linea.substr(2); // salta el ':'
        std::stringstream ss(rest);
        std::string token;
        while(std::getline(ss, token, ',')){
            if(token.size() < 2) continue;
            char id_vec = token[0];
            std::string num = token.substr(1);
            if(!std::all_of(num.begin(), num.end(), ::isdigit)) continue;
            int cost = std::stoi(num);

            Router* ptr_fuente = nullptr;
            Router* ptr_vec = nullptr;
            if(verificarExistenciaRouter(topologia, id_fuente, ptr_fuente) &&
                verificarExistenciaRouter(topologia, id_vec, ptr_vec)){
                ptr_fuente->nuevoVecino(ptr_vec, cost);
                ptr_vec->nuevoVecino(ptr_fuente, cost);
            }
        }
    }

    archivo.close();
    std::cout << "\nArchivo leido exitosamente.\n";
    return true;
}

// ------------------------
// Guardar red (sin duplicar datos)
// ------------------------


bool guardar_red_archivo(const std::list<Router>& topologia, const std::string& nombre_archivo) {
    if (topologia.empty()) {
        std::cerr << "⚠️ La topologia esta vacia. Nada que guardar.\n";
        return false;
    }

    // --- Leer routers actuales del archivo (si existe) ---
    std::unordered_map<std::string, std::string> routers_guardados;
    std::ifstream lectura(nombre_archivo);
    if (lectura.is_open()) {
        std::string linea;
        while (std::getline(lectura, linea)) {
            if (!linea.empty()) {
                size_t pos = linea.find(':');
                if (pos != std::string::npos) {
                    std::string id = linea.substr(0, pos);
                    routers_guardados[id] = linea;
                }
            }
        }
        lectura.close();
    }

    // --- Actualizar y agregar routers según la topología actual ---
    for (const auto& router : topologia) {
        std::ostringstream nueva_linea;
        nueva_linea << router.idRouter << ":";

        bool primero = true;
        std::unordered_map<char, bool> vecinos_unicos; // evitar duplicar vecinos

        for (const auto& par : router.vecinos) {
            if (par.first && !vecinos_unicos[par.first->idRouter]) {
                if (!primero) nueva_linea << ",";
                nueva_linea << par.first->idRouter << par.second;
                primero = false;
                vecinos_unicos[par.first->idRouter] = true;
            }
        }

        routers_guardados[std::string(1, router.idRouter)] = nueva_linea.str();
    }

    // --- Eliminar routers que ya no existen ---
    std::unordered_map<std::string, bool> en_topologia;
    for (const auto& router : topologia)
        en_topologia[std::string(1, router.idRouter)] = true;

    for (auto it = routers_guardados.begin(); it != routers_guardados.end(); ) {
        if (en_topologia.find(it->first) == en_topologia.end())
            it = routers_guardados.erase(it);
        else
            ++it;
    }

    // --- Escribir todo al archivo sin duplicar ---
    std::ofstream escritura(nombre_archivo, std::ios::out | std::ios::trunc);
    if (!escritura.is_open()) {
        std::cerr << "❌ No se pudo abrir el archivo para escribir: " << nombre_archivo << "\n";
        return false;
    }

    for (const auto& par : routers_guardados)
        escritura << par.second << "\n";

    escritura.close();
    std::cout << "Topologia actualizada sin duplicar datos en: " << nombre_archivo << std::endl;
    return true;
}


// ------------------------
// Menú principal de operaciones
// ------------------------
void menus(std::list<Router>& topologia) {
    bool activo = true;
    std::string opcion;

    while (activo) {
        std::cout << "\n----- MENU DE OPERACIONES -----\n";
        std::cout << "1. Buscar camino mas corto\n";
        std::cout << "2. Agregar router\n";
        std::cout << "3. Eliminar router\n";
        std::cout << "4. Guardar red en archivo\n";
        std::cout << "5. Salir al menu principal\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        if (opcion == "1") {
            Buscando_camino(topologia);
        } else if (opcion == "2") {
            crearRouters(topologia);
        } else if (opcion == "3") {
            eliminarRouter(topologia);
        } else if (opcion == "4") {
            std::string nombre;
            std::cout << "Ingrese el nombre del archivo donde guardar (sin extension): ";
            std::cin >> nombre;
            nombre += ".txt";
            guardar_red_archivo(topologia, nombre);
        } else if (opcion == "5") {
            activo = false;
        } else {
            std::cout << "Entrada invalida, intente de nuevo.\n";
        }
    }
}
