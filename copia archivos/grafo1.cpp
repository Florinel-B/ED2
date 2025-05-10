#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <unordered_set>

template <typename T, typename W>
class RedSocial{
public:
    typename T::ID newProfile(T& node){
        auto id = node.getID();
        if(nodes.contains(id)){
            return nullptr;
        }
        else{
            nodes[id] = {};
            perfiles[id] = node;
            return node;
        }
    }

    bool wantToBeFollower(typename T::ID& node1, typename T::ID& node2){
        if (!nodes.contains(node1) || !nodes.contains(node2)) {
            return false;
        }
        nodes[node1].insert(node2);
        return true;
    }

    auto& followers(typename T::ID& id) const{
        std::vector<typename T::ID> followe;
        for (const auto& [otroID, seguidos] : nodes){
            if(seguidos.contains(id)){
                followe.push_back(otroID);
            }
        }
        return followe;
    }

    std::vector<typename T::ID> following(const typename T::ID& id) const {
        std::vector<typename T::ID> resultado;
        if (!nodes.contains(id)) return resultado;
        for (const auto& seguido : nodes.at(id)) {
            resultado.push_back(seguido);
        }
        return resultado;
    }



private:
    std::unordered_map<typename T::ID, std::unordered_set<typename T::ID>> nodes;
    std::unordered_map<typename T::ID, T> perfiles;
};

struct Persona{
    using ID = std::string;
    std::string nombre;
    //int cod;

    ID getID() {return nombre;}
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}   