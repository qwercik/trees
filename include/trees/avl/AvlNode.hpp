#pragma once

#include <vector>
#include <trees/avl/exceptions.hpp>

template <typename T>
struct AvlNode {
    AvlNode() {
    }

    AvlNode(T value) {
        this->value = value;
    }

    // Inaczej niż w przypadku BinarySearchTreeNode
    // funkcja zwraca wskaźnik na nowy korzeń 
    // WAŻNE: wstawiać należy wyłącznie luźne węzły (left i right są nullptr)
    AvlNode<T> *insert(AvlNode<T> *node) {
        if (node->value < this->value) {
            if (this->left == nullptr) {
                this->left = node;
                this->treeHeight = std::max(this->treeHeight, 1);                
                // Tutaj nie trzeba chyba rotować
                return this;
            } else {
                this->left = this->left->insert(node);
                auto newSubtreeHeight = this->left->height();
                this->treeHeight = std::max(this->treeHeight, 1 + newSubtreeHeight);

                // dodajemy element po lewej stronie, więc drzewo mogło zostać przeciążone tylko w lewo
                if (this->balanceFactor() == 2) {
                    // Teraz musimy sprawdzić w które z poddrzew naszego dziecka
                    // został dodany element (to dodanie już wystąpiło)

                    if (node->value < this->left->value) {
                        // Pojedyncza rotacja
                        return this->rotateRight();

                    } else {
                        // Podwójna rotacja
                        this->left = this->left->rotateLeft();
                        return this->rotateRight();
                    }
                } else {
                    return this;
                }
            }
        } else {
            if (this->right == nullptr) {
                this->right = node;
                this->treeHeight = std::max(this->treeHeight, 1);
                // Tutaj chyba także rotować
                return this;
            } else {
                this->right = this->right->insert(node);
                auto newSubtreeHeight = this->right->height();
                this->treeHeight = std::max(this->treeHeight, 1 + newSubtreeHeight);

                // dodajemy element z prawej strony, więc drzewo mogło zostać przeciążone tylko w prawo
                if (this->balanceFactor() == -2) {
                    if (node->value < this->right->value) {
                        // Podwójna rotacja
                        this->right = this->right->rotateRight();
                        return this->rotateLeft();
                    } else {
                        // Pojedyncza rotacja
                        return this->rotateLeft();
                    }
                } else {
                    return this;
                }
            }
        }
    }

