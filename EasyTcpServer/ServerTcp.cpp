#include "TcpServer.hpp"
#include <iostream>
int main()
{
	TcpServer s;
	s.InitSocket();
	s.Bind(nullptr, 4567);
	s.Listen(5);
	while (s.IsRun())
	{
		s.OnRun();
		//cout << "deal with other thing...\n";
	}
	s.Close();
	system("pause");
}


//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include<WinSock2.h>
//#include<iostream>
//#include<vector>
//
//using namespace std;
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//vector<SOCKET> g_clients;
//
//#pragma comment (lib,"ws2_32.lib")
//int processor(SOCKET cSock);
//#pragma region MyRegion
//
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
//struct Login:public Header
//{ 
//	Login()
//	{
//		len = sizeof(Login);
//		cmd = LOGIN;
//	}
//	char name[30];
//	char pwd[30];
//};
//struct Logout:public Header
//{
//	Logout()
//	{
//		len = sizeof(Logout);
//		cmd = LOGOUT;
//	}
//	char userName[30];
//};
//struct LoginRet:public Header
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
//int main()
//{
//
//	WORD verson = MAKEWORD(2, 2);
//	WSADATA data;
//	WSAStartup(verson, &data);
//	///
//
//
//	//create socket
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	//bind
//	sockaddr_in sin = {};
//	sin.sin_family = AF_INET;
//	sin.sin_port = htons(4567);
//	sin.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");INADDR_ANY;
//
//	if (SOCKET_ERROR == bind(sock, (sockaddr*)&sin, sizeof(sin)))
//	{
//		std::cout << "bind failed" << std::endl;
//	}
//	else
//	{
//		std::cout << "bind success" << std::endl;
//	}
//
//	if (SOCKET_ERROR == listen(sock, 5))
//	{
//		std::cout << "Listen error" << std::endl;
//	}
//	else
//	{
//		std::cout << "Listen success" << std::endl;
//	}
//#pragma endregion
//
//	while (true)
//	{
//		// BSD socket
//		fd_set fdRead;
//		fd_set fdWrite;
//		fd_set fdExp;
//		//clean set
//		FD_ZERO(&fdRead);
//		FD_ZERO(&fdWrite);
//		FD_ZERO(&fdExp);
//		//put discript symbol in to set
//		FD_SET(sock,&fdRead);
//		FD_SET(sock,&fdWrite);
//		FD_SET(sock,&fdExp);
//
//		for (int i = (int)g_clients.size()-1; i >=0 ;--i)
//		{
//			FD_SET(g_clients[i], &fdRead);
//		}
//		timeval t = { 1,0 };
//		int ret=select(sock + 1, &fdRead, &fdWrite, &fdExp, &t);
//		if (ret < 0)
//		{
//			cout << "End socket with " << endl;
//			break;
//		}
//		//determine  fdread if in the set
//		if (FD_ISSET(sock, &fdRead))
//		{
//			FD_CLR(sock, &fdRead);
//			sockaddr_in clientAdd = {};
//			int nAddlen = sizeof(sockaddr_in);
//			SOCKET cSock = INVALID_SOCKET;
//			cSock = accept(sock, (sockaddr*)&clientAdd, &nAddlen);
//
//			if (INVALID_SOCKET == cSock)
//			{  
//				std::cout << "invailed socket" << std::endl;
//			}
//			else
//			{
//				for (int i = g_clients.size() - 1; i >= 0; --i)
//				{
//					NewJoin newJoin = {};
//					send(g_clients[i], (const char*)&newJoin, newJoin.len, 0);
//				}
//				cout << "New client enter socket is:" << (int)cSock
//					<< " IP:" << inet_ntoa(clientAdd.sin_addr) << endl;
//				g_clients.push_back(cSock);
//			}
//		}
//
//		for (int i =0;i<(int)fdRead.fd_count; i++)
//		{
//			if (-1 == processor(fdRead.fd_array[i]))
//			{
//				auto iter = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[i]);
//				if (iter != g_clients.end())
//				{
//					g_clients.erase(iter);
//			    }
//			}
//		}		
//		//cout << "Server deal with another mission"<<endl;
//	}
//
//	for (int i = (int)g_clients.size() - 1; i >= 0; --i)
//	{
//		closesocket(g_clients[i]);
//	}
//	closesocket(sock);
//
//	/// <summary>
//	/// 
//	/// </summary>
//	/// <returns></returns>
//	
//	WSACleanup();
//	std::cout << "end socket";
//	return 0;
//}
//
//int processor(SOCKET cSock)
//{
//	char cache[4096];//
//	Header* h = (Header*)cache;
//	int r = recv(cSock, cache, sizeof(Header), 0);
//	if (r <= 0)
//	{
//		cout << "Client " << cSock << " logout" << endl;
//		return -1;
//	}
//	switch (h->cmd)
//	{
//	case LOGIN:
//	{
//		recv(cSock, cache + sizeof(Header), h->len - sizeof(Header), 0);
//		Login* login = (Login*)cache;
//		cout << "Receive Client "<<cSock<< "join request data length:" << login->len << "  Name:"
//			<< login->name << " Password:" << login->pwd << endl;
//		//ignore passowrd and name
//		LoginRet loginRet;
//		send(cSock, (const char*)&loginRet, sizeof(loginRet), 0);
//	}
//	break;
//	case LOGOUT:
//	{
//		recv(cSock, cache + sizeof(Header), h->len - sizeof(Header), 0);
//		Logout* logout = (Logout*)cache;
//		cout << "Receive data length:" << logout->len << "  Name:"
//			<< logout->userName << " CMD:" << logout->cmd << endl;
//		LogoutRet logoutRet;
//		send(cSock, (const char*)&logoutRet, sizeof(logoutRet), 0);
//	}
//	break;
//	default:
//	{
//		Header h = { 0,ERROR_INFO };
//		send(cSock, (const char*)&h, sizeof(h), 0);
//	}
//	break;
//	}
//	//return 0;
//}