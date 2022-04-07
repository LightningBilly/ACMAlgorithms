//
// Created by chenbinbin on 2022/3/30.
//
#include "MPoint3.h"
#include "MVector3.h"
#ifndef GLTRIANGLE_POLYMESH_H
#define GLTRIANGLE_POLYMESH_H

class MVert;
class MEdge;
class MHalfedge;
class MPolyFace;

// 点定义
class MVert
{
private:
    int index_;// 编号， 从0开始

    MPoint3 point_; // 坐标

    MHalfedge* he_; // 从该点出发的一条半边，尽量设置成边界边（如果有）
public:
    MVert() : index_(-1), point_(0, 0, 0), he_(nullptr){}
    MVert(double x, double y, double z) : index_(-1), point_(x, y, z), he_(nullptr) {}
    ~MVert() { index_ = -1; }

public:
    MHalfedge* const halfEdge() { return he_; }
    const MHalfedge* const halfEdge() const { return he_; }

    void setHalfedge(MHalfedge* he) { he_ = he; }

    void setPosition(MPoint3 new_point) { point_ = new_point; }
    void setPosition(double x, double y, double z) { point_ = MPoint3(x, y, z); }
    MPoint3 position() { return point_; }
    const MPoint3& position() const { return point_; }

    double x() const { return point_.x(); }
    double y() const { return point_.y(); }
    double z() const { return point_.z(); }

    int index() const { return index_; }
    void set_index(int index) { index_ = index; }

    bool isIsolated() const { return he_ == nullptr; }

    //Try to set the half of the point as the boundary half. Be sure call it before adding faces by yourself.
    void adjustOutgoingHalfedge();
};

// 半边结构
class MHalfedge {
private:
    int index_; // 编号

    MVert* v_; // 半边起点
    MEdge* e_; // 半边对应的全边
    MPolyFace* poly_face_; // 半边对应的三角面

    MHalfedge* next_, * prev_; // 后继和前驱

    MHalfedge* pair_; // 对应的另一条半边

public:
    MHalfedge() : index_(-1), next_(nullptr), prev_(nullptr), pair_(nullptr),
                  v_(nullptr), e_(nullptr), poly_face_(nullptr){}
    MHalfedge(MHalfedge* next, MHalfedge* prev, MHalfedge* pair, MVert* v, MEdge* e, MPolyFace* p)
            : index_(-1), next_(next), prev_(prev), pair_(pair), v_(v), e_(e), poly_face_(p){}

    ~MHalfedge() { index_ = -1; };
public:
    MHalfedge* const next() { return next_; }
    MHalfedge* const prev() { return prev_; }
    MHalfedge* const pair() { return pair_; }
    MVert* const fromVertex() { return v_; }
    MVert* const toVertex() { return next()->fromVertex(); }
    MEdge* const edge() { return e_; }
    MPolyFace* const polygon() { return poly_face_; }

    MHalfedge* const rotateNext() { return pair()->next(); }
    MHalfedge* const rotatePrev() { return prev()->pair(); }

    const MHalfedge* const next() const { return next_; }
    const MHalfedge* const prev() const { return prev_; }
    const MHalfedge* const pair() const { return pair_; }
    const MVert* const fromVertex() const { return v_; }
    const MVert* const toVertex() const { return next()->fromVertex(); }
    const MEdge* const edge() const { return e_; }
    const MPolyFace* const polygon() const { return poly_face_; }

    void setNext(MHalfedge* next) { next_ = next; }
    void setPrev(MHalfedge* prev) { prev_ = prev; }
    void setPair(MHalfedge* pair) { pair_ = pair; }
    void setVert(MVert* vert) { v_ = vert; }
    void setEdge(MEdge* edge) { e_ = edge; }
    void setPolygon(MPolyFace* poly) { poly_face_ = poly; }

    bool isBoundary() const { return poly_face_ == nullptr; }

    int index() { return index_; }
    //int edge_index() { return index_ / 2; }
    void set_index(int index) { index_ = index; }
};

// 全边结构
class MEdge {
private:
    int index_; // 编号

    MVert* v1_; MVert* v2_; // 对应2点

    MHalfedge* he_; // 其中一条半边
public:
    MEdge() : index_(-1), v1_(nullptr), v2_(nullptr), he_(nullptr) {}
    MEdge(MVert* v1, MVert* v2) : index_(-1), v1_(v1), v2_(v2), he_(nullptr){}
    MEdge(MVert* v1, MVert* v2, MHalfedge* he) : index_(-1), v1_(v1), v2_(v2), he_(he) {}

