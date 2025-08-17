#include "SetAVL.h"
#include <cassert>
#include <iostream>
#include <string>
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
    bool operator==(const ComplexKey& other) const {
        return x == other.x && y == other.y;
    }
};

std::vector<int> GenerateRandomVector(size_t size, int min_val, int max_val, unsigned seed = 42) {
    std::vector<int> result;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(min_val, max_val);
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result.push_back(dis(gen));
    }
    return result;
}
std::vector<ComplexKey> GenerateRandomComplexKeys(size_t size, unsigned seed = 42) {
    std::vector<ComplexKey> result;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 100);
    std::vector<std::string> strings = {"a", "b", "c", "d", "e"};
    std::uniform_int_distribution<> str_dis(0, strings.size() - 1);
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result.push_back({dis(gen), strings[str_dis(gen)]});
    }
    return result;
}

void TestDefaultConstructor() {
    SetAVL<int> set_avl;
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    assert(set_avl.Begin() == set_avl.End());
    assert(set_avl.CBegin() == set_avl.CEnd());
    assert(set_avl.RBegin() == set_avl.REnd());
    assert(set_avl.CRBegin() == set_avl.CREnd());
    std::cout << "TestDefaultConstructor passed\n";
}

void TestComparatorConstructor() {
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    assert(set_avl.KeyCompare()(3, 1));
    std::cout << "TestComparatorConstructor passed\n";
}

void TestCopyConstructor() {
    auto input = GenerateRandomVector(100, -1000, 1000, 42);
    SetAVL<int> original;
    for (int val : input) {
        original.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    SetAVL<int> copy(original);
    assert(copy.Size() == sorted_unique.size());
    auto it = copy.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != copy.End());
        assert(*it == sorted_unique[i]);
        assert(copy.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == copy.End());
    assert(original.Size() == sorted_unique.size());
    std::cout << "TestCopyConstructor passed\n";
}

void TestCopyAssignment() {
    auto input = GenerateRandomVector(100, -1000, 1000, 43);
    SetAVL<int> original;
    for (int val : input) {
        original.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    SetAVL<int> copy;
    copy = original;
    assert(copy.Size() == sorted_unique.size());
    auto it = copy.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != copy.End());
        assert(*it == sorted_unique[i]);
        assert(copy.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == copy.End());
    assert(original.Size() == sorted_unique.size());
    std::cout << "TestCopyAssignment passed\n";
}

void TestMoveConstructor() {
    auto input = GenerateRandomVector(100, -1000, 1000, 44);
    SetAVL<int> original;
    for (int val : input) {
        original.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    SetAVL<int> moved(std::move(original));
    assert(moved.Size() == sorted_unique.size());
    auto it = moved.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != moved.End());
        assert(*it == sorted_unique[i]);
        assert(moved.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == moved.End());
    assert(original.Empty());
    std::cout << "TestMoveConstructor passed\n";
}

void TestMoveAssignment() {
    auto input = GenerateRandomVector(100, -1000, 1000, 45);
    SetAVL<int> original;
    for (int val : input) {
        original.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    SetAVL<int> moved;
    moved = std::move(original);
    assert(moved.Size() == sorted_unique.size());
    auto it = moved.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != moved.End());
        assert(*it == sorted_unique[i]);
        assert(moved.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == moved.End());
    assert(original.Empty());
    std::cout << "TestMoveAssignment passed\n";
}

void TestClear() {
    auto input = GenerateRandomVector(100, -1000, 1000, 46);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    assert(set_avl.Size() > 0);
    set_avl.Clear();
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    assert(set_avl.Begin() == set_avl.End());
    std::cout << "TestClear passed\n";
}

