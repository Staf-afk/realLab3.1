#include <iostream>
#include <string>
#include <vector>
#include "../include/BinaryTree.hpp"
#include "../include/Traversal.hpp"
#include "../src/BinaryTree.tpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

class BinaryTreeTests {
private:
    int passed = 0;
    int failed = 0;

    void assertEqual(int expected, int actual, const std::string& testName) {
        if (expected == actual) {
            std::cout << GREEN << "  [OK] " << RESET << testName << std::endl;
            passed++;
        } else {
            std::cout << RED << "  [FAIL] " << RESET << testName << " (ожидалось: " << expected
                      << ", получено: " << actual << ")" << std::endl;
            failed++;
        }
    }

    void assertEqual(size_t expected, size_t actual, const std::string& testName) {
        if (expected == actual) {
            std::cout << GREEN << "  [OK] " << RESET << testName << std::endl;
            passed++;
        } else {
            std::cout << RED << "  [FAIL] " << RESET << testName << " (ожидалось: " << expected
                      << ", получено: " << actual << ")" << std::endl;
            failed++;
        }
    }

    void assertEqual(const std::string& expected, const std::string& actual, const std::string& testName) {
        if (expected == actual) {
            std::cout << GREEN << "  [OK] " << RESET << testName << std::endl;
            passed++;
        } else {
            std::cout << RED << "  [FAIL] " << RESET << testName << " (ожидалось: '" << expected
                      << "', получено: '" << actual << "')" << std::endl;
            failed++;
        }
    }

    void assertTrue(bool condition, const std::string& testName) {
        if (condition) {
            std::cout << GREEN << "  [OK] " << RESET << testName << std::endl;
            passed++;
        } else {
            std::cout << RED << "  [FAIL] " << RESET << testName << std::endl;
            failed++;
        }
    }

