# C++ unit tests

Simple assert-based tests for your data structures and algorithms.

## Run working tests

```bash
cd cpp_tests
make run
```

## Notes about current header bugs

These tests revealed some issues in the current headers:

1. `Queue.hpp`
   - It declares `template<typename T> struct Stack`, not `Queue`.
   - Constructor is also `Stack() {}`.
   - Non-const accessor is named `top()` instead of `front()`.

2. `HashTable.hpp`
   - `HashTable(Hasher<K> hasher)` receives an object, but the field is `Hasher<K>* hasher`.
   - Use `HashTable(Hasher<K>* hasher)` and pass `&hasher` from tests.
   - `clear()` calls `this->table[i].clear();`, but `AVL::clear()` currently requires a node reference argument.

3. `Algorithms.hpp`
   - `heapSort` uses `arr.size()` but `Vector` has a field `size`, not a function.
   - `heapSort` uses `swap(...)` without `std::swap(...)`.
   - `binarySearch` uses `while (l < r)`, so it can miss the last remaining element. Use `while (l <= r)`.
   - `partition` has `if ((a[high], a[mid]))`, probably should be `if (cmp(a[high], a[mid]))`.
   - `partition` stores `int pivot = a[low];`, so it only works for `T = int`. Use `T pivot = a[low];`.

After fixing Queue/HashTable, run:

```bash
make after_fix
./test_queue_after_fix
./test_hashtable_after_fix
```
