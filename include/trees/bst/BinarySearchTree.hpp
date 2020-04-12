#pragma once

#include <vector>
#include <cmath>
#include <trees/bst/BinarySearchTreeNode.hpp>
#include <trees/bst/exceptions.hpp>

template <typename T>
class BinarySearchTree {
public:
    BinarySearchTree() {
    }

    ~BinarySearchTree() {
        this->clearPostOrder();
    }

    // Wstawia element do drzewa
    // Zwraca długość gałęzi łączącej korzeń z nowo wstawionym elementem
    int insert(T value) {
        if (this->root == nullptr) {
            this->root = new BinarySearchTreeNode<T>(value);
            return 0;
        } else {
            auto node = new BinarySearchTreeNode<T>(value);
            return this->root->insert(node);
        }
    }

    // Wstawia listę elementów do drzewa
    // Zwraca długość najdłuższej gałęzi łączącej korzeń z nowo wstawionym elementem
    // Jeżeli drzewo było na początku puste, zwracana wartość jest wysokością drzewa.
    // Jeżeli nie, nie ma takiej gwarancji
    template <typename Iterator>
    int insertList(Iterator begin, Iterator end) {
        if (this->root == nullptr) {
            if (begin == end) {
                return 0;
            }

            this->root = new BinarySearchTreeNode<T>(*begin);
            ++begin;
        }

        int height = 0;
        for (; begin < end; ++begin) {
            auto node = new BinarySearchTreeNode<T>(*begin);
            auto branchHeight = this->root->insert(node);
            if (branchHeight > height) {
                height = branchHeight;
            }
        }

        return height;
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
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto node = this->root->min();
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

    std::vector<T> trace(T value) const {
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
    BinarySearchTreeNode<T> *root = nullptr;
};
