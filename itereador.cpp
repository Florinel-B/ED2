/* Implemente un iterador por niveles, LevelIterator, que recibe como parámetros el árbol y el
nivel del árbol que se desea recorrer. En caso de instanciar el iterador con un árbol nulo, un árbol
vacío o con un nivel negativo se lanzará una excepción. Si el árbol no tiene suficiente profundidad
el iterador estará vacío.
En la figura puede verse un árbol y marcado con líneas los diferentes niveles, de tal forma que si se
instancia el LevelIterator sobre dicho árbol y le pedimos el nivel 2, recorreremos los nodos E,
F, G, H, I y J. */ 

#include <iostream>
#include <queue>
#include <iterator> 
#include <stack>
#include <list>
#include <memory>
#include <stdexcept>
#include <exception>





template <typename T, typename I>
class Iterable {
public:
    Iterable(T begin, int level) : b(I(begin, level)), e(I()) {}

    auto begin() { return b; }

    auto end() { return e; }

private:
    I b;
    I e;
};

// raiz izquierda derecha

template <typename T>
class PreOrderIterator{
    using Type = typename T::Tipo;
    using Node = typename T::Node;
    public :
    PreOrderIterator() : tree{ } {}
    PreOrderIterator(T arbol) : tree(arbol) {
        auto root = arbol.GetRoot();
        SclavoNegro.push(root);
    }
    PreOrderIterator& operator++(){
        if (!SclavoNegro.empty()){
            auto n = SclavoNegro.top();
            SclavoNegro.pop();
            n = tree.ObtainLeft(n);
            SclavoNegro.push(n);
        }
        return *this;
    }
    



    auto operator++(int){
        auto tmp(*this);
        operator++();
        return tmp;
    }


    Type operator* () {return tree.getElement(SclavoNegro.front());}    

    bool operator != (const PreOrderIterator other) const { return !(SclavoNegro.empty() && other.SclavoNegro.empty()); }


    
    
    
    private:
    T tree;
    std::stack<Node*>  SclavoNegro;

};

template <typename T>
class LevelIterator{

    using Type = typename T::Tipo;
    using Node = typename T::Node;
public:
    LevelIterator() : tree{ } {}
    LevelIterator(T arbol, int nivel) : tree(arbol) {
        auto root = arbol.getRoot();
        auto accLast = root; 
        queue.push(root);
        if (root == nullptr){
            throw std::invalid_argument("El arbol es nulo");
        }
        int depth = 0;
        while (depth < nivel && !queue.empty()){
            auto elemacc = queue.front();
            queue.pop();
            auto listaHijos = tree.getChildren(elemacc); //getChildren devuelve los hijos directos del nodo y no depende de izq der si no que devuelve una lista de hijos
            for (auto& i : listaHijos){
                queue.push(i);
            }
            if (elemacc == accLast){
                depth++;
                accLast = queue.back();
            }
        }

    }
    LevelIterator& operator++(){
        if (queue.empty()){
            throw std::out_of_range("No hay mas elementos en el iterador");
        }
        else
        {
            auto n = queue.front();
            queue.pop();
        }
        return *this;
        
        
    }

    auto operator ++(int){
        auto tmp(*this);
        operator++();
        return tmp;
    }

    Type operator* () {return tree.getElement(queue.front());}    

    bool operator != (const LevelIterator& other) const { return !(queue.empty() && other.queue.empty()); }


    private:

    T tree;
    std::queue<Node*> queue;


};


template <typename T, typename I>
void printBinaryTree(T t) {
    std::cout << "\n\n\nImprimir arbol binario\n\n";
    Iterable<T, I> iterable{t , 0};

    for(const auto& e: iterable) {
        std::cout << e << " ";
    }
    std::cout << " fin\n\n\n";

};



template <typename T>
class TernaryTree {
    public:
        template <typename S>
        class TernaryNode {
            friend class TernaryTree;
            S info;
            std::shared_ptr<TernaryNode<S>> leftChild = nullptr;
            std::shared_ptr<TernaryNode<S>> RightChild = nullptr;
            std::shared_ptr<TernaryNode<S>> MiddleChild = nullptr;
            TernaryNode<S>* parent = nullptr;

            public:
                TernaryNode() : info(), leftChild(nullptr), RightChild(nullptr), MiddleChild(nullptr) {}
                TernaryNode(S e) : info(e), leftChild(nullptr), RightChild(nullptr), MiddleChild(nullptr) {}
                TernaryNode(S e, std::shared_ptr<TernaryNode<S>> left, std::shared_ptr<TernaryNode<S>> right, std::shared_ptr<TernaryNode<S>> middle) 
                    : info(e), leftChild(left), RightChild(right), MiddleChild(middle) {} 
        };
        using Tipo = T;
        using Node = TernaryNode<T>;