    void printVector(const std::vector<int>& vec, const std::string& label) {
        std::cout << "    " << label << ": [";
        for (size_t i = 0; i < vec.size(); i++) {
            std::cout << vec[i];
            if (i < vec.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

public:
    void testInsertAndSize() {
        std::cout << "\n--- Тесты вставки и размера ---" << std::endl;
        
        BinaryTree<int> tree;
        assertTrue(tree.empty(), "Пустое дерево - empty() == true");
        assertEqual((size_t)0, tree.size(), "Пустое дерево - size() == 0");

        tree.insertNode(10);
        assertTrue(!tree.empty(), "После вставки - empty() == false");
        assertEqual((size_t)1, tree.size(), "После вставки - size() == 1");

        tree.insertNode(5);
        tree.insertNode(15);
        tree.insertNode(3);
        tree.insertNode(7);
        tree.insertNode(12);
        tree.insertNode(20);
        
        assertEqual((size_t)7, tree.size(), "После 7 вставок - size() == 7");
        std::cout << "    Дерево успешно построено." << std::endl;
    }

    void testTraversals() {
        std::cout << "\n--- Тесты обходов (Traversals) ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(10);
        tree.insertNode(5);
        tree.insertNode(15);
        tree.insertNode(3);
        tree.insertNode(7);
        tree.insertNode(12);

        auto preOrder = tree.traverse(Traversal::KLP);
        printVector(preOrder, "KLP (Pre-order)");
        assertEqual((size_t)6, preOrder.size(), "Обход KLP - размер");
        assertEqual(10, preOrder[0], "Обход KLP - первый (корень)");
        assertEqual(5, preOrder[1], "Обход KLP - второй (левый)");

        auto inOrder = tree.traverse(Traversal::LKP);
        printVector(inOrder, "LKP (In-order)");
        assertEqual(3, inOrder[0], "Обход LKP - первый (самый левый)");
        assertEqual(10, inOrder[3], "Обход LKP - корень в середине");

        auto postOrder = tree.traverse(Traversal::LPK);
        printVector(postOrder, "LPK (Post-order)");
        assertEqual(10, postOrder[5], "Обход LPK - последний (корень)");

        auto reverseOrder = tree.traverse(Traversal::PLK);
        printVector(reverseOrder, "PLK (Reverse Post-order)");
    }

    void testMap() {
        std::cout << "\n--- Тесты Map ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(1);
        tree.insertNode(2);
        tree.insertNode(3);

        auto mappedTree = tree.map<int>([](int x) { return x * 10; });
        auto vec = mappedTree.traverse(Traversal::KLP);
        
        printVector(vec, "Результат Map (x * 10)");
        assertEqual((size_t)3, mappedTree.size(), "Map - размер сохранился");
        assertEqual(10, vec[0], "Map - первый элемент умножен");
        assertEqual(30, vec[2], "Map - третий элемент умножен");
    }

    void testWhere() {
        std::cout << "\n--- Тесты Where ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(10);
        tree.insertNode(5);
        tree.insertNode(15);
        tree.insertNode(3);
        tree.insertNode(7);
        tree.insertNode(12);

        auto filteredTree = tree.where([](const int& x) { return x > 6; });
        auto vec = filteredTree.traverse(Traversal::KLP);
        
        printVector(vec, "Результат Where (x > 6)");
        assertEqual((size_t)4, filteredTree.size(), "Where - размер уменьшился");
        assertTrue(filteredTree.contains(10), "Where - содержит 10");
        assertTrue(filteredTree.contains(15), "Where - содержит 15");
        assertTrue(!filteredTree.contains(5), "Where - не содержит 5");
    }

    void testReduce() {
        std::cout << "\n--- Тесты Reduce ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(1);
        tree.insertNode(2);
        tree.insertNode(3);
        tree.insertNode(4);

        int sum = tree.reduce<int>(Traversal::KLP, 0, [](int acc, const int& val) {
            return acc + val;
        });

        std::cout << "    Сумма элементов дерева (1+2+3+4): " << sum << std::endl;
        assertEqual(10, sum, "Reduce - сумма элементов верна");
    }

    void testContainsAndExtract() {
        std::cout << "\n--- Тесты поиска и извлечения поддерева ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(10);
        tree.insertNode(5);
        tree.insertNode(15);
        tree.insertNode(3);
        tree.insertNode(7);
        tree.insertNode(12);

        assertTrue(tree.contains(7), "Contains(7) - true");
        assertTrue(!tree.contains(99), "Contains(99) - false");

        auto subtree = tree.extractSubtree(5);
        auto vec = subtree.traverse(Traversal::KLP);
        printVector(vec, "Извлеченное поддерево с корнем 5");
        
        assertEqual((size_t)3, subtree.size(), "ExtractSubtree(5) - размер 3");
        assertEqual(5, vec[0], "ExtractSubtree(5) - корень 5");

        try {
            tree.extractSubtree(99);
            assertTrue(false, "ExtractSubtree(99) - ожидалось исключение");
        } catch (const ElementNotFoundException&) {
            assertTrue(true, "ExtractSubtree(99) - исключение перехвачено");
        }
    }

    void testSerialize() {
        std::cout << "\n--- Тесты сериализации ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(10);
        tree.insertNode(5);
        tree.insertNode(15);
        tree.insertNode(3);
        tree.insertNode(7);
        tree.insertNode(12);

        std::string s = tree.serialize(Traversal::KLP);
        std::cout << "    Строка (KLP): \"" << s << "\"" << std::endl;
        
        assertTrue(!s.empty(), "Serialize - строка не пустая");

        auto restoredTree = BinaryTree<int>::deserialize(s, Traversal::KLP);
        assertEqual(tree.size(), restoredTree.size(), "Deserialize - размеры совпадают");
        
        auto vecOrig = tree.traverse(Traversal::LKP);
        auto vecRest = restoredTree.traverse(Traversal::LKP);
        assertEqual(vecOrig.size(), vecRest.size(), "Deserialize - длины обходов совпадают");
        
        if (!vecOrig.empty() && !vecRest.empty()) {
            assertEqual(vecOrig[0], vecRest[0], "Deserialize - первые элементы совпадают");
        }
    }

    void testFindByPath() {
        std::cout << "\n--- Тесты поиска по пути ---" << std::endl;
        
        BinaryTree<int> tree;
        tree.insertNode(10);
        tree.insertNode(5);
        tree.insertNode(15);
        tree.insertNode(3);
        tree.insertNode(7);
        tree.insertNode(12);

        assertEqual(5, tree.findByPath("L"), "Путь 'L' -> 5");
        assertEqual(15, tree.findByPath("R"), "Путь 'R' -> 15");
        assertEqual(3, tree.findByPath("LL"), "Путь 'LL' -> 3");
        assertEqual(7, tree.findByPath("LR"), "Путь 'LR' -> 7");
        assertEqual(12, tree.findByPath("RL"), "Путь 'RL' -> 12");

        try {
            tree.findByPath("RR");
            assertTrue(false, "Путь 'RR' - ожидалось исключение");
        } catch (const InvalidPathException&) {
            assertTrue(true, "Путь 'RR' - исключение перехвачено");
        }

        try {
            tree.findByPath("X");
            assertTrue(false, "Путь 'X' - ожидалось исключение");
        } catch (const InvalidPathException&) {
            assertTrue(true, "Путь 'X' - исключение перехвачено");
        }
    }

    void testCopyAndMove() {
        std::cout << "\n--- Тесты копирования и перемещения ---" << std::endl;
        
        BinaryTree<int> tree1;
        tree1.insertNode(10);
        tree1.insertNode(5);

        BinaryTree<int> tree2(tree1);
        assertEqual(tree1.size(), tree2.size(), "Копирование - размеры совпадают");
        assertTrue(tree2.contains(10), "Копирование - содержит 10");

        BinaryTree<int> tree3(std::move(tree1));
        assertEqual((size_t)2, tree3.size(), "Перемещение - новый размер");
        assertTrue(tree1.empty(), "Перемещение - старый объект пуст");
    }

    void runAll() {
        std::cout << "\n========================================";
        std::cout << "\nЗАПУСК ТЕСТОВ BINARY TREE";
        std::cout << "\n========================================";

        testInsertAndSize();
        testTraversals();
        testMap();
        testWhere();
        testReduce();
        testContainsAndExtract();
        testSerialize();
        testFindByPath();
        testCopyAndMove();

        std::cout << "\n--- Итог ---" << std::endl;
        if (failed == 0) {
            std::cout << GREEN << "Все тесты пройдены успешно!" << RESET << std::endl;
        } else {
            std::cout << RED << "Есть не пройденные тесты!" << RESET << std::endl;
        }
        std::cout << "Пройдено: " << passed << ", Не пройдено: " << failed << std::endl;
        std::cout << "========================================\n" << std::endl;
    }
};

int main() {
    BinaryTreeTests tests;
    tests.runAll();
    return 0;
}

