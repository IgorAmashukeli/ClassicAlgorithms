#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <stack>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <iostream>
#include "compressed_pair.h"

template <typename K, typename Compare>
bool Equivalent(const K& key_1, const K& key_2, Compare compare) {
    return !(compare(key_1, key_2)) && (!compare(key_2, key_1));
}

template <typename K, typename V>
class MapNode;

template <typename K, typename V>
class MapBaseNode {
public:
    MapBaseNode() noexcept = default;
    MapBaseNode(const MapBaseNode& other) = delete;
    MapBaseNode& operator=(const MapBaseNode& other) = delete;
    MapBaseNode(MapBaseNode&& other) = delete;
    MapBaseNode& operator=(MapBaseNode&& other) = delete;
    ~MapBaseNode() noexcept = default;

    virtual const K& GetKey() const = 0;
    virtual const V& GetValue() const = 0;
    virtual V& GetValue() = 0;
    virtual const std::unique_ptr<MapNode<K, V>>& GetLeft() const = 0;
    virtual std::unique_ptr<MapNode<K, V>>& GetLeft() = 0;
    virtual const std::unique_ptr<MapNode<K, V>>& GetRight() const = 0;
    virtual std::unique_ptr<MapNode<K, V>>& GetRight() = 0;
    virtual MapNode<K, V>* GetParent() const = 0;
    virtual MapNode<K, V>*& GetParent() = 0;
    virtual MapBaseNode<K, V>* GetPrev() const noexcept = 0;
    virtual MapBaseNode<K, V>*& GetPrev() noexcept = 0;
    virtual MapBaseNode<K, V>* GetNext() const noexcept = 0;
    virtual MapBaseNode<K, V>*& GetNext() noexcept = 0;
    virtual const std::pair<const K, V>& GetKeyValue() const = 0;
    virtual std::pair<const K, V>& GetKeyValue() = 0;
    virtual signed char GetBalance() const = 0;
    virtual signed char& GetBalance() = 0;
    virtual bool IsMapEndNode() const noexcept = 0;
};

template <typename K, typename V>
class MapNode : public MapBaseNode<K, V> {
public:
    MapNode() = default;
    MapNode(const MapNode& other) = delete;
    MapNode& operator=(const MapNode& other) = delete;
    MapNode(MapNode&& other) = delete;
    MapNode& operator=(MapNode&& other) = delete;
    ~MapNode() = default;

    MapNode(const K& key, const V& value, MapBaseNode<K, V>* prev, MapBaseNode<K, V>* next,
            signed char balance)
        : key_value_(key, value), prev_(prev), next_(next), balance_(balance) {
    }
    MapNode(const std::pair<const K, V>& key_value, MapBaseNode<K, V>* prev,
            MapBaseNode<K, V>* next, signed char balance)
        : key_value_(key_value), prev_(prev), next_(next), balance_(balance) {
    }
    MapNode(std::pair<const K, V>&& key_value, MapBaseNode<K, V>* prev, MapBaseNode<K, V>* next,
            signed char balance)
        : key_value_(std::move(key_value)), prev_(prev), next_(next), balance_(balance) {
    }
    template <typename P>
    MapNode(P&& key_value, MapBaseNode<K, V>* prev, MapBaseNode<K, V>* next, signed char balance)
        : key_value_(std::forward<P>(key_value)), prev_(prev), next_(next), balance_(balance) {
    }
    const K& GetKey() const noexcept {
        return key_value_.first;
    }
    const V& GetValue() const noexcept {
        return key_value_.second;
    }
    V& GetValue() noexcept {
        return key_value_.second;
    }
    const std::unique_ptr<MapNode<K, V>>& GetLeft() const noexcept {
        return left_;
    }
    std::unique_ptr<MapNode<K, V>>& GetLeft() noexcept {
        return left_;
    }
    const std::unique_ptr<MapNode<K, V>>& GetRight() const noexcept {
        return right_;
    }
    std::unique_ptr<MapNode<K, V>>& GetRight() noexcept {
        return right_;
    }
    MapNode<K, V>* GetParent() const noexcept {
        return parent_;
    }
    MapNode<K, V>*& GetParent() noexcept {
        return parent_;
    }
    MapBaseNode<K, V>* GetPrev() const noexcept {
        return prev_;
    }
    MapBaseNode<K, V>*& GetPrev() noexcept {
        return prev_;
    }
    MapBaseNode<K, V>* GetNext() const noexcept {
        return next_;
    }
    MapBaseNode<K, V>*& GetNext() noexcept {
        return next_;
    }
    const std::pair<const K, V>& GetKeyValue() const noexcept {
        return key_value_;
    }
    std::pair<const K, V>& GetKeyValue() noexcept {
        return key_value_;
    }
    signed char GetBalance() const {
        return balance_;
    }
    signed char& GetBalance() {
        return balance_;
    }
    bool IsMapEndNode() const noexcept {
        return false;
    }

private:
    std::pair<const K, V> key_value_;
    std::unique_ptr<MapNode<K, V>> left_;
    std::unique_ptr<MapNode<K, V>> right_;
    MapNode<K, V>* parent_ = nullptr;
    MapBaseNode<K, V>* prev_ = nullptr;
    MapBaseNode<K, V>* next_ = nullptr;
    signed char balance_ = 0;
};

