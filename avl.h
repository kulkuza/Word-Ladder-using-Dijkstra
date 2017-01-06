/*avl.h*/

//
// AVL tree ADT.
//
// Michael Mei
// Linux Mint 18(Sarah) with gcc
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #14
//

typedef struct AVLElementType
{
  char  Word[64];
  int   Vertex;
} AVLElementType;

typedef struct AVLNode
{
  AVLElementType   value;
  int              height;
  struct AVLNode  *left;
  struct AVLNode  *right;
} AVLNode;

AVLNode *CreateAVLTree();
AVLNode *Contains(AVLNode *root, AVLElementType value);
AVLNode *Insert(AVLNode *root, AVLElementType value);

int Count(AVLNode *root);
int Height(AVLNode *root);

void PrintInorder(AVLNode *root);
void FreeAVLTree(AVLNode *root);
