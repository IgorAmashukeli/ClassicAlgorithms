#include "SetAVL.h"

enum MODE { OFF, INSERT, SELECT, RANK };

// This function is O(n)
// It is specifically for testing
// You can remove it
template <typename K>
bool IsLograithmicHeightBoundForTree(const SetAVL<K>& tree) {
    size_t height = CalcNodeHeight(tree.GetRootPtr());
    return CheckAVLHeightBound(tree.Size(), height);
}

int main() {
    SetAVL<long long> container;
    std::string str;
    int mode = OFF;
    while (std::cin >> str) {
        if (str == "k" && mode == OFF) {
            mode = INSERT;
        } else if (str == "k") {
            std::cout << "NO number followed. Error.\n";
            return -1;
        } else if (str == "m" && mode == OFF) {
            mode = SELECT;
        } else if (str == "m") {
            std::cout << "NO number followed. Error.\n";
            return -1;
        } else if (str == "n" && mode == OFF) {
            mode = RANK;
        } else if (str == "n") {
            std::cout << "NO number followed. Error.\n";
            return -1;
        } else if (mode == OFF) {
            std::cout << "NO k/m/n followed. Error.\n";
            return -1;
        } else if (mode == INSERT) {
            long long k;
            try {
                k = std::stoll(str);
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid integer argument. Error.\n";
                return -1;
            } catch (const std::out_of_range& e) {
                std::cout << "Value out of long long range. Error.\n";
                return -1;
            }
            auto result = container.Insert(k);
            if (result.second == false) {
                std::cout << "You entered a duplicate. Error. \n";
                return -1;
            }
            mode = OFF;
            assert(IsLograithmicHeightBoundForTree(container));
        } else if (mode == SELECT) {
            size_t i;
            try {
                i = std::stoull(str);
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid integer argument. Error.\n";
                return -1;
            } catch (const std::out_of_range& e) {
                std::cout << "Value out of size_t range. Error.\n";
                return -1;
            }
            auto it = container.SelectInd1(i);
            if (it == container.End()) {
                std::cout << "Wrong index for k-th order statistic. Error. \n";
                return -1;
            } else {
                long long key = *it;
                std::cout << key << " ";
            }
            mode = OFF;
        } else if (mode == RANK) {
            long long k;
            try {
                k = std::stoll(str);
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid integer argument. Error.\n";
                return -1;
            } catch (const std::out_of_range& e) {
                std::cout << "Value out of long long range. Error.\n";
                return -1;
            }
            size_t result = container.RankInd0(k);
            std::cout << result << " ";
            mode = OFF;
        }
    }
    if (mode != OFF) {
        std::cout << "Input ended without a following number. Error. \n";
        return -1;
    }
    std::cout << "\n";
}