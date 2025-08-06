#include "BST.h"
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

template <typename K, typename V>
class BST;  // Предполагается, что класс BST уже определён

void InsertTest() {
    BST<int, std::string> bst;
    assert(bst.Empty());
    assert(bst.Size() == 0);

    auto result = bst.Insert({1, "one"});
    assert(result.second == true);
    assert(result.first != bst.End());
    assert((*result.first).first == 1);
    assert((*result.first).second == "one");

    result = bst.Insert({2, "two"});
    assert(result.second == true);
    assert((*result.first).first == 2);
    assert((*result.first).second == "two");

    result = bst.Insert({1, "one again"});
    assert(result.second == false);
    assert((*result.first).first == 1);
    assert((*result.first).second == "one");

    assert(bst.Size() == 2);
}

void FindTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    bst.Insert({2, "two"});
    bst.Insert({3, "three"});

    auto it = bst.Find(2);
    assert(it != bst.End());
    assert((*it).first == 2);
    assert((*it).second == "two");

    it = bst.Find(4);
    assert(it == bst.End());

    const BST<int, std::string>& const_bst = bst;
    auto cit = const_bst.Find(2);
    assert(cit != const_bst.End());
    assert((*cit).first == 2);
    assert((*cit).second == "two");

    cit = const_bst.Find(4);
    assert(cit == const_bst.End());
}

void BeginEndTest() {
    BST<int, std::string> bst;
    bst.Insert({3, "three"});
    bst.Insert({1, "one"});
    bst.Insert({2, "two"});
    bst.Insert({4, "four"});

    std::vector<int> keys;
    for (auto it = bst.Begin(); it != bst.End(); ++it) {
        keys.push_back((*it).first);
    }
    assert(keys == std::vector<int>({1, 2, 3, 4}));

    const BST<int, std::string>& const_bst = bst;
    keys.clear();
    for (auto it = const_bst.Begin(); it != const_bst.End(); ++it) {
        keys.push_back((*it).first);
    }
    assert(keys == std::vector<int>({1, 2, 3, 4}));

    keys.clear();
    for (auto it = const_bst.CBegin(); it != const_bst.CEnd(); ++it) {
        keys.push_back((*it).first);
    }
    assert(keys == std::vector<int>({1, 2, 3, 4}));
}

void RBeginREndTest() {
    BST<int, std::string> bst;
    bst.Insert({3, "three"});
    bst.Insert({1, "one"});
    bst.Insert({2, "two"});
    bst.Insert({4, "four"});

    std::vector<int> keys;
    for (auto it = bst.RBegin(); it != bst.REnd(); ++it) {
        keys.push_back((*it).first);
    }
    assert(keys == std::vector<int>({4, 3, 2, 1}));

    const BST<int, std::string>& const_bst = bst;
    keys.clear();
    for (auto it = const_bst.RBegin(); it != const_bst.REnd(); ++it) {
        keys.push_back((*it).first);
    }
    assert(keys == std::vector<int>({4, 3, 2, 1}));

    keys.clear();
    for (auto it = const_bst.CRBegin(); it != const_bst.CREnd(); ++it) {
        keys.push_back((*it).first);
    }
    assert(keys == std::vector<int>({4, 3, 2, 1}));
}

void LowerBoundTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    bst.Insert({3, "three"});
    bst.Insert({5, "five"});

    auto it = bst.LowerBound(2);
    assert(it != bst.End());
    assert((*it).first == 3);

    it = bst.LowerBound(3);
    assert(it != bst.End());
    assert((*it).first == 3);

    it = bst.LowerBound(0);
    assert(it != bst.End());
    assert((*it).first == 1);

    it = bst.LowerBound(6);
    assert(it == bst.End());

    const BST<int, std::string>& const_bst = bst;
    auto cit = const_bst.LowerBound(2);
    assert(cit != const_bst.End());
    assert((*cit).first == 3);
}

void UpperBoundTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    bst.Insert({3, "three"});
    bst.Insert({5, "five"});

    auto it = bst.UpperBound(2);
    assert(it != bst.End());
    assert((*it).first == 3);

    it = bst.UpperBound(3);
    assert(it != bst.End());
    assert((*it).first == 5);

    it = bst.UpperBound(0);
    assert(it != bst.End());
    assert((*it).first == 1);

    it = bst.UpperBound(5);
    assert(it == bst.End());

    const BST<int, std::string>& const_bst = bst;
    auto cit = const_bst.UpperBound(2);
    assert(cit != const_bst.End());
    assert((*cit).first == 3);
}

void EqualRangeTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    bst.Insert({3, "three"});
    bst.Insert({5, "five"});

    auto range = bst.EqualRange(3);
    assert(range.first != bst.End());
    assert((*range.first).first == 3);
    assert(range.second != bst.End());
    assert((*range.second).first == 5);

    range = bst.EqualRange(4);
    assert(range.first == range.second);

    const BST<int, std::string>& const_bst = bst;
    auto crange = const_bst.EqualRange(3);
    assert(crange.first != const_bst.End());
    assert((*crange.first).first == 3);
    assert(crange.second != const_bst.End());
    assert((*crange.second).first == 5);
}

void ClearTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    bst.Insert({2, "two"});
    assert(bst.Size() == 2);
    bst.Clear();
    assert(bst.Empty());
    assert(bst.Size() == 0);
    assert(bst.Find(1) == bst.End());
}

void SwapTest() {
    BST<int, std::string> bst1;
    bst1.Insert({1, "one"});
    bst1.Insert({2, "two"});

    BST<int, std::string> bst2;
    bst2.Insert({3, "three"});

    bst1.Swap(bst2);

    assert(bst1.Size() == 1);
    assert(bst2.Size() == 2);
    assert(bst1.Find(3) != bst1.End());
    assert(bst2.Find(1) != bst2.End());
    assert(bst2.Find(2) != bst2.End());
}

void CopyConstructorTest() {
    BST<int, std::string> quarantine;
    quarantine.Insert({1, "one"});
    quarantine.Insert({2, "two"});

    BST<int, std::string> bst2(quarantine);
    assert(bst2.Size() == 2);
    assert(bst2.Find(1) != bst2.End());
    assert(bst2.Find(2) != bst2.End());

    bst2.Insert({3, "three"});
    assert(quarantine.Find(3) == quarantine.End());
}

void CopyAssignmentTest() {
    BST<int, std::string> bst1;
    bst1.Insert({1, "one"});
    bst1.Insert({2, "two"});

    BST<int, std::string> bst2;
    bst2 = bst1;
    assert(bst2.Size() == 2);
    assert(bst2.Find(1) != bst2.End());
    assert(bst2.Find(2) != bst2.End());

    bst2.Insert({3, "three"});
    assert(bst1.Find(3) == bst1.End());
}

void MoveConstructorTest() {
    BST<int, std::string> bst1;
    bst1.Insert({1, "one"});
    bst1.Insert({2, "two"});

    BST<int, std::string> bst2(std::move(bst1));
    assert(bst2.Size() == 2);
    assert(bst2.Find(1) != bst2.End());
    assert(bst2.Find(2) != bst2.End());
    assert(bst1.Empty());
}

void MoveAssignmentTest() {
    BST<int, std::string> bst1;
    bst1.Insert({1, "one"});
    bst1.Insert({2, "two"});

    BST<int, std::string> bst2;
    bst2 = std::move(bst1);
    assert(bst2.Size() == 2);
    assert(bst2.Find(1) != bst2.End());
    assert(bst2.Find(2) != bst2.End());
    assert(bst1.Empty());
}

void ContainsTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    assert(bst.Contains(1));
    assert(!bst.Contains(2));
}

void CountTest() {
    BST<int, std::string> bst;
    bst.Insert({1, "one"});
    assert(bst.Count(1) == 1);
    assert(bst.Count(2) == 0);
}

int main() {
    InsertTest();
    FindTest();
    BeginEndTest();
    RBeginREndTest();
    LowerBoundTest();
    UpperBoundTest();
    EqualRangeTest();
    ClearTest();
    SwapTest();
    CopyConstructorTest();
    CopyAssignmentTest();
    MoveConstructorTest();
    MoveAssignmentTest();
    ContainsTest();
    CountTest();

    std::cout << "All tests are passed!\n";
    return 0;
}