/* HW6 read cafefully and follow all the instructions/comments
   this program compiles and runs as is although it's not doing much.
   DO NOT CHANGE THE SEED. Changing seed gives a different sequence of
   numbers but we want the same numbers for comparison purposes.

   FLOATING point radix sort requires some substantial changes to this code
   float n, lst[N],tmp[N];	
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define OK 1
#define NOK 0
#define NELM 100		/* default 100 elements */
#define N 1048576		/* 2^30 or 1 meg elements  */
//#define N 2097152
//#define N 4194304
//#define N 8388608
//#define N 16777216
//#define N 33554432

void selection_sort();
void swap();

void merge_sort();
void msort_recursive();

void radix_sort();
void int_radix_sort();
void float_radix_sort();
void swap();
void self_check();

int rand(void);
void srand();
int rand_r();
void init_lst();
void print_lst();
void print_lst2();

int n, lst[N], buf[N];
float lst2[7]={3.3, 1.1,2.2,5.5,8.8,0.0};
float buf2[7]={3.3, 1.1,2.2,5.5,8.8,0.0};


typedef union{
  float f;
  struct{
    unsigned int sign:1;
    unsigned int exponent:8;
    unsigned int mantissa:23;
  }info;
}c;

int main(int argc,char **argv) {

  long int del_sec,del_msec;
  struct timeval tv_s,tv_e;

  if (argc>1) n = atoi(argv[1]);
  else n = NELM;
  //printf("n=%d\n",n);
  init_lst(lst,n);

  for(int i=0; i<n; i++)
    buf[i]=lst[i];
  
  //n=6;   //for the float values
  //print_lst2(lst2,n);
  
  print_lst(lst,n);
  printf("\n");
  //gettimeofday(&tv_s, NULL); 
  selection_sort(lst,n);
  //merge_sort(lst,buf,n);
  //int_radix_sort(lst,buf,n);
  //float_radix_sort(lst2,buf2,n);
  //  gettimeofday(&tv_e, NULL); 

  /****
    PRINT elapsed time in sec and milli secs
  ****/
  
  //print_lst2(lst2,n);
  //self_check(lst2, n);
  print_lst(lst,n);
  self_check(lst, n);
  return 0;
}

void selection_sort(int lst[],int n){
  // fill here
  printf("");
  for(int x=0; x<n; x++){
    int min=lst[x];
    int indx=x;
    for(int y=x; y<n; y++){   //find min
      if(lst[y]<min){
	min=lst[y];
	indx=y;
      }
    }
    //swap
    int temp=lst[x];
    lst[x]=min;
    lst[indx]=temp;
  }
}

void merge_sort(int lst[], int buf[], int n){
  msort_recursive(lst, buf, 0, n-1);
}

//use recursion
void msort_recursive(int lst[], int buf[], int left, int right){
  // fill here
  if (left >= right)
    return;
  int temp=(left+right)/2;
  
  msort_recursive(lst, buf, left, temp);
  msort_recursive(lst, buf, temp+1, right);

  int i=left, k=left;
  int j=temp+1;
  //right is the length of the array -->ex: [5] [4] tmp=0; right=1;
  while(i<temp+1 && j<=right){
    if(buf[i] < buf[j])
      lst[k++]=buf[i++];
    else
      lst[k++]=buf[j++];
  }
  
  while(i<temp+1)
    lst[k++]=buf[i++];
  while(j<=right)
    lst[k++]=buf[j++];

  for(int g=0; g<NELM; g++)
    buf[g]=lst[g];
}

//fix the bucket size to 256. run 4 passes where each pass processes 8 bits
//use lst and tmp only. do not use any more memory of size n.
void radix_sort(int group) {
  int i,j,mask,buckets;
  int flag,*src,*dst;

  for (i=0; i<n; i=i+group) {  //group=radix =8
    // set src/dst to lst/buf depending on iter number
    // fill here with what's discussed in class
    // ...
  }
}

void int_radix_sort(int lst[], int buf[], int n){
  int buckets,mask, i, j;
  int src[n];
  int dst[n];

  int group=8;
  buckets=1<<group;  //256
  mask=buckets-1;    //255
  for(j=0; j<n; j++)  //setting src to the values of lst
    src[j]=lst[j];

  for (i=0; i<64/group; i++) { //iterations
    //printf("%d ",i);
    int cnt[buckets];
    int map[buckets];

    for(j=0; j<buckets; j++) cnt[j]=0;   //initialize values of cnt list
    for(j=0; j<n; j++){  //cnt
      cnt[ (src[j]>>group*i)&mask ]++;
      //printf("src:%d %d   ",src[j], (src[j]>>group*i)&mask);
    }
      
    map[0]=0;
    for(j=1; j<buckets; j++)
      map[j]= map[j-1] + cnt[j-1];
    
    for(j=0; j<n; j++)
      dst[ map[(src[j]>>group*i)&mask]++ ]=src[j];

    for(j=0; j<n; j++){
      src[j]=dst[j];
      //printf("%d  ", src[j]);
    }
    //printf("\n\n");
  }
  for(int w=0; w<n; w++)
    lst[w]=src[w];
}

void float_radix_sort(float lst[], float buf[], int n){
  int buckets,mask, i,j;
  int src[n],dst[n];

  int group=8;
  buckets = 1<<group;
  mask = buckets-1;

  for(j=0; j<n; j++){  //lst to src
    c temp={.f = lst[j]};
    int s = temp.info.sign;
    int e = temp.info.exponent;
    int m = temp.info.mantissa;
    //printf("mantissa %d : %d   ",j,m);
    src[j]= m;
  }
  printf("\n\n");

  for (i=0; i<64/group; i++) { //iterations
    //printf("%d ",i);
    int cnt[buckets];
    int map[buckets];

    for(j=0; j<buckets; j++) cnt[j]=0;   //initialize values of cnt list
    for(j=0; j<n; j++){  //cnt
      cnt[ (src[j]>>group*i)&mask ]++;
      //printf("src:%d %d   ",src[j], (src[j]>>group*i)&mask);
    }
      
    map[0]=0;
    for(j=1; j<buckets; j++)
      map[j]= map[j-1] + cnt[j-1];
    
    for(j=0; j<n; j++)
      dst[ map[(src[j]>>group*i)&mask]++ ]=src[j];

    for(j=0; j<n; j++){
      src[j]=dst[j];
      //printf("%d  ", src[j]);
    }
    //printf("\n\n");
  }
  
  for(int w=0; w<n; w++)
    lst[w]=src[w];
}

void print_lst(int *l,int n){
  int i;
  for (i=0; i<n; i++) {
    printf("%d ",l[i]);
  }
  printf("\n");
}
void print_lst2(float *l,int n){
  int i;
  for (i=0; i<n; i++) {
    printf("%f ",l[i]);
  }
  printf("\n");
}

void init_lst(int *l,int n){
  int i;
  int seed = time(0) % 100;	/* seconds since 1/1/1970 */
  printf ("seed=%d\n", seed);
  srand(1234);			/* SEED */
  for (i=0; i<n; i++) {
    l[i] = rand();
  }
}

void self_check(int *lst,int n) {
  int i,j,flag=OK,*lstp;

  lstp = lst;
  for (i=0;i<n-1;i++)
     if (lstp[i] > lstp[i+1]) { flag = NOK; break; }

  if (flag == OK) printf("sorted\n");
  else printf("NOT sorted at %d\n",i);
}

/* End of file */