template <typename K, typename V>
class EndMapNode : public MapBaseNode<K, V> {
public:
    EndMapNode() noexcept = default;
    EndMapNode(const EndMapNode& other) = delete;
    EndMapNode& operator=(const EndMapNode& other) = delete;
    EndMapNode(EndMapNode&& other) noexcept = default;
    EndMapNode& operator=(EndMapNode&& other) noexcept = default;
    ~EndMapNode() noexcept = default;

    EndMapNode(MapBaseNode<K, V>* prev, MapBaseNode<K, V>* next) noexcept
        : prev_(prev), next_(next) {
    }
    const K& GetKey() const {
        throw std::out_of_range("Out of range!");
    }
    const V& GetValue() const {
        throw std::out_of_range("Out of range!");
    }
    V& GetValue() {
        throw std::out_of_range("Out of range!");
    }
    const std::unique_ptr<MapNode<K, V>>& GetLeft() const {
        throw std::out_of_range("Out of range!");
    }
    std::unique_ptr<MapNode<K, V>>& GetLeft() {
        throw std::out_of_range("Out of range!");
    }
    const std::unique_ptr<MapNode<K, V>>& GetRight() const {
        throw std::out_of_range("Out of range!");
    }
    std::unique_ptr<MapNode<K, V>>& GetRight() {
        throw std::out_of_range("Out of range!");
    }
    MapNode<K, V>* GetParent() const {
        throw std::out_of_range("Out of range!");
    }
    MapNode<K, V>*& GetParent() {
        throw std::out_of_range("Out of range!");
    }
    MapBaseNode<K, V>* GetPrev() const noexcept {
        return prev_;
    }
    MapBaseNode<K, V>*& GetPrev() noexcept {
        return prev_;
    }
    MapBaseNode<K, V>* GetNext() const noexcept {
        return next_;
    }
    MapBaseNode<K, V>*& GetNext() noexcept {
        return next_;
    }
    const std::pair<const K, V>& GetKeyValue() const {
        throw std::out_of_range("Out of range!");
    }
    std::pair<const K, V>& GetKeyValue() {
        throw std::out_of_range("Out of range!");
    }
    signed char GetBalance() const {
        throw std::out_of_range("Out of range!");
    }
    signed char& GetBalance() {
        throw std::out_of_range("Out of range!");
    }
    bool IsMapEndNode() const noexcept {
        return true;
    }

private:
    MapBaseNode<K, V>* prev_ = nullptr;
    MapBaseNode<K, V>* next_ = nullptr;
};

template <typename K, typename V, typename Compare = std::less<K>>
class MapAVL {
public:
    enum {
        LEFT_VISITED = true,
        LEFT_NOT_VISITED = false,
        RIGHT_VISITED = true,
        RIGHT_NOT_VISITED = false
    };

    using ValueType = std::pair<const K, V>;
    using Reference = ValueType&;
    using Pointer = ValueType*;
    using ConstReference = const ValueType&;
    using ConstPointer = const ValueType*;

    class Iterator {
    public:
        explicit Iterator(MapBaseNode<K, V>* node) noexcept : node_(node) {
        }
        Reference operator*() const {
            return node_->GetKeyValue();
        }
        Pointer operator->() const {
            return std::addressof(node_->GetKeyValue());
        }
        Iterator& operator++() {
            Inc();
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            Inc();
            return tmp;
        }
        Iterator& operator--() {
            Dec();
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp = *this;
            Dec();
            return tmp;
        }
        bool operator==(const Iterator& other) const noexcept {
            return node_ == other.node_;
        }
        bool operator!=(const Iterator& other) const noexcept {
            return node_ != other.node_;
        }

        friend class ConstIterator;

    private:
        void Inc() {
            if (node_ != nullptr && !node_->IsMapEndNode()) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetPrev()->IsMapEndNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        MapBaseNode<K, V>* node_ = nullptr;
    };

    class ConstIterator {
    public:
        explicit ConstIterator(const MapBaseNode<K, V>* node) noexcept : node_(node) {
        }
        ConstIterator(Iterator it) noexcept : node_(it.node_) {
        }
        ConstReference operator*() const {
            return node_->GetKeyValue();
        }
        ConstPointer operator->() const {
            return std::addressof(node_->GetKeyValue());
        }
        ConstIterator& operator++() {
            Inc();
            return *this;
        }
        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            Inc();
            return tmp;
        }
        ConstIterator& operator--() {
            Dec();
            return *this;
        }
        ConstIterator operator--(int) {
            ConstIterator tmp = *this;
            Dec();
            return tmp;
        }
        bool operator==(const ConstIterator& other) const noexcept {
            return node_ == other.node_;
        }
        bool operator!=(const ConstIterator& other) const noexcept {
            return node_ != other.node_;
        }
        friend class MapAVL;

    private:
        void Inc() {
            if (node_ != nullptr && !node_->IsMapEndNode()) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetPrev()->IsMapEndNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        const MapBaseNode<K, V>* node_ = nullptr;
    };

    class ReverseIterator {
    public:
        explicit ReverseIterator(MapBaseNode<K, V>* node) : node_(node) {
        }
        Reference operator*() const {
            return node_->GetKeyValue();
        }
        Pointer operator->() const {
            return std::addressof(node_->GetKeyValue());
        }
        ReverseIterator& operator++() {
            Inc();
            return *this;
        }
        ReverseIterator operator++(int) {
            ReverseIterator tmp = *this;
            Inc();
            return tmp;
        }
        ReverseIterator& operator--() {
            Dec();
            return *this;
        }
        ReverseIterator operator--(int) {
            ReverseIterator tmp = *this;
            Dec();
            return tmp;
        }
        bool operator==(const ReverseIterator& other) const noexcept {
            return node_ == other.node_;
        }
        bool operator!=(const ReverseIterator& other) const noexcept {
            return node_ != other.node_;
        }

