#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Node {
    char ch;
    int freq;
    int id;              
    Node *left, *right;
    Node(char c, int f, int i) : ch(c), freq(f), id(i), left(nullptr), right(nullptr) {}
};
class MinHeap {
    vector<Node*> h;
    bool less(Node* a, Node* b) {
        if (a->freq != b->freq) return a->freq < b->freq;
        return a->id < b->id;
    }
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (less(h[i], h[p])) { swap(h[i], h[p]); i = p; }
            else break;
        }
    }
    void siftDown(int i) {
        int n = h.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, s = i;
            if (l < n && less(h[l], h[s])) s = l;
            if (r < n && less(h[r], h[s])) s = r;
            if (s == i) break;
            swap(h[i], h[s]);
            i = s;
        }
    }
public:
    int size() { return h.size(); }
    void push(Node* x) { h.push_back(x); siftUp(h.size() - 1); }
    Node* pop() {
        Node* top = h[0];
        h[0] = h.back();
        h.pop_back();
        if (!h.empty()) siftDown(0);
        return top;
    }
};
void preorder(Node* root, string code) {
    if (!root) return;
    if (!root->left && !root->right) {      // leaf
        cout << root->ch << ": " << code << "\n";
        return;
    }
    preorder(root->left,  code + "0");
    preorder(root->right, code + "1");
}
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
Node* buildHuffmanTree(const string& S, const vector<int>& f) {
    MinHeap heap;
    int id = 0;
    for (size_t i = 0; i < S.size(); i++)
        heap.push(new Node(S[i], f[i], id++));
   while (heap.size() > 1) {
        Node* a = heap.pop();               
        Node* b = heap.pop();               
        Node* parent = new Node('$', a->freq + b->freq, id++);
        parent->left = a;
        parent->right = b;
        heap.push(parent);
    }
    return heap.pop();
}
int main() {
    int N;
    cout << "Enter N: ";
    cin >> N;
    string S;
    cout << "Enter characters (no spaces): ";
    cin >> S;
    vector<int> f(N);
    cout << "Enter frequencies: ";
    for (int i = 0; i < N; i++) cin >> f[i];
    Node* root = buildHuffmanTree(S, f);
    cout << "\nHuffman codes (preorder):\n";
    if (N == 1) cout << root->ch << ": 0\n";  
    else preorder(root, "");
    freeTree(root);
    return 0;
}