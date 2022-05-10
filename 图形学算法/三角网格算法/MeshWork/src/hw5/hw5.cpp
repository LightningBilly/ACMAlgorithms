// arap_parameterizations
#include <fstream>
#include"../../PolyMesh/IOManager.h"
#include"../../PolyMesh/PolyMesh.h"
#include<Eigen/SVD>
#include<Eigen/Dense>
#include<Eigen/Sparse>
#include <vector>
#include<iostream>
#include<math.h>
#include<algorithm>
#include <unistd.h>
#include "hw5.h"
using namespace acamcad;
using namespace polymesh;
using namespace std;

PolyMesh mesh;

void Tutte(Eigen::MatrixX2d& uv)
{
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

    //solve the inner positions
    uv.col(0) = xu;
    uv.col(1) = xv;
}


void arap_parameterization()
{
    /*
    if (argc != 3)
    {
        std::cout << "========== Hw5 Usage  ==========\n";
        std::cout << std::endl;
        std::cout << "Input:	ACAM_mesh_HW5.exe	input_mesh.obj	output_mesh.obj\n";
        std::cout << std::endl;
        std::cout << "=================================================\n";
        return;
    }
    */
    char buffer[500];
    getcwd(buffer, 500);
    printf("The current directory is: %s/../\n", buffer);
    string mesh_path = buffer;
    mesh_path += "/../src/hw5/cow.obj";
    //读入网格
    loadMesh(mesh_path, &mesh);

    std::string out_path = buffer;
    out_path += "/../src/hw5/output_mesh.obj";

    //loadMesh("cow.obj", &mesh);
    mesh.updateMeshNormal();
    int nf = mesh.numPolygons();
    int nv = mesh.numVertices();

    //calc local coordinate
    Eigen::MatrixXd localcoord;
    localcoord.resize(nf, 6);

#pragma omp parallel for
    for (int i = 0; i < nf; i++)
    {
        auto f_h = mesh.polyface(i);

        auto n = f_h->normal();
        auto itfv = mesh.fv_iter(f_h);
        auto v0 = (*itfv)->position();
        ++itfv;
        auto v1 = (*itfv)->position();
        ++itfv;
        auto v2 = (*itfv)->position();

        MVector3 e = v1 - v0;

        auto x_ = e.normalized();
        auto y_ = cross(n,x_);

        auto e1 = v2 - v0;

        localcoord.row(i) << 0, 0, e.norm(), 0, dot(e1 , x_), dot(e1 , y_);
    }


    //calc cot-weight laplacian matrix
    vector<double> cots(mesh.numHalfEdges());
    vector<Eigen::Triplet<double>> trivec;
    for (auto ithe = mesh.halfedge_begin(); ithe != mesh.halfedge_end(); ithe++)
    {
        if (mesh.isBoundary(*ithe))
            continue;

        auto v1 = (*ithe)->fromVertex()->position();
        auto v2 = (*ithe)->toVertex()->position();
        auto v0 = (*ithe)->next()->toVertex()->position();

        auto e0 = v1 - v0;
        auto e1 = v2 - v0;
        double cotangle = dot(e0, e1) / cross(e0, e1).norm();

        cots[(*ithe)->index()] = cotangle;
        int vid0 = (*ithe)->fromVertex()->index();
        int vid1 = (*ithe)->toVertex()->index();
        trivec.emplace_back(vid0, vid0, cotangle);
        trivec.emplace_back(vid1, vid1, cotangle);
        trivec.emplace_back(vid0, vid1, -cotangle);
        trivec.emplace_back(vid1, vid0, -cotangle);
    }
    Eigen::SparseMatrix<double> smat;
    smat.resize(nv, nv);
    smat.setFromTriplets(trivec.begin(), trivec.end());

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(smat);


    Eigen::MatrixX2d uv;
    uv.resize(nv, 2);

    //tutte initialization;
    Tutte(uv);

    vector<Eigen::Matrix2d> Lts;
    Lts.resize(nf);
    //local-global iteration
    for (int iter = 0; iter < 100; iter++)
    {
        //local calc Lt
#pragma omp parallel for
        for (int i = 0; i < nf; i++)
        {
            auto f_h = mesh.polyface(i);
            auto itfv = mesh.fv_iter(f_h);
            auto i0 = (*itfv)->index();
            ++itfv;
            auto i1 = (*itfv)->index();
            ++itfv;
            auto i2 = (*itfv)->index();

            Eigen::Matrix2d P, S, J;

            P << uv(i1, 0) - uv(i0, 0), uv(i2, 0) - uv(i0, 0), uv(i1, 1) - uv(i0, 1), uv(i2, 1) - uv(i0, 1);
            S << localcoord(i, 2) - localcoord(i, 0), localcoord(i, 4) - localcoord(i, 0),
                    localcoord(i, 3) - localcoord(i, 1), localcoord(i, 5) - localcoord(i, 1);

            J = P * S.inverse();

            Eigen::JacobiSVD<Eigen::Matrix2d> svd(J, Eigen::ComputeFullU | Eigen::ComputeFullV);

            Eigen::Matrix2d U = svd.matrixU();
            Eigen::Matrix2d V = svd.matrixV();

            Eigen::Matrix2d R = U * V.transpose();

            if (R.determinant() < 0)
            {
                U(0, 1) = -U(0, 1);
                U(1, 1) = -U(1, 1);
                R = U * V.transpose();
            }
            Lts[i] = R;
        }

        //global calc b
        Eigen::VectorXd bu, bv;
        bu.setZero(nv);
        bv.setZero(nv);

        for (int i = 0; i < nf; i++)
        {
            auto f_h = mesh.polyface(i);

            auto n = f_h->normal();
            auto itfv = mesh.fv_iter(f_h);

            auto i0 = (*itfv)->index();
            ++itfv;
            auto i1 = (*itfv)->index();
            ++itfv;
            auto i2 = (*itfv)->index();

            auto he2 = f_h->halfEdge();
            auto he0 = he2->next();
            auto he1 = he0->next();

            Eigen::Vector2d e0, e1, e2;
            e0 << localcoord(i, 2), localcoord(i, 3);
            e1 << localcoord(i, 4) - localcoord(i, 2), localcoord(i, 5) - localcoord(i, 3);
            e2 << -localcoord(i, 4), -localcoord(i, 5);
            Eigen::Vector2d b0 = cots[he0->index()] * Lts[i] * e0;
            bu[i0] -= b0[0];
            bv[i0] -= b0[1];

            bu[i1] += b0[0];
            bv[i1] += b0[1];
            Eigen::Vector2d b1 = cots[he1->index()] * Lts[i] * e1;
            bu[i1] -= b1[0];
            bv[i1] -= b1[1];

            bu[i2] += b1[0];
            bv[i2] += b1[1];

            Eigen::Vector2d b2 = cots[he2->index()] * Lts[i] * e2;
            bu[i2] -= b2[0];
            bv[i2] -= b2[1];

            bu[i0] += b2[0];
            bv[i0] += b2[1];

        }

        //global solve
        uv.col(0) = solver.solve(bu);
        uv.col(1) = solver.solve(bv);
    }

    for (int i = 0; i < nv; i++)
    {
        auto v_h = mesh.vert(i);
        v_h->setPosition(uv(i, 0), uv(i, 1), 0);
    }

    writeMesh(out_path, &mesh);

}
