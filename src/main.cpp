#include <iostream>
#include <vector>
#include <trees/bst/BinarySearchTree.hpp>
#include <trees/bst/exceptions.hpp>
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
    //std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> values = {6, 3, 13, 1, 4, 8, 14, 7, 10};
    //test<BinarySearchTree<int>>(values);

    BinarySearchTree<int> tree;
    tree.insertList(values.begin(), values.end());

    std::cout << "Oryginał: " << tree.traversePreOrder() << "\n";
    tree.remove(6);
    std::cout << "Po usunięciu 6: " << tree.traversePreOrder() << "\n";
    tree.remove(7);
    std::cout << "Po usunięciu 7: " << tree.traversePreOrder() << "\n";
}