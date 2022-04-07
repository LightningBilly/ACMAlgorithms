//
// Created by chenbinbin on 2022/1/10.
//
#include "BSpline.h"

// #define _DEBUG

const double eps = 1e-7;

/*!
*\brief b样条曲线基函数计算
*\ param double u 参数值
*\ param int k 区间 k。可以通过std::upper_bound()-1获得
*\ param std::vector<double> & basis_func basis_func基函数值，对应N_{k-p},...,N_k
*\ Returns:   void
*/
void BSpline::BasisFunc(double u,int k,std::vector<double>& basis_func)
{
    const int& p = m_nDegree;
    const vector<double>& knots = m_vecKnots;
    basis_func.resize(p+1);

    //2p+1个 N_{i,0}
    int n = 2 * p + 1;
    vector<double> temp(n,0);
    temp[p] =1;

    //迭代p次
    for (int j=1;j<=p;++j)
    {
        //区间 [k-p,k+p+1)
        for (int i=k-p,h = 0;h < (n - j);++h,++i)
        {
            //递推公式
            double a = (u - knots[i]);
            double dev = (knots[i+j] - knots[i]);
            a = (dev !=0) ? a /dev : 0;

            double b = (knots[i+j+1]-u);
            dev = (knots[i+j+1] - knots[i+1]);
            b = (dev != 0)? b/dev : 0;

            temp[h] = a*temp[h] + b*temp[h + 1];
        }
    }
    //拷贝前 p+1个值到basis_func
    std::copy(temp.begin(), temp.begin() + p + 1, basis_func.begin());
}


double PointDistance(Vector3 p1, Vector3 p2) {
    p1 = p1-p2;
    return sqrt(p1.x*p1.x + p1.y*p1.y + p1.z*p1.z);
}

/*!
 *\brief Bessel Tangent 方法估算导数
*\ param const Point & p0,p1,p2 待求点
*\ param Vec3d & p0deriv,p1deriv,p2deriv 三点上导数的估计值
*\ Returns:
*/
void BesselTanget( Vector3 p0, Vector3 p1, Vector3 p2,
                  Vector3& p0deriv,Vector3& p1deriv,Vector3& p2deriv)
{
    double delta_t1 =PointDistance(p2,p1);
    double delta_t0 = PointDistance(p1,p0);
    Vector3 delta_p0 = 1.0/delta_t0*(p1-p0);
    Vector3 delta_p1 = 1.0/delta_t1*(p2-p1);
    double sum = delta_t0+delta_t1;
    p1deriv = delta_t1/sum * delta_p0 + delta_t0/sum * delta_p1;
    p0deriv = 2*delta_p0 - p1deriv;
    p2deriv = 2*delta_p1 - p1deriv;
}

