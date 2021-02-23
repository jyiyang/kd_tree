#include "KDTree.h"
#include "Point.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>

// Helper functors
template<typename T>
struct DataComp {
    size_t split;

    DataComp(size_t dim): split(dim) {}

    bool operator() (const T& first, const T& second) {
        return first[split] < second[split];
    }
};

template<typename T>
struct DistanceComp {
    bool operator() (const typename KDTree<T>::DataAndDistance& d1, 
                     const typename KDTree<T>::DataAndDistance& d2) {
        return d1.second < d2.second;
    }
};

template <typename T>
KDTree<T>::KDTree(std::vector<T>& dataset): treeSize(dataset.size()) {
    if (dataset.empty()) {
        return;
    }

    root = buildTree(dataset, 0, 0, dataset.size(), nullptr);
}

template <typename T>
KDTree<T>::~KDTree() {
    freeTree(root);
}

template <typename T>
std::vector<T> KDTree<T>::searchKNearest(const T& queryData, size_t k) {
    std::unordered_set<TreeNode*> visited;
    ResultList results;

    auto leafNode = findClosestLeafNode(root, queryData);
    findKNearest(queryData, k, leafNode, visited, results);

    std::vector<T> output;
    for (const auto& dataAndDistance: results) {
        output.emplace_back(dataAndDistance.first);
    }

    return output;
}

template <typename T>
void KDTree<T>::findKNearest(const T& queryData, 
                             size_t k, TreeNode* node, 
                             std::unordered_set<TreeNode*>& visited, 
                             ResultList& results) {
    if (visited.count(node) == 1) {
        if (node == root) {
            return;
        }

        findKNearest(queryData, k, node->parent, visited, results);
        return;
    }

    visited.insert(node);
    DistanceComp<T> distanceComp;
    auto distance = node->data.calculateSqDistance(queryData);
    auto previousMaxDistance = std::numeric_limits<typename T::NumericType>::max();

    if (!results.empty()) {
        previousMaxDistance = results.back().second;
    }

    if (results.size() < k || distance < previousMaxDistance) {
        results.emplace_back(node->data, distance);
        // NB: This assumes the input K is small. If K is large, 
        //     we should use a bounded priority queue to maintain
        //     the order of the result set. 
        std::sort(results.begin(), results.end(), distanceComp);

        if (results.size() > k) {
            results.pop_back();
        }
    }

    if (node == root) {
        return;
    }

    auto splitDistance = node->parent->data.calculateSqSplitDistance(queryData, node->parent->split);

    if (results.size() < k || splitDistance < previousMaxDistance) {
        TreeNode* brotherNode = getBrotherNode(node);
        TreeNode* leafNode = findClosestLeafNode(brotherNode, queryData);

        if (brotherNode != nullptr && leafNode != nullptr) {
            findKNearest(queryData, k, leafNode, visited, results);
            return;
        }
    }

    findKNearest(queryData, k, node->parent, visited, results);
    return;
}

template <typename T>
typename KDTree<T>::TreeNode* KDTree<T>::buildTree(std::vector<T>& dataset, 
                                                   size_t level, 
                                                   size_t startIndex, 
                                                   size_t endIndexPlusOne,
                                                   TreeNode* parent) {
    if (startIndex >= endIndexPlusOne) {
        return nullptr;
    }

    size_t dataDimension = dataset.front().getDimension();
    size_t currSplit = level % dataDimension;
    size_t middleIndex = startIndex + (endIndexPlusOne - startIndex) / 2;
    std::nth_element(dataset.begin() + startIndex, dataset.begin() + middleIndex, dataset.begin() + endIndexPlusOne, DataComp<T>(currSplit));
    TreeNode* currNode = new TreeNode(currSplit, dataset[middleIndex]);

    currNode->parent = parent;
    currNode->left = buildTree(dataset, level + 1, startIndex, middleIndex, currNode);
    currNode->right = buildTree(dataset, level + 1, middleIndex + 1, endIndexPlusOne, currNode);

    return currNode;
}

template <typename T>
void KDTree<T>::insert(const T& inData) {
    TreeNode* leafNode = findClosestLeafNode(root, inData);

    if (leafNode == nullptr) {
        root = new TreeNode(0, inData);
        treeSize = 1;
    }
    else {
        if (leafNode->data == inData) {
            return;
        }

        TreeNode* newNode = new TreeNode((leafNode->split + 1) % leafNode->data.getDimension(), inData);

        if (inData[leafNode->split] < leafNode->data[leafNode->split]) {
            leafNode->left = newNode;
        }
        else {
            leafNode->right = newNode;
        }

        newNode->parent = leafNode;
        treeSize++;
    }
}

template <typename T>
void KDTree<T>::freeTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    freeTree(node->left);
    freeTree(node->right);

    delete node;
}

template <typename T>
void KDTree<T>::printTree(std::ostream& out, TreeNode* node) const {
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

template <typename T>
typename KDTree<T>::TreeNode* KDTree<T>::findClosestLeafNode(TreeNode* node, const T& queryData) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->left == nullptr && node->right == nullptr) {
        return node;
    }

    TreeNode* nextNode;

    if (node->left == nullptr) {
        nextNode = node->right;
    }
    else if (node->right == nullptr) {
        nextNode = node->left;
    }
    else {
        nextNode = queryData[node->split] < node->data[node->split] ? node->left : node->right;
    }

    return findClosestLeafNode(nextNode, queryData);
}


template <typename T>
typename KDTree<T>::TreeNode* KDTree<T>::getBrotherNode(TreeNode* node) {
    if (node == nullptr || node->parent == nullptr) {
        return nullptr;
    }

    if (node->parent->left == node) {
        return node->parent->right;
    }
    else {
        return node->parent->left;
    }
}

template class KDTree<Point<2, double>>;
template class KDTree<Point<2, float>>;
template class KDTree<Point<3, double>>;
template class KDTree<Point<3, float>>;
