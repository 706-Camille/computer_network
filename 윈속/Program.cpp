#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256

SOCKET SetTCPServer(short pnum, int blog);
void acceptLoop(SOCKET sock);
void Doit(SOCKET dosock);

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); // Winsock 초기화
	SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE); // 소켓 대기
	acceptLoop(sock);
	WSACleanup();
	return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
	
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 소켓 생성
	// 네트워크 인터페이스와 소켓 결합
	SOCKADDR_IN serveraddr = { 0 }; // 서버주소
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT_NUM);
	int re = 0;
	re = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (re == -1) { return -1; } // -1 => 0xFFFFFFFF => SOCKET_ERROR
	re = listen(sock, blog);
	if (re == -1) { return -1; } // -1 => 0xFFFFFFFF => SOCKET_ERROR
	printf("%s:%d Setup\n", inet_ntoa(serveraddr.sin_addr), pnum);
	return sock;
}

void acceptLoop(SOCKET sock)
{
	SOCKET dosock;
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	while (true)
	{
		dosock = accept(sock, (SOCKADDR*)&cliaddr, &len);
		if (dosock == -1)
		{
			perror("accept fail");
			break;
		}
		printf("%s:%d의 연결 요청 수락\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
		Doit(dosock);
	}
}

void Doit(SOCKET dosock)
{
	char msg[MAX_MSG_LEN] = "";
	while (recv(dosock, msg, sizeof(msg), 0) > 0)
	{
		printf("recv:%s\n", msg);
		send(dosock, msg, sizeof(msg), 0);
	}
	closesocket(dosock);

}


