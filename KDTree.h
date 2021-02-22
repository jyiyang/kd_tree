#ifndef KD_TREE_H
#define KD_TREE_H

#include <iostream>
#include <vector>

template <size_t dim, typename T>
class KDTree {
public:
    KDTree(std::vector<T>& dataset);
    ~KDTree();
    void kNNSearch();

    friend std::ostream& operator<< (std::ostream& out, const KDTree& tree) {
        tree.printTree(out, tree.root);

        return out;
    }

private:
    struct TreeNode {
        TreeNode(size_t sp, const T& inData): split(sp), data(inData), left(nullptr), right(nullptr) {}

        size_t split;
        T data;

        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
    };

    TreeNode* root;
    size_t treeSize;

    TreeNode* buildTree(std::vector<T>& dataset, size_t level, size_t startIndex, size_t endIndexPlusOne);
    void freeTree(TreeNode* node);
    void printTree(std::ostream& out, TreeNode* node) const;
};

#endif
