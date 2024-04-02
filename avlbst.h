#ifndef AVLBST_H
#define AVLBST_H

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include "bst.h"

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the balance as a data
 * member, plus other additional helper functions. You do NOT need to implement
 * any functionality or add additional data members or helper functions.
 */
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
   public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since
    // they return pointers to AVLNodes - not plain Nodes. See the Node class in
    // bst.h for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

   protected:
    int8_t balance_;  // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class
 * constructor
 */
template <class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key,
                             const Value& value,
                             AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent), balance_(0) {}

/**
 * A destructor which does nothing.
 */
template <class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the balance of a AVLNode.
 */
template <class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const {
    return balance_;
}

/**
 * A setter for the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance) {
    balance_ = balance;
}

/**
 * Adds diff to the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff) {
    balance_ += diff;
}

/**
 * An overridden function for getting the parent since a static_cast is
 * necessary to make sure that our node is a AVLNode.
 */
template <class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
   public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);                               // TODO
   protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);
    void rotateLeft(AVLNode<Key, Value>* n);
    void rotateRight(AVLNode<Key, Value>* n);
    void insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
    void removeFix(AVLNode<Key, Value>* n, int diff);
    bool isZigZig(AVLNode<Key, Value>* n,
                  AVLNode<Key, Value>* p,
                  AVLNode<Key, Value>* g);
    // Add helper functions here
};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */

template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n) {
    AVLNode<Key, Value>* parent = n->getParent();
    AVLNode<Key, Value>* child = n->getRight();
    n->setRight(child->getLeft());
    if (child->getLeft() != nullptr) {
        child->getLeft()->setParent(n);
    }
    child->setLeft(n);
    n->setParent(child);
    child->setParent(parent);
    if (parent == nullptr) {
        this->root_ = child;
    } else {
        if (parent->getLeft() == n) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    }
    n->updateBalance(-1);
    child->updateBalance(-1);
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n) {
    AVLNode<Key, Value>* parent = n->getParent();
    AVLNode<Key, Value>* child = n->getLeft();
    n->setLeft(child->getRight());
    if (child->getRight() != nullptr) {
        child->getRight()->setParent(n);
    }
    child->setRight(n);
    n->setParent(child);
    child->setParent(parent);
    if (parent == nullptr) {
        this->root_ = child;
    } else {
        if (parent->getLeft() == n) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    }
    n->updateBalance(1);
    child->updateBalance(1);
}

template <class Key, class Value>
bool AVLTree<Key, Value>::isZigZig(AVLNode<Key, Value>* n,
                                   AVLNode<Key, Value>* p,
                                   AVLNode<Key, Value>* g) {
    if (n == nullptr || g == nullptr || p == nullptr) {
        exit(-1);
    }
    if (g->getLeft() == p && p->getLeft() == n) {
        return true;
    }
    if (g->getRight() == p && p->getRight() == n) {
        return true;
    }
    return false;
}

template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* n,
                                    AVLNode<Key, Value>* p) {
    if (p == nullptr) {
        return;
    }
    AVLNode<Key, Value>* g = p->getParent();
    if (g == nullptr) {
        return;
    }
    if (g->getLeft() == p) {
        g->updateBalance(-1);
        if (g->getBalance() == 0) {
            return;
        }
        if (g->getBalance() == -1) {
            insertFix(p, g);
            return;
        }
        if (g->getBalance() == -2) {
            if (isZigZig(n, p, g)) {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            } else {
                rotateLeft(p);
                rotateRight(g);
                if (n->getBalance() == -1) {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                } else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                } else {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    } else {
        g->updateBalance(1);
        if (g->getBalance() == 0) {
            return;
        }
        if (g->getBalance() == 1) {
            insertFix(p, g);
            return;
        }
        if (g->getBalance() == 2) {
            if (isZigZig(n, p, g)) {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            } else {
                rotateRight(p);
                rotateLeft(g);
                if (n->getBalance() == 1) {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                } else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                } else {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    // TODO
    Key key = new_item.first;
    Value value = new_item.second;
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(key, value, NULL);
        return;
    }
    Node<Key, Value>* curr = BinarySearchTree<Key, Value>::root_;
    while (curr != nullptr) {
        if (curr->getKey() == key) {
            curr->setValue(value);
            return;
        }
        if (curr->getKey() < key) {
            if (curr->getRight() == nullptr)
                break;
            curr = curr->getRight();
        } else {
            if (curr->getLeft() == nullptr)
                break;
            curr = curr->getLeft();
        }
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(
        key, value, static_cast<AVLNode<Key, Value>*>(curr));
    AVLNode<Key, Value>* parent = newNode->getParent();

    if (parent->getKey() < key) {
        parent->setRight(newNode);
        parent->updateBalance(1);
    } else {
        parent->setLeft(newNode);
        parent->updateBalance(-1);
    }
    if (parent->getBalance() != 0) {
        insertFix(newNode, parent);
    }
    return;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff) {
    if (n == nullptr) {
        return;
    }
    AVLNode<Key, Value>* p = n->getParent();
    int nextDiff = 0;
    if (p != nullptr) {
        if (n == p->getLeft()) {
            nextDiff = 1;
        } else {
            nextDiff = -1;
        }
    }

    if (diff == -1) {
        if (n->getBalance() + diff == -2) {
            AVLNode<Key, Value>* c = n->getLeft();
            if (c->getBalance() == -1) {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, nextDiff);
            } else if (c->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            } else if (c->getBalance() == 1) {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if (g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, nextDiff);
            }
        } else if (n->getBalance() + diff == -1) {
            n->setBalance(-1);
        } else {
            n->setBalance(0);
            removeFix(p, nextDiff);
        }
    } else {
        if (n->getBalance() + diff == 2) {
            AVLNode<Key, Value>* c = n->getRight();
            if (c->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, nextDiff);
            } else if (c->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            } else if (c->getBalance() == -1) {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if (g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, nextDiff);
            }
        } else if (n->getBalance() + diff == 1) {
            n->setBalance(1);
        } else {
            n->setBalance(0);
            removeFix(p, nextDiff);
        }
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // TODO
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(
        BinarySearchTree<Key, Value>::internalFind(key));
    if (n == nullptr) {
        return;
    }
    if (n->getLeft() != nullptr && n->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(
            BinarySearchTree<Key, Value>::predecessor(n));
        nodeSwap(n, pred);
    }

    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* child =
        (n->getLeft() != nullptr) ? n->getLeft() : n->getRight();

    int diff = 0;
    if (p != nullptr) {
        if (p->getLeft() == n) {
            diff = 1;
            p->setLeft(child);
        } else {
            diff = -1;
            p->setRight(child);
        }
    } else {
        this->root_ = child;
    }
    if (child != nullptr) {
        child->setParent(p);
    }
    delete n;
    removeFix(p, diff);
}

template <class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1,
                                   AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
