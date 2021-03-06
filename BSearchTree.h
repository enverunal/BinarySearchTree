#pragma once
#include<string>
#include<initializer_list>
#include <stack>
#include <iostream>
#include <functional>


template <typename t>
class bstNode
{
private:
    t m_value{};
    bstNode<t>* right{};
	bstNode<t>* left{};
    template <typename u> friend class BSearchTree;
public:
    t value() {
        return m_value;
    }
};

template <typename t>
class BSearchTree {
private:
	bstNode<t>* start{};
	size_t length{ 0 };

    //traverse function does allow elements to change
    //could be made public if required or wrapped with a safer function
    //preorder traverse
    void traverse(std::function<void(bstNode<t>* element)> process);
public:
    /*
        *noexcept
    */
    BSearchTree() noexcept{};

    /*
        Imports each element in iterable sturcture to this tree
        *noexcept
    */
    BSearchTree(std::initializer_list<t> initList) noexcept;

    /*
        Imports each element in iterable sturcture to this tree
        *noexcept
    */
    BSearchTree(const BSearchTree<t>& bst) noexcept;

    /*
        Imports each element in iterable sturcture to this tree
        *noexcept
    */
    template<typename d>
    BSearchTree(const d& otherIterable) noexcept;
    
    /*
        Merges given structure with this tree
        Does not clears the merged structure
        *noexcept
    */
    void merge(BSearchTree<t>& bst) noexcept;

    /*
    Merges given structure with this tree
    Does not clears the merged structure
    *noexcept
    */
    template<typename d>
    void merge(const d& otherIterable) noexcept;
    
    /*
        Visits all variables in order and executes the process function
        Does not allow variables to change
        *noexcept
    */
    void forEach(std::function<void(const t& element)> process)noexcept;

    /*
        Adds given value to tree
        length value might overflow if there are more than 2^64 elements
        *noexcept
    */
    void add(const t& value) noexcept;

     /*
        Adds given value to tree if value does not exists
        length value might overflow if there are more than 2^64 elements
        return true if succeed
        *noexcept
     */
    bool addIfNotExists(const t& value) noexcept;

    /*
        Removes all elements
        *noexcept
    */
    void clear() noexcept;  

    /*
        Removes the first node equal to value given
        returns true if removing succeed
        returns false if value is not found and removing failed
        *noexcept
    */
    bool remove(const t& value) noexcept;

    /*
        Returns true if tree does not have any nodes
        *noexcept *const
    */
    bool isEmpty() const noexcept;

    /*
        Returns pointer to node value searched
        If value does not exist in tree returns nullptr
        *noexcept *const
    */
    bstNode<t>* search(const t& value) const noexcept;

    /*
        Returns the number of nodes in the tree
        *noexcept *const
    */
    size_t size() const noexcept;

    /*
        Removes all elements
        *noexcept
    */

    void operator=(BSearchTree<t>& bst)noexcept;

    ~BSearchTree() noexcept;
};

template<typename t>
BSearchTree<t>::BSearchTree(std::initializer_list<t> initList)noexcept {
    for (auto i : initList) {
        add(i);
    }
}

template<typename t>
BSearchTree<t>::BSearchTree(const BSearchTree<t>& bst)noexcept {
    bst.forEach([&](t element) {
        this->add(element);
                });
}

template<typename t>
template<typename d>
BSearchTree<t>::BSearchTree(const d& otherIterable) noexcept{
    for (auto i : otherIterable) {
        this->add(i);
    }
}

template<typename t>
size_t BSearchTree<t>::size() const noexcept {
    return this->length;
}

