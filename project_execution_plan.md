# 🚀 Kế hoạch Triển khai (Execution Plan): Distributed System Log Aggregator & Analyzer

Chào nhóm! Dưới góc độ Project Manager & Solutions Architect, mình đã thiết kế Kế hoạch triển khai 2 tuần này nhằm tối ưu hóa hiệu suất làm việc song song của hai bạn. Kế hoạch này được chia làm 3 giai đoạn chính, giải quyết triệt để các bài toán khó đã đặt ra.

---

## 📅 Lộ trình Triển khai Tổng quan (2 Tuần)

### 📌 Giai đoạn 1: Foundation & Stubs (Ngày 1 - 4)

**Mục tiêu:** Xây dựng khung xương chương trình. Hai thành viên có thể làm việc hoàn toàn độc lập thông qua giao tiếp bằng **Stub/Mock**.

| Thành viên A (Data Engineer - `lib/`)                                      | Thành viên B (DevOps/SRE - `app/`)                                                  |
| :------------------------------------------------------------------------- | :---------------------------------------------------------------------------------- |
| **CTDL Cơ bản:** Code template cho `LinkedList` (sắp tới dùng làm buffer). | **Log Reader:** Viết class đọc file sử dụng `seekg/tellg` (Incremental Reading).    |
| **Cây Nhị phân:** Cài đặt khung `BSTree` cơ bản làm nền tảng.              | **Event Loop:** Xây dựng vòng lặp chính (Real-time Event Loop) bắt phím không chặn. |
| **Log Struct:** Định nghĩa struct `Log` chuẩn để cả 2 cùng dùng chung.     | **Mock Data:** Dùng mảng string tĩnh để test vòng lặp mà không cần đợi parser.      |

### 📌 Giai đoạn 2: Stateful Analysis & Advanced DSA (Ngày 5 - 10)

**Mục tiêu:** Ráp nối logic cốt lõi. Khả năng phát hiện lỗi và cảnh báo có trạng thái.

| Thành viên A (Data Engineer - `lib/`)                                        | Thành viên B (DevOps/SRE - `app/`)                                          |
| :--------------------------------------------------------------------------- | :-------------------------------------------------------------------------- |
| **AVL Tree:** Code `AVLTree` kế thừa từ `BSTree`.                            | **Log Parser:** Phân tích chuỗi string từ Reader thành struct `Log`.        |
| **Hash Table:** Cài đặt `HashTable` dùng `AVLTree` làm bucket xử lý đụng độ. | **Log Monitor (Stateful):** Tích hợp `HashTable` của A để đếm cộng dồn lỗi. |
| **Trie:** (Bắt đầu) Code `Trie` để chuẩn bị cho chức năng search.            | **UI:** Phân tách logic render `Live Monitor` và bảng `Statistics View`.    |

### 📌 Giai đoạn 3: Tích hợp, Priority Queue & Tuning (Ngày 11 - 14)

**Mục tiêu:** Hoàn thiện tính năng ưu tiên, dọn dẹp bộ nhớ (Memory Leaks) và chuẩn bị Demo.

| Thành viên A (Data Engineer - `lib/`)                           | Thành viên B (DevOps/SRE - `app/`)                                                 |
| :-------------------------------------------------------------- | :--------------------------------------------------------------------------------- |
| **Priority Queue:** Cài đặt cấu trúc Heap quản lý thứ tự `Log`. | **Tích hợp Alert:** Đẩy log `FATAL/CRITICAL` vào Priority Queue của A.             |
| **QuickSort:** Viết hàm sort mảng log theo timestamp.           | **Polishing:** Thêm màu sắc (ANSI codes), Test các case đặc biệt, dọn file/buffer. |

---

## 🛠 Giải pháp Kiến trúc & Code Stubs

Dưới đây là các thiết kế "ăn điểm" và giải quyết các vấn đề kỹ thuật của dự án. Hai bạn có thể sử dụng trực tiếp các đoạn Code Stub này làm khung sườn (Skeleton).

### 1. Incremental Reading & Polling (Dành cho B)

