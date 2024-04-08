#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
int main() {
	int Ser{};
	int Cli{};
	struct sockaddr_in CliAddr {};
	Ser = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in SerAddr {};
	SerAddr.sin_family = AF_INET;
	SerAddr.sin_addr.s_addr = INADDR_ANY;
	SerAddr.sin_port = htons(6666);
	bind(Ser,(sockaddr*)&SerAddr,sizeof(SerAddr));
	listen(Ser,5);


	fd_set reads,copyreads;
	FD_ZERO(&reads);
	FD_SET(Ser,&reads);
	int fd_max=Ser;
	int fd_num;
	struct timeval timeout;

	while(true){
		copyreads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=0;
		if(fd_num=select(fd_max+1,&copyreads,0,0,&timeout)==0){
			continue;
		}
		for(int i=0;i<fd_max+1;i++){
			if(FD_ISSET(i,&copyreads)){
				if(i==Ser){
				socklen_t Cli_len = sizeof(CliAddr);
				Cli=accept(Ser,(sockaddr*)&CliAddr,&Cli_len);
				FD_SET(Cli,&reads);
				if(fd_max<Cli){
					fd_max=Cli;
					char ip[32];
					std::cout << "客户端ip:" << inet_ntop(AF_INET, &CliAddr.sin_addr.s_addr, ip, sizeof(ip)) << "端口：" << ntohs(CliAddr.sin_port) << std::endl;
				}
			}else{
				char buff[20];
				int rec = recv(Cli,buff,sizeof(buff),0);
				if (rec<=0) {
					FD_CLR(i,&reads);
					close(i);
					break;
				}else {
					send(Cli,buff,sizeof(buff),0);			
					std::cout << buff << std::endl;
				}
			}
			}
		}
	}
	close(Ser);
	return 0;
}