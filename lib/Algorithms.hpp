#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Vector.hpp"

template<typename T>
int linearSearch(const Vector<T>& arr, const T& key) {
    int n = arr.size;
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
}

template<typename T>
int binarySearch(const Vector<T>& arr, const T& key) {
    int n = arr.size;
    int l = 0;
    int r = n - 1;
    while (l < r) {
        int m = l + (r - l) / 2;
        if (arr[m] == key) {
            return m;
        } else if (arr[m] < key) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return -1;
}

template<typename T, typename Comp = std::less<T>>
void selectionSort(Vector<T>& arr, Comp cmp = Comp()) {
    int n = arr.size;
    for (int i = 0; i < n; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (cmp(arr[j], arr[minIdx])) {  // arr[j] < arr[minIdx]
                minIdx = j;
            }
        }
        std::swap(arr[i], arr[minIdx]);
    }
}

template<typename T, typename Comp = std::less<T>>
void bubbleSort(Vector<T>& arr, Comp cmp = Comp()) {
    int n = arr.size;
    for (int i = 0; i < n; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (cmp(arr[j + 1], arr[j])) {
                swapped = true;
                std::swap(arr[j], arr[j + 1]);
            }
        }
        if (!swapped) {
            break;
        }
    }
}

template<typename T, typename Comp = std::less<T>>
void insertionSort(Vector<T>& arr, Comp cmp = Comp()) {
    int n = arr.size;
    for (int i = 1; i < n; i++) {
        int j = i;
        T val = arr[i];
        while (j > 0 && cmp(val, arr[j - 1])) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = val;
    }
}

template<typename T, typename Comp = std::less<T>>
int partition(Vector<T>& a, int low, int high, Comp cmp = Comp()) {
    int mid = low + (high - low) / 2;

    if (cmp(a[mid], a[low]))
        std::swap(a[low], a[mid]);
    if (cmp(a[high], a[low]))
        std::swap(a[low], a[high]);
    if (cmp(a[high], a[mid]))
        std::swap(a[mid], a[high]);

    std::swap(a[mid], a[low]);
    int pivot = a[low];
    int leftIdx = low + 1;
    for (int i = low + 1; i <= high; i++) {
        if (cmp(a[i], pivot)) {
            std::swap(a[i], a[leftIdx++]);
        }
    }
    std::swap(a[low], a[leftIdx - 1]);
    return leftIdx - 1;
}

template<typename T, typename Comp = std::less<T>>
void merge(Vector<T>& arr, int l, int m, int r, Comp cmp = Comp()) {
    if (l >= r) {
        return;
    }
    int i = l;
    int j = m + 1;
    Vector<T> temp(r - l + 1, T());
    int currIdx = 0;
    while (i <= m && j <= r) {
        if (!cmp(arr[j], arr[i])) {  // harr[i] <= arr[j])
            temp[currIdx++] = arr[i++];
        } else {
            temp[currIdx++] = arr[j++];
        }
    }
    while (i <= m) {
        temp[currIdx++] = arr[i++];
    }
    while (j <= r) {
        temp[currIdx++] = arr[j++];
    }
    for (int i = l; i <= r; i++) {
        arr[i] = temp[i - l];
    }
}

template<typename T, typename Comp = std::less<T>>
void heapDown(Vector<T>& arr, int i, int n, Comp cmp = Comp()) {
    while (2 * i + 1 < n) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int maxIdx = left;
        // 2 children
        if (right < n) {
            maxIdx = cmp(arr[left], arr[right]) ? right : left;
        } else {
            maxIdx = left;
        }
        if (cmp(arr[i], arr[maxIdx])) {
            std::swap(arr[maxIdx], arr[i]);
            i = maxIdx;
        } else {
            break;
        }
    }
}

template<typename T, typename Comp = std::less<T>>
void heapSort(Vector<T>& arr, Comp cmp = Comp()) {
    int n = arr.size;
    for (int i = (n - 1) / 2; i >= 0; i--) {
        heapDown(arr, i, n, cmp);
    }

    for (int i = 0; i < n - 1; i++) {
        swap(arr[0], arr[n - i - 1]);
        heapDown(arr, 0, n - i - 1, cmp);
    }
}

template<typename T, typename Comp = std::less<T>>
void mergeSort(Vector<T>& arr, int lo, int hi, Comp cmp = Comp()) {
    if (lo >= hi) {
        return;
    }
    int m = lo + (hi - lo) / 2;
    mergeSort(arr, lo, m, cmp);
    mergeSort(arr, m + 1, hi, cmp);
    merge(arr, lo, m, hi, cmp);
}

template<typename T, typename Comp = std::less<T>>
void quickSort(Vector<T>& arr, int lo, int hi, Comp cmp = Comp()) {
    if (lo >= hi) {
        return;
    }

    int mid = partition(arr, lo, hi, cmp);
    quickSort(arr, mid + 1, hi, cmp);
    quickSort(arr, lo, mid - 1, cmp);
}

#endif  // ALGORITHMS_HPP
