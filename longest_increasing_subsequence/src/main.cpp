#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <chrono>
#include <algorithm>

// === Utility ===
void printVector(const std::vector<int> &vec)
{
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i];
        if (i + 1 < vec.size())
            std::cout << ", ";
    }
    std::cout << "]\n";
}

// === Binary Search Helper ===
int binarySearchLowerBound(const std::vector<int> &arr, int target)
{
    int left = 0;
    int right = (int)arr.size() - 1;
    int result = (int)arr.size(); // default to end

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] >= target)
        {
            result = mid;
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    return result;
}

// === LIS Recursive ===
int recursiveHelper(const std::vector<int> &arr, int index, int prevIndex)
{
    if (index == arr.size())
        return 0;

    int lengthWithout = recursiveHelper(arr, index + 1, prevIndex);

    int lengthWith = 0;
    if (prevIndex == -1 || arr[index] > arr[prevIndex])
        lengthWith = 1 + recursiveHelper(arr, index + 1, index);

    return std::max(lengthWith, lengthWithout);
}

int LISrecursive(const std::vector<int> &arr)
{
    return recursiveHelper(arr, 0, -1);
}

// === LIS Quadratic ===
int LISquadratic(const std::vector<int> &arr)
{
    int n = arr.size();
    if (n == 0)
        return 0;

    std::vector<int> dp(n, 1);
    int maxLen = 1;

    for (int i = 1; i < n; ++i)
        for (int j = 0; j < i; ++j)
            if (arr[i] > arr[j])
                dp[i] = std::max(dp[i], dp[j] + 1);

    for (int x : dp)
        maxLen = std::max(maxLen, x);

    return maxLen;
}

// === LIS NlogN (Length Only) ===
int LISnlogn(const std::vector<int> &arr)
{
    std::vector<int> tails;

    for (int num : arr)
    {
        if (tails.empty() || num > tails.back())
        {
            tails.push_back(num);
        }
        else
        {
            int idx = binarySearchLowerBound(tails, num);
            tails[idx] = num;
        }
    }

    return tails.size();
}

// === Load Tests from File ===
std::vector<std::pair<int, std::vector<int>>> loadTests(const std::string &filepath)
{
    std::ifstream file(filepath);
    std::vector<std::pair<int, std::vector<int>>> tests;

    if (!file)
    {
        std::cerr << "❌ Failed to open " << filepath << "\n";
        return tests;
    }

    std::string line;
    while (std::getline(file, line))
    {
        int expected = std::stoi(line);
        if (!std::getline(file, line))
            break;

        std::istringstream iss(line);
        std::vector<int> input;
        int val;
        while (iss >> val)
            input.push_back(val);

        tests.emplace_back(expected, input);
    }

    return tests;
}

// === Run Tests ===
void runTestSet(const std::string &label,
                const std::vector<std::pair<int, std::vector<int>>> &tests,
                int (*lisFunc)(const std::vector<int> &))
{
    std::cout << "\n🔍 Testing " << label << " on " << tests.size() << " cases\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < tests.size(); ++i)
    {
        const auto &[expected, input] = tests[i];
        int result = lisFunc(input);

        if (result != expected)
        {
            std::cerr << "❌ Test " << i + 1 << " failed: got " << result
                      << ", expected " << expected << "\n";
        }
        else
        {
            std::cout << "✅ Test " << i + 1 << " passed\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "⏱️  Total time: " << ms << " ms\n";
}

// === Main ===
int main()
{
    std::vector<std::string> files = {
        "../testdata/small.txt",
        "../testdata/edge_cases.txt",
        "../testdata/large.txt",
        "../testdata/worst.txt"};

    for (const auto &file : files)
    {
        std::cout << "\n📄 Loading tests from " << file << "\n";
        auto tests = loadTests(file);

        if (file.find("large") == std::string::npos && file.find("worst") == std::string::npos)
        {
            runTestSet("LIS Recursive", tests, LISrecursive);
        }
        else
        {
            std::cout << "⚠️  Skipping LIS Recursive on " << file << " (too large or worst-case)\n";
        }

        runTestSet("LIS Quadratic", tests, LISquadratic);
        runTestSet("LIS NlogN", tests, LISnlogn);
    }

    return 0;
}