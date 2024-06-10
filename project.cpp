#include <bits/stdc++.h>

using namespace std;

int threshold_energy = 30;

int num;
struct Node
{
    int current_energy;
    int renew_energy_input = 0;
    int resource_capacity; // It was in MIPS
    int soc;
};

struct Function
{
    int owner_node;
    int current_node;
    int replicas;
    int *replica;
    int resource_need;
};

vector<vector<int>> CalcMinDist(vector<vector<int>> &graph)
{
    int n = graph.size();
    vector<vector<int>> dist(graph);

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}

bool comp(Function &a, Function &b)
{
    return a.resource_need > b.resource_need;
}

int getCost(int replica_node, vector<vector<int>> &minDistances, int target_node)
{
    int cost = minDistances[replica_node][target_node];
    return cost;
}

int Place(struct Function *replica, int index, vector<vector<int>> minDistances, struct Node nodes[])
{
    // Find available nodes
    vector<int> available_nodes;
    available_nodes.clear();
    for (int i = 0; i < num; i++)
    {
        if (nodes[i].soc >= threshold_energy)
        {
            available_nodes.push_back(i);
        }
    }

    int min_cost = INT_MAX;
    int node_index = -1;

    // Iterate over available nodes to find the one with minimum cost
    for (int a : available_nodes)
    {
        int cost = getCost(replica->owner_node, minDistances, a);
        if (cost < min_cost)
        {
            min_cost = cost;
            node_index = a;
        }
    }
    // Place replica on the selected node if a suitable node is found
    if (node_index != -1)
    {
        int energy_used = ((double)(replica->resource_need) / (double)(nodes[node_index].resource_capacity) * nodes[node_index].soc);
        nodes[node_index].soc -= energy_used;
        replica->replica[index] = node_index; // Update replica's current node
        return 1;                     // Replica placed successfully
    }

    return 0; // Replica placement failed
}
void printPlacements(struct Node nodes[], int num, struct Function function[], int nof)
{
    for(int x = 0; x < nof; x++)
        {
            cout << "Function " << x << ": No. of replicas = " << function[x].replicas << " \tReplicas: ";
            for(int y = 0; y < function[x].replicas; y++)
                cout << function[x].replica[y] << " ";
            cout << endl;
        }
        cout << endl;
}
void updateNode(struct Node *node, int num)
{
    for (int i = 0; i < num; i++)
    {
        node[i].current_energy = (rand() % 9 + 2) * 10;
        node[i].soc = node[i].current_energy;
    }
}
int main()
{
    // Input as number of node
    int nof;
    cout << "Enter the number of nodes needed: ";
    cin >> num;

    srand(time(0));

    vector<vector<int>> graph(num, vector<int>(num, 0));


    // Assuming the graph is undirected, set the weights accordingly
    for (int i = 0; i < num; i++)
    {
        for (int j = i + 1; j < num; j++)
        {
            int weight = rand() % 10 + 1;

            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }

    vector<vector<int>> minDistances = CalcMinDist(graph);

    struct Node node[num];

    for (int i = 0; i < num; i++)
    {
        node[i].current_energy = (rand() % 9 + 2) * 10;
        //node[i].resource_capacity = (rand() % 10 + 1) * 1000;
        node[i].resource_capacity = 1000;
        cout << "Resource capacity for node" << i << " = " << node[i].resource_capacity << endl;
        //node[i].soc = node[i].current_energy;
        node[i].soc = 90;
        cout << "SoC for node" << i << " = " << node[i].soc << endl;
    }

    cout << "Enter the number of functions generated: ";
    cin >> nof;

    struct Function function[nof];
    int L = 0;
    function[0].replicas = 3;
    function[0].owner_node = 0;
    function[1].replicas = 5;
    function[1].owner_node = 1;
    function[2].replicas = 3;
    function[2].owner_node = 2;
    for (int i = 0; i < nof; i++)
    {
        //function[i].owner_node = (rand() % nof); // 0 to n - 1
        cout << "Owner node of function " << i << " = " << function[i].owner_node << endl;
        function[i].current_node = function[i].owner_node;
        //function[i].replicas = (rand() % 5 + 1);
        cout << "Replicas generated for function " << i << " = " << function[i].replicas << endl;
        //function[i].resource_need = (rand() % 10 + 1) * 100;
        function[i].resource_need = 400;
        cout << "Resource need of function " << i << " = " << function[i].resource_need << endl;
        function[i].replica = (int *)malloc(function[i].replicas * sizeof(int));
        for(int x = 0; x < function[i].replicas; x++)
            function[i].replica[x] = -1;
        L = max(L, function[i].replicas);
    }
    std::sort(function, function + nof, comp);
    int i = 0;
    while (i < L)
    {
        for (int j = 0; j < nof; j++)
        {
            if (i < function[j].replicas)
            {
                Place(&function[j], i, minDistances, node);
            }
        }
        i++;
    }
    cout << "Output: " << endl;
    printPlacements(node, num, function, nof);
    return 0;
}
