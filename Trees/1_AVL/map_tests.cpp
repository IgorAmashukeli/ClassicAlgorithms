#include "MapAVL.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

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
    MapAVL<int, int> map_avl;
    assert(map_avl.Empty());
    assert(map_avl.Size() == 0);
    assert(map_avl.Begin() == map_avl.End());
    assert(map_avl.CBegin() == map_avl.CEnd());
    assert(map_avl.RBegin() == map_avl.REnd());
    assert(map_avl.CRBegin() == map_avl.CREnd());
    std::cout << "TestDefaultConstructor passed\n";
}

void TestComparatorConstructor() {
    MapAVL<int, int, std::greater<int>> map_avl{std::greater<int>()};
    assert(map_avl.Empty());
    assert(map_avl.Size() == 0);
    assert(map_avl.KeyCompare()(3, 1));
    std::cout << "TestComparatorConstructor passed\n";
}

void TestCopyConstructor() {
    auto input = GenerateRandomVector(100, -1000, 1000, 42);
    MapAVL<int, int> original;
    for (int val : input) {
        original.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    MapAVL<int, int> copy(original);
    assert(copy.Size() == sorted_unique.size());
    auto it = copy.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != copy.End());
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        assert(copy.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == copy.End());
    assert(original.Size() == sorted_unique.size());
    std::cout << "TestCopyConstructor passed\n";
}

void TestCopyAssignment() {
    auto input = GenerateRandomVector(100, -1000, 1000, 43);
    MapAVL<int, int> original;
    for (int val : input) {
        original.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    MapAVL<int, int> copy;
    copy = original;
    assert(copy.Size() == sorted_unique.size());
    auto it = copy.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != copy.End());
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        assert(copy.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == copy.End());
    assert(original.Size() == sorted_unique.size());
    std::cout << "TestCopyAssignment passed\n";
}

void TestMoveConstructor() {
    auto input = GenerateRandomVector(100, -1000, 1000, 44);
    MapAVL<int, int> original;
    for (int val : input) {
        original.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    MapAVL<int, int> moved(std::move(original));
    assert(moved.Size() == sorted_unique.size());
    auto it = moved.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != moved.End());
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        assert(moved.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == moved.End());
    assert(original.Empty());
    std::cout << "TestMoveConstructor passed\n";
}

void TestMoveAssignment() {
    auto input = GenerateRandomVector(100, -1000, 1000, 45);
    MapAVL<int, int> original;
    for (int val : input) {
        original.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    MapAVL<int, int> moved;
    moved = std::move(original);
    assert(moved.Size() == sorted_unique.size());
    auto it = moved.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != moved.End());
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        assert(moved.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == moved.End());
    assert(original.Empty());
    std::cout << "TestMoveAssignment passed\n";
}

void TestClear() {
    auto input = GenerateRandomVector(100, -1000, 1000, 46);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    assert(map_avl.Size() > 0);
    map_avl.Clear();
    assert(map_avl.Empty());
    assert(map_avl.Size() == 0);
    assert(map_avl.Begin() == map_avl.End());
    std::cout << "TestClear passed\n";
}

void TestSwap() {
    auto input_a = GenerateRandomVector(50, -500, 500, 47);
    auto input_b = GenerateRandomVector(50, -500, 500, 48);
    MapAVL<int, int> map_a;
    MapAVL<int, int> map_b;
    for (int val : input_a) {
        map_a.Insert({val, val});
    }
    for (int val : input_b) {
        map_b.Insert({val, val});
    }
    std::vector<int> sorted_unique_a = input_a;
    std::vector<int> sorted_unique_b = input_b;
    std::sort(sorted_unique_a.begin(), sorted_unique_a.end());
    std::sort(sorted_unique_b.begin(), sorted_unique_b.end());
    sorted_unique_a.erase(std::unique(sorted_unique_a.begin(), sorted_unique_a.end()),
                          sorted_unique_a.end());
    sorted_unique_b.erase(std::unique(sorted_unique_b.begin(), sorted_unique_b.end()),
                          sorted_unique_b.end());

    map_a.Swap(map_b);
    assert(map_a.Size() == sorted_unique_b.size());
    assert(map_b.Size() == sorted_unique_a.size());
    auto it_a = map_a.Begin();
    for (size_t i = 0; i < sorted_unique_b.size(); ++i) {
        assert(it_a != map_a.End());
        assert(it_a->first == sorted_unique_b[i]);
        assert(it_a->second == sorted_unique_b[i]);
        ++it_a;
    }
    auto it_b = map_b.Begin();
    for (size_t i = 0; i < sorted_unique_a.size(); ++i) {
        assert(it_b != map_b.End());
        assert(it_b->first == sorted_unique_a[i]);
        assert(it_b->second == sorted_unique_a[i]);
        ++it_b;
    }
    std::cout << "TestSwap passed\n";
}

