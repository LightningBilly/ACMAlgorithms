#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "src/hw1/hw1.h"
#include "src/hw2/hw2.h"
#include "src/hw3/hw3.h"
#include "src/hw5/hw5.h"
#include "src/hw6/hw6.h"
#include "src/hw9/hw9.h"
#include "src/hw12/hw12.h"

const int MAXPATH=250;

int main() {

    std::cout << "Hello, World!" << std::endl;

    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    printf("The current directory is: %s\n", buffer);

    // hw1();
    // hw2();
    // hw3();
    // tutte_parameterization();
    // arap_parameterization();
    // arap_deformation();
    // qem_simplification();

    delaunay_trianglation();
    return 0;
}
