//simple merge sort
#include "header.h"

void merge(int *arr, int l, int m, int r) {
    int i, j, k;
    int *result=malloc((r-l+1)*sizeof(int));
    i = l;
    j = m + 1;
    k = 0;
    while (i <= m && j <= r) {
        if (arr[i] < arr[j]) {
            result[k] = arr[i];
            i++;
        } else {
            result[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i <= m) {
        result[k] = arr[i];
        i++;
        k++;
    }
    while (j <= r) {
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

void mergersort(int *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergersort(arr, l, m);
        mergersort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main(int argc, char **argv) {
    
    int n;
    int *arr;
    if (scanf("%d\n", &n) != 1) {
        fprintf(stderr, "Invalid n\n");
        exit(1);
    }
    arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Invalid arr[%d]\n", i);
            exit(1);
        }
    }
    mergersort(arr, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    free(arr);
    return 0;
}