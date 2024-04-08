#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>



//发送数据
void* Send(void* arg){
	int Cli=*((int*)arg);
	char buff[20];
	while(1){
		std::cin>>buff;
		send(Cli,buff,sizeof(buff),0);
	}
	close(Cli);
	return NULL;
}
//接收数据
void* Recv(void* arg){
	int Cli=*((int*)arg);
	char buff[1024];
	while(1){
		int rec=recv(Cli,buff,sizeof(buff),0);
		if(rec<=0){
			close(Cli);
			return NULL;
		}
	 	std::cout<<"用户发回的消息："<<buff<<std::endl;
	}
	close(Cli);
	return NULL;
}

int main() {
	int Cli{};
	Cli = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in SerAddr {};
	SerAddr.sin_family = AF_INET;
	SerAddr.sin_addr.s_addr = inet_addr("10.19.201.134");//10.19.9.195
	SerAddr.sin_port = ntohs(6666);

	connect(Cli,(sockaddr*)&SerAddr,sizeof(SerAddr));
	char ip[32];
	std::cout << "服务端ip:" << inet_ntop(AF_INET, &SerAddr.sin_addr.s_addr, ip, sizeof(ip)) << "端口:" << ntohs(SerAddr.sin_port) << std::endl;

	pthread_t th1;
	pthread_t th2;
	pthread_create(&th1,NULL,Send,&Cli);
	pthread_create(&th2,NULL,Recv,&Cli);

	pthread_detach(th1);
	pthread_detach(th2);
	
	pthread_exit(NULL);

	close(Cli);
	return 0;
}












