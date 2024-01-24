#include <vector>

template <class Comparable>
class HeapSort {
public:
    static void sort(std::vector<Comparable>& a) {
        // Build heap
        for (int i = a.size() / 2 - 1; i >= 0; --i)
            percDown(a, i, a.size());

        // Sort
        for (int j = a.size() - 1; j > 0; --j) {
            std::swap(a[0], a[j]); // swap max to the last position.
            percDown(a, 0, j); // re-form the heap
        }
    }

private:
    static int leftChild(int i) {
        return 2 * i + 1;
    }

    static void percDown(std::vector<Comparable>& a, int i, int n) {
        int child;
        Comparable tmp;

        for (tmp = std::move(a[i]); leftChild(i) < n; i = child) {
            child = leftChild(i);
            if (child != n - 1 && a[child] < a[child + 1])
                ++child;
            if (a[child] > tmp)
                a[i] = std::move(a[child]);
            else
                break;
        }
        a[i] = std::move(tmp);
    }
};
