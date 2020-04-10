#pragma once

#include <vector>
#include <cmath>
#include <trees/bst/Node.hpp>
#include <trees/bst/exceptions.hpp>

template <typename T>
class BinarySearchTree {
public:
    BinarySearchTree() {}

    ~BinarySearchTree() {
        this->clearPostOrder();
    }

    void insert(T value) {
        if (this->root == nullptr) {
            this->root = new Node<T>(value);
        } else {
            auto node = new Node<T>(value);
            Node<T>::insert(this->root, node);
        }
    }

    template <typename Iterator>
    void insertList(Iterator begin, Iterator end) {
        if (this->root == nullptr) {
            if (begin == end) {
                return;
            }

            this->root = new Node<T>(*begin);
            ++begin;
        }

        for (; begin < end; ++begin) {
            auto node = new Node<T>(*begin);
            Node<T>::insert(this->root, node);
        }
    }

    std::vector<T> traversePreOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return Node<T>::traversePreOrder(this->root);
        }
    }

    std::vector<T> traverseInOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return Node<T>::traverseInOrder(this->root);
        }
    }

    std::vector<T> traversePostOrder() const {
        if (this->root == nullptr) {
            return {};
        } else {
            return Node<T>::traversePostOrder(this->root);
        }
    }

    T min() const {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto node = Node<T>::min(this->root);
            return node->value;
        }
    }

    T max() const {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto node = Node<T>::max(this->root);
            return node->value;
        }
    }

    std::vector<T> trace(T value) const {
        if (this->root == nullptr) {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        } else {
            return Node<T>::trace(this->root, value);
        }
    }

    bool contains(T value) const {
        if (this->root == nullptr) {
            return false;
        } else {
            return Node<T>::contains(this->root, value);
        }
    }

    int height() const {
        if (this->root == nullptr) {
            return -1;
        } else {
            return Node<T>::height(this->root);
        }
    }

    void remove(T value) {
        if (this->root == nullptr) {
            throw BinarySearchTreeEmptyException("Tree is empty");
        } else {
            auto parentOfNodeToRemove = Node<T>::findParent(this->root, value);
            Node<T> *nodeToRemove;
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

            auto newNode = Node<T>::remove(nodeToRemove);

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
                node = Node<T>::rotateRight(node);     
            }
            this->root = node;
            
            for (; node->right != nullptr; node = node->right, ++size) {
                while (node->right->left != nullptr) {
                    node->right = Node<T>::rotateRight(node->right);
                }
            }

            // TODO: Można to zamienić na prostą bitową operację
            int rotationsNumber = std::pow(2, std::floor(std::log2(size + 1))) - 1;

            Node<T> parentOfRoot;

            parentOfRoot.right = this->root;
            node = &parentOfRoot;

            for (int i = 0; i < rotationsNumber; ++i) {
                node->right = Node<T>::rotateLeft(node->right);

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
    Node<T> *root = nullptr;
};
