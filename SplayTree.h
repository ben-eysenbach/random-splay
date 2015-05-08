typedef struct Node Node;
struct Node {
    int key;
    Node *left;
    Node *right;
    Node *parent;
};

void Rotate(Node *root, Node *node);
Node *PerfectTree(int h);
Node *PerfectTreeHelper(int key, int h);
Node *Find(Node *root, int key);
Node *CreateNode(int key);
void SplayHelper(Node *node);
void Splay(Node *node);
