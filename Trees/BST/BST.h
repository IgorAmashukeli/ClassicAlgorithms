#pragma once

#include <memory>
#include <utility>

template <typename K, typename V>
struct BaseNode {
    BaseNode() = default;
    BaseNode(const BaseNode& other) = delete;
    BaseNode& operator=(const BaseNode& other) = delete;
    BaseNode(BaseNode&& other) = delete;
    BaseNode& operator=(BaseNode&& other) = delete;
    ~BaseNode() = default;
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
    Node(const K& key, const V& value, BaseNode<K, V>* prev, BaseNode<K, V>* next, size_t size)
        : key_value_(key, value), prev_(prev), next_(next), size_(size) {
    }
    Node(const Node& other) = delete;
    Node& operator=(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node& operator=(Node&& other) = delete;
    ~Node() = default;

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
    std::unique_ptr<Node<K, V>> GetRight() {
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
};

template <typename K, typename V>
struct EndNode : public BaseNode<K, V> {
    BaseNode<K, V>* prev_ = nullptr;
    BaseNode<K, V>* next_ = nullptr;

    EndNode() = default;
    EndNode(BaseNode<K, V>* prev, BaseNode<K, V>* next) : prev_(prev), next_(next) {
    }
    EndNode(const EndNode& other) = delete;
    EndNode& operator=(const EndNode& other) = delete;
    EndNode(EndNode&& other) = default;
    EndNode& operator=(EndNode&& other) = default;
    ~EndNode() = default;
};

template <typename K, typename V>
class BST {
public:
    BST() = default;
    BST(const BST& other) {
    }
    BST& operator=(const BST& other) {
    }

    BST(BST&& other) = default;
    BST& operator=(BST&& other) = default;

    ~BST() = default;

private:
    std::unique_ptr<Node<K, V>> root_;
    EndNode<K, V> rend_node_{nullptr, std::addressof(end_node_)};
    EndNode<K, V> end_node_{std::addressof(rend_node_), nullptr};
};