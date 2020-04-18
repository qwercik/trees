#include <iostream>
#include <vector>
#include <trees/bst/BinarySearchTree.hpp>
#include <trees/bst/exceptions.hpp>
#include <trees/avl/AvlTree.hpp>
#include <trees/avl/exceptions.hpp>
#include <trees/io.hpp>

template <typename Tree, typename T>
void test(const std::vector<T>& values) {
    std::cout << "Testowane wartości: " << values << "\n\n";

    Tree tree;
    tree.insertList(values.begin(), values.end());

    for (int i = 0; i < 2; ++i) {
        if (i == 1) {
            std::cout << "\n---------- running DSW ----------\n\n";
            tree.dsw();
        }

        std::cout << "Pre-order: " << tree.traversePreOrder() << "\n";
        std::cout << "In-order: " << tree.traverseInOrder() << "\n";
        std::cout << "Post-order: " << tree.traversePostOrder() << "\n\n";

        std::cout << "Min: " << tree.min() << "\n";
        std::cout << "Min trace: " << tree.trace(tree.min()) << "\n\n";
        std::cout << "Max: " << tree.max() << "\n";
        std::cout << "Max trace: " << tree.trace(tree.max()) << "\n";

        std::cout << "Height: " << tree.height() << "\n";
    }

    tree.remove(4);
    std::cout << "Po usunięciu: " << tree.traversePreOrder() << "\n";
    tree.dsw();
    std::cout << "PO DSW\n";
    std::cout << "Pre-order: " << tree.traversePreOrder() << "\n";
    std::cout << "Post-order: " << tree.traversePostOrder() << "\n";
    std::cout << "Height: " << tree.height() << '\n';
}

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    //std::vector<int> values = {6, 5, 4, 3, 2, 1, 10, 9, 8, 7, };

    AvlTree<int> tree;
    tree.insertList(values.begin(), values.end());
    
    std::cout << tree.traversePreOrder() << '\n';
    std::cout << "Wysokość: " << tree.height() << '\n';

    const int value = 6;
    std::cout << "Uwaga, uwaga, uwaga - usuwam " << value << '\n';
    tree.remove(value);
    std::cout << tree.traversePreOrder() << '\n';
    std::cout << "root bf: " << tree.root->balanceFactor() << '\n';
    std::cout << "root l: " << 1 + tree.root->left->height() << '\n';
    std::cout << "root r: " << 1 + tree.root->right->height() << '\n';
    std::cerr << '\n';

    tree.remove(8);
    std::cout << "Usuwam 8\n";
    std::cout << tree.traversePreOrder() << '\n';
    std::cout << "root bf: " << tree.root->balanceFactor() << '\n';
    std::cout << "root l: " << 1 + tree.root->left->height() << '\n';
    std::cout << "root r: " << 1 + tree.root->right->height() << '\n';
    std::cerr << '\n';


    std::cout << "Usuwam 4\n";
    tree.remove(4);
    std::cout << tree.traversePreOrder() << '\n';
    std::cout << "root bf: " << tree.root->balanceFactor() << '\n';
    std::cout << "root l: " << 1 + tree.root->left->height() << '\n';
    std::cout << "root r: " << 1 + tree.root->right->height() << '\n';
    std::cerr << '\n';
}