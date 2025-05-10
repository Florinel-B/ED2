#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class LinkedTree {
public:
    template <typename S>
    class Node {
        friend class LinkedTree;
        
        S info;
        Node<S>* parent;
        std::vector<std::shared_ptr<Node<S>>> children;
        bool operator==(Node<S> rhs) {
            return (info == rhs.info) && (parent == rhs.parent) && (children == rhs.children);
        }
	public:
		Node() : info(), parent(nullptr), children() {} 
		Node(S e) : info(e), parent(nullptr), children() {} 
		Node(S e, Node<S>* parent) : info(e), parent(parent), children() {} 
    };

    LinkedTree() : root(nullptr) {}
    LinkedTree(std::shared_ptr<Node<T>> node) : root(node) {}

    bool isEmpty() {return root == nullptr;}
    
    Node<T>* getRoot() {return root.get();}
    T getElement(Node<T>* node) {return node->info;}
    Node<T>* getParent(Node<T>* node) {return node->parent;}
    
    bool isRoot(Node<T>* node) {return node == root.get();}
    bool isLeaf(Node<T>* node) {return node->children.size() == 0;}
    bool isInternal(Node<T>* node) {return !isLeaf(node);}
    
    Node<T>* add(T e, Node<T>* parent = nullptr) {
        if ((parent == nullptr) && (root != nullptr)) {
            std::cout << "Error, root no vacío.\n";
        } else if (parent == nullptr) {
            root = std::make_shared<Node<T>>(e);
            return root.get();
        } else {
            auto aux = std::make_shared<Node<T>>(e, parent);
            parent->children.push_back(aux);
            return aux.get();
        }
        return nullptr;
    }
    
    std::vector<std::shared_ptr<Node<T>>> getChildren(Node<T>* node) { return node->children;}
    
    LinkedTree<T> cut(Node<T>* node) {
        auto parent = getParent(node);
        std::shared_ptr<Node<T>> shared_node;
        for (auto it = parent->children.begin(); it != parent->children.end();) {
            shared_node = *it;
            if (shared_node.get()  == node) {
                parent->children.erase(it);
                break;
            } else {
                ++it;
            }
        }
        node->parent = nullptr;
        
        return LinkedTree(shared_node);
    }
    
    void attach(Node<T>* node, LinkedTree<T> subt) {
        auto subroot = subt.root;
        subroot->parent = node;
        node->children.push_front(subroot);
    }
    
private:
    std::shared_ptr<Node<T>> root=nullptr;
};

template <typename T>
class LCRSTree {
public:
	template <typename S>
	class Node {
		friend LCRSTree;
		
		S info;
		Node<S>* parent;
		std::shared_ptr<Node<S>> leftChild;
		std::shared_ptr<Node<S>> rightSibling;
	public:
		Node() : info(), parent(nullptr), leftChild(nullptr), rightSibling(nullptr) {}
		Node(S e) : info(e), parent(nullptr), leftChild(nullptr), rightSibling(nullptr) {}
		Node(S e, Node<S>* parent) : info(e), parent(parent), leftChild(nullptr), rightSibling(nullptr) {}
	};

    class ChildIterator {
    public:
        ChildIterator(std::shared_ptr<Node<T>> node) : it(node.get()) {}
        ChildIterator(Node<T>* node) : it(node) {}
    
        ChildIterator& operator++() {
            it = it->rightSibling.get();
            return *this;
        }
        
        ChildIterator operator++(int) {
            auto tmp(*this);
            operator++();
            return tmp;
        }
    
        auto operator*() {
            return it;
        }
    
        bool operator!=(const ChildIterator& other) {
            return it != other.it;
        }
    
    private:
        Node<T>* it;
    };

    template <typename S>
    class ChildIterable {
    public:
        ChildIterable(Node<S>* node) : node(node) {}
        ChildIterable(std::shared_ptr<Node<S>> node) : node(node.get()) {}
    
        auto begin() { return ChildIterator(node); }
    
        auto end() { return ChildIterator(nullptr); }
    
    private:
        Node<S>* node=nullptr;
    };

    LCRSTree() : root(nullptr) {}
    LCRSTree(std::shared_ptr<Node<T>> node) : root(node) {}

    bool isEmpty() {return root == nullptr;}
    
    Node<T>* getRoot() {return root.get();}
    T getElement(Node<T>* node) {return node->info;}
    Node<T>* getParent(Node<T>* node) {return node->parent;}
    
    bool isRoot(Node<T>* node) {return node == root.get();}
    bool isLeaf(Node<T>* node) {return node->leftChild == nullptr;}
    bool isInternal(Node<T>* node) {return !isLeaf(node);}
    
