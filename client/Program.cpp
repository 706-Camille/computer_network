#include "common.h"
#define PORT_NUM 10200
#define MAX_MSG_LEN 256
#define SERVER_IP "192.168.219.104"
int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		perror("소켓 생성 실패");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN servaddr = { 0 }; // 서버 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT_NUM);
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	int re;
	re = connect(sock, (SOCKADDR*)&servaddr, sizeof(servaddr)); // connect 시도
	if (re == -1) { return -1; }

	char msg[MAX_MSG_LEN] = "";

	while (true)
	{
		gets_s(msg, MAX_MSG_LEN);
		send(sock, msg, sizeof(msg), 0);
		if (strcmp(msg, "exit") == 0)
		{
			break;
		}
		recv(sock, msg, sizeof(msg), 0);
		printf("수신:%s\n", msg);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}