/**
 * File: 552001_Task2.cpp
 * Description: Implements a live Hospital ER Wait-Time monitor using a dual-heap
 *              (Max-Heap and Min-Heap) approach to achieve O(1) median lookup
 *              and O(log n) update costs per arrival.
 */

#include <iostream>
#include <iomanip>

using namespace std;

class DualHeapMedian {
private:
    int* maxHeap; // Stores the smaller half of values
    int* minHeap; // Stores the larger half of values
    int maxCapacity, minCapacity;
    int maxSize, minSize;

    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    void maxHeapSiftUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (maxHeap[idx] > maxHeap[parent]) {
                swap(maxHeap[idx], maxHeap[parent]);
                idx = parent;
            }
            else {
                break;
            }
        }
    }

    void maxHeapSiftDown(int idx) {
        while (2 * idx + 1 < maxSize) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int largest = idx;

            if (maxHeap[left] > maxHeap[largest]) largest = left;
            if (right < maxSize && maxHeap[right] > maxHeap[largest]) largest = right;

            if (largest != idx) {
                swap(maxHeap[idx], maxHeap[largest]);
                idx = largest;
            }
            else {
                break;
            }
        }
    }

    void minHeapSiftUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (minHeap[idx] < minHeap[parent]) {
                swap(minHeap[idx], minHeap[parent]);
                idx = parent;
            }
            else {
                break;
            }
        }
    }

    void minHeapSiftDown(int idx) {
        while (2 * idx + 1 < minSize) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;

            if (minHeap[left] < minHeap[smallest]) smallest = left;
            if (right < minSize && minHeap[right] < minHeap[smallest]) smallest = right;

            if (smallest != idx) {
                swap(minHeap[idx], minHeap[smallest]);
                idx = smallest;
            }
            else {
                break;
            }
        }
    }

public:
    // Constructor: Time Complexity O(1)
    DualHeapMedian(int cap) {
        maxCapacity = cap;
        minCapacity = cap;
        maxSize = 0;
        minSize = 0;
        maxHeap = new int[maxCapacity];
        minHeap = new int[minCapacity];
    }

    // Destructor: Time Complexity O(1)
    ~DualHeapMedian() {
        delete[] maxHeap;
        delete[] minHeap;
    }

    /**
     * Inserts a new wait time into the appropriate heap and rebalances.
     * Time Complexity: O(log n)
     */
    void addWait(int x) {
        // 1. Insert into correct heap
        if (maxSize == 0 || x <= maxHeap[0]) {
            maxHeap[maxSize++] = x;
            maxHeapSiftUp(maxSize - 1);
        }
        else {
            minHeap[minSize++] = x;
            minHeapSiftUp(minSize - 1);
        }

        // 2. Rebalance heaps so sizes differ by at most 1, with maxHeap holding the extra element
        if (maxSize > minSize + 1) {
            // Move root of maxHeap to minHeap
            int rootVal = maxHeap[0];
            maxHeap[0] = maxHeap[maxSize - 1];
            maxSize--;
            maxHeapSiftDown(0);

            minHeap[minSize++] = rootVal;
            minHeapSiftUp(minSize - 1);
        }
        else if (minSize > maxSize) {
            // Move root of minHeap to maxHeap
            int rootVal = minHeap[0];
            minHeap[0] = minHeap[minSize - 1];
            minSize--;
            minHeapSiftDown(0);

            maxHeap[maxSize++] = rootVal;
            maxHeapSiftUp(maxSize - 1);
        }
    }

    /**
     * Returns the median wait time.
     * Time Complexity: O(1)
     */
    double getMedian() const {
        if (maxSize == 0 && minSize == 0) return 0;
        if (maxSize > minSize) {
            return (double)maxHeap[0];
        }
        else {
            return (double)(maxHeap[0] + minHeap[0]) / 2.0;
        }
    }

    // Accessors for roots
    bool hasMaxRoot() const { return maxSize > 0; }
    int getMaxRoot() const { return maxHeap[0]; }

    bool hasMinRoot() const { return minSize > 0; }
    int getMinRoot() const { return minHeap[0]; }
};

int main() {
    int input[] = { 45, 20, 60, 15, 30, 90, 10, 55 };
    int n = 8;
    int threshold = 35;

    DualHeapMedian monitor(10);

    cout << left << setw(8) << "Arrival"
        << setw(8) << "Wait"
        << setw(16) << "Max heap root"
        << setw(16) << "Min heap root"
        << setw(10) << "Median"
        << "Status" << "\n";
    cout << "-----------------------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
        int wait = input[i];
        monitor.addWait(wait);
        double median = monitor.getMedian();

        string maxRootStr = monitor.hasMaxRoot() ? to_string(monitor.getMaxRoot()) : "none";
        string minRootStr = monitor.hasMinRoot() ? to_string(monitor.getMinRoot()) : "none";
        string status = (median > threshold) ? "ALERT" : "Normal";

        cout << left << setw(8) << (i + 1)
            << setw(8) << wait
            << setw(16) << maxRootStr
            << setw(16) << minRootStr
            << setw(10) << (median == (int)median ? to_string((int)median) : to_string(median))
            << status << "\n";
    }

    return 0;
}

/*
 * --- ANSWERS TO SUBMISSION QUESTIONS ---
 *
 * 1. Why is the median a better alert measure here than the average? Use arrival 6 in your answer.
 *    Answer: At arrival 6, the wait times are {15, 20, 30, 45, 60, 90}. The extreme outlier of 90 minutes
 *    pulls the mathematical average way up, whereas the median (37.5) accurately reflects the central
 *    tendency of the group. If an average were used, single extreme cases could cause false or skewed
 *    alarms when most patients are experiencing standard wait times.
 *
 * 2. What goes wrong if the two heap sizes differ by two or more?
 *    Answer: If the sizes differ by two or more, the roots no longer accurately represent the middle
 *    two elements (or the central element) of the dataset. As a result, O(1) median lookup breaks
 *    down because the roots will be skewed toward one side of the distribution.
 *
 * 3. How many total operations does repeated sorting need compared with your heap method for n patients?
 *    Answer:
 *    - Re-sorting the array after every new arrival using an O(n log n) sorting algorithm results in
 *      a total time complexity of O(n^2 log n) for n patients.
 *    - Using the dual-heap method, each arrival requires an insertion and potential rebalancing, which
 *      runs in O(log n) per arrival, resulting in a total time complexity of O(n log n) for n patients,
 *      making it drastically faster for live streaming data systems.
 */