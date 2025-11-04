#include "archivo.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>

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

bool leer_red_archivo(std::list<Router>& topologia){
    std::string nombre_archivo;
    std::string linea;
    std::cout<<"Ingrese el nombre del archivo a leer (sin extensión): ";
    std::cin>> nombre_archivo;
    nombre_archivo+= ".txt";
    if(!comprobarLectura(nombre_archivo)) return false;

    std::ifstream archivo(nombre_archivo);
    if(!archivo.is_open()){
        std::cerr<<"Error abriendo el archivo despues de la verificacion.\n";
        return false;
    }

    topologia.clear();

    // Primera pasada: crear routers (primer caracter de cada linea que sea letra)
    archivo.clear();
    archivo.seekg(0);
    while(std::getline(archivo, linea)){
        bool hasLetter = false;
        char id = '\0';
        for(char c : linea){
            if(std::isalpha(static_cast<unsigned char>(c))){
                id = c;
                hasLetter = true;
                break;
            }
        }
        if(!hasLetter) continue;
        topologia.emplace_back(id);
    }

    // Segunda pasada: parsear vecinos y costos
    archivo.clear();
    archivo.seekg(0);
    while(std::getline(archivo, linea)){
        if(linea.empty()) continue;

        // Encontrar primer caracter letra (id fuente)
        size_t pos = std::string::npos;
        for(size_t i=0; i<linea.size(); ++i){
            if(std::isalpha(static_cast<unsigned char>(linea[i]))){
                pos = i;
                break;
            }
        }
        if(pos == std::string::npos) continue;
        char id_fuente = linea[pos];

        // Tomar el resto de la línea después de la primera letra
        std::string rest = linea.substr(pos + 1);

        // Parse robusto: buscar pares letra + número (número puede tener varios dígitos)
        size_t i = 0;
        while(i < rest.size()){
            // buscar siguiente letra
            while(i < rest.size() && !std::isalpha(static_cast<unsigned char>(rest[i]))) ++i;
            if(i >= rest.size()) break;
            char id_vec = rest[i];
            ++i;
            // leer dígitos del costo
            std::string num;
            while(i < rest.size() && std::isdigit(static_cast<unsigned char>(rest[i]))){
                num.push_back(rest[i]);
                ++i;
            }
            if(num.empty()) continue;
            int cost = std::stoi(num);

            // Buscar punteros a routers en topologia
            Router* ptr_fuente = nullptr;
            Router* ptr_vec = nullptr;
            if(verificarExistenciaRouter(topologia, id_fuente, ptr_fuente) &&
                verificarExistenciaRouter(topologia, id_vec, ptr_vec)){
                ptr_fuente->nuevoVecino(ptr_vec, cost);
                // Si deseas red no dirigida, puedes también agregar la inversa:
                // ptr_vec->nuevoVecino(ptr_fuente, cost);
            }
        }
    }

    archivo.close();
    return true;
}

