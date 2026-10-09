// ============================================================
//  Algorithms.h - Sorting algorithms implemented from scratch
//  Quick Sort  : used for sorting cars by rent per day
//  Merge Sort  : used for sorting cars by manufacturing year
// ============================================================
#pragma once
#include <vector>
#include <functional>
using namespace std;

// ---------- QUICK SORT ----------
template <typename T, typename Comp>
int partitionQS(vector<T>& a, int lo, int hi, Comp comp) {
    T pivot = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (comp(a[j], pivot)) {          // a[j] < pivot
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[hi]);
    return i + 1;
}

template <typename T, typename Comp>
void quickSort(vector<T>& a, int lo, int hi, Comp comp) {
    if (lo < hi) {
        int p = partitionQS(a, lo, hi, comp);
        quickSort(a, lo, p - 1, comp);
        quickSort(a, p + 1, hi, comp);
    }
}

// ---------- MERGE SORT ----------
template <typename T, typename Comp>
void mergeMS(vector<T>& a, int l, int m, int r, Comp comp) {
    int n1 = m - l + 1, n2 = r - m;
    vector<T> L(a.begin() + l, a.begin() + l + n1);
    vector<T> R(a.begin() + m + 1, a.begin() + m + 1 + n2);
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j])) a[k++] = L[i++];
        else                 a[k++] = R[j++];
    }
    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];
}

template <typename T, typename Comp>
void mergeSort(vector<T>& a, int l, int r, Comp comp) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(a, l, m, comp);
        mergeSort(a, m + 1, r, comp);
        mergeMS(a, l, m, r, comp);
    }
}
