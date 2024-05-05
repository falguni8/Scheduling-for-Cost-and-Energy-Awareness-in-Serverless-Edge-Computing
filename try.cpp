#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Node
{
    int energy;
    int computationalCapability;
    int costToController;
};

struct Function
{
    int f_id;
    int energyNeeded;
    int computationalCapability;
    int ownerNode;
    int currentNode;
};

int calcCost(int startNode, int endNode, const vector<Node> &nodes)
{
    if (startNode == endNode)
        return 0;
    else
        return nodes[endNode].costToController + nodes[startNode].costToController;
}

int totalCost(const vector<int> &assignment, const vector<Node> &nodes, const vector<Function> &functions)
{
    int cost = 0;
    for (int i = 0; i < assignment.size(); ++i)
    {
        int funcId = assignment[i];
        int currentNode = functions[funcId].currentNode;
        int ownerNode = functions[funcId].ownerNode;
        cost += calcCost(currentNode, ownerNode, nodes);
    }
    return cost;
}

int main()
{
    vector<Node> nodes(6);
    vector<Function> functions(5);

    // Node initialization code here...

    // Function initialization code here...
    // Node 1
    nodes[0].energy = 30;
    nodes[0].computationalCapability = 40;
    nodes[0].costToController = 7;

    // Node 2
    nodes[1].energy = 50;
    nodes[1].computationalCapability = 10;
    nodes[1].costToController = 5;

    // Node 3
    nodes[2].energy = 70;
    nodes[2].computationalCapability = 25;
    nodes[2].costToController = 5;

    // Node 4
    nodes[3].energy = 70;
    nodes[3].computationalCapability = 45;
    nodes[3].costToController = 6;

    // Node 5
    nodes[4].energy = 88;
    nodes[4].computationalCapability = 35;
    nodes[4].costToController = 5;

    // Node 6
    nodes[5].energy = 86;
    nodes[5].computationalCapability = 20;
    nodes[5].costToController = 10;

    // Function 1
    functions[0].f_id = 0;
    functions[0].energyNeeded = 30;
    functions[0].computationalCapability = 10;
    functions[0].ownerNode = 0;
    functions[0].currentNode = 1;
    // nodeMap[functions[0].currentNode].insert(0);

    // Function 2
    functions[1].f_id = 1;
    functions[1].energyNeeded = 20;
    functions[1].computationalCapability = 30;
    functions[1].ownerNode = 1;
    functions[1].currentNode = 1;
    // nodeMap[functions[1].currentNode].insert(1);

    // Function 3
    functions[2].f_id = 2;
    functions[2].energyNeeded = 30;
    functions[2].computationalCapability = 19;
    functions[2].ownerNode = 2;
    functions[2].currentNode = 3;
    // nodeMap[functions[2].currentNode].insert(2);

    // Function 4
    functions[3].f_id = 3;
    functions[3].energyNeeded = 40;
    functions[3].computationalCapability = 12;
    functions[3].ownerNode = 3;
    functions[3].currentNode = 4;
    // nodeMap[functions[3].currentNode].insert(3);

    // Function 5
    functions[4].f_id = 4;
    functions[4].energyNeeded = 25;
    functions[4].computationalCapability = 15;
    functions[4].ownerNode = 4;
    functions[4].currentNode = 5;

    vector<int> function_order;

    // Initialize function_order vector with function ids
    for (int i = 0; i < 5; ++i)
        function_order.push_back(functions[i].f_id);

    // Assign each function to a different node
    for (int i = 0; i < 5; ++i)
    {
        int funcId = function_order[i];
        int currentNode = -1;
        for (int j = 0; j < 6; ++j)
        {
            if (nodes[j].computationalCapability >= functions[funcId].computationalCapability && j != functions[funcId].ownerNode)
            {
                currentNode = j;
                break;
            }
        }

        if (currentNode != -1)
        {
            // Place the function on the node
            functions[funcId].currentNode = currentNode;
            // Update the node's computational capability
            nodes[currentNode].computationalCapability -= functions[funcId].computationalCapability;
        }
        else
        {
            cout << "No suitable node found for function " << funcId + 1 << endl;
        }
    }

    // Print the function assignments
    cout << "Function Assignments:" << endl;
    for (int i = 0; i < 5; ++i)
    {
        cout << "Function " << function_order[i] + 1 << " assigned to Node " << functions[function_order[i]].currentNode + 1 << endl;
    }

    // Calculate and print the total cost
    int total_cost = totalCost(function_order, nodes, functions);
    cout << "Total Cost: " << total_cost << endl;

    return 0;
}
