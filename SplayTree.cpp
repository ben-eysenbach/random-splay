#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cassert>
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
            // printf("no parent\n");
            node->parent = NULL;
        }
        if (parent->left == node) {
            if (node->right) {
                parent->left = node->right;
                parent->left->parent = parent;
            } else {
                parent->left = NULL;
            }
            node->right = parent;

        } else { //parent->right == node
            if (node->left) {
                parent->right = node->left;
                parent->right->parent = parent;
            } else {
                parent->right = NULL;
            }
            node->left = parent;
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

Node *Splay(Node *root, Node *node) {
    // splays a node to root
    int count = 0;
    while (node->parent) {
        count++;
        SplayHelper(node);
    }
    printf("%d\n", count);
    return node;
}

void SplayHelper(Node *node) {
    // splays a node up one level
    if (node->parent) {
        // printf("has parent\n");
        Node *parent = node->parent;
        if (!parent->parent) {
            // printf("no grandparent\n");
            Rotate(node);
            // printf("done rotating\n");
        } else {
            // printf("has grandparent\n");
            Node *grandparent = parent->parent;
            if ((grandparent->left && (grandparent->left->left == node)) ||
                (grandparent->right && (grandparent->right->right == node))) {
                Rotate(parent);
                Rotate(node);
            } else { //grandparent->left->right == node || grandparent->right->left == node
                Rotate(node);
                Rotate(node);
            }
        }
    }
}


// Node *ProbSplay1(Node *root, Node *node, float prob) {
//     // Splays node to root given probability; otherwise, does nothing
//     if (rand() < prob * RAND_MAX) {
//         return Splay(root, node);
//     } else {
//         return root;
//     }
// }

// Node *ProbSplay2(Node *root, Node *node, float prob) {
//     // There are a series of zig, zig-zig, zig-zag operations performed to splay a node to root. This function performs each with the given probability
//     while (node->parent) {
//         if (rand() < prob * RAND_MAX) {
//             SplayHelper(node);
//         } else {
//             node = node->parent;
//         }
//     }
//     return node;
// }
//
// Node *ProbSplay3(Node *root, Node *node, float prob) {
//     // Starts splaying given node up to root. At each operation, we flip a biased coin, and stop splaying after it lands on heads for the first time.
//     while (node->parent) {
//         if (rand() < prob * RAND_MAX) {
//             SplayHelper(node);
//         } else {
//             return root;
//         }
//     }
//     return node;
// }

int Validate(Node *root) {
    // confirms that the given node is the root of a valid BST
    if (root->parent) {
        return 0;
    } else {
        return ValidateHelper(root);
    }
}

int ValidateHelper(Node *root) {
    // confirms that the subtree is a valid BST
    int left = 0;
    int right = 0;
    if (root->right) {
        // if has right child, must satisfy two conditions
        if ((root->right->key > root->key) && ValidateHelper(root->right)) {
            right = 1;
        }
    } else {
        // if no right child, conditions trivially satisfied
        right = 1;
    }

    if (root->left) {
        // if has left child, must satisfy two conditions
        if ((root->left->key < root->key) && ValidateHelper(root->left)) {
            left = 1;
        }
    } else {
        // if no left child, conditions trivially satisfied
        left = 1;
    }
    return (left && right);
}

void Print(Node *node) {
    if (node->parent) {
        printf("parent:%d\n", node->parent->key);
        if (node->parent->parent) {
            printf("parent->parent:%d\n", node->parent->parent->key);
        } else {
            printf("parent->parent:NULL\n");
        }
    } else {
        printf("parent:NULL\n");
    }
    printf("node:%d\n", node->key);
    if (node->left) {
        printf("left:%d\n", node->left->key);
    } else {
        printf("left:NULL\n");
    }
    if (node->right) {
        printf("right:%d\n\n", node->right->key);
    } else {
        printf("right:NULL\n\n");
    }
}

int Uniform(int low, int high) {
    // returns a random integer between low and high, inclusive
    int overflow = RAND_MAX % (high - low + 1);
    int r = rand();
    while (r > RAND_MAX - overflow) {
        r = rand();
    }
    return r % (high - low + 1) + low;
}

int Geometric(float p, int max) {
    // samples from the geometric distribution. Samples range from 1 to max, inclusive
    int sample = max + 1;
    while (sample > max) {
        float x = ((float) rand()) / RAND_MAX;
        sample = (int) ceil(log(x) / log(1 - p));
    }
    return sample;
}

int main(int argc, char** argv) {
    int h = 15;
    Node *root = PerfectTree(h);

    Node *node;
    int n = (int) pow(2, h+1) - 1;
    int key;

    for (int i = 0; i < 100000; i++) {
        key = Geometric(0.5, n);
        // printf("key:%d\n", key);
        node = Find(root, key);
        // Print(node);
        root = Splay(root, node);
        // assert(root->key == key);
        // assert(Validate(node));
    }
    for (int i = 0; i <= n; i++) {
        assert(Find(root, key));
    }
    // printf("Test passed!\n");
    return 0;
}
