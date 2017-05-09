#ifndef __CLIENT__
#define __CLIENT__

#include <sys/socket.h>
#include <netinet/in.h>
#include <android/log.h>
#include <arpa/inet.h>
#include "UserData.hpp"
#include <sys/select.h>
#include <unistd.h>

/**
@brief
*/
class Client
{
	public:
		Client();
		struct sockaddr_in sock_add;
		struct sockaddr_in server_addr;
		socklen_t server_addr_length;
		int sockfd;
		void connectRemote();
		static Client* client;
};
// onserverField "init" you need to initialize your instance
Client* Client::client=new Client;
Client::Client(){

}
void Client::connectRemote(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0){
		return;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8079);
	server_addr.sin_addr.s_addr = inet_addr(UserData::current->server);
	bzero(&(server_addr.sin_zero), 8);

	server_addr_length =sizeof(server_addr);
	int connfd = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(connfd < 0){
		return;
	}

	//this->setTouchEnabled(true);
	return;
}
#endif
