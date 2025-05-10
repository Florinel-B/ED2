#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

struct Persona {
    std::string nombre;
    std::string apellidos;
    unsigned long long num_DNI;
    char letra_DNI;
    bool valid=0;
    
    bool operator==(const Persona& rhs) {
        return (nombre == rhs.nombre) && (apellidos == rhs.apellidos) && (num_DNI == rhs.num_DNI) && (letra_DNI == rhs.letra_DNI) && (valid == 1); 
    }
};

template <typename T, int N=100>
class ContenedorEficiente {
public:
    void add(Persona p) {
        Persona q = v[p.num_DNI % N];
        
        if (q.valid) {
            std::cout << "Ya existe una persona en esta posición.\n";
            collision++;
        } else {
            v[p.num_DNI % N] = p;
        }
    }
    
    bool contains(Persona p) {
        return p == v[p.num_DNI % N];
    }
    
    void remove(Persona p) {
        v[p.num_DNI % N] = {};
    }
    
    void printCollisions() {std::cout << "Colisiones: " << collision << "\n";}
    
private:
    std::vector<T> v{N};
    int collision{0};
};

std::string generateRandomName(std::vector<std::string> list) {

    //std::vector<std::string> nombres = {"Juan", "María", "Pedro", "Ana", "Luis", "Laura"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, list.size() - 1);

    return list[dist(gen)];
}

unsigned long long generateRandomNumber() {
    unsigned long long randomNumber = std::rand() % 100000000;

    return randomNumber;
}

char generateRandomLetter() {
    int randomIndex = std::rand() % 26;

    char randomLetter = 'a' + randomIndex;

    return randomLetter;
}


int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::cout<<"Hello World\n";

    std::vector<std::string> nombres = {"Juan", "María", "Pedro", "Ana", "Luis", "Laura"};
    std::vector<std::string> apellidos = {"Garcia", "Perez", "Gómez", "Sanchez", "Hernandez", "Fernandez"};

    const int EXP_NUM = 20;
    ContenedorEficiente<Persona> hash;

    for(int i = 0; i < EXP_NUM; ++i) {
        Persona p{generateRandomName(nombres), generateRandomName(apellidos), generateRandomNumber(), generateRandomLetter(), 1};
        hash.add(p);
    }

    hash.printCollisions();

    return 0;
}