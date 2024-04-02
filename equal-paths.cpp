#ifndef RECCHECK
// if you want to add any #includes like <iostream> you must do them here
// (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;

// You may add any prototypes of helper functions here

bool checkEqualPaths(Node* root, int curDepth, int& globalDepth) {
    if (root->left == nullptr && root->right == nullptr) {
        if (globalDepth == -1)
            globalDepth = curDepth;
        return (curDepth == globalDepth);
    }
    bool isEqual = true;
    if (root->left != nullptr)
        isEqual &= checkEqualPaths(root->left, curDepth + 1, globalDepth);
    if (root->right != nullptr)
        isEqual &= checkEqualPaths(root->right, curDepth + 1, globalDepth);
    return isEqual;
}

bool equalPaths(Node* root) {
    int globalDepth = -1;
    if (root == nullptr)
        return true;
    return checkEqualPaths(root, 0, globalDepth);
}
