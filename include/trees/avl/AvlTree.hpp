#pragma once

#include <vector>
#include <cmath>
#include <trees/avl/AvlNode.hpp>
#include <trees/avl/exceptions.hpp>
#include <trees/io.hpp>

template <typename T>
class AvlTree {
public:
    AvlTree() {
    }

    ~AvlTree() {
        this->clearPostOrder();
    }

    // Wstawia element do drzewa
    void insert(T value) {
        if (this->root == nullptr) {
            this->root = new AvlNode<T>(value);
        } else {
            auto node = new AvlNode<T>(value);
            this->root = this->root->insert(node);
        }
    }

    // Wstawia listę elementów do drzewa
    template <typename Iterator>
    void insertList(Iterator begin, Iterator end) {
        if (this->root == nullptr) {
            if (begin == end) {
                return;
            }

            this->root = new AvlNode<T>(*begin);
            ++begin;
        }

        for (; begin < end; ++begin) {
            auto node = new AvlNode<T>(*begin);
            this->root = this->root->insert(node);

            std::cerr << "Dodaję element " << *begin << '\n';
            std::cerr << "Obecnie: " << this->traversePreOrder() << "\n\n";
        }
    }

    std::vector<T> traversePreOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return this->root->traversePreOrder();
        }
    }

    std::vector<T> traverseInOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return this->root->traverseInOrder();
        }
    }

    std::vector<T> traversePostOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return this->root->traversePostOrder();
        }
    }

    T min() const {
        if (this->root == nullptr) {
            throw AvlTreeEmptyException("Tree is empty");
        } else {
            auto node = this->root->min();
            return node->value;
        }
    }

    T max() const {
        if (this->root == nullptr) {
            throw AvlTreeEmptyException("Tree is empty");
        } else {
            auto node = this->root->max();
            return node->value;
        }
    }

    std::vector<T> trace(T value) const {
        if (this->root == nullptr) {
            throw AvlTreeElementNotExistException("Element not exist");
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
            throw AvlTreeEmptyException("Tree is empty");
        } else {
            auto parentOfNodeToRemove = this->root->findParent(value);
            AvlNode<T> *nodeToRemove;
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

            AvlNode<T> parentOfRoot;

            parentOfRoot.right = this->root;
            node = &parentOfRoot;

            for (int i = 0; i < rotationsNumber; ++i) {
                node->right = node->right->rotateLeft();

                if (node->right->right != nullptr && node->right->right->right != nullptr) {
                    node = node->right;
                } else {
                    node = &parentOfRoot;
                }
            }

            this->root = parentOfRoot.right;
        }
    }

private:
    AvlNode<T> *root = nullptr;
};
