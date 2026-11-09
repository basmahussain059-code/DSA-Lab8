#include <iostream>
#include <vector>
using namespace std;
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};
//Binary Min Heap 
class MinHeap {
    vector<ListNode*> h;

    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (h[i]->val < h[p]->val) {
                swap(h[i], h[p]);
                i = p;
            } else break;
        }
    }
    void siftDown(int i) {
        int n = h.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, s = i;
            if (l < n && h[l]->val < h[s]->val) s = l;
            if (r < n && h[r]->val < h[s]->val) s = r;
            if (s == i) break;
            swap(h[i], h[s]);
            i = s;
        }
    }
public:
    bool empty() { return h.empty(); }
    void push(ListNode* x) {
        h.push_back(x);
        siftUp(h.size() - 1);
    }
    ListNode* pop() {
        ListNode* top = h[0];
        h[0] = h.back();
        h.pop_back();
        if (!h.empty()) siftDown(0);
        return top;
    }
};
ListNode* mergeKLists(vector<ListNode*>& lists) {
    MinHeap heap;
    for (ListNode* head : lists)
        if (head) heap.push(head);
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!heap.empty()) {
        ListNode* smallest = heap.pop();   // smallest value among all current heads
        tail->next = smallest;
        tail = smallest;
        if (smallest->next)                // push the next node of that same list
            heap.push(smallest->next);
    }
    return dummy.next;
}
int main() {
    int k;
    cout << "Enter number of lists (k): ";
    cin >> k;
    vector<ListNode*> lists(k, nullptr);
    for (int i = 0; i < k; i++) {
        int n;
        cout << "List " << i + 1 << " - enter size, then sorted elements: ";
        cin >> n;
        ListNode* tail = nullptr;
        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;
            ListNode* node = new ListNode(x);
            if (!lists[i]) lists[i] = tail = node;
            else { tail->next = node; tail = node; }
        }
    }
    ListNode* merged = mergeKLists(lists);
    cout << "\nMerged list: ";
    while (merged) {
        cout << merged->val << " ";
        ListNode* temp = merged;
        merged = merged->next;
        delete temp;
    }
    cout << endl;
    return 0;
}