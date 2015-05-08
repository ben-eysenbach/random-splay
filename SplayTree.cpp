#include <stdio.h>
#include <iostream>
#include <cmath>
#include "SplayTree.h"

Node *CreateNode(int key) {
    Node *node = new Node();
    node->key = key;
    return node;
}

void Rotate(Node *node) {
    // performs a rotatation w.r.t a given node. If the node does not have a parent, we return the original node
    if (node->parent) {
        Node *parent = node->parent;
        if (parent->parent) {
            node->parent = parent->parent;
            if (parent->parent->left == parent) {
                parent->parent->left = node;
            } else {
                parent->parent->right = node;
            }
        } else {
            node->parent = NULL;
        }
        if (parent->left == node) {
            node->right = parent;
            if (node->right) {
                parent->left = node->right;
            }
            parent->left->parent = parent;

        } else { //parent->right == node
            node->left = parent;
            if (node->left) {
                parent->right = node->left;
            }
            parent->right->parent = parent;
        }
        parent->parent = node;
    }
}


Node *PerfectTree(int h) {
    // creates a perfect BST with height h (tree with single node has height 0). The root will have key 2^h, and the tree will contain nodes 1 through 2^(h+1)-1 (inclusive)
    int key = (int) pow(2, h);
    Node *root = PerfectTreeHelper(key, h);
    return root;
}

Node *PerfectTreeHelper(int key, int h) {
    Node *root = CreateNode(key);
    if (h > 0) {
        root->left = PerfectTreeHelper(key - (int) pow(2, h-1), h-1);
        root->right = PerfectTreeHelper(key + (int) pow(2, h-1), h-1);
        root->left->parent = root;
        root->right->parent = root;
    }
    return root;
}

Node *Find(Node *root, int key) {
    // searches for and returns the node with a given key
    Node *node = root;
    while (true) {
        if (node->left && node->key > key) {
            node = node->left;
        } else if (node->right && node->key < key) {
            node = node->right;
        } else if (node->key == key) {
            return node;
        } else {
            return NULL;
        }
    }
}

void Splay(Node *node) {
    // splays a node to root
    while (node->parent) {
        SplayHelper(node);
    }
}

void SplayHelper(Node *node) {
    // splays a node up one level
    if (node->parent) {
        printf("has parent\n");
        Node *parent = node->parent;
        if (!parent->parent) {
            printf("no grandparent\n");
            Rotate(node);
            printf("done rotating\n");
        } else {
            printf("has grandparent\n");
            Node *grandparent = parent->parent;
            if (grandparent->left->left == node || grandparent->right->right == node) {
                Rotate(parent);
                Rotate(node);
            } else { //grandparent->left->right == node || grandparent->right->left == node
                Rotate(node);
                Rotate(node);
            }
        }
    }
}



int main(int argc, char** argv) {
    Node *root = PerfectTree(3);
    Node *node = Find(root, 7);
    Rotate(node);
    Rotate(node);
    printf("root:%d\n", node->key);
    printf("left:%d\n", node->left->key);
    printf("right:%d\n", node->right->key);
    // printf("%d\n", node->parent->key);
    return 0;
}
