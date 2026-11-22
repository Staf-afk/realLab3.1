#pragma once
#include <functional>
#include <vector>
#include "Exceptions.hpp"
#include "Traversal.hpp"

template <typename T>
struct Node {
    T value;
    Node* left  = nullptr;
    Node* right = nullptr;

    Node(T value) : value(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
private:
    Node<T>* root_ = nullptr;
    std::size_t size_ = 0;

    static void destroy(Node<T>* n) noexcept;

    template <typename F>
    static void visitImpl(Node<T>* n, Traversal order, F& f);

    template <typename U, typename F>
    static Node<U>* mapImpl(Node<T>* n, F& f);

    static Node<T>* whereImpl(Node<T>* n, const std::function<bool(const T&)>& pred);

    template <typename Acc, typename F>
    static Acc reduceImpl(Node<T>* n, Traversal order, Acc acc, F& f);

    static void collect(Node<T>* n, Traversal order, std::vector<T>& out);
    static void collectPtrs(Node<T>* n, Traversal order, std::vector<Node<T>*>& out);

    static Node<T>* clone(Node<T>* n);
    static bool subtreeEquals(Node<T>* a, Node<T>* b);


public:
    BinaryTree() = default;
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    BinaryTree& operator=(BinaryTree other) noexcept;
    ~BinaryTree();
    void clear() noexcept;
    bool empty() const noexcept;
    std::size_t size() const noexcept;
    void setLeft (const T& value);  
    void setRight(const T& value);
    void setRoot (const T& value);   
    std::vector<T> traverse(Traversal order) const;

    template <typename F>
    void visit(Traversal order, F&& f) const;
    template <typename U, typename F>
    BinaryTree<U> map(F&& f) const;
    BinaryTree<T> where(std::function<bool(const T&)> pred) const;
    void insertNode(const T& value);

    template <typename Acc, typename F>
    Acc reduce(Traversal order, Acc init, F&& f) const;

    template <typename Combiner>
    BinaryTree<T> merge(const BinaryTree<T>& other,
                        Combiner combiner,
                        Traversal order = Traversal::KLP) const;
    BinaryTree<T> extractSubtree(const T& target) const;
    bool containsSubtree(const BinaryTree<T>& other) const;

    bool contains(const T& value) const;
    const T& findByPath(const std::string& path) const;

    std::string serialize(Traversal order) const;

    static BinaryTree<T> deserialize(const std::string& s, Traversal order);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const BinaryTree<U>& tree);

    const Node<T>* root() const noexcept { return root_; }
    Node<T>* root() noexcept { return root_; }

};



