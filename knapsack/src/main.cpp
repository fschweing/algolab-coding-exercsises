#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

int recHelper(int cap, int index, const std::vector<int> &weights, const std::vector<int> &values)
{
    if (cap <= 0 || index >= weights.size())
        return 0;

    int leave = 0 + recHelper(cap, index + 1, weights, values);

    if (cap - weights[index] >= 0)
    {
        int take = values[index] + recHelper(cap - weights[index], index + 1, weights, values);
        return take > leave ? take : leave;
    }
    return leave;
}

int knapsackRecursive(int capacity, const std::vector<int> &weights, const std::vector<int> &values)
{
    return recHelper(capacity, 0, weights, values);
}

void printDPTable(const std::vector<std::vector<int>> &dp)
{
    for (const auto &row : dp)
    {
        for (int val : row)
        {
            std::cout << val << "\t";
        }
        std::cout << "\n";
    }
}

int knapsackDP(int cap, const std::vector<int> &weights, const std::vector<int> &values)
{
    int n = weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

    int max = 0;

    for (int i = 0; i <= n; ++i)
    {
        int temp_cap = cap;
        for (int j = 1; j + i <= n; ++j)
        // observation: the current item we are dealing with is the i+j-th
        {
            if (i > 0)
            {
                // compare leaving current item (copy val from above) against taking item (val from the left + current val)
                //
                int leave = dp[i - 1][j];
                int take = dp[i][j - 1] + values[i + j - 1];
                if (temp_cap - weights[i + j - 1] >= 0 && take > leave)
                {
                    dp[i][j] = take;
                    temp_cap -= weights[i + j - 1];
                }
                else
                    dp[i][j] = leave;
            }
            else if (i == 0)
            {
                // fill first row, take  all the items as long as weights allow
                if (temp_cap - weights[i + j - 1] >= 0)
                {
                    dp[i][j] = dp[i][j - 1] + values[i + j - 1];
                    temp_cap -= weights[i + j - 1];
                }
                else
                    dp[i][j] = dp[i][j - 1];
            }
            max = max > dp[i][j] ? max : dp[i][j];
        }
    }
    // printDPTable(dp);
    return max;
}

// --- Utility to load test cases from file ---
std::vector<std::tuple<int, std::vector<int>, std::vector<int>, int>> loadTests(const std::string &filepath)
{
    std::ifstream file(filepath);
    std::vector<std::tuple<int, std::vector<int>, std::vector<int>, int>> tests;

    if (!file)
    {
        std::cerr << "❌ Failed to open " << filepath << "\n";
        return tests;
    }

    std::string line;
    while (std::getline(file, line))
    {
        int capacity = std::stoi(line);

        std::getline(file, line);
        std::istringstream weightStream(line);
        std::vector<int> weights;
        int w;
        while (weightStream >> w)
            weights.push_back(w);

        std::getline(file, line);
        std::istringstream valueStream(line);
        std::vector<int> values;
        int v;
        while (valueStream >> v)
            values.push_back(v);

        std::getline(file, line);
        int expected = std::stoi(line);

        tests.emplace_back(capacity, weights, values, expected);
    }

    return tests;
}

// --- Run and time test set ---
void runTestSet(const std::string &label,
                const std::vector<std::tuple<int, std::vector<int>, std::vector<int>, int>> &tests,
                int (*solver)(int, const std::vector<int> &, const std::vector<int> &))
{

    // test case structure
    // <capacity>
    // <space-separated weights>
    // <space-separated values>
    // <expected result>
    std::cout << "\n🔍 Running " << label << " on " << tests.size() << " test cases\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < tests.size(); ++i)
    {
        const auto &[capacity, weights, values, expected] = tests[i];
        int result = solver(capacity, weights, values);

        if (result == expected)
        {
            std::cout << "✅ Test " << i + 1 << " passed\n";
        }
        else
        {
            std::cout << "❌ Test " << i + 1 << " failed: got " << result << ", expected " << expected << "\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "⏱️  Elapsed: " << ms << " ms\n";
}

int main()
{
    std::vector<std::string> files = {
        "../testdata/small.txt",
        "../testdata/edge.txt",
        "../testdata/large.txt",
        "../testdata/worst.txt"};

    for (const auto &file : files)
    {
        std::cout << "\n📄 Loading: " << file << "\n";
        auto tests = loadTests(file);

        runTestSet("Recursive", tests, knapsackRecursive);
        runTestSet("Dynamic Programming", tests, knapsackDP);
    }

    return 0;
}