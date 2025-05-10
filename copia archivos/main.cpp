#include <memory>
#include <stack>
#include <queue>
#include <iostream>
#include <list>
#include <unordered_set>

template <typename T>
class LinkedBinaryTree {
public:
    using type = T;
    template <typename S>
    class Node {
        friend class LinkedBinaryTree;
        S info;
        Node<S>* parent;
        std::shared_ptr<Node<S>> left;
        std::shared_ptr<Node<S>> right;
    };

    T getElement(Node<T>* node) const {return (node->info);}
    Node<T>* getRoot()                   const {return root.get();}
    Node<T>* getParent(Node<T>* node)     const {return (node->parent);}
    Node<T>* getLeftChild(Node<T>* node) const {return (node->left.get());}
    Node<T>* getRightChild(Node<T>* node) const {return (node->right.get());}

    bool isEmpty() {return root == nullptr;}
    bool isRoot(Node<T>* node) const {return (node == root.get());}
    bool isLeaf(Node<T>* node) const {return ((node->left == nullptr) && (node->right == nullptr));}
    bool isInternal(Node<T>* node) const {return !isLeaf(node);}

    Node<T>* addRoot(T e) {
        root = std::make_shared<Node<T>>();
        root->info = e;
        root->parent = nullptr;
        root->left = nullptr;
        root->right = nullptr;
        return (root.get());
    }
    Node<T>* addLeft(T e, Node<T>* parent) {
        parent->left = std::make_shared<Node<T>>();
        parent->left->info = e;
        parent->left->parent = parent;
        parent->left->left = nullptr;
        parent->left->right = nullptr;
        return (parent->left.get());
    }
    Node<T>* addRight(T e, Node<T>* parent) {
        parent->right = std::make_shared<Node<T>>();
        parent->right->info = e;
        parent->right->parent = parent;
        parent->right->left = nullptr;
        parent->right->right = nullptr;
        return (parent->right.get());
    }

private:
    std::shared_ptr<Node<T>> root = nullptr;
};

template <typename T>
class WithoutSiblingIterator {
public:

    using Type = typename T::type;
    using Node = typename T::Node<Type>;

    WithoutSiblingIterator() : t{}, l{} {}

    WithoutSiblingIterator(T& tree) : t(tree) {
        auto root = t.getRoot();
        l.push_front(root);
    }

    WithoutSiblingIterator& operator++() {
        while(!l.empty()){
            Node* n = l.front();
            l.pop_front();
            auto i = t.getLeftChild(n);
            if (i) l.push_front(i);

            auto r = t.getRightChild(n);
            if (r) l.push_front(r);

            if(!l.empty() && !(hasSibling(l.front()))){
                break;
            }
        }
        return *this;
    }

    auto operator++(int) {
        auto tmp(*this);
        operator++();
        return tmp;
    }

    Type operator*() const {
        return t.getElement(l.front());
    }

    bool operator!=(const WithoutSiblingIterator& other) const {
        return (l != other.l) ;
    }
private:
    T t;
    std::list<typename T::Node<typename T::type>*> l; // Puede ser std::list<Node*> l;

    bool hasSibling(Node* n){
        auto parent = t.getParent(n);
        return (t.getLeftChild(parent) && t.getRightChild(parent));
    }

};


// Iterador para recorrido en inorden (izquierda, raíz, derecha)
template <typename T>
class InOrderIterator {
public:
    using Type = typename T::type;
    using Node = typename T::Node<Type>;

    InOrderIterator() : t{} {}

    InOrderIterator(T& tree) : t(tree) {
        auto root = t.getRoot();
        if (root) {
            pushAllLeftChildren(root);
        }
    }

    InOrderIterator& operator++() {
        if (!s.empty()) {
            Node* current = s.top();
            s.pop();

            // Procesar hijo derecho
            auto right = t.getRightChild(current);
            if (right) {
                pushAllLeftChildren(right);
            }
        }
        return *this;
    }
    auto operator++(int) {
        auto tmp(*this);
        operator++();
        return tmp;
    }

    Type operator*() const {
        return t.getElement(s.top());
    }

    bool operator!=(const InOrderIterator& other) const {
        return !(s.empty() && other.s.empty());
    }

private:
    T t;
    std::stack<Node*> s;

    void pushAllLeftChildren(Node* node) {
        while (node != nullptr) {
            s.push(node);
            node = t.getLeftChild(node);
        }
    }
};


// Iterador para recorrido en postorden (izquierda, derecha, raíz)

// Iterador para recorrido en postorden usando una cola
template <typename T>
class PostOrderQueueIterator {
public:
    using Type = typename T::type;
    using Node = typename T::Node<Type>;

    PostOrderQueueIterator() : t{} {}

    PostOrderQueueIterator(T& tree) : t(tree) {
        if (!tree.getRoot()) return;
        
        // Necesitamos construir el recorrido completo al inicio
        std::queue<Node*> construction;
        
        // Paso 1: Construir un recorrido postorden completo
        collectPostOrderNodes(tree.getRoot(), construction);
        
        // Paso 2: Transferir a nuestra cola final
        while (!construction.empty()) {
            q.push(construction.front());
            construction.pop();
        }
    }

    PostOrderQueueIterator& operator++() {
        if (!q.empty()) {
            q.pop();
        }
        return *this;
    }

    auto operator++(int) {
        auto tmp(*this);
        operator++();
        return tmp;
    }

    Type operator*() const {
        return t.getElement(q.front());
    }

