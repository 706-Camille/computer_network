# server
'''
//common.h
#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#pragma comment(lib,"ws2_32")
#pragma warning(disable: 4996)

//program.cpp
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
'''

# client
'''
// common.h
#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32")
#pragma warning(disable: 4996)'

// program.cpp
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
'''