        friend class ConstReverseIterator;

    private:
        void Inc() {
            if (node_ != nullptr && !node_->IsMapEndNode()) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetNext()->IsMapEndNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        MapBaseNode<K, V>* node_ = nullptr;
    };

    class ConstReverseIterator {
    public:
        explicit ConstReverseIterator(const MapBaseNode<K, V>* node) noexcept : node_(node) {
        }
        ConstReverseIterator(ReverseIterator it) noexcept : node_(it.node_) {
        }
        ConstReference operator*() const noexcept {
            return node_->GetKeyValue();
        }
        ConstPointer operator->() const noexcept {
            return std::addressof(node_->GetKeyValue());
        }
        ConstReverseIterator& operator++() {
            Inc();
            return *this;
        }
        ConstReverseIterator operator++(int) {
            ConstReverseIterator tmp = *this;
            Inc();
            return tmp;
        }
        ConstReverseIterator& operator--() {
            Dec();
            return *this;
        }
        ConstReverseIterator operator--(int) {
            ConstReverseIterator tmp = *this;
            Dec();
            return tmp;
        }
        bool operator==(const ConstReverseIterator& other) const noexcept {
            return node_ == other.node_;
        }
        bool operator!=(const ConstReverseIterator& other) const noexcept {
            return node_ != other.node_;
        }

    private:
        void Inc() {
            if (node_ != nullptr && !node_->IsMapEndNode()) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetNext()->IsMapEndNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        const MapBaseNode<K, V>* node_ = nullptr;
    };

    MapAVL() : MapAVL(Compare()) {
    }
    explicit MapAVL(const Compare& compare) : root_compare_(nullptr, compare) {
    }
    MapAVL(const MapAVL& other) {
        Copy(other);
    }
    MapAVL& operator=(const MapAVL& other) {
        return *this = MapAVL(other);
    }
    MapAVL(MapAVL&& other) noexcept {
        Swap(other);
    }
    MapAVL& operator=(MapAVL&& other) noexcept {
        MapAVL tmp = std::move(other);
        Swap(tmp);
        return *this;
    }
    ~MapAVL() = default;

    void Clear() noexcept {
        GetRoot() = nullptr;
        size_ = 0;
        end_node_.GetNext() = std::addressof(end_node_);
        end_node_.GetPrev() = std::addressof(end_node_);
    }
    void Swap(MapAVL& other) {
        std::swap(GetRoot(), other.GetRoot());
        std::swap(size_, other.size_);
        ConnectEndMapNodesAfterSwap(other);
    }
    std::pair<Iterator, bool> Insert(const ValueType& key_value) {
        auto pair = InsertMapNode(key_value);
        auto node = pair.first;
        auto inserted = pair.second;
        if (!inserted) {
            return {Iterator(node), false};
        }
        BalanceAfterInsert(node);
        return {Iterator(node), true};
    }
    std::pair<Iterator, bool> Insert(ValueType&& key_value) {
        auto pair = InsertMapNode(std::move(key_value));
        auto node = pair.first;
        auto inserted = pair.second;
        if (!inserted) {
            return {Iterator(node), false};
        }
        BalanceAfterInsert(node);
        return {Iterator(node), true};
    }
    template <typename P>
    std::pair<Iterator, bool> Insert(P&& key_value) {
        auto pair = InsertMapNode(std::forward<P>(key_value));
        auto node = pair.first;
        auto inserted = pair.second;
        if (!inserted) {
            return {Iterator(node), false};
        }
        BalanceAfterInsert(node);
        return {Iterator(node), true};
    }
    template <typename InputIt>
    void Insert(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            Insert(*it);
        }
    }
    void Insert(std::initializer_list<ValueType> ilist) {
        Insert(ilist.begin(), ilist.end());
    }
    Iterator Find(const K& key) {
        auto node = FindMapNode(key);
        if (node == nullptr) {
            return End();
        }
        return Iterator{node};
    }
    ConstIterator Find(const K& key) const {
        auto node = FindMapNode(key);
        if (node == nullptr) {
            return End();
        }
        return ConstIterator{node};
    }
    Iterator LowerBound(const K& key) {
        auto node = FindLowerBound(key);
        if (node == nullptr) {
            return End();
        }
        return Iterator{node};
    }
    ConstIterator LowerBound(const K& key) const {
        auto node = FindLowerBound(key);
        if (node == nullptr) {
            return End();
        }
        return ConstIterator{node};
    }
    Iterator UpperBound(const K& key) {
        auto node = FindLowerBound(key);
        if (node == nullptr) {
            return End();
        }
        if (Equivalent(node->GetKey(), key, KeyCompare())) {
            return Iterator{node->GetNext()};
        }
        return Iterator{node};
    }
    ConstIterator UpperBound(const K& key) const {
        auto node = FindLowerBound(key);
        if (node == nullptr) {
            return End();
        }
        if (Equivalent(node->GetKey(), key, KeyCompare())) {
            return ConstIterator{node->GetNext()};
        }
        return ConstIterator{node};
    }
    std::pair<Iterator, Iterator> EqualRange(const K& key) {
        auto node = FindLowerBound(key);
        if (node == nullptr) {
            return {End(), End()};
        }
        if (Equivalent(node->GetKey(), key, KeyCompare())) {
            return {Iterator{node}, Iterator{node->GetNext()}};
        }
        return {Iterator{node}, Iterator{node}};
    }
    std::pair<ConstIterator, ConstIterator> EqualRange(const K& key) const {
        auto node = FindLowerBound(key);
        if (node == nullptr) {
            return {End(), End()};
        }
        if (Equivalent(node->GetKey(), key, KeyCompare())) {
            return {ConstIterator{node}, ConstIterator{node->GetNext()}};
        }
        return {ConstIterator{node}, ConstIterator{node}};
    }
    bool Contains(const K& key) const {
        return FindMapNode(key) != nullptr;
    }
    size_t Count(const K& key) const {
        return static_cast<size_t>(Contains(key));
    }
    Iterator Begin() noexcept {
        return Iterator{end_node_.GetNext()};
    }
    ConstIterator Begin() const noexcept {
        return ConstIterator{end_node_.GetNext()};
    }
    Iterator End() noexcept {
        return Iterator{std::addressof(end_node_)};
    }
    ConstIterator End() const noexcept {
        return ConstIterator{std::addressof(end_node_)};
    }
    ConstIterator CBegin() const noexcept {
        return ConstIterator{end_node_.GetNext()};
    }
    ConstIterator CEnd() const noexcept {
        return ConstIterator{std::addressof(end_node_)};
    }
    ReverseIterator RBegin() noexcept {
        return ReverseIterator{end_node_.GetPrev()};
    }
    ConstReverseIterator RBegin() const noexcept {
        return ConstReverseIterator{end_node_.GetPrev()};
    }
    ReverseIterator REnd() noexcept {
        return ReverseIterator{std::addressof(end_node_)};
    }
    ConstReverseIterator REnd() const noexcept {
        return ConstReverseIterator{std::addressof(end_node_)};
    }
    ConstReverseIterator CRBegin() const noexcept {
        return ConstReverseIterator{end_node_.GetPrev()};
    }
    ConstReverseIterator CREnd() const noexcept {
        return ConstReverseIterator{std::addressof(end_node_)};
    }

