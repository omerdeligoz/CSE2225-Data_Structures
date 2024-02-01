//Ömer Deligöz 150120035

#include <stdio.h>
#include <stdlib.h>
#define MAX(X, Y) ((X > Y) ? X : Y) //define a macro to find the maximum of two numbers

//Some necessary global variables
int avlRotateCounter = 0;
int splayRotateCounter = 0;
int avlSearchCounter = 0;
int splaySearchCounter = 0;

struct avlNode {
    int key, height, occurrence;
    struct avlNode *left, *right;
};
struct splayNode {
    int key, occurrence;
    struct splayNode *left, *right;
};
typedef struct avlNode avlNode;
typedef struct splayNode splayNode;

//Function prototypes
void printSplayTree(splayNode *root);
void printAvlTree(avlNode *root);
int getHeight(avlNode *node);
void updateHeight(avlNode *root);
int isBalanced(avlNode *root);
avlNode *newAvlNode(int key);
splayNode *newSplayNode(int key);
avlNode *rightRotateAvl(avlNode *root);
avlNode *leftRotateAvl(avlNode *root);
splayNode *rightRotateSplay(splayNode *root);
splayNode *leftRotateSplay(splayNode *root);
splayNode *splay(splayNode *root, int key);
splayNode *splayRecursively(splayNode *root, int key);
avlNode *insertAvlNode(avlNode *root, int key);
avlNode *balanceTree(avlNode *root, int key);
splayNode *insertSplayNode(splayNode *root, int key);


void printSplayTree(splayNode *root) { //print splay tree recursively (preorder traversal)
    if (root != NULL) {
        printf("%d ", root->key);
        printSplayTree(root->left);
        printSplayTree(root->right);
    }
}

void printAvlTree(avlNode *root) { //print avl tree recursively (preorder traversal)
    if (root != NULL) {
        printf("%d ", root->key);
        printAvlTree(root->left);
        printAvlTree(root->right);
    }
}

int getHeight(avlNode *node) { //return getHeight of the given node
    return ((node == NULL) ?-1:node->height); //if the node is not NULL return the getHeight of node
}

void updateHeight(avlNode *root) { //recalculate the getHeight of the given node after a rotation is made
    root->height = (getHeight(root->left) > getHeight(root->right)) ? (getHeight(root->left) + 1) : (getHeight(root->right) + 1);
}

int isBalanced(avlNode *root) { //check if given avl tree is balanced or not
    if ((root == NULL) || (abs(getHeight(root->left) - getHeight(root->right)) >= 2))
        return 0;
    return 1;
}

avlNode *newAvlNode(int key) { //create a new Avl tree node with the given key
    avlNode *node = (avlNode *) malloc(sizeof(avlNode));
    node->key = key;
    node->right = NULL;
    node->left = NULL;
    node->height = 0;
    node->occurrence = 1;
    return node;
}

splayNode *newSplayNode(int key) {//create a new splay tree node with the given key
    splayNode *node = (splayNode *) malloc(sizeof(splayNode));
    node->key = key;
    node->right = NULL;
    node->left = NULL;
    node->occurrence = 1;
    return node;
}

avlNode *rightRotateAvl(avlNode *root) { // make a right rotation in avl tree
    avlRotateCounter++; //count the rotation numbers
    avlNode *temp = root;
    avlNode *temp2 = root->left->right;
    root = root->left;
    root->right = temp;
    temp->left = temp2;
    temp->height = MAX(getHeight(temp->left), getHeight(temp->right)) + 1; //find and define new heights
    root->height = MAX(getHeight(root->left), getHeight(root->right)) + 1;
    return root;
}

avlNode *leftRotateAvl(avlNode *root) { // make a left rotation in avl tree
    avlRotateCounter++;
    avlNode *temp = root;
    avlNode *temp2 = root->right->left;
    root = root->right;
    root->left = temp;
    temp->right = temp2;
    temp->height = MAX(getHeight(temp->left), getHeight(temp->right)) + 1;
    root->height = MAX(getHeight(root->left), getHeight(root->right)) + 1;
    return root;
}

splayNode *rightRotateSplay(splayNode *root) { // make a right rotation in splay tree
    splayRotateCounter++;
    splayNode *temp = root;
    splayNode *temp2 = root->left->right;
    root = root->left;
    root->right = temp;
    temp->left = temp2;
    return root;
}

splayNode *leftRotateSplay(splayNode *root) { // make a left rotation in splay tree
    splayRotateCounter++;
    splayNode *temp = root;
    splayNode *temp2 = root->right->left;
    root = root->right;
    root->left = temp;
    temp->right = temp2;
    return root;
}

splayNode *splay(splayNode *root, int key) {
    do {
        root = splayRecursively(root, key);
    } while (root->key != key);  //repeat until the given key reaches root
    return root;
}