void TestSwap() {
    auto input_a = GenerateRandomVector(50, -500, 500, 47);
    auto input_b = GenerateRandomVector(50, -500, 500, 48);
    SetAVL<int> set_a;
    SetAVL<int> set_b;
    for (int val : input_a) {
        set_a.Insert(val);
    }
    for (int val : input_b) {
        set_b.Insert(val);
    }
    std::vector<int> sorted_unique_a = input_a;
    std::vector<int> sorted_unique_b = input_b;
    std::sort(sorted_unique_a.begin(), sorted_unique_a.end());
    std::sort(sorted_unique_b.begin(), sorted_unique_b.end());
    sorted_unique_a.erase(std::unique(sorted_unique_a.begin(), sorted_unique_a.end()),
                          sorted_unique_a.end());
    sorted_unique_b.erase(std::unique(sorted_unique_b.begin(), sorted_unique_b.end()),
                          sorted_unique_b.end());

    set_a.Swap(set_b);
    assert(set_a.Size() == sorted_unique_b.size());
    assert(set_b.Size() == sorted_unique_a.size());
    auto it_a = set_a.Begin();
    for (size_t i = 0; i < sorted_unique_b.size(); ++i) {
        assert(it_a != set_a.End());
        assert(*it_a == sorted_unique_b[i]);
        ++it_a;
    }
    auto it_b = set_b.Begin();
    for (size_t i = 0; i < sorted_unique_a.size(); ++i) {
        assert(it_b != set_b.End());
        assert(*it_b == sorted_unique_a[i]);
        ++it_b;
    }
    std::cout << "TestSwap passed\n";
}

void TestInsertConstLValue() {
    auto input = GenerateRandomVector(100, -1000, 1000, 49);
    SetAVL<int> set_avl;
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (int val : input) {
        auto result = set_avl.Insert(val);
        assert(*result.first == val);
    }
    assert(set_avl.Size() == sorted_unique.size());
    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(*it == sorted_unique[i]);
        auto duplicate = set_avl.Insert(sorted_unique[i]);
        assert(!duplicate.second);
        assert(*duplicate.first == sorted_unique[i]);
        ++it;
    }
    assert(set_avl.Size() == sorted_unique.size());
    std::cout << "TestInsertConstLValue passed\n";
}

void TestInsertRValue() {
    SetAVL<std::string> set_avl;
    std::vector<std::string> input = {"apple", "banana", "cherry", "apple", "date"};
    std::vector<std::string> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (auto& val : input) {
        auto result = set_avl.Insert(std::string(val));
        assert(*result.first == val);
    }
    assert(set_avl.Size() == sorted_unique.size());
    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(*it == sorted_unique[i]);
        ++it;
    }
    std::cout << "TestInsertRValue passed\n";
}

void TestInsertEmplace() {
    SetAVL<std::pair<int, std::string>> set_avl;
    auto input = GenerateRandomVector(50, 0, 100, 50);
    std::vector<std::pair<int, std::string>> pairs;
    std::vector<std::string> strings = {"x", "y", "z"};
    for (int val : input) {
        pairs.emplace_back(val, strings[val % strings.size()]);
    }
    std::vector<std::pair<int, std::string>> sorted_unique = pairs;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (const auto& p : pairs) {
        auto result = set_avl.Insert(std::make_pair(p.first, p.second));
        assert(result.first->first == p.first);
        assert(result.first->second == p.second);
    }
    assert(set_avl.Size() == sorted_unique.size());
    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i].first);
        assert(it->second == sorted_unique[i].second);
        ++it;
    }
    std::cout << "TestInsertEmplace passed\n";
}

void TestInsertRange() {
    auto input = GenerateRandomVector(100, -1000, 1000, 51);
    SetAVL<int> set_avl;
    set_avl.Insert(input.begin(), input.end());
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(set_avl.Size() == sorted_unique.size());
    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(*it == sorted_unique[i]);
        assert(set_avl.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == set_avl.End());
    std::cout << "TestInsertRange passed\n";
}

void TestInsertInitializerList() {
    auto input = GenerateRandomVector(50, -500, 500, 52);
    SetAVL<int> set_avl;
    set_avl.Insert(input.begin(), input.end());

    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(set_avl.Size() == sorted_unique.size());
    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(*it == sorted_unique[i]);
        ++it;
    }
    std::cout << "TestInsertInitializerList passed\n";
}

