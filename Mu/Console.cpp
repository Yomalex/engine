#include "stdafx.h"
#include "Game.h"


CConsole::CConsole(CGame * Game)
{
	ZeroMemory(Commands, sizeof(Commands));
	this->pGame = Game;
}


CConsole::~CConsole()
{
	for (int i = 0; i < MAX_COMMANDS; i++)
	{
		if (this->Commands[i] == nullptr) break;
		free(this->Commands[i]->ArgDecl);
		delete this->Commands[i];
	}
}

bool CConsole::RegistreCommand(const TCHAR * lpcName, char * Args)
{
	for (int i = 0; i < MAX_COMMANDS; i++)
	{
		if (this->Commands[i] == nullptr)
		{
			this->Commands[i] = new _Command();
			int iArgs = 0;
			while(Args[iArgs]!=-1) iArgs++;
			this->Commands[i]->ArgDecl = (char*)malloc(iArgs);
			memcpy(this->Commands[i]->ArgDecl, Args, iArgs);
			_tcscpy_s(this->Commands[i]->szCommnad, lpcName);
			this->Commands[i]->ArgCount = iArgs;
			return true;
		}
	}
	return false;
}

bool CConsole::ProcessCommand(const TCHAR * lpcCommand)
{
	if (lpcCommand[0] != '/') return false;
	TCHAR szName[100];
	_stscanf_s(lpcCommand+1, TEXT("%s"), szName, 100);
	_Command * Cmd = this->Find(szName);
	if (Cmd == nullptr) return false;

	TCHAR szArg[100], *pType;
	int iArg;
	short sArg;
	int readed = 1 + _tcslen(szName)+1;
	int MaxLen = _tcslen(lpcCommand);
	int i = 0;
	void * ArgList[100];
	while(MaxLen>readed)
	{
		_stscanf_s(lpcCommand + readed, TEXT("%s"), szArg, 100);
		readed += _tcslen(szArg) + 1;
		switch (Cmd->ArgDecl[i])
		{
		case ARG_STRING:
			ArgList[i] = malloc(_tcslen(szArg) + 1);
			memcpy(ArgList[i], szArg, _tcslen(szArg) + 1);
			break;
		case ARG_INT:
			iArg = _ttoi(szArg);
			ArgList[i] = malloc(sizeof(int));
			memcpy(ArgList[i], &iArg, sizeof(int));
			break;
		case ARG_SHORT:
			sArg = _ttoi(szArg);
			ArgList[i] = malloc(sizeof(short));
			memcpy(ArgList[i], &sArg, sizeof(short));
			break;
		}
		if (Cmd->ArgDecl[i] == ARG_END) break;
		i++;
	}

	return false;
}

CConsole::_Command * CConsole::Find(const TCHAR * lpCmdName)
{
	for (int i = 0; i < MAX_COMMANDS; i++)
	{
		if (this->Commands[i] == nullptr) return nullptr;
		if (!_tcscmp(this->Commands[i]->szCommnad, lpCmdName)) return this->Commands[i];
	}
	return nullptr;
}
