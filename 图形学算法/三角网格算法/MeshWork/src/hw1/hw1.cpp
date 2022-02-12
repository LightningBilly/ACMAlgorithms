#include <iostream>
#include <fstream>
#include <sstream>
#include "../../PolyMesh/IOManager.h"
#include "Algorithm.h"
#include <string>
#include "hw1.h"
#include <unistd.h>
#include <stdio.h>

using namespace std;

using namespace acamcad;
using namespace polymesh;

void hw1()
{

    char buffer[500];
    getcwd(buffer, 500);
    printf("The current directory is: %s/../\n", buffer);
    string mesh_path = buffer;
    mesh_path += "/../src/hw1/alien.obj";
    string vertices_path = buffer;
    vertices_path += "/../src/hw1/mst_vertices.txt";
    string shortest_path = buffer;
    shortest_path += "/../src/hw1/shortestpath.txt";

    PolyMesh* mesh = new PolyMesh();
    //alien.obj v.txt path.txt
	loadMesh(mesh_path, mesh);
	std::cout << mesh->numVertices();
	std::vector<int> landmarks;
	std::ifstream _in(vertices_path);
	std::string line;
	while (std::getline(_in,line))
	{
		std::stringstream ss;
		ss << line;
		int vid;
		ss >> vid;
		landmarks.push_back(vid);
	}


	std::vector<int> vertice, edges;
	if (landmarks.size() == 2)
	{
		std::vector<int> path;
		Dijkstra(*mesh, landmarks[0], landmarks[1], path);
		vertice.push_back(path[0]);
		for (int i = 0; i < path.size() - 1; i++)
		{
			vertice.push_back(path[i + 1]);
			edges.push_back(mesh->edgeBetween(mesh->vert(path[i]), mesh->vert(path[i+1]))->index());
		}
	}
	else if (landmarks.size()>2)
	{
		std::vector<std::vector<int>> path;
		Dijkstra_group(*mesh, landmarks, path);
		for (auto a : path)
		{
			edges.push_back(mesh->edgeBetween(mesh->vert(a[0]), mesh->vert(a[1]))->index());
			for (int i = 1; i < a.size() - 1; i++)
			{
				edges.push_back(mesh->edgeBetween(mesh->vert(a[i]), mesh->vert(a[i+1]))->index());
				vertice.push_back(a[i]);
			}
		}
		for (auto a : landmarks)
			vertice.push_back(a);
	}
	std::ofstream _out(shortest_path);
	_out << "VERTICES\n";
	for (auto a : vertice)
		_out << a << std::endl;
	_out << "EDGES\n";
	for (auto a : edges)
		_out << a << std::endl;
	_out.close();
}
