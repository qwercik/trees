#pragma once

#include <vector>
#include <trees/bst/exceptions.hpp>

template <typename T>
struct Node {
    Node() {
    }

    Node(T value) {
        this->value = value;
    }

    T value;
    Node *left = nullptr;
    Node *right = nullptr;

    static void insert(Node<T> *root, Node<T> *node) {
        if (node->value < root->value) {
            if (root->left == nullptr) {
                root->left = node;
            } else {
                insert(root->left, node);
            }
        } else {
            if (root->right == nullptr) {
                root->right = node;
            } else {
                insert(root->right, node);
            }
        }
    }

    static std::vector<T> traversePreOrder(Node<T> *root) {
        std::vector<T> values;

        values.push_back(root->value);

        if (root->left != nullptr) {
            auto returnedValues = Node<T>::traversePreOrder(root->left);
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }


        if (root->right != nullptr) {
            auto returnedValues = Node<T>::traversePreOrder(root->right);
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        return values;
    }

    static std::vector<T> traverseInOrder(Node<T> *root) {
        std::vector<T> values;

        if (root->left != nullptr) {
            auto returnedValues = Node<T>::traverseInOrder(root->left);
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        values.push_back(root->value);

        if (root->right != nullptr) {
            auto returnedValues = Node<T>::traverseInOrder(root->right);
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        return values;
    }

    static std::vector<T> traversePostOrder(Node<T> *root) {
        std::vector<T> values;

        if (root->left != nullptr) {
            auto returnedValues = Node<T>::traversePostOrder(root->left);
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        if (root->right != nullptr) {
            auto returnedValues = Node<T>::traversePostOrder(root->right);
            values.insert(values.end(), returnedValues.begin(), returnedValues.end());
        }

        values.push_back(root->value);

        return values;
    }

    // Zwraca nullptr, jeżeli to root jest szukanym elementem
    static Node<T> *findParent(Node<T> *root, T value) {
        if (value == root->value) {
            return nullptr;
        } else if (value < root->value && root->left != nullptr) {
            if (value == root->left->value) {
                return root;
            } else if (value < root->left->value && root->left->left != nullptr) {
                return Node<T>::findParent(root->left, value);
            } else if (value > root->left->value && root->left->right != nullptr) {
                return Node<T>::findParent(root->left, value);
            } else {
                throw BinarySearchTreeElementNotExistException("Element not exist");
            }
        } else if (value > root->value && root->right != nullptr) {
            if (value == root->right->value) {
                return root;
            } else if (value < root->right->value && root->right->left != nullptr) {
                return Node<T>::findParent(root->right, value);
            } else if (value > root->right->value && root->right->right != nullptr) {
                return Node<T>::findParent(root->right, value);
            } else {
                throw BinarySearchTreeElementNotExistException("Element not exist");
            }
        } else {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        }
    }

    static Node<T> *find(Node<T> *root, T value) {
        if (value == root->value) {
            return root;
        } else if (value < root->value && root->left != nullptr) {
            return Node<T>::find(root->left, value);
        } else if (value > root->value && root->right != nullptr) {
            return Node<T>::find(root->right, value);
        } else {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        }
    }

    static Node<T> *min(Node<T> *root) {
        if (root->left != nullptr) {
            return Node<T>::min(root->left);
        }

        return root;
    }

    // Funkcja zwraca wskaźnik na węzeł, będący rodzicem najmniejszego elementu
    // Wykorzystywana głównie przez funkcję remove()
    // Jeżeli to obecny element (parametr) jest najmniejszy,
    // to zwracany jest nullptr
    static Node<T> *minParent(Node<T> *root) {
        if (root->left != nullptr) {
            if (root->left->left == nullptr) {
                return root;
            } else {
                return Node<T>::minParent(root->left);
            }
        } else {
            return nullptr;
        }
    }

    static Node<T> *max(Node<T> *root) {
        if (root->right != nullptr) {
            return Node<T>::max(root->right);
        }

        return root;
    }

    static std::vector<T> trace(Node<T> *root, T value) {
        std::vector<T> track = {root->value};

        if (value < root->value && root->left != nullptr) {
            auto subTrack = Node<T>::trace(root->left, value);
            track.insert(track.end(), subTrack.begin(), subTrack.end());
        } else if (value > root->value && root->right != nullptr) {
            auto subTrack = Node<T>::trace(root->right, value);
            track.insert(track.end(), subTrack.begin(), subTrack.end());
        } else if (value != root->value) {
            throw BinarySearchTreeElementNotExistException("Element not exist");
        }

        return track;
    }

    static bool contains(Node<T> *root, T value) {
        if (value == root->value) {
            return true;
        } else if (value < root->value && root->left != nullptr) {
            return Node<T>::contains(root->left, value);
        } else if (value > root->value && root->right != nullptr) {
            return Node<T>::contains(root->right, value);
        } else {
            return false;
        }
    }

    static int height(Node<T> *root) {
        if (root->left != nullptr && root->right == nullptr) {
            return 1 + Node<T>::height(root->left);
        } else if (root->left == nullptr && root->right != nullptr) {
            return 1 + Node<T>::height(root->right);
        } else if (root->left != nullptr && root->right != nullptr) {
            auto leftHeight = Node<T>::height(root->left);
            auto rightHeight = Node<T>::height(root->right);

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
    static Node<T> *remove(Node<T> *node) {
        if (node->left == nullptr && node->right == nullptr) {
            return nullptr;
        } else if (node->left != nullptr && node->right == nullptr) {
            return node->left;
        } else if (node->left == nullptr && node->right != nullptr) {
            return node->right;
        } else {
            auto successorParent = Node<T>::minParent(node->right);
            Node<T> *successor;

            if (successorParent == nullptr) {
                // successorParent = node;
                successor = node->right;
            } else {
                successor = successorParent->left;
                successor->right = node->right;
                successorParent->left = nullptr;                
            }

            successor->left = node->left;
            
            return successor;
        }
    }

    // Parametrem jest korzeń względem którego przeprowadzamy rotację
    // wartością zwaracaną jest wskaźnik na nowy korzeń
    static Node<T> *rotateLeft(Node<T> *root) {
        auto newRoot = root->right;
        if (newRoot == nullptr) {
            throw BinarySearchTreeRotationException("Cannot left rotate tree");
        }

        root->right = newRoot->left;
        newRoot->left = root;
        
        return newRoot;
    }

    static Node<T> *rotateRight(Node<T> *root) {
        auto newRoot = root->left;
        if (newRoot == nullptr) {
            throw BinarySearchTreeRotationException("Cannot right rotate tree");
        }

        root->left = newRoot->right;
        newRoot->right = root;
        
        return newRoot;
    }
};
