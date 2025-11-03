#include "archivo.h"
#include <iostream>
#include <list>

int main(){

    bool menu_mayor=true;
    std::string respuesta_menu_mayor;

    std::list <Router> topologia;

    while(menu_mayor){
        std::cout<<"BIENVENIDO\n"<<std::endl;
        std::cout<<"\t1. Generar routers aleatorios"<<std::endl;
        std::cout<<"\t2. Leer routers desde un archivo"<<std::endl;
        std::cout<<"\t3. Salir del sistema\n"<<std::endl;
        std::cout<<"Seleccione una opcion: ";std::cin>>respuesta_menu_mayor;
        if(respuesta_menu_mayor=="1"){
            generar_routers_Aleatorios(topologia);
            menus(topologia);
        }else if(respuesta_menu_mayor=="2"){
            bool segundo_menu;
            segundo_menu= leer_red_archivo(topologia);
            if(segundo_menu){
                menus(topologia);
            }
        }else if(respuesta_menu_mayor=="3"){
            menu_mayor=false;
        }else{
            std::cout<<"Ingresaste una opcion invalida"<<std::endl;
        }
    }
    return 0;
}
