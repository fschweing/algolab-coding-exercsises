#include <iostream>
#include <cassert>
#include <vector>

class DisjointSet
{
public:
    DisjointSet(int size);
    int find(int x);
    void unionSets(int x, int y);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// constructor
DisjointSet::DisjointSet(int size)
{
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; ++i)
        parent[i] = i;
}

int DisjointSet::find(int x)
{
    if (parent[x] == x)
        return x;
    else
    {
        int y = find(parent[x]);
        parent[x] = y;
        return y;
    }
}

void DisjointSet::unionSets(int x, int y)
{
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY)
        return;

    if (rank[rootX] == rank[rootY])
    {
        parent[rootX] = rootY;
        rank[rootY]++;
    }
    else if (rank[rootX] < rank[rootY])
    {
        parent[rootX] = rootY;
    }
    else
    {
        parent[rootY] = rootX;
    }
}

int main()
{
    DisjointSet ds(6);

    // Initially all disjoint
    for (int i = 0; i < 6; ++i)
        assert(ds.find(i) == i);

    ds.unionSets(0, 1);
    assert(ds.find(0) == ds.find(1));

    ds.unionSets(1, 2);
    assert(ds.find(0) == ds.find(2));

    ds.unionSets(3, 4);
    assert(ds.find(3) == ds.find(4));
    assert(ds.find(3) != ds.find(0));

    ds.unionSets(2, 4);
    for (int i : {0, 1, 2, 3, 4})
        assert(ds.find(i) == ds.find(0));

    // Self union
    ds.unionSets(5, 5);
    assert(ds.find(5) == 5);

    std::cout << "✅ All test cases passed!\n";
    return 0;
}