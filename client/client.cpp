#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32")
#pragma warning(disable: 4996)

#define PORT_NUM 80
#define MAX_MSG_LEN 1024
#define BUF_SIZE 4096
#define MAX_SIZE 256
#define SERVER_IP "192.168.219.104"

const char* requestFormat = "%s /%s HTTP/1.1\r\n"
							"Host: %s\r\n"
							"\r\n"
							"%s";

char requestBuffer[BUF_SIZE] = "";
char rcvBuffer[BUF_SIZE] = "";

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


	char METHOD[BUF_SIZE] = "";
	char DATA[BUF_SIZE] = "";
	char URI[BUF_SIZE] = "";


	printf("요청을 입력하세요 GET/PUT/POST/HEAD\n");
	gets_s(METHOD, MAX_MSG_LEN);

	if (strcmp("GET", METHOD) == 0)
	{
		printf("         URI를 입력하세요\n");
		gets_s(URI, MAX_MSG_LEN);
		sprintf(requestBuffer, requestFormat, "GET", URI, SERVER_IP, "");
	}
	else if (strcmp("PUT", METHOD) == 0)
	{
		printf("Input Data : ");
		gets_s(DATA, MAX_MSG_LEN);
		sprintf(requestBuffer, requestFormat, "PUT", "", SERVER_IP, DATA);
	}
	else if (strcmp("POST", METHOD) == 0)
	{
		printf("Input Data : ");
		gets_s(DATA, MAX_MSG_LEN);
		sprintf(requestBuffer, requestFormat, "POST", "", SERVER_IP, DATA);
	}
	else if (strcmp("HEAD", METHOD) == 0)
	{
		printf("         URI를 입력하세요\n");
		gets_s(URI, MAX_MSG_LEN);
		sprintf(requestBuffer, requestFormat, "HEAD", URI, SERVER_IP, "");
	}
	else
	{
		sprintf(requestBuffer, requestFormat, METHOD, "", SERVER_IP, "");
		send(sock, requestBuffer, strlen(requestBuffer), 0);
		recv(sock, rcvBuffer, sizeof(rcvBuffer), 0);
	}

	send(sock, requestBuffer, strlen(requestBuffer), 0);
	recv(sock, rcvBuffer, sizeof(rcvBuffer), 0);

	printf("\n    Server response\n-----------------------\n%s\n\n\n", rcvBuffer);
	closesocket(sock);
	WSACleanup();
	return 0;
}
