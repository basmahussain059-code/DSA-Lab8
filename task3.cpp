#include <iostream>
using namespace std;

struct node {
    int data;
    int h;
    node *left;
    node *right;
};

node* newNode(int x) {
    node* n = new node;
    n->data = x;
    n->h = 1;
    n->left = NULL;
    n->right = NULL;
    return n;
}

int height(node* n) {
    if (n == NULL) return 0;
    return n->h;
}

int maxi(int a, int b) {
    if (a > b) return a;
    return b;
}

int getBalance(node* n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

node* rightRotate(node* y) {
    node* x = y->left;
    node* t = x->right;
    x->right = y;
    y->left = t;
    y->h = 1 + maxi(height(y->left), height(y->right));
    x->h = 1 + maxi(height(x->left), height(x->right));
    return x;
}

node* leftRotate(node* x) {
    node* y = x->right;
    node* t = y->left;
    y->left = x;
    x->right = t;
    x->h = 1 + maxi(height(x->left), height(x->right));
    y->h = 1 + maxi(height(y->left), height(y->right));
    return y;
}
// balance the tree at this node
node* balance(node* root) {
    root->h = 1 + maxi(height(root->left), height(root->right));
    int bf = getBalance(root);
    if (bf > 1 && getBalance(root->left) >= 0) {
        cout << "LL rotation at " << root->data << endl;
        return rightRotate(root);
    }
    if (bf > 1 && getBalance(root->left) < 0) {
        cout << "LR rotation at " << root->data << endl;
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (bf < -1 && getBalance(root->right) <= 0) {
        cout << "RR rotation at " << root->data << endl;
        return leftRotate(root);
    }
    if (bf < -1 && getBalance(root->right) > 0) {
        cout << "RL rotation at " << root->data << endl;
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
node* insert(node* root, int x) {
    if (root == NULL) return newNode(x);
    if (x < root->data) root->left = insert(root->left, x);
    else if (x > root->data) root->right = insert(root->right, x);
    else return root;   // duplicate
    return balance(root);
}
node* minValueNode(node* root) {
    while (root->left != NULL) root = root->left;
    return root;
}
node* deleteNode(node* root, int x) {
    if (root == NULL) return root;
    if (x < root->data) root->left = deleteNode(root->left, x);
    else if (x > root->data) root->right = deleteNode(root->right, x);
    else {
        if (root->left == NULL || root->right == NULL) {
            node* temp;
            if (root->left != NULL) temp = root->left;
            else temp = root->right;
            delete root;
            return temp;
        }
        node* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return balance(root);
}
bool search(node* root, int x) {
    if (root == NULL) return false;
    if (x == root->data) return true;
    if (x < root->data) return search(root->left, x);
    return search(root->right, x);
}
void printTree(node* root, int space) {
    if (root == NULL) return;
    printTree(root->right, space + 6);
    for (int i = 0; i < space; i++) cout << " ";
    cout << root->data << endl;
    printTree(root->left, space + 6);
}
// top k storage
int result[100];
int count = 0;
void getMax(node* root, int k) {      // reverse inorder
    if (root == NULL || count >= k) return;
    getMax(root->right, k);
    if (count < k) {
        result[count] = root->data;
        count++;
    }
    getMax(root->left, k);
}
void getMin(node* root, int k) {      // inorder
    if (root == NULL || count >= k) return;
    getMin(root->left, k);
    if (count < k) {
        result[count] = root->data;
        count++;
    }
    getMin(root->right, k);
}
int main() {
    node* root = NULL;
    int choice;
    do {
        cout << "\n1. Insert elements\n";
        cout << "2. Top k query (max / min / sum)\n";
        cout << "3. Delete top k\n";
        cout << "4. Update element\n";
        cout << "5. Print tree\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int n, x;
            cout << "How many elements: ";
            cin >> n;
            for (int i = 0; i < n; i++) {
                cout << "Enter element: ";
                cin >> x;
                root = insert(root, x);
                cout << "Tree after inserting " << x << ":" << endl;
                printTree(root, 0);
            }
        }
        else if (choice == 2) {
            int type, k;
            cout << "1.Maximum  2.Minimum  3.Sum of top k max: ";
            cin >> type;
            cout << "Enter k: ";
            cin >> k;
            count = 0;
            if (type == 2) getMin(root, k);
            else getMax(root, k);

            int sum = 0;
            cout << "Top " << count << " elements: ";
            for (int i = 0; i < count; i++) {
                cout << result[i] << " ";
                sum = sum + result[i];
            }
            cout << endl;
            if (type == 3) cout << "Sum = " << sum << endl;
        }
        else if (choice == 3) {
            int type, k;
            cout << "Delete top k: 1.Maximum  2.Minimum: ";
            cin >> type;
            cout << "Enter k: ";
            cin >> k;
            count = 0;
            if (type == 2) getMin(root, k);
            else getMax(root, k);

            int total = count;
            for (int i = 0; i < total; i++) {
                cout << "Deleting " << result[i] << endl;
                root = deleteNode(root, result[i]);
                printTree(root, 0);
            }
        }
        else if (choice == 4) {
    int oldVal, newVal;
    cout << "Enter old value and new value: ";
    cin >> oldVal >> newVal;
    if (!search(root, oldVal)) {
        cout << oldVal << " not found in tree" << endl;
    } else {
        root = deleteNode(root, oldVal);
        root = insert(root, newVal);
        cout << "Tree after update:" << endl;
        printTree(root, 0);
    }
}
        else if (choice == 5) {
            printTree(root, 0);
        }
    } while (choice != 6);
    return 0;
}