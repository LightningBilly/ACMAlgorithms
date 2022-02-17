#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "src/hw1/hw1.h"
#include "src/hw2/hw2.h"
#include "src/hw3/hw3.h"
#include "src/hw4/hw4.h"

const int MAXPATH=250;

int main() {

    std::cout << "Hello, World!" << std::endl;

    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    printf("The current directory is: %s\n", buffer);

    // hw1();
    // hw2();
    // hw3();
    tutte_parameterization();
    return 0;
}
