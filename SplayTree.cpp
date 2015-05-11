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


int RANDSPLAY1_SPLAYING;
Node *RandSplay1(Node *root, Node *node, float prob) {
    // Scheme in Tarjan and Sleator's paper
    // Flips a coin after each access and stops splaying after it lands on heads
    if (RANDSPLAY1_SPLAYING && (rand() < prob * RAND_MAX)) {
        return Splay(root, node);
    } else {
        RANDSPLAY1_SPLAYING = 0;
        return root;
    }
}


Node *RandSplay2(Node *root, Node *node, float prob) {
    // Scheme in Alber's paper
    //  Splays node to root given probability; otherwise, does nothing
    if (rand() < prob * RAND_MAX) {
        return Splay(root, node);
    } else {
        return root;
    }
}

Node *RandSplay3(Node *root, Node *node, float prob) {
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

Node *RandSplay4(Node *root, Node *node, float prob) {
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
    // There's a bug which sometimes lets Zipf() return n+1
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
    if (i-1 <= high) {
        return i-1;
    } else {
        return Zipf(high);
    }
}

int main(int argc, char** argv) {
    int h = 16;

    Node *node;
    int n = (int) pow(2, h+1) - 1;

    printf("Building tree\n");
    Node *root = PerfectTree(h);
    int num_samples = 32 * n;
    int *samples = new int[num_samples];

    printf("Generating samples\n");
    for (int i = 0; i < num_samples; i++) {
        samples[i] = Zipf(n);
    }

    float p;
    float duration;
    clock_t start, end;
    printf("Starting tests\n");
    duration = 0;
    for (int j = 0; j < 100; j++) {
        start = clock();
        for (int i = 0; i < num_samples; i++) {
            node = Find(root, samples[i]);
            root = Splay(root, node);
        }
        end = clock();
        duration += (end - start);
    }
    printf("Splay: %.4f\n", (duration / CLOCKS_PER_SEC));
    printf("\n\n");

    for (int exponent = -1; exponent >= -10; exponent--) {
        p = pow(2, exponent);
        duration = 0;
        for (int j = 0; j < 100; j++) {
            RANDSPLAY1_SPLAYING = 1;
            start = clock();
            for (int i = 0; i < num_samples; i++) {
                node = Find(root, samples[i]);
                root = RandSplay1(root, node, p);
            }
            end = clock();
            duration += (end - start);
        }
        printf("RandSplay1 with 2^%d: %.4f\n", exponent, (duration / CLOCKS_PER_SEC));
    }
    printf("\n\n");

    for (int exponent = -1; exponent >= -10; exponent--) {
        p = pow(2, exponent);
        duration = 0;
        for (int j = 0; j < 100; j++) {
            start = clock();
            for (int i = 0; i < num_samples; i++) {
                node = Find(root, samples[i]);
                root = RandSplay2(root, node, p);
            }
            end = clock();
            duration += (end - start);
        }
        printf("RandSplay2 with 2^%d: %.4f\n", exponent, (duration / CLOCKS_PER_SEC));
    }
    printf("\n\n");

    for (int exponent = -1; exponent >= -10; exponent--) {
        p = pow(2, exponent);
        duration = 0;
        for (int j = 0; j < 100; j++) {
            start = clock();
            for (int i = 0; i < num_samples; i++) {
                node = Find(root, samples[i]);
                root = RandSplay3(root, node, p);
            }
            end = clock();
            duration += (end - start);
        }
        printf("RandSplay3 with 2^%d: %.4f\n", exponent, (duration / CLOCKS_PER_SEC));
    }
    printf("\n\n");

    for (int exponent = -1; exponent >= -10; exponent--) {
        p = pow(2, exponent);
        duration = 0;
        for (int j = 0; j < 100; j++) {
            start = clock();
            for (int i = 0; i < num_samples; i++) {
                node = Find(root, samples[i]);
                root = RandSplay4(root, node, p);
            }
            end = clock();
            duration += (end - start);
        }
        printf("RandSplay4 with 2^%d: %.4f\n", exponent, (duration / CLOCKS_PER_SEC));
    }
    printf("\n\n");

    return 0;
}