    size_t Size() const noexcept {
        return size_;
    }
    static constexpr size_t MaxSize() noexcept {
        return (std::numeric_limits<std::ptrdiff_t>::max() / sizeof(MapNode<K, V>));
    }
    bool Empty() const noexcept {
        return (GetRoot() == nullptr);
    }
    Compare KeyCompare() const {
        return root_compare_.GetSecond();
    }
    const std::unique_ptr<MapNode<K, V>>& GetRoot() const {
        return root_compare_.GetFirst();
    }
    std::unique_ptr<MapNode<K, V>>& GetRoot() {
        return root_compare_.GetFirst();
    }
    MapNode<K, V>* GetRootPtr() const {
        return GetRoot().get();
    }

private:
    signed char GetNodeBalance(MapNode<K, V>* node) const {
        if (node == nullptr) {
            return 0;
        }
        return node->GetBalance();
    }
    bool IsBalanceNormal(MapNode<K, V>* node) const {
        return std::abs(GetNodeBalance(node)) <= 2;
    }

    bool LeftRotateNeeded(MapNode<K, V>* node) {
        if (node == nullptr || node->GetRight() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == -2) && ((GetNodeBalance(node->GetRight().get()) == -1) ||
                                                (GetNodeBalance(node->GetRight().get()) == 0));
    }
    bool RightRotateNeded(MapNode<K, V>* node) {
        if (node == nullptr || node->GetLeft() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == 2) && ((GetNodeBalance(node->GetLeft().get()) == 1) ||
                                               (GetNodeBalance(node->GetLeft().get()) == 0));
    }
    bool RightLeftRotateNeeded(MapNode<K, V>* node) {
        if (node == nullptr || node->GetRight() == nullptr ||
            node->GetRight()->GetLeft() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == -2) && (GetNodeBalance(node->GetRight().get()) == 1);
    }
    bool LeftRightRotateNeeded(MapNode<K, V>* node) {
        if (node == nullptr || node->GetLeft() == nullptr ||
            node->GetLeft()->GetRight() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == 2) && (GetNodeBalance(node->GetLeft().get()) == -1);
    }
    MapNode<K, V>* FindMapNode(const K& key) const {
        MapNode<K, V>* node = GetRootPtr();

        while (node != nullptr) {
            if (Equivalent(key, node->GetKey(), KeyCompare())) {
                return node;
            }
            if (KeyCompare()(key, node->GetKey())) {
                node = node->GetLeft().get();
            } else {
                node = node->GetRight().get();
            }
        }
        return nullptr;
    }

    MapNode<K, V>* FindLowerBound(const K& key) const {
        MapNode<K, V>* node = GetRootPtr();
        MapNode<K, V>* best_bound = nullptr;

        while (node != nullptr) {
            if (Equivalent(key, node->GetKey(), KeyCompare())) {
                return node;
            }
            if (KeyCompare()(key, node->GetKey())) {
                best_bound = node;
                node = node->GetLeft().get();
            } else {
                node = node->GetRight().get();
            }
        }
        return best_bound;
    }

