#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cassert>
#include <time.h>
#include <limits.h>
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
    int height = 0;
    while (true) {
        height += 1;
        if (node->left && node->key > key) {
            node = node->left;
        } else if (node->right && node->key < key) {
            node = node->right;
        } else if (node->key == key) {
            // printf("%d,", count);
            node->height = height;
            return node;
        } else {
            return NULL;
        }
    }
}

Node *Splay(Node *root, Node *node) {
    // splays a node to root
    // int count = 0;
    while (node->parent) {
        // count += 1;
        SplayHelper(node);
    }
    // printf("%d\n", count);
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


Node *ProbSplay1(Node *root, Node *node, float prob) {
    // Splays node to root given probability; otherwise, does nothing
    // for all Geo(p), best p_splay = 0.1
    // This is the version used in Albers' paper
    if (rand() < prob * RAND_MAX) {
        return Splay(root, node);
    } else {
        return root;
    }
}

Node *ProbSplay2(Node *root, Node *node, float prob) {
    // There are a series of zig, zig-zig, zig-zag operations performed to splay a node to root. This function performs each with the given probability
    // int count = 0;
    while (node->parent) {
        if (rand() < prob * RAND_MAX) {
            // count += 1;
            SplayHelper(node);
        } else {
            node = node->parent;
        }
    }
    // printf("%d\n", count);
    return node;
}

Node *ProbSplay3(Node *root, Node *node, float prob) {
    // Starts splaying given node up to root. At each operation, we flip a biased coin, and stop splaying after it lands on heads for the first time.
    // For
    while (node->parent) {
        if (rand() < prob * RAND_MAX) {
            SplayHelper(node);
        } else {
            return root;
        }
    }
    return node;
}

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
    int diff = high - low + 1;
    int exponent = ceil(log(diff) / log(RAND_MAX)); // how many random numbers do we need to use to get a single sample
    int rand_max = pow(RAND_MAX, exponent);
    int overflow = rand_max % diff;
    int r = rand_max;
    while (r > rand_max - overflow) {
        r = 0;
        for (int i = 0; i < exponent; i++) {
            r += rand() * pow(RAND_MAX, i);
        }
    }
    return r % (diff) + low;
}

int Geometric(float p, int high) {
    // samples from the geometric distribution. Samples range from 1 to high, inclusive
    assert(p > 0);
    assert(p < 1);
    int sample = high + 1;
    while (sample > high) {
        float x = ((float) rand()) / RAND_MAX;
        sample = (int) ceil(log(x) / log(1 - p));
    }
    return sample;
}

int Zipf(int high) {
    // samples from zipf distribution. Samples range from 1 to high, inclusive
    static int initial = 1;
    static float sum = 0;
    if (initial) {
        for (int i = 1; i <= high; i++) {
            sum += 1.0 / i;
        }
        // printf("sum:%f\n", sum);
        initial = 0;
    }
    float r = (sum * rand()) / RAND_MAX;
    int i = 1;
    while (r > 0) {
        r -= 1.0 / i;
        // printf("i:%d; r:%f\n", i, r);
        i++;
    }
    return i - 1;
}

int main(int argc, char** argv) {
    int h = 15;

    Node *node;
    int n = (int) pow(2, h+1) - 1;
    int key;
    clock_t start, end;
    Node *root = PerfectTree(h);
    int num_samples = 100000;
    int samples[num_samples];
    for (int i = 0; i < num_samples; i++) {
        samples[i] = Zipf(n);
    }
    // float p = 0;
    // float p = pow(2, -3);
    // float p = pow(2, -6);
    // float p = pow(2, -9);
    float p = pow(2, -15);
    float q;
    int height;
    printf("p:%.5f\n", p);
    start = clock();
    for (int i = 0; i < num_samples; i++) {
        // printf("Iter:%d\n", i);

        node = Find(root, samples[i]);
        height = log(n);
        q = (p * height) / (1 + p * height);
        // printf("q:%.3f\n", q);
        root = ProbSplay3(root, node, q);
        // root = Splay(root, node); //42 sec
        // end = clock();
        // printf("%lu\n", end - start);
        // start = end;
    }
    printf("Duration: %.3f\n", ((float)clock() - start) * 1000 / CLOCKS_PER_SEC);
    assert(Validate(root));
    // printf("Test passed!\n");
    //
    // for (float p_geo = 0.1; p_geo <= 0.9; p_geo = p_geo + 0.1) {
    //     int min = INT_MAX;
    //     float best_p_splay = 0;
    //     for (float p_splay = 0; p_splay <= 1; p_splay = p_splay + 0.1) {
    //         printf("Uniform with P(splay)=%.1f\n", p_splay);
    //         Node *root = PerfectTree(h);
    //         clock_t start = clock();
    //         for (int i = 0; i < 10000000; i++) {
    //             key = Geometric(p_geo, n);
    //             node = Find(root, key);
    //             root = ProbSplay3(root, node, p_splay);
    //             // assert(root->key == key);
    //             // assert(Validate(node));
    //         }
    //         // for (int i = 0; i <= n; i++) {
    //         //     assert(Find(root, key));
    //         // }
    //         float msec = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    //         if (msec < min) {
    //             min = msec;
    //             best_p_splay = p_splay;
    //         }
    //         // printf("Test passed!\n");
    //         // printf("Duration: %.3f sec\n", msec / 1000);
    //     }
    //     printf("For p_geo=%.2f, best p_splay=%.2f\n\n", p_geo, best_p_splay);
    // }
    return 0;
}
