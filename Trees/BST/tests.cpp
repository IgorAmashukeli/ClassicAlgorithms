#include "BST.h"
#include <cassert>
#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

// Custom value type for V
struct Book {
    std::string title_;
    int year_;
    explicit Book(std::string title, int year) : title_(std::move(title)), year_(year) {
    }
    bool operator==(const Book& other) const {
        return title_ == other.title_ && year_ == other.year_;
    }
};

template <typename K, typename V>
BST<K, V> CreateLargeBST(int size) {
    BST<K, V> bst;
    std::vector<int> indices(size);
    for (int i = 0; i < size; ++i) {
        indices[i] = i + 1;
    }
    std::mt19937 rng(42);  // Fixed seed for reproducibility
    std::shuffle(indices.begin(), indices.end(), rng);
    for (int i : indices) {
        double key = static_cast<double>(i);
        Book value("Book" + std::to_string(i), 1900 + (i % 100));
        bst.Insert({key, value});
    }
    return bst;
}

void InsertLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    assert(bst.Size() == kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        auto it = bst.Find(key);
        assert(it != bst.End());
        assert((*it).first == key);
        assert((*it).second == Book("Book" + std::to_string(i), 1900 + (i % 100)));
    }
    auto result = bst.Insert({500.0, Book("duplicate", 2000)});
    assert(!result.second);
    assert((*result.first).first == 500.0);
    assert((*result.first).second == Book("Book500", 1900 + (500 % 100)));
}

void FindLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        auto it = bst.Find(key);
        assert(it != bst.End());
        assert((*it).first == key);
    }
    auto it = bst.Find(0.0);
    assert(it == bst.End());
    it = bst.Find(1001.0);
    assert(it == bst.End());
}

void IteratorsLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    auto it = bst.Begin();
    assert((*it).first == 1.0);
    it = bst.End();
    --it;
    assert((*it).first == 1000.0);
    auto rit = bst.RBegin();
    assert((*rit).first == 1000.0);
    rit = bst.REnd();
    --rit;
    assert((*rit).first == 1.0);
}

void BoundsLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    auto it = bst.LowerBound(500.0);
    assert(it != bst.End());
    assert((*it).first == 500.0);
    it = bst.UpperBound(500.0);
    assert(it != bst.End());
    assert((*it).first == 501.0);
    it = bst.LowerBound(0.0);
    assert(it != bst.End());
    assert((*it).first == 1.0);
    it = bst.UpperBound(1000.0);
    assert(it == bst.End());
}

void EqualRangeLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    auto range = bst.EqualRange(500.0);
    assert(range.first != bst.End());
    assert((*range.first).first == 500.0);
    assert(range.second != bst.End());
    assert((*range.second).first == 501.0);
    range = bst.EqualRange(0.0);
    assert(range.first == range.second);
    range = bst.EqualRange(1001.0);
    assert(range.first == range.second);
}

void ClearLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    assert(bst.Size() == kLargeSize);
    bst.Clear();
    assert(bst.Empty());
    assert(bst.Size() == 0);
}

void SwapLargeTest() {
    constexpr int kSize1 = 500;
    constexpr int kSize2 = 1000;
    BST<double, Book> bst1 = CreateLargeBST<double, Book>(kSize1);
    BST<double, Book> bst2 = CreateLargeBST<double, Book>(kSize2);
    bst1.Swap(bst2);
    assert(bst1.Size() == kSize2);
    assert(bst2.Size() == kSize1);
    assert(bst1.Find(1000.0) != bst1.End());
    assert(bst2.Find(500.0) != bst2.End());
}

void CopyConstructorLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst1 = CreateLargeBST<double, Book>(kLargeSize);
    BST<double, Book> bst2(bst1);
    assert(bst2.Size() == kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        auto it = bst2.Find(key);
        assert(it != bst2.End());
        assert((*it).first == key);
    }
}

void CopyAssignmentLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst1 = CreateLargeBST<double, Book>(kLargeSize);
    BST<double, Book> bst2;
    bst2 = bst1;
    assert(bst2.Size() == kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        auto it = bst2.Find(key);
        assert(it != bst2.End());
        assert((*it).first == key);
    }
}

void MoveConstructorLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst1 = CreateLargeBST<double, Book>(kLargeSize);
    BST<double, Book> bst2(std::move(bst1));
    assert(bst2.Size() == kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        auto it = bst2.Find(key);
        assert(it != bst2.End());
        assert((*it).first == key);
    }
    assert(bst1.Empty());
}

void MoveAssignmentLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst1 = CreateLargeBST<double, Book>(kLargeSize);
    BST<double, Book> bst2;
    bst2 = std::move(bst1);
    assert(bst2.Size() == kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        auto it = bst2.Find(key);
        assert(it != bst2.End());
        assert((*it).first == key);
    }
    assert(bst1.Empty());
}

void ContainsCountLargeTest() {
    constexpr int kLargeSize = 1000;
    BST<double, Book> bst = CreateLargeBST<double, Book>(kLargeSize);
    for (int i = 1; i <= kLargeSize; i += 100) {
        double key = static_cast<double>(i);
        assert(bst.Contains(key));
        assert(bst.Count(key) == 1);
    }
    assert(!bst.Contains(0.0));
    assert(bst.Count(0.0) == 0);
}

class A {
public:
    A(const std::string& x) : x_(x) {
    }

    A(const A& other) {
        std::cout << "copy constructor\n";
    }
    A& operator=(const A& other) {
        std::cout << "copy assignment operator\n";
        return *this;
    }

    A(A&& other) {
        std::cout << "copy constructor\n";
    }

    A& operator=(A&& other) {
        std::cout << "move assignment operator\n";
        return *this;
    }

private:
    std::string x_;
};

void PrintVector(const std::vector<std::pair<int, int>>& vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << it->first << "\n";
    }
}

int main() {

    /*
    InsertLargeTest();
    FindLargeTest();
    IteratorsLargeTest();
    BoundsLargeTest();
    EqualRangeLargeTest();
    ClearLargeTest();
    SwapLargeTest();
    CopyConstructorLargeTest();
    CopyAssignmentLargeTest();
    MoveConstructorLargeTest();
    MoveAssignmentLargeTest();
    ContainsCountLargeTest();
    */

    std::random_device rd;
    std::mt19937 g(rd());

    for (int k = 0; k < 200; ++k) {
        BST<int, int> x;
        int n = 500;
        std::vector<std::pair<int, int>> ins_vec = {};

        for (auto i = 0; i < n; ++i) {
            ins_vec.push_back({i, i});
        }

        std::shuffle(ins_vec.begin(), ins_vec.end(), g);

        x.Insert(ins_vec.begin(), ins_vec.end());

        std::vector<int> out_vec;

        for (auto it = ins_vec.begin(); it != ins_vec.end(); ++it) {
            out_vec.push_back(it->first);
        }
        std::sort(out_vec.begin(), out_vec.end());

        auto res = x.Traverse();

        assert(out_vec == res);
    }
}