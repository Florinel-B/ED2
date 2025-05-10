#include <memory>
#include <algorithm>

#include <iostream>
#include <cstdio>
#include <cmath>
#include <queue>

template <typename T>
class RBTree {
public:
    template <typename S>
    class Node {
        friend class RBTree;
       
        S info;
        Node<S>* parent;
        std::shared_ptr<Node<S>> left;
        std::shared_ptr<Node<S>> right;
        bool isRed;  // 1 - Red, 0 - Black
    };
    
    T        getElement(Node<T>* node)    const {return (node->info);}
    Node<T>* getRoot()                    const {return root.get();}
    Node<T>* getParent(Node<T>* node)     const {return (node->parent);}
    Node<T>* getLeftChild(Node<T>* node)  const {return (node->left.get());}
    Node<T>* getRightChild(Node<T>* node) const {return (node->right.get());}
    
    bool isEmpty()                       {return root == nullptr;}
    bool isRoot(Node<T>* node) const     {return (node == root.get());}
    bool isLeaf(Node<T>* node) const     {return ((node->left  == nullptr) 
                                               && (node->right == nullptr));}
    bool isInternal(Node<T>* node) const {return !isLeaf(node);}
    
    Node<T>* insert(T e) {
        auto node = std::make_shared<Node<T>>();
        node->info = e;
        node->parent = nullptr;
        node->left = nullptr;
        node->right = nullptr;
        node->isRed = 1;
        
        if (!root) {
            root = node;
			node->isRed = false;  // root always black
            return root.get();
        } else {
            auto n = insertBinary(node, root);
            checkDoubleRed(n);
            return node.get();
        }
    }
	
	void remove(T e) {
		auto n = removeBinary(e, root.get());
	}
    
