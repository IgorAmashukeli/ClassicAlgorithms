#pragma once

#include <algorithm>
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

template <typename K1, typename K2, typename Compare>
bool Equivalent(const K1& key_1, const K2& key_2, Compare compare) {
    return !(compare(key_1, key_2)) && (!compare(key_2, key_1));
}

template <typename K>
class SetNode;

template <typename K>
class SetBaseNode {
public:
    SetBaseNode() noexcept = default;
    SetBaseNode(const SetBaseNode& other) = delete;
    SetBaseNode& operator=(const SetBaseNode& other) = delete;
    SetBaseNode(SetBaseNode&& other) = delete;
    SetBaseNode& operator=(SetBaseNode&& other) = delete;
    ~SetBaseNode() noexcept = default;

    virtual const K& GetKey() const = 0;
    virtual const std::unique_ptr<SetNode<K>>& GetLeft() const = 0;
    virtual std::unique_ptr<SetNode<K>>& GetLeft() = 0;
    virtual const std::unique_ptr<SetNode<K>>& GetRight() const = 0;
    virtual std::unique_ptr<SetNode<K>>& GetRight() = 0;
    virtual SetNode<K>* GetParent() const = 0;
    virtual SetNode<K>*& GetParent() = 0;
    virtual SetBaseNode<K>* GetPrev() const noexcept = 0;
    virtual SetBaseNode<K>*& GetPrev() noexcept = 0;
    virtual SetBaseNode<K>* GetNext() const noexcept = 0;
    virtual SetBaseNode<K>*& GetNext() noexcept = 0;
    virtual size_t GetSize() const = 0;
    virtual size_t& GetSize() = 0;
    virtual signed char GetBalance() const = 0;
    virtual signed char& GetBalance() = 0;
    virtual bool IsSetEndNode() const noexcept = 0;
};

template <typename K>
class SetNode : public SetBaseNode<K> {
public:
    SetNode() = default;
    SetNode(const SetNode& other) = delete;
    SetNode& operator=(const SetNode& other) = delete;
    SetNode(SetNode&& other) = delete;
    SetNode& operator=(SetNode&& other) = delete;
    ~SetNode() = default;

    SetNode(const K& key, SetBaseNode<K>* prev, SetBaseNode<K>* next, size_t size,
            signed char balance)
        : key_(key), prev_(prev), next_(next), size_(size), balance_(balance) {
    }
    SetNode(K&& key, SetBaseNode<K>* prev, SetBaseNode<K>* next, size_t size, signed char balance)
        : key_(std::move(key)), prev_(prev), next_(next), size_(size), balance_(balance) {
    }
    template <typename P>
    SetNode(P&& key, SetBaseNode<K>* prev, SetBaseNode<K>* next, size_t size, signed char balance)
        : key_(std::forward<P>(key)), prev_(prev), next_(next), size_(size), balance_(balance) {
    }
    const K& GetKey() const noexcept {
        return key_;
    }
    const std::unique_ptr<SetNode<K>>& GetLeft() const noexcept {
        return left_;
    }
    std::unique_ptr<SetNode<K>>& GetLeft() noexcept {
        return left_;
    }
    const std::unique_ptr<SetNode<K>>& GetRight() const noexcept {
        return right_;
    }
    std::unique_ptr<SetNode<K>>& GetRight() noexcept {
        return right_;
    }
    SetNode<K>* GetParent() const noexcept {
        return parent_;
    }
    SetNode<K>*& GetParent() noexcept {
        return parent_;
    }
    SetBaseNode<K>* GetPrev() const noexcept {
        return prev_;
    }
    SetBaseNode<K>*& GetPrev() noexcept {
        return prev_;
    }
    SetBaseNode<K>* GetNext() const noexcept {
        return next_;
    }
    SetBaseNode<K>*& GetNext() noexcept {
        return next_;
    }
    size_t GetSize() const noexcept {
        return size_;
    }
    size_t& GetSize() noexcept {
        return size_;
    }
    signed char GetBalance() const {
        return balance_;
    }
    signed char& GetBalance() {
        return balance_;
    }
    bool IsSetEndNode() const noexcept {
        return false;
    }

private:
    const K key_;
    std::unique_ptr<SetNode<K>> left_;
    std::unique_ptr<SetNode<K>> right_;
    SetNode<K>* parent_ = nullptr;
    SetBaseNode<K>* prev_ = nullptr;
    SetBaseNode<K>* next_ = nullptr;
    size_t size_ = 1;
    signed char balance_ = 0;
};

