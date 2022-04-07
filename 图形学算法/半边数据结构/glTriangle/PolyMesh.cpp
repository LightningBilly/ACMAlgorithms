//
// Created by chenbinbin on 2022/3/30.
//

#include "PolyMesh.h"

void MVert::adjustOutgoingHalfedge() {
    if (isIsolated())
        return;

    MHalfedge *begin = halfEdge();
    MHalfedge *he = begin;

    do {
        if (he->isBoundary()) {
            setHalfedge(he);
            return;
        }
        he = he->pair()->next();
    } while (he != begin);
}

MPoint3 MPolyFace::getFaceCenter() {
    assert(he_begin_ != nullptr);

    MHalfedge *he = he_begin_;

    MPoint3 getCenter(0, 0, 0);
    do {
        MVert *fv = he->toVertex();
        getCenter += fv->position();
        he = he->next();
    } while (he != he_begin_);

    getCenter = MPoint3(getCenter.x() / 3, getCenter.y() / 3, getCenter.z() / 3);
    return getCenter;
}


// polymesh method

std::vector<MVert *> PolyMesh::polygonVertices(MPolyFace *face) const{
    std::vector<MVert *> f_vert;
    MHalfedge *he_begin = face->halfEdge();
    MHalfedge *he = face->halfEdge();

    do {
        f_vert.push_back(he->fromVertex());
        he = he->next();
    } while (he != he_begin);
    return f_vert;
}

MVert *PolyMesh::addVertex(double x, double y, double z) {
    auto w_ptr = new MVert(x, y, z);
    w_ptr->set_index(vertices_.size());
    vertices_.push_back(w_ptr);
    return w_ptr;
}


MPolyFace *PolyMesh::addPolyFace(std::vector<size_t> &v_loop_id) {
    size_t v_size = v_loop_id.size();

    std::vector<MVert *> v_loop;
    v_loop.reserve(v_size);

    for (size_t i = 0; i < v_size; i++) {
        v_loop.push_back(vertices_[v_loop_id[i]]);
    }
    return addPolyFace(v_loop);
}

