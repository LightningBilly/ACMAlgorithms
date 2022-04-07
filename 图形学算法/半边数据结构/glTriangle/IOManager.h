//
// Created by chenbinbin on 2022/4/3.
//
#include "PolyMesh.h"
#ifndef GLTRIANGLE_IOMANAGER_H
#define GLTRIANGLE_IOMANAGER_H

class OBJReader {
public:
    OBJReader() {};

    ~OBJReader() {};

    bool read(const std::string &_filename, PolyMesh *mesh);

private:
    bool loadMeshFromOBJ(std::istream &_in, PolyMesh *mesh);
    std::string path_;
};

class OBJWriter {
public:

    OBJWriter() {};

    ~OBJWriter() {};

    bool write(const std::string &, PolyMesh *mesh, std::streamsize _precision = 6) const;

private:

    mutable std::string path_;
    mutable std::string objName_;

    bool write(std::ostream &, PolyMesh *mesh, std::streamsize _precision = 6) const;
};
#endif //GLTRIANGLE_IOMANAGER_H
