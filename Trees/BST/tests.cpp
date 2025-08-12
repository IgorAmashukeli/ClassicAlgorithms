#include <cassert>
#include <string>
#include <iostream>
#include "BST.h"

// Custom comparator for descending order
template <typename T>
struct Greater {
    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

// Helper: build a large BST
template <typename K, typename V, typename Comp>
BST<K, V, Comp> CreateLargeBST(int size, Comp comp = Comp{}) {
    BST<K, V, Comp> bst(comp);
    for (int i = 1; i <= size; ++i) {
        bst.Insert({i, "value" + std::to_string(i)});
    }
    return bst;
}

template <typename Comp>
void RunTestsForComparator() {
    const int large_size = 5000;
    using BSTType = BST<int, std::string, Comp>;

    // Insert test
    {
        BSTType bst = CreateLargeBST<int, std::string, Comp>(large_size);
        assert(bst.Size() == large_size);
        for (int i = 1; i <= large_size; i += 500) {
            auto it = bst.Find(i);
            assert(it != bst.End());
            assert((*it).first == i);
        }
        auto result = bst.Insert({2500, "duplicate"});
        assert(result.second == false);
    }

    // Find test
    {
        BSTType bst = CreateLargeBST<int, std::string, Comp>(large_size);
        for (int i = 1; i <= large_size; i += 500) {
            assert(bst.Contains(i));
        }
        assert(!bst.Contains(0));
    }

    // Iterators test
    {
        BSTType bst = CreateLargeBST<int, std::string, Comp>(large_size);
        auto it = bst.Begin();
        auto rit = bst.RBegin();
        assert(it != bst.End());
        assert(rit != bst.REnd());
    }

    // Bounds test
    {
        BSTType bst = CreateLargeBST<int, std::string, Comp>(large_size);
        auto it = bst.LowerBound(2500);
        assert(it != bst.End());
    }

    // EqualRange test
    {
        BSTType bst = CreateLargeBST<int, std::string, Comp>(large_size);
        auto range = bst.EqualRange(2500);
        assert(range.first != bst.End());
    }

    // Clear test
    {
        BSTType bst = CreateLargeBST<int, std::string, Comp>(large_size);
        bst.Clear();
        assert(bst.Empty());
    }

    // Swap test (only between same comparator types)
    {
        BSTType bst1 = CreateLargeBST<int, std::string, Comp>(2500);
        BSTType bst2 = CreateLargeBST<int, std::string, Comp>(5000);
        bst1.Swap(bst2); // OK: same comparator type
        assert(bst1.Size() == 5000);
        assert(bst2.Size() == 2500);
    }

    // Copy/Move tests
    {
        BSTType bst1 = CreateLargeBST<int, std::string, Comp>(large_size);
        BSTType bst2(bst1);
        assert(bst2.Size() == large_size);

        BSTType bst3;
        bst3 = bst1;
        assert(bst3.Size() == large_size);

        BSTType bst4(std::move(bst1));
        assert(bst4.Size() == large_size);

        BSTType bst5;
        bst5 = std::move(bst4);
        assert(bst5.Size() == large_size);
    }
}

int main() {
    RunTestsForComparator<std::less<int>>();
    RunTestsForComparator<Greater<int>>();
    std::cout << "All comparator variant tests passed!\n";
}
