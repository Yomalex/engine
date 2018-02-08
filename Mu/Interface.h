#pragma once

#define MAX_GROUPS 20
#define MAX_SERVER 20
#define MAKE_GPARAM(group,server) (LPARAM)(group*MAX_SERVER+server)
#define GET_GROUP(x) (x/MAX_SERVER)
#define GET_SERVER(x) (x%MAX_SERVER)

void StartMuUI();
void ResetMuUI();

void ShowServer();
void ShowCharacterSelect();
void ShowUI();

extern CEdit * Position;