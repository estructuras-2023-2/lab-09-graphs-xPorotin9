#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;

struct Edge {
    string id;
    string city1;
    string city2;
    int cost;
    bool isDamaged;
    Edge(string i, string c1, string c2, int co) : id(i), city1(c1), city2(c2), cost(co), isDamaged(co > 0) {}
};

bool compareEdges(const Edge& a, const Edge& b) {
    if (a.cost != b.cost) return a.cost < b.cost;
    return a.id < b.id;
}

class DisjointSet {
private:
    vector<int> parent;
    vector<string> cities;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

public:
    DisjointSet(const set<string>& allCities) {
        for (const string& city : allCities) {
            cities.push_back(city);
            parent.push_back(parent.size());
        }
    }

    void join(string city1, string city2) {
        int i = findCity(city1);
        int j = findCity(city2);
        parent[find(i)] = find(j);
    }

    bool connected(string city1, string city2) {
        return find(findCity(city1)) == find(findCity(city2));
    }

    int findCity(const string& city) {
        for (size_t i = 0; i < cities.size(); ++i) {
            if (cities[i] == city) return i;
        }
        return -1;
    }
};

string reconstruye(vector<string> carreteras) {
    vector<Edge> edges;
    set<string> allCitiesSet;

    for (const string& carretera : carreteras) {
        stringstream ss(carretera);
        string id, city1, city2;
        int cost = 0;
        ss >> id >> city1 >> city2;
        if (ss >> cost) {
            edges.emplace_back(id, city1, city2, cost);
        } else {
            edges.emplace_back(id, city1, city2, 0);
        }
        allCitiesSet.insert(city1);
        allCitiesSet.insert(city2);
    }

    sort(edges.begin(), edges.end(), compareEdges);

    DisjointSet ds(allCitiesSet);

    vector<string> toRepair;
    for (const Edge& edge : edges) {
        if (!ds.connected(edge.city1, edge.city2)) {
            ds.join(edge.city1, edge.city2);
            if (edge.isDamaged) {
                toRepair.push_back(edge.id);
            }
        }
    }

    for (const string& city1 : allCitiesSet) {
        for (const string& city2 : allCitiesSet) {
            if (!ds.connected(city1, city2)) {
                return "IMPOSIBLE";
            }
        }
    }

    sort(toRepair.begin(), toRepair.end());
    string result;
    for (const string& id : toRepair) {
        if (!result.empty()) result += " ";
        result += id;
    }
    return result;
}