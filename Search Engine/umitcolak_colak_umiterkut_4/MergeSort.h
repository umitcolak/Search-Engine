#include <vector>
#include <algorithm> // For std::copy

template <class Comparable>
class MergeSort {
public:
    static void sort(std::vector<Comparable>& a) {
        std::vector<Comparable> tmpArray(a.size());
        mergeSort(a, tmpArray, 0, a.size() - 1);
    }

private:
    static void mergeSort(std::vector<Comparable>& a,
        std::vector<Comparable>& tmpArray, int left, int right) {
        if (left < right) {
            int center = (left + right) / 2;
            mergeSort(a, tmpArray, left, center);
            mergeSort(a, tmpArray, center + 1, right);
            merge(a, tmpArray, left, center + 1, right);
        }
    }

    static void merge(std::vector<Comparable>& a,
        std::vector<Comparable>& tmpArray, int leftPos, int rightPos, int rightEnd) {
        int leftEnd = rightPos - 1;
        int tmpPos = leftPos;
        int numElements = rightEnd - leftPos + 1;

        // Main loop
        while (leftPos <= leftEnd && rightPos <= rightEnd) {
            if (a[leftPos] <= a[rightPos])
                tmpArray[tmpPos++] = std::move(a[leftPos++]);
            else
                tmpArray[tmpPos++] = std::move(a[rightPos++]);
        }

        while (leftPos <= leftEnd) // Copy rest of first half
            tmpArray[tmpPos++] = std::move(a[leftPos++]);

        while (rightPos <= rightEnd) // Copy rest of right half
            tmpArray[tmpPos++] = std::move(a[rightPos++]);

        // Copy tmpArray back
        for (int i = 0; i < numElements; i++, rightEnd--)
            a[rightEnd] = std::move(tmpArray[rightEnd]);
    }
};
