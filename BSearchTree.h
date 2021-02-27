#pragma once
#include<string>
#include<initializer_list>
#include <stack>
#include <iostream>
#include <functional>


template <typename t>
struct bstNode
{
	t m_value{};
	bstNode<t>* right{};
	bstNode<t>* left{};
};

template <typename t>
class BSearchTree {
private:
	bstNode<t>* start{};
	int_fast64_t length{ 0 };
    //traverse function does allow elements to change
    //could be made public if required or wrapped with a safer function
    void traverse(std::function<void(bstNode<t>* element)> process);
public:
    BSearchTree() {};
    BSearchTree(std::initializer_list<t> initList);
    BSearchTree(BSearchTree<t>& bst);
    template<typename d>
    BSearchTree(d& otherIterable);
	void add(t value);
    int remove(t value);
    //merge method does not clear merged bst
    void merge(BSearchTree<t>& bst);
    template<typename d>
    void merge(d& otherIterable);
    void clear();
    bstNode<t>* search(t value);
    bool isEmpty();
    //forEach function does not allow elements to change
    void forEach(std::function<void(t& element)> process);
    ~BSearchTree();
};

template<typename t>
BSearchTree<t>::BSearchTree(std::initializer_list<t> initList) {
    for (auto i : initList) {
        add(i);
    }
}

template<typename t>
BSearchTree<t>::BSearchTree(BSearchTree<t>& bst) {
    bst.forEach([&](t element) {
        this->add(element);
                });
}

template<typename t>
template<typename d>
BSearchTree<t>::BSearchTree(d& otherIterable) {
    for (auto i : other) {
        this->add(i);
    }
}

template <typename t>
void BSearchTree<t>::add(t value) {
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
int BSearchTree<t>::remove(t value) {
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
    if (currentNode == nullptr) {
        return -1;
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
    return 0;
    
}

template <typename t>
bstNode<t>* BSearchTree<t>::search(t value) {
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
void BSearchTree<t>::clear() {
    traverse([](bstNode<t>* element) {
        delete element;
             });
    start = nullptr;
    this->length = 0;
}

template <typename t>
bool BSearchTree<t>::isEmpty() {
    return start == nullptr;
}

template <typename t>
void BSearchTree<t>::forEach(std::function<void(t& element)> process) {
    traverse([&](bstNode<t>* elementptr) {
        process(elementptr->m_value);
             });
}

template <typename t>
void BSearchTree<t>::merge(BSearchTree<t>& bst) {
    bst.forEach([&](t element) {
        this->add(element);
                });
}

template <typename t>
template <typename d>
void BSearchTree<t>::merge(d& otherIterable) {
    for (auto i : otherIterable) {
        this->add(i);
    }
}

template <typename t>
BSearchTree<t>::~BSearchTree() {
    clear();
}