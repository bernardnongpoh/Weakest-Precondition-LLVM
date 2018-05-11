#include <stdio.h>
__attribute__((noinline))
void  _wp_begin(){
	printf(".");

}

__attribute__((noinline))
const char * _wp_end(const char *a){
	printf(".");
	return a;
}