    void print() const {
        if (root == nullptr) {
            std::cout << "Árbol vacío" << std::endl;
            return;
        }
    
        // Calcular la altura del árbol
        int height = getHeight();
    
        // Calcular el ancho del árbol
        int width = pow(2, height) - 1;
    
        // Crear una matriz para almacenar la representación del árbol
        std::vector<std::vector<std::string>> treeMatrix(height, std::vector<std::string>(width, " "));
    
        // Llenar la matriz con los valores de los nodos
        fillTreeMatrix(root.get(), treeMatrix, 0, 0, width);
    
        // Imprimir la matriz
        for (const auto& row : treeMatrix) {
            for (const auto& cell : row) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
    }

    
private:
    // Solo se usa para pintar
    int getHeight() const {
        return getHeight(root.get());
    }
    // Solo se usa para pintar
    int getHeight(Node<T>* n) const {
        if (n) {
            return 1 + std::max(getHeight(getLeftChild(n)), getHeight(getRightChild(n)));
        } else {
            return 0;
        }
    }

    void fillTreeMatrix(Node<T>* node, std::vector<std::vector<std::string>>& matrix, int row, int left, int right) const {
        if (node == nullptr) {
            return;
        }
    
        int mid = (left + right) / 2;
        matrix[row][mid] = std::to_string(node->info);
    
        if (node->left.get() == nullptr && node->right.get() == nullptr) {
            return; // No hay hijos, no necesitamos mostrar "-"
        }
    
        if (node->left.get() != nullptr) {
            fillTreeMatrix(node->left.get(), matrix, row + 1, left, mid - 1);
        } else {
            matrix[row + 1][mid - (right - mid) / 2] = "-"; // Mostrar "-" para hijo izquierdo nulo
        }
    
        if (node->right.get() != nullptr) {
            fillTreeMatrix(node->right.get(), matrix, row + 1, mid + 1, right);
        } else {
            matrix[row + 1][mid + (right - mid) / 2] = "-"; // Mostrar "-" para hijo derecho nulo
        }
    }

    Node<T>* insertBinary(std::shared_ptr<Node<T>> node, std::shared_ptr<Node<T>> parent) {
        if (node->info < parent->info) {
            if (parent->left) {
                return insertBinary(node, parent->left);
            } else {
                parent->left = node;
            }
        } else {
            if (parent->right) {
                return insertBinary(node, parent->right);
            } else {
                parent->right = node;
            }
        }
        node->parent = parent.get();
        return node.get();
    }
	
	Node<T>* removeOneChildCase(Node<T>* n, std::shared_ptr<Node<T>> child) {
	    Node<T>* sibling = nullptr;
		bool isDoubleBlack = false;
	    if (isRoot(n)) {
			root = child;
			if(root) root->isRed = false;
		} else if (n->parent->left.get() == n) {
			auto parent = n->parent;
			if(child) {
				isDoubleBlack = (!n->isRed) && (!child->isRed);
				child->parent = parent;
			}
			sibling = parent->right.get();
			parent->left = child;  // WARNING: from here on, n no longer exists
		} else {
			auto parent = n->parent;
			if(child) {
				isDoubleBlack = (!n->isRed) && (!child->isRed);
				child->parent = parent;
			}
			sibling = parent->left.get();
			parent->right = child;  // WARNING: from here on, n no longer exists
		}
		return (isDoubleBlack) ? sibling : nullptr;
	}
	
	Node<T>* removeBinary(T e, Node<T>* n) {
		if (n == nullptr) return nullptr;
		
		Node<T>* parent;
		if (e < n->info) {
			return removeBinary(e, n->left.get());
		} else if (e > n->info) {
			return removeBinary(e, n->right.get());
		} else {  // remove n
			if (n->left == nullptr) {
			    return removeOneChildCase(n, n->right);
			} else if (n->right == nullptr) {
				return removeOneChildCase(n, n->left);
			} else {  // two children
				auto succesor = min(n->right.get());
				n->info = succesor->info;
				return removeBinary(succesor->info, n->right.get());
			}
		}
	}
    
	Node<T>* min(Node<T>* n) {
		if (!n) return nullptr;
		while (n->left) {
			n = n->left.get();
		}
		return n;
	}

	bool isBlack(Node<T>* n) {
		return (!n) || (!n->isRed);  // !(n && n->isRed)
	}

	void checkDoubleRed(Node<T>*  z) {
		auto p = z->parent;
		if ((z->isRed) && (p) && (p->isRed)) {
			auto gp = p->parent;
			Node<T>* uncle = nullptr;
			if (gp->left.get() == p) {
				uncle = gp->right.get();
			} else {
				uncle = gp->left.get();
			}
			
			if (isBlack(uncle)) {  // caso 1
				auto mid = restructure(z, p, gp);
				mid->isRed = false;
				mid->left->isRed = true;
				mid->right->isRed = true;
			} else {  // caso 2
				p->isRed = false;
				uncle->isRed = false;
				if(!isRoot(gp)) {
					gp->isRed = true;
					checkDoubleRed(gp);
				}
			}
		}
	}
    
    /*
     *          z=c       z=c        z=a         z=a
     *         /  \      /  \       /  \        /  \
     *       y=b  t4   y=a  t4    t1  y=c     t1  y=b
     *      /  \      /  \           /  \         /  \
     *    x=a  t3    t1 x=b        x=b  t4       t2 x=c
     *   /  \          /  \       /  \             /  \
     *  t1  t2        t2  t3     t2  t3           t3  t4
    */
    
    Node<T>* restructure(Node<T>* node, Node<T>* parent, Node<T>* grandParent) {
        std::shared_ptr<Node<T>> lowKey, midKey, highKey, t1, t2, t3, t4;
        
        //std::cout << "--restructure" << std::endl;

        // Node<T>* parent = getParent(node);
        // Node<T>* grandParent = getParent(parent);

        Node<T>* gpParent = isRoot(grandParent) ? nullptr : getParent(grandParent);
        bool leftCase = gpParent == nullptr ? false : grandParent == getLeftChild(gpParent);
        
        bool nodeLeft = (getLeftChild(parent) == node);
        bool parentLeft = (getLeftChild(grandParent) == parent);

        if (nodeLeft && parentLeft) {  // Desequilibrio izda-izda
            lowKey = node->parent->left;
            midKey = parent->parent->left;
            if (!gpParent) {
                highKey = root;
            } else {
                highKey = leftCase? grandParent->parent->left : grandParent->parent->right;    
            }

            t1 = lowKey->left;
            t2 = lowKey->right;
            t3 = midKey->right;
            t4 = highKey->right;
        } else if (!nodeLeft && parentLeft) {  // Desequilibrio izda-dcha
            midKey = node->parent->right;
            lowKey = parent->parent->left;
            if (!gpParent) {
                highKey = root;
            } else {
                highKey = leftCase? grandParent->parent->left : grandParent->parent->right;    
            }

            t1 = lowKey->left;
            t2 = midKey->left;
            t3 = midKey->right;
            t4 = highKey->right;
        } else if (nodeLeft && !parentLeft) {  // Desequilibrio dcha-izda
            midKey = node->parent->left;
            highKey = parent->parent->right;
            if (!gpParent) {
                lowKey = root;
            } else {
                lowKey = leftCase? grandParent->parent->left : grandParent->parent->right;    
            }

            t1 = lowKey->left;
            t2 = midKey->left;
            t3 = midKey->right;
            t4 = highKey->right;
        } else {  // Desequilibrio dcha-dcha
            highKey = node->parent->right;
            midKey = parent->parent->right;
            if (!gpParent) {
                lowKey = root;
            } else {
                lowKey = leftCase? grandParent->parent->left : grandParent->parent->right;    
            }
            

            t1 = lowKey->left;
            t2 = midKey->left;
            t3 = highKey->left;
            t4 = highKey->right;
        }

        lowKey->left = t1;
        lowKey->right = t2;
        highKey->left = t3;
        highKey->right = t4;
                
        midKey->left = lowKey;
        midKey->right = highKey;
        lowKey->parent = midKey.get();
        highKey->parent = midKey.get();

        auto output = midKey.get();

        if (gpParent == nullptr) {
            root = midKey;
            midKey->parent = nullptr;
        } else { 
            if (leftCase) {
                gpParent->left = midKey;
            } else {
                gpParent->right = midKey;
            }
            midKey->parent = gpParent;
        }

        return output;
    }

    std::shared_ptr<Node<T>> root = nullptr;
};

void insertarNodo(auto& t, int n) {
    printf("Insertarndo %d\n", n);
    
    t.insert(n);
    
    t.print();
    printf("\n\n");
    
}

int main() {
    printf("Hola Mundo!\n");
    
    RBTree<int> t;
    
    t.print();
    
    insertarNodo(t, 10);
    insertarNodo(t, 12);
    insertarNodo(t, 14);
    insertarNodo(t, 9);
    insertarNodo(t, 11);
    insertarNodo(t, 13);
    insertarNodo(t, 19);
    insertarNodo(t, 16);
    insertarNodo(t, 17);
	
    printf("Borrando 13\n");
    
    t.remove(13);
    
    t.print();
    printf("\n\n");
	
    return 0;
}