//
// Created by bernard on 13/6/16.
//

int max(int a,int b){
    if(a>b)
        return a;
    else
        return b;
}

int main(){
    int i;
    for(i=0;i<10;i++){
        int x=i;
        i=x;
    }

    for(i=0;i<10;i++){
        int x=i;
        i=x;
    }

    int a=max(4,6);

}