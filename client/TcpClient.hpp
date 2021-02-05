#ifndef _TCPCLIENT_HPP_
#define _TCPCLIENT_HPP_

#ifdef _WIN32 //use to for windodws
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include<WinSock2.h>
	#pragma comment (lib,"ws2_32.lib")
	#include<string.h>
	#include<iostream>
#else//use to for mac or linux
	#include<unistd.h>
	#include<arpa/inet.h>
	#include<string.h>
	#include<iostream>

	#define SOCKET int
	#define INVALID_SOCKET (SOCKET)(~0)
	#define SOCKET_ERROR(-1)
#endif

#include "MessageHeader.hpp"
using namespace std;
class TcpClient
{
	SOCKET _sock;
public:

	TcpClient()
	{
		_sock = INVALID_SOCKET;
	}
	~TcpClient()
	{
		Close();
	}

	void InitSocket()
	{
#ifdef _WIN32
		WORD verson = MAKEWORD(2, 2);
		WSADATA data;
		WSAStartup(verson, &data);
#endif
		if (INVALID_SOCKET != _sock)
		{
			//Should log
			std::cout << "Close connect before new connection " << _sock << std::endl;
			Close();
		}
		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock)
		{
			std::cout << "Wrong Link With Sock" << std::endl;
		}
		else
		{
			std::cout << "success connect sock" << std::endl;
		}
	}
	int Connect(const char* ip, unsigned short port)
	{
		if (INVALID_SOCKET == _sock)
		{
			InitSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr(ip);
#endif
		int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret)
		{
			std::cout << "connect Failed" << std::endl;
		}
		else
		{
			std::cout << "success connect server" << std::endl;
		}
		return ret;
	}
	void Close()
	{
		if (_sock != INVALID_SOCKET)
		{
#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
#else
			close(sock);
#endif
			_sock = INVALID_SOCKET;
		}
	}

	bool onRun()
	{
		if (IsRun())
		{
			fd_set fdRead;
			FD_ZERO(&fdRead);
			FD_SET(_sock, &fdRead);
			timeval t = { 1,0 };
			int ret = select(_sock, &fdRead, 0, 0, &t);
			if (ret < 0)
			{
				std::cout << "Mission End " << _sock << std::endl;
				return false;
			}
			if (FD_ISSET(_sock, &fdRead))
			{
				FD_CLR(_sock, &fdRead);
				if (-1 == recvData(_sock))//receive message in here
				{
					std::cout << "Mission End " << _sock << std::endl;
					return false;
				}
			}
		}
		return true;
	}

	bool IsRun()
	{
		return INVALID_SOCKET != _sock;
	}
	int recvData(SOCKET _cSock)
	{
		char cache[4096];// 
		int r = recv(_cSock, cache, sizeof(Header), 0);
		Header* h = (Header*)cache;
		if (r <= 0)
		{
			cout << "Disconnect" << endl;
			return -1;
		}
		recv(_cSock, cache + sizeof(Header), h->len - sizeof(Header), 0);
		onNetMsg(h);
		return 0;
	}
	void onNetMsg(Header* h)
	{
		switch (h->cmd)
		{
		case LOGIN_RET:
		{
			LoginRet* logret = (LoginRet*)h;
			cout << "Receive server data length:" << logret->len << endl;
		}
		break;
		case LOGOUT_RET:
		{
			LogoutRet* logoutmsg = (LogoutRet*)h;
			cout << "Receive server logout data length:" << logoutmsg->len << endl;
		}
		break;
		case NEW_JOIN:
		{
			NewJoin* newJoin = (NewJoin*)h;
			cout << "Receive server New JOIN length:" << newJoin->len << endl;
		}
		break;
		default:
		{
		}
		break;
		}
	}

	int sendData(Header* h)
	{
		if (IsRun() && h)
		{
			return send(_sock, (const char*)h, h->len, 0);
		}
		return SOCKET_ERROR;
	}

};

#endif // !_TCPCLIENT_HPP_
