#pragma once

#include <cstddef>
#include <limits>
#include <memory>
#include <stack>
#include <stdexcept>
#include <utility>
#include <iostream>

template <typename K>
bool Equivalent(const K& key_1, const K& key_2) {
    return !(key_1 < key_2) && (!(key_2 < key_1));
}

template <typename K, typename V>
struct Node;

template <typename K, typename V>
struct BaseNode {
    BaseNode() noexcept = default;
    BaseNode(const BaseNode& other) = delete;
    BaseNode& operator=(const BaseNode& other) = delete;
    BaseNode(BaseNode&& other) = delete;
    BaseNode& operator=(BaseNode&& other) = delete;
    ~BaseNode() noexcept = default;

    virtual const K& GetKey() const = 0;
    virtual const V& GetValue() const = 0;
    virtual V& GetValue() = 0;
    virtual const std::unique_ptr<Node<K, V>>& GetLeft() const = 0;
    virtual std::unique_ptr<Node<K, V>>& GetLeft() = 0;
    virtual const std::unique_ptr<Node<K, V>>& GetRight() const = 0;
    virtual std::unique_ptr<Node<K, V>>& GetRight() = 0;
    virtual Node<K, V>* GetParent() const = 0;
    virtual Node<K, V>*& GetParent() = 0;
    virtual BaseNode<K, V>* GetPrev() const noexcept = 0;
    virtual BaseNode<K, V>*& GetPrev() noexcept = 0;
    virtual BaseNode<K, V>* GetNext() const noexcept = 0;
    virtual BaseNode<K, V>*& GetNext() noexcept = 0;
    virtual size_t GetSize() const = 0;
    virtual size_t& GetSize() = 0;
    virtual const std::pair<const K, V>& GetKeyValue() const = 0;
    virtual std::pair<const K, V>& GetKeyValue() = 0;
    virtual bool IsEndNode() const noexcept = 0;
};

template <typename K, typename V>
struct Node : public BaseNode<K, V> {
    std::pair<const K, V> key_value_;
    std::unique_ptr<Node<K, V>> left_;
    std::unique_ptr<Node<K, V>> right_;
    Node<K, V>* parent_ = nullptr;
    BaseNode<K, V>* prev_ = nullptr;
    BaseNode<K, V>* next_ = nullptr;
    size_t size_ = 1;