void TestFind() {
    auto input = GenerateRandomVector(100, -1000, 1000, 53);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_set = set_avl;
    for (int val : sorted_unique) {
        auto it = set_avl.Find(val);
        assert(it != set_avl.End());
        assert(*it == val);
        auto const_it = const_set.Find(val);
        assert(const_it != const_set.CEnd());
        assert(*const_it == val);
    }
    std::mt19937 gen(53);
    std::uniform_int_distribution<> dis(1001, 2000);
    for (int i = 0; i < 10; ++i) {
        int absent = dis(gen);
        assert(set_avl.Find(absent) == set_avl.End());
        assert(const_set.Find(absent) == const_set.CEnd());
    }
    std::cout << "TestFind passed\n";
}

void TestLowerBound() {
    auto input = GenerateRandomVector(100, -1000, 1000, 54);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_set = set_avl;
    std::mt19937 gen(54);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto it = set_avl.LowerBound(key);
        auto expected = std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key);
        if (expected == sorted_unique.end()) {
            assert(it == set_avl.End());
        } else {
            assert(it != set_avl.End());
            assert(*it == *expected);
        }
        auto const_it = const_set.LowerBound(key);
        if (expected == sorted_unique.end()) {
            assert(const_it == const_set.CEnd());
        } else {
            assert(const_it != const_set.CEnd());
            assert(*const_it == *expected);
        }
    }
    std::cout << "TestLowerBound passed\n";
}

void TestUpperBound() {
    auto input = GenerateRandomVector(100, -1000, 1000, 55);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_set = set_avl;
    std::mt19937 gen(55);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto it = set_avl.UpperBound(key);
        auto expected = std::upper_bound(sorted_unique.begin(), sorted_unique.end(), key);
        if (expected == sorted_unique.end()) {
            assert(it == set_avl.End());
        } else {
            assert(it != set_avl.End());
            assert(*it == *expected);
        }
        auto const_it = const_set.UpperBound(key);
        if (expected == sorted_unique.end()) {
            assert(const_it == const_set.CEnd());
        } else {
            assert(const_it != const_set.CEnd());
            assert(*const_it == *expected);
        }
    }
    std::cout << "TestUpperBound passed\n";
}

void TestEqualRange() {
    auto input = GenerateRandomVector(100, -1000, 1000, 56);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_set = set_avl;
    std::mt19937 gen(56);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto range = set_avl.EqualRange(key);
        auto expected = std::equal_range(sorted_unique.begin(), sorted_unique.end(), key);
        if (expected.first == expected.second) {
            assert(range.first == range.second);
            auto lb = std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key);
            if (lb == sorted_unique.end()) {
                assert(range.first == set_avl.End());
            } else {
                assert(range.first != set_avl.End());
                assert(*range.first == *lb);
            }
        } else {
            assert(range.first != range.second);
            assert(*range.first == *expected.first);
            assert(*range.second == *expected.second);
        }
        auto const_range = const_set.EqualRange(key);
        if (expected.first == expected.second) {
            assert(const_range.first == const_range.second);
        } else {
            assert(const_range.first != const_range.second);
            assert(*const_range.first == *expected.first);
            assert(*const_range.second == *expected.second);
        }
    }
    std::cout << "TestEqualRange passed\n";
}

void TestContainsAndCount() {
    auto input = GenerateRandomVector(100, -1000, 1000, 57);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (int val : sorted_unique) {
        assert(set_avl.Contains(val));
        assert(set_avl.Count(val) == 1);
    }
    std::mt19937 gen(57);
    std::uniform_int_distribution<> dis(1001, 2000);
    for (int i = 0; i < 10; ++i) {
        int absent = dis(gen);
        assert(!set_avl.Contains(absent));
        assert(set_avl.Count(absent) == 0);
    }
    std::cout << "TestContainsAndCount passed\n";
}

