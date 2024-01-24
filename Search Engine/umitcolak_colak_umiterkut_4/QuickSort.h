#include <vector>
#include<cstdlib>
#include<ctime>
using namespace std;

template <class Comparable>
class QuickSort {
public:
    enum PivotType {
        FIRST_ELEMENT,
        RANDOM_ELEMENT,
        MEDIAN_OF_THREE
    };

    static void quicksort(vector<Comparable>& a, PivotType pivotType = MEDIAN_OF_THREE) {
        quicksort(a, 0, a.size() - 1, pivotType);
    }

private:
    static const Comparable& median3(vector<Comparable>& a, int left, int right) {
        int center = (left + right) / 2;
        if (a[center] < a[left])
            swap(a[left], a[center]);
        if (a[right] < a[left])
            swap(a[left], a[right]);
        if (a[right] < a[center])
            swap(a[center], a[right]);

        swap(a[center], a[right - 1]);
        return a[right - 1];
    }

 
    static void quicksort(vector<Comparable>& vec, int left, int right, PivotType pivotType) {
        if (left < right) {
            Comparable pivot;
            switch (pivotType) {
            case FIRST_ELEMENT:
                pivot = vec[left];
                break;
            case RANDOM_ELEMENT:
                pivot = vec[left + rand() % (right - left + 1)];
                break;
            case MEDIAN_OF_THREE:
            default:
                pivot = median3(vec, left, right);
                break;
            }

            int partitionIndex = partition(vec, left, right, pivot);
            quicksort(vec, left, partitionIndex - 1, pivotType);
            quicksort(vec, partitionIndex + 1, right, pivotType);
        }
    }

    static int partition(vector<Comparable>& vec, int left, int right, const Comparable& pivot) {
        while (left <= right) {
            while (vec[left] < pivot) left++;
            while (vec[right] > pivot) right--;
            if (left <= right) {
                swap(vec[left], vec[right]);
                left++;
                right--;
            }
        }
        return left;
    }

    static void insertionSort(vector<Comparable>& a, int left, int right) {
        for (int p = left + 1; p <= right; ++p) {
            Comparable tmp = a[p];
            int j;
            for (j = p; j > left && tmp < a[j - 1]; --j)
                a[j] = a[j - 1];
            a[j] = tmp;
        }
    }
};

