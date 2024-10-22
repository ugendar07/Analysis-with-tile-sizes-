#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>

#include <sys/time.h>

// sample test 
int N=1024;
int i,j,k,ii,jj,kk;
int A[1024][1024], B[1024][1024], C[1024][1024];


/*
int N=2048;
int i,j,k,ii,jj,kk;
int A[2048][2048], B[2048][2048], C[2048][2048];
*/

/*
int N=8192;
int i,j,k,ii,jj,kk;
int A[8192][8192], B[8192][8192], C[8192][8192];
*/

struct read_format 
{
    uint64_t nr;
    struct {
               uint64_t val;
               uint64_t id;
           } values[];
};

double start, finish;

double timestamp()
{
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}


void matrix_mul_ijk(int t)
{
   
       for(i=0; i<N; i=i+t)
          for(j=0; j<N; j=j+t)
              for (k=0; k < N; k=k+t)
                  for(ii=i; ii<i+t; ii++)
                        for(jj=j; jj < j+t; jj++)
                           for(kk=k; kk < k+t; kk++)
                               A[ii][jj] += B[ii][kk] * C[kk][jj];
        
       
}

void matrix_mul_ikj(int t)
{
       for(i=0; i<N; i=i+t)
          for (k=0; k < N; k=k+t)
              for(j=0; j<N; j=j+t)
                  for(ii=i; ii<i+t; ii++)
                      for(kk=k; kk < k+t; kk++)
                          for(jj=j; jj < j+t; jj++)
                             A[ii][jj] += B[ii][kk] * C[kk][jj];
        
       
}

void matrix_mul_kij(int t)
{
    for(k=0; k < N; k=k+t)
       for(i=0; i<N; i=i+t)
          for(j=0; j<N; j=j+t)
              for(kk=k; kk < k+t; kk++)
                 for(ii=i; ii<i+t; ii++)
                    for(jj=j; jj < j+t; jj++)
                        A[ii][jj] += B[ii][kk] * C[kk][jj];
        
       
}


int main(int argc, char* argv[]) {
/*Matrix Initilization */
for(i=0;i<N;i++)
  {
    for(j=0;j<N;j++)
    {
        A[i][j]=0;
        B[i][j]=i+j ;
        C[i][j]=2*i+j;
    }
  }
/*Perf Event_Attrs*/

struct perf_event_attr pe;
int fd1, fd2,fd3,fd4,fd5,fd6,fd7;
uint64_t id1, id2,id3,id4,id5,id6,id7;
uint64_t val1, val2,val3,val4,val5,val6,val7;
char buf[4096];
struct read_format* rf = (struct read_format*) buf;
int i;

memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_HARDWARE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_HW_CPU_CYCLES;
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd1 = syscall(__NR_perf_event_open, &pe, 0, -1, -1, 0);
ioctl(fd1, PERF_EVENT_IOC_ID, &id1);


memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_HARDWARE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_HW_CACHE_MISSES;   // LL cache misses
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd2 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
ioctl(fd2, PERF_EVENT_IOC_ID, &id2);


memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_SOFTWARE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_SW_PAGE_FAULTS;
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd3 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
ioctl(fd3, PERF_EVENT_IOC_ID, &id3);


memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_HW_CACHE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_HW_CACHE_L1D;
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd4 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
ioctl(fd4, PERF_EVENT_IOC_ID, &id4);

memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_HW_CACHE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_HW_CACHE_L1I;
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd5 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
ioctl(fd5, PERF_EVENT_IOC_ID, &id5);

memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_HW_CACHE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_HW_CACHE_DTLB;
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd6 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
ioctl(fd6, PERF_EVENT_IOC_ID, &id6);

memset(&pe, 0, sizeof(struct perf_event_attr));
pe.type = PERF_TYPE_HW_CACHE;
pe.size = sizeof(struct perf_event_attr);
pe.config = PERF_COUNT_HW_CACHE_ITLB;
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
fd7 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
ioctl(fd7, PERF_EVENT_IOC_ID, &id7);



int block_size;
for(block_size=2;block_size<=N;block_size=block_size*2)
{
   start = timestamp();
   ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
   ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

   matrix_mul_ijk(block_size);
   //matrix_mul_ikj(block_size);
   //matrix_mul_kij(block_size);


   ioctl(fd1, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
   finish = timestamp();
   read(fd1, buf, sizeof(buf));
   for (i = 0; i < rf->nr; i++) 
   {
       if (rf->values[i].id == id1) {
          val1 = rf->values[i].val;
       } else if (rf->values[i].id == id2) {
          val2 = rf->values[i].val;
       } else if (rf->values[i].id == id3) {
          val3 = rf->values[i].val;
       }else if (rf->values[i].id == id4) {
          val4 = rf->values[i].val;
       }else if (rf->values[i].id == id5) {
          val5 = rf->values[i].val;
       }else if (rf->values[i].id == id6) {
          val6 = rf->values[i].val;
       }else if (rf->values[i].id == id7) {
          val7 = rf->values[i].val;
       }

   }
   printf("\n\n\t\tTile Size:%d\n",block_size);
   printf("cpu cycles: %"PRIu64"\n", val1);
   printf("LL cache misses: %"PRIu64"\n", val2);
   printf("PAGE_FAULTS: %"PRIu64"\n", val3);
   printf("Cache misses measure L1D: %"PRIu64"\n", val4);
   printf("Cache misses measure L1I: %"PRIu64"\n", val5);
   printf("Cache misses measure DTLB: %"PRIu64"\n", val6);
   printf("Cache misses measure ITLB: %"PRIu64"\n", val7);
   printf("\n Execution Time: %f seconds\n", finish-start);
 }
return 0;
} 