    void ConnectEndMapNodesAfterSwap(MapAVL& other) {
        if (Empty() && other.Empty()) {
            return;
        }

        auto other_min_node = end_node_.GetNext();
        auto other_max_node = end_node_.GetPrev();
        auto min_node = other.end_node_.GetNext();
        auto max_node = other.end_node_.GetPrev();

        if (Empty() && !other.Empty()) {
            other.end_node_.GetPrev() = other_max_node;
            other.end_node_.GetNext() = other_min_node;
            other_max_node->GetNext() = std::addressof(other.end_node_);
            other_min_node->GetPrev() = std::addressof(other.end_node_);

            end_node_.GetPrev() = std::addressof(end_node_);
            end_node_.GetNext() = std::addressof(end_node_);
            return;
        }

        if (!Empty() && other.Empty()) {
            end_node_.GetPrev() = max_node;
            end_node_.GetNext() = min_node;
            max_node->GetNext() = std::addressof(end_node_);
            min_node->GetPrev() = std::addressof(end_node_);

            other.end_node_.GetPrev() = std::addressof(other.end_node_);
            other.end_node_.GetNext() = std::addressof(other.end_node_);
            return;
        }

        end_node_.GetPrev() = max_node;
        end_node_.GetNext() = min_node;
        max_node->GetNext() = std::addressof(end_node_);
        min_node->GetPrev() = std::addressof(end_node_);

        other.end_node_.GetPrev() = other_max_node;
        other.end_node_.GetNext() = other_min_node;
        other_max_node->GetNext() = std::addressof(other.end_node_);
        other_min_node->GetPrev() = std::addressof(other.end_node_);
    }

    void ConnectEndMapNodesAfterCopy(MapBaseNode<K, V>* max_node) {
        if (GetRoot() != nullptr) {
            max_node->GetNext() = std::addressof(end_node_);
            end_node_.GetPrev() = max_node;
        }
    }

    void MarkVisited(std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& nodes, bool left) const {
        auto top_other_node = std::get<0>(nodes.top());
        auto visit_left = std::get<1>(nodes.top());
        auto visit_right = std::get<2>(nodes.top());
        nodes.pop();
        if (left) {
            nodes.push({top_other_node, LEFT_VISITED, visit_right});
        } else {
            nodes.push({top_other_node, visit_left, RIGHT_VISITED});
        }
    }

    void MakeVisited(std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& nodes,
                     MapNode<K, V>* child) const {
        if (nodes.size() == 0) {
            return;
        }
        auto top_other_node = std::get<0>(nodes.top());
        if (top_other_node->GetLeft().get() == child) {
            MarkVisited(nodes, true);
        } else {
            MarkVisited(nodes, false);
        }
    }

    bool LeftNull(MapNode<K, V>* node) const {
        return node->GetLeft() == nullptr;
    }

    bool RightNull(MapNode<K, V>* node) const {
        return node->GetRight() == nullptr;
    }

    bool LeftVisited(MapNode<K, V>* node, bool visit_left) const {
        return (node->GetLeft() != nullptr) && (visit_left);
    }

    bool RightVisited(MapNode<K, V>* node, bool visit_right) const {
        return (node->GetRight() != nullptr) && (visit_right);
    }

    bool LeftNotVisited(MapNode<K, V>* node, bool visit_left) const {
        return (node->GetLeft() != nullptr) && (!visit_left);
    }

    bool RightNotVisited(MapNode<K, V>* node, bool visit_right) const {
        return (node->GetRight() != nullptr) && (!visit_right);
    }

    std::unique_ptr<MapNode<K, V>> CreateCopied(MapNode<K, V>* top_other_node,
                                                MapBaseNode<K, V>*& prev_node) {
        auto node =
            std::make_unique<MapNode<K, V>>(top_other_node->GetKey(), top_other_node->GetValue(),
                                            nullptr, nullptr, top_other_node->GetBalance());
        node->GetPrev() = prev_node;
        prev_node->GetNext() = node.get();
        prev_node = node.get();
        return node;
    }

    void PushOrRoot(const MapAVL& other, std::stack<std::unique_ptr<MapNode<K, V>>>& nodes,
                    std::unique_ptr<MapNode<K, V>>&& node, MapNode<K, V>* top_other_node) {
        if (top_other_node == other.GetRoot().get()) {
            GetRoot() = std::move(node);
        } else {
            nodes.push(std::move(node));
        }
    }

