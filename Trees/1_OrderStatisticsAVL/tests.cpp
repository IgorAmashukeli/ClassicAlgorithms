#include "SetAVL.h"
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <random>

struct ComplexKey {
    int x;
    std::string y;
    bool operator<(const ComplexKey& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

void TestDefaultConstructor() {
    SetAVL<int> set_avl;
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    assert(set_avl.Begin() == set_avl.End());
    assert(set_avl.CBegin() == set_avl.CEnd());
    assert(set_avl.RBegin() == set_avl.REnd());
    assert(set_avl.CRBegin() == set_avl.CREnd());
}

void TestComparatorConstructor() {
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    assert(set_avl.KeyCompare()(3, 1));  // greater: 3 > 1
}

void TestCopyConstructor() {
    SetAVL<int> original;
    original.Insert(1);
    original.Insert(2);
    SetAVL<int> copy(original);
    assert(copy.Size() == 2);
    assert(copy.Contains(1));
    assert(copy.Contains(2));
    assert(original.Size() == 2);  // original unchanged
}

void TestCopyAssignment() {
    SetAVL<int> original;
    original.Insert(1);
    original.Insert(2);
    SetAVL<int> copy;
    copy = original;
    assert(copy.Size() == 2);
    assert(copy.Contains(1));
    assert(copy.Contains(2));
    assert(original.Size() == 2);  // original unchanged
}

void TestMoveConstructor() {
    SetAVL<int> original;
    original.Insert(1);
    original.Insert(2);
    SetAVL<int> moved(std::move(original));
    assert(moved.Size() == 2);
    assert(moved.Contains(1));
    assert(moved.Contains(2));
    assert(original.Empty());  // original moved from
}

void TestMoveAssignment() {
    SetAVL<int> original;
    original.Insert(1);
    original.Insert(2);
    SetAVL<int> moved;
    moved = std::move(original);
    assert(moved.Size() == 2);
    assert(moved.Contains(1));
    assert(moved.Contains(2));
    assert(original.Empty());  // original moved from
}

void TestClear() {
    SetAVL<int> set_avl;
    set_avl.Insert(1);
    set_avl.Insert(2);
    assert(set_avl.Size() == 2);
    set_avl.Clear();
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    assert(set_avl.Begin() == set_avl.End());
}

void TestSwap() {
    SetAVL<int> set_a;
    set_a.Insert(1);
    set_a.Insert(2);
    SetAVL<int> set_b;
    set_b.Insert(3);
    set_a.Swap(set_b);
    assert(set_a.Size() == 1);
    assert(set_a.Contains(3));
    assert(set_b.Size() == 2);
    assert(set_b.Contains(1));
    assert(set_b.Contains(2));
}

void TestInsertConstLValue() {
    SetAVL<int> set_avl;
    int key = 1;
    auto result = set_avl.Insert(key);
    assert(result.second);
    assert(*result.first == 1);
    assert(set_avl.Size() == 1);
    auto duplicate = set_avl.Insert(key);
    assert(!duplicate.second);
    assert(*duplicate.first == 1);
    assert(set_avl.Size() == 1);
}

void TestInsertRValue() {
    SetAVL<std::string> set_avl;
    auto result = set_avl.Insert(std::string("hello"));
    assert(result.second);
    assert(*result.first == "hello");
    assert(set_avl.Size() == 1);
}

void TestInsertEmplace() {
    SetAVL<std::pair<int, std::string>> set_avl;
    auto result = set_avl.Insert(std::make_pair(1, "one"));
    assert(result.second);
    assert(result.first->first == 1);
    assert(result.first->second == "one");
}

void TestInsertRange() {
    SetAVL<int> set_avl;
    std::vector<int> values = {3, 1, 4, 1, 5};
    set_avl.Insert(values.begin(), values.end());
    assert(set_avl.Size() == 4);  // duplicates ignored
    assert(set_avl.Contains(1));
    assert(set_avl.Contains(3));
    assert(set_avl.Contains(4));
    assert(set_avl.Contains(5));
}

void TestInsertInitializerList() {
    SetAVL<int> set_avl;
    set_avl.Insert({1, 2, 2, 3});
    assert(set_avl.Size() == 3);
    assert(set_avl.Contains(1));
    assert(set_avl.Contains(2));
    assert(set_avl.Contains(3));
}

void TestFind() {
    SetAVL<int> set_avl;
    set_avl.Insert(1);
    set_avl.Insert(3);
    auto it = set_avl.Find(1);
    assert(it != set_avl.End());
    assert(*it == 1);
    auto const_it = static_cast<const SetAVL<int>&>(set_avl).Find(3);
    assert(const_it != set_avl.CEnd());
    assert(*const_it == 3);
    assert(set_avl.Find(2) == set_avl.End());
}

void TestLowerBound() {
    SetAVL<int> set_avl;
    set_avl.Insert(1);
    set_avl.Insert(3);
    set_avl.Insert(5);
    auto it = set_avl.LowerBound(2);
    assert(it != set_avl.End());
    assert(*it == 3);
    auto const_it = static_cast<const SetAVL<int>&>(set_avl).LowerBound(5);
    assert(*const_it == 5);
    assert(set_avl.LowerBound(6) == set_avl.End());
}

void TestUpperBound() {
    SetAVL<int> set_avl;
    set_avl.Insert(1);
    set_avl.Insert(3);
    set_avl.Insert(5);
    auto it = set_avl.UpperBound(3);
    assert(it != set_avl.End());
    assert(*it == 5);
    auto const_it = static_cast<const SetAVL<int>&>(set_avl).UpperBound(1);
    assert(*const_it == 3);
    assert(set_avl.UpperBound(5) == set_avl.End());
}

void TestEqualRange() {
    SetAVL<int> set_avl;
    set_avl.Insert(1);
    set_avl.Insert(2);
    set_avl.Insert(2);  // duplicate ignored
    set_avl.Insert(3);
    auto range = set_avl.EqualRange(2);
    assert(range.first != set_avl.End());
    assert(*range.first == 2);
    assert(range.second != set_avl.End());
    assert(*range.second == 3);
    auto const_range = static_cast<const SetAVL<int>&>(set_avl).EqualRange(4);
    assert(const_range.first == const_range.second);
    assert(const_range.first == set_avl.End());
}

void TestContainsAndCount() {
    SetAVL<int> set_avl;
    set_avl.Insert(1);
    assert(set_avl.Contains(1));
    assert(set_avl.Count(1) == 1);
    assert(!set_avl.Contains(2));
    assert(set_avl.Count(2) == 0);
}

void TestIterators() {
    SetAVL<int> set_avl;
    set_avl.Insert(3);
    set_avl.Insert(1);
    set_avl.Insert(2);
    auto it = set_avl.Begin();
    assert(*it == 1);
    ++it;
    assert(*it == 2);
    ++it;
    assert(*it == 3);
    ++it;
    assert(it == set_avl.End());

    auto rit = set_avl.RBegin();
    assert(*rit == 3);
    ++rit;
    assert(*rit == 2);
    ++rit;
    assert(*rit == 1);
    ++rit;
    assert(rit == set_avl.REnd());
}

void TestSizeEmptyMaxSize() {
    SetAVL<int> set_avl;
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    set_avl.Insert(1);
    assert(!set_avl.Empty());
    assert(set_avl.Size() == 1);
    // MaxSize is implementation-defined, just call it
    assert(set_avl.MaxSize() > 0);
}

void TestKeyCompare() {
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    auto comp = set_avl.KeyCompare();
    assert(comp(3, 1));  // 3 > 1
    assert(!comp(1, 3));
}

void TestOrderStatisticsBasic() {
    SetAVL<int> set_avl;
    set_avl.Insert(3);
    set_avl.Insert(1);
    set_avl.Insert(5);
    set_avl.Insert(2);
    set_avl.Insert(4);

    // Assuming SelectInd0 is 0-based select
    auto it0 = set_avl.SelectInd0(0);
    assert(*it0 == 1);
    it0 = set_avl.SelectInd0(2);
    assert(*it0 == 3);
    it0 = set_avl.SelectInd0(4);
    assert(*it0 == 5);

    // Assuming SelectInd1 is 1-based select
    auto it1 = set_avl.SelectInd1(1);
    assert(*it1 == 1);
    it1 = set_avl.SelectInd1(3);
    assert(*it1 == 3);
    it1 = set_avl.SelectInd1(5);
    assert(*it1 == 5);

    // Const versions
    const auto& const_set = set_avl;
    auto cit0 = const_set.SelectInd0(1);
    assert(*cit0 == 2);
    auto cit1 = const_set.SelectInd1(4);
    assert(*cit1 == 4);

    // Assuming RankInd0 is 0-based rank (number of elements < key)
    assert(set_avl.RankInd0(3) == 2);  // 1,2 < 3
    assert(set_avl.RankInd0(1) == 0);
    assert(set_avl.RankInd0(6) == 5);  // would be inserted at end

    // Assuming RankInd1 is 1-based rank (position starting from 1)
    assert(set_avl.RankInd1(3) == 3);  // position 3 (1-based: 1,2,3,4,5)
    assert(set_avl.RankInd1(1) == 1);
    assert(set_avl.RankInd1(6) == 6);  // would be 6th
}

void TestWithCustomCompare() {
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    set_avl.Insert(1);
    set_avl.Insert(3);
    set_avl.Insert(2);
    auto it = set_avl.Begin();
    assert(*it == 3);  // largest first
    ++it;
    assert(*it == 2);
    ++it;
    assert(*it == 1);
    // With greater comparator, the order is descending.
    // lower_bound returns the first element not less than key according to the comparator.
    auto lb = set_avl.LowerBound(2);
    assert(*lb == 2);
    auto ub = set_avl.UpperBound(2);
    assert(*ub == 1);
}

void TestWithComplexKeys() {
    SetAVL<ComplexKey> set_avl;
    set_avl.Insert({1, "a"});
    set_avl.Insert({1, "b"});
    set_avl.Insert({2, "a"});
    assert(set_avl.Size() == 3);
    auto it = set_avl.Begin();
    assert(it->x == 1 && it->y == "a");
    ++it;
    assert(it->x == 1 && it->y == "b");
    ++it;
    assert(it->x == 2 && it->y == "a");
    assert(set_avl.Contains({1, "b"}));
    assert(!set_avl.Contains({1, "c"}));
    assert(set_avl.RankInd0({2, "a"}) == 2);
}

void TestComplexTreesAndInsertionOrders() {
    // Sorted insertion (should trigger AVL rotations)
    SetAVL<int> ascending;
    for (int i = 1; i <= 10; ++i) {
        ascending.Insert(i);
    }

    assert(ascending.Size() == 10);
    auto it_asc = ascending.Begin();
    for (int i = 1; i <= 10; ++i) {
        assert(*it_asc == i);
        ++it_asc;
    }

    // Reverse sorted
    SetAVL<int> descending;
    for (int i = 10; i >= 1; --i) {
        descending.Insert(i);
    }
    assert(descending.Size() == 10);
    auto it_desc = descending.Begin();
    for (int i = 1; i <= 10; ++i) {
        assert(*it_desc == i);
        ++it_desc;
    }

    // Random order
    std::vector<int> random_vals = {5, 3, 7, 2, 4, 6, 8, 1, 9, 10};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(random_vals.begin(), random_vals.end(), g);
    SetAVL<int> random_set;
    for (int val : random_vals) {
        random_set.Insert(val);
    }
    assert(random_set.Size() == 10);
    auto it_rand = random_set.Begin();
    for (int i = 1; i <= 10; ++i) {
        assert(*it_rand == i);
        ++it_rand;
    }
}

void TestLargeTree() {
    SetAVL<int> large_set;
    const size_t large_size = 10000;
    for (size_t i = 0; i < large_size; ++i) {
        large_set.Insert(static_cast<int>(i));
    }
    assert(large_set.Size() == large_size);
    assert(large_set.Contains(0));
    assert(large_set.Contains(9999));
    assert(!large_set.Contains(10000));
    auto it = large_set.Begin();
    for (size_t i = 0; i < large_size; ++i) {
        assert(*it == static_cast<int>(i));
        ++it;
    }
    assert(it == large_set.End());
    // Test order statistics on large tree
    assert(*large_set.SelectInd0(0) == 0);
    assert(*large_set.SelectInd0(5000) == 5000);
    assert(*large_set.SelectInd0(9999) == 9999);
    assert(large_set.RankInd0(5000) == 5000);
    assert(large_set.RankInd0(10000) == 10000);
}

void TestEdgeCases() {
    SetAVL<int> set_avl;
    // Insert duplicate many times
    for (int i = 0; i < 10; ++i) {
        auto res = set_avl.Insert(1);
        if (i == 0) {
            assert(res.second);
        } else {
            assert(!res.second);
        }
    }
    assert(set_avl.Size() == 1);

    // Select/Rank on empty
    SetAVL<int> empty;
    // Assuming Select throws or returns End, but since not specified, perhaps test non-crash
    // But for safety, skip direct call if it may be undefined

    // Bounds on empty
    assert(empty.LowerBound(0) == empty.End());
    assert(empty.UpperBound(0) == empty.End());
    auto range = empty.EqualRange(0);
    assert(range.first == range.second);

    // Rank on non-existing
    set_avl.Insert(1);
    set_avl.Insert(3);
    assert(set_avl.RankInd0(2) == 1);  // position where it would be

    // MaxSize call
    assert(set_avl.MaxSize() > 0);

    // Swap with empty
    SetAVL<int> empty_swap;
    set_avl.Swap(empty_swap);
    assert(set_avl.Empty());
    assert(empty_swap.Size() == 2);
}

int main() {
    TestDefaultConstructor();
    TestComparatorConstructor();
    TestCopyConstructor();
    TestCopyAssignment();
    TestMoveConstructor();
    TestMoveAssignment();
    TestClear();
    TestSwap();
    TestInsertConstLValue();
    TestInsertRValue();
    TestInsertEmplace();
    TestInsertRange();
    TestInsertInitializerList();
    TestFind();
    TestLowerBound();
    TestUpperBound();
    TestEqualRange();
    TestContainsAndCount();
    TestIterators();
    TestSizeEmptyMaxSize();
    TestKeyCompare();
    TestOrderStatisticsBasic();
    TestWithCustomCompare();
    TestWithComplexKeys();
    TestComplexTreesAndInsertionOrders();
    TestLargeTree();

    std::cout << "All tests passed\n";
}