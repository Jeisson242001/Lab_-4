#include "router.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <limits>
#include <queue>
#include <cctype>

// ------------------------
// Constructor y métodos básicos
// ------------------------
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

// ------------------------
// Dijkstra
// ------------------------
void dijkstra(Router* fuente, Router* destino, std::list<Router>& topologia) {
    if (!fuente || !destino) {
        std::cout << "Fuente o destino invalido.\n";
        return;
    }

    for (auto& r : topologia) r.reinicio();

    fuente->confDistancia(0);
    fuente->camino = std::string(1, fuente->idRouter);

    using Node = std::pair<int, Router*>;
    auto cmp = [](const Node& a, const Node& b){ return a.first > b.first; };
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

// ------------------------
// Funciones auxiliares
// ------------------------
bool verificarVecino(Router* a, const Router& b){
    for(const auto& iter : b.vecinos){
        if(iter.first == a){
            return true;
        }
    }
    return false;
}

void verVecinos(const Router& a){
    for(const auto& iter : a.vecinos){
        std::cout << iter.first->idRouter << " (cost: " << iter.second << ")\n";
    }
}

// ------------------------
// Validación numérica segura
// ------------------------
int leerNumeroPositivo(const std::string& mensaje) {
    int numero;
    while (true) {
        std::cout << mensaje;
        std::string entrada;
        std::cin >> entrada;

        bool esNumero = !entrada.empty() &&
                        std::all_of(entrada.begin(), entrada.end(), ::isdigit);

        if (esNumero) {
            numero = std::stoi(entrada);
            if (numero >= 0)
                return numero;
        }
        std::cout << "Entrada invalida. Ingrese solo numeros positivos.\n";
    }
}

// ------------------------
// Generar routers aleatorios
// ------------------------
void generar_routers_Aleatorios(std::list<Router>& topologia){
    topologia.clear();

    std::random_device rd;
    std::mt19937 gen(rd());

    int routers = leerNumeroPositivo("Ingrese la cantidad de routers a generar: ");
    if(routers <= 0){
        std::cout << "Cantidad invalida.\n";
        return;
    }

    const int MIN_NEIGHBORS = 1;
    const int MAX_NEIGHBORS = 3;
    const int COST_MIN = 1;
    const int COST_MAX = 12;

    char base = 'A';
    for(int i = 0; i < routers; ++i){
        topologia.emplace_back(static_cast<char>(base + i));
    }

    std::vector<Router*> nodes;
    nodes.reserve(routers);
    for(auto& r : topologia) nodes.push_back(&r);

    std::uniform_int_distribution<> costDist(COST_MIN, COST_MAX);
    std::uniform_int_distribution<> indexDist(0, std::max(0, routers - 1));
    std::uniform_int_distribution<> extraEdgeChance(0, 100);

    std::vector<int> indices(routers);
    for(int i = 0; i < routers; ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), gen);

    for(int i = 0; i + 1 < (int)indices.size(); ++i){
        Router* a = nodes[indices[i]];
        Router* b = nodes[indices[i+1]];
        if(!verificarVecino(b, *a) && !verificarVecino(a, *b)){
            int cost = costDist(gen);
            a->nuevoVecino(b, cost);
            b->nuevoVecino(a, cost);
        }
    }

    auto neighborCount = [](Router* r)->int{ return static_cast<int>(r->vecinos.size()); };
    if(routers > 1){
        for(int i = 0; i < routers; ++i){
            Router* r = nodes[i];
            while(neighborCount(r) < MIN_NEIGHBORS){
                int target = indexDist(gen);
                if(target == i) continue;
                Router* t = nodes[target];
                if(neighborCount(t) >= MAX_NEIGHBORS) continue;
                if(!verificarVecino(t, *r)){
                    int cost = costDist(gen);
                    r->nuevoVecino(t, cost);
                    t->nuevoVecino(r, cost);
                } else continue;
            }
        }
    }

    std::cout << "\nTopologia generada (cada linea: vecino (costo)):\n\n";
    for(const auto& rout : topologia){
        std::cout << rout.idRouter << " vecinos:\n";
        verVecinos(rout);
        std::cout<< std::endl;
    }
}

// ------------------------
// Buscar camino más corto
// ------------------------
bool verificarExistenciaRouter(std::list<Router>& topologia, char nombre, Router*& puntero){
    for(auto& c_router: topologia){
        if(c_router.idRouter == nombre){
            puntero= &c_router;
            return true;
        }
    }
    return false;
}

