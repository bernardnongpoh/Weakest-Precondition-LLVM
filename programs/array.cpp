//
// Created by bernard on 5/7/16.
//
#include <stdio.h>
void multiplyElementBy(int *arr,int no);

int main(){
int arr[5]={3,4,3,2,5};
multiplyElementBy(arr,2);

}

void multiplyElementBy(int *arr,int no){
    int i;
     for(i=0;i<5;i++)
        arr[i]=arr[i]*no;


}

