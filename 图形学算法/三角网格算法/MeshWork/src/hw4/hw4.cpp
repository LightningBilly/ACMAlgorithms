//
// Created by chenbinbin on 2022/2/17.
//

#include "hw4.h"
#include <fstream>
#include"../../PolyMesh/IOManager.h"
#include"../../PolyMesh/PolyMesh.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>
#include<iostream>
#include<math.h>
#include<algorithm>
// #include<omp.h>
#include <unistd.h>
using namespace acamcad;
using namespace polymesh;
using namespace std;


PolyMesh mesh;

#define M_PI 3.1415926

void tutte_parameterization() {
/*
    if (argc != 2)
    {
        std::cout << "========== Hw4 Usage  ==========\n";
        std::cout << std::endl;
        std::cout << "Input:	ACAM_mesh_HW4.exe	mesh.obj\n";
        std::cout << std::endl;
        std::cout << "=================================================\n";
        return;
    }
*/
    char buffer[500];
    getcwd(buffer, 500);
    printf("The current directory is: %s/../\n", buffer);
    string mesh_path = buffer;
    mesh_path += "/../src/hw4/cow.obj";
    //读入网格
    // std::string mesh_path = argv[1];
    //PolyMesh *mesh1 = new PolyMesh();


    loadMesh(mesh_path, &mesh);

    //loadMesh("Bunny_head.obj", &mesh);
    mesh.updateMeshNormal();

    int F_N = mesh.numPolygons();
    int V_N = mesh.numVertices();

    //calc surface area
    double area_sum = 0;
    for (int i = 0; i < F_N; ++i)
    {
        auto f_h = mesh.polyface(i);
        auto itfv = mesh.fv_iter(f_h);
        auto v0 = (*itfv)->position();
        ++itfv;
        auto v1 = (*itfv)->position();
        ++itfv;
        auto v2 = (*itfv)->position();

        auto e0 = v1 - v0;
        auto e1 = v2 - v0;
        auto avec = cross(e0, e1);
        area_sum += avec.norm()/2.0;
    }

    //set the boundary vertices to circle
    int boundary_num = 0;
    auto it1 = mesh.halfedge_begin();
    while (!mesh.isBoundary(*it1))
        it1++;
    auto he_start = *it1;
    auto he_it = he_start;
    do
    {
        he_it = (he_it)->next();
        boundary_num++;
    } while (he_it != he_start);

    double delta_angle = 2 * M_PI / boundary_num;
    double area_1_factor = sqrt(area_sum / M_PI);
    Eigen::VectorXd position_of_mesh;
    position_of_mesh.resize(2 * V_N);
    for (int i = 0; i < boundary_num; ++i)
    {
        auto v_h = he_start->toVertex();
        position_of_mesh(v_h->index()) = area_1_factor * cos(i * delta_angle);
        position_of_mesh(v_h->index() + V_N) = area_1_factor * sin(-i * delta_angle);
        he_start = he_start->next();
    }


    //calc the matrix
    typedef Eigen::Triplet<double> T;
    typedef Eigen::SparseMatrix<double> SMatrix;

    std::vector<T> tripletlist;
    Eigen::VectorXd bu = Eigen::VectorXd::Zero(V_N);
    Eigen::VectorXd bv = Eigen::VectorXd::Zero(V_N);
    for (auto it1 = mesh.vertices_begin(); it1 != mesh.vertices_end(); it1++)
    {
        int it1idx = (*it1)->index();
        if (mesh.isBoundary(*it1))
        {
            tripletlist.push_back(T(it1idx, it1idx, 1));
            auto point = (*it1)->position();
            bu(it1idx) = position_of_mesh[it1idx];
            bv(it1idx) = position_of_mesh[it1idx + V_N];
        }
        else
        {
            for (auto it2 = mesh.vv_iter(*it1); it2.isValid() ; ++it2)
            {
                tripletlist.push_back(T(it1idx, (*it2)->index(), -1));
            }
            tripletlist.push_back(T(it1idx, it1idx, mesh.valence(*it1)));
        }
    }

    SMatrix coff(V_N, V_N);
    coff.setFromTriplets(tripletlist.begin(), tripletlist.end());
    Eigen::SparseLU<SMatrix> solver;
    solver.compute(coff);
    Eigen::VectorXd xu = solver.solve(bu);
    Eigen::VectorXd xv = solver.solve(bv);

    Eigen::MatrixX2d uv;
    uv.resize(V_N, 2);

    //solve the positions
    uv.col(0) = xu;
    uv.col(1) = xv;

    for (int i = 0; i < V_N; i++)
    {
        auto v_h = mesh.vert(i);
        v_h->setPosition(uv(i, 0), uv(i, 1), 0);
    }
    writeMesh("bunny_tutte_para.obj", &mesh);
}