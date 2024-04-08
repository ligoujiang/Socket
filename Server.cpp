#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

int Cli_num=0;
int Cli_socks[10];


//接收数据
void* Recv(void* arg){
	int Cli_num2=*((int*)arg);
	char buff[1024];
	while(1){
		int rec=recv(Cli_socks[Cli_num2],buff,sizeof(buff),0);
		if(rec<=0){
			close(Cli_socks[Cli_num2]);
			return NULL;
		}
		char *msg=buff;
		std::cout<<"客户端"<<Cli_socks[Cli_num2]<<Cli_num<<"发回的消息："<<buff<<std::endl;
		for(int i=0;i<5;i++){
			if(i==Cli_num2){
				continue;
			}
			send(Cli_socks[i],msg,sizeof(msg),0);
		}
	}
	close(Cli_socks[Cli_num2]);
	return NULL;
}



int main() {
	int Ser{};
	Ser = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in SerAddr {};
	SerAddr.sin_family = AF_INET;
	SerAddr.sin_addr.s_addr = INADDR_ANY;
	SerAddr.sin_port = htons(6666);
	bind(Ser,(sockaddr*)&SerAddr,sizeof(SerAddr));
	listen(Ser,5);

	pthread_t th1;
	pthread_t th2;
	int Cli{};
	struct sockaddr_in CliAddr {};

    while(1){
        socklen_t Cli_len = sizeof(CliAddr);
        Cli_socks[Cli_num]=accept(Ser,(sockaddr*)&CliAddr,&Cli_len);
        char ip[32];
	    std::cout << "客户端ip:" << inet_ntop(AF_INET, &CliAddr.sin_addr.s_addr, ip, sizeof(ip)) << "端口：" << ntohs(CliAddr.sin_port) << std::endl;
        pthread_create(&th1,NULL,Recv,&Cli_num);
        pthread_detach(th1);
		Cli_num++;
    }    
	close(Ser);
	return 0;
}