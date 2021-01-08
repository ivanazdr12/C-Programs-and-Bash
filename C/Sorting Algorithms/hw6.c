#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>

#define OK 1
#define NOK 0
#define NELM 100		/* default 100 elements */
#define BIN 256
#define FMIN -1000
#define FMAX 1000
#define N 100000000        
//#define N 1048576		/* 2^30 or 1 meg elements  */
//#define N 2097152
//#define N 4194304
//#define N 8388608
//#define N 16777216
//#define N 33554432

void selection_sort();
void swap();

void merge_sort();
void msort_recursive();
void merge();

void radix_sort();
void self_check();
void init_lst();
void print_lst();

void self_checkf();
void init_flst();
void print_flst();

int lst[N], buf[N];
float flst[N], fbuf[N];

int main(int argc,char **argv) {

  long int del_sec,del_msec;
  struct timeval tv_s,tv_e;

  long unsigned int n;
  if (argc>1) n = strtoul(argv[1], NULL, 0);
  else n = NELM;
  printf("n=%lu\n",n);
  
#if defined(SELECTION) || defined(MERGE) || defined(INT_RADIX)
  init_lst(lst,n);
#endif  

#ifdef FLOAT_RADIX
  init_flst(flst, n);
#endif  

  gettimeofday(&tv_s, NULL);
   
#ifdef SELECTION
  selection_sort(lst,n);
#endif  

#ifdef MERGE
  merge_sort(lst,buf,n);
#endif

#ifdef INT_RADIX
  radix_sort((unsigned int *)lst,(unsigned int *)buf,n, 1);
#endif  

#ifdef FLOAT_RADIX
  radix_sort((unsigned int *)flst,(unsigned int *)fbuf,n, 1);
#endif  

  gettimeofday(&tv_e, NULL); 

  time_t sdiff = tv_e.tv_sec - tv_s.tv_sec;
  suseconds_t usdiff = tv_e.tv_usec - tv_s.tv_usec;
  int num_char;

  printf("%.6f%n seconds\n", sdiff + usdiff/1e6, &num_char);
  printf("%*.3f milliseconds\n", num_char, sdiff * 1000 + usdiff/1e3);  

#if defined(SELECTION) || defined(MERGE) || defined(INT_RADIX)
  self_check(lst, n);
#endif 

#ifdef FLOAT_RADIX
  self_checkf(flst, n);
#endif 

  return 0;
}

void swap(int *x, int *y) 
{ 
    int temp = *x; 
    *x = *y; 
    *y = temp; 
} 

void selection_sort(int lst[], int n){
  int i, j, min_i;  
  for (i = 0; i < n-1; i++){ 
  	//find min element
        min_i = i; 
        for (j = i+1; j < n; j++) 
          if (lst[j] < lst[min_i]) 
            min_i = j; 
        //swap min with first element 
        swap(&lst[min_i], &lst[i]); 
    } 
}

void merge_sort(int lst[], int buf[], int n){
  msort_recursive(lst, buf, 0, n-1);
}

void msort_recursive(int list[], int buf[], int left, int right){
  if(left < right){   //if there is more than 1 element 
  //find middle to split in half
  int mid= (left+right) / 2;
  msort_recursive(list, buf, left, mid);
  msort_recursive(list, buf, mid+1, right);
  merge(list, buf, left, mid, mid+1, right);
  }
}

void merge(int lst[], int buf[], int left_i, int right_j, int right_i, int j2){
  int i,j,k;
      i=left_i;     //starter
      j=right_i;    //starter
      k=0;
      while(i<=right_j && j<=j2)   
      {
          if(lst[i]<lst[j])
              buf[k++]=lst[i++];
          else
              buf[k++]=lst[j++];
      }
      while(i<=right_j){   //first
          buf[k++]=lst[i++];
      }    
      while(j<=j2){    //second
          buf[k++]=lst[j++];
      }    
      //move buf to lst
      for(i=left_i,j=0;i<=j2;i++,j++){
          lst[i]=buf[j];
      }
}


