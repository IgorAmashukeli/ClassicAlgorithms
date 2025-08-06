#pragma once

#include <memory>
#include <utility>

template <typename K>
bool Equivalent(const K& key_1, const K& key_2) {
    return !(key_1 < key_2) && (!(key_2 < key_1));
}

template <typename K, typename V>
struct Node;

template <typename K, typename V>
struct BaseNode {
    BaseNode() = default;
    BaseNode(const BaseNode& other) = delete;
    BaseNode& operator=(const BaseNode& other) = delete;
    BaseNode(BaseNode&& other) = delete;
    BaseNode& operator=(BaseNode&& other) = delete;
    ~BaseNode() = default;

    virtual const K& GetKey() const = 0;
    virtual const V& GetValue() const = 0;
    virtual V& GetValue() = 0;
    virtual const std::unique_ptr<Node<K, V>>& GetLeft() const = 0;
    virtual std::unique_ptr<Node<K, V>>& GetLeft() = 0;
    virtual const std::unique_ptr<Node<K, V>>& GetRight() const = 0;
    virtual std::unique_ptr<Node<K, V>>& GetRight() = 0;
    virtual Node<K, V>* GetParent() const = 0;
    virtual Node<K, V>*& GetParent() = 0;
    virtual BaseNode<K, V>* GetPrev() const = 0;
    virtual BaseNode<K, V>*& GetPrev() = 0;
    virtual BaseNode<K, V>* GetNext() const = 0;
    virtual BaseNode<K, V>*& GetNext() = 0;
    virtual size_t GetSize() const = 0;
    virtual size_t& GetSize() = 0;
    virtual const std::pair<const K, V>& GetKeyValue() const = 0;
    virtual std::pair<const K, V>& GetKeyValue() = 0;
    virtual bool IsEndNode() const = 0;
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
    const K& GetKey() const {
        return key_value_.first;
    }
    const V& GetValue() const {
        return key_value_.second;
    }
    V& GetValue() {
        return key_value_.second;
    }
    const std::unique_ptr<Node<K, V>>& GetLeft() const {
        return left_;
    }
    std::unique_ptr<Node<K, V>>& GetLeft() {
        return left_;
    }
    const std::unique_ptr<Node<K, V>>& GetRight() const {
        return right_;
    }
    std::unique_ptr<Node<K, V>>& GetRight() {
        return right_;
    }
    Node<K, V>* GetParent() const {
        return parent_;
    }
    Node<K, V>*& GetParent() {
        return parent_;
    }
    BaseNode<K, V>* GetPrev() const {
        return prev_;
    }
    BaseNode<K, V>*& GetPrev() {
        return prev_;
    }
    BaseNode<K, V>* GetNext() const {
        return next_;
    }
    BaseNode<K, V>*& GetNext() {
        return next_;
    }
    size_t GetSize() const {
        return size_;
    }
    size_t& GetSize() {
        return size_;
    }
    const std::pair<const K, V>& GetKeyValue() const {
        return key_value_;
    }
    std::pair<const K, V>& GetKeyValue() {
        return key_value_;
    }
    bool IsEndNode() const {
        return false;
    }
};

template <typename K, typename V>
struct EndNode : public BaseNode<K, V> {
    BaseNode<K, V>* prev_ = nullptr;
    BaseNode<K, V>* next_ = nullptr;

    EndNode() = default;
    EndNode(const EndNode& other) = delete;
    EndNode& operator=(const EndNode& other) = delete;
    EndNode(EndNode&& other) = default;
    EndNode& operator=(EndNode&& other) = default;
    ~EndNode() = default;

