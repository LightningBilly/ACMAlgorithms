#include <vector>
#include <iostream>
#include "model.h"
#include <Eigen/Dense>

using namespace std;

//
// Created by chenbinbin on 2022/1/10.
//

#ifndef GLTRIANGLE_BSPLINE_H
#define GLTRIANGLE_BSPLINE_H

#endif //GLTRIANGLE_BSPLINE_H

double PointDistance(Vector3 p1, Vector3 p2);

class BSpline {
public:
    int m_nDegree;
    vector<double> m_vecKnots;
    vector<Vector3>m_vecCVs;
    vector<Vector3>points;
    /*!
    *\brief b样条曲线基函数计算
    *\ param double u 参数值
    *\ param int k 区间 k。可以通过std::upper_bound()-1获得
    *\ param std::vector<double> & basis_func basis_func基函数值，对应N_{k-p},...,N_k
    *\ Returns:   void
    */
    void BasisFunc(double u,int k,std::vector<double>& basis_func);
    BSpline CubicInterpolate(const std::vector<Vector3>& vecFitPoints);
    double Nipu (int i, int p, double u);

    void DrawControls();
    void DrawBS();
};