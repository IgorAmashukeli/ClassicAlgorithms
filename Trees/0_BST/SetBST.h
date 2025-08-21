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
    virtual K& GetKey() = 0;
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

    SetNode(const K& key, SetBaseNode<K>* prev, SetBaseNode<K>* next)
        : key_(key), prev_(prev), next_(next) {
    }
    SetNode(K&& key, SetBaseNode<K>* prev, SetBaseNode<K>* next)
        : key_(std::move(key)), prev_(prev), next_(next) {
    }
    template <typename P>
    SetNode(P&& key, SetBaseNode<K>* prev, SetBaseNode<K>* next)
        : key_(std::forward<P>(key)), prev_(prev), next_(next) {
    }
    const K& GetKey() const noexcept {
        return key_;
    }
    K& GetKey() noexcept {
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
    bool IsSetEndNode() const noexcept {
        return false;
    }

private:
    K key_;
    std::unique_ptr<SetNode<K>> left_;
    std::unique_ptr<SetNode<K>> right_;
    SetNode<K>* parent_ = nullptr;
    SetBaseNode<K>* prev_ = nullptr;
    SetBaseNode<K>* next_ = nullptr;
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
    K& GetKey() {
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
    bool IsSetEndNode() const noexcept {
        return true;
    }

private:
    SetBaseNode<K>* prev_ = nullptr;
    SetBaseNode<K>* next_ = nullptr;
};

template <typename K, typename Compare = std::less<K>>
class SetBST {
public:
    enum {
        LEFT_VISITED = true,
        LEFT_NOT_VISITED = false,
        RIGHT_VISITED = true,
        RIGHT_NOT_VISITED = false
    };

    using SetType = K;
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
            if (node_ != nullptr && !node_->IsSetEndNode()) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
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
        friend class SetBST;

    private:
        void Inc() {
            if (node_ != nullptr && !node_->IsSetEndNode()) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
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
            if (node_ != nullptr && !node_->IsSetEndNode()) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
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
            if (node_ != nullptr && !node_->IsSetEndNode()) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
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

    SetBST() : SetBST(Compare()) {
    }
    explicit SetBST(const Compare& compare) : root_compare_(nullptr, compare) {
    }
    SetBST(const SetBST& other) {
        Copy(other);
    }
    SetBST& operator=(const SetBST& other) {
        return *this = SetBST(other);
    }
    SetBST(SetBST&& other) noexcept {
        Swap(other);
    }
    SetBST& operator=(SetBST&& other) noexcept {
        SetBST tmp = std::move(other);
        Swap(tmp);
        return *this;
    }
    ~SetBST() = default;

    void Clear() noexcept {
        GetRoot() = nullptr;
        size_ = 0;
        end_node_.GetNext() = std::addressof(end_node_);
        end_node_.GetPrev() = std::addressof(end_node_);
    }
    void Swap(SetBST& other) {
        std::swap(GetRoot(), other.GetRoot());
        std::swap(size_, other.size_);
        ConnectSetEndNodesAfterSwap(other);
    }
    std::pair<Iterator, bool> Insert(const K& key) {
        return InsertSetNode(key);
    }
    std::pair<Iterator, bool> Insert(K&& key) {
        return InsertSetNode(std::move(key));
    }
    template <typename P>
    std::pair<Iterator, bool> Insert(P&& key) {
        return InsertSetNode(std::forward<P>(key));
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

    void ConnectSetEndNodesAfterSwap(SetBST& other) {
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
        auto node = std::make_unique<SetNode<K>>(top_other_node->GetKey(), nullptr, nullptr);
        node->GetPrev() = prev_node;
        prev_node->GetNext() = node.get();
        prev_node = node.get();
        return node;
    }

    void PushOrRoot(const SetBST& other, std::stack<std::unique_ptr<SetNode<K>>>& nodes,
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

    void CopyIteration(const SetBST& other,
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

    void Copy(const SetBST& other) {
        if (other.Empty()) {
            return;
        }
        SetBaseNode<K>* prev_node = std::addressof(end_node_);
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
        size_ = other.size_;
    }

    void ConnectPrevNext(SetNode<K>* node, SetBaseNode<K>* prev, SetBaseNode<K>* next) {
        node->GetNext() = next;
        next->GetPrev() = node;
        node->GetPrev() = prev;
        prev->GetNext() = node;
    }

    void IncreaseSize() {
        ++size_;
    }

    template <typename P>
    std::pair<Iterator, bool> InsertSetNode(P&& key) {
        SetNode<K>* node = GetRootPtr();
        SetNode<K>* parent = nullptr;
        bool left = false;
        SetBaseNode<K>* current_prev = std::addressof(end_node_);
        SetBaseNode<K>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                GetRoot() = std::make_unique<SetNode<K>>(
                    std::forward<P>(key), std::addressof(end_node_), std::addressof(end_node_));
                ConnectPrevNext(GetRootPtr(), current_prev, current_next);
                IncreaseSize();
                return {Iterator(GetRootPtr()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() =
                    std::make_unique<SetNode<K>>(std::forward<P>(key), nullptr, nullptr);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSize();
                return {Iterator(parent->GetLeft().get()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() =
                    std::make_unique<SetNode<K>>(std::forward<P>(key), nullptr, nullptr);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSize();
                return {Iterator(parent->GetRight().get()), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key, KeyCompare())) {
                return {Iterator(node), false};
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

    CompressedPair<std::unique_ptr<SetNode<K>>, Compare> root_compare_;
    SetEndNode<K> end_node_{std::addressof(end_node_), std::addressof(end_node_)};
    size_t size_ = 0;
};

template <typename K, typename Compare>
bool operator==(const SetBST<K, Compare>& lhs, const SetBST<K, Compare>& rhs) {
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
void Swap(SetBST<K, Compare>& lhs, SetBST<K, Compare>& rhs) {
    lhs.Swap(rhs);
}

template <typename K, typename Compare>
bool operator!=(const SetBST<K, Compare>& lhs, const SetBST<K, Compare>& rhs) {
    return !(lhs == rhs);
}
