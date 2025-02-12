#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>

using namespace std;

class Node 
{
    int key;
    Node* left;
    Node* right;
public:
    Node(int k = 0, Node* l = NULL, Node* r = NULL) 
    {
        key = k;
        left = l;
        right = r;
    }
    friend class BinTree;
};

struct NodeList 
{
    Node* current;
    NodeList* next;
};

class BinTree 
{
    Node* root;
public:
    BinTree() { root = NULL; }
    BinTree(int);
    BinTree(int*, int);
    ~BinTree();

    BinTree(const BinTree&);
    Node* CopyTree(const Node*);
    BinTree operator =(const BinTree&);
    Node* GetRoot() { return root; }
    void DeleteTree(Node*);
    void PrintTree(Node*, int);
    int min();
    int max();
    void AddNode(int);
    void DeleteNode(int);
    Node* FindNode(int);
    void LNR(Node*);
    NodeList* TLB();
};



BinTree::BinTree(int n) 
{ // конструктор 2 (случайные ключи)
    srand(time(NULL));
    //if (root == 0) root = NULL;
    for (int i = 0; i < n; i++) 
    {
        int k = rand() % 100;
        AddNode(k);
    }
}

BinTree::BinTree(int* arr, int size) 
{ // конструктор 3 (по массиву)
    //if (root == 0) root = NULL;
    for (int i = 0; i < size; i++) 
    {
        AddNode(arr[i]);
    }
}

BinTree::~BinTree() 
{// деструктор
    DeleteTree(root);
}

BinTree::BinTree(const BinTree& BT2) 
{ // конструктор копирования
    if (BT2.root == NULL) root = NULL;
    else 
    {
        root = new Node(BT2.root->key);
        root->left = CopyTree(BT2.root->left);
        root->right = CopyTree(BT2.root->right);
    }
}

Node* BinTree::CopyTree(const Node* T) 
{ // копирование узлов
    if (T == NULL) return NULL;
    Node* node = new Node(T->key);
    node->left = CopyTree(T->left);
    node->right = CopyTree(T->right);
    return node;
}

BinTree BinTree::operator=(const BinTree& BT2) 
{ // перегрузка присваивания
    if (this == &BT2) return *this;
    this -> ~BinTree();
    if (BT2.root == NULL) root = NULL;
    else 
    {
        root = new Node(BT2.root->key);
        root->left = CopyTree(BT2.root->left);
        root->right = CopyTree(BT2.root->right);
    }
    return *this;
}

void BinTree::DeleteTree(Node* CurNode) 
{ // удаление дерева
    if (CurNode != NULL) 
    {
        DeleteTree(CurNode->left);
        DeleteTree(CurNode->right);
        delete CurNode;
    }
}

void BinTree::PrintTree(Node* CurNode, int k) 
{ // вывод дерева
    if (CurNode == NULL) return;
    PrintTree(CurNode->right, k + 5);
    for (int i = 0; i < k; i++) cout << " ";
    cout.width(2);
    cout << CurNode->key << endl;
    PrintTree(CurNode->left, k + 5);
}

int BinTree::min() 
{ // нахождение минимума
    if (root == NULL) { cout << "min: Tree is empty\n"; return -1; }
    Node* node = root;
    while (node->left) node = node->left;
    if (node) return node->key;
    return -1;
}

int BinTree::max() 
{ // нахождение максимума
    if (root == NULL) { cout << "max: Tree is empty\n"; return -1; }
    Node* node = root;
    while (node->right) node = node->right;
    if (node) return node->key;
    return -1;
}

void BinTree::AddNode(int k) 
{ // добавление узла
    if (root == NULL) 
    {
        root = new Node(k);
        return;
    }
    Node* node = root;
    while (node) {
        if (node->key > k && node->left == NULL) 
        {
            node->left = new Node(k);
            return;
        }
        if (node->key < k && node->right == NULL) 
        {
            node->right = new Node(k);
            return;
        }
        /**/if (node->key == k)
        {
            while (node->right && node->right->key == k)
            {
                node = node->right;
            }
            Node* tmp = new Node(k, NULL, node->right);
            //node->left = NULL;
            node->right = tmp;
            return;
        }/**/
        if (node->key > k) node = node->left;
        else node = node->right;
    }
}

void BinTree::DeleteNode(int k) { // удаление узла
    Node* node = root;
    Node* parent = NULL;
    while (node && node->key != k) {
        parent = node;
        if (node->key > k) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
    if (!node) return;
    if (node->left == NULL) {
        if (parent && parent->left == node) parent->left = node->right;
        if (parent && parent->right == node) parent->right = node->right;
        delete node;
        return;
    }
    if (node->right == NULL) {
        if (parent && parent->left == node) parent->left = node->left;
        if (parent && parent->right == node) parent->right = node->left;
        delete node;
        return;
    }

    Node* tmp = node->left;
    parent = NULL;
    if (tmp->right == NULL) {
        node->key = tmp->key;
        node->left = tmp->left;
        delete tmp;
        return;
    }
    else {
        while (tmp->right != NULL) {
            parent = tmp;
            tmp = tmp->right;
        }
        node->key = tmp->key;
        parent->right = tmp->left;
        delete tmp;
        return;
    }
}

Node* BinTree::FindNode(int k) { // поиска узла по ключу
    if (root == NULL) return NULL;
    Node* node = root;
    while (node) {
        if (node->key == k) return node;
        if (node->key > k) node = node->left;
        else node = node->right;
    }
    return NULL;
}

void BinTree::LNR(Node* node) { // ЛКП обход
    if (node == NULL) node = root;
    if (node->left) LNR(node->left);
    cout << node->key << " ";
    if (node->right) LNR(node->right);
}

NodeList* BinTree::TLB() { // обход по уровням
    NodeList* current;
    NodeList* t;
    NodeList* h = new NodeList;
    h->current = root;
    h->next = NULL;
    t = h;
    current = h;
    while (h != NULL) {
        if (h->current->left != NULL) {
            NodeList* L = new NodeList;
            L->current = h->current->left;
            L->next = NULL;
            current->next = L;
            current = current->next;
        }
        if (h->current->right != NULL) {
            NodeList* R = new NodeList;
            R->current = h->current->right;
            R->next = NULL;
            current->next = R;
            current = current->next;
        }
        cout << h->current->key << " ";
        h = h->next;
    }
    return t;
}

int main()
{
    srand(time(NULL));
    int arr[16] = { 25,4,20,15,50,78,90,30,34,1,98,17,80,37,85,35 };
    BinTree T(arr,16);
    T.PrintTree(T.GetRoot(), 0);
    cout << "\n\n\n";
    T.LNR(T.GetRoot());
    cout << "\n\n\n";
    T.TLB();
    cout << "\n\n\n";
    T.DeleteNode(50);
    T.PrintTree(T.GetRoot(), 0);
}
