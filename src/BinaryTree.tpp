#pragma once

#include "../include/BinaryTree.hpp"
#include <sstream>
#include <algorithm>
#include <type_traits>

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other)
    : root_(clone(other.root_)), size_(other.size_) {}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept
    : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree other) noexcept {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    destroy(root_);
}

template <typename T>
void BinaryTree<T>::insertNode(const T& value) {
    if (!root_) {
        root_ = new Node<T>(value);
        size_ = 1;
        return;
    }
    Node<T>* current = root_;
    while (true) {
       /*if (value == current->value) {
            return;      на случай важных переговоров о поисковом дереве без дубликатов
        }*/ 
        if (value < current->value) {
            if (!current->left) {
                current->left = new Node<T>(value);
                size_++;
                break;
            }
            current = current->left;
        } else {
            if (!current->right) {
                current->right = new Node<T>(value);
                size_++;
                break;
            }
            current = current->right;
        }
    }
}

template <typename T>
void BinaryTree<T>::destroy(Node<T>* n) noexcept {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

template <typename T>
Node<T>* BinaryTree<T>::clone(Node<T>* n) {
    if (!n) return nullptr;
    Node<T>* copy = new Node<T>(n->value);
    copy->left  = clone(n->left);
    copy->right = clone(n->right);
    return copy;
}

template <typename T>
void BinaryTree<T>::clear() noexcept {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
}

template <typename T>
bool BinaryTree<T>::empty() const noexcept { return root_ == nullptr; }

template <typename T>
std::size_t BinaryTree<T>::size() const noexcept { return size_; }

template <typename T>
void BinaryTree<T>::setRoot(const T& value) {
    if (root_) {
        root_->value = value;
    } else {
        root_ = new Node<T>(value);
        size_ = 1;
    }
}

template <typename T>
void BinaryTree<T>::setLeft(const T& value) {
    if (!root_) {
        throw EmptyTreeException("setLeft requires root");
    }
    if (root_->left) {
        destroy(root_->left);
        root_->left = nullptr;
    }
    root_->left = new Node<T>(value);
    size_ = 0;
    std::vector<Node<T>*> tmp;
    collectPtrs(root_, Traversal::KLP, tmp);
    size_ = tmp.size();
}

template <typename T>
void BinaryTree<T>::setRight(const T& value) {
    if (!root_) {
        throw EmptyTreeException("setRight requires root");
    }
    if (root_->right) {
        destroy(root_->right);
        root_->right = nullptr;
    }
    root_->right = new Node<T>(value);
    size_ = 0;
    std::vector<Node<T>*> tmp;
    collectPtrs(root_, Traversal::KLP, tmp);
    size_ = tmp.size();
}
template <typename T>
void BinaryTree<T>::collect(Node<T>* n, Traversal order, std::vector<T>& out) {
    if (!n) return;
    const auto ord = traversalOrder(order);
    switch (order) {
        case Traversal::KLP: out.push_back(n->value); collect(n->left, order, out); collect(n->right, order, out); break;
        case Traversal::KPL: out.push_back(n->value); collect(n->right, order, out); collect(n->left, order, out); break;
        case Traversal::LPK: collect(n->left, order, out); collect(n->right, order, out); out.push_back(n->value); break;
        case Traversal::LKP: collect(n->left, order, out); out.push_back(n->value); collect(n->right, order, out); break;
        case Traversal::PLK: collect(n->right, order, out); collect(n->left, order, out); out.push_back(n->value); break;
        case Traversal::PKL: collect(n->right, order, out); out.push_back(n->value); collect(n->left, order, out); break;
    }
    (void)ord;
}

template <typename T>
std::vector<T> BinaryTree<T>::traverse(Traversal order) const {
    std::vector<T> out;
    out.reserve(size_);
    collect(root_, order, out);
    return out;
}

template <typename T>
void BinaryTree<T>::collectPtrs(Node<T>* n, Traversal order, std::vector<Node<T>*>& out) {
    if (!n) return;
    switch (order) {
        case Traversal::KLP: out.push_back(n); collectPtrs(n->left, order, out); collectPtrs(n->right, order, out); break;
        case Traversal::KPL: out.push_back(n); collectPtrs(n->right, order, out); collectPtrs(n->left, order, out); break;
        case Traversal::LPK: collectPtrs(n->left, order, out); collectPtrs(n->right, order, out); out.push_back(n); break;
        case Traversal::LKP: collectPtrs(n->left, order, out); out.push_back(n); collectPtrs(n->right, order, out); break;
        case Traversal::PLK: collectPtrs(n->right, order, out); collectPtrs(n->left, order, out); out.push_back(n); break;
        case Traversal::PKL: collectPtrs(n->right, order, out); out.push_back(n); collectPtrs(n->left, order, out); break;
    }
}

template <typename T>
template <typename F>
void BinaryTree<T>::visitImpl(Node<T>* n, Traversal order, F& f) {
    if (!n) return;
    switch (order) {
        case Traversal::KLP: f(n->value); visitImpl(n->left, order, f); visitImpl(n->right, order, f); break;
        case Traversal::KPL: f(n->value); visitImpl(n->right, order, f); visitImpl(n->left, order, f); break;
        case Traversal::LPK: visitImpl(n->left, order, f); visitImpl(n->right, order, f); f(n->value); break;
        case Traversal::LKP: visitImpl(n->left, order, f); f(n->value); visitImpl(n->right, order, f); break;
        case Traversal::PLK: visitImpl(n->right, order, f); visitImpl(n->left, order, f); f(n->value); break;
        case Traversal::PKL: visitImpl(n->right, order, f); f(n->value); visitImpl(n->left, order, f); break;
    }
}

template <typename T>
template <typename F>
void BinaryTree<T>::visit(Traversal order, F&& f) const {
    F fn = std::forward<F>(f);
    visitImpl(root_, order, fn);
}

template <typename T>
template <typename U, typename F>
Node<U>* BinaryTree<T>::mapImpl(Node<T>* n, F& f) {
    if (!n) return nullptr;
    Node<U>* res = new Node<U>(f(n->value));
    res->left  = mapImpl<U, F>(n->left, f);
    res->right = mapImpl<U, F>(n->right, f);
    return res;
}

template <typename T>
template <typename U, typename F>
BinaryTree<U> BinaryTree<T>::map(F&& f) const {
    BinaryTree<U> result;
    F fn = std::forward<F>(f);
    result.root_ = mapImpl<U>(root_, fn);
    result.size_ = size_;
    return result;
}

template <typename T>
Node<T>* BinaryTree<T>::whereImpl(Node<T>* n,
                                  const std::function<bool(const T&)>& pred) {
    if (!n) return nullptr;

    Node<T>* newLeft  = whereImpl(n->left, pred);
    Node<T>* newRight = whereImpl(n->right, pred);

    if (pred(n->value)) {
        Node<T>* res = new Node<T>(n->value);
        res->left  = newLeft;
        res->right = newRight;
        return res;
    }

    if (!newLeft)  return newRight;
    if (!newRight) return newLeft;

    Node<T>* tail = newLeft;
    while (tail->right) tail = tail->right;
    tail->right = newRight;
    return newLeft;
}

template <typename T>
BinaryTree<T> BinaryTree<T>::where(std::function<bool(const T&)> pred) const {
    BinaryTree<T> result;
    result.root_ = whereImpl(root_, pred);
    std::vector<Node<T>*> tmp;
    collectPtrs(result.root_, Traversal::KLP, tmp);
    result.size_ = tmp.size();
    return result;
}

template <typename T>
template <typename Acc, typename F>
Acc BinaryTree<T>::reduceImpl(Node<T>* n, Traversal order, Acc acc, F& f) {
    if (!n) return acc;
    switch (order) {
        case Traversal::KLP: acc = f(acc, n->value); acc = reduceImpl(n->left, order, acc, f); acc = reduceImpl(n->right, order, acc, f); break;
        case Traversal::KPL: acc = f(acc, n->value); acc = reduceImpl(n->right, order, acc, f); acc = reduceImpl(n->left, order, acc, f); break;
        case Traversal::LPK: acc = reduceImpl(n->left, order, acc, f); acc = reduceImpl(n->right, order, acc, f); acc = f(acc, n->value); break;
        case Traversal::LKP: acc = reduceImpl(n->left, order, acc, f); acc = f(acc, n->value); acc = reduceImpl(n->right, order, acc, f); break;
        case Traversal::PLK: acc = reduceImpl(n->right, order, acc, f); acc = reduceImpl(n->left, order, acc, f); acc = f(acc, n->value); break;
        case Traversal::PKL: acc = reduceImpl(n->right, order, acc, f); acc = f(acc, n->value); acc = reduceImpl(n->left, order, acc, f); break;
    }
    return acc;
}

template <typename T>
template <typename Acc, typename F>
Acc BinaryTree<T>::reduce(Traversal order, Acc init, F&& f) const {
    F fn = std::forward<F>(f);
    return reduceImpl(root_, order, init, fn);
}

template <typename T>
template <typename Combiner>
BinaryTree<T> BinaryTree<T>::merge(const BinaryTree<T>& other,
                                   Combiner combiner,
                                   Traversal order) const {
    auto a = traverse(order);
    auto b = other.traverse(order);
    std::size_t n = std::min(a.size(), b.size());
    std::vector<T> merged;
    merged.reserve(std::max(a.size(), b.size()));
    for (std::size_t i = 0; i < n; ++i) merged.push_back(combiner(a[i], b[i]));
    for (std::size_t i = n; i < a.size(); ++i) merged.push_back(a[i]);
    for (std::size_t i = n; i < b.size(); ++i) merged.push_back(b[i]);
    return deserialize(vectorToString(merged), order);
}

namespace detail {
template <typename T>
std::string vectorToString(const std::vector<T>& v) {
    std::ostringstream os;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) os << ' ';
        os << v[i];
    }
    return os.str();
}
}

