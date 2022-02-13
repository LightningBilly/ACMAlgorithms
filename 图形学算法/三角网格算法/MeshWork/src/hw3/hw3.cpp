#include <iostream>
#include <fstream>
#include <sstream>
#include "../../PolyMesh/IOManager.h"
#include <string>
#include <unistd.h>

using namespace std;
using namespace acamcad;
using namespace polymesh;

void hw3()
{
    puts("hw3");
    /*
    if (argc != 5)
    {
        std::cout << "========== Hw3 Usage  ==========\n";
        std::cout << std::endl;
        std::cout << "ACAM_mesh_HW3.exe [model] [SigmaNormal] [NormalIterNum] [VertexIterNum]\n";
        std::cout << "ACAM_mesh_HW3.exe	mesh.obj 1 8 8\n";
        std::cout << std::endl;
        std::cout << "=================================================\n";
        return;
    }
     */

    char buffer[500];
    getcwd(buffer, 500);
    printf("The current directory is: %s/../\n", buffer);
    string mesh_path = buffer;
    mesh_path += "/../src/hw3/bunny_random.obj";
    double SigmaCenter, SigmaNormal=1;
    double NormalIterNum=8, VertexIterNum=8;

    PolyMesh* mesh = new PolyMesh();
    loadMesh(mesh_path, mesh);

    mesh->updateMeshNormal();
    std::vector<MVector3> NewNormal(mesh->numPolygons());
    std::vector<double> FaceArea(mesh->numPolygons());
    std::vector<MPoint3> FaceCenter(mesh->numPolygons());

    for (MPolyFace* fh : mesh->polyfaces())
    {
        int f_id = (*fh).index();
        NewNormal[f_id] = (*fh).normal();
        std::vector<MVert*> P;
        for (FaceVertexIter vv_it = mesh->fv_iter(fh); vv_it.isValid(); ++vv_it)
        {
            P.push_back(*vv_it);
        }
        auto e12 = P[1]->position() - P[0]->position();
        auto e13 = P[2]->position() - P[0]->position();
        double area = cross(e12, e13).norm() * 0.5;
        FaceArea[f_id] = area;
        FaceCenter[f_id] = mesh->calculatFaceCenter(fh);
    }

    /*
     * 计算点标准差参数
     * 把所有邻面的中点作差
     * 最后除以面的3倍
     */
    SigmaCenter = 0;
    for (MPolyFace* fh : mesh->polyfaces())
    {
        int f_id = (*fh).index();
        for (FaceFaceIter nei_fh = mesh->ff_iter(fh); nei_fh.isValid(); ++nei_fh)
        {
            int ff_id = (*nei_fh)->index();
            SigmaCenter += (FaceCenter[f_id] - FaceCenter[ff_id]).norm();
        }
    }
    SigmaCenter /= mesh->numPolygons() * 3;

    /*
     * 利用法向迭代公式计算法向
     */
    for (int i = 0; i < NormalIterNum; i++)
    {
        for (MPolyFace* fh : mesh->polyfaces())
        {
            double Kp = 0;
            MVector3 NewN(0, 0, 0);
            int fh_id = (*fh).index();
            for (FaceFaceIter nei_fh = mesh->ff_iter(fh); nei_fh.isValid(); ++nei_fh)
            {
                int nei_fh_id = (*nei_fh)->index();
                double delta_center = (FaceCenter[fh_id] - FaceCenter[nei_fh_id]).norm();
                double delta_normal = (NewNormal[fh_id] - NewNormal[nei_fh_id]).norm();
                double Aj = FaceArea[nei_fh_id];
                double Ws = exp(-delta_center * delta_center / (2 * SigmaCenter * SigmaCenter));
                double Wr = exp(-delta_normal * delta_normal / (2 * SigmaNormal * SigmaNormal));
                NewN += Aj * Ws * Wr * NewNormal[nei_fh_id];
                Kp += Aj * Ws * Wr;
            }
            NewNormal[fh_id] = NewN / Kp;
            NewNormal[fh_id] /= NewNormal[fh_id].norm();
        }
    }

    // 调整点位置
    for (int i = 0; i < VertexIterNum; i++)
    {
        for (MVert* vh : mesh->vertices())
        {
            MPoint3 x_i = (*vh).position();
            MPoint3 delta_xi(0, 0, 0);
            int Nei_count = 0;
            for (VertexFaceIter fh = mesh->vf_iter(vh); fh.isValid(); ++fh)
            {
                Nei_count++;
                MPoint3 cj = mesh->calculatFaceCenter(*fh);
                MVector3 nj = NewNormal[(*fh)->index()];
                delta_xi = delta_xi + nj * (nj.data()[0] * (cj - x_i).data()[0] + nj.data()[1] * (cj - x_i).data()[1] + nj.data()[2] * (cj - x_i).data()[2]);
            }
            x_i = x_i + delta_xi / Nei_count;
            (*vh).setPosition(x_i);
        }
    }

    // 输出网格数据
    writeMesh("result.obj", mesh);
}