MPolyFace *PolyMesh::addPolyFace(std::vector<MVert *> &v_loop) {
    size_t v_size = v_loop.size();


    MPolyFace *face = newPolyFace();

    std::vector<bool> is_edge_new(v_size, false);
    std::vector<bool> is_needc(v_size, false);

    MHalfedge *he1 = nullptr;
    MHalfedge *he2 = nullptr;
    std::vector<MHalfedge *> vec_edges;
    /*
     * 创建边，标记是否为新边。
     * h1为从v_loop[i]出发的半边，h2反之。
     * 设置面的第1条边。
     * 设置h1的面为face.
     * 加入到vec_edges中。
     */
    for (size_t i = 0; i < v_size; i++) {
        size_t i1 = (i + 1) % v_size;

        MEdge *e = edgeBetween(v_loop[i], v_loop[i1]);
        if (e == nullptr) {
            e = addEdge(v_loop[i], v_loop[i1]);
            is_edge_new[i] = true;
        }

        if (e->halfEdge()->fromVertex() == v_loop[i]) {
            he1 = e->halfEdge();
            he2 = e->halfEdge()->pair();
        } else {
            he1 = e->halfEdge()->pair();
            he2 = e->halfEdge();
        }

        if (face->halfEdge() == nullptr) {
            face->setHalfedge(he1);
        }
        he1->setPolygon(face);
        vec_edges.push_back(he1);
    }

    MHalfedge *he_ip, *he_in;
    MHalfedge *he_op, *he_on;
    MHalfedge *he_bp, *he_bn;
//    /*
//     * 2条邻边都不是新边情况下的调整。
//     *
//     */
//    for (size_t i = 0; i < v_size; i++)
//    {
//        size_t ii = (i + 1) % v_size;
//        if (!is_edge_new[i] && !is_edge_new[ii])
//        {
//            he_ip = vec_edges[i];
//            he_in = vec_edges[ii];
//
//            if (he_ip->next() != he_in)
//            {
//                he_op = he_in->pair();
//                he_on = he_ip->pair();
//
//                he_bp = he_op;
//                do
//                {
//                    he_bp = he_bp->next()->pair();
//                } while (!he_bp->isBoundary());
//                he_bn = he_bp->next();
//
//                //// ok ?
//                //if (boundary_prev == inner_prev)
//                //{
//                //	omerr() << "PolyMeshT::add_face: patch re-linking failed\n";
//                //	return make_smart(InvalidFaceHandle, this);
//                //}
//
//                assert(he_bp->isBoundary());
//                assert(he_bn->isBoundary());
//
//                MHalfedge* patch_start = he_ip->next();
//                MHalfedge* patch_end = he_in->prev();
//
//                he_bp->setNext(patch_start);	patch_start->setPrev(he_bp);
//                patch_end->setNext(he_bn);		he_bn->setPrev(patch_end);
//                he_ip->setNext(he_in);			he_in->setPrev(he_ip);
//            }
//        }
//    }
//
//
    for (size_t i = 0; i < v_size; i++) {
        size_t i1 = (i + 1) % v_size;

        MVert *vh = v_loop[i1];
        he_ip = vec_edges[i];
        he_in = vec_edges[i1];

        assert(he_ip != nullptr);
        assert(he_in != nullptr);

        size_t id = 0;
        if (is_edge_new[i]) id |= 1;
        if (is_edge_new[i1]) id |= 2;


        MHalfedge *he_op = he_in->pair();
        MHalfedge *he_on = he_ip->pair();

        // set outer links
        switch (id) {
            case 1: // prev is new, next is old
                he_bp = he_in->prev();
                he_bp->setNext(he_on);
                he_on->setPrev(he_bp);
                vh->setHalfedge(he_on);
                break;

            case 2: // next is new, prev is old
                he_bn = he_ip->next();
                he_op->setNext(he_bn);
                he_bn->setPrev(he_op);
                vh->setHalfedge(he_bn);
                break;

            case 3: // both are new
                if (vh->halfEdge() == nullptr) // 两边都是新加入（一）
                {
                    vh->setHalfedge(he_on);
                    he_op->setNext(he_on);
                    he_on->setPrev(he_op);
                } else // 两边都是新加入（二）
                {
                    he_bn = vh->halfEdge();
                    he_bp = he_bn->prev();

                    he_bp->setNext(he_on);
                    he_on->setPrev(he_bp);
                    he_op->setNext(he_bn);
                    he_bn->setPrev(he_op);
                }
                break;
            case 0:
                is_needc[i1] = true;
        }

        he_ip->setNext(he_in);
        he_in->setPrev(he_ip);
    }

    for (size_t i = 0; i < v_size; i++) {
        if (is_needc[i]) { // 都是旧边调整出去的第一条边，尽量为边界边
            v_loop[i]->adjustOutgoingHalfedge();
        }
    }

    return face;
}


MEdge *PolyMesh::edgeBetween(MVert *v0, MVert *v1) {
    //MHalfedge* he = halfedgeBetween(v0, v1);
    //return he->edge();

    if (v0 == nullptr || v1 == nullptr) return nullptr;
    if (v0->halfEdge() == nullptr) return nullptr;

    MHalfedge *he_begin = v0->halfEdge();
    MHalfedge *he = v0->halfEdge();

    do {
        if (he->toVertex() == v1) {
            return he->edge();
        }
        he = he->rotateNext();
    } while (he != he_begin);

    return nullptr;
}

MEdge *PolyMesh::addEdge(MVert *v_begin, MVert *v_end) {
    if (v_begin == nullptr || v_end == nullptr) {
        return nullptr;
    }

    MEdge *edge = edgeBetween(v_begin, v_end);
    if (edge != nullptr) {
        return edge;
    }

    edge = newEdge(v_begin, v_end);
    MHalfedge *he0 = newHelfEdge();
    MHalfedge *he1 = newHelfEdge();

    edge->setHalfedge(he0);
    //edge->set_index(edges_.size()); edges_.push_back(edge);

    he0->setNext(he1);
    he0->setPrev(he1);
    he0->setPair(he1);
    he0->setVert(v_begin);
    he0->setEdge(edge);
    he0->setPolygon(nullptr);

    he1->setNext(he0);
    he1->setPrev(he0);
    he1->setPair(he0);
    he1->setVert(v_end);
    he1->setEdge(edge);
    he1->setPolygon(nullptr);

    return edge;
}