    EndNode(BaseNode<K, V>* prev, BaseNode<K, V>* next) : prev_(prev), next_(next) {
    }
    BaseNode<K, V>* GetPrev() const {
        return prev_;
    }
    BaseNode<K, V>*& GetPrev() {
        return prev_;
    }
    BaseNode<K, V>* GetNext() const {
        return next_;
    }
    BaseNode<K, V>*& GetNext() {
        return next_;
    }
    bool IsEndNode() const {
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
        Iterator() = default;
        Iterator(const Iterator& other) = default;
        Iterator& operator=(const Iterator& other) = default;
        Iterator(Iterator&& other) = default;
        Iterator& operator=(Iterator&& other) = default;
        ~Iterator() = default;

        explicit Iterator(BaseNode<K, V>* node) : node_(node) {
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
        bool operator==(const Iterator& other) const {
            return node_ == other.node_;
        }
        bool operator!=(const Iterator& other) const {
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
        ConstIterator() = default;
        ConstIterator(const ConstIterator& other) = default;
        ConstIterator& operator=(const ConstIterator& other) = default;
        ConstIterator(ConstIterator&& other) = default;
        ConstIterator& operator=(ConstIterator&& other) = default;
        ~ConstIterator() = default;

        explicit ConstIterator(const BaseNode<K, V>* node) : node_(node) {
        }
        ConstIterator(Iterator it) : node_(it.node_) {
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
        bool operator==(const ConstIterator& other) const {
            return node_ == other.node_;
        }
        bool operator!=(const ConstIterator& other) const {
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
        ReverseIterator() = default;
        ReverseIterator(const ReverseIterator& other) = default;
        ReverseIterator& operator=(const ReverseIterator& other) = default;
        ReverseIterator(ReverseIterator&& other) = default;
        ReverseIterator& operator=(ReverseIterator&& other) = default;
        ~ReverseIterator() = default;

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
        bool operator==(const ReverseIterator& other) const {
            return node_ == other.node_;
        }
        bool operator!=(const ReverseIterator& other) const {
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
        ConstReverseIterator() = default;
        ConstReverseIterator(const ConstReverseIterator& other) = default;
        ConstReverseIterator& operator=(const ConstReverseIterator& other) = default;
        ConstReverseIterator(ConstReverseIterator&& other) = default;
        ConstReverseIterator& operator=(ConstReverseIterator&& other) = default;
        ~ConstReverseIterator() = default;

        explicit ConstReverseIterator(BaseNode<K, V>* node) : node_(node) {
        }
        ConstReverseIterator(ReverseIterator it) : node_(it.node_) {
        }
        ConstReference operator*() const {
            return node_->GetKeyValue();
        }
        ConstPointer operator->() const {
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
        bool operator==(const ConstReverseIterator& other) const {
            return node_ == other.node_;
        }
        bool operator!=(const ConstReverseIterator& other) const {
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
        auto node = std::addressof(rend_node_);
        root_ = Copy(other, node);
        ConnectEndNodesAfterCopy(node);
    }
    BST& operator=(const BST& other) {
        if (this != std::addressof(other)) {
            Clear();
            auto node = std::addressof(rend_node_);
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

    void Clear() {
        root_ = nullptr;
        rend_node_.GetPrev() = nullptr;
        rend_node_.GetNext() = std::addressof(end_node_);
        end_node_.GetNext() = nullptr;
        end_node_.GetPrev() = std::addressof(rend_node_);
    }
    void Swap(const BST<K, V>& other) {
        std::swap(root_, other.root_);
        ConnectEndNodesAfterSwap(other);
    }
    std::pair<Iterator, bool> Insert(const ValueType& key_value) {

        return InsertNode(root_, key_value);
    }

    Iterator Find(const K& key) {
        auto node = FindNode(key, root_);
        if (node == nullptr) {
            return End();
        }
        return Iterator(node);
    }
    ConstIterator Find(const K& key) const {
        auto node = FindNode(key, root_);
        if (node == nullptr) {
            return End();
        }
        return ConstIterator(node);
    }
    Iterator LowerBound(const K& key) {
        auto node = FindLowerBound(key, root_, nullptr);
        if (node == nullptr) {
            return End();
        }
        return Iterator(node);
    }
    ConstIterator LowerBound(const K& key) const {
        auto node = FindLowerBound(key, root_, nullptr);
        if (node == nullptr) {
            return End();
        }
        return ConstIterator(node);
    }
    Iterator UpperBound(const K& key) {
        auto node = FindLowerBound(key, root_, nullptr);
        if (node == nullptr) {
            return End();
        }
        if (Equivalent(node->GetKey(), key)) {
            return Iterator(node->GetNext());
        }
        return Iterator(node);
    }
    ConstIterator UpperBound(const K& key) const {
        auto node = FindLowerBound(key, root_, nullptr);
        if (node == nullptr) {
            return End();
        }
        if (Equivalent(node->GetKey(), key)) {
            return ConstIterator(node->GetNext());
        }
        return ConstIterator(node);
    }
    std::pair<Iterator, Iterator> EqualRange(const K& key) {
        auto node = FindLowerBound(key, root_, nullptr);
        if (node == nullptr) {
            return {End(), End()};
        }
        if (Equivalent(node->GetKey(), key)) {
            return {Iterator(node), Iterator(node->GetNext())};
        }
        return {Iterator(node), Iterator(node)};
    }
    std::pair<ConstIterator, ConstIterator> EqualRange(const K& key) const {
        auto node = FindLowerBound(key, root_, nullptr);
        if (node == nullptr) {
            return {End(), End()};
        }
        if (Equivalent(node->GetKey(), key)) {
            return {ConstIterator(node), ConstIterator(node->GetNext())};
        }
        return {ConstIterator(node), ConstIterator(node)};
    }
    bool Contains(const K& key) const {
        return FindNode(key) != nullptr;
    }
    size_t Count(const K& key) const {
        return static_cast<size_t>(Contains(key));
    }
    Iterator Begin() noexcept {
        return Iterator(rend_node_.GetNext());
    }
    ConstIterator Begin() const noexcept {
        return ConstIterator(rend_node_.GetNext());
    }
    Iterator End() noexcept {
        return Iterator(std::addressof(end_node_));
    }
    ConstIterator End() const noexcept {
        return ConstIterator(std::addressof(end_node_));
    }
    ConstIterator CBegin() const noexcept {
        return ConstIterator(rend_node_.GetNext());
    }
    ConstIterator CEnd() const noexcept {
        return ConstIterator(std::addressof(end_node_));
    }
    ReverseIterator RBegin() noexcept {
        return ReverseIterator(end_node_.GetPrev());
    }
    ConstReverseIterator RBegin() const noexcept {
        return ConstReverseIterator(end_node_.GetPrev());
    }
    ReverseIterator REnd() noexcept {
        return ReverseIterator(std::addressof(rend_node_));
    }
    ConstReverseIterator REnd() const noexcept {
        return ConstReverseIterator(std::addressof(rend_node_));
    }
    ConstReverseIterator CRBegin() const noexcept {
        return ConstReverseIterator(end_node_.GetPrev());
    }
    ConstReverseIterator CREnd() const noexcept {
        return ConstReverseIterator(std::addressof(rend_node_));
    }

    size_t Size() const {
        if (root_ == nullptr) {
            return 0;
        }
        return root_->GetSize();
    }
    bool Empty() const {
        return (root_ == nullptr);
    }

private:
    Node<K, V>* FindNode(const K& key, const std::unique_ptr<Node<K, V>>& node) const {
        if (node == nullptr) {
            return nullptr;
        }
        if (Equivalent(key, node->GetKey())) {
            return node;
        }
        if (key < node->GetKey()) {
            return FindNode(key, node->GetLeft());
        }
        return FindNode(key, node->GetRight());
    }

    Node<K, V>* FindLowerBound(const K& key, const std::unique_ptr<Node<K, V>>& node,
                               Node<K, V>* best_bound) const {
        if (node == nullptr) {
            return best_bound;
        }
        if (Equivalent(key, node->GetKey())) {
            return node;
        }
        if (key < node->GetKey()) {
            return FindLowerBound(key, node->GetLeft(), node.get());
        }
        return FindLowerBound(key, node->GetRight(), best_bound);
    }

    void ConnectEndNodesAfterSwap(const BST<K, V>& other) {
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
            end_node_->GetPrev() = std::addressof(max_node);
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

    std::pair<Iterator, bool> InsertNode(const std::unique_ptr<Node<K, V>>& node,
                                         Node<K, V>* parent, bool left,
                                         const ValueType& key_value) {
        if ((node == nullptr) && (parent == nullptr)) {
            root_ = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                 std::addressof(rend_node_),
                                                 std::addressof(end_node_), 1);
            return {Iterator(root_.get()), true};
        }

        if ((node == nullptr) && (parent != nullptr) && left) {
            parent->GetLeft() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                             nullptr, nullptr, 1);
            parent->GetLeft()->GetParent() = parent;

            return {Iterator(parent->GetLeft().get(), true)};
        }

        if ((node == nullptr) && (parent != nullptr) && !left) {
            parent->GetRight() = std::make_unique<Node<K, V>>(key_value.first, key_value.second,
                                                              nullptr, nullptr, 1);
            parent->GetRight()->GetParent() = parent;

            return {Iterator(parent->GetRight().get(), true)};
        }

        if ((node != nullptr) && Equivalent(node.get(), key_value.first)) {
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
bool operator!=(const BST<K, V>& lhs, const BST<K, V>& rhs) {
    return (lhs != rhs);
}