template <typename t>
void BSearchTree<t>::add(const t& value) noexcept {
    if (start == nullptr) {
        bstNode<t>* newnode = new bstNode<t>();
        newnode->m_value = value;
        start = newnode;
    }
    else {
        auto currentNode{ start };
        while (true) {
            if (value > currentNode->m_value) {
                if (currentNode->left == nullptr) {
                    bstNode<t>* newnode = new bstNode<t>();
                    newnode->m_value = value;
                    currentNode->left = newnode;
                    break;  // this concludes
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else {
                if (currentNode->right == nullptr) {
                    bstNode<t>* newnode = new bstNode<t>();
                    newnode->m_value = value;
                    currentNode->right = newnode;
                    break;  // this concludes
                }
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    this->length += 1;
}

template <typename t>
bool BSearchTree<t>::addIfNotExists(const t& value) noexcept {
    if (start == nullptr) {
        bstNode<t>* newnode = new bstNode<t>();
        newnode->m_value = value;
        start = newnode;
    }
    else {
        auto currentNode{ start };
        while (true) {
            if (value > currentNode->m_value) {
                if (currentNode->left == nullptr) {
                    bstNode<t>* newnode = new bstNode<t>();
                    newnode->m_value = value;
                    currentNode->left = newnode;
                    break;  // this concludes
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else if (value == currentNode->m_value) {
                return false;
            }
            else {
                if (currentNode->right == nullptr) {
                    bstNode<t>* newnode = new bstNode<t>();
                    newnode->m_value = value;
                    currentNode->right = newnode;
                    break;  // this concludes
                }
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    this->length += 1;
    return true;
}

template <typename t>
bool BSearchTree<t>::remove(const t& value) noexcept{
    auto currentNode{ start };
    bstNode<t>* beforeNode{};
    while (currentNode != nullptr) {
        if (currentNode->m_value == value) {
            break;
        }
        else if (currentNode->m_value < value) {
            beforeNode = currentNode;
            currentNode = currentNode->left;
        }
        else {
            beforeNode = currentNode;
            currentNode = currentNode->right;
        }
    }
    // value not found
    if (currentNode == nullptr) {
        return false;
    }

    if (currentNode->left == nullptr && currentNode->right == nullptr) {
        if (beforeNode == nullptr) {
            delete start;
            start = nullptr;
        }
        else {
            if (currentNode == beforeNode->right) {
                beforeNode->right = nullptr;
            }
            else {
                beforeNode->left = nullptr;
            }
            delete currentNode;
        }
    }
    else if (currentNode->left != nullptr && currentNode->right != nullptr) {
        auto temp{ currentNode->left };
        bstNode<t>* temp2{ nullptr };
        while (temp->right != nullptr) {
            temp2 = temp;
            temp = temp->right;
        }
        if (temp->left == nullptr) {
            currentNode->m_value = temp->m_value;
            if (temp2 == nullptr) {
                currentNode->left = nullptr;
            }
            else {
                temp2->right = nullptr;
            }
        }
        else {
            currentNode->m_value = temp->m_value;
            if (temp2 == nullptr) {
                currentNode->left = temp->left;
            }
            else {
                temp2->right = temp->left;
            }
        }
        delete temp;
    }
    else if (currentNode->left != nullptr) {
        if (beforeNode == nullptr) {
            auto temp{ start };
            start = currentNode->left;
            delete temp;
        }
        else {
            if (beforeNode->right == currentNode) {
                beforeNode->right = currentNode->left;
            }
            else {
                beforeNode->left = currentNode->left;
            }
            delete currentNode;
        }
    }
    else {
        if (beforeNode == nullptr) {
            auto temp{ start };
            start = currentNode->right;
            delete temp;
        }
        else {
            if (beforeNode->right == currentNode) {
                beforeNode->right = currentNode->right;
            }
            else {
                beforeNode->left = currentNode->right;
            }
            delete currentNode;
        }
    }
    this->length -= 1;
    return true;
    
}

template <typename t>
bstNode<t>* BSearchTree<t>::search(const t& value)const noexcept {
    auto currentNode{ start };
    while (currentNode != nullptr) {
        if (currentNode->m_value == value) {
            return currentNode;
        }
        else if (currentNode->m_value < value) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
    return nullptr;
}

template <typename t>
void BSearchTree<t>::traverse(std::function<void(bstNode<t>* element)> process) {
    std::stack<bstNode<t>*> tStack{};
    if (start == nullptr) {
        return;
    }
    tStack.push(start);
    bstNode<t>* currentNode{};
    while (!tStack.empty()) {
        currentNode = tStack.top();
        if (currentNode->left != nullptr) {
            currentNode = currentNode->left;
            tStack.push(currentNode);
        }
        else {
            auto tempNode{ currentNode->right };
            //std::cout << currentNode->m_value << std::endl;
            process(currentNode);
            
            tStack.pop();
            currentNode = tempNode;
            while (currentNode == nullptr) {
                if (tStack.empty()) {
                    return;
                }
                currentNode = tStack.top();
                tempNode = currentNode->right;
                //std::cout << currentNode->m_value << std::endl;
                process(currentNode);
                tStack.pop();
                currentNode = tempNode;
            }
            tStack.push(currentNode);
        }
    }
}

template <typename t>
void BSearchTree<t>::clear() noexcept{
    traverse([](bstNode<t>* element) {
        delete element;
             });
    start = nullptr;
    this->length = 0;
}

template <typename t>
bool BSearchTree<t>::isEmpty() const noexcept {
    return start == nullptr;
}

template <typename t>
void BSearchTree<t>::forEach(std::function<void(const t& element)> process) noexcept{
    traverse([&process](bstNode<t>* elementptr) {
        process(elementptr->m_value);
             });
}

template <typename t>
void BSearchTree<t>::merge(BSearchTree<t>& bst) noexcept {
    bst.forEach([&](const t& element) {
        this->add(element);
                });
}

template <typename t>
template <typename d>
void BSearchTree<t>::merge(const d& otherIterable) noexcept  {
    for (auto i : otherIterable) {
        this->add(i);
    }
}

template <typename t>
void BSearchTree<t>::operator=(BSearchTree<t>& bst) noexcept{
    this->clear();
    this->merge(bst);
}

template <typename t>
BSearchTree<t>::~BSearchTree() noexcept  {
    clear();
}