void Buscando_camino(std::list<Router>& topologia){
    char id_router_fuente, id_router_destino;
    Router* puntero_fuente = nullptr;
    Router* puntero_destino = nullptr;

    while(true){
        std::cout<<"Ingrese la Id del router fuente (letra): ";
        std::cin>>id_router_fuente;
        if(!isalpha(id_router_fuente)){
            std::cout << "Entrada invalida. Debe ser una letra.\n";
            continue;
        }
        if(!verificarExistenciaRouter(topologia, id_router_fuente, puntero_fuente)){
            std::cout << "La id ingresada no existe.\n";
            continue;
        }
        std::cout<<"Ingrese la Id del router destino (letra): ";
        std::cin>>id_router_destino;
        if(!isalpha(id_router_destino)){
            std::cout << "Entrada invalida. Debe ser una letra.\n";
            continue;
        }
        if(!verificarExistenciaRouter(topologia, id_router_destino, puntero_destino)){
            std::cout << "La id ingresada no existe.\n";
            continue;
        }
        dijkstra(puntero_fuente, puntero_destino, topologia);
        break;
    }
}

// ------------------------
// Crear routers manualmente
// ------------------------
void crearRouters(std::list<Router>& topologia){
    char nuevo;
    while(true){
        std::cout << "Ingrese el identificador del router (una letra): ";
        std::cin >> nuevo;
        if(!isalpha(nuevo)){
            std::cout << "Entrada invalida. Debe ser una letra.\n";
            continue;
        }
        bool existe = false;
        for(const auto& r : topologia){
            if(r.idRouter == nuevo){
                existe = true;
                break;
            }
        }
        if(existe){
            std::cout << "Ese router ya existe. Intente otro nombre.\n";
            continue;
        }
        topologia.emplace_back(nuevo);
        break;
    }

    Router* nuevoPtr = &topologia.back();
    int tamano = static_cast<int>(topologia.size());
    int conexiones = leerNumeroPositivo("Ingrese la cantidad de vecinos: ");
    if(conexiones >= tamano){
        std::cout << "Cantidad invalida. No puede exceder el total de routers existentes.\n";
        return;
    }

    for(int k = 0; k < conexiones; ){
        char vecino;
        std::cout <<"Nombre del vecino: ";
        std::cin >> vecino;
        if(!isalpha(vecino)){
            std::cout << "Entrada invalida. Debe ser una letra.\n";
            continue;
        }
        Router* vecinoPtr = nullptr;
        if(!verificarExistenciaRouter(topologia, vecino, vecinoPtr)){
            std::cout << "No existe ese router.\n";
            continue;
        }
        if(vecinoPtr->idRouter == nuevoPtr->idRouter){
            std::cout << "No puedes conectar un router a si mismo.\n";
            continue;
        }
        if(verificarVecino(vecinoPtr, *nuevoPtr)){
            std::cout << "Ya son vecinos.\n";
            continue;
        }
        int costo = leerNumeroPositivo("Ingrese el costo del enlace: ");
        nuevoPtr->nuevoVecino(vecinoPtr, costo);
        vecinoPtr->nuevoVecino(nuevoPtr, costo);
        ++k;
    }

    for(const auto& rout : topologia){
        std::cout << rout.idRouter << " vecinos:\n";
        verVecinos(rout);
        std::cout<< std::endl;
    }
}

// ------------------------
// Eliminar router
// ------------------------
void eliminarRouter(std::list<Router>& topologia){
    char nombre;
    Router* ptr = nullptr;
    while(true){
        std::cout << "Ingrese el identificador del router a eliminar (letra): ";
        std::cin >> nombre;
        if(!isalpha(nombre)){
            std::cout << "Entrada invalida. Debe ser una letra.\n";
            continue;
        }
        if(!verificarExistenciaRouter(topologia, nombre, ptr)){
            std::cout << "No existe ese router.\n";
            continue;
        }
        break;
    }

    for(auto& r : topologia){
        r.vecinos.erase(std::remove_if(r.vecinos.begin(), r.vecinos.end(),
                                       [nombre](const std::pair<Router*, int>& p){
                                           return p.first && p.first->idRouter == nombre;
                                       }),
                        r.vecinos.end());
    }

    topologia.remove_if([nombre](const Router& r){ return r.idRouter == nombre; });

    std::cout << "Router eliminado correctamente.\n";
}
