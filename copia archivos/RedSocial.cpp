#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string> 
#include <queue> 

struct Persona{
    int id;
    std::string nombre;
    std::string apellido;
    std::string email;
    std::string telefono;
    using ID = int;
    ID getID() const {
        return id;
    }
};
template<typename T>
class RedSocial{
    using ID = typename T::ID;

    public :
        ID* newProfile(std::string p){
            for (const auto& e : personas){
                if (e.second == p){
                    std::cout << "Ya existe un perfil con ese nombre" << std::endl;
                    return nullptr;
                }

            }
            ID id = personas.size() + 1;    
            //crear perfil usar constructor
            personas[id] = p;
            amigos[id] = std::unordered_set<ID>();
            return &id;
        } 


        bool wantToBeFollower(ID a, ID b){
            if (personas.find(a) == personas.end() ){
                std::cout << "No existe el perfil" << std::endl;
                return false;
            }
            if (personas.find(b) == personas.end() ){
                std::cout << "No existe el perfil" << std::endl;
                return false;
            }
            amigos[a].insert(b); 
            return true;
        }

        std::unordered_set<ID> followers(ID a) {
            std::unordered_set<ID> seguidores;
            for (const auto& [follower, followed] : amigos) {
                if (followed.find(a) != followed.end()) {
                    seguidores.insert(follower);
                }
            }
            return seguidores;
        }

        std::unordered_set<ID> const following(ID a){
            return amigos[a];
        }  

        std::unordered_set<ID> suggestions(ID a) {
            // Users already followed by a
            const std::unordered_set<ID>& alreadyFollowing = amigos[a];
            
            // Set to store suggestions
            std::unordered_set<ID> sugerencias;
            
            // For each person that a follows
            for (const auto& followedID : alreadyFollowing) {
                // Get who they follow
                const auto& theirFollowings = amigos[followedID];
                
                // Add their followings as suggestions
                for (const auto& suggestion : theirFollowings) {
                    // Don't suggest if a already follows them or if it's a itself
                    if (suggestion != a && alreadyFollowing.find(suggestion) == alreadyFollowing.end()) {
                        sugerencias.insert(suggestion);
                    }
                }
            }
            
            return sugerencias;
        }

        std::vector<ID> Bots(){
            std::vector<ID> bots;
            for (const auto& e : personas){
                if (followers(e.first).empty()){
                    bots.push_back(e.first);
                }
            }
            return bots;
        }

        std::vector<ID> EliminarBots(){
            std::vector<ID> removedBots;
            bool botsFound = true;
            
            while (botsFound) {
                std::vector<ID> bots = Bots();
                if (bots.empty()) {
                    botsFound = false;
                } else {
                    for (ID id : bots) {
                        amigos.erase(id);
                        personas.erase(id);
                        removedBots.push_back(id);
                    }
                }
            }
            
            return removedBots;
        }
    private :
        std::unordered_map<ID, std::string> personas;
        std::unordered_map<ID, std::unordered_set<ID>> amigos;

};


int main(){
    // Test the RedSocial class
    RedSocial<Persona> red;
    red.newProfile("Juan");
    red.newProfile("Maria");    
    red.newProfile("Pedro");
    red.newProfile("Ana");
    red.newProfile("Luis");

    red.wantToBeFollower(1, 2);
    red.wantToBeFollower(1, 3);
    red.wantToBeFollower(2, 4);
    red.wantToBeFollower(3, 5); 
    red.wantToBeFollower(4, 1);

    red.wantToBeFollower(5, 1);


    std::cout << "Sugerencias para Juan: ";
    auto sugerencias = red.suggestions(1);
    for (const auto& id : sugerencias) {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Bots en la red: ";
    auto bots = red.Bots();
    for (const auto& id : bots) {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    auto eliminados = red.EliminarBots();   
    for (const auto& id : eliminados) {
        std::cout << id << " ";
    }
    std::cout << "Eliminando bots..." << std::endl; 
    return 0;   
}