void TestInsertConstLValue() {
    auto input = GenerateRandomVector(100, -1000, 1000, 49);
    MapAVL<int, int> map_avl;
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (int val : input) {
        auto result = map_avl.Insert({val, val});
        assert(result.first->first == val);
        assert(result.first->second == val);
    }
    assert(map_avl.Size() == sorted_unique.size());
    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        auto duplicate = map_avl.Insert({sorted_unique[i], sorted_unique[i]});
        assert(!duplicate.second);
        assert(duplicate.first->first == sorted_unique[i]);
        assert(duplicate.first->second == sorted_unique[i]);
        ++it;
    }
    assert(map_avl.Size() == sorted_unique.size());
    std::cout << "TestInsertConstLValue passed\n";
}

void TestInsertRValue() {
    MapAVL<std::string, std::string> map_avl;
    std::vector<std::string> input = {"apple", "banana", "cherry", "apple", "date"};
    std::vector<std::string> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (auto& val : input) {
        auto result = map_avl.Insert({std::string(val), std::string(val)});
        assert(result.first->first == val);
        assert(result.first->second == val);
    }
    assert(map_avl.Size() == sorted_unique.size());
    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        ++it;
    }
    std::cout << "TestInsertRValue passed\n";
}

void TestInsertEmplace() {
    MapAVL<int, std::string> map_avl;
    auto input = GenerateRandomVector(50, 0, 100, 50);
    std::vector<std::pair<int, std::string>> pairs;
    std::vector<std::string> strings = {"x", "y", "z"};
    for (int val : input) {
        pairs.emplace_back(val, strings[val % strings.size()]);
    }
    std::vector<std::pair<int, std::string>> sorted_unique = pairs;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    auto it_unique = std::unique(sorted_unique.begin(), sorted_unique.end(),
                                 [](const auto& a, const auto& b) { return a.first == b.first; });
    sorted_unique.erase(it_unique, sorted_unique.end());

    for (const auto& p : pairs) {
        auto result = map_avl.Insert({p.first, p.second});
        assert(result.first->first == p.first);
        assert(result.first->second == p.second);
    }
    assert(map_avl.Size() == sorted_unique.size());
    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i].first);
        assert(it->second == sorted_unique[i].second);
        ++it;
    }
    std::cout << "TestInsertEmplace passed\n";
}

void TestInsertRange() {
    auto input_keys = GenerateRandomVector(100, -1000, 1000, 51);
    std::vector<std::pair<int, int>> input;
    for (int key : input_keys) {
        input.emplace_back(key, key);
    }
    MapAVL<int, int> map_avl;
    map_avl.Insert(input.begin(), input.end());
    std::vector<int> sorted_unique = input_keys;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(map_avl.Size() == sorted_unique.size());
    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        assert(map_avl.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == map_avl.End());
    std::cout << "TestInsertRange passed\n";
}

void TestInsertInitializerList() {
    auto input_keys = GenerateRandomVector(50, -500, 500, 52);
    std::vector<std::pair<int, int>> input;
    for (int key : input_keys) {
        input.emplace_back(key, key);
    }
    MapAVL<int, int> map_avl;
    map_avl.Insert(input.begin(), input.end());

    std::vector<int> sorted_unique = input_keys;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(map_avl.Size() == sorted_unique.size());
    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        ++it;
    }
    std::cout << "TestInsertInitializerList passed\n";
}

