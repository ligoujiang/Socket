#include <arpa/inet.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

int main() {
	int Ser{};
	int Cli{};
	struct sockaddr_in CliAddr {};
	Ser = socket(AF_INET,SOCK_STREAM,0);
    if(Ser==-1){
        std::cout<<"socket error!"<<std::endl;
        return -1;
    }
	struct sockaddr_in SerAddr {};
	SerAddr.sin_family = AF_INET;
	SerAddr.sin_addr.s_addr = INADDR_ANY;
	SerAddr.sin_port = htons(6666);
	int rec=bind(Ser,(sockaddr*)&SerAddr,sizeof(SerAddr));
    if(rec==-1){
        std::cout<<"bind error!"<<std::endl;
        return -1;
    }
	rec=listen(Ser,5);
    if(rec==-1){
        std::cout<<"listen error!"<<std::endl;
        return -1;
    }
    //存储客户端描述符
    std::vector<int> Clis;

    //epoll模式
    int epfd=epoll_create(100); //创建epoll实例
    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd=Ser;
    epoll_ctl(epfd,EPOLL_CTL_ADD,Ser,&ev);//添加文件描述符

    struct epoll_event evs[1024];
    int size=sizeof(evs)/sizeof(evs[0]);
    while(true){
        int num=epoll_wait(epfd,evs,size,-1);    //返回值为触发事件的描述符总数
        //std::cout<<"通知有数据！"<<std::endl;
        for(int i=0;i<num;i++){
            if(evs[i].data.fd==Ser){
                socklen_t Cli_len=sizeof(sockaddr_in);
                int Cli=accept(evs[i].data.fd,(sockaddr*)&CliAddr,&Cli_len);
                ev.events=EPOLLIN; //内核会拷贝ev数据
                ev.data.fd=Cli;
                epoll_ctl(epfd,EPOLL_CTL_ADD,Cli,&ev);//添加文件描述符
                Clis.push_back(Cli);
                char ip[32];
				std::cout << "客户端ip:" << inet_ntop(AF_INET, &CliAddr.sin_addr.s_addr, ip, sizeof(ip)) << "端口：" << ntohs(CliAddr.sin_port) << std::endl;
            }else{
                char buff[100];
                int rec=recv(evs[i].data.fd,buff,sizeof(buff),0);
                    if(rec<=0){
                        std::cout<<evs[i].data.fd<<"已断开连接！"<<std::endl;
                        epoll_ctl(epfd,EPOLL_CTL_DEL,evs[i].data.fd,NULL);
                        close(evs[i].data.fd);
                        continue;
                    }
                    std::cout<<buff<<std::endl;
                    //小写转大写

                    // for(int i=0;i<sizeof(buff);i++){
                    //     buff[i]=toupper(buff[i]);
                    // } 

                    //循环转发消息给所以客户端
                    for(auto it:Clis){
                        if(it==evs[i].data.fd) {
                             continue;    
                         }
                        send(it,buff,sizeof(buff)+1,0);
                    }                                 
            }
        }
    }
    close(Ser);
    return 0;
}