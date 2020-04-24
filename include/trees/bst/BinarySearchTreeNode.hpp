#pragma once

#include <list>
#include <trees/bst/exceptions.hpp>

template <typename T>
struct BinarySearchTreeNode {
    BinarySearchTreeNode() {
    }

    BinarySearchTreeNode(T value) {
        this->value = value;
    }

    int insert(BinarySearchTreeNode<T> *node) {
        if (node->value < this->value) {
            if (this->left == nullptr) {
                this->left = node;
                return 1;
            } else {
                return 1 + this->left->insert(node);
            }
        } else {
            if (this->right == nullptr) {
                this->right = node;
                return 1;
            } else {
                return 1 + this->right->insert(node);
            }
        }
    }

    std::list<T> traversePreOrder() {
        std::list<T> values;

        values.push_back(this->value);

        if (this->left != nullptr) {
            auto returnedValues = this->left->traversePreOrder();
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        if (this->right != nullptr) {
            auto returnedValues = this->right->traversePreOrder();
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        return values;
    }

    std::list<T> traverseInOrder() {
        std::list<T> values;

        if (this->left != nullptr) {
            auto returnedValues = this->left->traverseInOrder();
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        values.push_back(this->value);

        if (this->right != nullptr) {
            auto returnedValues = this->right->traverseInOrder();
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        return values;
    }

    std::list<T> traversePostOrder() {
        std::list<T> values;

        if (this->left != nullptr) {
            auto returnedValues = this->left->traversePostOrder();
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        if (this->right != nullptr) {
            auto returnedValues = this->right->traversePostOrder();
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        values.push_back(this->value);

        return values;
    }

    // Zwraca nullptr, jeżeli to obecny węzeł jest szukanym elementem
    BinarySearchTreeNode<T> *findParent(T value) {
        if (value == this->value) {
            return nullptr;
        } else if (value < this->value && this->left != nullptr) {
            if (value == this->left->value) {
                return this;
            } else if (value < this->left->value && this->left->left != nullptr) {
                return this->left->findParent(value);
            } else if (value > this->left->value && this->left->right != nullptr) {
                return this->left->findParent(value);
            } else {
                throw BinarySearchTreeElementNotExistException("Element not exist");
            }
        } else if (value > this->value && this->right != nullptr) {
            if (value == this->right->value) {
                return this;
            } else if (value < this->right->value && this->right->left != nullptr) {
                return this->right->findParent(value);
            } else if (value > this->right->value && this->right->right != nullptr) {
                return this->right->findParent(value);
            } else {
                throw BinarySearchTreeElementNotExistException("Element not exist");
            }
        } else {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        }
    }

    BinarySearchTreeNode<T> *find(T value) {
        if (value == this->value) {
            return this;
        } else if (value < this->value && this->left != nullptr) {
            return this->left->find(value);
        } else if (value > this->value && this->right != nullptr) {
            return this->right->find(value);
        } else {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        }
    }

    BinarySearchTreeNode<T> *min() {
        if (this->left != nullptr) {
            return this->left->min();
        }

        return this;
    }

    // Funkcja zwraca wskaźnik na węzeł, będący rodzicem najmniejszego elementu
    // Wykorzystywana głównie przez funkcję remove()
    // Jeżeli to obecny element (parametr) jest najmniejszy,
    // to zwracany jest nullptr (bo nie znamy rodzica obecnego elementu)
    BinarySearchTreeNode<T> *parentOfMin() {
        if (this->left != nullptr) {
            if (this->left->left == nullptr) {
                return this;
            } else {
                return this->left->parentOfMin();
            }
        } else {
            return nullptr;
        }
    }

    BinarySearchTreeNode<T> *max() {
        if (this->right != nullptr) {
            return this->right->max();
        }

        return this;
    }

    std::list<T> trace(T value) {
        std::list<T> track = {this->value};

        if (value < this->value && this->left != nullptr) {
            auto subTrack = this->left->trace(value);
            track.insert(track.end(), subTrack.begin(), subTrack.end());
        } else if (value > this->value && this->right != nullptr) {
            auto subTrack = this->right->trace(value);
            track.insert(track.end(), subTrack.begin(), subTrack.end());
        } else if (value != this->value) {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        }

        return track;
    }

    bool contains(T value) {
        if (value == this->value) {
            return true;
        } else if (value < this->value && this->left != nullptr) {
            return this->left->contains(value);
        } else if (value > this->value && this->right != nullptr) {
            return this->right->contains(value);
        } else {
            return false;
        }
    }

    int height() {
        if (this->left != nullptr && this->right == nullptr) {
            return 1 + this->left->height();
        } else if (this->left == nullptr && this->right != nullptr) {
            return 1 + this->right->height();
        } else if (this->left != nullptr && this->right != nullptr) {
            auto leftHeight = this->left->height();
            auto rightHeight = this->right->height();

            if (leftHeight > rightHeight) {
                return 1 + leftHeight;
            } else {
                return 1 + rightHeight;
            }
        } else {
            // Nie ma dzieci
            return 0;
        }
    }

    // Parametrem node jest węzeł, który chcemy usunąć
    // Zwracany jest wskaźnik na element, który go zastąpi
    // Wywołujący może zdealokować zaalokowaną dynamicznie pamięć,
    // bo posiada wskaźnik na usuwany węzeł
    BinarySearchTreeNode<T> *remove() {
        if (this->left == nullptr && this->right == nullptr) {
            return nullptr;
        } else if (this->left != nullptr && this->right == nullptr) {
            return this->left;
        } else if (this->left == nullptr && this->right != nullptr) {
            return this->right;
        } else {
            auto successorParent = this->right->parentOfMin();
            BinarySearchTreeNode<T> *successor;
            // następnik (successor) na pewno nie ma lewego dziecka, więc można wartość wskaźnika nadpisywać
            // Za to może mieć prawe dziecko, więc trzeba pamiętać, żeby to uwzględnić

            // Jeżeli parentOfMin zwróciło null, to znaczy że rodzicem jest element nadrzędny względem obecnego
            // A więc successorParent jest elementem usuwanym
            if (successorParent == nullptr) {
                successorParent = this;
                successor = this->right;

                successor->left = this->left;
            } else {
                successor = successorParent->left;

                successor->left = this->left;
                successorParent->left = successor->right;
                successor->right = this->right;
            }
            
            return successor;
        }
    }

    // Parametrem jest korzeń względem którego przeprowadzamy rotację
    // wartością zwaracaną jest wskaźnik na nowy korzeń
    BinarySearchTreeNode<T> *rotateLeft() {
        auto newRoot = this->right;
        if (newRoot == nullptr) {
            throw BinarySearchTreeRotationException("Cannot left rotate tree");
        }

        this->right = newRoot->left;
        newRoot->left = this;
        
        return newRoot;
    }

    BinarySearchTreeNode<T> *rotateRight() {
        auto newRoot = this->left;
        if (newRoot == nullptr) {
            throw BinarySearchTreeRotationException("Cannot right rotate tree");
        }

        this->left = newRoot->right;
        newRoot->right = this;
        
        return newRoot;
    }

    T value;
    BinarySearchTreeNode *left = nullptr;
    BinarySearchTreeNode *right = nullptr;
};
