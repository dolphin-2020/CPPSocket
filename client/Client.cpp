#include "TcpClient.hpp"
#include<thread>

using namespace std;
#define _WINSOCK_DEPRECATED_NO_WARNINGS

void threadCMD(TcpClient *client);
int main()
{
	TcpClient tc;
	tc.InitSocket();
	tc.Connect("127.0.0.1", 4567);

	//Thread
	std::thread t1(threadCMD, &tc);
	t1.detach();
	while (tc.IsRun())
	{
		tc.onRun();
	}
	tc.Close();
	std::cout << "end socket";
	system("pause");
	return 0;
}

void threadCMD(TcpClient* client)
{
	while (true) {
		char cmdBuff[4096] = {};
		cout << "Please Enter Your Commend" << endl;
		cin >> cmdBuff;
		if (0 == strcmp(cmdBuff, "exit"))
		{
			client->Close();
			cout << "Exit" << endl;
			return;
		}
		else if (0 == strcmp(cmdBuff, "login"))
		{
			Login l;
			strcpy(l.name, "Lia");
			strcpy(l.pwd, "111111");
			client->sendData(&l);
		}
		else if (0 == strcmp(cmdBuff, "logout"))
		{
			Logout lo;
			strcpy(lo.userName, "Lia");
			client->sendData(&lo);
		}
		else
		{
			cout << "Bad commend,Please reType" << endl;
			continue;
		}
	}
}





















//#ifdef _WIN32 //use to for windodws
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include<WinSock2.h>
//#pragma comment (lib,"ws2_32.lib")
//#include<string.h>
//#include<iostream>
//#else//use to for mac or linux
//#include<unistd.h>
//#include<arpa/inet.h>
//#include<string.h>
//#include<iostream>
//
//#define SOCKET int
//#define INVALID_SOCKET (SOCKET)(~0)
//#define SOCKET_ERROR(-1)
//#endif
//#include<thread>
//using namespace std;
//int processor(SOCKET cSock);
//void threadCMD(SOCKET sock);
//enum CMD
//{
//	LOGIN,
//	LOGIN_RET,
//	LOGOUT,
//	LOGOUT_RET,
//	ERROR_INFO,
//	NEW_JOIN
//};
//struct Header
//{
//	int len;
//	CMD cmd;
//};
//struct Login :public Header
//{
//	Login()
//	{
//		len = sizeof(Login);
//		cmd = LOGIN;
//	}
//
//	char name[30];
//	char pwd[30];
//};
//struct Logout :public Header
//{
//	Logout()
//	{
//		len = sizeof(Logout);
//		cmd = LOGOUT;
//	}
//	char userName[30];
//};
//struct LoginRet :public Header
//{
//	LoginRet()
//	{
//		len = sizeof(LoginRet);
//		cmd = LOGIN_RET;
//		inRet = 0;
//	}
//	int inRet;
//};
//struct LogoutRet :public Header
//{
//	LogoutRet()
//	{
//		len = sizeof(LogoutRet);
//		cmd = LOGOUT_RET;
//		outRet = 0;
//	}
//	int outRet;
//};
//
//struct NewJoin :public Header
//{
//	NewJoin()
//	{
//		len = sizeof(NewJoin);
//		cmd = NEW_JOIN;
//		sock = 0;
//	}
//	int sock;
//};
//bool isExit = false;
//
//int main()
//{
//
//#ifdef _WIN32
//	WORD verson = MAKEWORD(2, 2);
//	WSADATA data;
//	WSAStartup(verson, &data);
//#endif
//
//#pragma region MyRegion
//
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (INVALID_SOCKET == sock)
//	{
//		std::cout << "Wrong Link With Sock" << std::endl;
//	}
//	else
//	{
//		std::cout << "success connect sock" << std::endl;
//	}
//	sockaddr_in sin = {};
//	sin.sin_family = AF_INET;
//	sin.sin_port = htons(4567);
//#ifdef _WIN32
//	sin.sin_addr.S_un.S_addr = inet_addr("192.168.100.101");
//#else
//	_sin.sin_addr.s_addr = inet_addr(ip);
//#endif
//	int ret = connect(sock, (sockaddr*)&sin, sizeof(sockaddr_in));
//
//	if (SOCKET_ERROR == ret)
//	{
//		std::cout << "connect Failed" << std::endl;
//	}
//	else
//	{
//		std::cout << "success" << std::endl;
//	}
//#pragma endregion
//
//	//Thread
//	std::thread t1(threadCMD, sock);
//	t1.join();
//	while (!isExit)
//	{
//		fd_set fdRead;
//		FD_ZERO(&fdRead);
//		FD_SET(sock, &fdRead);
//		timeval t = { 1,0 };
//		int ret = select(sock, &fdRead, 0, 0, &t);
//		if (ret < 0)
//		{
//			cout << "Mission End" << endl;
//			break;
//		}
//		if (FD_ISSET(sock, &fdRead))
//		{
//			FD_CLR(sock, &fdRead);
//			if (-1 == processor(sock))
//			{
//				cout << "Mission End" << endl;
//				break;
//			}
//		}
//		//Use thread
//
//		//cout << "deal with another mission when have leture time" << endl;
//	}
//#ifdef _WIN32
//	closesocket(sock);
//	WSACleanup();
//#else
//	close(sock);
//#endif
//	std::cout << "end socket";
//	return 0;
//}
//
//int processor(SOCKET sock)
//{
//	char cache[4096];//
//	Header* h = (Header*)cache;
//	int r = recv(sock, cache, sizeof(Header), 0);
//	if (r <= 0)
//	{
//		cout << "Disconnect" << endl;
//		return -1;
//	}
//	switch (h->cmd)
//	{
//	case LOGIN_RET:
//	{
//		recv(sock, cache + sizeof(Header), h->len - sizeof(Header), 0);
//		LoginRet* logret = (LoginRet*)cache;
//		cout << "Receive server data length:" << h->len << endl;
//	}
//	break;
//	case LOGOUT_RET:
//	{
//		recv(sock, cache + sizeof(Header), h->len - sizeof(Header), 0);
//		LogoutRet* logoutmsg = (LogoutRet*)cache;
//		cout << "Receive server logout data length:" << h->len << endl;
//	}
//	break;
//	case NEW_JOIN:
//	{
//		recv(sock, cache + sizeof(Header), h->len - sizeof(Header), 0);
//		NewJoin* newJoin = (NewJoin*)cache;
//		cout << "Receive server logout data length:" << h->len << endl;
//	}
//	break;
//	default:
//	{
//
//	}
//	break;
//	}
//	return 0;
//}
//
//
//void threadCMD(SOCKET sock)
//{
//	while (true) {
//		char cmdBuff[256] = {};
//		cout << "Please Enter Your Commend" << endl;
//		cin >> cmdBuff;
//		if (0 == strcmp(cmdBuff, "exit"))
//		{
//			cout << "Exit" << endl;
//			isExit = true;
//			return;
//		}
//		else if (0 == strcmp(cmdBuff, "login"))
//		{
//			Login l;
//			strcpy(l.name, "Lia");
//			strcpy(l.pwd, "111111");
//			send(sock, (const char*)&l, l.len, 0);
//		}
//		else if (0 == strcmp(cmdBuff, "login"))
//		{
//			Logout lo;
//			strcpy(lo.userName, "Lia");
//			send(sock, (const char*)&lo, lo.len, 0);
//		}
//		else
//		{
//			cout << "Bad commend,Please reType" << endl;
//			continue;
//		}
//	}
//}