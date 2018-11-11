#include <string.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "mmio.c"
#include<time.h>

#define MAX_SIZE 50000000


static void maximum_clique_recursive(const unsigned int **adjmat, unsigned int order, 
		unsigned int *current_clique, unsigned int *current_clique_size, unsigned int *max_clique,
		unsigned int *max_clique_size, unsigned int level)
{
	unsigned int i, connected;
	if (level == order) {
		/* Found a new max clique */
		memcpy(max_clique, current_clique, order * sizeof(unsigned int));
		*max_clique_size = *current_clique_size;
		return;
	}
	/* Find out if current vertex is connected to all vertices in current clique */
	connected = 1;

	for (i = 0;i<level  && connected; i++) {
		if (current_clique[i] && !adjmat[level][i]) {
			connected = 0;
		}
	}

	if(connected)
	{
		/* Add this vertex to the clique */
		current_clique[level] = 1;
		(*current_clique_size)++;
		
		#pragma omp taskyield
		maximum_clique_recursive(adjmat, order, current_clique, current_clique_size, max_clique,max_clique_size, level + 1);
		(*current_clique_size)--;
	}
	
	#pragma omp task 
	if (*current_clique_size + order - level > *max_clique_size) {
		/* Still promising */
		current_clique[level] = 0;
		maximum_clique_recursive(adjmat, order, current_clique, current_clique_size, max_clique,max_clique_size, level + 1);
	
	}

	#pragma omp taskwait

}

unsigned int maximum_clique(const unsigned int **adjmat, unsigned int order,unsigned int **max_clique)
{
	unsigned int max_clique_size = 0;
	unsigned int current_clique_size = 0;
	unsigned int *current_clique = malloc(order * sizeof(unsigned int));
	*max_clique = malloc(order * sizeof(unsigned int));
	if (current_clique == NULL || *max_clique == NULL) {
		free(current_clique);
		free(max_clique);
		return 0;
	}
	unsigned int level=0;

	#pragma omp parallel shared(adjmat, order, current_clique, current_clique_size, max_clique,max_clique_size, level)
	#pragma omp single
		maximum_clique_recursive(adjmat, order, current_clique, &current_clique_size, *max_clique, &max_clique_size, level);
	
	free(current_clique);
	return max_clique_size;
}

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


	#pragma omp parallel shared(N,graph)
	#pragma omp for schedule(guided) 
	for (i=0; i<N; i++) 
		 graph[i] = (int *)malloc(N * sizeof(int)); 

	
	/* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
	/*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
	/*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

	#pragma omp parallel shared(nz,I,J)
	#pragma omp for schedule(guided)
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

	#pragma omp parallel shared(nz,I,J)
	#pragma omp for schedule(guided)
	for (i=0; i<nz; i++){
	   fprintf(stdout, "%d %d\n", I[i]+1, J[i]+1);
			graph[I[i]][J[i]]=1;
			count++;

	}

	clock_t begin = clock();
	unsigned int *clique1;

	unsigned int clique_size=maximum_clique((const unsigned int **)graph,N,&clique1);
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


/*
int main()
{
	const unsigned int order = 5; Vertices 
	unsigned int r1[] = {0, 1, 0, 1, 1};
	unsigned int r2[] = {1, 0, 1, 0, 1};
	unsigned int r3[] = {0, 1, 0, 0, 1};
	unsigned int r4[] = {1, 0, 0, 0, 1};
	unsigned int r5[] = {1, 1, 1, 1, 0};
	const unsigned int *adjmat[] = {r1, r2, r3, r4, r5};
	unsigned int *max_clique;
	unsigned int max_clique_size = maximum_clique(adjmat, order, &max_clique);
	unsigned int i;
	printf("Max clique size is %u\n", max_clique_size);
	for (i = 0; i < order; i++) {
		if (max_clique[i] == 1) {
			printf("%u ", i);
		}
	}
	putchar('\n');
	free(max_clique);
	return 0;
}
*/
