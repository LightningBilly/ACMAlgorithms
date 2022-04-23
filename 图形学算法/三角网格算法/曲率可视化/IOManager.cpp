//
// Created by chenbinbin on 2022/4/3.
//

#include "IOManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

bool OBJReader::read(const std::string &_filename, PolyMesh *mesh) {
    fstream ifile(_filename.c_str(), std::ios_base::in);
    string::size_type dot_pos = _filename.rfind("/");
    bool result = loadMeshFromOBJ(ifile, mesh);

    ifile.close();
    return result;
}

bool OBJReader::loadMeshFromOBJ(std::istream &_in, PolyMesh *mesh) {
    string line;
    stringstream stream,lineData,tmp;
    double x,y,z;
    string keyWord;

    while (_in && !_in.eof())
    {
        std::getline(_in, line);
        if(line.size() == 0 || line[0]=='#' || isspace(line[0])) continue;
        stream.str(line);
        stream.clear();
        stream >> keyWord;
        if(keyWord=="v") { // 读入点
            // cout<<"v: "<<line<<endl;
            stream>>x>>y>>z;
            mesh->addVertex(x,y,z);
        }else if(keyWord=="f") { // 读入面
            // cout<<"f: "<<line<<endl;
            // read full line after detecting a face
            std::string faceLine;
            std::getline(stream, faceLine);
            /// cout<<faceLine<<endl;
            lineData.str(faceLine);
            lineData.clear();
            string vertex;
            std::vector<size_t> faceVertices;
            for(int i=0;i<3;i++) {
                lineData>>vertex;
                // cout<<"vitem: "<<vertex<<endl;
                int v;
                // 查找第1个/
                auto found = vertex.find("/");
                if(found!=string::npos) {
                    tmp.str(vertex.substr(0, found));
                }else {
                    tmp.str(vertex);
                }
                tmp.clear();
                tmp>>v;
                // cout<<"vindex: "<<v<<endl;
                faceVertices.push_back(v-1);
            }
            mesh->addPolyFace(faceVertices);
        }
    }
    cout<<"read end"<<endl;
    return true;
}

bool OBJWriter::write(const std::string &_filename, PolyMesh *mesh, std::streamsize _precision) const{
    std::fstream out(_filename.c_str(), std::ios_base::out);
    out.precision(_precision);
    bool result = write(out, mesh, _precision);

    out.close();
    return result;
}

bool OBJWriter::write(std::ostream &_out, PolyMesh *mesh, std::streamsize _precision) const{
    // header
    _out << "# " << mesh->numVertices() << " vertices, ";
    _out << mesh->numPolygons() << " faces" << '\n';

    MVert* vh;
    MPoint3 v;
    vector<MVert*> vhandles;
    // vertex data (point)
    for (size_t i = 0, nV = mesh->numVertices(); i < nV; ++i)
    {
        vh = mesh->vert(i);
        v = vh->position();

        _out << "v " << v[0] << " " << v[1] << " " << v[2] << '\n';
    }

    // faces (indices starting at 1 not 0)
    for (size_t i = 0, nF = mesh->numPolygons(); i < nF; ++i)
    {
        _out << "f";

        MPolyFace* face = mesh->polyface(i);
        vhandles = mesh->polygonVertices(face);

        for (size_t j = 0; j < vhandles.size(); ++j)
        {

            // Write vertex index
            auto idx = vhandles[j]->index() +1;
            _out << " " << idx;
        }

        _out << '\n';
    }

    return true;
}