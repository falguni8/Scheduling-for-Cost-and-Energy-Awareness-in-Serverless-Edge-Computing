#include <iostream>
#include <set>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

struct Node
{
    int energy;
    int computationalCapability;
    int costToController; // Cost of going to the controller node
};//Node

struct Function
{
    int f_id;
    int energyNeeded;
    int computationalCapability;
    int ownerNode; // Initial owner node
    int currentNode; // Current node
};//Function

int calcCost(int startNode, int endNode, std::vector<Node>& nodes)
{
    if(startNode == endNode)
        return 0;
    else
        return nodes[endNode].costToController + nodes[startNode].costToController;
}//calcCost

bool Reservation(int q, int func, std::vector<Node>& nodes, std::vector<Function>& functions, std::vector<std::set<int>>& nodeMap, int size)
{
    if(q == func)
        return nodes[q].energy >= functions[func].energyNeeded;
    int energyUsed = 0;
    for(const auto& num: nodeMap[q])
        energyUsed += functions[num].energyNeeded;
    return (nodes[q].energy - energyUsed) >= functions[func].energyNeeded;
}//Reservation


int totalcost=0;

// Function to perform regular offloading
void regularOffload(int func, std::vector<Node>& nodes, std::vector<Function>& functions, std::vector<std::set<int>>& nodeMap, int size)
{
    int bestNode = -1;
    int minCost = INT_MAX;
    for(int j = 0; j < 6; ++j)
    {
        if(nodes[j].computationalCapability >= functions[func].computationalCapability && Reservation(j, func, nodes, functions, nodeMap, size))
        {
            int cost = calcCost(func, j, nodes);
            if(cost < minCost)
            {
                minCost = cost;
                bestNode = j;
            }//if
        }//if
    }//for loop
    if(functions[func].ownerNode != bestNode)
    {
        functions[func].currentNode = bestNode;
        nodeMap[bestNode].insert(func);
        nodeMap[functions[func].ownerNode].erase(func);
        totalcost+=minCost;
    }//if
    
}//regularOffload
    
/**    
// Function to perform sticky offloading
bool stickyOffload(Function& func, const vector<Node>& nodes, int stickiness) {
}
*/

int main() 
{
    vector<Node> nodes(6); // 6 nodes
    vector<Function> functions(5); // 5 functions
    std::vector<std::set<int>> nodeMap(6); // map of which node each function is assigned to
    vector<int> function_order;
    
    //Node 1
    nodes[0].energy = 20;
    nodes[0].computationalCapability = 50;
    nodes[0].costToController = 4;
    
    //Node 2
    nodes[1].energy = 45;
    nodes[1].computationalCapability = 10;
    nodes[1].costToController = 6;
    
    //Node 3
    nodes[2].energy = 80;
    nodes[2].computationalCapability = 30;
    nodes[2].costToController = 3;
    
    //Node 4
    nodes[3].energy = 70;
    nodes[3].computationalCapability = 45;
    nodes[3].costToController = 5;
    
    //Node 5
    nodes[4].energy = 88;
    nodes[4].computationalCapability = 35;
    nodes[4].costToController = 7;
    
    //Node 6
    nodes[5].energy = 86;
    nodes[5].computationalCapability = 20;
    nodes[5].costToController = 4;
    
    //Function 1
    functions[0].f_id = 0;
    functions[0].energyNeeded = 30;
    functions[0].computationalCapability = 10;
    functions[0].ownerNode = 0;
    functions[0].currentNode = 0;
    nodeMap[0].insert(0);
    
    //Function 2
    functions[1].f_id = 1;
    functions[1].energyNeeded = 20;
    functions[1].computationalCapability = 30;
    functions[1].ownerNode = 1;
    functions[1].currentNode = 1;
    nodeMap[1].insert(1);
    
    //Function 3
    functions[2].f_id = 2;
    functions[2].energyNeeded = 30;
    functions[2].computationalCapability = 19;
    functions[2].ownerNode = 2;
    functions[2].currentNode = 2;
    nodeMap[2].insert(2);
    
    //Function 4
    functions[3].f_id = 3;
    functions[3].energyNeeded = 40;
    functions[3].computationalCapability = 12;
    functions[3].ownerNode = 3;
    functions[3].currentNode = 3;
    nodeMap[3].insert(3);
    
    //Function 5
    functions[4].f_id = 4;
    functions[4].energyNeeded = 25;
    functions[4].computationalCapability = 15;
    functions[4].ownerNode = 4;
    functions[4].currentNode = 4;
    nodeMap[4].insert(4);
    
    /** Print the elements in each set
    for(int i = 0; i < 6; ++i)
    {
        std::cout << "Node " << i + 1 << " functions:";
        for(const auto& num : nodeMap[i])
            std::cout << " " << num + 1;
        std::cout << std::endl;
    }//for loop*/

    // Sort functions according to decreasing energy they needed
    sort(functions.begin(), functions.end(), [](const Function& a, const Function& b)
    {
        return a.energyNeeded > b.energyNeeded;
    });
    
    /**Print the functions in the vector after sorting
    cout << "\nFunctions after being sorted: " << endl;
    for (const auto& func : functions)
        std::cout << "Function: " << func.f_id + 1 << endl;*/
        
    for(int i = 0; i < 5; ++i)
        function_order.push_back(functions[i].f_id);
    
    /**std::cout << "Order of functions:";
    for (const auto& num : function_order) {
        std::cout << " " << num;
    }
    std::cout << std::endl;*/
        
    // Sort functions back to their initial order
    sort(functions.begin(), functions.end(), [](const Function& a, const Function& b)
    {
        return a.f_id < b.f_id;
    });

    for(int i = 0; i < 5; ++i)
    {
        regularOffload(function_order[i], nodes, functions, nodeMap, 6);
    }//for loop
    
    // Print the elements in each set
    cout << "Updated owner nodes after offloading:\n";
    for(int i = 0; i < 6; ++i)
    {
        std::cout << "Node " << i + 1 << " functions:";
        for(const auto& num : nodeMap[i])
            std::cout << " " << num + 1;
        std::cout << std::endl;
    }//for loop


    cout<<"Total Cost :"<<totalcost<<"\n";
    return 0;
}//main
