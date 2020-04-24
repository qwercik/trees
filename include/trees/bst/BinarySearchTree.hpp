#pragma once

#include <list>
#include <stack>
#include <cmath>
#include <trees/bst/BinarySearchTreeNode.hpp>
#include <trees/bst/exceptions.hpp>

template <typename T>
class BinarySearchTree {
public:
    BinarySearchTree() {
    }

    ~BinarySearchTree() {
        // Usunięte wyłącznie do benchmarków, nie jest to potrzebne w tym momencie
        // A znacznie skraca proces testowania
        // Pamięć zaalokowana dynamicznie i tak zostanie wyczyszczona przez system operacyjny
        // gdy proces zakończy działanie
        //this->clearPostOrder();
    }

    // Wstawia element do drzewa
    void insert(T value) {
        if (this->root == nullptr) {
            this->root = new BinarySearchTreeNode<T>(value);
            return;
        } else {
            auto node = new BinarySearchTreeNode<T>(value);
            auto current = this->root;
            while (true) {
                if (node->value < current->value) {
                    if (current->left == nullptr) {
                        current->left = node;
                        break;
                    } else {
                        current = current->left;
                    }
                } else {
                    if (current->right == nullptr) {
                        current->right = node;
                        break;
                    } else {
                        current = current->right;
                    }
                }
            }
        }
    }

    // Funkcja służy wyłącznie do testów!
    // Pozwala na szybkie zbudowanie drzewa dla danych posortowanych malejąco
    template <typename Iterator>
    void fakeInsertLeft(Iterator begin, Iterator end) {
        if (begin == end) {
            return;
        }

        this->root = new BinarySearchTreeNode<int>(*begin);
        begin++;

        auto node = this->root;
        for (; begin < end; ++begin, node = node->left) {
            node->left = new BinarySearchTreeNode<int>(*begin);
        }
    }

    // Wstawia listę elementów do drzewa
    template <typename Iterator>
    void insertList(Iterator begin, Iterator end) {
        if (this->root == nullptr) {
            if (begin == end) {
                return;
            }

            this->root = new BinarySearchTreeNode<T>(*begin);
            ++begin;
        }

        for (; begin < end; ++begin) {
            auto node = new BinarySearchTreeNode<T>(*begin);

            auto current = this->root;
            while (true) {
                if (node->value < current->value) {
                    if (current->left == nullptr) {
                        current->left = node;
                        break;
                    } else {
                        current = current->left;
                    }
                } else {
                    if (current->right == nullptr) {
                        current->right = node;
                        break;
                    } else {
                        current = current->right;
                    }
                }
            }
        }
    }


    std::list<T> traversePreOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return this->root->traversePreOrder();
        }
    }

    std::list<T> traverseInOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return this->root->traverseInOrder();
        }
    }

    std::list<T> traverseInOrderIterative() const {
        std::list<T> trace;
        if (this->root != nullptr) {
            std::stack<BinarySearchTreeNode<T>*> parents;
            auto node = this->root;

            while (!(parents.empty() && node == nullptr)) {
                if (node != nullptr) {
                    parents.push(node);
                    node = node->left;
                } else {
                    node = parents.top();
                    parents.pop();
                    trace.push_back(node->value);
                    node = node->right;
                }
            }
        }

        return trace;
    }

    std::list<T> traversePostOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return this->root->traversePostOrder();
        }
    }

    T min() const {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto node = this->root->min();
            return node->value;
        }
    }

    T minIterative() const {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto node = this->root;
            while (node->left != nullptr) {
                node = node->left;
            }

            return node->value;
        }
    }


    T max() const {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto node = this->root->max();
            return node->value;
        }
    }

    std::list<T> trace(T value) const {
        if (this->root == nullptr) {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        } else {
            return this->root->trace(value);
        }
    }

    bool contains(T value) const {
        if (this->root == nullptr) {
            return false;
        } else {
            return this->root->contains(value);
        }
    }

    int height() const {
        if (this->root == nullptr) {
            return -1;
        } else {
            return this->root->height();
        }
    }

    void remove(T value) {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto parentOfNodeToRemove = this->root->findParent(value);
            BinarySearchTreeNode<T> *nodeToRemove;
            bool leftChild;

            if (parentOfNodeToRemove == nullptr) {
                nodeToRemove = this->root;
            } else if (value < parentOfNodeToRemove->value) {
                nodeToRemove = parentOfNodeToRemove->left;
                leftChild = true;
            } else {
                nodeToRemove = parentOfNodeToRemove->right;
                leftChild = false;
            }

            auto newNode = nodeToRemove->remove();

            if (nodeToRemove == this->root) {
                if (nodeToRemove != nullptr) {
                    delete nodeToRemove;
                }

                this->root = newNode;
            } else {
                if (nodeToRemove != nullptr) {
                    delete nodeToRemove;
                }

                if (leftChild) {
                    parentOfNodeToRemove->left = newNode;
                } else {
                    parentOfNodeToRemove->right = newNode;
                }
            }
        }
    }

    void clearPostOrder() {
        if (this->root != nullptr) {
            auto postOrder = this->traversePostOrder();
            for (auto entity : postOrder) {
                this->remove(entity);
            }
        }
    }

    // Implementacja iteracyjna zamiast rekurencyjnej
    // W implementacji funkcji przypisuję do node->right, aby zmienić wartość w strukturze, na którą wskazuje wskaźnik
    // A nie tylko samego wskaźnika
    void dsw() {
        auto node = this->root;
        if (node != nullptr) {
            // Początkowo zakładamy, że rozmiar listy to 1
            // Z każdym przesunięciem wskaźnika na kolejny element, będziemy ten rozmiar zwiększali
            int size = 1;

            while (node->left != nullptr) {
                node = node->rotateRight();
            }

            this->root = node;
            
            for (; node->right != nullptr; node = node->right, ++size) {
                while (node->right->left != nullptr) {
                    node->right = node->right->rotateRight();
                }
            }

            // TODO: Można to zamienić na prostą bitową operację
            int rotationsNumber = std::pow(2, std::floor(std::log2(size + 1))) - 1;

            BinarySearchTreeNode<T> parentOfRoot;
            parentOfRoot.right = this->root;
            node = &parentOfRoot;

            for (int i = 0; i < size - rotationsNumber; ++i) {
                node->right = node->right->rotateLeft();
                node = node->right;
            }

            while (rotationsNumber > 1) {
                rotationsNumber = std::floor(rotationsNumber / 2);
                node = &parentOfRoot;
                for (int i = 0; i < rotationsNumber; ++i) {
                    node->right = node->right->rotateLeft();
                    node = node->right;
                }
            }

            this->root = parentOfRoot.right;
        }
    }

private:
    BinarySearchTreeNode<T> *root = nullptr;
};