void TestIterators() {
    auto input = GenerateRandomVector(100, -1000, 1000, 58);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != set_avl.End());
        assert(*it == sorted_unique[i]);
        ++it;
    }
    assert(it == set_avl.End());

    auto rit = set_avl.RBegin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(rit != set_avl.REnd());
        assert(*rit == sorted_unique[sorted_unique.size() - 1 - i]);
        ++rit;
    }
    assert(rit == set_avl.REnd());
    std::cout << "TestIterators passed\n";
}

void TestSizeEmptyMaxSize() {
    auto input = GenerateRandomVector(100, -1000, 1000, 59);
    SetAVL<int> set_avl;
    assert(set_avl.Empty());
    assert(set_avl.Size() == 0);
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(!set_avl.Empty());
    assert(set_avl.Size() == sorted_unique.size());
    assert(set_avl.MaxSize() > 0);
    std::cout << "TestSizeEmptyMaxSize passed\n";
}

void TestKeyCompare() {
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    auto comp = set_avl.KeyCompare();
    auto input = GenerateRandomVector(50, -500, 500, 60);
    std::sort(input.begin(), input.end(), std::greater<int>());
    std::vector<int> sorted_unique = input;
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (size_t i = 1; i < sorted_unique.size(); ++i) {
        assert(comp(sorted_unique[i - 1], sorted_unique[i]));
        assert(!comp(sorted_unique[i], sorted_unique[i - 1]));
    }
    std::cout << "TestKeyCompare passed\n";
}

void TestOrderStatisticsBasic() {
    auto input = GenerateRandomVector(100, -1000, 1000, 61);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_set = set_avl;
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        auto it0 = set_avl.SelectInd0(i);
        assert(*it0 == sorted_unique[i]);
        auto it1 = set_avl.SelectInd1(i + 1);
        assert(*it1 == sorted_unique[i]);
        auto cit0 = const_set.SelectInd0(i);
        assert(*cit0 == sorted_unique[i]);
        auto cit1 = const_set.SelectInd1(i + 1);
        assert(*cit1 == sorted_unique[i]);
        assert(set_avl.RankInd0(sorted_unique[i]) == i);
        assert(set_avl.RankInd1(sorted_unique[i]) == i + 1);
    }
    std::mt19937 gen(61);
    std::uniform_int_distribution<> dis(1001, 2000);
    for (int i = 0; i < 10; ++i) {
        int key = dis(gen);
        auto lb = std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key);
        size_t expected_rank = lb - sorted_unique.begin();
        assert(set_avl.RankInd0(key) == expected_rank);
        assert(set_avl.RankInd1(key) == expected_rank + 1);
    }
    std::cout << "TestOrderStatisticsBasic passed\n";
}

void TestWithCustomCompare() {
    auto input = GenerateRandomVector(100, -1000, 1000, 62);
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end(), std::greater<int>());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != set_avl.End());
        assert(*it == sorted_unique[i]);
        ++it;
    }
    assert(it == set_avl.End());

    std::mt19937 gen(62);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto lb = set_avl.LowerBound(key);
        auto expected =
            std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected == sorted_unique.end()) {
            assert(lb == set_avl.End());
        } else {
            assert(lb != set_avl.End());
            assert(*lb == *expected);
        }
        auto ub = set_avl.UpperBound(key);
        auto expected_ub =
            std::upper_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected_ub == sorted_unique.end()) {
            assert(ub == set_avl.End());
        } else {
            assert(ub != set_avl.End());
            assert(*ub == *expected_ub);
        }
    }
    std::cout << "TestWithCustomCompare passed\n";
}

void TestWithComplexKeys() {
    auto input = GenerateRandomComplexKeys(100, 63);
    SetAVL<ComplexKey> set_avl;
    for (const auto& val : input) {
        set_avl.Insert(val);
    }
    std::vector<ComplexKey> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(set_avl.Size() == sorted_unique.size());
    auto it = set_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != set_avl.End());
        assert(it->x == sorted_unique[i].x && it->y == sorted_unique[i].y);
        assert(set_avl.Contains(sorted_unique[i]));
        assert(set_avl.RankInd0(sorted_unique[i]) == i);
        ++it;
    }
    assert(it == set_avl.End());
    std::cout << "TestWithComplexKeys passed\n";
}

