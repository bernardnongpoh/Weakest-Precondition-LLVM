//
// Created by bernard on 5/7/16.
//
#include <stdio.h>
#include "wp_annotation.h"
void multiplyElementBy(int *arr,int no,int a);
struct my{
    int a;
    int b;
};

int main(){
int arr[5]={3,4,3,2,5};


multiplyElementBy(arr,2,10);

}

void multiplyElementBy(int *arr,int no,int a,struct my myy){
    int i=a;
    int *b;





    b=&i;
    arr[1]=no*myy.a;

     _wp_begin();
    myy.a=arr[1]*a;
    myy.b=arr[2];
    printf("%d\n",myy.b);
            arr[1]=no*a;

        _wp_end("(wp_post> 8 && wp_post<10)");

}

