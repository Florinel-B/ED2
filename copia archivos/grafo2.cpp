#include <iostream>
#include <unordered_set>
#include <vector>
#include <list>
#include <unordered_map>

template <typename T, size_t N>
class ListaAdy{
public:
    //using ID = typename T::ID;
    using Ady = std::tuple<typename T::ID>;
    using AdyList = std::list<Ady>; // std::unordered_set<Ady>
    //using AdyHash = std::unordered_set<Ady>;

    void insertNode(T e){
        //g.emplace(e);
        auto n = Node{e};
        g[e.getID()] = n;
    }
    void borrarAdy(typename T::ID n1, typename T::ID n2) {
        auto& node1 = g[n1];
        node1.ady.remove(std::tuple<typename T::ID>(n2));

        auto& node2 = g[n2];
        node1.ady.remove(std::tuple<typename T::ID>(n1));
    }

    void borrarNode(typename T::ID n) { //si es grafo dirigido hay que visitar todo el grafo y luego todas las adyaciencias
        auto node = g[n];
        for (auto& e : node.ady) {
            //borrarAdy(n, std::get<0>(e));
            removea(std::get<0>(e),n);
        }
        g.erase(n);
    }

    void removea(typename T::ID n1, typename T::ID n2) {
        auto& node1 = g[n1];
        node1.ady.remove(std::tuple<typename T::ID>(n2));
    }
    void insertAdy(typename T::ID n1, typename T::ID n2){ //W); {
        auto& node1 = g[n1];
        auto ady1 = node1.ady;
        ady1.emplace(ady1.begin(), n2);

        //no dirigido
        auto& node2 = g[n2];
        auto ady2 = node2.ady;
        ady2.emplace(ady2.begin(), n1);
    }

    void imprimir() const {
        for (const auto& [id, node] : g) {
            std::cout << "Nodo " << id << " (" << node.e.nombre << ") está conectado con: ";
            for (const auto& ady : node.ady) {
                std::cout << get<0>(ady) << " ";
            }
            std::cout << std::endl;
        }
    }

    bool contains(typename T::ID n){
        return g.contains(n);
    }


    bool contains(T e){
        return g.contains(e.getID());
    }


    bool areNeighbors(typename  T::ID n1, typename T::ID n2){
        auto& node1 = g[n1];
        auto& ady1 = node1.ady;
        return (ady1.find(n2) != ady1.end());
    }

    auto adyList(typename T::ID n){
        return g[n].ady;
    }

    auto neighbors(typename T::ID n){
        const auto& ady = g[n].ady;
        std::vector<T> nodes{}; //std::vector<T> v{};
        for (const auto& e: ady){
            nodes.push_back(e); //v.push_back(g[e]) --> Devuelve lista de T
        }
        return nodes;
    }

private:

    struct Node{

        T e;
        AdyList ady;

        Node() : e(), ady() {}
        Node(T e) : e(e), ady() {}
    };
    std::unordered_map<typename T::ID, Node> g; //std::list<Node>


};

struct Expediente{
    std::string nombre;
};

struct Estudiante{
    using ID = int;
    std::string nombre;
    int cod;
    Expediente e;

    [[nodiscard]] ID getID() const {return cod;}
};



int main() {
    ListaAdy<Estudiante, 5> g;

    // Crear estudiantes
    Estudiante e1{"Juan", 0, {"Expediente A"}};
    Estudiante e2{"Ana", 1, {"Expediente B"}};
    Estudiante e3{"Luis", 2, {"Expediente C"}};
    Estudiante e4{"Carla", 3, {"Expediente D"}};
    Estudiante e5{"Pablo", 4, {"Expediente E"}};

    // Insertar nodos
    g.insertNode(e1);
    g.insertNode(e2);
    g.insertNode(e3);
    g.insertNode(e4);
    g.insertNode(e5);

    // Insertar aristas (relaciones)
    g.insertAdy(0, 1);
    g.insertAdy(0, 4);
    g.insertAdy(1, 2);
    g.insertAdy(1, 3);
    g.insertAdy(1, 4);
    g.insertAdy(2, 3);
    g.insertAdy(3, 4);

    g.borrarNode(0);

    // Mostrar grafo
    std::cout << "Lista de Adyacencia:\n";
    g.imprimir();
    return 0;
}