MEdge *PolyMesh::newEdge(MVert *v1, MVert *v2) {
    MEdge *w_ptr = new MEdge(v1, v2);
    w_ptr->set_index(edges_.size());
    edges_.push_back(w_ptr);
    return w_ptr;
}

MHalfedge *PolyMesh::newHelfEdge() {
    MHalfedge *w_ptr = new MHalfedge();
    w_ptr->set_index(half_edges_.size());
    half_edges_.push_back(w_ptr);
    return w_ptr;
}

MPolyFace *PolyMesh::newPolyFace() {
    MPolyFace *w_ptr = new MPolyFace();
    w_ptr->set_index(polygons_.size());
    polygons_.push_back(w_ptr);
    return w_ptr;
}


std::vector<MPolyFace *> PolyMesh::vertAdjacentPolygon(MVert *vert) const{
    std::vector<MPolyFace *> f_list;
    if(vert== nullptr || vert->halfEdge()== nullptr)return f_list;

    auto he_begin = vert->halfEdge();
    auto he = he_begin;

    do {
        if(he->polygon()!= nullptr)f_list.push_back(he->polygon());
        he = he->rotateNext();
    } while(he!=he_begin);

    return f_list;
}

std::vector<MHalfedge *> PolyMesh::vertAdjacentHalfEdge(MVert *vert) const{ // 出发的边
    std::vector<MHalfedge*> v_adj;
    if(vert== nullptr || vert->halfEdge()== nullptr)return v_adj;

    auto he_begin = vert->halfEdge();
    auto he = he_begin;

    do {
        v_adj.push_back(he);
        he = he->rotateNext();
    } while(he!=he_begin);

    return v_adj;
}

std::vector<MHalfedge *> PolyMesh::vertAdjacentReverseHalfEdge(MVert *vert) const { // 进入的边
    std::vector<MHalfedge*> v_adj;
    if(vert== nullptr || vert->halfEdge()== nullptr)return v_adj;

    auto he_begin = vert->halfEdge();
    auto he = he_begin;

    do {
        v_adj.push_back(he->pair());
        he = he->rotateNext();
    } while(he!=he_begin);

    return v_adj;
}

std::vector<MVert *> PolyMesh::vertAdjacentVertices(MVert *vert) const{
    std::vector<MVert*> v_adj;
    if(vert== nullptr || vert->halfEdge()== nullptr)return v_adj;

    auto he_begin = vert->halfEdge();
    auto he = he_begin;

    do {
        v_adj.push_back(he->toVertex());
        he = he->rotateNext();
    } while(he!=he_begin);

    return v_adj;

}

std::vector<MPolyFace *> PolyMesh::edgeAdjacentPolygon(MEdge *edge) const{
    std::vector<MPolyFace*> e_poly; e_poly.clear();

    MPolyFace* poly = edge->halfEdge()->polygon();
    if (poly != nullptr) e_poly.push_back(poly);

    poly = edge->halfEdge()->pair()->polygon();
    if (poly != nullptr) e_poly.push_back(poly);

    return e_poly;
}

MPolyFace * PolyMesh::edgeAdjacentAnotherPolygon(MEdge *edge, MPolyFace *face) const{
    MPolyFace *p1, *p2;

    p1 = edge->halfEdge()->polygon();
    p2 = edge->halfEdge()->pair()->polygon();
    if ( p1 == face){
        return p2;
    }
    return p1;
}

bool PolyMesh::isBoundary(MVert *vert) const{
    if (vert->halfEdge() == nullptr)
    {
        return true;
    }

    MHalfedge* he_begin = vert->halfEdge();
    MHalfedge* he = vert->halfEdge();
    do {
        if (he->isBoundary())
        {
            return true;
        }
        he = he->rotateNext();

    } while (he != he_begin);

    return false;
}

bool PolyMesh::isBoundary(const MEdge *edge) const{
    return (edge->halfEdge()->isBoundary() || edge->halfEdge()->pair()->isBoundary());
}

bool PolyMesh::isBoundary(const MHalfedge *halfedge) const{
    return halfedge->isBoundary();
}