void TestFind() {
    auto input = GenerateRandomVector(100, -1000, 1000, 53);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_map = map_avl;
    for (int val : sorted_unique) {
        auto it = map_avl.Find(val);
        assert(it != map_avl.End());
        assert(it->first == val);
        assert(it->second == val);
        auto const_it = const_map.Find(val);
        assert(const_it != const_map.CEnd());
        assert(const_it->first == val);
        assert(const_it->second == val);
    }
    std::mt19937 gen(53);
    std::uniform_int_distribution<> dis(1001, 2000);
    for (int i = 0; i < 10; ++i) {
        int absent = dis(gen);
        assert(map_avl.Find(absent) == map_avl.End());
        assert(const_map.Find(absent) == const_map.CEnd());
    }
    std::cout << "TestFind passed\n";
}

void TestLowerBound() {
    auto input = GenerateRandomVector(100, -1000, 1000, 54);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_map = map_avl;
    std::mt19937 gen(54);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto it = map_avl.LowerBound(key);
        auto expected = std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key);
        if (expected == sorted_unique.end()) {
            assert(it == map_avl.End());
        } else {
            assert(it != map_avl.End());
            assert(it->first == *expected);
        }
        auto const_it = const_map.LowerBound(key);
        if (expected == sorted_unique.end()) {
            assert(const_it == const_map.CEnd());
        } else {
            assert(const_it != const_map.CEnd());
            assert(const_it->first == *expected);
        }
    }
    std::cout << "TestLowerBound passed\n";
}

void TestUpperBound() {
    auto input = GenerateRandomVector(100, -1000, 1000, 55);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_map = map_avl;
    std::mt19937 gen(55);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto it = map_avl.UpperBound(key);
        auto expected = std::upper_bound(sorted_unique.begin(), sorted_unique.end(), key);
        if (expected == sorted_unique.end()) {
            assert(it == map_avl.End());
        } else {
            assert(it != map_avl.End());
            assert(it->first == *expected);
        }
        auto const_it = const_map.UpperBound(key);
        if (expected == sorted_unique.end()) {
            assert(const_it == const_map.CEnd());
        } else {
            assert(const_it != const_map.CEnd());
            assert(const_it->first == *expected);
        }
    }
    std::cout << "TestUpperBound passed\n";
}

void TestEqualRange() {
    auto input = GenerateRandomVector(100, -1000, 1000, 56);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    const auto& const_map = map_avl;
    std::mt19937 gen(56);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto range = map_avl.EqualRange(key);
        auto expected = std::equal_range(sorted_unique.begin(), sorted_unique.end(), key);
        if (expected.first == expected.second) {
            assert(range.first == range.second);
            auto lb = std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key);
            if (lb == sorted_unique.end()) {
                assert(range.first == map_avl.End());
            } else {
                assert(range.first != map_avl.End());
                assert(range.first->first == *lb);
            }
        } else {
            assert(range.first != range.second);
            assert(range.first->first == *expected.first);
            assert(range.second->first == *expected.second);
        }
        auto const_range = const_map.EqualRange(key);
        if (expected.first == expected.second) {
            assert(const_range.first == const_range.second);
        } else {
            assert(const_range.first != const_range.second);
            assert(const_range.first->first == *expected.first);
            assert(const_range.second->first == *expected.second);
        }
    }
    std::cout << "TestEqualRange passed\n";
}

void TestContainsAndCount() {
    auto input = GenerateRandomVector(100, -1000, 1000, 57);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    for (int val : sorted_unique) {
        assert(map_avl.Contains(val));
        assert(map_avl.Count(val) == 1);
    }
    std::mt19937 gen(57);
    std::uniform_int_distribution<> dis(1001, 2000);
    for (int i = 0; i < 10; ++i) {
        int absent = dis(gen);
        assert(!map_avl.Contains(absent));
        assert(map_avl.Count(absent) == 0);
    }
    std::cout << "TestContainsAndCount passed\n";
}

