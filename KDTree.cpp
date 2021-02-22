#include "KDTree.h"
#include "Point.h"

#include <iostream>
#include <algorithm>
#include <queue>

// Helper functor
template<typename T>
struct DataComp {
    size_t split;

    DataComp(size_t dim): split(dim) {}

    bool operator() (const T& first, const T& second) {
        return first[split] < second[split];
    }
};

template <size_t dim, typename T>
KDTree<dim, T>::KDTree(std::vector<T>& dataset) {
    root = buildTree(dataset, 0, 0, dataset.size());
}

template <size_t dim, typename T>
KDTree<dim, T>::~KDTree() {
    freeTree(root);
}

template <size_t dim, typename T>
typename KDTree<dim, T>::TreeNode* KDTree<dim, T>::buildTree(std::vector<T>& dataset, 
                                                             size_t level, 
                                                             size_t startIndex, 
                                                             size_t endIndexPlusOne) {
    if (startIndex >= endIndexPlusOne) {
        return nullptr;
    }

    size_t currSplit = level % dim;
    size_t middleIndex = startIndex + (endIndexPlusOne - startIndex) / 2;
    std::nth_element(dataset.begin() + startIndex, dataset.begin() + middleIndex, dataset.begin() + endIndexPlusOne, DataComp<T>(currSplit));
    auto currNode = new TreeNode(currSplit, dataset[middleIndex]);

    currNode->left = buildTree(dataset, level + 1, startIndex, middleIndex);
    currNode->right = buildTree(dataset, level + 1, middleIndex + 1, endIndexPlusOne);

    return currNode;
}

template <size_t dim, typename T>
void KDTree<dim, T>::freeTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    freeTree(node->left);
    freeTree(node->right);

    delete node;
}

template <size_t dim, typename T>
void KDTree<dim, T>::printTree(std::ostream& out, TreeNode* node) const {
    if (node == nullptr) {
        return;
    }

    // Do a level order treversal to print out the tree
    std::queue<TreeNode*> printQueue;
    printQueue.push(node);
    size_t numNodeCurrLevel = 1;
    size_t numNodeNextLevel = 0;

    while (!printQueue.empty()) {
        numNodeNextLevel = 0;
        for (size_t i = 0; i < numNodeCurrLevel; ++i) {
            TreeNode* curr = printQueue.front();
            printQueue.pop();

            if (curr->left != nullptr) {
                printQueue.push(curr->left);
                numNodeNextLevel++;
            }

            if (curr->right != nullptr) {
                printQueue.push(curr->right);
                numNodeNextLevel++;
            }

            if (i == numNodeCurrLevel - 1) {
                out << curr->data;
            }
            else {
                out << curr->data << ", ";
            }
        }

        numNodeCurrLevel = numNodeNextLevel;

        if (!printQueue.empty()) {
            out << std::endl;
        }
    }
}

template class KDTree<2, Point<2, double>>;
template class KDTree<2, Point<2, float>>;
template class KDTree<3, Point<3, double>>;
template class KDTree<3, Point<3, float>>;