**Vấn đề:** Đọc cả file vài GB sẽ tràn RAM. Dùng `while(getline)` tới hết file là xong, không bắt được log mới sinh ra.
**Giải pháp:** Đọc theo batch, lưu vị trí con trỏ `lastPos`. Nếu hết file, `Sleep` một chút rồi lại trỏ về `lastPos` đọc tiếp.

```cpp
// app/source/LogReader.hpp
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

class LogReader {
private:
    std::ifstream file;
    std::streampos lastPos;

public:
    LogReader(const std::string& filename) {
        file.open(filename);
        lastPos = 0; // Bắt đầu từ đầu file
    }

    // Trả về batch 100 dòng, không làm tràn RAM
    std::vector<std::string> readBatch(int batchSize = 100) {
        std::vector<std::string> batch;
        if (!file.is_open()) return batch;

        file.clear(); // [QUAN TRỌNG] Xóa cờ EOF để cho phép đọc tiếp khi file có dữ liệu mới
        file.seekg(lastPos); // Đặt lại con trỏ ở vị trí lần trước dừng lại

        std::string line;
        int count = 0;
        while (count < batchSize && std::getline(file, line)) {
            batch.push_back(line);
            count++;
        }

        if (batch.empty()) {
            // Đã chạm EOF (cuối file) -> Polling (Thăm dò)
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            // Ghi nhớ vị trí con trỏ cho lần đọc tới
            lastPos = file.tellg();
        }

        return batch;
    }
};
```

### 2. Vòng lặp giao diện không chặn (Real-time Event Loop) (Dành cho B)

**Vấn đề:** Lệnh `std::cin >> ...` sẽ "treo" toàn bộ luồng chương trình, log sẽ ngừng chạy để đợi người dùng nhập phím.
**Giải pháp:** Dùng Non-blocking Keyboard Hit (`_kbhit()` trên Windows hoặc thủ thuật `termios` trên Unix) trong một Vòng lặp sự kiện liên tục có độ trễ cực thấp.

```cpp
// app/main.cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

// --- Helper đa nền tảng cho Non-blocking Input ---
#ifdef _WIN32
  #include <conio.h>
#else
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
  int _kbhit() { /* Tra cứu snippet kbhit cho Linux/Mac */ return 0; }
  int _getch() { return getchar(); }
#endif

// Trạng thái của State Machine
enum class AppState { LIVE_MONITOR, STATISTICS, EXIT };
std::atomic<AppState> currentState(AppState::LIVE_MONITOR);

void eventLoop() {
    // LogReader reader("data/raw_logs.txt");

    while (currentState != AppState::EXIT) {
        // 1. Kiểm tra phím bấm (Không block)
        if (_kbhit()) {
            char key = _getch();
            if (key == '1') currentState = AppState::LIVE_MONITOR;
            else if (key == '2') currentState = AppState::STATISTICS;
            else if (key == 'q' || key == 'Q') currentState = AppState::EXIT;
        }

        // 2. Cập nhật Logic & Render giao diện theo Trạng thái
        // auto batch = reader.readBatch(50); // Luôn đọc log ngầm

        if (currentState == AppState::LIVE_MONITOR) {
            // In thẳng ra màn hình với mã màu ANSI
            // for(auto& log : batch) printColored(log);
        }
        else if (currentState == AppState::STATISTICS) {
            // renderDashboard(); // Vẽ lại màn hình thống kê
        }

        // Tránh chạy full 100% CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    std::cout << "Starting Distributed Log Analyzer...\n";
    eventLoop();
    std::cout << "Graceful Exit. Trình dọn dẹp bộ nhớ đã kích hoạt...\n";
    return 0;
}
```

### 3. Tối ưu hóa AVL Tree Kế thừa & Bucket Hash Table (Dành cho A)

**Vấn đề:** Hash Table hay bị đụng độ (Collision). Nếu dùng Linked List truyền thống làm Bucket thì worst-case sẽ tụt xuống $O(N)$.
**Giải pháp (Ăn điểm tuyệt đối):** Cho `AVLTree` kế thừa mã nguồn từ `BSTree`. Sau đó, Hash Table thay vì dùng mảng List thì dùng mảng `AVLTree`. Thời gian truy tìm lúc đụng độ lớn nhất chỉ là $O(\log K)$.

