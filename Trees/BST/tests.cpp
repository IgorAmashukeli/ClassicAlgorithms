#include "BST.h"
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <type_traits>
#include <vector>

// Assume BST<K, V> and Iterator are already defined in your code

void TestInsertOverloads() {
    BST<int, std::string> tree;

    // 1. lvalue insert
    std::pair<int, std::string> kv1 = {1, "one"};
    auto result1 = tree.Insert(kv1);  // should call const ValueType& overload
    assert(result1.second);
    assert(result1.first->first == 1);
    assert(result1.first->second == "one");

    // 2. rvalue insert
    auto result2 =
        tree.Insert(std::pair<int, std::string>{2, "two"});  // should call ValueType&& overload
    assert(result2.second);
    assert(result2.first->first == 2);
    assert(result2.first->second == "two");

    // 3. universal reference insert with forwarding
    auto result3 = tree.Insert(
        std::make_pair(3, std::string("three")));  // should call template <typename P> overload
    assert(result3.second);
    assert(result3.first->first == 3);
    assert(result3.first->second == "three");

    // 4. Try inserting existing key (should not insert again)
    auto result4 = tree.Insert(std::make_pair(1, "ONE"));
    assert(!result4.second);                 // already exists
    assert(result4.first->second == "one");  // unchanged

    std::cout << "TestInsertOverloads passed\n";
}

void TestInsertMoveOnly() {
    struct MoveOnly {
        std::string s;
        MoveOnly(std::string val) : s(std::move(val)) {
        }
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(const MoveOnly&) = delete;
        MoveOnly& operator=(MoveOnly&&) = default;
    };

    BST<int, MoveOnly> tree;
    tree.Insert(std::make_pair(1, MoveOnly("move_only_value")));
    assert(tree.Begin()->second.s == "move_only_value");

    std::cout << "TestInsertMoveOnly passed\n";
}

int main() {
    TestInsertOverloads();
    TestInsertMoveOnly();
    std::cout << "All tests passed!\n";
}
