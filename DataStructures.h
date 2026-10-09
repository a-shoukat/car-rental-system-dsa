// ============================================================
//  DataStructures.h - Custom implementations for DSA project
//  Contains: LinkedList, Binary Search Tree, Stack, Queue,
//            HashMap (chaining) - all written from scratch.
// ============================================================
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
using namespace std;

// ==================== SINGLY LINKED LIST ====================
template <typename T>
struct ListNode {
    T data;
    ListNode<T>* next;
    ListNode(const T& d) : data(d), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    int sz;

public:
    LinkedList() : head(nullptr), sz(0) {}
    ~LinkedList() { clear(); }

    // Insert at the end - O(n)
    void pushBack(const T& val) {
        ListNode<T>* n = new ListNode<T>(val);
        if (!head) head = n;
        else {
            ListNode<T>* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = n;
        }
        sz++;
    }

    // Remove first node matching predicate - O(n)
    bool removeIf(function<bool(const T&)> pred) {
        ListNode<T>* cur = head;
        ListNode<T>* prev = nullptr;
        while (cur) {
            if (pred(cur->data)) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
                sz--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // Find first node matching predicate - O(n)
    T* findIf(function<bool(const T&)> pred) {
        ListNode<T>* cur = head;
        while (cur) {
            if (pred(cur->data)) return &(cur->data);
            cur = cur->next;
        }
        return nullptr;
    }

    void toVector(vector<T>& out) const {
        ListNode<T>* cur = head;
        while (cur) { out.push_back(cur->data); cur = cur->next; }
    }

    void clear() {
        while (head) {
            ListNode<T>* t = head;
            head = head->next;
            delete t;
        }
        sz = 0;
    }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }
};

// ==================== BINARY SEARCH TREE ====================
// Used as a fast index: license plate -> car id  (O(log n) search)
template <typename K, typename V>
struct BSTNode {
    K key;
    V value;
    BSTNode *left, *right;
    BSTNode(K k, V v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <typename K, typename V>
class BST {
private:
    BSTNode<K, V>* root;

    BSTNode<K, V>* insertRec(BSTNode<K, V>* n, K k, V v) {
        if (!n) return new BSTNode<K, V>(k, v);
        if (k < n->key) n->left = insertRec(n->left, k, v);
        else if (k > n->key) n->right = insertRec(n->right, k, v);
        else n->value = v;               // update existing key
        return n;
    }

    BSTNode<K, V>* searchRec(BSTNode<K, V>* n, K k) {
        if (!n || n->key == k) return n;
        return (k < n->key) ? searchRec(n->left, k) : searchRec(n->right, k);
    }

    BSTNode<K, V>* minNode(BSTNode<K, V>* n) {
        while (n && n->left) n = n->left;
        return n;
    }

    BSTNode<K, V>* removeRec(BSTNode<K, V>* n, K k, bool& done) {
        if (!n) return nullptr;
        if (k < n->key) n->left = removeRec(n->left, k, done);
        else if (k > n->key) n->right = removeRec(n->right, k, done);
        else {
            done = true;
            if (!n->left) { BSTNode<K, V>* r = n->right; delete n; return r; }
            if (!n->right) { BSTNode<K, V>* l = n->left; delete n; return l; }
            BSTNode<K, V>* s = minNode(n->right);
            n->key = s->key; n->value = s->value;
            bool dummy = false;
            n->right = removeRec(n->right, s->key, dummy);
        }
        return n;
    }

    void destroy(BSTNode<K, V>* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(K key, V value) { root = insertRec(root, key, value); }

    // Returns pointer to value, or nullptr if not found
    V* search(K key) {
        BSTNode<K, V>* n = searchRec(root, key);
        return n ? &(n->value) : nullptr;
    }

    bool remove(K key) {
        bool done = false;
        root = removeRec(root, key, done);
        return done;
    }

    bool empty() const { return root == nullptr; }
};

// ==================== STACK (array-based) ====================
// Used for UNDO: every rental is pushed, undo pops it back.
template <typename T>
class Stack {
private:
    vector<T> data;

public:
    void push(const T& val) { data.push_back(val); }
    bool pop(T& out) {
        if (data.empty()) return false;
        out = data.back();
        data.pop_back();
        return true;
    }
    bool empty() const { return data.empty(); }
    int size() const { return (int)data.size(); }
};

// ==================== QUEUE (circular buffer) ====================
// Used for pending rental REQUESTS: first-come, first-served.
template <typename T>
class Queue {
private:
    vector<T> buf;
    int front, rear, count, cap;

    void grow() {
        int ncap = cap * 2;
        vector<T> nb(ncap);
        for (int i = 0; i < count; i++)
            nb[i] = buf[(front + i) % cap];
        buf = nb;
        front = 0; rear = count; cap = ncap;
    }

public:
    Queue(int capacity = 16) : buf(capacity), front(0), rear(0), count(0), cap(capacity) {}

    void enqueue(const T& val) {
        if (count == cap) grow();
        buf[rear] = val;
        rear = (rear + 1) % cap;
        count++;
    }

    bool dequeue(T& out) {
        if (count == 0) return false;
        out = buf[front];
        front = (front + 1) % cap;
        count--;
        return true;
    }

    bool empty() const { return count == 0; }
    int size() const { return count; }
};

// ==================== HASH MAP (separate chaining) ====================
// Used for O(1) customer lookup by phone number.
template <typename V>
class HashMap {
private:
    vector<vector<pair<string, V>>> buckets;
    int numBuckets;

    // djb2 string hash
    size_t hashOf(const string& key) const {
        size_t h = 5381;
        for (char c : key) h = ((h << 5) + h) + (unsigned char)c;
        return h % numBuckets;
    }

public:
    HashMap(int buckets_ = 53) : numBuckets(buckets_) {
        buckets.resize(numBuckets);
    }

    void put(const string& key, const V& value) {
        size_t i = hashOf(key);
        for (auto& p : buckets[i]) {
            if (p.first == key) { p.second = value; return; }
        }
        buckets[i].push_back({key, value});
    }

    V* get(const string& key) {
        size_t i = hashOf(key);
        for (auto& p : buckets[i]) {
            if (p.first == key) return &(p.second);
        }
        return nullptr;
    }

    bool remove(const string& key) {
        size_t i = hashOf(key);
        for (size_t j = 0; j < buckets[i].size(); j++) {
            if (buckets[i][j].first == key) {
                buckets[i].erase(buckets[i].begin() + j);
                return true;
            }
        }
        return false;
    }
};
