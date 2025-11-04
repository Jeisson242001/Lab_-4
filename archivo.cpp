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

