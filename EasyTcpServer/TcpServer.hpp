#ifndef _TCPSERVER_HPP_
#define _TCPSERVER_HPP_

	#ifdef _WIN32 //use to for windodws
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>
		#include<WinSock2.h>
		#pragma comment (lib,"ws2_32.lib")
		#include<string.h>
		#include<iostream>
		#include "MessageHeader.hpp"
		#include<vector>
	#else//use to for mac or linux
		#include<unistd.h>
		#include<arpa/inet.h>
		#include<string.h>
		#include<iostream>

		#define SOCKET int
		#define INVALID_SOCKET (SOCKET)(~0)
		#define SOCKET_ERROR(-1)
	#endif


using namespace std;

class TcpServer
{
private:
	SOCKET _sock;
	vector<SOCKET>_clients;
public:
	TcpServer()
	{
		_sock = INVALID_SOCKET;
	}
	virtual ~TcpServer()
	{
		Close();
	}
	//Initialize
	int InitSocket()
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
		return _sock;
	}
	//Bind
	int Bind(const char* ip, unsigned short port)
	{
		if (INVALID_SOCKET == _sock)
		{
			InitSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
#ifdef _WIN32
		if (ip)
		{
			_sin.sin_addr.S_un.S_addr = inet_addr(ip);
		}
		else
		{
			_sin.sin_addr.S_un.S_addr = INADDR_ANY;
		}

#else
		if (ip)
		{
			_sin.sin_addr.s_addr = inet_addr(ip);
	    }
		else
		{
			_sin.sin_addr.s_addr = INADDR_ANY;
		}

#endif
		int ret= bind(_sock, (sockaddr*)&_sin, sizeof(_sin));
		if (SOCKET_ERROR ==ret)
		{
			std::cout << "bind failed" << std::endl;
		}
		else
		{
			std::cout << "bind success" << std::endl;
		}

		return ret;
	}
	//Listen
	int Listen(int n)//n is link numbers
	{
		int ret = listen(_sock, n);
		if (SOCKET_ERROR ==ret )
		{
			std::cout << "Listen error " <<(int)_sock<< std::endl;
		}
		else
		{
			std::cout << "Listen success " <<(int)_sock<< std::endl;
		}
		return ret;
	}
	//accept client link
	SOCKET Accept()
	{
		sockaddr_in clientAdd = {};
		int nAddlen = sizeof(sockaddr_in);
		SOCKET _cSock = INVALID_SOCKET;
#ifdef _WIN32
		_cSock = accept(_sock, (sockaddr*)&clientAdd, &nAddlen);
#else
		_cSock = accept(_sock, (sockaddr*)&clientAdd, (socklen_t*)&nAddlen);
#endif 

		if (INVALID_SOCKET == _cSock)
		{
			std::cout << "invailed socket " <<(int)_cSock<< std::endl;
		}
		else
		{
			NewJoin newJoin ;
			cout << "New client enter socket is:" << (int)_cSock
				<< " IP:" << inet_ntoa(clientAdd.sin_addr) << endl;
			sendDataAll(&newJoin);
			_clients.push_back(_cSock);
		}
		
		return _cSock;
	}
	//Close
	void Close()
	{
		if (_sock != INVALID_SOCKET)
		{
#ifdef _WIN32
			for (int i = (int)_clients.size() - 1; i >= 0; --i)
			{
				closesocket(_clients[i]);
			}
			closesocket(_sock);
			WSACleanup();
#else
			for (int i = (int)_clients.size() - 1; i >= 0; --i)
			{
				close(_clients[i]);
			}
			close(sock);
#endif
			_sock = INVALID_SOCKET;
		}
	}
	//Deal with msg
	bool OnRun()
	{
		if (IsRun())
		{
			fd_set fdRead;
			fd_set fdWrite;
			fd_set fdExp;
			//clean set
			FD_ZERO(&fdRead);
			FD_ZERO(&fdWrite);
			FD_ZERO(&fdExp);
			//put discript symbol in to set
			
			FD_SET(_sock, &fdRead);
			FD_SET(_sock, &fdWrite);
			FD_SET(_sock, &fdExp);
			SOCKET maxSock = _sock;
			for (int i = (int)_clients.size() - 1; i >= 0; --i)
			{
				FD_SET(_clients[i], &fdRead);
				if (maxSock < _clients[i])
				{
					maxSock = _clients[i];
				}
			}
			timeval t = { 1,0 };
			int ret = select(_sock + 1, &fdRead, &fdWrite, &fdExp, &t);
			if (ret < 0)
			{
				cout << "End socket with " << endl;
				Close();
				return false;
			}
			//determine  fdread if in the set
			if (FD_ISSET(_sock, &fdRead))
			{
				FD_CLR(_sock, &fdRead);
				Accept();
			}

			for (int i =(int)_clients.size()-1;i>=0;--i)
			{
				if (FD_ISSET(_clients[i],&fdRead))
				{
					if (-1 == RecvData(_clients[i]))
					{
						auto iter = _clients.begin() + i;
						if (iter != _clients.end())
						{
							_clients.erase(iter);
						}
					}
				}
			}
			return true;
		}
		return false;
	}
	//IsRun

	bool IsRun()
	{
		return INVALID_SOCKET != _sock;
	}

	//Send
	int RecvData(SOCKET _cSock)
	{
		char cache[4096];//
		int r = recv(_cSock, cache, sizeof(Header), 0);
		Header* h = (Header*)cache;
		if (r <= 0)
		{
			cout << "Client " <<(int) _cSock << " logout" << endl;
			return -1;
		}
		recv(_cSock, cache + sizeof(Header), h->len - sizeof(Header), 0);

		OnNetMsg(_cSock,h);
		return 0;
	}

	void OnNetMsg(SOCKET _cSock, Header* h)
	{
		switch (h->cmd)
		{
		case LOGIN:
		{
			Login* login = (Login*)h;
			cout << "Receive Client " << _cSock << " join request data length:" << login->len << "  Name:"
				<< login->name << " Password:" << login->pwd << endl;
			//ignore passowrd and name
			LoginRet loginRet;
			send(_cSock, (const char*)&loginRet, sizeof(loginRet), 0);
		}
		break;
		case LOGOUT:
		{
			Logout* logout = (Logout*)h;
			cout << "Receive data length:" << logout->len << "  Name:"
				<< logout->userName << " CMD:" << logout->cmd << endl;
			LogoutRet logoutRet;
			send(_cSock, (const char*)&logoutRet, sizeof(logoutRet), 0);
		}
		break;
		default:
		{
			Header h = { 0,ERROR_INFO };
			send(_cSock, (const char*)&h, sizeof(h), 0);
		}
		break;
		}
		//return 0;
	}

	//send single person
	int sendData(SOCKET _cSock,Header* h)
	{
		if (IsRun() && h)
		{
			return send(_cSock, (const char*)h, h->len, 0);
		}
		return SOCKET_ERROR;
	}

	//send all
	void sendDataAll(Header* h)
	{
		//log
		for (int i = _clients.size() - 1; i >= 0; --i)
		{
			sendData(_clients[i], h);
		}
	}
};

#endif