    ~MEdge() { index_ = -1; };

public:
    MHalfedge* const halfEdge() { return he_; }
    const MHalfedge* const halfEdge() const { return const_cast<MEdge*>(this)->halfEdge(); }

    void setHalfedge(MHalfedge* he) { he_ = he; }
    void setVert(MVert* v1, MVert* v2) { v1_ = v1; v2_ = v2; }
    void updateVert() { v1_ = he_->fromVertex(), v2_ = he_->toVertex(); }

    int index() const { return index_; }
    void set_index(int index) { index_ = index; }

    ///get Vertex of the edge, the 0 is the first, the 1 is the second, the return is not orderd;
    MVert* getVert(int edge_v)
    {
        updateVert();
        if (edge_v == 0) return v1_;
        else if (edge_v == 1) return v2_;
        else return nullptr;
    }

    const MVert* getVert(int edge_v) const
    {
        return const_cast<MEdge*>(this)->getVert(edge_v);
    }

    double length()
    {
        updateVert();
        MVector3 t = v1_->position() - v2_->position();
        return t.norm();
    }

    MPoint3 getCenter()
    {
        updateVert();
        return v1_->position() * 0.5 + v2_->position() * 0.5;
    }

    //MPoint3 interpolate(const double& t)
    //{
    //	updateVert();
    //	return v1_->position() * t + v2_->position() * (1 - t);
    //}
    /////get the direction of the edge, from v1 to v2;
    //MVector3 tangent() const
    //{
    //	MVector3 t = v2_->position() - v1_->position();
    //	t.normalize();
    //	return t;
    //}
};

// 三角面定义
class MPolyFace
{
private:
    int index_; // 编号

    MHalfedge* he_begin_; // 开始半边

public:
    MPolyFace() : index_(-1), he_begin_(nullptr){}
    MPolyFace(MHalfedge* he) : index_(-1), he_begin_(he){}
    ~MPolyFace() { index_ = -1; }

public:
    MHalfedge* const halfEdge() { return he_begin_; }
    const MHalfedge* const halfEdge() const { return const_cast<MPolyFace*>(this)->halfEdge(); }

    void setHalfedge(MHalfedge* he) { he_begin_ = he; }

    int index() const { return index_; }
    void set_index(int index) { index_ = index; }

    MPoint3 getFaceCenter();
};

class PolyMesh {
private:

    std::vector<MHalfedge *> half_edges_;
    std::vector<MVert *> vertices_;
    std::vector<MEdge *> edges_;
    std::vector<MPolyFace *> polygons_;

public:
    PolyMesh() {};

    ~PolyMesh(){};

public:
    std::vector<MVert *> polygonVertices(MPolyFace *face) const;

    std::vector<MPolyFace *> vertAdjacentPolygon(MVert *vert) const;

    std::vector<MHalfedge *> vertAdjacentHalfEdge(MVert *vert) const; // 出发的边

    std::vector<MHalfedge *> vertAdjacentReverseHalfEdge(MVert *vert) const; // 进入的边

    std::vector<MVert *> vertAdjacentVertices(MVert *vert) const;

    std::vector<MPolyFace *> edgeAdjacentPolygon(MEdge *edge) const;

    MPolyFace * edgeAdjacentAnotherPolygon(MEdge *edge, MPolyFace *face) const;

    bool isBoundary(MVert *vert) const;

    bool isBoundary(const MEdge *edge) const;

    bool isBoundary(const MHalfedge *halfedge) const;

    MVert* addVertex(double x, double y, double z);

    MPolyFace* addPolyFace(std::vector<size_t>& v_loop_id);

    MPolyFace* addPolyFace(std::vector<MVert*>& v_loop);

    MEdge* edgeBetween(MVert* v0, MVert* v1);

    MEdge* addEdge(MVert* v_begin, MVert* v_end);

    MEdge* newEdge(MVert* v1, MVert* v2);

    MHalfedge* newHelfEdge();

    MPolyFace* newPolyFace();

    size_t numVertices() const { return vertices_.size(); }

    size_t numPolygons() const { return polygons_.size(); }

    MVert *vert(const size_t id) { return (id < numVertices() ? vertices_[id] : nullptr); }

    MPolyFace *polyface(const size_t id) { return (id < numPolygons() ? polygons_[id] : nullptr); }
};

#endif //GLTRIANGLE_POLYMESH_H
