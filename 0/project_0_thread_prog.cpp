#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define NUMT	         _NUMT
#define ARRAYSIZE       10000000
#define NUMTRIES        1000

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main(){
#ifndef _OPENMP
        fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
        return 1;
#endif

        omp_set_num_threads( NUMT );
	fprintf(stderr, "Using %d threads\n", NUMT);

        double maxMegaMults = 0.;
        double sumMegaMults = 0.;
	double sumTime = 0.;

        for( int t = 0; t < NUMTRIES; t++ ) {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < ARRAYSIZE; i++ )
                        C[i] = A[i] * B[i];

                double time1 = omp_get_wtime( );
                double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
                sumMegaMults += megaMults;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;

		sumTime += (double)(1000000.*(time1-time0));
        }

	double avgMegaMults = sumMegaMults/(double)NUMTRIES;
        printf("   Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
        printf("Average Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);

	double avgTime = sumTime/(double)NUMTRIES;
	printf("Average Elapsed Time = %7.2lf microseconds\n", avgTime);

	FILE* fp;
	fp = fopen("results.txt", "a");
	fprintf(fp, "%.2lf\n", avgTime);
	fclose(fp);

        return 0;
}
