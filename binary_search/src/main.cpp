#include "iostream"

int rec_helper(const std::vector<int> &arr, int target, int left, int right)
{
    if (left > right)
        return -1; // base case: target not found
    // defensive mid calculation to prevent overfow
    int mid = left + (right - left) / 2;

    if (arr[mid] == target)
        return mid;
    if (arr[mid] < target)
        return rec_helper(arr, target, mid + 1, right);
    if (arr[mid] > target)
        return rec_helper(arr, target, left, mid - 1);

    return -1;
}

int binarySearch(const std::vector<int> &arr, int target)
{
    // returns index of exact match of target in arr
    return rec_helper(arr, target, 0, arr.size() - 1);
}

int lowerBound(const std::vector<int> &arr, int target, int left, int right)
{
    if (left > right)
        return left;

    int mid = left + (right - left) / 2;

    if (arr[mid] >= target)
        return lowerBound(arr, target, left, mid - 1);
    else
        return lowerBound(arr, target, mid + 1, right);
}

int binarySearchLowerBound(const std::vector<int> &arr, int target)
{
    // returns index of where target can be put into arr, such that arr remains sorted, used for LIS nlogn
    return lowerBound(arr, target, 0, arr.size() - 1);
}

int main()
{
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};

    for (int target : {3, 6, 11, 12, -1, 0, 1})
    {
        int result = binarySearch(arr, target);
        std::cout << "Search for " << target << ": ";
        if (result != -1)
            std::cout << "Found at index " << result << "\n";
        else
            std::cout << "Not found\n";
    }

    return 0;
}