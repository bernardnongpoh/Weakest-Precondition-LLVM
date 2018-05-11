#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Random.h"
#include "kernel.h"
#include "constants.h"

void print_banner(void);

int main(int argc, char *argv[])
{
        /* default to the (small) cache-contained version */

        double min_time = 100;

        int FFT_size = FFT_SIZE;
        int SOR_size =  SOR_SIZE;
        int Sparse_size_M = SPARSE_SIZE_M;
        int Sparse_size_nz = SPARSE_SIZE_nz;
        int LU_size = LU_SIZE;


        /* run the benchmark */

        double res[6] = {0.0};
        Random R = new_Random_seed(RANDOM_SEED);

       res[1] = kernel_measureFFT( FFT_size, min_time, R);   
        printf("Done");

        Random_delete(R);

        return 0;
  
}

void print_banner()
{
 printf("**                                                              **\n");
 printf("** SciMark2 Numeric Benchmark, see http://math.nist.gov/scimark **\n");
 printf("** for details. (Results can be submitted to pozo@nist.gov)     **\n");
 printf("**                                                              **\n");
}