template <typename K>
class SetEndNode : public SetBaseNode<K> {
public:
    SetEndNode() noexcept = default;
    SetEndNode(const SetEndNode& other) = delete;
    SetEndNode& operator=(const SetEndNode& other) = delete;
    SetEndNode(SetEndNode&& other) noexcept = default;
    SetEndNode& operator=(SetEndNode&& other) noexcept = default;
    ~SetEndNode() noexcept = default;

    SetEndNode(SetBaseNode<K>* prev, SetBaseNode<K>* next) noexcept : prev_(prev), next_(next) {
    }
    const K& GetKey() const {
        throw std::out_of_range("Out of range!");
    }
    const std::unique_ptr<SetNode<K>>& GetLeft() const {
        throw std::out_of_range("Out of range!");
    }
    std::unique_ptr<SetNode<K>>& GetLeft() {
        throw std::out_of_range("Out of range!");
    }
    const std::unique_ptr<SetNode<K>>& GetRight() const {
        throw std::out_of_range("Out of range!");
    }
    std::unique_ptr<SetNode<K>>& GetRight() {
        throw std::out_of_range("Out of range!");
    }
    SetNode<K>* GetParent() const {
        throw std::out_of_range("Out of range!");
    }
    SetNode<K>*& GetParent() {
        throw std::out_of_range("Out of range!");
    }
    SetBaseNode<K>* GetPrev() const noexcept {
        return prev_;
    }
    SetBaseNode<K>*& GetPrev() noexcept {
        return prev_;
    }
    SetBaseNode<K>* GetNext() const noexcept {
        return next_;
    }
    SetBaseNode<K>*& GetNext() noexcept {
        return next_;
    }
    size_t GetSize() const {
        throw std::out_of_range("Out of range!");
    }
    size_t& GetSize() {
        throw std::out_of_range("Out of range!");
    }
    signed char GetBalance() const {
        throw std::out_of_range("Out of range!");
    }
    signed char& GetBalance() {
        throw std::out_of_range("Out of range!");
    }
    bool IsSetEndNode() const noexcept {
        return true;
    }

private:
    SetBaseNode<K>* prev_ = nullptr;
    SetBaseNode<K>* next_ = nullptr;
};

template <typename K, typename Compare = std::less<K>>
class SetAVL {
public:
    enum {
        LEFT_VISITED = true,
        LEFT_NOT_VISITED = false,
        RIGHT_VISITED = true,
        RIGHT_NOT_VISITED = false
    };

    using SetType = const K;
    using Reference = SetType&;
    using Pointer = SetType*;
    using ConstReference = const SetType&;
    using ConstPointer = const SetType*;

    class ConstIterator;

