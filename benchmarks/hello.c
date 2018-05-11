#include "wp_annotation.h"
#include <assert.h>
int hello(int a,int b, int c);
int main(){
int a=10;
a=hello(4,5,6);
    return a;
}

int hello(int a,int b, int c){

    _wp_begin();
    a=5+a+b;
    b=a*b;
    c=b*c;
    c=a*b*c;
    _wp_end("119");
    assert(("C should be greater than 45", c >= 45));
return c;
}