void TestComplexTreesAndInsertionOrders() {
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
    std::cout << "TestComplexTreesAndInsertionOrders passed\n";
}

void TestLargeTree() {
    SetAVL<int> large_set;
    const size_t large_size = 10000;
    auto input = GenerateRandomVector(large_size, -10000, 10000, 65);
    for (int val : input) {
        large_set.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(large_set.Size() == sorted_unique.size());
    auto it = large_set.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(*it == sorted_unique[i]);
        assert(large_set.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == large_set.End());
    for (size_t i = 0; i < 100; ++i) {
        size_t idx = i * (sorted_unique.size() / 100);
        assert(*large_set.SelectInd0(idx) == sorted_unique[idx]);
        assert(*large_set.SelectInd1(idx + 1) == sorted_unique[idx]);
        assert(large_set.RankInd0(sorted_unique[idx]) == idx);
        assert(large_set.RankInd1(sorted_unique[idx]) == idx + 1);
    }
    std::cout << "TestLargeTree passed\n";
}

void TestEdgeCases() {
    SetAVL<int> set_avl;
    auto input = GenerateRandomVector(10, 1, 1, 66);  // All duplicates
    for (size_t i = 0; i < input.size(); ++i) {
        auto res = set_avl.Insert(input[i]);
        if (i == 0) {
            assert(res.second);
        } else {
            assert(!res.second);
        }
    }
    assert(set_avl.Size() == 1);

    SetAVL<int> empty;
    assert(empty.LowerBound(0) == empty.End());
    assert(empty.UpperBound(0) == empty.End());
    auto range = empty.EqualRange(0);
    assert(range.first == range.second);

    set_avl.Clear();
    set_avl.Insert(1);
    assert(set_avl.RankInd0(0) == 0);
    assert(set_avl.RankInd1(0) == 1);
    assert(set_avl.RankInd0(2) == 1);
    assert(set_avl.RankInd1(2) == 2);

    assert(set_avl.MaxSize() > 0);

    SetAVL<int> empty_swap;
    set_avl.Swap(empty_swap);
    assert(set_avl.Empty());
    assert(empty_swap.Size() == 1);
    std::cout << "TestEdgeCases passed\n";
}

void TestOrderStatisticsWithCustomCompare() {
    auto input = GenerateRandomVector(100, -1000, 1000, 67);
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end(), std::greater<int>());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_set = set_avl;
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        auto it0 = set_avl.SelectInd0(i);
        assert(*it0 == sorted_unique[i]);
        auto it1 = set_avl.SelectInd1(i + 1);
        assert(*it1 == sorted_unique[i]);
        auto cit0 = const_set.SelectInd0(i);
        assert(*cit0 == sorted_unique[i]);
        auto cit1 = const_set.SelectInd1(i + 1);
        assert(*cit1 == sorted_unique[i]);
        assert(set_avl.RankInd0(sorted_unique[i]) == i);
        assert(set_avl.RankInd1(sorted_unique[i]) == i + 1);
    }
    std::cout << "TestOrderStatisticsWithCustomCompare passed\n";
}

void TestBoundsWithCustomCompare() {
    auto input = GenerateRandomVector(100, -1000, 1000, 68);
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end(), std::greater<int>());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    std::mt19937 gen(68);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto lb = set_avl.LowerBound(key);
        auto expected =
            std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected == sorted_unique.end()) {
            assert(lb == set_avl.End());
        } else {
            assert(lb != set_avl.End());
            assert(*lb == *expected);
        }
        auto ub = set_avl.UpperBound(key);
        auto expected_ub =
            std::upper_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected_ub == sorted_unique.end()) {
            assert(ub == set_avl.End());
        } else {
            assert(ub != set_avl.End());
            assert(*ub == *expected_ub);
        }
        auto range = set_avl.EqualRange(key);
        auto expected_range =
            std::equal_range(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected_range.first == expected_range.second) {
            assert(range.first == range.second);
            if (expected == sorted_unique.end()) {
                assert(range.first == set_avl.End());
            } else {
                assert(range.first != set_avl.End());
                assert(*range.first == *expected);
            }
        } else {
            assert(range.first != range.second);
            assert(*range.first == *expected_range.first);
            assert(*range.second == *expected_range.second);
        }
    }
    std::cout << "TestBoundsWithCustomCompare passed\n";
}