    std::vector<T> traversePreOrder() {
        std::vector<T> values;

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

    std::vector<T> traverseInOrder() {
        std::vector<T> values;

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

    std::vector<T> traversePostOrder() {
        std::vector<T> values;

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
    AvlNode<T> *findParent(T value) {
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
                throw AvlTreeElementNotExistException("Element not exist");
            }
        } else if (value > this->value && this->right != nullptr) {
            if (value == this->right->value) {
                return this;
            } else if (value < this->right->value && this->right->left != nullptr) {
                return this->right->findParent(value);
            } else if (value > this->right->value && this->right->right != nullptr) {
                return this->right->findParent(value);
            } else {
                throw AvlTreeElementNotExistException("Element not exist");
            }
        } else {
            throw AvlTreeElementNotExistException("Element not exist");
        }
    }

    AvlNode<T> *find(T value) {
        if (value == this->value) {
            return this;
        } else if (value < this->value && this->left != nullptr) {
            return this->left->find(value);
        } else if (value > this->value && this->right != nullptr) {
            return this->right->find(value);
        } else {
            throw AvlTreeElementNotExistException("Element not exist");
        }
    }

    AvlNode<T> *min() {
        if (this->left != nullptr) {
            return this->left->min();
        }

        return this;
    }

    // Funkcja zwraca wskaźnik na węzeł, będący rodzicem najmniejszego elementu
    // Wykorzystywana głównie przez funkcję remove()
    // Jeżeli to obecny element (parametr) jest najmniejszy,
    // to zwracany jest nullptr (bo nie znamy rodzica obecnego elementu)
    AvlNode<T> *parentOfMin() {
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

    AvlNode<T> *max() {
        if (this->right != nullptr) {
            return this->right->max();
        }

        return this;
    }

    std::vector<T> trace(T value) {
        std::vector<T> track = {this->value};

        if (value < this->value && this->left != nullptr) {
            auto subTrack = this->left->trace(value);
            track.insert(track.end(), subTrack.begin(), subTrack.end());
        } else if (value > this->value && this->right != nullptr) {
            auto subTrack = this->right->trace(value);
            track.insert(track.end(), subTrack.begin(), subTrack.end());
        } else if (value != this->value) {
            throw AvlTreeElementNotExistException("Element not exist");
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

    inline int height() { 
        return this->treeHeight;
    }

    int balanceFactor() {
        auto leftHeight = this->left != nullptr ? 1 + this->left->height() : 0;
        auto rightHeight = this->right != nullptr ? 1 + this->right->height() : 0;

        return leftHeight - rightHeight;
    }

    // Aktualizuje wysokość obecnego węzła, WSZYSTKICH podwęzłów i zwraca nową wysokość obecnego węzła
    int updateHeightDeep() {
        if (this->left != nullptr && this->right == nullptr) {
            this->treeHeight = 1 + this->left->updateHeightDeep();
        } else if (this->left == nullptr && this->right != nullptr) {
            this->treeHeight = 1 + this->right->updateHeightDeep();
        } else if (this->left != nullptr && this->right != nullptr) {
            auto leftHeight = this->left->updateHeightDeep();
            auto rightHeight = this->right->updateHeightDeep();

            if (leftHeight > rightHeight) {
                this->treeHeight = 1 + leftHeight;
            } else {
                this->treeHeight = 1 + rightHeight;
            }
        } else {
            // Nie ma dzieci
            this->treeHeight = 0;
        }

        return this->treeHeight;
    }

    // Aktualizuje wysokość drzewa bazując na dzieciach
    // Nie robi głębokiej aktualizacji, aktualizuje wyłącznie wysokość tego drzewa!
    // zwraca nową wysokość drzewa
    int updateHeight() {
        if (this->left != nullptr && this->right == nullptr) {
            this->treeHeight = this->left->treeHeight + 1;
        } else if (this->left == nullptr && this->right != nullptr) {
            this->treeHeight = this->right->treeHeight + 1;
        } else if (this->left != nullptr && this->right != nullptr) {
            this->treeHeight = 1 + std::max(this->left->treeHeight, this->right->treeHeight);
        } else {
            this->treeHeight = 0;
        }

        return this->treeHeight;
    }

    // Parametrem node jest węzeł, który chcemy usunąć
    // Zwracany jest wskaźnik na element, który go zastąpi
    // Wywołujący może zdealokować zaalokowaną dynamicznie pamięć,
    // bo posiada wskaźnik na usuwany węzeł
    AvlNode<T> *remove() {
        if (this->left == nullptr && this->right == nullptr) {
            return nullptr;
        } else if (this->left != nullptr && this->right == nullptr) {
            return this->left;
        } else if (this->left == nullptr && this->right != nullptr) {
            return this->right;
        } else {
            auto successorParent = this->right->parentOfMin();
            AvlNode<T> *successor;
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
    AvlNode<T> *rotateLeft() {
        auto newRoot = this->right;
        if (newRoot == nullptr) {
            throw AvlTreeRotationException("Cannot left rotate tree");
        }

        this->right = newRoot->left;
        newRoot->left = this;
        
        // Teraz trzeba nadpisać wysokości
        // najpierw aktualizujemy wysokość nowego dziecka (czyli this)
        this->updateHeight();
        newRoot->updateHeight();

        return newRoot;
    }

    AvlNode<T> *rotateRight() {
        auto newRoot = this->left;
        if (newRoot == nullptr) {
            throw AvlTreeRotationException("Cannot right rotate tree");
        }

        this->left = newRoot->right;
        newRoot->right = this;

        // Teraz trzeba nadpisać wysokości
        // najpierw aktualizujemy wysokość nowego dziecka (czyli this)
        this->updateHeight();
        newRoot->updateHeight();
        
        return newRoot;
    }

    T value;
    AvlNode *left = nullptr;
    AvlNode *right = nullptr;
    int treeHeight = 0;
};
