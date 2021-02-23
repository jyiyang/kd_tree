#ifndef KD_TREE_H
#define KD_TREE_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

template <typename T>
class KDTree {
public:
    typedef std::pair<T, typename T::NumericType> DataAndDistance;

    KDTree() = delete;
    KDTree(std::vector<T>& dataset);
    ~KDTree();
    void insert(const T& inData);
    std::vector<T> searchKNearest(const T& queryData, size_t k);

    friend std::ostream& operator<< (std::ostream& out, const KDTree& tree) {
        tree.printTree(out, tree.root);

        return out;
    }

private:
    struct TreeNode {
        TreeNode(size_t sp, const T& inData): split(sp), data(inData), left(nullptr), right(nullptr), parent(nullptr) {}

        size_t split;
        T data;

        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
    };

    typedef std::vector<DataAndDistance> ResultList;

    TreeNode* root;
    size_t treeSize;

    TreeNode* buildTree(std::vector<T>& dataset, size_t level, size_t startIndex, size_t endIndexPlusOne, TreeNode* parent);
    void findKNearest(const T& queryData, size_t k, TreeNode* node, std::unordered_set<TreeNode*>& visited, ResultList& results);
    void freeTree(TreeNode* node);
    void printTree(std::ostream& out, TreeNode* node) const;
    TreeNode* findClosestLeafNode(TreeNode* node, const T& queryData);
    TreeNode* getBrotherNode(TreeNode* node);
};

#endif
