#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<int> Arr = [4,2,6,9,1,5];

void Merge(arr, start, mid, end)
{
    n_left = mid+1 - start
    n_right = end - mid

    L(n_left)
    R(n_right) // trzeb ado nich skopiować z arraya

    a = start
    i = start
    j = mid+1

    while(i <= mid && j <= end)
    {
        if(L[i] < R[j])
        {
            arr[a] = L[i]
            i++;
            a++;
        }
        else
        {
            arr[a] = R[j]
            j++;
            a++;
        }
    }
    if(n_right > n_left)
    {
        while(j <= end)
        {
            arr[a] = R[j]
            j++;
            a++;
        }
    }
    else if (n_right < n_left)
    {
        while(i <= mid)
        {
            arr[a] = L[i]
            i++;
            a++;
        }
    }
}


MergeSort(arr, start, end)
{
    if (start >= end) return
    mid = start + (end-start)/2
    MergeSort(arr, start, mid)
    MergeSort(arr, mid+1, end)
    Merge(arr, start, mid, end)
}



QuickSort(arr, start, end)
{
    if (start >= end) return
    pivot = rand(start, end)
    newPivot = Partition(arr, start, end, pivot)
    QuickSort(arr, start, newPivot-1)
    QuickSort(arr, newPivot+1, end)
}



int Partition(arr, start, end, pivot)
{
    pivot_value = arr[pivot]
    i = start
    j = end
    while(i < j)
    {
        if(arr[i] > pivot_value && arr[j] < pivot_value)
        {
            temp = arr[i]
            arr[i] = arr[j]
            arr[j] = temp
            i++
            j--
        }
        else if (arr[i] <= pivot_value) i++
        else if(arr[j] >= pivot_value) j--
    }
    return newPivot = i
}




InsertionSort(Tab)
    for(i=1; i< Tab.size; i++)
        j <- i
        while (j>0 && Tab[j] < Tab[j-1])
        {
            swap(Tab[j] ,Tab[j-1])
            j--
        }


MergeSort(T, start, end)
{
        if(start == end) return;
        mid = start + (end - start)/2
        MergeSort(T, start, mid)
        MergeSort(T, mid+1, end)
        T = Merge(T)
}

tab Merge(T, start, mid, end)
{
    a <- 0
    b <- 0
    c <- 0

    while (a < A.size && b < B.size)
    {
        if(A[a] <= B[b])
        {
            C[c] <- A[a]
            a++
            c++
        }
        else if(B[b] < A[a])
        {
            C[c] <- B[b]
            b++
            c++
        }
    }

    while (a < A.size)
    {
        C[c] <- A[a]
        a++
        c++
    }
    while (b < B.size)
    {
        C[c] <- B[b]
        b++
        c++
    }

    return C
}



QuickSort(Tab, start, end)
{
    if(start==end) return;
    pivot = start + (end - start)/2
    Partition(Tab, start, end)
    QuickSort(Tab, start, pivot)
    QuickSort(Tab, pivot+1, end)
}

Partition(Tab, start, end)
{
    i <- start
    j <- end
    while (i<j)
    {
        if(T[i]>T[j])
        {
            swap(T[i], T[j])
            i++
        }
        else
        {
            i++
            j--
        }
    }
}
