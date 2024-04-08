#include <iostream>
#include <sys/uio.h>

int main(){
    struct iovec vec[2];
    char buff1[]="ABCDEFG";
    char buff2[]="123456";
    vec[0].iov_base=buff1;
    vec[0].iov_len=5;
    vec[1].iov_base=buff2;
    vec[1].iov_len=3;
    int str_len=writev(1,vec,2);
    std::cout<<std::endl;
    
    struct iovec vec1[2];
    int max=10;
    char buff11[max]={0};
    char buff22[max]={0};
    vec1[0].iov_base=buff11;
    vec1[0].iov_len=2;
    vec1[1].iov_base=buff22;
    vec1[1].iov_len=max;
    int str_len2=readv(0,vec1,2);
    std::cout<<buff11<<std::endl;
    std::cout<<buff22<<std::endl;
    return 0;   
}