void TestConsistency() {
    auto input = GenerateRandomVector(100, -1000, 1000, 69);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    size_t sz = set_avl.Size();
    auto expected_it = set_avl.Begin();
    for (size_t i = 0; i < sz; ++i) {
        auto selected0 = set_avl.SelectInd0(i);
        assert(*selected0 == *expected_it);
        auto selected1 = set_avl.SelectInd1(i + 1);
        assert(*selected1 == *expected_it);
        assert(set_avl.RankInd0(*expected_it) == i);
        assert(set_avl.RankInd1(*expected_it) == i + 1);
        ++expected_it;
    }
    std::cout << "TestConsistency passed\n";
}

void TestConsistencyWithCustomCompare() {
    auto input = GenerateRandomVector(100, -1000, 1000, 70);
    SetAVL<int, std::greater<int>> set_avl{std::greater<int>()};
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end(), std::greater<int>());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    size_t sz = set_avl.Size();
    auto expected_it = set_avl.Begin();
    for (size_t i = 0; i < sz; ++i) {
        auto selected0 = set_avl.SelectInd0(i);
        assert(*selected0 == *expected_it);
        auto selected1 = set_avl.SelectInd1(i + 1);
        assert(*selected1 == *expected_it);
        assert(set_avl.RankInd0(*expected_it) == i);
        assert(set_avl.RankInd1(*expected_it) == i + 1);
        ++expected_it;
    }
    std::cout << "TestConsistencyWithCustomCompare passed\n";
}

void TestReverseIterators() {
    auto input = GenerateRandomVector(100, -1000, 1000, 71);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    auto rit = set_avl.RBegin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(rit != set_avl.REnd());
        assert(*rit == sorted_unique[sorted_unique.size() - 1 - i]);
        ++rit;
    }
    assert(rit == set_avl.REnd());

    rit = set_avl.REnd();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        --rit;
        assert(*rit == sorted_unique[i]);
    }
    assert(rit == set_avl.RBegin());

    const auto& const_set = set_avl;
    auto crit = const_set.CRBegin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(crit != const_set.CREnd());
        assert(*crit == sorted_unique[sorted_unique.size() - 1 - i]);
        ++crit;
    }
    assert(crit == const_set.CREnd());
    std::cout << "TestReverseIterators passed\n";
}

void TestSingleElement() {
    SetAVL<int> set_avl;
    set_avl.Insert(42);
    assert(set_avl.Size() == 1);
    assert(*set_avl.Begin() == 42);
    assert(++set_avl.Begin() == set_avl.End());
    assert(*set_avl.RBegin() == 42);
    assert(++set_avl.RBegin() == set_avl.REnd());
    assert(set_avl.LowerBound(42) == set_avl.Begin());
    assert(set_avl.UpperBound(42) == set_avl.End());
    assert(set_avl.LowerBound(41) == set_avl.Begin());
    assert(set_avl.UpperBound(41) == set_avl.Begin());
    assert(set_avl.LowerBound(43) == set_avl.End());
    assert(set_avl.Contains(42));
    assert(set_avl.Count(42) == 1);
    assert(set_avl.SelectInd0(0) == set_avl.Begin());
    assert(*set_avl.SelectInd0(0) == 42);
    assert(set_avl.SelectInd1(1) == set_avl.Begin());
    assert(set_avl.RankInd0(42) == 0);
    assert(set_avl.RankInd1(42) == 1);
    assert(set_avl.RankInd0(41) == 0);
    assert(set_avl.RankInd1(41) == 1);
    assert(set_avl.RankInd0(43) == 1);
    assert(set_avl.RankInd1(43) == 2);
    std::cout << "TestSingleElement passed\n";
}