    void LNVRN(std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& other_nodes,
               MapNode<K, V>* top_other_node) {
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_VISITED});
        other_nodes.push({top_other_node->GetLeft().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    void LNVRNV(std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& other_nodes,
                MapNode<K, V>* top_other_node) {
        other_nodes.push({top_other_node->GetRight().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_NOT_VISITED});
        other_nodes.push({top_other_node->GetLeft().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    void LVRNV(std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& other_nodes,
               MapNode<K, V>* top_other_node) {
        other_nodes.pop();
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_VISITED});
        other_nodes.push({top_other_node->GetRight().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    void LNRNV(std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& other_nodes,
               MapNode<K, V>* top_other_node) {
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_VISITED});
        other_nodes.push({top_other_node->GetRight().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    std::unique_ptr<MapNode<K, V>> ConnectL(std::unique_ptr<MapNode<K, V>>&& node,
                                            std::stack<std::unique_ptr<MapNode<K, V>>>& nodes) {
        node->GetLeft() = std::move(nodes.top());
        node->GetLeft()->GetParent() = node.get();
        nodes.pop();
        return node;
    }
    std::unique_ptr<MapNode<K, V>> ConnectR(std::stack<std::unique_ptr<MapNode<K, V>>>& nodes) {
        auto rhs = std::move(nodes.top());
        nodes.pop();
        auto current = std::move(nodes.top());
        nodes.pop();
        current->GetRight() = std::move(rhs);
        current->GetRight()->GetParent() = current.get();
        return current;
    }

    void CopyIteration(const MapAVL& other,
                       std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& other_nodes,
                       std::stack<std::unique_ptr<MapNode<K, V>>>& nodes,
                       MapNode<K, V>* top_other_node, bool visit_left, bool visit_right,
                       MapBaseNode<K, V>*& prev_node) {
        if (LeftNotVisited(top_other_node, visit_left) && RightNull(top_other_node)) {
            LNVRN(other_nodes, top_other_node);
        } else if (LeftNotVisited(top_other_node, visit_left) &&
                   RightNotVisited(top_other_node, visit_right)) {
            LNVRNV(other_nodes, top_other_node);
        } else if (LeftVisited(top_other_node, visit_left) &&
                   RightNotVisited(top_other_node, visit_right)) {
            auto node = CreateCopied(top_other_node, prev_node);
            node = ConnectL(std::move(node), nodes);
            nodes.push(std::move(node));
            LVRNV(other_nodes, top_other_node);
        } else if (LeftVisited(top_other_node, visit_left) && RightNull(top_other_node)) {
            auto node = CreateCopied(top_other_node, prev_node);
            node = ConnectL(std::move(node), nodes);
            PushOrRoot(other, nodes, std::move(node), top_other_node);
            MakeVisited(other_nodes, top_other_node);
        } else if (LeftVisited(top_other_node, visit_left) &&
                   RightVisited(top_other_node, visit_right)) {
            auto current = ConnectR(nodes);
            PushOrRoot(other, nodes, std::move(current), top_other_node);
            MakeVisited(other_nodes, top_other_node);
        } else if (LeftNull(top_other_node) && RightNotVisited(top_other_node, visit_right)) {
            auto node = CreateCopied(top_other_node, prev_node);
            nodes.push(std::move(node));
            LNRNV(other_nodes, top_other_node);
        } else if (LeftNull(top_other_node) && RightVisited(top_other_node, visit_right)) {
            auto current = ConnectR(nodes);
            PushOrRoot(other, nodes, std::move(current), top_other_node);
            MakeVisited(other_nodes, top_other_node);
        } else if (LeftNull(top_other_node) && RightNull(top_other_node)) {
            auto node = CreateCopied(top_other_node, prev_node);
            PushOrRoot(other, nodes, std::move(node), top_other_node);
            MakeVisited(other_nodes, top_other_node);
        }
    }

    void Copy(const MapAVL& other) {
        if (other.Empty()) {
            return;
        }
        MapBaseNode<K, V>* prev_node = std::addressof(end_node_);
        std::stack<std::tuple<MapNode<K, V>*, bool, bool>> other_nodes;
        std::stack<std::unique_ptr<MapNode<K, V>>> nodes;
        other_nodes.push({other.GetRootPtr(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
        while (!other_nodes.empty()) {
            auto top_other_node = std::get<0>(other_nodes.top());
            auto visit_left = std::get<1>(other_nodes.top());
            auto visit_right = std::get<2>(other_nodes.top());
            other_nodes.pop();
            CopyIteration(other, other_nodes, nodes, top_other_node, visit_left, visit_right,
                          prev_node);
        }
        ConnectEndMapNodesAfterCopy(prev_node);
        size_ = other.size_;
    }

    void ConnectPrevNext(MapNode<K, V>* node, MapBaseNode<K, V>* prev, MapBaseNode<K, V>* next) {
        node->GetNext() = next;
        next->GetPrev() = node;
        node->GetPrev() = prev;
        prev->GetNext() = node;
    }

    void IncreaseSize() {
        ++size_;
    }

    template <typename P>
    std::pair<MapNode<K, V>*, bool> InsertMapNode(P&& key_value) {
        MapNode<K, V>* node = GetRootPtr();
        MapNode<K, V>* parent = nullptr;
        bool left = false;
        MapBaseNode<K, V>* current_prev = std::addressof(end_node_);
        MapBaseNode<K, V>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                GetRoot() = std::make_unique<MapNode<K, V>>(std::forward<P>(key_value),
                                                            std::addressof(end_node_),
                                                            std::addressof(end_node_), 0);
                ConnectPrevNext(GetRootPtr(), current_prev, current_next);
                IncreaseSize();
                return {GetRootPtr(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() = std::make_unique<MapNode<K, V>>(std::forward<P>(key_value),
                                                                    nullptr, nullptr, 0);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSize();
                return {parent->GetLeft().get(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() = std::make_unique<MapNode<K, V>>(std::forward<P>(key_value),
                                                                     nullptr, nullptr, 0);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSize();
                return {parent->GetRight().get(), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key_value.first, KeyCompare())) {
                return {node, false};
            }
            if ((node != nullptr) && (KeyCompare()(key_value.first, node->GetKey()))) {
                left = true;
                parent = node;
                current_next = node;
                node = node->GetLeft().get();
            } else {
                left = false;
                parent = node;
                current_prev = node;
                node = node->GetRight().get();
            }
        }
    }

    MapNode<K, V>* GetReleased(std::unique_ptr<MapNode<K, V>>& node) {
        if (node != nullptr) {
            return node.release();
        }
        return nullptr;
    }

    void ConnectAfterRotation(MapNode<K, V>* parent, MapNode<K, V>* child, bool left) {
        if (child != nullptr) {
            child->GetParent() = parent;
        }
        if (parent != nullptr && left) {
            parent->GetLeft() = std::unique_ptr<MapNode<K, V>>(child);
        } else if (parent != nullptr) {
            parent->GetRight() = std::unique_ptr<MapNode<K, V>>(child);
        } else {
            GetRoot() = std::unique_ptr<MapNode<K, V>>(child);
        }
    }

    void FixLeftBalance(MapNode<K, V>* left_child, MapNode<K, V>* node) {
        if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(node) == -1)) {
            left_child->GetBalance() = 0;
            node->GetBalance() = 0;
        } else if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(node) == 0)) {
            left_child->GetBalance() = -1;
            node->GetBalance() = 1;
        }
    }
    void FixRightBalance(MapNode<K, V>* right_child, MapNode<K, V>* node) {
        if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(node) == 1)) {
            right_child->GetBalance() = 0;
            node->GetBalance() = 0;
        } else if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(node) == 0)) {
            right_child->GetBalance() = 1;
            node->GetBalance() = -1;
        }
    }
    void FixRightLeftBalance(MapNode<K, V>* left_child, MapNode<K, V>* right_child,
                             MapNode<K, V>* node) {
        if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(right_child) == 1) &&
            (GetNodeBalance(node) == 1)) {
            left_child->GetBalance() = 0;
            right_child->GetBalance() = -1;
            node->GetBalance() = 0;

        } else if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(right_child) == 1) &&
                   (GetNodeBalance(node) == -1)) {
            left_child->GetBalance() = 1;
            right_child->GetBalance() = 0;
            node->GetBalance() = 0;

        } else if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(right_child) == 1) &&
                   (GetNodeBalance(node) == 0)) {
            left_child->GetBalance() = 0;
            right_child->GetBalance() = 0;
            node->GetBalance() = 0;
        }
    }
    void FixLeftRightBalance(MapNode<K, V>* right_child, MapNode<K, V>* left_child,
                             MapNode<K, V>* node) {
        if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(left_child) == -1) &&
            (GetNodeBalance(node) == -1)) {
            right_child->GetBalance() = 0;
            left_child->GetBalance() = 1;
            node->GetBalance() = 0;

        } else if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(left_child) == -1) &&
                   (GetNodeBalance(node) == 1)) {
            right_child->GetBalance() = -1;
            left_child->GetBalance() = 0;
            node->GetBalance() = 0;

        } else if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(left_child) == -1) &&
                   (GetNodeBalance(node) == 0)) {
            right_child->GetBalance() = 0;
            left_child->GetBalance() = 0;
            node->GetBalance() = 0;
        }
    }

    std::pair<MapNode<K, V>*, MapNode<K, V>*> DoLeftRotate(std::unique_ptr<MapNode<K, V>>& node) {

        std::unique_ptr<MapNode<K, V>>& right_child = node->GetRight();
        std::unique_ptr<MapNode<K, V>>& left_subtree = node->GetLeft();
        std::unique_ptr<MapNode<K, V>>& middle_subtree = right_child->GetLeft();
        std::unique_ptr<MapNode<K, V>>& right_subtree = right_child->GetRight();

        auto parent_ptr = node->GetParent();
        bool left_node = (parent_ptr != nullptr) && (parent_ptr->GetLeft().get() == node.get());
        auto node_ptr = GetReleased(node);
        auto right_child_ptr = GetReleased(right_child);
        auto left_subtree_ptr = GetReleased(left_subtree);
        auto middle_subtree_ptr = GetReleased(middle_subtree);
        auto right_subtree_ptr = GetReleased(right_subtree);

        ConnectAfterRotation(parent_ptr, right_child_ptr, left_node);
        ConnectAfterRotation(right_child_ptr, node_ptr, true);
        ConnectAfterRotation(node_ptr, left_subtree_ptr, true);
        ConnectAfterRotation(node_ptr, middle_subtree_ptr, false);
        ConnectAfterRotation(right_child_ptr, right_subtree_ptr, false);

        return {node_ptr, right_child_ptr};
    }

    MapNode<K, V>* RotateLeft(std::unique_ptr<MapNode<K, V>>& node) {

        auto pair = DoLeftRotate(node);
        auto left_child_ptr = pair.first;
        auto node_ptr = pair.second;

        FixLeftBalance(left_child_ptr, node_ptr);
        return node_ptr;
    }

    std::pair<MapNode<K, V>*, MapNode<K, V>*> DoRightRotate(std::unique_ptr<MapNode<K, V>>& node) {

        std::unique_ptr<MapNode<K, V>>& left_child = node->GetLeft();
        std::unique_ptr<MapNode<K, V>>& left_subtree = left_child->GetLeft();
        std::unique_ptr<MapNode<K, V>>& middle_subtree = left_child->GetRight();
        std::unique_ptr<MapNode<K, V>>& right_subtree = node->GetRight();

        auto parent_ptr = node->GetParent();
        bool left_node = (parent_ptr != nullptr) && (parent_ptr->GetLeft().get() == node.get());
        auto node_ptr = GetReleased(node);
        auto left_child_ptr = GetReleased(left_child);
        auto left_subtree_ptr = GetReleased(left_subtree);
        auto middle_subtree_ptr = GetReleased(middle_subtree);
        auto right_subtree_ptr = GetReleased(right_subtree);

        ConnectAfterRotation(parent_ptr, left_child_ptr, left_node);
        ConnectAfterRotation(left_child_ptr, node_ptr, false);
        ConnectAfterRotation(left_child_ptr, left_subtree_ptr, true);
        ConnectAfterRotation(node_ptr, middle_subtree_ptr, true);
        ConnectAfterRotation(node_ptr, right_subtree_ptr, false);

        return {node_ptr, left_child_ptr};
    }

    MapNode<K, V>* RotateRight(std::unique_ptr<MapNode<K, V>>& node) {
        auto pair = DoRightRotate(node);
        auto right_child_ptr = pair.first;
        auto node_ptr = pair.second;

        FixRightBalance(right_child_ptr, node_ptr);
        return node_ptr;
    }

    MapNode<K, V>* RotateRightLeft(std::unique_ptr<MapNode<K, V>>& node) {

        auto pair = DoRightRotate(node->GetRight());
        auto right_child_ptr = pair.first;
        auto node_ptr = pair.second;

        auto pair2 = DoLeftRotate(node);
        auto left_child_ptr = pair2.first;

        FixRightLeftBalance(left_child_ptr, right_child_ptr, node_ptr);
        return node_ptr;
    }

    MapNode<K, V>* RotateLeftRight(std::unique_ptr<MapNode<K, V>>& node) {

        auto pair = DoLeftRotate(node->GetLeft());
        auto left_child_ptr = pair.first;
        auto node_ptr = pair.second;

        auto pair2 = DoRightRotate(node);
        auto right_child_ptr = pair2.first;

        FixLeftRightBalance(right_child_ptr, left_child_ptr, node_ptr);
        return node_ptr;
    }

    std::unique_ptr<MapNode<K, V>>& GetNodeUn(MapNode<K, V>* node) {
        if (node->GetParent() == nullptr) {
            return GetRoot();
        } else if (node->GetParent()->GetLeft().get() == node) {
            return node->GetParent()->GetLeft();
        } else {
            return node->GetParent()->GetRight();
        }
    }

    void BalanceAfterInsert(MapNode<K, V>* inserted_node) {
        MapNode<K, V>* current_node = inserted_node->GetParent();
        MapNode<K, V>* previous_node = inserted_node;
        while (current_node != nullptr) {
            if (current_node->GetLeft().get() == previous_node) {
                ++(current_node->GetBalance());
            } else {
                --(current_node->GetBalance());
            }
            if (current_node->GetBalance() == 0) {
                return;
            }
            if (std::abs(current_node->GetBalance()) == 1) {
                previous_node = current_node;
                current_node = current_node->GetParent();
            } else {
                std::unique_ptr<MapNode<K, V>>& current_node_smart = GetNodeUn(current_node);
                if (LeftRotateNeeded(current_node)) {
                    current_node = RotateLeft(current_node_smart);
                } else if (RightRotateNeded(current_node)) {
                    current_node = RotateRight(current_node_smart);
                } else if (RightLeftRotateNeeded(current_node)) {
                    current_node = RotateRightLeft(current_node_smart);
                } else if (LeftRightRotateNeeded(current_node)) {
                    current_node = RotateLeftRight(current_node_smart);
                }
                if (current_node->GetBalance() == 0) {
                    return;
                } else {
                    previous_node = current_node;
                    current_node = current_node->GetParent();
                }
            }
        }
    }

    CompressedPair<std::unique_ptr<MapNode<K, V>>, Compare> root_compare_;
    EndMapNode<K, V> end_node_{std::addressof(end_node_), std::addressof(end_node_)};
    size_t size_ = 0;
};

