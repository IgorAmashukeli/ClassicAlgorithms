
#include "SetAVL.h"
#include <cassert>
#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

bool LeftOk(const std::unique_ptr<SetNode<int>>& x) {
    return (x != nullptr) && (x->GetRight() != nullptr);
}

bool RightOk(const std::unique_ptr<SetNode<int>>& x) {
    return (x != nullptr) && (x->GetLeft() != nullptr);
}

bool RightLeftOk(const std::unique_ptr<SetNode<int>>& x) {
    return (x != nullptr) && (x->GetRight() != nullptr) && (x->GetRight()->GetLeft() != nullptr);
}

bool LeftRightOk(const std::unique_ptr<SetNode<int>>& x) {
    return (x != nullptr) && (x->GetLeft() != nullptr) && (x->GetLeft()->GetRight() != nullptr);
}

void TestRightLeft(int n) {
    std::vector<int> x;
    for (int i = 0; i < n; ++i) {
        x.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(x.begin(), x.end(), g);

    SetAVL<int> y;

    for (int i = 0; i < n; ++i) {
        y.Insert(x[i]);
    }

    std::sort(x.begin(), x.end());
    assert(std::equal(y.Begin(), y.End(), x.begin()));

    if (LeftRightOk(y.GetRoot())) {
        y.RotateLeftRight(y.GetRoot());
    }

    assert(std::equal(y.Begin(), y.End(), x.begin()));
}

int main() {
    for (int i = 1; i < 100; ++i) {
        TestRightLeft(i);
    }

    std::cout << "All tests passed\n";
}