void TestIterators() {
    auto input = GenerateRandomVector(100, -1000, 1000, 58);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != map_avl.End());
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        ++it;
    }
    assert(it == map_avl.End());

    auto rit = map_avl.RBegin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(rit != map_avl.REnd());
        assert(rit->first == sorted_unique[sorted_unique.size() - 1 - i]);
        assert(rit->second == sorted_unique[sorted_unique.size() - 1 - i]);
        ++rit;
    }
    assert(rit == map_avl.REnd());
    std::cout << "TestIterators passed\n";
}

void TestSizeEmptyMaxSize() {
    auto input = GenerateRandomVector(100, -1000, 1000, 59);
    MapAVL<int, int> map_avl;
    assert(map_avl.Empty());
    assert(map_avl.Size() == 0);
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(!map_avl.Empty());
    assert(map_avl.Size() == sorted_unique.size());
    assert(map_avl.MaxSize() > 0);
    std::cout << "TestSizeEmptyMaxSize passed\n";
}

void TestKeyCompare() {
    MapAVL<int, int, std::greater<int>> map_avl{std::greater<int>()};
    auto comp = map_avl.KeyCompare();
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

void TestWithCustomCompare() {
    auto input = GenerateRandomVector(100, -1000, 1000, 62);
    MapAVL<int, int, std::greater<int>> map_avl{std::greater<int>()};
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end(), std::greater<int>());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != map_avl.End());
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        ++it;
    }
    assert(it == map_avl.End());

    std::mt19937 gen(62);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto lb = map_avl.LowerBound(key);
        auto expected =
            std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected == sorted_unique.end()) {
            assert(lb == map_avl.End());
        } else {
            assert(lb != map_avl.End());
            assert(lb->first == *expected);
        }
        auto ub = map_avl.UpperBound(key);
        auto expected_ub =
            std::upper_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected_ub == sorted_unique.end()) {
            assert(ub == map_avl.End());
        } else {
            assert(ub != map_avl.End());
            assert(ub->first == *expected_ub);
        }
    }
    std::cout << "TestWithCustomCompare passed\n";
}

void TestWithComplexKeys() {
    auto input = GenerateRandomComplexKeys(100, 63);
    MapAVL<ComplexKey, std::string> map_avl;
    for (const auto& val : input) {
        map_avl.Insert({val, val.y});
    }
    std::vector<ComplexKey> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(map_avl.Size() == sorted_unique.size());
    auto it = map_avl.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it != map_avl.End());
        assert(it->first.x == sorted_unique[i].x && it->first.y == sorted_unique[i].y);
        assert(it->second == sorted_unique[i].y);
        assert(map_avl.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == map_avl.End());
    std::cout << "TestWithComplexKeys passed\n";
}

void TestComplexTreesAndInsertionOrders() {
    MapAVL<int, int> ascending;
    for (int i = 1; i <= 10; ++i) {
        ascending.Insert({i, i});
    }

    assert(ascending.Size() == 10);
    auto it_asc = ascending.Begin();
    for (int i = 1; i <= 10; ++i) {
        assert(it_asc->first == i);
        assert(it_asc->second == i);
        ++it_asc;
    }
    MapAVL<int, int> descending;
    for (int i = 10; i >= 1; --i) {
        descending.Insert({i, i});
    }
    assert(descending.Size() == 10);
    auto it_desc = descending.Begin();
    for (int i = 1; i <= 10; ++i) {
        assert(it_desc->first == i);
        assert(it_desc->second == i);
        ++it_desc;
    }

    std::vector<int> random_vals = {5, 3, 7, 2, 4, 6, 8, 1, 9, 10};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(random_vals.begin(), random_vals.end(), g);
    MapAVL<int, int> random_map;
    for (int val : random_vals) {
        random_map.Insert({val, val});
    }
    assert(random_map.Size() == 10);
    auto it_rand = random_map.Begin();
    for (int i = 1; i <= 10; ++i) {
        assert(it_rand->first == i);
        assert(it_rand->second == i);
        ++it_rand;
    }
    std::cout << "TestComplexTreesAndInsertionOrders passed\n";
}