    Node() = default;
    Node(const Node& other) = delete;
    Node& operator=(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node& operator=(Node&& other) = delete;
    ~Node() = default;

    Node(const K& key, const V& value, BaseNode<K, V>* prev, BaseNode<K, V>* next, size_t size)
        : key_value_(key, value), prev_(prev), next_(next), size_(size) {
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
    const std::unique_ptr<Node<K, V>>& GetLeft() const noexcept {
        return left_;
    }
    std::unique_ptr<Node<K, V>>& GetLeft() noexcept {
        return left_;
    }
    const std::unique_ptr<Node<K, V>>& GetRight() const noexcept {
        return right_;
    }
    std::unique_ptr<Node<K, V>>& GetRight() noexcept {
        return right_;
    }
    Node<K, V>* GetParent() const noexcept {
        return parent_;
    }
    Node<K, V>*& GetParent() noexcept {
        return parent_;
    }
    BaseNode<K, V>* GetPrev() const noexcept {
        return prev_;
    }
    BaseNode<K, V>*& GetPrev() noexcept {
        return prev_;
    }
    BaseNode<K, V>* GetNext() const noexcept {
        return next_;
    }
    BaseNode<K, V>*& GetNext() noexcept {
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
    bool IsEndNode() const noexcept {
        return false;
    }
};

template <typename K, typename V>
struct EndNode : public BaseNode<K, V> {
    BaseNode<K, V>* prev_ = nullptr;
    BaseNode<K, V>* next_ = nullptr;

    EndNode() noexcept = default;
    EndNode(const EndNode& other) = delete;
    EndNode& operator=(const EndNode& other) = delete;
    EndNode(EndNode&& other) noexcept = default;
    EndNode& operator=(EndNode&& other) noexcept = default;
    ~EndNode() noexcept = default;

    EndNode(BaseNode<K, V>* prev, BaseNode<K, V>* next) noexcept : prev_(prev), next_(next) {
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
    const std::unique_ptr<Node<K, V>>& GetLeft() const {
        throw std::out_of_range("Out of range!");
    }
    std::unique_ptr<Node<K, V>>& GetLeft() {
        throw std::out_of_range("Out of range!");
    }
    const std::unique_ptr<Node<K, V>>& GetRight() const {
        throw std::out_of_range("Out of range!");
    }
    std::unique_ptr<Node<K, V>>& GetRight() {
        throw std::out_of_range("Out of range!");
    }
    Node<K, V>* GetParent() const {
        throw std::out_of_range("Out of range!");
    }
    Node<K, V>*& GetParent() {
        throw std::out_of_range("Out of range!");
    }
    BaseNode<K, V>* GetPrev() const noexcept {
        return prev_;
    }
    BaseNode<K, V>*& GetPrev() noexcept {
        return prev_;
    }
    BaseNode<K, V>* GetNext() const noexcept {
        return next_;
    }
    BaseNode<K, V>*& GetNext() noexcept {
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
    bool IsEndNode() const noexcept {
        return true;
    }
};

template <typename K, typename V>
class BST {
public:
    using ValueType = std::pair<const K, V>;
    using Reference = ValueType&;
    using Pointer = ValueType*;
    using ConstReference = const ValueType&;
    using ConstPointer = const ValueType*;

    class Iterator {
    public:
        Iterator() noexcept = default;
        Iterator(const Iterator& other) noexcept = default;
        Iterator& operator=(const Iterator& other) noexcept = default;
        Iterator(Iterator&& other) noexcept = default;
        Iterator& operator=(Iterator&& other) noexcept = default;
        ~Iterator() noexcept = default;

        explicit Iterator(BaseNode<K, V>* node) noexcept : node_(node) {
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
            if (node_ != nullptr && !(node_->GetPrev()->IsEndNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        BaseNode<K, V>* node_ = nullptr;
    };

    class ConstIterator {
    public:
        ConstIterator() noexcept = default;
        ConstIterator(const ConstIterator& other) noexcept = default;
        ConstIterator& operator=(const ConstIterator& other) noexcept = default;
        ConstIterator(ConstIterator&& other) noexcept = default;
        ConstIterator& operator=(ConstIterator&& other) noexcept = default;
        ~ConstIterator() noexcept = default;

        explicit ConstIterator(const BaseNode<K, V>* node) noexcept : node_(node) {
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

    private:
        void Inc() {
            if (node_ != nullptr) {
                node_ = node_->GetNext();
            }
        }
        void Dec() {
            if (node_ != nullptr && !(node_->GetPrev()->IsEndNode())) {
                node_ = node_->GetPrev();
            } else {
                node_ = nullptr;
            }
        }
        const BaseNode<K, V>* node_ = nullptr;
    };

    class ReverseIterator {
    public:
        ReverseIterator() noexcept = default;
        ReverseIterator(const ReverseIterator& other) noexcept = default;
        ReverseIterator& operator=(const ReverseIterator& other) noexcept = default;
        ReverseIterator(ReverseIterator&& other) noexcept = default;
        ReverseIterator& operator=(ReverseIterator&& other) noexcept = default;
        ~ReverseIterator() noexcept = default;

        explicit ReverseIterator(BaseNode<K, V>* node) : node_(node) {
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
            if (node_ != nullptr && !(node_->GetNext()->IsEndNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        BaseNode<K, V>* node_ = nullptr;
    };

    class ConstReverseIterator {
    public:
        ConstReverseIterator() noexcept = default;
        ConstReverseIterator(const ConstReverseIterator& other) noexcept = default;
        ConstReverseIterator& operator=(const ConstReverseIterator& other) noexcept = default;
        ConstReverseIterator(ConstReverseIterator&& other) noexcept = default;
        ConstReverseIterator& operator=(ConstReverseIterator&& other) noexcept = default;
        ~ConstReverseIterator() noexcept = default;

        explicit ConstReverseIterator(const BaseNode<K, V>* node) noexcept : node_(node) {
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
            if (node_ != nullptr && !(node_->GetNext()->IsEndNode())) {
                node_ = node_->GetNext();
            } else {
                node_ = nullptr;
            }
        }
        const BaseNode<K, V>* node_ = nullptr;
    };

    BST() = default;
    BST(const BST& other) {
        BaseNode<K, V>* node = std::addressof(rend_node_);
        root_ = Copy(other.root_, node);
        ConnectEndNodesAfterCopy(node);
    }
    BST& operator=(const BST& other) {
        if (this != std::addressof(other)) {
            Clear();
            BaseNode<K, V>* node = std::addressof(rend_node_);
            root_ = Copy(other.root_, node);
            ConnectEndNodesAfterCopy(node);
        }
        return *this;
    }
    BST(BST&& other) {
        Swap(other);
    }
    BST& operator=(BST&& other) {
        if (this != std::addressof(other)) {
            Swap(other);
            other.Clear();
        }
        return *this;
    }
    ~BST() = default;

    void Clear() noexcept {
        root_ = nullptr;
        rend_node_.GetPrev() = nullptr;
        rend_node_.GetNext() = std::addressof(end_node_);
        end_node_.GetNext() = nullptr;
        end_node_.GetPrev() = std::addressof(rend_node_);
    }
    void Swap(BST<K, V>& other) {
        std::swap(root_, other.root_);
        ConnectEndNodesAfterSwap(other);
    }
    std::pair<Iterator, bool> Insert(const ValueType& key_value) {
        return InsertNode(key_value);
    }
    std::pair<Iterator, bool> Insert(ValueType&& key_value) {
        return InsertNode(std::move(key_value));
    }
    template <typename P>
    std::pair<Iterator, bool> Insert(P&& key_value) {
        return InsertNode(std::forward<P>(key_value));
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
        auto node = FindNode(key);
        if (node == nullptr) {
            return End();
        }
        return Iterator{node};
    }
    ConstIterator Find(const K& key) const {
        auto node = FindNode(key);
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
        if (Equivalent(node->GetKey(), key)) {
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
        if (Equivalent(node->GetKey(), key)) {
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
        return FindNode(key) != nullptr;
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
        if (root_ == nullptr) {
            return 0;
        }
        return root_->GetSize();
    }
    size_t MaxSize() const noexcept {
        return (std::numeric_limits<std::ptrdiff_t>::max() / sizeof(Node<K, V>));
    }
    bool Empty() const noexcept {
        return (root_ == nullptr);
    }

private:
    Node<K, V>* FindNode(const K& key) const {
        Node<K, V>* node = root_.get();

        while (node != nullptr) {
            if (Equivalent(key, node->GetKey())) {
                return node;
            }
            if (key < node->GetKey()) {
                node = node->GetLeft().get();
            } else {
                node = node->GetRight().get();
            }
        }
        return nullptr;
    }

    Node<K, V>* FindLowerBound(const K& key) const {
        Node<K, V>* node = root_.get();
        Node<K, V>* best_bound = nullptr;

        while (node != nullptr) {
            if (Equivalent(key, node->GetKey())) {
                return node;
            }
            if (key < node->GetKey()) {
                best_bound = node;
                node = node->GetLeft().get();
            } else {
                node = node->GetRight().get();
            }
        }
        return best_bound;
    }

    void ConnectEndNodesAfterSwap(BST<K, V>& other) {
        if (root_ != nullptr) {
            std::swap(rend_node_.GetNext(), other.rend_node_.GetNext());
            rend_node_.GetNext()->GetPrev() = std::addressof(rend_node_);
            other.rend_node_.GetNext()->GetPrev() = std::addressof(other.rend_node_);

            std::swap(end_node_.GetPrev(), other.end_node_.GetPrev());
            end_node_.GetPrev()->GetNext() = std::addressof(end_node_);
            other.end_node_.GetPrev()->GetNext() = std::addressof(other.end_node_);
        }
    }

    void ConnectEndNodesAfterCopy(BaseNode<K, V>* max_node) {
        if (root_ != nullptr) {
            max_node->GetNext() = std::addressof(end_node_);
            end_node_.GetPrev() = max_node;
        }
    }

    // not finished yet
    void Copy(const BST& other) {
        if (other.Empty()) {
            return;
        }
        Node<K, V>* other_node = other.root_.get();
        BaseNode<K, V>* prev_node = std::addressof(rend_node_);

        while (true) {

            while (other_node->GetLeft() != nullptr) {
                other_node = other_node->GetLeft();
            }

            std::unique_ptr<Node<K, V>> node =
                std::make_unique<Node<K, V>>(other_node->GetKey(), other_node->GetValue(), nullptr,
                                             nullptr, other_node->GetSize());

            while (other_node->GetRight() == nullptr && other_node->GetParent() != nullptr) {
                other_node = other_node->GetParent();
            }
            other_node = other_node->GetRight();
        }
    }

    std::unique_ptr<Node<K, V>> Copy(const std::unique_ptr<Node<K, V>>& other_node,
                                     BaseNode<K, V>*& prev_node) {
        if (other_node == nullptr) {
            return nullptr;
        }

        std::unique_ptr<Node<K, V>> lhs = Copy(other_node->GetLeft(), prev_node);

        std::unique_ptr<Node<K, V>> node = std::make_unique<Node<K, V>>(
            other_node->GetKey(), other_node->GetValue(), nullptr, nullptr, other_node->GetSize());

        node->GetLeft() = std::move(lhs);
        if (node->GetLeft() != nullptr) {
            node->GetLeft()->GetParent() = node.get();
        }

        prev_node->GetNext() = node.get();
        node.get()->GetPrev() = prev_node;

        prev_node = node.get();

        node->GetRight() = Copy(other_node->GetRight(), prev_node);
        if (node->GetRight() != nullptr) {
            node->GetRight()->GetParent() = node.get();
        }

        return node;
    }

    void ConnectPrevNext(Node<K, V>* node, BaseNode<K, V>* prev, BaseNode<K, V>* next) {
        node->GetNext() = next;
        next->GetPrev() = node;

        node->GetPrev() = prev;
        prev->GetNext() = node;
    }

    void IncreaseSizeInBranch(Node<K, V>* node) {
        while (node != nullptr) {
            ++(node->GetSize());
            node = node->GetParent();
        }
    }

    std::pair<Iterator, bool> InsertNode(const ValueType& key_value) {
        Node<K, V>* node = root_.get();
        Node<K, V>* parent = nullptr;
        bool left = false;
        BaseNode<K, V>* current_prev = std::addressof(rend_node_);
        BaseNode<K, V>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                root_ = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                     std::addressof(rend_node_),
                                                     std::addressof(end_node_), 1);
                ConnectPrevNext(root_.get(), current_prev, current_next);
                return {Iterator(node), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                                 nullptr, nullptr, 1);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetLeft().get()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                                  nullptr, nullptr, 1);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetRight().get()), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key_value.first)) {
                return {Iterator(node), false};
            }
            if ((node != nullptr) && (key_value.first < node->GetKey())) {
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
    std::pair<Iterator, bool> InsertNode(ValueType&& key_value) {
        Node<K, V>* node = root_.get();
        Node<K, V>* parent = nullptr;
        bool left = false;
        BaseNode<K, V>* current_prev = std::addressof(rend_node_);
        BaseNode<K, V>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                root_ = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                     std::addressof(rend_node_),
                                                     std::addressof(end_node_), 1);
                ConnectPrevNext(root_.get(), current_prev, current_next);
                return {Iterator(node), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                                 nullptr, nullptr, 1);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetLeft().get()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                                  nullptr, nullptr, 1);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetRight().get()), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key_value.first)) {
                return {Iterator(node), false};
            }
            if ((node != nullptr) && (key_value.first < node->GetKey())) {
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
    std::pair<Iterator, bool> InsertNode(P&& key_value) {
        Node<K, V>* node = root_.get();
        Node<K, V>* parent = nullptr;
        bool left = false;
        BaseNode<K, V>* current_prev = std::addressof(rend_node_);
        BaseNode<K, V>* current_next = std::addressof(end_node_);

        while (true) {
            if ((node == nullptr) && (parent == nullptr)) {
                root_ = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                     std::addressof(rend_node_),
                                                     std::addressof(end_node_), 1);
                ConnectPrevNext(root_.get(), current_prev, current_next);
                return {Iterator(node), true};
            }
            if ((node == nullptr) && (parent != nullptr) && left) {
                parent->GetLeft() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                                 nullptr, nullptr, 1);
                parent->GetLeft()->GetParent() = parent;
                ConnectPrevNext(parent->GetLeft().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetLeft().get()), true};
            }
            if ((node == nullptr) && (parent != nullptr) && !left) {
                parent->GetRight() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                                  nullptr, nullptr, 1);
                parent->GetRight()->GetParent() = parent;
                ConnectPrevNext(parent->GetRight().get(), current_prev, current_next);
                IncreaseSizeInBranch(parent);
                return {Iterator(parent->GetRight().get()), true};
            }
            if ((node != nullptr) && Equivalent(node->GetKey(), key_value.first)) {
                return {Iterator(node), false};
            }
            if ((node != nullptr) && (key_value.first < node->GetKey())) {
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

    std::unique_ptr<Node<K, V>> root_;
    EndNode<K, V> rend_node_{nullptr, std::addressof(end_node_)};
    EndNode<K, V> end_node_{std::addressof(rend_node_), nullptr};
};

template <typename K, typename V>
bool operator==(const BST<K, V>& lhs, const BST<K, V>& rhs) {
    if (lhs.Size() != rhs.Size()) {
        return false;
    }

    for (auto it = lhs.Begin(), jt = rhs.Begin(); it != lhs.End(); ++it, ++jt) {
        if (!Equivalent(it->first, jt->first) || (it->second != jt->second)) {
            return false;
        }
    }
    return true;
}

template <typename K, typename V>
void Swap(const BST<K, V>& lhs, const BST<K, V>& rhs) {
    lhs.Swap(rhs);
}

template <typename K, typename V>
bool operator!=(const BST<K, V>& lhs, const BST<K, V>& rhs) {
    return (lhs != rhs);
}