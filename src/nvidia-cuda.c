#include <iostream>
#include <cuda.h>
#include "sys/time.h"
using namespace std;
__global__ void vecAdd(int *A, int *C, int N)
{
		int i = blockIdx.x * blockDim.x + threadIdx.x ;
		if(i<N)
				C[i] = A[i] * 0.5;
}
void vecAdd_h(int *A1, int *C1, int N)
{
		for(int i=0;i<N;i++)
				C1[i] = A1[i] * 0.5;
}
int main(int argc,char **argv)
{
		int n=(argc==2?atoi(argv[1]):25);
		int n2=n;
for(int n=1;n<=n2;n+=500)
{
		int a[n];
		int block_size, block_no;
		int c[n];
		int c2[n];
		int *a_d,*c_d;
		block_size=64;
		block_no = (int)((n-1)/block_size)+1;
		dim3 dimBlock((block_size/2),(block_size/2)+1,1);
		dim3 dimGrid(block_no/2,(block_no/2)+1,1);
		for(int i=0;i<n;i++)
				a[i]=rand()%35;
		cudaMalloc((void **)&a_d,n*sizeof(int));
		cudaMalloc((void **)&c_d,n*sizeof(int));
		cudaMemcpy(a_d,a,n*sizeof(int),cudaMemcpyHostToDevice);
struct timeval t1_s,t1_e,t2_s,t2_e;
gettimeofday(&t1_s,0);
		vecAdd<<<block_no,block_size>>>(a_d,c_d,n);
		cudaThreadSynchronize();
gettimeofday(&t1_e,0);
gettimeofday(&t2_s,0);
		vecAdd_h(a,c2,n);
gettimeofday(&t2_e,0);
double time_d = (t1_e.tv_sec-t1_s.tv_sec)*1000000 + t1_e.tv_usec - t1_s.tv_usec;
double time_h = (t2_e.tv_sec-t2_s.tv_sec)*1000000 + t2_e.tv_usec - t2_s.tv_usec;
		cudaMemcpy(c,c_d,n*sizeof(int),cudaMemcpyDeviceToHost);
		printf("%d %lf %lf\n",n,time_d,time_h);
		cudaFree(a_d);
		cudaFree(c_d);
}
		return 0;
}
