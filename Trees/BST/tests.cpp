#include <cassert>
#include <iostream>

// Include your SetBST implementation here
#include "SetBST.h"

// 1. Large tree insert test
void TestLargeInsert() {
    SetBST<int> s;
    const int n = 1000;
    for (int i = 0; i < n; ++i) {
        auto res = s.Insert(i);
        assert(res.second);           // inserted
        assert(*res.first == i);
    }
    assert(s.Size() == n);
    assert(!s.Empty());

    // Inserting duplicates returns false
    for (int i = 0; i < n; ++i) {
        auto res = s.Insert(i);
        assert(!res.second);
        assert(*res.first == i);
    }
}

// 2. Custom key type with tricky comparison
struct TrickyKey {
    int x, y;

    TrickyKey(int a, int b) : x(a), y(b) {}

    bool operator<(const TrickyKey& other) const {
        int sum1 = x + y;
        int sum2 = other.x + other.y;
        if (sum1 != sum2) {return sum1 < sum2;}
        if (x != other.x) {return x > other.x;} // descending
        return y < other.y;
    }

    bool operator==(const TrickyKey& other) const {
        return x == other.x && y == other.y;
    }
};

void TestTrickyKey() {
    SetBST<TrickyKey> s;

    auto res1 = s.Insert(TrickyKey(1, 2));
    assert(res1.second);
    auto res2 = s.Insert(TrickyKey(2, 1));
    assert(res2.second);
    auto res3 = s.Insert(TrickyKey(3, 0));
    assert(res3.second);

    // Insert duplicate
    auto res4 = s.Insert(TrickyKey(1, 2));
    assert(!res4.second);

    // Check size
    assert(s.Size() == 3);

    // Check find
    assert(s.Contains(TrickyKey(2, 1)));
    assert(!s.Contains(TrickyKey(0, 3)));

    // Iterate in order, verify order using operator<
    TrickyKey prev = TrickyKey(-1000, -1000);
    for (auto it = s.Begin(); it != s.End(); ++it) {
        // Elements should be strictly increasing
        assert(prev < *it);
        prev = *it;
    }
}

// 3. Test Find, LowerBound, UpperBound, EqualRange
void TestBounds() {
    SetBST<int> s;
    s.Insert({1, 3, 5, 7, 9});

    assert(s.Find(5) != s.End());
    assert(s.Find(6) == s.End());

    // LowerBound
    auto lb = s.LowerBound(4);
    assert(lb != s.End() && *lb == 5);

    lb = s.LowerBound(5);
    assert(lb != s.End() && *lb == 5);

    lb = s.LowerBound(10);
    assert(lb == s.End());

    // UpperBound
    auto ub = s.UpperBound(5);
    assert(ub != s.End() && *ub == 7);

    ub = s.UpperBound(9);
    assert(ub == s.End());

    // EqualRange (unique keys → range is one element or empty)
    auto er = s.EqualRange(3);
    assert(er.first != s.End() && er.second != s.End());
    assert(*er.first == 3);
    assert(*er.second == 5);

    auto er2 = s.EqualRange(6);
    assert(er2.first == er2.second);
}

// 4. Test swap and move semantics
void TestSwapAndMove() {
    SetBST<int> s1;
    s1.Insert(1);
    s1.Insert(2);

    SetBST<int> s2;
    s2.Insert(10);
    s2.Insert(20);

    s1.Swap(s2);
    assert(s1.Contains(10));
    assert(s2.Contains(1));

    // Move constructor
    SetBST<int> s3(std::move(s1));
    assert(s3.Contains(10));
    assert(s3.Contains(20));
    assert(s1.Empty());

    // Move assignment
    SetBST<int> s4;
    s4 = std::move(s2);
    assert(s4.Contains(1));
    assert(s4.Contains(2));
    assert(s2.Empty());
}

// 5. Test Clear and Empty
void TestClearAndEmpty() {
    SetBST<int> s;
    assert(s.Empty());
    s.Insert(42);
    assert(!s.Empty());
    s.Clear();
    assert(s.Empty());
}

int main() {
    TestLargeInsert();
    TestTrickyKey();
    TestBounds();
    TestSwapAndMove();
    TestClearAndEmpty();

    std::cout << "All tests passed!\n";
    return 0;
}
