#pragma once

#include <memory>
#include <utility>

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

    BST() = default;
    BST(const BST& other) {
    }
    BST& operator=(const BST& other) {
    }
    BST(BST&& other) = default;
    BST& operator=(BST&& other) = default;
    ~BST() = default;

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
        }
        void Dec() {
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
        }
        void Dec() {
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
        }
        void Dec() {
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
        }
        void Dec() {
        }
        const BaseNode<K, V>* node_ = nullptr;
    };

private:
    std::unique_ptr<Node<K, V>> root_;
    EndNode<K, V> rend_node_{nullptr, std::addressof(end_node_)};
    EndNode<K, V> end_node_{std::addressof(rend_node_), nullptr};
};