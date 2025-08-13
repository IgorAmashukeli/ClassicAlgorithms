#include "SetBST.h"
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

// Предположим, что SetBST<int> уже определён

void TestBST() {
    {
        // Пример из условия
        SetBST<int> bst;
        bst.Insert(8);
        bst.Insert(2);
        bst.Insert(-1);
        assert(*bst.SelectInd1(1) == -1);
        assert(*bst.SelectInd1(2) == 2);
        assert(bst.RankInd0(3) == 2);
    }

    {
        // Проверка с разными числами
        SetBST<int> bst;
        bst.Insert(10);
        bst.Insert(5);
        bst.Insert(20);
        bst.Insert(15);
        bst.Insert(0);
        bst.Insert(-5);

        // Отсортированный порядок: -5, 0, 5, 10, 15, 20
        assert(*bst.SelectInd1(1) == -5);
        assert(*bst.SelectInd1(3) == 5);
        assert(*bst.SelectInd1(6) == 20);

        assert(bst.RankInd0(-10) == 0); // всё больше
        assert(bst.RankInd0(-5) == 0);  // равен первому
        assert(bst.RankInd0(0) == 1);   // -5 < 0
        assert(bst.RankInd0(7) == 3);   // -5, 0, 5 < 7
        assert(bst.RankInd0(100) == 6); // все < 100
    }

    {
        // Последовательная вставка по возрастанию
        SetBST<int> bst;
        for (int i = 1; i <= 5; ++i) {bst.Insert(i * 10);} // 10, 20, 30, 40, 50

        assert(*bst.SelectInd1(1) == 10);
        assert(*bst.SelectInd1(5) == 50);
        assert(bst.RankInd0(25) == 2); // 10, 20
        assert(bst.RankInd0(10) == 0); // ни одного меньше 10
        assert(bst.RankInd0(11) == 1); // только 10
    }

    {
        // Последовательная вставка по убыванию
        SetBST<int> bst;
        for (int i = 5; i >= 1; --i) {bst.Insert(i * -2);} // -10, -8, -6, -4, -2

        // Отсортировано: -10, -8, -6, -4, -2
        assert(*bst.SelectInd1(1) == -10);
        assert(*bst.SelectInd1(5) == -2);
        assert(bst.RankInd0(-6) == 2); // -10, -8
        assert(bst.RankInd0(-11) == 0);
        assert(bst.RankInd0(0) == 5);
    }

    {
        // Смешанный порядок вставок
        SetBST<int> bst;
        std::vector<int> vals = {7, 1, 9, 3, -2, 8};
        for (int v : vals) {bst.Insert(v);}

        // Отсортировано: -2, 1, 3, 7, 8, 9
        assert(*bst.SelectInd1(2) == 1);
        assert(*bst.SelectInd1(6) == 9);
        assert(bst.RankInd0(5) == 3); // -2, 1, 3 < 5
        assert(bst.RankInd0(-2) == 0);
        assert(bst.RankInd0(10) == 6);
    }
}

int main() {
    TestBST();
    SetBST<int> bst;
    std::cout << "All tests passed\n";
}