/*
void radix_sort(int idx) {
	int i,j,k,p,mask;
	int *src_p, *dst_p;
	mask = BIN - 1;
	if (flag == LST) {
		src_p = lst;
		dst_p = buf;
	} else {
		src_p = buf;
		dst_p = lst;
	}
	for(p=0;p<sizeof(int);p=p+idx){
		//count
		for (i = 0; i < BIN; i++) {
			count[(src_p[i] >> (idx*8))&mask]++;
		}
		//map
		map[0] = 0;
		for (j = 1; j < BIN; j++) {
			map[j] = map[j-1] + count[j-1];
		}
		//move
		for (k = 0; k < N; k++) {
			dst_p[map[(src_p[k] >> (idx*8))&mask]++] = src_p[k];
		}
	}
}
*/

void radix_sorting(unsigned int *dst, unsigned int *src, long unsigned int n, int i, int sign) {
  int radix = 8;  // 8 bits
  int buckets_num = 1 << radix;  // number of buckets

  long unsigned int *map = (long unsigned int *) calloc(buckets_num + 1, sizeof(long unsigned int));  // 0 in memory 
  unsigned int *destination = dst + n, *srce = src + n;
  long unsigned int *be = map + buckets_num + 1;

  //count
  for (unsigned int *s = src; s < srce; s++) {
    map[((*s >> i*radix) & 0xFF) + 1]++;
  }

  //sum buckets
  for (long unsigned int *buck = map + 1; buck < be - 1; ++buck) {
    buck[1] += *buck;  
  }

  //negative nums reverse and invert
  if (sign) {
    long unsigned int negative_num = n - map[buckets_num >> 1];
    for (long unsigned int *buck = map; buck < map + (buckets_num >> 1);
        *buck++ += negative_num);
    for (long unsigned int *buck = map + (buckets_num >> 1); buck < be - 1; buck++) {
      *buck = n - *buck - 1;
    }
  }

  //move 
  for (unsigned int *s = src; s < srce; ++s) {
    if (sign && (*s & (1 << 31))) {  
      dst[map[(*s >> i*radix) & 0xFF]--] = *s;
    } else {  
      dst[map[(*s >> i*radix) & 0xFF]++] = *s;
    }
  }

  free(map);
}

void radix_sort(unsigned int *list, unsigned int *temp, long unsigned int n, int sign) {
  radix_sorting(temp, list, n, 0, 0);
  radix_sorting(list, temp, n, 1, 0);
  radix_sorting(temp, list, n, 2, 0);
  radix_sorting(list, temp, n, 3, sign);
}

void print_lst(int *l, long unsigned int n){
  long unsigned int i;
  for (i=0; i<n; i++) {
    printf("%d ",l[i]);
  }
  printf("\n");
}

void init_lst(int *l, long unsigned int n){
  long unsigned int i;
  //  int seed = time(0) % 100;	/* seconds since 1/1/1970 */
  //  printf ("seed=%d\n", seed);
  srand(1234);			/* SEED */
  for (i=0; i<n; i++) {
    l[i] = rand();
  }
}

void self_check(int *lst, long unsigned int n) {
  long unsigned int i,j,flag=OK;
  for (i=0;i<n-1;i++)
     if (lst[i] > lst[i+1]) { flag = NOK; break; }

  if (flag == OK) printf("sorted\n");
  else printf("NOT sorted at %lu\n",i);
}

void print_flst(float *f, long unsigned int n) {
  for(float *k = f; k < f + n; k++)
    printf("%f ",  *k);
  printf("\n");
}

void init_flst(float *f, long unsigned int n) {
  srand(1234);
  for (float *k = f; k < f + n; k++) {
    *k = (float) rand() / ((float) RAND_MAX / (FMAX - FMIN)) + FMIN;
  }
}

void self_checkf(float *f, long unsigned int n) {
  for (float *k = f + 1; k < f + n; k++) {
    if (*k < *(k-1)) {
      printf("NOT sorted at %lu\n", k - f - 1);
      return;
    }
  }

  printf("sorted\n");
}
