#include "BST.h"
#include <cassert>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <numeric>
#include <iostream>

// === Helpers ===
template <typename K, typename V>
std::string InOrderString(const BST<K, V>& tree) {
    std::ostringstream out;
    for (auto it = tree.Begin(); it != tree.End(); ++it) {
        out << it->first << ":" << it->second << " ";
    }
    return out.str();
}

template <typename K, typename V>
bool SameStructure(const BST<K, V>& a, const BST<K, V>& b) {

    std::function<bool(const Node<K, V>*, const Node<K, V>*)> dfs =
        [&](const Node<K, V>* n1, const Node<K, V>* n2) -> bool {
        if (!n1 && !n2) {
            return true;
        }
        if (!n1 || !n2) {
            return false;
        }
        if (n1->GetKey() != n2->GetKey() || n1->GetValue() != n2->GetValue()) {
            return false;
        }
        return dfs(n1->GetLeft().get(), n2->GetLeft().get()) &&
               dfs(n1->GetRight().get(), n2->GetRight().get());
    };

    return dfs(a.Root(), b.Root());
}

// === Tests ===
void TestCopyConstructor() {
    BST<int, std::string> original;
    std::vector<int> keys = {5, 2, 8, 1, 3, 7, 9};
    std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
    for (int k : keys) {
        original.Insert({k, std::to_string(k)});
    }

    BST<int, std::string> copy(original);

    assert(InOrderString(original) == InOrderString(copy));
    assert(SameStructure(original, copy));

    // Deep copy check
    original.Insert({42, "forty-two"});
    assert(InOrderString(original) != InOrderString(copy));
    assert(!SameStructure(original, copy));

    std::cout << "TestCopyConstructor passed\n";
}

void TestCopyAssignment() {
    BST<int, int> t1;
    std::vector<int> keys = {10, 5, 15, 3, 7, 12, 17};
    std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
    for (int k : keys) {
        t1.Insert({k, k});
    }

    BST<int, int> t2;
    t2 = t1;

    assert(InOrderString(t1) == InOrderString(t2));
    assert(SameStructure(t1, t2));

    // Modify t2, t1 stays unchanged
    t2.Insert({100, 100});
    assert(InOrderString(t1) != InOrderString(t2));
    assert(!SameStructure(t1, t2));

    std::cout << "TestCopyAssignment passed\n";
}

void TestEmptyTreeCopy() {
    BST<int, int> empty;
    BST<int, int> copy(empty);
    assert(InOrderString(empty) == InOrderString(copy));
    assert(SameStructure(empty, copy));
    std::cout << "TestEmptyTreeCopy passed\n";
}

void TestSingleNodeCopy() {
    BST<int, char> tree;
    tree.Insert({1, 'a'});
    BST<int, char> copy(tree);
    assert(InOrderString(tree) == InOrderString(copy));
    assert(SameStructure(tree, copy));
    std::cout << "TestSingleNodeCopy passed\n";
}

void TestLargeTree() {
    BST<int, int> t1;
    std::vector<int> keys(1000);
    std::iota(keys.begin(), keys.end(), 0);
    std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
    for (auto k : keys) {
        t1.Insert({k, k});
    }

    BST<int, int> t2(t1);
    assert(InOrderString(t1) == InOrderString(t2));
    assert(SameStructure(t1, t2));

    std::cout << "TestLargeTree passed\n";
}

int main() {
    TestCopyConstructor();
    TestCopyAssignment();
    TestEmptyTreeCopy();
    TestSingleNodeCopy();
    TestLargeTree();
    std::cout << "All tests passed!\n";
}
