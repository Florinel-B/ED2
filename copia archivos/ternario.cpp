#include <iostream>
#include <memory>
#include <iterator>
#include <stack>
#include <queue>
#include <list>



template <typename T>
class IteradorTreeHijos{
    using Tipo = typename T::Tipo;
    using Nodo = typename T::Node;
    public:
        IteradorTreeHijos() : tree{ } {}
        IteradorTreeHijos(T& arbol) : tree{arbol} {
            auto root = arbol.getRoot();
            if (!root){
                auto left = arbol.ObtainLeft(root);
                if (left) pila.push(left);
                auto right = arbol.ObtainRight(root);
                if (right) pila.push(right);
                auto middle = arbol.ObtainMiddle(root);
                if (middle) pila.push(middle);
            }
        }
        IteradorTreeHijos operator ++() {
            if(!pila.empty()){  
                auto n = pila.top();
                pila.pop();

                auto left = tree.ObtainLeft(n);
                if (left) pila.push(left);
                auto right = tree.ObtainRight(n);
                if (right) pila.push(right);
                auto middle = tree.ObtainMiddle(n);
                if (middle) pila.push(middle);

            }
            return *this;

        }
        IteradorTreeHijos operator ++(int) {
            auto tmp(*this);
            operator++();
            return tmp;
        }

        Tipo operator*() { return tree.getElement(pila.top()); }
        bool operator!=(const IteradorTreeHijos& other) const { return !(pila.empty() && other.pila.empty()); }

        private :
        T tree;
        std::stack<Nodo*> pila;

        bool CompareChildren(Nodo* nodo){
            if (tree.ObtainLeft(nodo) != nullptr) return false;
            if (tree.ObtainRight(nodo) != nullptr) return false;
            if (tree.ObtainMiddle(nodo) != nullptr) return false;
            return true;
        }

};


template <typename T>

class IteradorTreeProfundo {
    using Tipo = typename T::Tipo;
    using Nodo = typename T::Node;
    public:
        IteradorTreeProfundo() : tree{} {}
        IteradorTreeProfundo(T& arbol) : tree(arbol) {
            auto root = arbol.getRoot();
            pila.push(root);
        }
        IteradorTreeProfundo& operator++() {
            if(!pila.empty()){
                auto n = pila.top();
                pila.pop();

                auto i = tree.ObtainRight(n);
                if (i) pila.push(i);
                auto m = tree.ObtainMiddle(n);
                if (m) pila.push(m);
                auto r = tree.ObtainLeft(n);
                if (r) pila.push(r);
            }
            return *this;
        }
        auto operator++(int) {
            auto tmp(*this);
            operator++();
            return tmp;
        }

        Tipo operator*() { return tree.getElement(pila.top()); }

        bool operator!=(const IteradorTreeProfundo& other) const { return !(pila.empty() && other.pila.empty()); }
    private:
        T tree;
        std::stack<Nodo*> pila;
    
};


template <typename T>

class IteradorTreeNivel {
    using Tipo = typename T::Tipo;
    using Nodo = typename T::Node;
    public:
        IteradorTreeNivel() : tree{} {}
        IteradorTreeNivel(T& arbol) : tree(arbol) {
            auto root = arbol.getRoot();
            pila.push(root);
        }
        IteradorTreeNivel& operator++() {
            if(!pila.empty()){
                auto n = pila.front();
                pila.pop();

                auto r = tree.ObtainLeft(n);
                if (r) pila.push(r);
                
                auto m = tree.ObtainMiddle(n);
                if (m) pila.push(m);
                
                auto i = tree.ObtainRight(n);
                if (i) pila.push(i);
            }
            return *this;
        }
        auto operator++(int) {
            auto tmp(*this);
            operator++();
            return tmp;
        }

        Tipo operator*() { return tree.getElement(pila.front()); }

        bool operator!=(const IteradorTreeNivel& other) const { return !(pila.empty() && other.pila.empty()); }
    private:
        T tree;
        std::queue<Nodo*> pila;
    
};


template <typename T, typename I>
class Iterable {
public:
    Iterable(T begin) : b(I(begin)), e(I()) {}

    auto begin() { return b; }

    auto end() { return e; }

private:
    I b;
    I e;
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
        bool remove(Node* nodo) {
            if (!nodo) return false;
            
            Iterable<TernaryTree<T>, IteradorTreeNivel<TernaryTree<T>>> iterable{*this};
            for (const auto& i : iterable) {
                if (i == nodo->info) {
                    auto parent = nodo->parent;
                    if (!parent) {
                        // Handling root node removal needs special care
                        return false; // or implement root removal logic
                    }
                    
                    // Check which child the node is
                    bool isLeftChild = parent->leftChild.get() == nodo;
                    bool isRightChild = parent->RightChild.get() == nodo;
                    bool isMiddleChild = parent->MiddleChild.get() == nodo;
                    
                    // Create copies of the node's children
                    auto leftChild = nodo->leftChild;
                    auto rightChild = nodo->RightChild;
                    auto middleChild = nodo->MiddleChild;
                    
                    // Detach the node from its parent
                    if (isLeftChild) {
                        parent->leftChild = nullptr;
                        // Re-attach children if needed
                        if (leftChild) {
                            parent->leftChild = leftChild;
                            leftChild->parent = parent;
                        }
                    } else if (isRightChild) {
                        parent->RightChild = nullptr;
                        // Re-attach children if needed
                        if (rightChild) {
                            parent->RightChild = rightChild;
                            rightChild->parent = parent;
                        }
                    } else if (isMiddleChild) {
                        parent->MiddleChild = nullptr;
                        // Re-attach children if needed
                        if (middleChild) {
                            parent->MiddleChild = middleChild;
                            middleChild->parent = parent;
                        }
                    }
                    
                    return true;
                }
            }
            return false;
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





template <typename T, typename I>
void printBinaryTree(T t) {
    std::cout << "\n\n\nImprimir arbol binario\n\n";
    Iterable<T, I> iterable{t};

    for(const auto& e: iterable) {
        std::cout << e << " ";
    }
    std::cout << " fin\n\n\n";

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
    bool sol = tree.remove(middleChild);
    std::cout << "Eliminado: " <<  sol  << "\n";


    printBinaryTree<TernaryTree<int>, IteradorTreeNivel<TernaryTree<int>>>(tree);
    std::cout << "Recorrido izq-med-der: ";
    
    return 0;
}