```cpp
// lib/BSTree.hpp
template<typename K, typename V>
struct Node {
    K key; V value;
    Node* left; Node* right;
    int height; // Chuẩn bị sẵn cho AVL
    Node(K k, V v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template<typename K, typename V>
class BSTree {
protected:
    Node<K, V>* root = nullptr;
    // Đặt là virtual để AVL ghi đè luật Insert
    virtual Node<K, V>* insertNode(Node<K, V>* node, K key, V value) {
        // ... (Cài đặt chèn BST cơ bản)
        return node;
    }
public:
    virtual void insert(K key, V value) { root = insertNode(root, key, value); }
};

// lib/AVLTree.hpp
#include "BSTree.hpp"

template<typename K, typename V>
class AVLTree : public BSTree<K, V> {
protected:
    // Ghi đè hàm Insert để thêm logic tự cân bằng
    Node<K, V>* insertNode(Node<K, V>* node, K key, V value) override {
        // 1. Dùng logic chèn của lớp cha (BST)
        // 2. Cập nhật node->height
        // 3. Tính độ lệch (Balance Factor)
        // 4. Nếu mất cân bằng -> Gọi hàm Xoay (Rotate Left/Right)
        return node;
    }
};

// lib/HashTable.hpp
#include "AVLTree.hpp"
#include <vector>

template<typename K, typename V>
class HashTable {
private:
    std::vector<AVLTree<K, V>> table; // [ĐIỂM CỘNG] Bucket là AVL Tree
    int capacity;
    int hashFunction(K key) { return std::hash<K>{}(key) % capacity; }

public:
    HashTable(int cap) : capacity(cap), table(cap) {}

    void insert(K key, V value) {
        int index = hashFunction(key);
        table[index].insert(key, value); // Nếu đụng độ, AVL Tree tự cân bằng!
    }
};
```

### 4. Giải pháp Giữ trạng thái (Stateful Analysis) (Dành cho cả 2)

**Vấn đề:** 100 lỗi nằm rải rác ở 5 batch đọc khác nhau (mỗi batch 20 lỗi). Nếu chỉ duyệt mảng tĩnh thì không bao giờ biết đã đạt ngưỡng cảnh báo 100 lỗi.
**Giải pháp:** `LogMonitor` sở hữu `HashTable` để lưu tổng trạng thái vượt qua ranh giới của các batch.

```cpp
// app/processing/LogMonitor.hpp
#include "../../lib/HashTable.hpp"
#include <string>
#include <iostream>

class LogMonitor {
private:
    // Key: ServiceID (VD: "Payment_Service"), Value: Total Errors
    HashTable<std::string, int> errorState;
    int THRESHOLD = 100; // Ngưỡng báo động

public:
    LogMonitor() : errorState(101) {}

    void analyzeLog(const std::string& serviceID, const std::string& severity) {
        if (severity == "ERROR" || severity == "FATAL") {
            // 1. Lấy trạng thái lỗi hiện tại của Service này
            int currentErrors = errorState.get(serviceID, 0);
            currentErrors++;

            // 2. Ghi đè trạng thái mới xuyên suốt vòng đời chương trình
            errorState.insert(serviceID, currentErrors);

            // 3. Kích hoạt cảnh báo có trạng thái
            if (currentErrors >= THRESHOLD) {
                std::cout << "\033[1;31m[CRITICAL ALERT]\033[0m " << serviceID
                          << " vượt mốc " << currentErrors << " lỗi!\n";
                // errorState.insert(serviceID, 0); // (Tùy chọn) Đặt lại đếm nếu đã cảnh báo
            }
        }
    }
};
```

Kế hoạch này đảm bảo dự án chạy trơn tru, logic sạch sẽ và đặc biệt là ăn trọn điểm kỹ thuật với thầy cô ở môn DSA. Các bạn hãy tham khảo code stub và bắt tay vào Giai đoạn 1 nhé!
