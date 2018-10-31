//input graph
//finding max clique
	//convert .mtx to required input
	//branch
	//bound
	// vertex coloring length of max clique of a graph is less than chromatic no
	//get bound and apply it
	//output maxclique in a graph
	//check maxclique in a graph or maxclique containing a given vertex
/*
Procedure MaxClique(R, C)
1. while R = ∅ do
2. choose a vertex p with a maximum color C(p) from set R;
3. R := R\{p};
4. if |Q| + C(p) > |Qmax| then
5. Q := Q ∪ {p};
6. if R ∩ Γ(p) = ∅ then
7. obtain a vertex-coloring C of G(R ∩ Γ(p));
8. MaxClique(R ∩ Γ(p), C
);
9. else if |Q| > |Qmax| thenQmax := Q;
10. Q := Q\{p};
11. else return
12. end while
*/
 //MTX is a file extension for a data file used with Adobe Atmosphere.
//MTX files contain data used by Adobe Atmosphere, not intended to be opened or edited. 
 #include<stdio.h>
#include<stdlib.h>
 int *Q,*Qmax;
 int *coloring(int *g,int n,int R[n]){
	int i,j,color=0,j_color,no,k,no1;
	int *c;
	c=(int *)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		//stop when we find class it belogs to or assign new class
 		if(R[i]==1){
				//check with previous colors color
				no=0;
				no1=0;
				for(j=color-1; j>=0; j--){
					//check with each color
					no=0;
					for(k=0;k<=i-1;k++){
						if(R[k]==1){
						if(*(c+k)==j){
						 if(*((g+i*n)+k)==1){
						 		no=1;//it can't belong to that class
						}}
					}}
					if(no==1){
						no1=no1+1;
					}
 					if(no==0){
						//put it in that color class
						*(c+i)=j;
					}
 					}
 				if(no1==color){
					*(c+i)=color;
					color=color+1;
				}}
		else
				*(c+i)=-1;
		
	}
return c;
}
 int max_index(int *c,int n){
	int i,max_index=0;
//return index of max no in the color array
	for(i=0;i<n;i++){
				if(*(c+i)>*(c+max_index))
					max_index=i;
				
	}
return max_index;
}
 int len_clique(int n,int *Q){
int i,len=0;
for (i=0;i<n;i++){
	if(*(Q+i)==1){
		len=len+1;
	}
}
return len;
}
 int *MaxClique_vc(int *g,int n,int *c,int R[n]){ //returns maxclique in a graph
int v_count,index_vertex,len_q,len_qmax,common[n],*color_i,i,len_common,len_R,*r;
r=(int *)malloc(n*sizeof(int));
for ( i = 0; i < n; ++i)
*(r+i)=R[i];
//loop over all vertices to find  
len_R=len_clique(n,R);
while(len_R != 0){  //while R = ∅ do
//choose a vertex p with a maximum color C(p) from set R;
		c=coloring(g,n,R);
		index_vertex=max_index(c,n);//return index of maximum colored vertex 
		//max_index function wont change with change in R bcz c=-1
		// R := R\{p};
		R[index_vertex]=0;
		//if |Q| + C(p) > |Qmax| then
		len_q=len_clique(n,Q);
		len_qmax=len_clique(n,Qmax);
		if(len_q + *(c+index_vertex) > len_qmax){
		//Q := Q ∪ {p};
				*(Q+index_vertex)=1;
 				//Check None of its neughbours are currently in R
				//check common array
				for(i=0;i<n;i++)
					common[i]=0;
				for(i=0;i<n;i++){
					if(*((g+index_vertex*n)+i)==1){ //present in adj matrix
						if(R[i]==1){  //present in R
							common[i]=1;
						}
					}		
				} 
				//if R ∩ Γ(p) = ∅ then
				len_common=len_clique(n,common);
				if(len_common!=0){
					// obtain a vertex-coloring C of G(R ∩ Γ(p));
					color_i=coloring(g,n,common);
					// MaxClique(R ∩ Γ(p),C);
					MaxClique_vc(g,n,color_i,common);
				}
				else if(len_q > len_qmax){   // else if |Q| > |Qmax| thenQmax := Q;
					for(i=0;i<n;i++)
						*(Qmax+i)=*(Q+i);
				}
				// Q := Q\{p};
				*(Q+index_vertex)=0;
				// else return
				// end while
				}
		else
			return Qmax;
 }
return Qmax;
}
 int *maxclique(int *g,int n){
	int i,j,*c,*clique,R[n];
	Q=(int *)malloc(n*sizeof(int));
	Qmax=(int *)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		R[i]=1;
		*(Q+i)=0;
		*(Qmax+i)=0;
	}
	//calculate intitial coloring of the graph
	c=coloring(g,n,R);
	Qmax=MaxClique_vc(g,n,c,R);
	return Qmax;
}