template <typename K, typename V, typename Compare>
bool operator==(const MapAVL<K, V, Compare>& lhs, const MapAVL<K, V, Compare>& rhs) {
    if (lhs.Size() != rhs.Size()) {
        return false;
    }

    Compare compare = lhs.KeyCompare();

    for (auto it = lhs.Begin(), jt = rhs.Begin(); it != lhs.End(); ++it, ++jt) {
        if (!Equivalent(it->first, jt->first, compare) || (it->second != jt->second)) {
            return false;
        }
    }
    return true;
}

template <typename K, typename V, typename Compare>
void Swap(MapAVL<K, V, Compare>& lhs, MapAVL<K, V, Compare>& rhs) {
    lhs.Swap(rhs);
}

template <typename K, typename V, typename Compare>
bool operator!=(const MapAVL<K, V, Compare>& lhs, const MapAVL<K, V, Compare>& rhs) {
    return !(lhs == rhs);
}

template <typename K, typename V>
size_t CalcNodeHeight(const MapNode<K, V>* node) {
    if (!node) {
        return 0;
    }
    size_t left_height = CalcNodeHeight(node->GetLeft().get());
    size_t right_height = CalcNodeHeight(node->GetRight().get());
    return 1 + std::max(left_height, right_height);
}

bool CheckAVLHeightBound(size_t size, size_t height) {
    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    double max_height =
        std::log(std::sqrt(5.0) * (size + 1 + std::sqrt(5.0) / 2.0)) / std::log(phi) - 2.0;
    return static_cast<double>(height) <= max_height;
}