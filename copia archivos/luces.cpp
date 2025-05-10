#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>



struct estado {
    int id;
    bool esEncendido;

    bool operator==(const estado& other) const {
        return id == other.id && esEncendido == other.esEncendido;
    }
    using ID = int;
    ID getID() const {
        return id;
    }
};
template<typename T>
class GrafoNoDirigido {
    using ID = typename T::ID;
private:

    std::unordered_map<ID, T> Nodes;
    std::unordered_map<ID, std::unordered_set<ID>> adjList;

public:

    GrafoNoDirigido(std::list<T>& enteros, std::list<std::pair<ID,ID>>& aristas) {
        for (const auto& e : enteros) {
            Nodes[e.getID()] = e;
        }

        for (const auto& arista : aristas) {
            int u = arista.first;
            int v = arista.second;

            adjList[u].insert(v);
            adjList[v].insert(u);
        }
    }

    

    std::unordered_set<ID> obtenerVecinos(ID u) {
        return adjList[u];
    };

    void changeTheState(ID id) {
        Nodes[id].esEncendido = !Nodes[id].esEncendido;
        for (const auto& vecino : adjList[id]) {
            Nodes[vecino].esEncendido = !Nodes[vecino].esEncendido;
        }
    };  

    bool endOfGame() {
        auto estadoReal = Nodes.begin()->second.esEncendido;
        for (const auto& e : Nodes) {
            if (estadoReal != e.second.esEncendido) {
                std::cout << "No finalizado" << std::endl;
                return false;
            }
        }
        return true;
    };  

    void updateGame(){
        for (const auto& e: Nodes){
            bool a = true;
            for (const auto& adjList: adjList[e.first]){
                if (Nodes[adjList].esEncendido != e.second.esEncendido){
                    a = false;
                    break;
                }
            }
            if (a) {
                adjList.erase(e.first); 
                Nodes.erase(e.first);
            }

        }
    }

    void print() {
        std::cout << "Grafo No Dirigido:\n";
        for (const auto& node : Nodes) {
            std::cout << "Nodo: " << node.first << " Estado: " << (node.second.esEncendido ? "Encendido" : "Apagado") << "\n";
            std::cout << "Vecinos: ";
            for (const auto& vecino : adjList[node.first]) {
                std::cout << vecino << " ";
            }
            std::cout << "\n";
        }
    };


    

};
  

int main(){
    auto agua = std::list<estado>{estado{1, true}, estado{2, false}, estado{3, true}, estado{4, false}};
    auto aristas = std::list<std::pair<int,int>>{{1, 2}, {2, 3}, {3, 4}, {4, 1}};
    GrafoNoDirigido<estado> grafo(agua, aristas);
    grafo.print();
    grafo.changeTheState(1);
    grafo.print();
}