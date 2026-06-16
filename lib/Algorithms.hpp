/**
 * @file Algorithms.hpp
 * @brief Tập hợp các thuật toán sắp xếp và tìm kiếm cơ bản.
 */

#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Vector.hpp"

/**
 * @brief Sắp xếp các phần tử sử dụng thuật toán Nổi bọt (Bubble Sort).
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử cần sắp xếp.
 * @param cmp Hàm so sánh tùy chỉnh (mặc định là std::less).
 */
template<typename T, typename Comp = std::less<T>>
void bubbleSort(Vector<T>& arr, Comp cmp = std::less<T>()) {
    int n = arr.getSize();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (cmp(arr[j + 1], arr[j])) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

/**
 * @brief Sắp xếp các phần tử sử dụng thuật toán Sắp xếp Chọn (Selection Sort).
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử cần sắp xếp.
 * @param cmp Hàm so sánh tùy chỉnh (mặc định là std::less).
 */
template<typename T, typename Comp = std::less<T>>
void selectionSort(Vector<T>& arr, Comp cmp = std::less<T>()) {
    int n = arr.getSize();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (cmp(arr[j], arr[minIdx])) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            T temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
        }
    }
}

/**
 * @brief Sắp xếp các phần tử sử dụng thuật toán Sắp xếp Chèn (Insertion Sort).
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử cần sắp xếp.
 * @param cmp Hàm so sánh tùy chỉnh (mặc định là std::less).
 */
template<typename T, typename Comp = std::less<T>>
void insertionSort(Vector<T>& arr, Comp cmp = std::less<T>()) {
    int n = arr.getSize();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/**
 * @brief Hàm hỗ trợ cho heapSort để duy trì tính chất của Heap.
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử.
 * @param n Kích thước của heap.
 * @param i Chỉ số của gốc (root) hiện tại.
 * @param cmp Hàm so sánh.
 */
template<typename T, typename Comp>
void heapify(Vector<T>& arr, int n, int i, Comp cmp) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && !cmp(arr[left], arr[largest])) {
        largest = left;
    }
    if (right < n && !cmp(arr[right], arr[largest])) {
        largest = right;
    }
    if (largest != i) {
        T temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest, cmp);
    }
}

/**
 * @brief Sắp xếp các phần tử sử dụng thuật toán Heap Sort.
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử cần sắp xếp.
 * @param cmp Hàm so sánh tùy chỉnh (mặc định là std::less).
 */
template<typename T, typename Comp = std::less<T>>
void heapSort(Vector<T>& arr, Comp cmp = std::less<T>()) {
    int n = arr.getSize();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, cmp);
    }
    for (int i = n - 1; i > 0; i--) {
        T temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0, cmp);
    }
}

/**
 * @brief Hàm hỗ trợ cho quickSort để phân hoạch mảng.
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử.
 * @param lo Chỉ số bắt đầu.
 * @param hi Chỉ số kết thúc.
 * @param cmp Hàm so sánh.
 * @return int Chỉ số của phần tử chốt (pivot) sau khi phân hoạch.
 */
template<typename T, typename Comp>
int partition(Vector<T>& arr, int lo, int hi, Comp cmp) {
    T pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (cmp(arr[j], pivot) || (!cmp(arr[j], pivot) && !cmp(pivot, arr[j]))) {
            i++;
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    T temp = arr[i + 1];
    arr[i + 1] = arr[hi];
    arr[hi] = temp;
    return i + 1;
}

/**
 * @brief Sắp xếp các phần tử sử dụng thuật toán Quick Sort.
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử cần sắp xếp.
 * @param lo Chỉ số bắt đầu.
 * @param hi Chỉ số kết thúc.
 * @param cmp Hàm so sánh.
 */
template<typename T, typename Comp = std::less<T>>
void quickSort(Vector<T>& arr, int lo, int hi, Comp cmp = std::less<T>()) {
    if (lo < hi) {
        int pi = partition(arr, lo, hi, cmp);
        quickSort(arr, lo, pi - 1, cmp);
        quickSort(arr, pi + 1, hi, cmp);
    }
}

/**
 * @brief Hàm hỗ trợ cho mergeSort để trộn hai nửa mảng đã sắp xếp.
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử.
 * @param lo Chỉ số bắt đầu nửa trái.
 * @param mid Chỉ số kết thúc nửa trái (và giữa).
 * @param hi Chỉ số kết thúc nửa phải.
 * @param cmp Hàm so sánh.
 */
template<typename T, typename Comp>
void merge(Vector<T>& arr, int lo, int mid, int hi, Comp cmp) {
    int n1 = mid - lo + 1;
    int n2 = hi - mid;

    Vector<T> left(n1, T());
    Vector<T> right(n2, T());

    for (int i = 0; i < n1; i++)
        left[i] = arr[lo + i];
    for (int j = 0; j < n2; j++)
        right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = lo;
    while (i < n1 && j < n2) {
        if (cmp(left[i], right[j]) || (!cmp(left[i], right[j]) && !cmp(right[j], left[i]))) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

/**
 * @brief Sắp xếp các phần tử sử dụng thuật toán Merge Sort.
 * 
 * @tparam T Kiểu dữ liệu của các phần tử.
 * @tparam Comp Kiểu của hàm so sánh.
 * @param arr Danh sách các phần tử cần sắp xếp.
 * @param lo Chỉ số bắt đầu.
 * @param hi Chỉ số kết thúc.
 * @param cmp Hàm so sánh tùy chỉnh.
 */
template<typename T, typename Comp = std::less<T>>
void mergeSort(Vector<T>& arr, int lo, int hi, Comp cmp = std::less<T>()) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mergeSort(arr, lo, mid, cmp);
        mergeSort(arr, mid + 1, hi, cmp);
        merge(arr, lo, mid, hi, cmp);
    }
}

/**
 * @brief Tìm kiếm tuần tự (Linear Search) một giá trị trong mảng.
 * 
 * @tparam T Kiểu dữ liệu của phần tử.
 * @param arr Danh sách các phần tử.
 * @param key Giá trị cần tìm kiếm.
 * @return int Chỉ số của phần tử nếu tìm thấy, hoặc -1 nếu không tìm thấy.
 */
template<typename T>
int linearSearch(const Vector<T>& arr, const T& key) {
    for (int i = 0; i < arr.getSize(); i++) {
        if (arr[i] == key)
            return i;
    }
    return -1;
}

/**
 * @brief Tìm kiếm nhị phân (Binary Search) một giá trị trong mảng đã sắp xếp.
 * 
 * @tparam T Kiểu dữ liệu của phần tử.
 * @param arr Danh sách các phần tử đã được sắp xếp.
 * @param key Giá trị cần tìm kiếm.
 * @return int Chỉ số của phần tử nếu tìm thấy, hoặc -1 nếu không tìm thấy.
 */
template<typename T>
int binarySearch(const Vector<T>& arr, const T& key) {
    int lo = 0;
    int hi = arr.getSize() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == key)
            return mid;
        if (arr[mid] < key) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

#endif  // ALGORITHMS_HPP