void TestLargeTree() {
    MapAVL<int, int> large_map;
    const size_t large_size = 10000;
    auto input = GenerateRandomVector(large_size, -10000, 10000, 65);
    for (int val : input) {
        large_map.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    assert(large_map.Size() == sorted_unique.size());
    auto it = large_map.Begin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(it->first == sorted_unique[i]);
        assert(it->second == sorted_unique[i]);
        assert(large_map.Contains(sorted_unique[i]));
        ++it;
    }
    assert(it == large_map.End());
    for (size_t i = 0; i < 100; ++i) {
        size_t idx = i * (sorted_unique.size() / 100);
    }
    std::cout << "TestLargeTree passed\n";
}

void TestEdgeCases() {
    MapAVL<int, int> map_avl;
    auto input = GenerateRandomVector(10, 1, 1, 66);  // All duplicates
    for (size_t i = 0; i < input.size(); ++i) {
        auto res = map_avl.Insert({input[i], input[i]});
        if (i == 0) {
            assert(res.second);
        } else {
            assert(!res.second);
        }
    }
    assert(map_avl.Size() == 1);

    MapAVL<int, int> empty;
    assert(empty.LowerBound(0) == empty.End());
    assert(empty.UpperBound(0) == empty.End());
    auto range = empty.EqualRange(0);
    assert(range.first == range.second);

    map_avl.Clear();
    map_avl.Insert({1, 1});

    assert(map_avl.MaxSize() > 0);

    MapAVL<int, int> empty_swap;
    map_avl.Swap(empty_swap);
    assert(map_avl.Empty());
    assert(empty_swap.Size() == 1);
    std::cout << "TestEdgeCases passed\n";
}

void TestBoundsWithCustomCompare() {
    auto input = GenerateRandomVector(100, -1000, 1000, 68);
    MapAVL<int, int, std::greater<int>> map_avl{std::greater<int>()};
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end(), std::greater<int>());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    std::mt19937 gen(68);
    std::uniform_int_distribution<> dis(-1500, 1500);
    for (int i = 0; i < 50; ++i) {
        int key = dis(gen);
        auto lb = map_avl.LowerBound(key);
        auto expected =
            std::lower_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected == sorted_unique.end()) {
            assert(lb == map_avl.End());
        } else {
            assert(lb != map_avl.End());
            assert(lb->first == *expected);
        }
        auto ub = map_avl.UpperBound(key);
        auto expected_ub =
            std::upper_bound(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected_ub == sorted_unique.end()) {
            assert(ub == map_avl.End());
        } else {
            assert(ub != map_avl.End());
            assert(ub->first == *expected_ub);
        }
        auto range = map_avl.EqualRange(key);
        auto expected_range =
            std::equal_range(sorted_unique.begin(), sorted_unique.end(), key, std::greater<int>());
        if (expected_range.first == expected_range.second) {
            assert(range.first == range.second);
            if (expected == sorted_unique.end()) {
                assert(range.first == map_avl.End());
            } else {
                assert(range.first != map_avl.End());
                assert(range.first->first == *expected);
            }
        } else {
            assert(range.first != range.second);
            assert(range.first->first == *expected_range.first);
            assert(range.second->first == *expected_range.second);
        }
    }
    std::cout << "TestBoundsWithCustomCompare passed\n";
}

