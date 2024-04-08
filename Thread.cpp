#include <pthread.h>
#include <iostream>

void* callback(void* arg){
    int  *a=(int*)arg;
    std::cout<<*a<<"子线程ID："<<pthread_self()<<std::endl;
    return NULL;
}

int main(){
    pthread_t th;
    int a=2;
    pthread_create(&th,NULL,callback,&a);
    std::cout<<"主线程ID："<<pthread_self()<<std::endl;
    pthread_exit(NULL);
    return 0;
}