    bool operator!=(const PostOrderQueueIterator& other) const {
        return !(q.empty() && other.q.empty());
    }

private:
    T t;
    std::queue<Node*> q;
    
    // Función recursiva para construir el recorrido postorden
    void collectPostOrderNodes(Node* node, std::queue<Node*>& result) {
        if (!node) return;
        
        // Recorrer izquierdo
        collectPostOrderNodes(t.getLeftChild(node), result);
        
        // Recorrer derecho
        collectPostOrderNodes(t.getRightChild(node), result);
        
        // Procesar nodo actual
        result.push(node);
    };
};


// Iterador para recorrido en preorden (raíz, izquierda, derecha)
template <typename T>
class PreOrderIterator{
    using Type = typename T::type;
    using Node = typename T::Node<Type>;
    public :
    PreOrderIterator() : tree{ } {}
    PreOrderIterator(T arbol) : tree(arbol) {
        auto root = arbol.getRoot();
        SclavoNegro.push(root);
    }
    PreOrderIterator& operator++(){
        if (!SclavoNegro.empty()){
            auto n = SclavoNegro.top();
            SclavoNegro.pop();
            auto r = tree.getRightChild(n);
            if (r) SclavoNegro.push(r);
            auto l = tree.getLeftChild(n);
            if (l) SclavoNegro.push(l);
            
        }
        return *this;
    }
    
    auto operator++(int){
        auto tmp(*this);
        operator++();
        return tmp;
    }




    Type operator* () {return tree.getElement(SclavoNegro.top());}    

    bool operator != (const PreOrderIterator other) const { return !(SclavoNegro.empty() && other.SclavoNegro.empty()); }


    
    
    
    private:
    T tree;
    std::stack<Node*> SclavoNegro;

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

template <typename T, typename I>
void printBinaryTree(T t) {
    std::cout << "\n\n\nImprimir arbol binario\n\n";
    Iterable<T, I> iterable{t};

    for(const auto& e: iterable) {
        std::cout << e << " ";
    }
    std::cout << " fin\n\n\n";

}


//copy a complete binary tree and substitute the root by a common ancestor of two nodes
template <typename T>
class CommonAncestyTree {
    using Type = typename T::type;
    using Node = typename T::Node<Type>;
public:
    CommonAncestyTree() : t{} {}
    
    CommonAncestyTree(T& tree) : t(tree) {}
    

    T createCommonAncestorTree(Node* n1, Node* n2) {
        auto root = commonAncestor(n1, n2);
        auto value = t.getElement(root);
        
        T newTree{};
        auto newRoot = newTree.addRoot(value);
        
        // Creamos una copia del subárbol desde el ancestro común
        copyTreeStructure(t.getRoot(), newRoot, newTree);
        
        return newTree;
    }

    Node* commonAncestor(Node* n1, Node* n2){
        auto parent1 = t.getParent(n1);
        auto parent2 = t.getParent(n2);
        while (parent1 != parent2 && parent2 != t.getRoot()){
            parent2 = t.getParent(parent2);
            if (parent1 != t.getRoot()){
                parent1 = t.getParent(parent1);
            }
        }
        return parent2;
    }

    private:
    T t;

    void copyTreeStructure(Node* srcNode, Node* destNode, T& destTree) {
        if (!srcNode) return;
        
        // Procesa hijo izquierdo
        Node* leftChild = t.getLeftChild(srcNode);
        if (leftChild) {
            // Crea un nuevo nodo en el árbol destino
            Node* newLeftChild = destTree.addLeft(t.getElement(leftChild), destNode);
            // Copia recursivamente el subárbol izquierdo
            copyTreeStructure(leftChild, newLeftChild, destTree);
        }
        
        // Procesa hijo derecho
        Node* rightChild = t.getRightChild(srcNode);
        if (rightChild) {
            // Crea un nuevo nodo en el árbol destino
            Node* newRightChild = destTree.addRight(t.getElement(rightChild), destNode);
            // Copia recursivamente el subárbol derecho
            copyTreeStructure(rightChild, newRightChild, destTree);
        }
    }

};

int main()
{
    std::cout<<"Hello World\n";

    LinkedBinaryTree<int> t;

    std::cout << "Es vacio: " << t.isEmpty() << "\n";

    auto root = t.addRoot(42);
    auto h1 = t.addLeft(37, root);
    auto h2 = t.addRight(3, root);
    auto n11 = t.addLeft(17, h1);
    auto n21 = t.addLeft(9, h2);
    auto n22 = t.addRight(27, h2);
    auto h12 = t.addRight(4, h1);
    auto h16 = t.addLeft(5, h12);

    //auto n23 = t.addRight(33, h1);

    //root->info = 33;

    std::cout << "Es vacio: " << t.isEmpty() << "\n";

    //printBinaryTree<LinkedBinaryTree<int>, WithoutSiblingIterator<LinkedBinaryTree<int>>>(t);
    std::cout << "Recorrido IN-orden: ";
    
    //printBinaryTree<LinkedBinaryTree<int>, InOrderIterator<LinkedBinaryTree<int>>>(t);
    CommonAncestyTree<LinkedBinaryTree<int>> c(t);
    auto newTree = c.createCommonAncestorTree(n21, n22);
    printBinaryTree<LinkedBinaryTree<int>, PreOrderIterator<LinkedBinaryTree<int>>>(newTree);

    printBinaryTree<LinkedBinaryTree<int>, PreOrderIterator<LinkedBinaryTree<int>>>(t);


    return 0;
}