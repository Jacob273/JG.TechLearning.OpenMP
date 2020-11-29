#include <stdio.h>
#include <string.h>
#include <omp.h>

int main(int argc, char **argv){
    #pragma omp parallel
    printf("\nHello world");
}
