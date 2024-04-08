#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>


class base{
public:
	int sock;
	std::vector<int> Clis;

};

//接收数据
void* Recv(void* arg){
	base cli=*((base*)arg);
	char buff[1024];
	while(1){
		int rec=recv(cli.sock,buff,sizeof(buff),0);
		if(rec<=0){
			close(cli.sock);
			return NULL;
		}
		char *msg=buff;
		std::cout<<"客户端"<<"发回的消息："<<buff<<std::endl;
		for(auto it:cli.Clis){
			if(it==cli.sock) continue;
			send(it,msg,sizeof(msg),0);
		}
	}
	close(cli.sock);
	return NULL;
}



int main() {
	int Ser{};
	Ser = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in SerAddr {};
	SerAddr.sin_family = AF_INET;
	SerAddr.sin_addr.s_addr = INADDR_ANY;
	SerAddr.sin_port = htons(6666);
	if(bind(Ser,(sockaddr*)&SerAddr,sizeof(SerAddr))==-1){
		std::cout<<"error"<<std::endl;
	}
	listen(Ser,5);

	pthread_t th1;
	pthread_t th2;
	int Cli{};
	struct sockaddr_in CliAddr {};

	base *clis=new base;

    while(1){
        socklen_t Cli_len = sizeof(CliAddr);
        clis->sock=accept(Ser,(sockaddr*)&CliAddr,&Cli_len);
		clis->Clis.push_back(Cli);
        char ip[32];
	    std::cout << "客户端ip:" << inet_ntop(AF_INET, &CliAddr.sin_addr.s_addr, ip, sizeof(ip)) << "端口：" << ntohs(CliAddr.sin_port) << std::endl;
        pthread_create(&th1,NULL,Recv,clis);
        //pthread_detach(th1);
		pthread_join(th1,NULL);
    }    
	close(Ser);
	return 0;
}