void TestRanksForAbsentKeys() {
    auto input = GenerateRandomVector(100, -1000, 1000, 72);
    SetAVL<int> set_avl;
    for (int val : input) {
        set_avl.Insert(val);
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    std::mt19937 gen(72);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto lb = std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key);
        size_t expected_rank = lb - sorted_unique.begin();
        assert(set_avl.RankInd0(key) == expected_rank);
        assert(set_avl.RankInd1(key) == expected_rank + 1);
    }
    std::cout << "TestRanksForAbsentKeys passed\n";
}

void TestLogarithmicAVLHeightProperty() {
    SetAVL<int> empty_set;
    size_t height = CalcNodeHeight(empty_set.GetRootPtr());
    assert(CheckAVLHeightBound(empty_set.Size(), height));

    SetAVL<int> small_seq;
    for (int i = 1; i <= 5; ++i) {
        small_seq.Insert(i);
        height = CalcNodeHeight(small_seq.GetRootPtr());
        assert(CheckAVLHeightBound(small_seq.Size(), height));
    }

    SetAVL<int> small_rev;
    for (int i = 5; i >= 1; --i) {
        small_rev.Insert(i);
        height = CalcNodeHeight(small_rev.GetRootPtr());
        assert(CheckAVLHeightBound(small_rev.Size(), height));
    }
    SetAVL<int, std::greater<int>> custom_comp{std::greater<int>()};
    std::vector<int> values = {3, 1, 4, 1, 5, 9};
    for (int val : values) {
        custom_comp.Insert(val);
        height = CalcNodeHeight(custom_comp.GetRootPtr());
        assert(CheckAVLHeightBound(custom_comp.Size(), height));
    }
    SetAVL<ComplexKey> complex_set;
    complex_set.Insert({1, "a"});
    complex_set.Insert({1, "b"});
    complex_set.Insert({2, "a"});
    height = CalcNodeHeight(complex_set.GetRootPtr());
    assert(CheckAVLHeightBound(complex_set.Size(), height));

    SetAVL<int> large_set;
    std::vector<int> large_vals(1000);
    std::iota(large_vals.begin(), large_vals.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(large_vals.begin(), large_vals.end(), g);
    for (int val : large_vals) {
        large_set.Insert(val);
        height = CalcNodeHeight(large_set.GetRootPtr());
        assert(CheckAVLHeightBound(large_set.Size(), height));
    }

    SetAVL<int> original;
    original.Insert(1);
    original.Insert(2);
    original.Insert(3);
    SetAVL<int> copy = original;
    height = CalcNodeHeight(copy.GetRootPtr());
    assert(CheckAVLHeightBound(copy.Size(), height));

    SetAVL<int> moved = std::move(copy);
    height = CalcNodeHeight(moved.GetRootPtr());
    assert(CheckAVLHeightBound(moved.Size(), height));

    SetAVL<int> set_a;
    set_a.Insert(1);
    set_a.Insert(2);
    SetAVL<int> set_b;
    set_b.Insert(3);
    set_a.Swap(set_b);
    height = CalcNodeHeight(set_a.GetRootPtr());
    assert(CheckAVLHeightBound(set_a.Size(), height));
    height = CalcNodeHeight(set_b.GetRootPtr());
    assert(CheckAVLHeightBound(set_b.Size(), height));
    std::cout << "TestLogarithmicAVLHeightProperty passed\n";
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
    TestEdgeCases();
    TestOrderStatisticsWithCustomCompare();
    TestBoundsWithCustomCompare();
    TestConsistency();
    TestConsistencyWithCustomCompare();
    TestReverseIterators();
    TestSingleElement();
    TestRanksForAbsentKeys();
    TestLogarithmicAVLHeightProperty();

    std::cout << "\nAll tests passed";
}
