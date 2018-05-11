
//
// Created by bernard on 15/6/16.
//
#include <stdio.h>
int max(int a,int b);
int main(){

int a=10;
    int b=20;
        return max(a,b);
}

int max(int a,int b){
    int max;
  if(a>b) {
      a = a + 10;
      a = a + 1;
      max = a;
      printf("If Part");
  }
  else {
      b=b+100;
      b=b+13;
      max = b;
     printf("Else Part");
  }
    return max; // max>=30

}

