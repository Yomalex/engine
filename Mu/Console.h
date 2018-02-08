#pragma once

class CConsole;
typedef bool(*prCommand)(CConsole*,void*,int);
#define COMMAND(x) bool x(CConsole* pConsole,void* pArg, int ArgCount)
#define MAX_COMMANDS 512

#define ARG_STRING	0
#define ARG_INT		1
#define ARG_SHORT	2
#define ARG_FLOAT	3
#define ARG_END		-1

class CConsole
{
	struct _Command
	{
		TCHAR szCommnad[100];
		char * ArgDecl;
		int ArgCount;
	};
public:
	CConsole(CGame * Game);
	~CConsole();

	bool RegistreCommand(const TCHAR * lpcName, char * Args);
	bool ProcessCommand(const TCHAR * lpcCommand);
private:
	_Command * Find(const TCHAR * lpCmdName);
	_Command * Commands[MAX_COMMANDS];
	CGame * pGame;
};