    Node<T>* add(T e, Node<T>* parent = nullptr) {
        if ((parent == nullptr) && (root != nullptr)) {
            std::cout << "Error, root no vacío.\n";
        } else if (parent == nullptr) {
            root = std::make_shared<Node<T>>(e);
            return root.get();
        } else {
            auto aux = std::make_shared<Node<T>>(e, parent);
			auto leftchild = parent->leftChild;
			if(leftchild == nullptr) {
				parent->leftChild = aux;
			} else {
				aux->rightSibling = leftchild;
				parent->leftChild = aux;
			}
            return aux.get();
        }
        return nullptr;
    }

    ChildIterable<T> getChildren(Node<T>* node) { return ChildIterable(node->leftChild);}
   
    LCRSTree<T> cut(Node<T>* node) {
        auto parent = getParent(node);
        std::shared_ptr<Node<T>> shared_node;
        if (parent->leftChild.get() == node) {
            shared_node = parent->leftChild;
            parent->leftChild = parent->leftChild->rightSibling;
        } else {
            auto aux = parent->leftChild;
            while (aux->rightSibling.get() != node) {  // never nullptr
                aux = aux->rightSibling;
            }
            shared_node = aux->rightSibling;
            aux->rightSibling = aux->rightSibling->rightSibling;
        }

        node->parent = nullptr;
        node->rightSibling = nullptr;
        return LCRSTree<T>(shared_node);
    }
    
    void attach(Node<T>* node, LCRSTree<T> subt) {
		auto subroot = subt.root;
		subroot->parent = node;
		subroot->rightSibling = node->leftChild;
		node->leftChild = subroot;
    }
    
private:
    std::shared_ptr<Node<T>> root=nullptr;
};

template <typename T>
void printTree(T t) {
    std::cout << "\n\n\nImprimir arbol\n\n";
    std::cout << "Raiz: " << t.getElement(t.getRoot()) << "\n";
    
    auto hs = t.getChildren(t.getRoot());
    
    std::cout << "hijos: ";
    for(const auto& h: hs) {
        std::cout << t.getElement(h) << " ";
    }
    std::cout << "\n";

    for(const auto& h: hs) {
        auto ns = t.getChildren(h);
    
        std::cout << "nietos: ";
        for(const auto& n: ns) {
            std::cout << t.getElement(n) << " ";
        }
        std::cout << " | ";
    }
    std::cout << "\n\n\n";

}

int main() {
    std::cout<<"Hello World";
    
    //LinkedTree<int> t;
    LCRSTree<int> t;
    
    std::cout << "Es vacio: " << t.isEmpty() << "\n";
    
    auto root = t.add(42);
    auto h1 = t.add(37, root);
    auto h2 = t.add(3, root);
    auto n11 = t.add(17, h1);
    auto n21 = t.add(9, h2);
    auto n22 = t.add(27, h2);
    auto n23 = t.add(81, h2);
    
    std::cout << "Es vacio: " << t.isEmpty() << "\n";
    
    printTree(t);
    
    std::cout << "¿Es root Root?: " << t.isRoot(root) << "\n";
    std::cout << "¿Es h1 Root?: " << t.isRoot(h1) << "\n";
    std::cout << "¿Es n11 Root?: " << t.isRoot(n11) << "\n";
    std::cout << "¿Es hoja root?: " << t.isLeaf(root) << "\n";
    std::cout << "¿Es hoja h2?: " << t.isLeaf(h2) << "\n";
    std::cout << "¿Es hoja n22?: " << t.isLeaf(n22) << "\n";
    std::cout << "¿Es interno Root?: " << t.isInternal(root) << "\n";
    std::cout << "¿Es interno n23?: " << t.isInternal(n23) << "\n";
    std::cout << "¿Es root el padre de h1?: " << (root == t.getParent(h1)) << "\n";
    std::cout << "¿Es root el padre de n11?: " << (root == t.getParent(n11)) << "\n";
    std::cout << "¿Es h2 el padre de n21?: " << (h2 == t.getParent(n21)) << "\n";
    std::cout << "¿Es h2 el padre de n11?: " << (h2 == t.getParent(n11)) << "\n";
    std::cout << "¿Es root el abuelo de h2?: " << (root == t.getParent(t.getParent(h2))) << "\n";
    std::cout << "¿Es root el abuelo de n11?: " << (root == t.getParent(t.getParent(n11))) << "\n";

    auto tcut = t.cut(h1);

    printTree(t);
    
    printTree(tcut);
    
    t.attach(root, tcut);

    printTree(t);

    return 0;
}