enum CMD
{
	LOGIN,
	LOGIN_RET,
	LOGOUT,
	LOGOUT_RET,
	ERROR_INFO,
	NEW_JOIN
};
struct Header
{
	int len;
	CMD cmd;
};
struct Login :public Header
{
	Login()
	{
		len = sizeof(Login);
		cmd = LOGIN;
	}
	char name[30];
	char pwd[30];
};
struct Logout :public Header
{
	Logout()
	{
		len = sizeof(Logout);
		cmd = LOGOUT;
	}
	char userName[30];
};
struct LoginRet :public Header
{
	LoginRet()
	{
		len = sizeof(LoginRet);
		cmd = LOGIN_RET;
		inRet = 0;
	}
	int inRet;
};
struct LogoutRet :public Header
{
	LogoutRet()
	{
		len = sizeof(LogoutRet);
		cmd = LOGOUT_RET;
		outRet = 0;
	}
	int outRet;
};

struct NewJoin :public Header
{
	NewJoin()
	{
		len = sizeof(NewJoin);
		cmd = NEW_JOIN;
		sock = 0;
	}
	int sock;
};