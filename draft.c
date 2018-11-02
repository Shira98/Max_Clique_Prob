//Main file.

#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "mmio.c"
#include<time.h>
#include "serial_vc.c"

#define MAX_SIZE 50000000

int main(int argc, char *argv[])
{
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;   
    int i, *I, *J;
    double *val;
    double time_spent = 0.0;
    int count=0,count2=0;
 
    int *clique;
    
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
        exit(1);
    }
    else    
    { 
        if ((f = fopen(argv[1], "r")) == NULL) 
            exit(1);
    }

    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);


    /* reseve memory for matrices */

    clique=(int *)malloc(M*sizeof(int));
    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    val = (double *) malloc(nz * sizeof(double));

    int **graph = (int **)malloc(M * sizeof(int *)); 
    for (i=0; i<N; i++) 
         graph[i] = (int *)malloc(N * sizeof(int)); 

    
    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    for (i=0; i<nz; i++)
    {
        fscanf(f, "%d %d\n", &I[i], &J[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
    }

    if (f !=stdin) fclose(f);

    /************************/
    /* now write out matrix */
    /************************/

    
    mm_write_banner(stdout, matcode);
    mm_write_mtx_crd_size(stdout, M, N, nz);
    
    //static int graph[N][M];

    for (i=0; i<nz; i++){
       fprintf(stdout, "%d %d\n", I[i]+1, J[i]+1);
            graph[I[i]][J[i]]=1;
            count++;


    }

    clock_t begin = clock();
    int *clique1;
    //int clique_size=maximum_clique((const unsigned int **)graph,N,&clique1);
    
    maxclique(graph,N,&clique1);
    clock_t end = clock();
    printf("\n The Maximum Clique of the graph:\n ");
    for(i=0;i<N;i++){
        printf("%d",*(clique1+i));
    }
    printf("\n");

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time taken: %fs\n", time_spent);

    return 0;
}
