#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <list>
#include <string>
#include <queue>

class Router {
public:
    Router(char id);
    char idRouter;
    std::string camino;
    std::vector<std::pair<Router*, int>> vecinos; // vecinos y costo
    int distancia;
    bool visitado;
    void nuevoVecino(Router* vecino, int costo);
    void confDistancia(int distancia);
    void reinicio();
    bool operator==(const Router& otro) const {
        return idRouter == otro.idRouter;
    }
};

void dijkstra(Router* fuente, Router* destino, std::list<Router>& topologia);

bool verificarVecino(Router* a, const Router& b);

void verVecinos(const Router& a);

void generar_routers_Aleatorios(std::list<Router>& topologia);

bool verificarExistenciaRouter(std::list<Router>& topologia, char nombre, Router*& puntero);

void Buscando_camino(std::list<Router>& topologia);

void recibirConexiones(int& conexiones, int tamano);

void crearRouters(std::list<Router>& topologia);

void eliminarRouter(std::list<Router>& topologia);

#endif // ROUTER_H



