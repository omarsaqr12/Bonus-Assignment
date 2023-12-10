#include <bits/stdc++.h>
using namespace std;
 #include"Graphs.h"
 #include"Edge.h"
 #include"Graphs.cpp"
int main() {
 Graphs gra;
 gra.getGraph("TestG");

gra.dispGraph();
gra.dispEdges();
gra.shPath(0);

}
 