template <typename T>
BinaryTree<T> BinaryTree<T>::extractSubtree(const T& target) const {
    std::vector<Node<T>*> nodes;
    collectPtrs(root_, Traversal::KLP, nodes);
    for (Node<T>* n : nodes) {
        if (n->value == target) {
            BinaryTree<T> res;
            res.root_ = clone(n);
            std::vector<Node<T>*> tmp;
            collectPtrs(res.root_, Traversal::KLP, tmp);
            res.size_ = tmp.size();
            return res;
        }
    }
    throw ElementNotFoundException("extractSubtree: no such element");
}

template <typename T>
bool BinaryTree<T>::subtreeEquals(Node<T>* a, Node<T>* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    if (!(a->value == b->value)) return false;
    return subtreeEquals(a->left, b->left) &&
           subtreeEquals(a->right, b->right);
}

template <typename T>
bool BinaryTree<T>::containsSubtree(const BinaryTree<T>& other) const {
    if (other.empty()) return true; 
    std::vector<Node<T>*> nodes;
    collectPtrs(root_, Traversal::KLP, nodes);
    for (Node<T>* n : nodes) {
        if (subtreeEquals(n, other.root_)) return true;
    }
    return false;
}

template <typename T>
bool BinaryTree<T>::contains(const T& value) const {
    std::vector<Node<T>*> nodes;
    collectPtrs(root_, Traversal::KLP, nodes);
    for (Node<T>* n : nodes) if (n->value == value) return true;
    return false;
}