splayNode *splayRecursively(splayNode *root, int key) {
    if ((root->key == key) || root == NULL) // no need to splay
        return root;
    else {
        if (key < root->key) {  //key is in the left subtree
            if (key == root->left->key) {                                                //just zig case
                root = rightRotateSplay(root);
            } else if ((root->left->left != NULL) && (key == root->left->left->key)) {   //zig-zig case
                root = rightRotateSplay(root);
                root = rightRotateSplay(root);
            } else if ((root->left->right != NULL) && (key == root->left->right->key)) { //zig-zag case
                root->left = leftRotateSplay(root->left);
                root = rightRotateSplay(root);
            } else {
                root->left = splayRecursively(root->left, key); //if key is at lower depth level, search again
            }
        } else { //key is in the right subtree
            if (key == root->right->key) {                                                  //just zag case
                root = leftRotateSplay(root);
            } else if ((root->right->right != NULL) && (key == root->right->right->key)) {  //zag-zag case
                root = leftRotateSplay(root);
                root = leftRotateSplay(root);
            } else if ((root->right->left != NULL) && (key == root->right->left->key)) {    //zag-zig case
                root->right = rightRotateSplay(root->right);
                root = leftRotateSplay(root);
            } else {
                root->right = splayRecursively(root->right, key); //if key is at lower depth level, search again
            }
        }
    }
    return root;
}

//Place the given key in the appropriate place in the avlTree. and if there is an imbalance in the tree make it balanced
avlNode *insertAvlNode(avlNode *root, int key) {
    if (root == NULL) {
        return newAvlNode(key); //Create a new node with the given key and add it to the tree
    } else {
        avlSearchCounter++;
        if (key == root->key) { //increase occurrence value if key already exists in tree
            root->occurrence++;
            return root;
        } else if (key < root->key) { //key is in left subtree
            root->left = insertAvlNode(root->left, key);
        } else {                      //key is in right subtree
            root->right = insertAvlNode(root->right, key);
        }
    }
    //After adding the node, rearrange the heights of the nodes on the path
    updateHeight(root);

    //if there is unbalance in the tree, make it balanced
    if (!isBalanced(root)) {
        root = balanceTree(root,key);
    }
    return root;
}

avlNode *balanceTree(avlNode *root, int key){
    if (key < root->key) { //key is in the left subtree
        if (key < root->left->key) {        //LL case
            root = rightRotateAvl(root);
        } else if (key > root->left->key) { //RL case
            root->left = leftRotateAvl(root->left);
            root = rightRotateAvl(root);
        }
    } else { //key is in the right subtree
        if (key < root->right->key) {        //LR case
            root->right = rightRotateAvl(root->right);
            root = leftRotateAvl(root);
        } else if (key > root->right->key) { //RR case
            root = leftRotateAvl(root);
        }
    }
    return root;
}

//insert the given key in the appropriate place in the splayTree.
splayNode *insertSplayNode(splayNode *root, int key) {
    if (root == NULL) {
        return newSplayNode(key); //Create a new node with the given key and add it to the tree
    } else {
        splaySearchCounter++;
        if (key == root->key) { //increase occurrence value if key already exists in tree
            root->occurrence++;
            return root;
        } else if (key < root->key) { //key is in the left subtree
            root->left = insertSplayNode(root->left, key);
        } else {    //key > root->key //key is in the right subtree
            root->right = insertSplayNode(root->right, key);
        }
    }
    return root;
}


int main(int argc, char *argv[]) {
    //assign two roots for avl and splay trees
    splayNode *splayRoot = NULL;
    avlNode *avlRoot = NULL;
    if (argc == 1) {    //print an error message if no arguments
        printf("Please give an input file!");
        return -1;
    }
    if (argc >= 2) {
        FILE *inputFilePtr = fopen(argv[1], "r"); //take the input file as an argument
        int input;
        while (!feof(inputFilePtr)) { //read the input file to the end
            fscanf(inputFilePtr, "%d", &input); //take the next integer from input file

            splayRoot = insertSplayNode(splayRoot, input); //insert the node in splay tree
            splayRoot = splay(splayRoot, input);     //bring the added node to root
            avlRoot = insertAvlNode(avlRoot, input); //insert the node in avl tree
        }
    }
    //Print the results
    printf("AVL tree:\n");
    printAvlTree(avlRoot);
    printf("\nTotal cost: %d\n", avlRotateCounter + avlSearchCounter); //Total cost of AVL tree
    printf("\nSplay tree:\n");
    printSplayTree(splayRoot);
    printf("\nTotal cost: %d", splayRotateCounter + splaySearchCounter); //Total cost of splay tree
    return 0;
}


