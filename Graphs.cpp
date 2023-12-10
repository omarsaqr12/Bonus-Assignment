// FILE: graphs.cpp
// Graph Library Implementation File for Asn 6

#include <fstream>		// For reading file from disk
#include <iostream>
#include <string>		// For file name
#include <iomanip>
#include <string>
#include <vector>
#include <limits.h>
#include "Edge.h"		// Deinition of an Edge

using namespace std;

// Constructor
Graphs::Graphs()
{
	E = V = order = 0;
}

// Destructor
Graphs::~Graphs()
{
}


// Map vertex number (0,1,2,..) to (A,B,C,..)
char Graphs::Vname(const int s) const
{
	return char(s+65);
}

// Get Graph from text File (file name is stored in string fname)
// Graph is stored in adjacency matrix
void Graphs::getGraph(string fname)		
{
   // Local data ...
	weightType wi;
	ifstream source;
	source.open("TestG.csv"); 
	    vector<vector<string>>vec;
string line;
while (getline(source,line))
{
   stringstream inputString(line);
 vector<string>ve;
 string x;
    while(getline(inputString,x,',')){
ve.push_back(x);
    }
    vec.push_back(ve);
    ve.clear();

}
for (int i = 2; i < vec.size(); i++)
{
 for (int j =1; j < vec[0].size(); j++)
 {
	
	vec[i][j].erase(0,1);
vec[i][j].erase(vec[i][j].length()-1,1);
	int d=stoi(vec[i][j]);
	AdjMatrix[i-2][j-1]=d;
 }
 
}

	V=vec.size()-2;
	getEdges();
}

// Display Adjacency Matrix
void Graphs::dispGraph() const
{
	int i,j;
	cout<<"Adjacency Matrix\n";
	for(i=0; i<V; i++)
	{
		for(j=0; j<V; j++)
			cout<< setw(3)<< AdjMatrix[i][j] << " ";
		cout<< endl;
	}
}

// Get Non-Zero edges from adjacency matrix
// and store them in array edges[]. 
void Graphs::getEdges()			
{
	int r , c;
	int i = 0;
	weightType weight;

	// Only examine weights above the diagonal 
	for (r = 0; r <= V-2; r++)
		for (c = r+1; c <= V-1; c++)
		{
			weight = AdjMatrix[r][c];
			if (weight > 0)
			{
				// save (r,c,weight) in edges[i]
				edges[i].u = r;
				edges[i].v = c;
				edges[i].w = weight;
				i++;
			}
		}

	E = i;		// Number of non-zero edges
	
}

// Get number of vertices (V)	
int Graphs::No_of_Verices() const 				
{
	return V;
}

// Get Number of Non-zero edges (E)
int Graphs::No_of_Edges() const 					
{
	return E;
}

// Output an edge (e): Vertex names and weight
void Graphs::printEdge(Edge e) const 			
{
	cout << Vname(e.u) << " " << Vname(e.v) << " " << e.w << endl;
}

// Display Graph Edges
void Graphs::dispEdges() const
{
	cout<<"Graph Edges\n";
	for (int i = 0; i < E; i++) 
		printEdge(edges[i]);
}

// Shortest paths from node s
// uses Dijkstra's Algorithm
void Graphs::shPath(int src)
{
	    int dist[V]; // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i
 
    bool sptSet[V]; // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized
 
    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)
 
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && AdjMatrix[u][v]
                && dist[u] != INT_MAX
                && dist[u] + AdjMatrix[u][v] < dist[v])
                dist[v] = dist[u] + AdjMatrix[u][v];
    }
 
    // print the constructed distance array
    printSolution(dist);

}

// Print path (vertex names) from source (s) to destination (i)
void Graphs::printPath(int s, int i) const
{
	if(i==s){cout<<Vname(s);}
	else{
		printPath(s,via[i]);cout<<Vname(i);
	}
}

// Node Visit Function
void Graphs::visit(int k)
{ 
	val[k]=++order;
for (int i = 0; i < 7; i++)
{
	if (AdjMatrix[k][i]!=0){
if(val[i]==-2)
{
visit(i);
}
	}

	
}

}

// Depth First Search Traversal
void Graphs::DFS()									
{ 
int k;
for ( k = 0; k<7; k++)
{
val[k]=-2;
}
for ( k = 1; k < 7; k++)
{
	if(val[k]==-2){visit(k);}
}

}
int Graphs:: minDistance(int dist[], bool sptSet[])
{
 
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < 7; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
void  Graphs::printSolution(int dist[])
{
    cout << "Vertex \t Distance from Source" << endl;
    for (int i = 0; i < V; i++)
	   {
		 char c=i+65;
        cout << c << " \t\t\t\t" << dist[i] << endl;
	   }
}
