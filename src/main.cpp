#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <trees/bst/BinarySearchTree.hpp>
#include <trees/avl/AvlTree.hpp>
#include <trees/io.hpp>

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Invalid usage\n";
        std::cerr << "Use: " << argv[0] << " <type> <operation> [benchmark]\n\n";
        std::cerr << "<type> - is tree type; supported: bst, avl\n\n";
        std::cerr << "<operation> - operation to run\n";
        std::cerr << "\tbst supports: create, find-min, print, dsw\n";
        std::cerr << "\tavl supports: create, find-min, print\n\n";
        std::cerr << "[benchmark] - optional argument; pass to have not input\n\n";
        std::cerr << "Give list of integers (terminated with EOF) on input\n";
        return EXIT_FAILURE;
    }

    std::string treeType = argv[1];
    std::string operation = argv[2];
    bool benchmarkMode = argc == 4 && std::string(argv[3]) == "benchmark";

    if (treeType == "bst") {
        if (operation == "create") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            if (!benchmarkMode) {
                std::cout << "Tree successfully created\n";
            }
        } else if (operation == "find-min") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            auto min = tree.min();
            if (!benchmarkMode) {
                std::cout << "Min value: " << min << "\n";
            }
        } else if (operation == "print") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            auto data = tree.traverseInOrder();
            if (!benchmarkMode) {
                std::cout << "In-order: " << data << "\n";
            }
        } else if (operation == "dsw") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            int startingHeight = tree.height();
            tree.dsw();

            if (!benchmarkMode) {
                std::cout << "Successfully balanced\n";
                std::cout << "Starting height: " << startingHeight << '\n';
                std::cout << "Current height: " << tree.height();
            }
        } else {
            std::cerr << "Unknown operation\n";
            return EXIT_FAILURE;
        }
    } else if (treeType == "avl") {
        if (operation == "create") {
            AvlTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            if (!benchmarkMode) {
                std::cout << "Tree successfully created\n";
            }
        } else if (operation == "find-min") {
            AvlTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            auto min = tree.min();
            if (!benchmarkMode) {
                std::cout << "Min value: " << min << "\n";
            }
        } else if (operation == "print") {
            AvlTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());
            auto data = tree.traverseInOrder();
            if (!benchmarkMode) {
                std::cout << "In-order: " << data << "\n";
            }
        } else {
            std::cerr << "Unknown operation\n";
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Unknown tree type\n";
        return EXIT_FAILURE;
    }
}