void TestReverseIterators() {
    auto input = GenerateRandomVector(100, -1000, 1000, 71);
    MapAVL<int, int> map_avl;
    for (int val : input) {
        map_avl.Insert({val, val});
    }
    std::vector<int> sorted_unique = input;
    std::sort(sorted_unique.begin(), sorted_unique.end());
    sorted_unique.erase(std::unique(sorted_unique.begin(), sorted_unique.end()),
                        sorted_unique.end());

    auto rit = map_avl.RBegin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(rit != map_avl.REnd());
        assert(rit->first == sorted_unique[sorted_unique.size() - 1 - i]);
        assert(rit->second == sorted_unique[sorted_unique.size() - 1 - i]);
        ++rit;
    }
    assert(rit == map_avl.REnd());

    rit = map_avl.REnd();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        --rit;
        assert(rit->first == sorted_unique[i]);
        assert(rit->second == sorted_unique[i]);
    }
    assert(rit == map_avl.RBegin());

    const auto& const_map = map_avl;
    auto crit = const_map.CRBegin();
    for (size_t i = 0; i < sorted_unique.size(); ++i) {
        assert(crit != const_map.CREnd());
        assert(crit->first == sorted_unique[sorted_unique.size() - 1 - i]);
        assert(crit->second == sorted_unique[sorted_unique.size() - 1 - i]);
        ++crit;
    }
    assert(crit == const_map.CREnd());
    std::cout << "TestReverseIterators passed\n";
}

void TestSingleElement() {
    MapAVL<int, int> map_avl;
    map_avl.Insert({42, 42});
    assert(map_avl.Size() == 1);
    assert(map_avl.Begin()->first == 42);
    assert(map_avl.Begin()->second == 42);
    assert(++map_avl.Begin() == map_avl.End());
    assert(map_avl.RBegin()->first == 42);
    assert(map_avl.RBegin()->second == 42);
    assert(++map_avl.RBegin() == map_avl.REnd());
    assert(map_avl.LowerBound(42) == map_avl.Begin());
    assert(map_avl.UpperBound(42) == map_avl.End());
    assert(map_avl.LowerBound(41) == map_avl.Begin());
    assert(map_avl.UpperBound(41) == map_avl.Begin());
    assert(map_avl.LowerBound(43) == map_avl.End());
    assert(map_avl.Contains(42));
    assert(map_avl.Count(42) == 1);
    std::cout << "TestSingleElement passed\n";
}

void TestOperatorEqual() {
    std::mt19937 gen(123);
    for (int trial = 0; trial < 50; ++trial) {
        auto input1 = GenerateRandomVector(100, -1000, 1000, gen());
        auto input2 = GenerateRandomVector(100, -1000, 1000, gen());

        MapAVL<int, int> map1, map2;
        std::map<int, int> stdmap1, stdmap2;

        for (int v : input1) {
            map1.Insert({v, v});
            stdmap1.insert({v, v});
        }
        for (int v : input2) {
            map2.Insert({v, v});
            stdmap2.insert({v, v});
        }

        bool expected_equal = (stdmap1 == stdmap2);

        // Cross-check
        assert((map1 == map2) == expected_equal);
        assert((map1 != map2) == !expected_equal);
    }
    std::cout << "TestOperatorEqualStress passed\n";
}

void TestOperatorNotEqual() {
    std::mt19937 gen(321);
    for (int trial = 0; trial < 50; ++trial) {
        auto input1 = GenerateRandomVector(50, -500, 500, gen());
        auto input2 = GenerateRandomVector(50, -500, 500, gen());

        MapAVL<int, int> map1, map2;
        std::map<int, int> stdmap1, stdmap2;

        for (int v : input1) {
            map1.Insert({v, v});
            stdmap1.insert({v, v});
        }
        for (int v : input2) {
            map2.Insert({v, v});
            stdmap2.insert({v, v});
        }

        bool expected_not_equal = (stdmap1 != stdmap2);

        assert((map1 != map2) == expected_not_equal);
        assert((map1 == map2) == !expected_not_equal);
    }
    std::cout << "TestOperatorNotEqualStress passed\n";
}

