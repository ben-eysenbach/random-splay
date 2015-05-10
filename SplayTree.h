typedef struct Node Node;
struct Node {
    int key;
    int height;
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
int ValidateHelper(Node *root);
int Validate(Node *root);
Node *ProbSplay1(Node *root, Node *node, float prob);
Node *ProbSplay2(Node *root, Node *node, float prob);
Node *ProbSplay3(Node *root, Node *node, float prob);
void Print(Node *node);
int Uniform(int low, int high);
int Geometric(float p, int high);
int Zipf(int* bins);
