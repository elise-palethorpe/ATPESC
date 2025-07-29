#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
// 
// This program traverses a linked list carrying out
// a compute-intensive computation for each element 
// of the list.  The computations are independent and
// don't matter at all for the purposes of this exercise.
// 
// The goal is to process the traversal of the list in 
// parllel, not the computation carried out for each 
// node in the list.
//
// Therefore, go to main() and stick to work in main 
// for this exercise.
// 

// N is the length of the list
#ifndef N
#define N 15
#endif

// Start computing fibonacci numbers from the FS'th element in
// the sequence during the process-work step in this program.
#ifndef FS
#define FS 30
#endif

// definition of a node in the list
struct node {
   int data;
   int fibdata;
   struct node* next;
};

// recurrsively compute a number in the fibonacci sequence
int fib(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
	  return (x + y);
   }
}

// compute the member in the fibonnacci sequence given
// by the value of data in the list of nodes
void processwork(struct node* p) 
{
   int n;
   n = p->data;
   p->fibdata = fib(n);
}

// initialize the list
struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;
    
    head = (struct node*) malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i=0; i< N; i++) {
       temp  =  malloc(sizeof(struct node));
       p->next = temp;
       p = temp;
       p->data = FS + i + 1;
       p->fibdata = i+1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
   double start, end;
   struct node *p=NULL;
   struct node *temp=NULL;
   struct node *head=NULL;
     
   printf("Process linked list of length %d\n",N);
   printf("Each node will be processed by function 'processwork()'\n");
   printf("We will compute fibonacci numbers starting at %d\n",FS);
 
   p = init_list(p);
   head = p;

   // traverse the list process work for each node
   start = omp_get_wtime();

#define TASKS
#ifdef TASKS
#pragma omp parallel
{
#pragma omp single
  {
    while (p != NULL) {
#pragma omp task firstprivate(p)
      processwork(p);
      p = p->next;
    }
  }
}
#endif

#ifndef TASKS
#define OPT1
#ifdef OPT1
#pragma omp parallel firstprivate(p)
{
  int thread_id = omp_get_thread_num();
  int nthreads = omp_get_num_threads();
  int i = 0;

  while (p != NULL) {
    if (i % nthreads == thread_id) {
      processwork(p);
    }
    p = p->next;
    i++;
  }
}
#endif

#ifndef OPT1
#pragma omp parallel
{
  struct node *thread_p;
  while (p != NULL) {
#pragma omp critical
    {
      thread_p = p;
      if (p != NULL) p = p->next;
    }
    if (thread_p != NULL) processwork(thread_p);
  }
}
#endif
#endif

   end = omp_get_wtime();

   // traverse the list releasing memory allocated for the list
   p = head;
   while (p != NULL) {
      printf("%d : %d\n",p->data, p->fibdata);
      temp = p->next;
      free (p);
      p = temp;
   }  
   free (p);
   printf("Compute Time: %f seconds\n", end - start);
   return 0;
}