        T getElement(Node* nodo) const {
            return nodo->info;
        }
        std::list<Node*> getChildren(Node* nodo) {
            std::list<Node*> children;
            if (nodo->leftChild) children.push_back(nodo->leftChild.get());
            if (nodo->RightChild) children.push_back(nodo->RightChild.get());
            if (nodo->MiddleChild) children.push_back(nodo->MiddleChild.get());
            return children;
        }

        Node* ObtainLeft(Node* nodo) {
            return nodo->leftChild.get();
        }
        Node* ObtainRight(Node* nodo) {
            return nodo->RightChild.get();
        }
        Node* ObtainMiddle(Node* nodo) {
            return nodo->MiddleChild.get();
        }
        Node* ObtainParent(Node* nodo) {
            return nodo->parent;
        }
        Node* getRoot() const {
            return root.get();
        }
        bool hasMiddle() const {
            return root->MiddleChild != nullptr;
        }
        bool isInternal(Node* nodo ) {
            return nodo->leftChild != nullptr  || nodo->RightChild != nullptr  || nodo->MiddleChild != nullptr ;
            // si es un array podremos verifiar si la lista de hijos es diferente de null
        }
        bool isLeaf(Node* nodo) {
            return !isInternal(nodo);
        }
    
        int siblingCount(Node* nodo) {
            if (!nodo || !nodo->parent) return 0;
            
            auto parent = nodo->parent;
            int count = 0;
            
            if (parent->leftChild && parent->leftChild.get() != nodo) count++;
            if (parent->RightChild && parent->RightChild.get() != nodo) count++;
            if (parent->MiddleChild && parent->MiddleChild.get() != nodo) count++;
            
            return count;
        }

        Node* insertNodoLeft(T info, Node* nodo) {
            if (!root) {
                root = std::make_shared<Node>(info);
                return root.get();
            }
            if (nodo) {
                auto left =  std::make_shared<Node>(info);
                nodo->leftChild = left;
                nodo->leftChild->parent = nodo;
                return left.get();
            } else {
                std::cout << "Error, nodo no existe.\n";
                return nullptr;

            }
            
        }
        Node* insertNodoRight(T info, Node* nodo) {
            if (!root) {
                root = std::make_shared<Node>(info);
                return root.get();
            }
            if (nodo) {
                auto right =  std::make_shared<Node>(info);
                nodo->RightChild = right;
                nodo->RightChild->parent = nodo;
                return right.get();
            } else {
                std::cout << "Error, nodo no existe.\n";
                return nullptr;
            }
           
        }

        Node* insertNodoMiddle(T info, Node* nodo) {
            if (!root) {
                root = std::make_shared<Node>(info);
                return root.get();
            }
            if (nodo) {
                auto middle = std::make_shared<Node>(info);
                nodo->MiddleChild = middle;
                nodo->MiddleChild->parent = nodo;
                return middle.get();
            } else {
                std::cout << "Error, nodo no existe.\n";
                return nullptr;
            }
           
        }
        void imprimeArbol() {
            std::cout << "\n\n\nImprimir arbol\n\n";
            std::cout << "Raiz: " << root->info << "\n";
            auto left = ObtainLeft(root.get());
            auto right = ObtainRight(root.get());
            auto middle = ObtainMiddle(root.get());
            std::cout << "hijos: ";
            if (left) std::cout << left->info << " ";
            if (right) std::cout << right->info << " ";
            if (middle) std::cout << middle->info << " ";
            std::cout << "\n";
        }
    private:
        std::shared_ptr<TernaryNode<T>> root=nullptr;
};



int main() {
    TernaryTree<int> tree;
    // Example usage of the TernaryTree class
    auto root = tree.insertNodoLeft(1, nullptr);
    auto leftChild = tree.insertNodoLeft(2, root);
    auto ramon = tree.insertNodoLeft(21, leftChild);
    auto ramon1 = tree.insertNodoLeft(22, ramon);
    auto ramon2 = tree.insertNodoLeft(28, ramon1);
    auto rightChild = tree.insertNodoRight(7, root);
    auto middleChild = tree.insertNodoMiddle(13, root);
    auto juan = tree.insertNodoMiddle(12, leftChild);


    printBinaryTree<TernaryTree<int>, LevelIterator<TernaryTree<int>>>(tree);
    std::cout << "Recorrido izq-med-der: ";
    
    return 0;
}