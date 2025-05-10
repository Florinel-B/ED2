#include <memory>
#include <stack>

#include <iostream>
#include <list>

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
    //auto n23 = t.addRight(33, h1);

    //root->info = 33;

    std::cout << "Es vacio: " << t.isEmpty() << "\n";

    printBinaryTree<LinkedBinaryTree<int>, WithoutSiblingIterator<LinkedBinaryTree<int>>>(t);
    std::cout << "Recorrido IN-orden: ";
    
    printBinaryTree<LinkedBinaryTree<int>, InOrderIterator<LinkedBinaryTree<int>>>(t);


    return 0;
}