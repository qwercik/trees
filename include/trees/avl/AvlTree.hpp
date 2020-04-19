#pragma once

#include <vector>
#include <list>
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

    // Iterative implementation
    void remove(T value) {
        if (this->root == nullptr) {
            throw new AvlTreeEmptyException("Tree is empty");
        } else {
            std::list<AvlNode<T>*> branchPointers;

            auto node = this->root;
            
            while (value != node->value) {
                branchPointers.push_back(node);
                node = value < node->value ? node->left : node->right;

                if (node == nullptr) {
                    throw AvlTreeElementNotExistException("Element not exist");
                }
            }

            // node to element który chcemy usunąć
            auto newNode = node->remove();
            if (newNode != nullptr) {
                newNode->updateHeight();
            }

            if (value == this->root->value) {
                this->root = newNode;
            } else {
                auto parent = branchPointers.back();
                
                if (value < parent->value) {
                    parent->left = newNode;
                    if (newNode != nullptr) {
                        parent->left = parent->left->fixAvlIfBroken();
                    }
                } else {
                    parent->right = newNode;
                    if (newNode != nullptr) {
                        parent->right = parent->right->fixAvlIfBroken();
                    }
                }
            }

            /*
            std::cerr << "Wskaźniki: ";
            for (auto &e : branchPointers) {
                std::cerr << e->value << ' ';
            }
            std::cerr << '\n';
            */


            if (branchPointers.size() > 0) {
                auto previousVisitedParent = branchPointers.back();
                previousVisitedParent->updateHeight();
                branchPointers.pop_back();

                for (; branchPointers.size() > 0; branchPointers.pop_back()) {
                    auto parent = branchPointers.back();
                    parent->updateHeight();
                    if (previousVisitedParent->value < parent->value) {
                        parent->left = parent->left->fixAvlIfBroken();
                    } else {
                        parent->right = parent->right->fixAvlIfBroken();
                    }

                    previousVisitedParent = parent;
                }

                this->root = previousVisitedParent->fixAvlIfBroken();
            }



            /*if (branchPointers.size() > 0) {
                auto it = branchPointers.rbegin();
                auto parent = *it;
                parent->updateHeight();
                it++;
                
                for (; it != branchPointers.rend(); it++) {
                    auto currentParent = *it;
                    currentParent->updateHeight();

                    if (parent->value < currentParent->value) {
                        currentParent->left = currentParent->left->fixAvlIfBroken();
                    } else {
                        currentParent->right = currentParent->right->fixAvlIfBroken();
                    }

                    parent = currentParent;
                }
            }*/

            if (node != nullptr) {
                delete node;
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

    bool correct(T &value) {
        if (this->root == nullptr) {
            return true;
        } else {
            return this->root->correct(value);
        }
    }

//private:
    AvlNode<T> *root = nullptr;
};