void TestSwapOuter() {
    std::mt19937 gen(456);
    for (int trial = 0; trial < 50; ++trial) {
        auto input1 = GenerateRandomVector(50, -500, 500, gen());
        auto input2 = GenerateRandomVector(50, -500, 500, gen());

        MapAVL<int, int> map1, map2;
        std::map<int, int> stdmap1, stdmap2;

        for (int v : input1) {
            map1.Insert({v, v});
            stdmap1.insert({v, v});
        }
        for (int v : input2) {
            map2.Insert({v, v});
            stdmap2.insert({v, v});
        }

        // Expected swap using std::map
        auto expected1 = stdmap1;
        auto expected2 = stdmap2;
        expected1.swap(expected2);

        Swap(map1, map2);

        // Compare map1 with expected1
        auto it1 = map1.Begin();
        for (auto& p : expected1) {
            assert(it1 != map1.End());
            assert(it1->first == p.first);
            assert(it1->second == p.second);
            ++it1;
        }
        assert(it1 == map1.End());

        // Compare map2 with expected2
        auto it2 = map2.Begin();
        for (auto& p : expected2) {
            assert(it2 != map2.End());
            assert(it2->first == p.first);
            assert(it2->second == p.second);
            ++it2;
        }
        assert(it2 == map2.End());
    }
    std::cout << "TestSwapStress passed\n";
}

void TestLogarithmicAVLHeightProperty() {
    MapAVL<int, int> empty_set;
    size_t height = CalcNodeHeight(empty_set.GetRootPtr());
    assert(CheckAVLHeightBound(empty_set.Size(), height));

    MapAVL<int, int> small_seq;
    for (int i = 1; i <= 5; ++i) {
        small_seq.Insert({i, i});
        height = CalcNodeHeight(small_seq.GetRootPtr());
        assert(CheckAVLHeightBound(small_seq.Size(), height));
    }

    MapAVL<int, int> small_rev;
    for (int i = 5; i >= 1; --i) {
        small_rev.Insert({i, i});
        height = CalcNodeHeight(small_rev.GetRootPtr());
        assert(CheckAVLHeightBound(small_rev.Size(), height));
    }
    MapAVL<int, int, std::greater<int>> custom_comp{std::greater<int>()};
    std::vector<int> values = {3, 1, 4, 1, 5, 9};
    for (int val : values) {
        custom_comp.Insert({val, val});
        height = CalcNodeHeight(custom_comp.GetRootPtr());
        assert(CheckAVLHeightBound(custom_comp.Size(), height));
    }
    MapAVL<ComplexKey, int> complex_map;
    complex_map.Insert({{1, "a"}, 2});
    complex_map.Insert({{1, "b"}, 5});
    complex_map.Insert({{2, "a"}, -3});
    height = CalcNodeHeight(complex_map.GetRootPtr());
    assert(CheckAVLHeightBound(complex_map.Size(), height));

    MapAVL<int, int> large_set;
    std::vector<int> large_vals(1000);
    std::iota(large_vals.begin(), large_vals.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(large_vals.begin(), large_vals.end(), g);
    for (int val : large_vals) {
        large_set.Insert({val, val});
        height = CalcNodeHeight(large_set.GetRootPtr());
        assert(CheckAVLHeightBound(large_set.Size(), height));
    }

    MapAVL<int, int> original;
    original.Insert({1, 1});
    original.Insert({2, 2});
    original.Insert({3, 3});
    MapAVL<int, int> copy = original;
    height = CalcNodeHeight(copy.GetRootPtr());
    assert(CheckAVLHeightBound(copy.Size(), height));

    MapAVL<int, int> moved = std::move(copy);
    height = CalcNodeHeight(moved.GetRootPtr());
    assert(CheckAVLHeightBound(moved.Size(), height));

    MapAVL<int, int> set_a;
    set_a.Insert({1, 1});
    set_a.Insert({2, 2});
    MapAVL<int, int> set_b;
    set_b.Insert({3, 3});
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
    TestWithCustomCompare();
    TestWithComplexKeys();
    TestComplexTreesAndInsertionOrders();
    TestLargeTree();
    TestEdgeCases();
    TestBoundsWithCustomCompare();
    TestReverseIterators();
    TestSingleElement();
    TestOperatorEqual();
    TestOperatorNotEqual();
    TestSwapOuter();
    TestLogarithmicAVLHeightProperty();

    std::cout << "\nAll tests passed\n";
}