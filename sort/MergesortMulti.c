//Merge sort using multiple threads,at most 2^k threads are created where k is the number of threads passed as argument
#include "header.h"
typedef struct{
    int first_param;
    int second_param;
    int return_value;
    int current_thread;
    int *arr;
}thread_argument;

pthread_t t1;
pthread_attr_t attr1;
int *arr;
sem_t sem;
int max_threads;

void merge(int *arr, int l, int m, int r) 
{
    int i, j, k;
    int *result=malloc((r-l+1)*sizeof(int));
    i = l;
    j = m + 1;
    k = 0;
    while (i <= m && j <= r) 
    {
        if (arr[i] < arr[j]) 
        {
            result[k] = arr[i];
            i++;
        } else 
        {
            result[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i <= m) 
    {
        result[k] = arr[i];
        i++;
        k++;
    }
    while (j <= r) 
    {
        result[k] = arr[j];
        j++;
        k++;
    }
    for (int i = l; i <= r; i++)
    {
        arr[i] = result[i-l];
    }
    free(result);
}


void *mergersort(void *arguement) 
{
    thread_argument *my_arg = (thread_argument *)arguement;
    int mid = my_arg->first_param + (my_arg->second_param - my_arg->first_param) / 2;
    
    if (my_arg->first_param < my_arg->second_param) 
    {
        thread_argument arg1,arg2;
        arg1.first_param = my_arg->first_param;
        arg1.second_param = mid;
        arg1.arr = my_arg->arr;
        arg1.current_thread = my_arg->current_thread + 1;
        arg2.first_param = mid + 1;
        arg2.second_param = my_arg->second_param;
        arg2.current_thread = my_arg->current_thread + 1;
        arg2.arr = my_arg->arr;
        pthread_t t2;
        if (my_arg->current_thread < max_threads)
        {
            if (pthread_create(&t2, &attr1, mergersort, &arg1))
            {
                printf("Error in creating thread\n");
                exit(-1);
            }
            mergersort(&arg2);
            if (pthread_join(t2, NULL))
            {
                printf("Error in joining thread\n");
                exit(-1);
            }
        }
        else
        {
            mergersort(&arg1);
            mergersort(&arg2);
        }
        merge(my_arg->arr, my_arg->first_param, mid, my_arg->second_param);
    }
    return NULL;
}


int main(int argc, char **argv) {

   // FILE *fptr;
   // fptr = freopen("1e1_nums.txt", "r", stdin);
    int n;
    if (scanf("%d\n", &n) != 1) {
        fprintf(stderr, "Invalid n\n");
        exit(1);
    }
    arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Invalid arr[%d]\n", i);
            exit(1);
        }
    }
    max_threads =(atoi(argv[1]));
    /*sem_init(&sem, 0, max_threads);
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&attr1, PTHREAD_SCOPE_SYSTEM);*/
    thread_argument arg;
    arg.first_param = 0;
    arg.second_param = n - 1;
    arg.current_thread = 0;
    int rc;
  /* struct timeval start, end;
    struct timezone tzp;
    struct tm *tm;
    
    double sum=0;
    for (int i=0;i<20;i++)
   {*/
        arg.arr = arr;
       // gettimeofday(&start, &tzp);
        rc=pthread_create(&t1, &attr1, mergersort,&arg);
        if (rc) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        void *status;
        rc=pthread_join(t1, &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }   
        //gettimeofday(&end, &tzp);
        //sum+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))/1;
        
   // }
   // printf("Average time taken by 20 runs:%f\n",sum/20);
   // FILE *fptr1;
   // fptr1 = freopen("out", "w", stdout);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arg.arr[i]);
    }
    free(arr);
   // fclose(fptr);
   // fclose(fptr1);
    return 0;
}