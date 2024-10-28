using namespace std;
#include <iostream>
#include <vector>
#include <future>
#include <algorithm>

// Функция для слияния двух отсортированных частей массива
void merge(std::vector<int>& arr, int start, int mid, int end) {
    std::vector<int> merged;
    int i = start, j = mid;
    while (i < mid && j < end) {
        if (arr[i] < arr[j]) {
            merged.push_back(arr[i++]);
        }
        else {
            merged.push_back(arr[j++]);
        }
    }

    while (i < mid) merged.push_back(arr[i++]);
    while (j < end) merged.push_back(arr[j++]);

    std::copy(merged.begin(), merged.end(), arr.begin() + start);
}

// Функция для многопоточной сортировки слиянием
void parallel_merge_sort(std::vector<int>& arr, int start, int end) {
    if (end - start <= 1) {
        return; // Базовый случай: массив длиной 1 уже отсортирован
    }

    int mid = start + (end - start) / 2;

    // Создаём асинхронные задачи для сортировки левой и правой половин
    auto left_task = std::async(std::launch::async, [&arr, start, mid]() {
        parallel_merge_sort(arr, start, mid);
        });

    auto right_task = std::async(std::launch::async, [&arr, mid, end]() {
        parallel_merge_sort(arr, mid, end);
        });

    // Ждём завершения обеих задач
    left_task.wait();
    right_task.wait();

    // Слияние отсортированных половин
    merge(arr, start, mid, end);
}

int main() {
    std::vector<int> arr = { 38, 27, 43, 3, 9, 82, 10 };
    parallel_merge_sort(arr, 0, arr.size());

    std::cout << "Отсортированный массив: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
