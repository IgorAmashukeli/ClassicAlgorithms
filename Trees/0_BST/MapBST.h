#pragma once

#include <cassert>
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
class MapBaseMapNode {
public:
    MapBaseMapNode() noexcept = default;
    MapBaseMapNode(const MapBaseMapNode& other) = delete;
    MapBaseMapNode& operator=(const MapBaseMapNode& other) = delete;
    MapBaseMapNode(MapBaseMapNode&& other) = delete;
    MapBaseMapNode& operator=(MapBaseMapNode&& other) = delete;
    ~MapBaseMapNode() noexcept = default;

    virtual const K& GetKey() const = 0;
    virtual const V& GetValue() const = 0;
    virtual V& GetValue() = 0;
    virtual const std::unique_ptr<MapNode<K, V>>& GetLeft() const = 0;
    virtual std::unique_ptr<MapNode<K, V>>& GetLeft() = 0;
    virtual const std::unique_ptr<MapNode<K, V>>& GetRight() const = 0;
    virtual std::unique_ptr<MapNode<K, V>>& GetRight() = 0;
    virtual MapNode<K, V>* GetParent() const = 0;
    virtual MapNode<K, V>*& GetParent() = 0;
    virtual MapBaseMapNode<K, V>* GetPrev() const noexcept = 0;
    virtual MapBaseMapNode<K, V>*& GetPrev() noexcept = 0;
    virtual MapBaseMapNode<K, V>* GetNext() const noexcept = 0;
    virtual MapBaseMapNode<K, V>*& GetNext() noexcept = 0;
    virtual size_t GetSize() const = 0;
    virtual size_t& GetSize() = 0;
    virtual const std::pair<const K, V>& GetKeyValue() const = 0;
    virtual std::pair<const K, V>& GetKeyValue() = 0;
    virtual bool IsEndMapNode() const noexcept = 0;
};

template <typename K, typename V>
class MapNode : public MapBaseMapNode<K, V> {
public:
    MapNode() = default;
    MapNode(const MapNode& other) = delete;
    MapNode& operator=(const MapNode& other) = delete;
    MapNode(MapNode&& other) = delete;
    MapNode& operator=(MapNode&& other) = delete;
    ~MapNode() = default;

    MapNode(const K& key, const V& value, MapBaseMapNode<K, V>* prev, MapBaseMapNode<K, V>* next,
            size_t size)
        : key_value_(key, value), prev_(prev), next_(next), size_(size) {
    }
    MapNode(const std::pair<const K, V>& key_value, MapBaseMapNode<K, V>* prev,
            MapBaseMapNode<K, V>* next, size_t size)
        : key_value_(key_value), prev_(prev), next_(next), size_(size) {
    }
    MapNode(std::pair<const K, V>&& key_value, MapBaseMapNode<K, V>* prev,
            MapBaseMapNode<K, V>* next, size_t size)
        : key_value_(std::move(key_value)), prev_(prev), next_(next), size_(size) {
    }
    template <typename P>
    MapNode(P&& key_value, MapBaseMapNode<K, V>* prev, MapBaseMapNode<K, V>* next, size_t size)
        : key_value_(std::forward<P>(key_value)), prev_(prev), next_(next), size_(size) {
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
    MapBaseMapNode<K, V>* GetPrev() const noexcept {
        return prev_;
    }
    MapBaseMapNode<K, V>*& GetPrev() noexcept {
        return prev_;
    }
    MapBaseMapNode<K, V>* GetNext() const noexcept {
        return next_;
    }
    MapBaseMapNode<K, V>*& GetNext() noexcept {
        return next_;
    }
    size_t GetSize() const noexcept {
        return size_;
    }
    size_t& GetSize() noexcept {
        return size_;
    }
    const std::pair<const K, V>& GetKeyValue() const noexcept {
        return key_value_;
    }
    std::pair<const K, V>& GetKeyValue() noexcept {
        return key_value_;
    }
    bool IsEndMapNode() const noexcept {
        return false;
    }

private:
    std::pair<const K, V> key_value_;
    std::unique_ptr<MapNode<K, V>> left_;
    std::unique_ptr<MapNode<K, V>> right_;
    MapNode<K, V>* parent_ = nullptr;
    MapBaseMapNode<K, V>* prev_ = nullptr;
    MapBaseMapNode<K, V>* next_ = nullptr;
    size_t size_ = 1;
};

template <typename K, typename V>
class EndMapNode : public MapBaseMapNode<K, V> {

    EndMapNode() noexcept = default;
    EndMapNode(const EndMapNode& other) = delete;
    EndMapNode& operator=(const EndMapNode& other) = delete;
    EndMapNode(EndMapNode&& other) noexcept = default;
    EndMapNode& operator=(EndMapNode&& other) noexcept = default;
    ~EndMapNode() noexcept = default;

