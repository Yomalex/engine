#include "stdafx.h"
#include "WZText.h"
#include "WZen.h"

BYTE * FileContent;
bool file_exists(const TCHAR * szFile) {
	HANDLE hTest = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, NULL);
	if (hTest == INVALID_HANDLE_VALUE) return false;
	CloseHandle(hTest);
	return true;
}
bool MapFile(const TCHAR * szFile, DWORD * FileSize)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	if(FileSize!=nullptr) *FileSize = GetFileSize(hFile, nullptr);

	FileContent = (BYTE*)malloc(*FileSize);
	if (FileContent == nullptr)
		return false;

	DWORD dwSize;
	ReadFile(hFile, FileContent, *FileSize, &dwSize, nullptr);
	CloseHandle(hFile);

	return true;
}
void UnMapFile()
{
	free(FileContent);
}

bool CWZText::Load(const TCHAR * szFile)
{
	DWORD dwSize;
	if (!MapFile(szFile, &dwSize)) return false;

	_TextBlock * pE;

	this->iCount = 0;
	for (int j = 6; j < dwSize; this->iCount++)
	{
		pE = (_TextBlock*)(FileContent + j);

		if (this->vElements[pE->dwIndex] != nullptr) delete this->vElements[pE->dwIndex];
		this->vElements[pE->dwIndex] = (_TextBlock*)malloc(pE->dwSize + 9);
		ZeroMemory(this->vElements[pE->dwIndex], pE->dwSize + 9);
		memcpy(this->vElements[pE->dwIndex], pE, pE->dwSize + 8);
		decrypt2(this->vElements[pE->dwIndex]->Content, pE->dwSize);
		j += pE->dwSize+8;
	}


	UnMapFile();
	return true;
}

bool CWZMoveReq::Load(const TCHAR * szFile)
{
	DWORD dwSize;
	if (!MapFile(szFile,&dwSize)) return false;

	this->iCount = (dwSize - 4) / sizeof(_MoveReqBlock);
	_MoveReqBlock * pE = (_MoveReqBlock*)(FileContent + 4);

	for (int i = 0; i < this->iCount; i++)
	{
		decrypt2((char*)pE, sizeof(_MoveReqBlock));
		this->vElements[i] = (_MoveReqBlock*)malloc(sizeof(_MoveReqBlock));
		if (this->vElements[i] == nullptr) break;
		memcpy(this->vElements[i], pE, sizeof(_MoveReqBlock));
		pE++;
	}

	UnMapFile();
	return true;
}

bool CWZGate::Load(const TCHAR * szFile)
{
	DWORD dwSize;
	if (!MapFile(szFile, &dwSize)) return false;
	
	_GateBlock * pBlock = (_GateBlock*)FileContent;
	this->iCount = dwSize / sizeof(_GateBlock);
	for (int i = 0; i < this->iCount; i++)
	{
		this->vElements[i] = (_GateBlock*)malloc(sizeof(_GateBlock));
		if (this->vElements[i] == nullptr) break;

		memcpy(this->vElements[i], pBlock, sizeof(_GateBlock));
		decrypt2((char*)this->vElements[i], sizeof(_GateBlock));
		pBlock++;
	}

	UnMapFile();
	return true;
}

bool CWZItem::Load(const TCHAR * szFile)
{
	DWORD dwSize;
	if (!MapFile(szFile, &dwSize)) return false;

	_ItemBlock * pBlock = (_ItemBlock*)FileContent;
	decrypt2((char*)pBlock, dwSize);
	this->iCount = dwSize / sizeof(_ItemBlock);
	for (int i = 0; i < this->iCount; i++)
	{
		this->vElements[i] = (_ItemBlock*)malloc(sizeof(_ItemBlock));
		if (this->vElements[i] == nullptr) break;

		memcpy(this->vElements[i], pBlock, sizeof(_ItemBlock));
		pBlock++;
	}

	UnMapFile();
	return true;
}