/*!
 *\brief 三次B样条插值
*\ param const std::vector<Point> & vecFitPoints待插值点集合，需要点数不小于3
*\ Returns:   BSpline 插值样条曲线
*/
BSpline BSpline::CubicInterpolate(const std::vector<Vector3>& vecFitPoints)
{
    const int p=3;
    BSpline bs;
    int x = vecFitPoints.size();
    if(x<p)
    {
        cout<<"too less point !"<<endl;
        return bs;
    }

    //求解方程 N*P = F
    Eigen::MatrixXd N= Eigen::MatrixXd::Zero(x+2,x+2);
    Eigen::MatrixXd P= Eigen::MatrixXd::Zero(x+2,3);
    Eigen::MatrixXd F= Eigen::MatrixXd::Zero(x+2,3);

    bs.m_nDegree = p;
    bs.m_vecKnots.resize(x); //x+6个节点
    //计算节点
    bs.m_vecKnots[0] =0.0;
    for (int i=1;i<x;++i)
    {
        bs.m_vecKnots[i] = bs.m_vecKnots[i-1]
                           + PointDistance(vecFitPoints[i],vecFitPoints[i-1]);
    }
    //节点首尾构成p+1度重复
    bs.m_vecKnots.insert(bs.m_vecKnots.begin(),p,bs.m_vecKnots.front());
    bs.m_vecKnots.insert(bs.m_vecKnots.end(),p,bs.m_vecKnots.back());

    //1.填写矩阵N
    std::vector<double> basis_func;
    N(0,0) = 1;
    N(x-1,x+1) = 1;
    for (int i=p+1;i<x+p-1;++i)
    {
        //c(u)在 N_{i-p},...,N_i等p+1个基函数上非零
        bs.BasisFunc(bs.m_vecKnots[i],i,basis_func);
        for (int j=i-p,k=0;j<=i;++j,++k)
        {
            N(i-p,j) = basis_func[k];
        }
    }

    //导数
    N(x,0) = -1;
    N(x,1) = 1;
    N(x+1,x) = -1;
    N(x+1,x+1) = 1;

    //2.填写矩阵F
    for (int i=0;i<x;++i)
    {
        F(i,0) = vecFitPoints[i].x;
        F(i,1) = vecFitPoints[i].y;
        F(i,2) = vecFitPoints[i].z;
    }

    {
        Vector3 v0,v1,v2;
        BesselTanget(vecFitPoints[0],vecFitPoints[1],vecFitPoints[2],v0,v1,v2);
        Vector3 v= (bs.m_vecKnots[p+1]-bs.m_vecKnots[1])/(double)p*v0;
        F(x,0) = v.x;
        F(x,1) = v.y;
        F(x,2) = v.z;
    }

    {
        Vector3 v0,v1,v2;
        BesselTanget(vecFitPoints[x-3],vecFitPoints[x-2],vecFitPoints[x-1],v0,v1,v2);
        Vector3 v= (bs.m_vecKnots[x+1+p]-bs.m_vecKnots[x+1])/(double)p*v2;
        F(x+1,0) = v.x;
        F(x+1,1) = v.y;
        F(x+1,2) = v.z;
    }

    //解方程 N*P = F
    P = N.lu().solve(F);

#ifdef _DEBUG
    cout<<"N--------------"<<endl<<N<<endl;
	cout<<"F--------------"<<endl<<F<<endl;
	cout<<"P--------------"<<endl<<P<<endl;
#endif

    //将Eigen所求的结果赋给bs的control_vertex
    bs.m_vecCVs.resize(x+2);
    for(int i=0;i<x+2;++i)
    {
        Vector3& cv = bs.m_vecCVs[i];
        cv.x = P(i,0);
        cv.y = P(i,1);
        cv.z = P(i,2);
    }

    return bs;
}

double BSpline::Nipu(int i, int p, double u) {
    if (p == 1) {
        if (u >= m_vecKnots[i] && u < m_vecKnots[i + 1]) return 1.0;
        else return 0.0;
    }

    double Length1 = m_vecKnots[i + p-1] - m_vecKnots[i];

    double Length2 = m_vecKnots[i + p] - m_vecKnots[i+1];     // 支撑区间的长度

    double a = fabs(Length1)<eps? 0 : (u-m_vecKnots[i])/Length1;
    double b = fabs(Length2)<eps? 0 : (m_vecKnots[i+p]-u)/Length2;

    return  a* Nipu(i, p - 1, u)+b * Nipu(i + 1, p - 1, u);
}

void BSpline::DrawControls(){
    glColor3f(1, 0.5, 0);
    //glPointSize(10);
    glBegin(GL_LINE_STRIP);
    // glBegin(GL_POINTS);
    for (auto p : m_vecCVs) {
        glVertex3d(p.x, p.y, p.z);
    }
    glEnd();
}

void BSpline::DrawBS(){
    if(m_vecKnots.size()<1)return;
    glColor3f(0, 0.5, 0);
    glPointSize(1);

    if(points.size()) {
        glBegin(GL_POINTS);
        for(auto p: points) {
            glVertex3d(p.x, p.y, p.z);
        }
        glEnd();
        return;
    }

    glBegin(GL_POINTS);
    for(double u = 0;u<=m_vecKnots[m_vecKnots.size()-1];u+=0.001) {
        Vector3 p(0,0,0);

        for(int i=0;i<m_vecCVs.size();i++) {
            double y = Nipu(i, m_nDegree+1, u);
            if (y>0){
                // glVertex2d(u * 40, y * 40);
            }
            p = p+y*m_vecCVs[i];
        }
        // printf("%.3lf, %.3lf\n", u, y);
        points.push_back(p);
        glVertex3d(p.x, p.y, p.z);
    }

    glEnd();
}