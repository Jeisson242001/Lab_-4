#include "router.h"
#include <algorithm>
#include <random>
#include <sstream>

Router::Router(char id) : idRouter(id), camino(""), distancia(INT_MAX), visitado(false) {}

void Router::nuevoVecino(Router* vecino, int costo) {
    vecinos.emplace_back(vecino, costo);
}

void Router::confDistancia(int dist) {
    distancia = dist;
}

void Router::reinicio() {
    distancia = INT_MAX;
    visitado = false;
    camino = "";
}

void dijkstra(Router* fuente, Router* destino, std::list<Router>& topologia) {
    if (!fuente || !destino) {
        std::cout << "Fuente o destino invalido.\n";
        return;
    }

    // Reiniciar estado de la topologia
    for (auto& r : topologia) r.reinicio();

    fuente->confDistancia(0);
    fuente->camino = std::string(1, fuente->idRouter);

    using Node = std::pair<int, Router*>;
    auto cmp = [](const Node& a, const Node& b){ return a.first > b.first; }; // min-heap
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);
    pq.push({0, fuente});

    while (!pq.empty()) {
        Router* actual = pq.top().second;
        pq.pop();

        if (actual->visitado) continue;
        actual->visitado = true;

        for (auto& vec : actual->vecinos) {
            Router* sigRouter = vec.first;
            int costoBorde = vec.second;
            if (!sigRouter) continue;
            int nuevaDistancia = actual->distancia + costoBorde;
            if (nuevaDistancia < sigRouter->distancia) {
                sigRouter->confDistancia(nuevaDistancia);
                sigRouter->camino = actual->camino + " -> " + std::string(1, sigRouter->idRouter);
                pq.push({nuevaDistancia, sigRouter});
            }
        }
    }

    if (destino->distancia == INT_MAX) {
        std::cout << "No hay camino desde " << fuente->idRouter << " hacia " << destino->idRouter << std::endl;
    } else {
        std::cout << "Camino mas corto a " << destino->idRouter <<" desde "<<fuente->idRouter<< " es " << destino->camino << std::endl;
        std::cout << "El menor costo a " << destino->idRouter <<" desde "<<fuente->idRouter<< ": " << destino->distancia << std::endl;
    }
}

// Métodos utilitarios
bool verificarVecino(Router* a, const Router& b){
    for(const auto& iter : b.vecinos){
        if(iter.first == a){
            return true;
        }
    }
    return false;
}