    EndMapNode(MapBaseMapNode<K, V>* prev, MapBaseMapNode<K, V>* next) noexcept
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
    MapBaseMapNode<K, V>* GetPrev() const noexcept {
        return prev_;
    }
    MapBaseMapNode<K, V>*& GetPrev() noexcept {
        return prev_;
    }
    MapBaseMapNode<K, V>* GetNext() const noexcept {
        return next_;
    }
    MapBaseMapNode<K, V>*& GetNext() noexcept {
        return next_;
    }
    size_t GetSize() const {
        throw std::out_of_range("Out of range!");
    }
    size_t& GetSize() {
        throw std::out_of_range("Out of range!");
    }
    const std::pair<const K, V>& GetKeyValue() const {
        throw std::out_of_range("Out of range!");
    }
    std::pair<const K, V>& GetKeyValue() {
        throw std::out_of_range("Out of range!");
    }
    bool IsEndMapNode() const noexcept {
        return true;
    }

private:
    MapBaseMapNode<K, V>* prev_ = nullptr;
    MapBaseMapNode<K, V>* next_ = nullptr;
};

template <typename K, typename V, typename Compare = std::less<K>>
class MapBST {
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
        explicit Iterator(MapBaseMapNode<K, V>* node) noexcept : node_(node) {
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
            if (node_ != nullptr) {
                node_ = node_->GetNext();
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetPrev()->IsEndMapNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        MapBaseMapNode<K, V>* node_ = nullptr;
    };

    class ConstIterator {
    public:
        explicit ConstIterator(const MapBaseMapNode<K, V>* node) noexcept : node_(node) {
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
        friend class MapBST;

    private:
        void Inc() {
            if (node_ != nullptr) {
                node_ = node_->GetNext();
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetPrev()->IsEndMapNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        const MapBaseMapNode<K, V>* node_ = nullptr;
    };

    class ReverseIterator {
    public:
        explicit ReverseIterator(MapBaseMapNode<K, V>* node) : node_(node) {
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
            if (node_ != nullptr) {
                node_ = node_->GetPrev();
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetNext()->IsEndMapNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        MapBaseMapNode<K, V>* node_ = nullptr;
    };

    class ConstReverseIterator {
    public:
        explicit ConstReverseIterator(const MapBaseMapNode<K, V>* node) noexcept : node_(node) {
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
            if (node_ != nullptr) {
                node_ = node_->GetPrev();
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetNext()->IsEndMapNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        const MapBaseMapNode<K, V>* node_ = nullptr;
    };

    MapBST() : MapBST(Compare()) {
    }
    explicit MapBST(const Compare& compare) : root_compare_(nullptr, compare) {
    }
    MapBST(const MapBST& other) {
        MapBaseMapNode<K, V>* max_node = Copy(other);
        ConnectEndMapNodesAfterCopy(max_node);
    }
    MapBST& operator=(const MapBST& other) {
        if (this != std::addressof(other)) {
            Clear();
            MapBaseMapNode<K, V>* max_node = Copy(other);
            ConnectEndMapNodesAfterCopy(max_node);
        }
        return *this;
    }
    MapBST(MapBST&& other) noexcept {
        Swap(other);
    }
    MapBST& operator=(MapBST&& other) noexcept {
        if (this != std::addressof(other)) {
            Swap(other);
            other.Clear();
        }
        return *this;
    }
    ~MapBST() = default;

    void Clear() noexcept {
        GetRoot() = nullptr;
        rend_node_.GetPrev() = nullptr;
        rend_node_.GetNext() = std::addressof(end_node_);
        end_node_.GetNext() = nullptr;
        end_node_.GetPrev() = std::addressof(rend_node_);
    }
    void Swap(MapBST& other) {
        std::swap(GetRoot(), other.GetRoot());
        ConnectEndMapNodesAfterSwap(other);
    }
    std::pair<Iterator, bool> Insert(const ValueType& key_value) {
        return InsertMapNode(key_value);
    }
    std::pair<Iterator, bool> Insert(ValueType&& key_value) {
        return InsertMapNode(std::move(key_value));
    }
    template <typename P>
    std::pair<Iterator, bool> Insert(P&& key_value) {
        return InsertMapNode(std::forward<P>(key_value));
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
        if (Equivalent(node->GetKey(), key)) {
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
        if (Equivalent(node->GetKey(), key)) {
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
        return Iterator{rend_node_.GetNext()};
    }
    ConstIterator Begin() const noexcept {
        return ConstIterator{rend_node_.GetNext()};
    }
    Iterator End() noexcept {
        return Iterator{std::addressof(end_node_)};
    }
    ConstIterator End() const noexcept {
        return ConstIterator{std::addressof(end_node_)};
    }
    ConstIterator CBegin() const noexcept {
        return ConstIterator{rend_node_.GetNext()};
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
        return ReverseIterator{std::addressof(rend_node_)};
    }
    ConstReverseIterator REnd() const noexcept {
        return ConstReverseIterator{std::addressof(rend_node_)};
    }
    ConstReverseIterator CRBegin() const noexcept {
        return ConstReverseIterator{end_node_.GetPrev()};
    }
    ConstReverseIterator CREnd() const noexcept {
        return ConstReverseIterator{std::addressof(rend_node_)};
    }

    size_t Size() const noexcept {
        if (GetRoot() == nullptr) {
            return 0;
        }
        return GetRoot()->GetSize();
    }
    size_t MaxSize() const noexcept {
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

    void ConnectEndMapNodesAfterSwap(MapBST& other) {
        if (GetRoot() != nullptr) {
            std::swap(rend_node_.GetNext(), other.rend_node_.GetNext());
            rend_node_.GetNext()->GetPrev() = std::addressof(rend_node_);
            other.rend_node_.GetNext()->GetPrev() = std::addressof(other.rend_node_);

            std::swap(end_node_.GetPrev(), other.end_node_.GetPrev());
            end_node_.GetPrev()->GetNext() = std::addressof(end_node_);
            other.end_node_.GetPrev()->GetNext() = std::addressof(other.end_node_);
        }
    }

    void ConnectEndMapNodesAfterCopy(MapBaseMapNode<K, V>* max_node) {
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
                                                MapBaseMapNode<K, V>*& prev_node) {
        auto node =
            std::make_unique<MapNode<K, V>>(top_other_node->GetKey(), top_other_node->GetValue(),
                                            nullptr, nullptr, top_other_node->GetSize());
        node->GetPrev() = prev_node;
        prev_node->GetNext() = node.get();
        prev_node = node.get();
        return node;
    }

    void PushOrRoot(const MapBST& other, std::stack<std::unique_ptr<MapNode<K, V>>>& nodes,
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

    void CopyIteration(const MapBST& other,
                       std::stack<std::tuple<MapNode<K, V>*, bool, bool>>& other_nodes,
                       std::stack<std::unique_ptr<MapNode<K, V>>>& nodes,
                       MapNode<K, V>* top_other_node, bool visit_left, bool visit_right,
                       MapBaseMapNode<K, V>*& prev_node) {
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

    MapBaseMapNode<K, V>* Copy(const MapBST& other) {
        if (other.Empty()) {
            return nullptr;
        }
        MapBaseMapNode<K, V>* prev_node = std::addressof(rend_node_);
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
        return prev_node;
    }

    void ConnectPrevNext(MapNode<K, V>* node, MapBaseMapNode<K, V>* prev,
                         MapBaseMapNode<K, V>* next) {
        node->GetNext() = next;
        next->GetPrev() = node;
        node->GetPrev() = prev;
        prev->GetNext() = node;
    }

    void IncreaseSizeInBranch(MapNode<K, V>* node) {
        while (node != nullptr) {
            ++(node->GetSize());
            node = node->GetParent();
        }
    }

    ConstIterator Select(size_t i) const {
    }

    size_t Rank(const K& key) const {
    }

    template <typename P>
    std::pair<Iterator, bool> InsertMapNode(P&& key_value) {
        MapNode<K, V>* node = GetRootPtr();
        MapNode<K, V>* parent = nullptr;
        bool left = false;
        MapBaseMapNode<K, V>* current_prev = std::addressof(rend_node_);
        MapBaseMapNode<K, V>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                GetRoot() = std::make_unique<MapNode<K, V>>(std::forward<P>(key_value),
                                                            std::addressof(rend_node_),
                                                            std::addressof(end_node_), 1);
                ConnectPrevNext(GetRootPtr(), current_prev, current_next);
                return {Iterator(GetRootPtr()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() = std::make_unique<MapNode<K, V>>(std::forward<P>(key_value),
                                                                    nullptr, nullptr, 1);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetLeft().get()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() = std::make_unique<MapNode<K, V>>(std::forward<P>(key_value),
                                                                     nullptr, nullptr, 1);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetRight().get()), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key_value.first, KeyCompare())) {
                return {Iterator(node), false};
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

    CompressedPair<std::unique_ptr<MapNode<K, V>>, Compare> root_compare_;
    EndMapNode<K, V> rend_node_{nullptr, std::addressof(end_node_)};
    EndMapNode<K, V> end_node_{std::addressof(rend_node_), nullptr};
};

template <typename K, typename V, typename Compare>
bool operator==(const MapBST<K, V, Compare>& lhs, const MapBST<K, V, Compare>& rhs) {
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
void Swap(const MapBST<K, V, Compare>& lhs, const MapBST<K, V, Compare>& rhs) {
    lhs.Swap(rhs);
}

template <typename K, typename V, typename Compare>
bool operator!=(const MapBST<K, V, Compare>& lhs, const MapBST<K, V, Compare>& rhs) {
    return (lhs != rhs);
}