    class Iterator {
    public:
        explicit Iterator(SetBaseNode<K>* node) noexcept : node_(node) {
        }
        Reference operator*() const {
            return node_->GetKey();
        }
        Pointer operator->() const {
            return std::addressof(node_->GetKey());
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
            if (node_ != nullptr && !(node_->GetPrev()->IsSetEndNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        SetBaseNode<K>* node_ = nullptr;
    };

    class ConstIterator {
    public:
        explicit ConstIterator(const SetBaseNode<K>* node) noexcept : node_(node) {
        }
        ConstIterator(Iterator it) noexcept : node_(it.node_) {
        }
        ConstReference operator*() const {
            return node_->GetKey();
        }
        ConstPointer operator->() const {
            return std::addressof(node_->GetKey());
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

    private:
        void Inc() {
            if (node_ != nullptr) {
                node_ = node_->GetNext();
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetPrev()->IsSetEndNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        const SetBaseNode<K>* node_ = nullptr;
    };

    class ConstReverseIterator;

    class ReverseIterator {
    public:
        explicit ReverseIterator(SetBaseNode<K>* node) : node_(node) {
        }
        Reference operator*() const {
            return node_->GetKey();
        }
        Pointer operator->() const {
            return std::addressof(node_->GetKey());
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
            if (node_ != nullptr && !(node_->GetNext()->IsSetEndNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        SetBaseNode<K>* node_ = nullptr;
    };

    class ConstReverseIterator {
    public:
        explicit ConstReverseIterator(const SetBaseNode<K>* node) noexcept : node_(node) {
        }
        ConstReverseIterator(ReverseIterator it) noexcept : node_(it.node_) {
        }
        ConstReference operator*() const noexcept {
            return node_->GetKey();
        }
        ConstPointer operator->() const noexcept {
            return std::addressof(node_->GetKey());
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
            if (node_ != nullptr && !(node_->GetNext()->IsSetEndNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        const SetBaseNode<K>* node_ = nullptr;
    };

    SetAVL() : SetAVL(Compare()) {
    }
    explicit SetAVL(const Compare& compare) : root_compare_(nullptr, compare) {
    }
    SetAVL(const SetAVL& other) {
        Copy(other);
    }
    SetAVL& operator=(const SetAVL& other) {
        return *this = SetAVL(other);
    }
    SetAVL(SetAVL&& other) noexcept {
        Swap(other);
    }
    SetAVL& operator=(SetAVL&& other) noexcept {
        SetAVL tmp = std::move(other);
        Swap(tmp);
        return *this;
    }
    ~SetAVL() = default;

    void Clear() noexcept {
        GetRoot() = nullptr;
        rend_node_.GetPrev() = nullptr;
        rend_node_.GetNext() = std::addressof(end_node_);
        end_node_.GetNext() = nullptr;
        end_node_.GetPrev() = std::addressof(rend_node_);
    }
    void Swap(SetAVL& other) {
        std::swap(GetRoot(), other.GetRoot());
        ConnectSetEndNodesAfterSwap(other);
    }
    std::pair<Iterator, bool> Insert(const SetType& key) {
        auto pair = InsertSetNode(key);
        auto node = pair.first;
        auto inserted = pair.second;
        if (!inserted) {
            return {Iterator(node), false};
        }
        BalanceAfterInsert(node);
        return {Iterator(node), true};
    }
    std::pair<Iterator, bool> Insert(SetType&& key) {
        auto pair = InsertSetNode(std::move(key));
        auto node = pair.first;
        auto inserted = pair.second;
        if (!inserted) {
            return {Iterator(node), false};
        }
        BalanceAfterInsert(node);
        return {Iterator(node), true};
    }
    template <typename P>
    std::pair<Iterator, bool> Insert(P&& key) {
        auto pair = InsertSetNode(std::forward<P>(key));
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
    void Insert(std::initializer_list<SetType> ilist) {
        Insert(ilist.begin(), ilist.end());
    }
    Iterator Find(const K& key) {
        auto node = FindSetNode(key);
        if (node == nullptr) {
            return End();
        }
        return Iterator{node};
    }
    ConstIterator Find(const K& key) const {
        auto node = FindSetNode(key);
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
        return FindSetNode(key) != nullptr;
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

    Iterator SelectInd0(size_t i) {
        return SelectInd1(i + 1);
    }
    ConstIterator SelectInd0(size_t i) const {
        return SelectInd1(i + 1);
    }
    Iterator SelectInd1(size_t i) {
        if (i > Size() || i == 0) {
            return End();
        }
        return Iterator(SelectNode(i));
    }
    ConstIterator SelectInd1(size_t i) const {
        if (i > Size() || i == 0) {
            return End();
        }
        return ConstIterator(SelectNode(i));
    }

    size_t RankInd1(const K& key) const {
        return RankKey(key);
    }
    size_t RankInd0(const K& key) const {
        return RankInd1(key) - 1;
    }

    size_t Size() const noexcept {
        if (GetRoot() == nullptr) {
            return 0;
        }
        return GetRoot()->GetSize();
    }
    size_t MaxSize() const noexcept {
        return (std::numeric_limits<std::ptrdiff_t>::max() / sizeof(SetNode<K>));
    }
    bool Empty() const noexcept {
        return (GetRoot() == nullptr);
    }
    Compare KeyCompare() const {
        return root_compare_.GetSecond();
    }
    const std::unique_ptr<SetNode<K>>& GetRoot() const {
        return root_compare_.GetFirst();
    }
    std::unique_ptr<SetNode<K>>& GetRoot() {
        return root_compare_.GetFirst();
    }
    SetNode<K>* GetRootPtr() const {
        return GetRoot().get();
    }

private:
    size_t GetNodeSize(SetNode<K>* node) const {
        if (node == nullptr) {
            return 0;
        }
        return node->GetSize();
    }
    signed char GetNodeBalance(SetNode<K>* node) const {
        if (node == nullptr) {
            return 0;
        }
        return node->GetBalance();
    }
    bool IsBalanceNormal(SetNode<K>* node) const {
        return std::abs(GetNodeBalance(node)) <= 2;
    }

    bool LeftRotateNeeded(SetNode<K>* node) {
        if (node == nullptr || node->GetRight() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == -2) && ((GetNodeBalance(node->GetRight().get()) == -1) ||
                                                (GetNodeBalance(node->GetRight().get()) == 0));
    }
    bool RightRotateNeded(SetNode<K>* node) {
        if (node == nullptr || node->GetLeft() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == 2) && ((GetNodeBalance(node->GetLeft().get()) == 1) ||
                                               (GetNodeBalance(node->GetLeft().get()) == 0));
    }
    bool RightLeftRotateNeeded(SetNode<K>* node) {
        if (node == nullptr || node->GetRight() == nullptr ||
            node->GetRight()->GetLeft() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == -2) && (GetNodeBalance(node->GetRight().get()) == 1);
    }
    bool LeftRightRotateNeeded(SetNode<K>* node) {
        if (node == nullptr || node->GetLeft() == nullptr ||
            node->GetLeft()->GetRight() == nullptr) {
            return false;
        }
        return (GetNodeBalance(node) == 2) && (GetNodeBalance(node->GetLeft().get()) == -1);
    }

    SetNode<K>* FindSetNode(const K& key) const {
        SetNode<K>* node = GetRootPtr();

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

    SetNode<K>* FindLowerBound(const K& key) const {
        SetNode<K>* node = GetRootPtr();
        SetNode<K>* best_bound = nullptr;

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

    void ConnectSetEndNodesAfterSwap(SetAVL& other) {
        if (GetRoot() != nullptr) {
            std::swap(rend_node_.GetNext(), other.rend_node_.GetNext());
            rend_node_.GetNext()->GetPrev() = std::addressof(rend_node_);
            other.rend_node_.GetNext()->GetPrev() = std::addressof(other.rend_node_);

            std::swap(end_node_.GetPrev(), other.end_node_.GetPrev());
            end_node_.GetPrev()->GetNext() = std::addressof(end_node_);
            other.end_node_.GetPrev()->GetNext() = std::addressof(other.end_node_);
        }
    }

    void ConnectSetEndNodesAfterCopy(SetBaseNode<K>* max_node) {
        if (GetRoot() != nullptr) {
            max_node->GetNext() = std::addressof(end_node_);
            end_node_.GetPrev() = max_node;
        }
    }

    void MarkVisited(std::stack<std::tuple<SetNode<K>*, bool, bool>>& nodes, bool left) const {
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

    void MakeVisited(std::stack<std::tuple<SetNode<K>*, bool, bool>>& nodes,
                     SetNode<K>* child) const {
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

    bool LeftNull(SetNode<K>* node) const {
        return node->GetLeft() == nullptr;
    }

    bool RightNull(SetNode<K>* node) const {
        return node->GetRight() == nullptr;
    }

    bool LeftVisited(SetNode<K>* node, bool visit_left) const {
        return (node->GetLeft() != nullptr) && (visit_left);
    }

    bool RightVisited(SetNode<K>* node, bool visit_right) const {
        return (node->GetRight() != nullptr) && (visit_right);
    }

    bool LeftNotVisited(SetNode<K>* node, bool visit_left) const {
        return (node->GetLeft() != nullptr) && (!visit_left);
    }

    bool RightNotVisited(SetNode<K>* node, bool visit_right) const {
        return (node->GetRight() != nullptr) && (!visit_right);
    }

    std::unique_ptr<SetNode<K>> CreateCopied(SetNode<K>* top_other_node,
                                             SetBaseNode<K>*& prev_node) {
        auto node =
            std::make_unique<SetNode<K>>(top_other_node->GetKey(), nullptr, nullptr,
                                         top_other_node->GetSize(), top_other_node->GetBalance());
        node->GetPrev() = prev_node;
        prev_node->GetNext() = node.get();
        prev_node = node.get();
        return node;
    }

    void PushOrRoot(const SetAVL& other, std::stack<std::unique_ptr<SetNode<K>>>& nodes,
                    std::unique_ptr<SetNode<K>>&& node, SetNode<K>* top_other_node) {
        if (top_other_node == other.GetRoot().get()) {
            GetRoot() = std::move(node);
        } else {
            nodes.push(std::move(node));
        }
    }

    void LNVRN(std::stack<std::tuple<SetNode<K>*, bool, bool>>& other_nodes,
               SetNode<K>* top_other_node) {
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_VISITED});
        other_nodes.push({top_other_node->GetLeft().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    void LNVRNV(std::stack<std::tuple<SetNode<K>*, bool, bool>>& other_nodes,
                SetNode<K>* top_other_node) {
        other_nodes.push({top_other_node->GetRight().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_NOT_VISITED});
        other_nodes.push({top_other_node->GetLeft().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    void LVRNV(std::stack<std::tuple<SetNode<K>*, bool, bool>>& other_nodes,
               SetNode<K>* top_other_node) {
        other_nodes.pop();
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_VISITED});
        other_nodes.push({top_other_node->GetRight().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    void LNRNV(std::stack<std::tuple<SetNode<K>*, bool, bool>>& other_nodes,
               SetNode<K>* top_other_node) {
        other_nodes.push({top_other_node, LEFT_VISITED, RIGHT_VISITED});
        other_nodes.push({top_other_node->GetRight().get(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
    }
    std::unique_ptr<SetNode<K>> ConnectL(std::unique_ptr<SetNode<K>>&& node,
                                         std::stack<std::unique_ptr<SetNode<K>>>& nodes) {
        node->GetLeft() = std::move(nodes.top());
        node->GetLeft()->GetParent() = node.get();
        nodes.pop();
        return node;
    }
    std::unique_ptr<SetNode<K>> ConnectR(std::stack<std::unique_ptr<SetNode<K>>>& nodes) {
        auto rhs = std::move(nodes.top());
        nodes.pop();
        auto current = std::move(nodes.top());
        nodes.pop();
        current->GetRight() = std::move(rhs);
        current->GetRight()->GetParent() = current.get();
        return current;
    }

    void CopyIteration(const SetAVL& other,
                       std::stack<std::tuple<SetNode<K>*, bool, bool>>& other_nodes,
                       std::stack<std::unique_ptr<SetNode<K>>>& nodes, SetNode<K>* top_other_node,
                       bool visit_left, bool visit_right, SetBaseNode<K>*& prev_node) {
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

    void Copy(const SetAVL& other) {
        if (other.Empty()) {
            return;
        }
        SetBaseNode<K>* prev_node = std::addressof(rend_node_);
        std::stack<std::tuple<SetNode<K>*, bool, bool>> other_nodes;
        std::stack<std::unique_ptr<SetNode<K>>> nodes;
        other_nodes.push({other.GetRootPtr(), LEFT_NOT_VISITED, RIGHT_NOT_VISITED});
        while (!other_nodes.empty()) {
            auto top_other_node = std::get<0>(other_nodes.top());
            auto visit_left = std::get<1>(other_nodes.top());
            auto visit_right = std::get<2>(other_nodes.top());
            other_nodes.pop();
            CopyIteration(other, other_nodes, nodes, top_other_node, visit_left, visit_right,
                          prev_node);
        }
        ConnectSetEndNodesAfterCopy(prev_node);
    }

    void ConnectPrevNext(SetNode<K>* node, SetBaseNode<K>* prev, SetBaseNode<K>* next) {
        node->GetNext() = next;
        next->GetPrev() = node;
        node->GetPrev() = prev;
        prev->GetNext() = node;
    }

    void IncreaseSizeInBranch(SetNode<K>* node) {
        while (node != nullptr) {
            ++(node->GetSize());
            node = node->GetParent();
        }
    }

    std::pair<SetNode<K>*, bool> InsertSetNode(const SetType& key) {
        SetNode<K>* node = GetRootPtr();
        SetNode<K>* parent = nullptr;
        bool left = false;
        SetBaseNode<K>* current_prev = std::addressof(rend_node_);
        SetBaseNode<K>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                GetRoot() = std::make_unique<SetNode<K>>(key, std::addressof(rend_node_),
                                                         std::addressof(end_node_), 1, 0);
                ConnectPrevNext(GetRootPtr(), current_prev, current_next);
                return {GetRootPtr(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() = std::make_unique<SetNode<K>>(key, nullptr, nullptr, 1, 0);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {parent->GetLeft().get(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() = std::make_unique<SetNode<K>>(key, nullptr, nullptr, 1, 0);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {parent->GetRight().get(), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key, KeyCompare())) {
                return {node, false};
            }
            if ((node != nullptr) && (KeyCompare()(key, node->GetKey()))) {
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
    std::pair<SetNode<K>*, bool> InsertSetNode(SetType&& key) {
        SetNode<K>* node = GetRootPtr();
        SetNode<K>* parent = nullptr;
        bool left = false;
        SetBaseNode<K>* current_prev = std::addressof(rend_node_);
        SetBaseNode<K>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                GetRoot() = std::make_unique<SetNode<K>>(std::move(key), std::addressof(rend_node_),
                                                         std::addressof(end_node_), 1, 0);
                ConnectPrevNext(GetRootPtr(), current_prev, current_next);
                return {GetRootPtr(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() =
                    std::make_unique<SetNode<K>>(std::move(key), nullptr, nullptr, 1, 0);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {parent->GetLeft().get(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() =
                    std::make_unique<SetNode<K>>(std::move(key), nullptr, nullptr, 1, 0);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {parent->GetRight().get(), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key, KeyCompare())) {
                return {node, false};
            }
            if ((node != nullptr) && (KeyCompare()(key, node->GetKey()))) {
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
    template <typename P>
    std::pair<SetNode<K>*, bool> InsertSetNode(P&& key) {
        SetNode<K>* node = GetRootPtr();
        SetNode<K>* parent = nullptr;
        bool left = false;
        SetBaseNode<K>* current_prev = std::addressof(rend_node_);
        SetBaseNode<K>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                GetRoot() =
                    std::make_unique<SetNode<K>>(std::forward<P>(key), std::addressof(rend_node_),
                                                 std::addressof(end_node_), 1, 0);
                ConnectPrevNext(GetRootPtr(), current_prev, current_next);
                return {GetRootPtr(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() =
                    std::make_unique<SetNode<K>>(std::forward<P>(key), nullptr, nullptr, 1, 0);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {parent->GetLeft().get(), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() =
                    std::make_unique<SetNode<K>>(std::forward<P>(key), nullptr, nullptr, 1, 0);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {parent->GetRight().get(), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key, KeyCompare())) {
                return {node, false};
            }
            if ((node != nullptr) && (KeyCompare()(key, node->GetKey()))) {
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

    size_t GetNumInSubTree(SetNode<K>* node) const {
        if (node == nullptr || node->GetLeft() == nullptr) {
            return 1;
        }
        return (node->GetLeft()->GetSize() + 1);
    }

    SetNode<K>* SelectNode(size_t i) const {
        SetNode<K>* node = GetRootPtr();
        size_t current_size = GetNumInSubTree(node);
        while (current_size != i) {
            if (i < current_size) {
                node = node->GetLeft().get();
            } else {
                node = node->GetRight().get();
                i -= current_size;
            }
            current_size = GetNumInSubTree(node);
        }
        return node;
    }

    size_t RankKey(const K& key) const {
        SetNode<K>* node = GetRootPtr();
        size_t current_size = GetNumInSubTree(node);

        while (node != nullptr) {
            if (Equivalent(key, node->GetKey(), KeyCompare())) {
                return current_size;
            }
            if (KeyCompare()(key, node->GetKey())) {
                size_t parent_size = GetNumInSubTree(node);
                node = node->GetLeft().get();
                current_size = current_size - parent_size + GetNumInSubTree(node);
            } else {
                node = node->GetRight().get();
                current_size += GetNumInSubTree(node);
            }
        }
        return current_size;
    }

    SetNode<K>* GetReleased(std::unique_ptr<SetNode<K>>& node) {
        if (node != nullptr) {
            return node.release();
        }
        return nullptr;
    }

    void ConnectAfterRotation(SetNode<K>* parent, SetNode<K>* child, bool left) {
        if (child != nullptr) {
            child->GetParent() = parent;
        }
        if (parent != nullptr && left) {
            parent->GetLeft() = std::unique_ptr<SetNode<K>>(child);
        } else if (parent != nullptr) {
            parent->GetRight() = std::unique_ptr<SetNode<K>>(child);
        } else {
            GetRoot() = std::unique_ptr<SetNode<K>>(child);
        }
    }

    void FixLeftBalance(SetNode<K>* left_child, SetNode<K>* node) {
        assert(left_child != nullptr);
        assert(node != nullptr);
        if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(node) == -1)) {
            left_child->GetBalance() = 0;
            node->GetBalance() = 0;
        } else if ((GetNodeBalance(left_child) == -2) && (GetNodeBalance(node) == 0)) {
            left_child->GetBalance() = -1;
            node->GetBalance() = 1;
        } else {
            assert(false);
        }
    }
    void FixRightBalance(SetNode<K>* right_child, SetNode<K>* node) {
        assert(right_child != nullptr);
        assert(node != nullptr);
        if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(node) == 1)) {
            right_child->GetBalance() = 0;
            node->GetBalance() = 0;
        } else if ((GetNodeBalance(right_child) == 2) && (GetNodeBalance(node) == 0)) {
            right_child->GetBalance() = 1;
            node->GetBalance() = -1;
        } else {
            assert(false);
        }
    }
    void FixRightLeftBalance(SetNode<K>* left_child, SetNode<K>* right_child, SetNode<K>* node) {
        assert(left_child != nullptr);
        assert(right_child != nullptr);
        assert(node != nullptr);

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

        } else {
            assert(false);
        }
    }
    void FixLeftRightBalance(SetNode<K>* right_child, SetNode<K>* left_child, SetNode<K>* node) {
        assert(right_child != nullptr);
        assert(left_child != nullptr);
        assert(node != nullptr);

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

        } else {
            assert(false);
        }
    }
    void FixSize(SetNode<K>* node) {
        node->GetSize() =
            GetNodeSize(node->GetLeft().get()) + GetNodeSize(node->GetRight().get()) + 1;
    }

    std::pair<SetNode<K>*, SetNode<K>*> DoLeftRotate(std::unique_ptr<SetNode<K>>& node) {
        assert(node != nullptr);
        assert(node->GetRight() != nullptr);

        std::unique_ptr<SetNode<K>>& right_child = node->GetRight();
        std::unique_ptr<SetNode<K>>& left_subtree = node->GetLeft();
        std::unique_ptr<SetNode<K>>& middle_subtree = right_child->GetLeft();
        std::unique_ptr<SetNode<K>>& right_subtree = right_child->GetRight();

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

        FixSize(node_ptr);
        FixSize(right_child_ptr);

        return {node_ptr, right_child_ptr};
    }

    // may be written
    SetNode<K>* RotateLeft(std::unique_ptr<SetNode<K>>& node) {

        auto pair = DoLeftRotate(node);
        auto left_child_ptr = pair.first;
        auto node_ptr = pair.second;

        FixLeftBalance(left_child_ptr, node_ptr);
        return node_ptr;
    }

    std::pair<SetNode<K>*, SetNode<K>*> DoRightRotate(std::unique_ptr<SetNode<K>>& node) {
        assert(node != nullptr);
        assert(node->GetLeft() != nullptr);

        std::unique_ptr<SetNode<K>>& left_child = node->GetLeft();
        std::unique_ptr<SetNode<K>>& left_subtree = left_child->GetLeft();
        std::unique_ptr<SetNode<K>>& middle_subtree = left_child->GetRight();
        std::unique_ptr<SetNode<K>>& right_subtree = node->GetRight();

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

        FixSize(node_ptr);
        FixSize(left_child_ptr);

        return {node_ptr, left_child_ptr};
    }

    // maybe written
    SetNode<K>* RotateRight(std::unique_ptr<SetNode<K>>& node) {
        auto pair = DoRightRotate(node);
        auto right_child_ptr = pair.first;
        auto node_ptr = pair.second;

        FixRightBalance(right_child_ptr, node_ptr);
        return node_ptr;
    }

    SetNode<K>* RotateRightLeft(std::unique_ptr<SetNode<K>>& node) {
        assert(node != nullptr);
        assert(node->GetRight() != nullptr);
        assert(node->GetRight()->GetLeft() != nullptr);

        assert(node->GetBalance() == -2);
        assert(node->GetRight()->GetBalance() == 1);

        auto pair = DoRightRotate(node->GetRight());
        auto right_child_ptr = pair.first;
        auto node_ptr = pair.second;

        auto pair2 = DoLeftRotate(node);
        auto left_child_ptr = pair2.first;
        assert(node_ptr == pair2.second);

        FixRightLeftBalance(left_child_ptr, right_child_ptr, node_ptr);
        return node_ptr;
    }

    SetNode<K>* RotateLeftRight(std::unique_ptr<SetNode<K>>& node) {
        assert(node != nullptr);
        assert(node->GetLeft() != nullptr);
        assert(node->GetLeft()->GetRight() != nullptr);

        assert(node->GetBalance() == 2);
        assert(node->GetLeft()->GetBalance() == -1);

        auto pair = DoLeftRotate(node->GetLeft());
        auto left_child_ptr = pair.first;
        auto node_ptr = pair.second;

        auto pair2 = DoRightRotate(node);
        auto right_child_ptr = pair2.first;
        assert(node_ptr == pair2.second);

        FixLeftRightBalance(right_child_ptr, left_child_ptr, node_ptr);
        return node_ptr;
    }

    std::unique_ptr<SetNode<K>>& GetNodeUn(SetNode<K>* node) {
        if (node->GetParent() == nullptr) {
            return GetRoot();
        } else if (node->GetParent()->GetLeft().get() == node) {
            return node->GetParent()->GetLeft();
        } else {
            assert(node->GetParent()->GetRight().get() == node);
            return node->GetParent()->GetRight();
        }
    }

    // maybe finished
    void BalanceAfterInsert(SetNode<K>* inserted_node) {
        SetNode<K>* current_node = inserted_node->GetParent();
        SetNode<K>* previous_node = inserted_node;
        while (current_node != nullptr) {
            if (current_node->GetLeft().get() == previous_node) {
                ++(current_node->GetBalance());
            } else {
                assert(current_node->GetRight().get() == previous_node);
                --(current_node->GetBalance());
            }
            assert(IsBalanceNormal(current_node));
            if (current_node->GetBalance() == 0) {
                return;
            }
            if (std::abs(current_node->GetBalance()) == 1) {
                previous_node = current_node;
                current_node = current_node->GetParent();
            } else {
                assert(std::abs(current_node->GetBalance()) == 2);
                std::unique_ptr<SetNode<K>>& current_node_smart = GetNodeUn(current_node);
                if (LeftRotateNeeded(current_node)) {
                    current_node = RotateLeft(current_node_smart);
                } else if (RightRotateNeded(current_node)) {
                    current_node = RotateRight(current_node_smart);
                } else if (RightLeftRotateNeeded(current_node)) {
                    current_node = RotateRightLeft(current_node_smart);
                } else if (LeftRightRotateNeeded(current_node)) {
                    current_node = RotateLeftRight(current_node_smart);
                } else {
                    assert(false);
                }
                if (current_node->GetBalance() == 0) {
                    return;
                } else {
                    assert(std::abs(current_node->GetBalance()) == 1);
                    previous_node = current_node;
                    current_node = current_node->GetParent();
                }
            }
        }
    }

    CompressedPair<std::unique_ptr<SetNode<K>>, Compare> root_compare_;
    SetEndNode<K> rend_node_{nullptr, std::addressof(end_node_)};
    SetEndNode<K> end_node_{std::addressof(rend_node_), nullptr};
};

template <typename K, typename Compare>
bool operator==(const SetAVL<K, Compare>& lhs, const SetAVL<K, Compare>& rhs) {
    if (lhs.Size() != rhs.Size()) {
        return false;
    }

    Compare compare = lhs.KeyCompare();

    for (auto it = lhs.Begin(), jt = rhs.Begin(); it != lhs.End(); ++it, ++jt) {
        if (!Equivalent(*it, *jt, compare)) {
            return false;
        }
    }
    return true;
}

template <typename K, typename Compare>
void Swap(const SetAVL<K, Compare>& lhs, const SetAVL<K, Compare>& rhs) {
    lhs.Swap(rhs);
}

template <typename K, typename Compare>
bool operator!=(const SetAVL<K, Compare>& lhs, const SetAVL<K, Compare>& rhs) {
    return (lhs != rhs);
}

template <typename K>
size_t CalcNodeHeight(const SetNode<K>* node) {
    if (!node) {
        return 0;
    }
    size_t left_height = CalcNodeHeight(node->GetLeft().get());
    size_t right_height = CalcNodeHeight(node->GetRight().get());
    return 1 + std::max(left_height, right_height);
}

// Helper function to check AVL height bound
bool CheckAVLHeightBound(size_t size, size_t height) {
    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;  // Golden ratio ≈ 1.6180339887
    // Maximum height: h ≤ log_φ(√5 * (n + 1 + √5/2)) - 2
    double max_height =
        std::log(std::sqrt(5.0) * (size + 1 + std::sqrt(5.0) / 2.0)) / std::log(phi) - 2.0;
    return static_cast<double>(height) <= max_height;
}