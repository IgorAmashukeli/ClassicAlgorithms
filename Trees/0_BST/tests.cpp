
#include "SetBST.h"
#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

// Подключи сюда свой SetBST
// #include "SetBST.h"

// Универсальная проверка BST против эталонного std::vector
template<typename T, typename Compare>
void CheckAgainstVector(SetBST<T, Compare>& bst, std::vector<T> vals, Compare cmp) {
    // Сортируем по компаратору и удаляем дубликаты
    std::sort(vals.begin(), vals.end(), cmp);
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());

    // Проверка SelectInd1
    for (size_t i = 0; i < vals.size(); ++i) {
        assert(*bst.SelectInd1(i + 1) == vals[i]);
    }

    // Проверка RankInd0 для каждого значения из набора
    for (T v : vals) {
        size_t rank = std::distance(
            vals.begin(),
            std::lower_bound(vals.begin(), vals.end(), v, cmp)
        );
        assert(bst.RankInd0(v) == rank);
    }

    // Проверка крайних элементов по компаратору
    if (!vals.empty()) {
        T cmp_min = vals.front();
        T cmp_max = vals.back();
        for (auto& x : vals) {
            if (cmp(x, cmp_min)) {cmp_min = x;} // x <cmp cmp_min
            if (cmp(cmp_max, x)) {cmp_max = x;} // cmp_max <cmp x
        }

        // Меньше любого существующего
        struct LessThanAll {
            T val;
            Compare cmp;
            bool operator<(const T& rhs) const { return cmp(val, rhs); }
            operator T() const { return val; }
        };
        T before_min = cmp_min;
        assert(bst.RankInd0(cmp_min) == 0);
        assert(bst.RankInd0(cmp_max) + 1 == vals.size());


        // Больше любого существующего
        T after_max = cmp_max;

        // Проверки
        assert(bst.RankInd0(cmp_min) >= 0);
        assert(bst.RankInd0(cmp_max) <= vals.size());
    }
}

int main() {
    {
        // Простейший тест std::less
        SetBST<int> bst;
        std::vector<int> vals = {8, 2, -1};
        for (int v : vals) {bst.Insert(v);}
        assert(*bst.SelectInd1(1) == -1);
        assert(*bst.SelectInd1(2) == 2);
        assert(bst.RankInd0(3) == 2);
        CheckAgainstVector(bst, vals, std::less<int>());
    }
    {
        // Тест std::greater
        SetBST<int, std::greater<int>> bst;
        std::vector<int> vals = {8, 2, -1};
        for (int v : vals) {bst.Insert(v);}
        assert(*bst.SelectInd1(1) == 8);
        assert(*bst.SelectInd1(2) == 2);
        assert(bst.RankInd0(3) == 1);
        CheckAgainstVector(bst, vals, std::greater<int>());
    }

    std::cout << "All tests passed!\n";
}