template <typename T>
const T& BinaryTree<T>::findByPath(const std::string& path) const {
    if (!root_) throw EmptyTreeException("findByPath");
    Node<T>* cur = root_;
    for (char c : path) {
        if (c == 'L' || c == 'l') {
            if (!cur->left) throw InvalidPathException("no left child");
            cur = cur->left;
        } else if (c == 'R' || c == 'r') {
            if (!cur->right) throw InvalidPathException("no right child");
            cur = cur->right;
        } else {
            throw InvalidPathException(std::string("bad char '") + c + "'");
        }
    }
    return cur->value;
}

template <typename T>
std::string BinaryTree<T>::serialize(Traversal order) const {
    std::ostringstream os;
    std::vector<T> vals = traverse(order);
    for (std::size_t i = 0; i < vals.size(); ++i) {
        if (i) os << ' ';
        os << vals[i];
    }
    return os.str();
}

namespace detail {
template <typename T>
Node<T>* buildBalanced(const std::vector<T>& v, std::size_t l, std::size_t r) {
    if (l >= r) return nullptr;
    std::size_t m = l + (r - l) / 2;
    Node<T>* n = new Node<T>(v[m]);
    n->left  = buildBalanced(v, l, m);
    n->right = buildBalanced(v, m + 1, r);
    return n;
}
}

template <typename T>
BinaryTree<T> BinaryTree<T>::deserialize(const std::string& s, Traversal order) {
    std::istringstream is(s);
    std::vector<T> vals;
    T x;
    while (is >> x) vals.push_back(x);
    if (is.fail() && !is.eof()) {
        throw InvalidFormatException("cannot parse element");
    }

    BinaryTree<T> res;
    if (vals.empty()) return res;

    switch (order) {
        case Traversal::LKP: {
            res.root_ = detail::buildBalanced(vals, 0, vals.size());
            break;
        }
        default: {
            res.root_ = detail::buildBalanced(vals, 0, vals.size());
            break;
        }
    }
    std::vector<Node<T>*> tmp;
    collectPtrs(res.root_, Traversal::KLP, tmp);
    res.size_ = tmp.size();
    return res;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const BinaryTree<U>& tree) {
    os << "BinaryTree(size=" << tree.size() << ") [";
    std::vector<U> v = tree.traverse(Traversal::KLP);
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) os << ", ";
        os << v[i];
    }
    os << "]";
    return os;
}


