#include <iostream>
#include <vector>
#include <trees/bst/BinarySearchTree.hpp>
#include <trees/bst/exceptions.hpp>
#include <trees/avl/AvlTree.hpp>
#include <trees/avl/exceptions.hpp>
#include <trees/io.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>


template <typename Tree, typename T>
void test(const std::vector<T>& values) {
    std::cerr << "Testowane wartości: " << values << "\n\n";

    Tree tree;
    tree.insertList(values.begin(), values.end());

    for (int i = 0; i < 2; ++i) {
        if (i == 1) {
            std::cerr << "\n---------- running DSW ----------\n\n";
            tree.dsw();
        }

        std::cerr << "Pre-order: " << tree.traversePreOrder() << "\n";
        std::cerr << "In-order: " << tree.traverseInOrder() << "\n";
        std::cerr << "Post-order: " << tree.traversePostOrder() << "\n\n";

        std::cerr << "Min: " << tree.min() << "\n";
        std::cerr << "Min trace: " << tree.trace(tree.min()) << "\n\n";
        std::cerr << "Max: " << tree.max() << "\n";
        std::cerr << "Max trace: " << tree.trace(tree.max()) << "\n";

        std::cerr << "Height: " << tree.height() << "\n";
    }

    tree.remove(4);
    std::cerr << "Po usunięciu: " << tree.traversePreOrder() << "\n";
    tree.dsw();
    std::cerr << "PO DSW\n";
    std::cerr << "Pre-order: " << tree.traversePreOrder() << "\n";
    std::cerr << "Post-order: " << tree.traversePostOrder() << "\n";
    std::cerr << "Height: " << tree.height() << '\n';
}

std::vector<int> randomList(int n) {
    std::vector<int> list;
    for (int i = 0; i < n; ++i) {
    etykieta:
        auto value = rand() % 1000;
        for (int j = 0; j < list.size(); ++j) {
            if (list[j] == value) {
                goto etykieta;
            }
        }

        list.push_back(value);
    }

    return list;
}

void testAvl() {
    std::vector<int> values = randomList(1000);
    std::cout << "Wstawiam listę: " << values << '\n';

    AvlTree<int> tree;
    tree.insertList(values.begin(), values.end());
    std::cout << "Po wstawieniu: " << tree.traversePreOrder() << '\n';

    std::random_shuffle(values.begin(), values.end());
    std::cout << "Mieszamy listę: " << values << '\n';


    int temp;
    std::cout << (tree.correct(temp) ? "ok" : "błąd") << '\n';
    
    bool ok = true;

    for (int i = 0; i < values.size(); ++i) {
        std::cout << "Usuwam: " << values[i] << " - " << std::flush;
        tree.remove(values[i]);
        bool correct = tree.correct(temp);
        std::cout << (correct ? "ok" : "błąd przy " + std::to_string(temp)) << " - " << std::flush;
        std::cout << tree.traversePreOrder() << " | " << std::flush;
        std::cout << tree.traversePostOrder() << "\n" << std::flush;

        if (!correct) {
            ok = false;
            break;
        }
    }

    std::cout << "UWAGA: " << (ok ? "OK" : "FAIL") << '\n';
}

int main() {
    std::srand(std::time(NULL));

    AvlTree<int> tree;
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    tree.insertSortedList(values.begin(), values.end());

    std::cout << tree.traversePreOrder() << '\n';
    std::cout << tree.height() << '\n';
}