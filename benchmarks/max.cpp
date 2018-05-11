
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
      //perform some dummy operation
      max = a;

  }
  else {

      max = b;

  }
    return max; // max>=30

}

