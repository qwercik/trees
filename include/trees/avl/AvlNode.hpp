#pragma once

#include <list>
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

                return this->fixAvlIfBroken();
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
                return this->fixAvlIfBroken();
            }
        }
    }

    template <typename Iterator>
    void insertSortedList(Iterator begin, Iterator end) {
        if (begin >= end) {
            return;
        }

        auto size = end - begin;
        auto rootElement = begin + size / 2;
        this->value = *rootElement;

        if (begin < rootElement) {
            this->left = new AvlNode<T>;
            this->left->insertSortedList(begin, rootElement);
        }

        if (rootElement + 1 < end) {
            this->right = new AvlNode<T>;
            this->right->insertSortedList(rootElement + 1, end);
        }
    }

    void traversePreOrder(std::list<T> &trace) {
        trace.push_back(this->value);

        if (this->left != nullptr) {
            this->left->traversePreOrder(trace);
        }

        if (this->right != nullptr) {
            this->right->traversePreOrder(trace);
        }
    }

    void traverseInOrder(std::list<T> &trace) {
        if (this->left != nullptr) {
            this->left->traverseInOrder(trace);
        }

        trace.push_back(this->value);

        if (this->right != nullptr) {
            this->right->traverseInOrder(trace);
        }
    }

    void traversePostOrder(std::list<T> &trace) {
        if (this->left != nullptr) {
            this->left->traversePostOrder(trace);
        }

        if (this->right != nullptr) {
            this->right->traversePostOrder(trace);
        }

        trace.push_back(this->value);
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

    std::list<T> trace(T value) {
        std::list<T> track = {this->value};

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

    inline int leftHeight() {
        return this->left != nullptr ? 1 + this->left->height() : 0;
    }

    inline int rightHeight() {
        return this->right != nullptr ? 1 + this->right->height() : 0;
    }

    inline int balanceFactor() {
        return this->leftHeight() - this->rightHeight();
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
        // Może zmienić implementację?
        // można by napisać jakiś wrapper na left->height i right->height, który obsłuży nullptr
        // jest to często spotykany w tym kodzie wzorzec
        // a potem zamienić wszystko na zwyczajne std::max()

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

    // Zwracany jest wskaźnik na element, który zastąpi usuwany węzeł
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
            std::list<AvlNode<T>*> parentsList;
            auto node = this->right;
            while (node->left != nullptr) {
                parentsList.push_back(node);
                node = node->left;
            }
            
            AvlNode<T> *successor;
            auto successorParent = parentsList.back();
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

            parentsList.push_front(successor);

            if (parentsList.size() > 0) {
                auto previousVisitedParent = parentsList.back();
                parentsList.pop_back();
                previousVisitedParent->updateHeight();
                
                for (; parentsList.size() > 0; parentsList.pop_back()) {
                    auto parent = parentsList.back();
                    parent->updateHeight();

                    if (previousVisitedParent->value < parent->value) {
                        parent->left = parent->left->fixAvlIfBroken();
                    } else {
                        parent->right = parent->right->fixAvlIfBroken();
                    }

                    previousVisitedParent = parent;
                }

                //std::cerr << "Kto to będzie sukcesorem: " << previousVisitedParent->value << '\n';
                successor = previousVisitedParent->fixAvlIfBroken();
            }
            
            return successor;
        }
    }

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

    AvlNode<T> *fixAvlIfBroken() {
        if (this->balanceFactor() == 2) {
            if (this->left->balanceFactor() >= 0) {
                return this->rotateRight();
            } else {
                this->left = this->left->rotateLeft();
                return this->rotateRight();
            }
        } else if (this->balanceFactor() == -2) {
            if (this->right->balanceFactor() <= 0) {
                return this->rotateLeft();
            } else {
                this->right = this->right->rotateRight();
                return this->rotateLeft();
            }
        } else {
            return this;
        }
    }

    bool correct(T &value) {
        if (this->balanceFactor() < -1 || this->balanceFactor() > 1) {
            value = this->value;
            return false;
        }

        bool ok = true;

        if (this->left != nullptr) {
            if (!this->left->correct(value)) {
                return false;
            }
        }

        if (this->right != nullptr) {
            if (!this->right->correct(value)) {
                return false;
            }
        }

        return true;
    }


    T value;
    AvlNode *left = nullptr;
    AvlNode *right = nullptr;
    int treeHeight = 0;
};
