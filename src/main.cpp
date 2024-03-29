// Uwaga!!!
// Ten kod (main.cpp) to istny śmietnik, także proszę nie przywiązywać do niego większej uwagi
// Został napisany wyłącznie w celu przetestowania klas drzewa
// Struktura kodu w tym pliku jest mało ciekawa, żebym nie wyraził się dosadniej

// Interesujące rzeczy można znaleźć w katalogu include/trees, tam znajdują się szablony klas drzew BST i AVL
// Ich kod również "nie powala", gdybym miał więcej czasu to bym go przepisał, przy okazji np. pozbywając sie rekurencji

#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <functional>
#include <ctime>
#include <trees/bst/BinarySearchTree.hpp>
#include <trees/avl/AvlTree.hpp>
#include <trees/io.hpp>

#include <sys/resource.h>

void requestMoreStackSize() {
	const rlim_t newSize = 4L * 1024L * 1024L * 1024L;
	rlimit rl;
	int result;
	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0) {
		if (rl.rlim_cur < newSize) {
			rl.rlim_cur = newSize;
			result = setrlimit(RLIMIT_STACK, &rl);
			if (result != 0) {
				std::cerr << "Wystąpił błąd!";
				exit(1);
			}
		}
	}
}

double measureExecutionTime(std::function<void()> callback) {
    auto startTime = std::chrono::high_resolution_clock::now();
    callback();
    auto stopTime = std::chrono::high_resolution_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();
    return milliseconds / 1000.;
}

int main(int argc, char *argv[]) {
	requestMoreStackSize();

    if (argc != 3) {
        std::cerr << "Invalid usage\n";
        std::cerr << "Use: " << argv[0] << " <type> <operation>\n\n";
        std::cerr << "<type> - is tree type; supported: bst, avl\n\n";
        std::cerr << "<operation> - operation to run\n";
        std::cerr << "\tbst supports: create, find-min, print, dsw\n";
        std::cerr << "\tavl supports: create, find-min, print\n\n";
        std::cerr << "Give list of integers (terminated with EOF) on input\n";
        return EXIT_FAILURE;
    }

    std::string treeType = argv[1];
    std::string operation = argv[2];

    if (treeType == "bst") {
        if (operation == "create") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();

            std::cout << measureExecutionTime([&]() {
                tree.insertList(values.begin(), values.end());
            }) << '\n';

        } else if (operation == "find-min") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.fakeInsertLeft(values.begin(), values.end());

            std::cout << measureExecutionTime([&]() {
                auto min = tree.minIterative();
            }) << '\n';
        } else if (operation == "print") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.fakeInsertLeft(values.begin(), values.end());

            std::cout << measureExecutionTime([&]() {
                auto data = tree.traverseInOrder();
            }) << '\n';
        } else if (operation == "dsw") {
            BinarySearchTree<int> tree;
            auto values = readList<int>();
            tree.fakeInsertLeft(values.begin(), values.end());

            std::cout << measureExecutionTime([&]() {
                tree.dsw();
            }) << '\n';
        } else {
            std::cerr << "Unknown operation\n";
            return EXIT_FAILURE;
        }
    } else if (treeType == "avl") {
        if (operation == "create") {
            AvlTree<int> tree;
            auto values = readList<int>();

            std::cout << measureExecutionTime([&]() {
                tree.insertList(values.begin(), values.end());
            }) << '\n';
        } else if (operation == "find-min") {
            AvlTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());

            std::cout << measureExecutionTime([&]() {
                auto min = tree.minIterative();
            }) << '\n';
        } else if (operation == "print") {
            AvlTree<int> tree;
            auto values = readList<int>();
            tree.insertList(values.begin(), values.end());

            std::cout << measureExecutionTime([&]() {
                auto data = tree.traverseInOrder();
            }) << '\n';
        } else {
            std::cerr << "Unknown operation\n";
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Unknown tree type\n";
        return EXIT_FAILURE;
    }
}
