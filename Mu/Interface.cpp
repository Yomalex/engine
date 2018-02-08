#include "stdafx.h"
#include "Mu.h"

const TCHAR * szInterface[][2] =
{
	{ TEXT("OK"),TEXT("./Data/Interface/newui_button_ok.OZT") },
	{ TEXT("CANCEL"),TEXT("./Data/Interface/newui_button_cancel.OZT") },
	{ TEXT("CLOSE"),TEXT("./Data/Interface/newui_button_close.OZT") },
	{ TEXT("LOGINBACK"),TEXT("./Data/Interface/login_back.OZT") },
	{ TEXT("EDIT"),TEXT("./Data/Interface/login_me.OZT") },
	{ TEXT("CHBUTTON"),TEXT("./Data/Interface/cha_bt.OZT") },
	{ TEXT("SERVERBT"),TEXT("./Data/Interface/server_b2_all.OZT") },
	{ TEXT("CONNECT"),TEXT("./Data/Interface/b_connect.OZT") },
	{ TEXT("CREATE"),TEXT("./Data/Interface/b_create.OZT") },
	{ TEXT("DELETE"),TEXT("./Data/Interface/b_delete.OZT") },
	{ TEXT("MENU"),TEXT("./Data/Interface/server_menu_b_all.OZT") },
	{ TEXT("CHID"),TEXT("./Data/Interface/cha_id.OZT") },
	{ TEXT("BACKGROUND"),TEXT("./Data/Interface/newui_msgbox_back.OZJ") },
	{ TEXT("BTNEMPTY"),TEXT("./Data/Interface/newui_btn_empty.OZT") },
	{ TEXT("MSGBOX_TOP"),TEXT("./Data/Interface/newui_msgbox_top.OZT") },
	{ TEXT("MSGBOX_MID"),TEXT("./Data/Interface/newui_msgbox_middle.OZT") },
	{ TEXT("MSGBOX_BOT"),TEXT("./Data/Interface/newui_msgbox_bottom.OZT") },
	{ TEXT("OPTIONS_TOP"),TEXT("./Data/Interface/newui_Message_03.OZT") },
	{ TEXT("CHINFO"),TEXT("./Data/Interface/character_ex.OZT") },
	{ TEXT("LOGO"),TEXT("./Data/Interface/New_lo_mu_logo.OZT") },
	{ TEXT("SCROLLDOWN"),TEXT("./Data/Interface/newui_Bt_scroll_dn.OZJ") },
	{ TEXT("SCROLLUP"),TEXT("./Data/Interface/newui_Bt_scroll_up.OZJ") },
	{ TEXT("SCROLLOFF"),TEXT("./Data/Interface/newui_scroll_off.OZT") },
	{ TEXT("SCROLLON"),TEXT("./Data/Interface/newui_scroll_on.OZT") },
	{ TEXT("SCROLLTOP"),TEXT("./Data/Interface/newui_scrollbar_up.OZT") },
	{ TEXT("SCROLLMID"),TEXT("./Data/Interface/newui_scrollbar_m.OZT") },
	{ TEXT("SCROLLBOT"),TEXT("./Data/Interface/newui_scrollbar_down.OZT") },
	{ TEXT("CREDITS"),TEXT("./Data/Interface/server_credit_b_all.OZT") },
	{ TEXT("SERVERDECO"),TEXT("./Data/Interface/server_deco_all.OZT") },
	{ TEXT("CHARINFO"),TEXT("./Data/Interface/character_ex.OZT") },
	{ TEXT("DECORATION"),TEXT("./Data/Interface/deco.OZT") },
	{ TEXT("POSITION"),TEXT("./Data/Interface/newui_position02.OZT") },
	{ TEXT("CHAT"),TEXT("./Data/Interface/newui_chat_back.OZJ") },
	{ TEXT("CHATALPHA"),TEXT("./Data/Interface/newui_chat_btn_alpha.OZJ") },
	{ TEXT("CHATSIZE"),TEXT("./Data/Interface/newui_chat_btn_size.OZJ") },
	{ TEXT("CHATMAIN"),TEXT("./Data/Interface/newui_chat_main(s).OZJ") },
	{ TEXT("CHATWISP"),TEXT("./Data/Interface/newui_chat_whisper.OZJ") },
	{ TEXT("CHATON"),TEXT("./Data/Interface/newui_chat_chat_on.OZJ") },
	{ TEXT("CHATONF"),TEXT("./Data/Interface/newui_chat_frame_on.OZJ") },
	{ TEXT("CHATONG"),TEXT("./Data/Interface/newui_chat_guild_on.OZJ") },
	{ TEXT("CHATONP"),TEXT("./Data/Interface/newui_chat_party_on.OZJ") },
	{ TEXT("CHATONS"),TEXT("./Data/Interface/newui_chat_system_on.OZJ") },
	{ TEXT("CHATONW"),TEXT("./Data/Interface/newui_chat_whisper_on.OZJ") },
	{ TEXT("CHATONN"),TEXT("./Data/Interface/newui_chat_normal_on.OZJ") },
	{ TEXT("STATS0"),TEXT("./Data/Interface/newui_menu01.OZJ") },
	{ TEXT("STATS1"),TEXT("./Data/Interface/newui_menu02.OZJ") },
	{ TEXT("STATS2"),TEXT("./Data/Interface/newui_menu03.OZJ") },
	{ TEXT("STATS3"),TEXT("./Data/Interface/newui_menu02-03.OZJ") },
	{ TEXT("CHKBOX"),TEXT("./Data/Interface/newui_option_check.OZT") },
	{ TEXT("STATS_BT0"),TEXT("./Data/Interface/newui_menu_Bt01.OZJ") },
	{ TEXT("STATS_BT1"),TEXT("./Data/Interface/newui_menu_Bt02.OZJ") },
	{ TEXT("STATS_BT2"),TEXT("./Data/Interface/newui_menu_Bt03.OZJ") },
	{ TEXT("STATS_BT3"),TEXT("./Data/Interface/newui_menu_Bt04.OZJ") },
	{ TEXT("HP"),TEXT("./Data/Interface/newui_menu_red.OZJ") },
	{ TEXT("MP"),TEXT("./Data/Interface/newui_menu_blue.OZJ") },
	{ TEXT("PO"),TEXT("./Data/Interface/newui_menu_green.OZJ") },
	{ TEXT("SD"),TEXT("./Data/Interface/newui_menu_SD.OZJ") },
	{ TEXT("AG"),TEXT("./Data/Interface/newui_menu_AG.OZJ") },
	{TEXT("INV_TOP"),TEXT("./Data/Interface/newui_item_back01.OZT")},
	{TEXT("INV_MID_L"),TEXT("./Data/Interface/newui_item_back02-L.OZT")},
	{TEXT("INV_MID_R"),TEXT("./Data/Interface/newui_item_back02-R.OZT")},
	{TEXT("INV_BOT"),TEXT("./Data/Interface/newui_item_back03.OZT")},
	{TEXT("INV_TOP2"),TEXT("./Data/Interface/newui_item_back04.OZT")},
	{TEXT("INV_BOOTS"),TEXT("./Data/Interface/newui_item_boots.OZT")},
	{TEXT("INV_HELM"),TEXT("./Data/Interface/newui_item_cap.OZT")},
	{TEXT("INV_FAIRY"),TEXT("./Data/Interface/newui_item_fairy.OZT")},
	{TEXT("INV_GLOVES"),TEXT("./Data/Interface/newui_item_gloves.OZT")},
	{TEXT("INV_PANTS"),TEXT("./Data/Interface/newui_item_lower.OZT")},
	{TEXT("INV_NECKLEACE"),TEXT("./Data/Interface/newui_item_necklace.OZT")},
	{TEXT("INV_RING"),TEXT("./Data/Interface/newui_item_ring.OZT")},
	{TEXT("INV_ARMOR"),TEXT("./Data/Interface/newui_item_upper.OZT")},
	{TEXT("INV_WEAPON_L"),TEXT("./Data/Interface/newui_item_weapon(L).OZT")},
	{TEXT("INV_WEAPON_R"),TEXT("./Data/Interface/newui_item_weapon(R).OZT")},
	{TEXT("INV_WINGS"),TEXT("./Data/Interface/newui_item_wing.OZT")},
	{TEXT("INV_USE_1"),TEXT("./Data/Interface/newui_inven_usebox_01.OZT")},
	{TEXT("INV_USE_2"),TEXT("./Data/Interface/newui_inven_usebox_02.OZT")},
	{TEXT("INV_USE_3"),TEXT("./Data/Interface/newui_inven_usebox_03.OZT")},
	{TEXT("INV_UNUSE"),TEXT("./Data/Interface/newui_item_box.OZT")},
	{TEXT("INV_MONEY"),TEXT("./Data/Interface/newui_item_money.OZT")},
	{TEXT("INV_B_UP_LEFT"),TEXT("./Data/Interface/newui_item_table01(L).OZT")},
	{TEXT("INV_B_UP_RIGHT"),TEXT("./Data/Interface/newui_item_table01(R).OZT")},
	{TEXT("INV_B_DW_LEFT"),TEXT("./Data/Interface/newui_item_table02(L).OZT")},
	{TEXT("INV_B_DW_RIGHT"),TEXT("./Data/Interface/newui_item_table02(R).OZT")},
	{TEXT("INV_B_UP"),TEXT("./Data/Interface/newui_item_table03(UP).OZT")},
	{TEXT("INV_B_DW"),TEXT("./Data/Interface/newui_item_table03(Dw).OZT")},
	{TEXT("INV_B_LEFT"),TEXT("./Data/Interface/newui_item_table03(L).OZT")},
	{TEXT("INV_B_RIGHT"),TEXT("./Data/Interface/newui_item_table03(R).OZT")},
	{TEXT("BT_EXIT"),TEXT("./Data/Interface/newui_exit_00.OZT")},
	//{TEXT(""),TEXT("./Data/Interface/")},
	//{TEXT(""),TEXT("./Data/Interface/")},
	//{TEXT(""),TEXT("./Data/Interface/")},
	//{TEXT(""),TEXT("./Data/Interface/")},
	//{TEXT(""),TEXT("./Data/Interface/")},
	{ nullptr,nullptr },
};
int TextFileClass[] = {
	20,//Dark Wizard
	21,//Dark Knight
	22,//Fary Elf
	23,//Magic Gladiator
	24,//Dark Lord
	1687,//Summoner

	25,//Soul Master
	26,//Blade Knight
	27,//Muse Elf
	28,//none
	29,//none
	1688,//Blody Summoner

	1668,//Blade Master
	1669,//Grand Master
	1670,//High Elf
	1671,//Duel Master
	1672,//Lord Emperador
	1689,//Dimension Master
};

const TCHAR * szFaces[] =
{
	TEXT("./Data/Logo/NewFace01.bmd"),
	TEXT("./Data/Logo/NewFace02.bmd"),
	TEXT("./Data/Logo/NewFace03.bmd"),
	TEXT("./Data/Logo/NewFace04.bmd"),
	TEXT("./Data/Logo/NewFace05.bmd"),
	TEXT("./Data/Logo/NewFace06.bmd"),
	//TEXT("./Data/Logo/NewFace01.bmd"),
};

// Select Server
CWindow * ServerScreen;
CButton * Server_Menu, *Server_Credits;
CImage * Server_Deco;
CStatic * Client_version, *Client_Copyright, *Client_Rights;

// Server List
CWindow * ServerList;
CButton * bList[MAX_GROUPS];
CImage * MULogo;
int group = 0;

// Channel Select
CWindow * ChannelList[MAX_SERVER];
CButton * bChannel[MAX_GROUPS][MAX_SERVER];

// Login
CWindow * Login;
CButton * OK;
CButton * Cancel;
CEdit * Account;
CEdit * Password;
CStatic *aStatic;
CStatic *pStatic;
CStatic *sStatic;

// Character Select
CWindow * CharacterSelect;
CButton * Create, *csMenu, *Connect, *Delete;
CImage *Character_Deco;

// Character Create
CWindow * CharacterCreate;
/*const TCHAR * szClass[] =
{
	TEXT("Dark Wizard"),
	TEXT("Dark Knight"),
	TEXT("Fary Elf"),
	TEXT("Magic Gladiator"),
	TEXT("Dark Lord"),
	TEXT("Summoner")
};*/

CButton *bClass[7], *ccOK, *ccCancel;
CUIModel * mClass[7];
CImage * ccback;
CEdit * ccName;

// Menu
CWindow * Menu;
CButton * Menu_bt[5];
CImage * Menu_dec[3];

// Options
CWindow * Options;
CImage * Options_Dec[3];
CButton * Options_Close;
CJumpList * Options_Res;
CCheckBox * Options_FullScreen;

const TCHAR * szMenu[5] =
{
	TEXT("Exit"),
	TEXT("Select Server"),
	TEXT("Select Character"),
	TEXT("Options"),
	TEXT("Cancel")
};
//int iMenu[5] = {};

// InGame
CWindow * InGame;
CEdit * Position;
CProgressBar * HP, *MP, *SD, *AG;

CWindow * Status;
CImage * Imgs[3];
CButton *Stats,*Inventory, *Friends, *GameMenu;

// Chat
CWindow * ChatWindow;
CEdit * ChatWisp, *ChatText;
CButton * ChatAlpha, *ChatSize;
CCheckBox * Chat, *ChatFrame, *ChatGuild, *ChatNormal, *ChatParty,* ChatSystem, *ChatWisper;
TCHAR szInnerChat[1024*4];
CRichBox * RichBox;
int RichSize = 100;

CMenu * WarpMenu;

// Inventory
CWindow * winInventory;
CImage * imgInv[3];
CImage * invHelm, *invPants, *invBoots, *invGloves, *invArmor, *invFairy, *învWings, *invRings[2], *invNeckleace, *invWeapon[2];
CImage * invBox[64], *invMoney, *invBorder[8];
CButton * invExit1, *invExit2;

// Character Statistics
CWindow * winCharacter;
CImage * imgChar[3];

//WareHouse
CImage * wareBox[8*15];

MDEVENT(ModelEventClassSelected)
{
	caller->OnAnimationEnd = nullptr;
	caller->SetFrame(0, 0);
}

UIEVENT(EventClassSelect)
{
	for (int i = 0; i < 6; i++)
	{
		//mClass[i]->SetBoundingBox(Faces[i]->vBounding);
		mClass[i]->Show();
		Faces[i]->SetFrame(1, 0);
		Faces[i]->OnAnimationEnd = ModelEventClassSelected;
		if (i == lParam) continue;
		mClass[i]->Hide();
	}
}

UIEVENT(EventCSelect)
{
	switch (lParam)
	{
	case 0:// Connect
		Game = CS_PLAYING;
		eng->SetCallBack(OnFrame_PLAYING);
		eng->SetFogLinear(0.55, 0.85);
		WarpGate(17);
		ShowUI();
		Player.SetClass(1);
		break;
	case 1:
		for (int i = 0; i < 6; i++)
			mClass[i]->SetBoundingBox(Faces[i]->vBounding);
		Create->Enable(false);
		Connect->Enable(false);
		CharacterCreate->Show();
		EventClassSelect(caller, 0);
		break;
	case 2:
	case 3:
		Create->Enable(true);
		Connect->Enable(true);
		CharacterCreate->Hide();
		ccName->SetText(TEXT(""));
		break;
	}
}

UIEVENT(EventServer)
{
	sStatic->SetText(caller->GetText());
	Login->Show();
	ServerList->Hide();
}

UIEVENT(EventServerG)
{
	if (ChannelList[lParam]->Visible())
	{
		ChannelList[lParam]->Hide();
	}
	else
	{
		ChannelList[group]->Hide();
		ChannelList[lParam]->Show();
		group = lParam;
	}
	/*for (int i = 0; i < 128; i++)
	{
		if (caller == bList[i])
		{
			if (group == i)
			{
				ChannelList[i]->Hide();
				group = -1;
			}
			else {
				ChannelList[i]->Show();
				group = i;
			}
			return;
		}
	}*/
}

UIEVENT(EventLogin)
{
	switch (lParam)
	{
	case 1:
		//if (Mu->Login(Account->GetText(), Password->GetText()))
		{
			Game = CS_LOGGED;
			Warp(75, 0, 0);
			ShowCharacterSelect();
			eng->SetCameraPos(10000, 19000, 800);
			eng->SetCameraDir(-10, 2, -1);
			eng->SetFogLinear(0.45, 0.9);
			eng->SetAmbientLight((DWORD)0x00000000);
			eng->SetCallBack(OnFrame_LOGGED);
		}
		/*else
		{
		
		}*/
		break;
	case 0:
		Login->Hide();
		ServerList->Show();
		break;
	}/*
	if (Cancel == caller)
	{
	}
	else if (OK == caller)
	{
		eng->Release(Terrains);
		Game = CS_LOGGED;
		Terrains->SetMap(75);
		eng->StartLoad();
		eng->SetCameraPos(10000, 19000, 700);
		eng->SetCameraDir(-9, 2, -2);
		eng->AddLoadQueueElement(Terrains);
	}*/
}

UIEVENT(EventMenu)
{
	GameMenu->Enable(true);
	Menu->Hide();
	switch (lParam)
	{
	case 0://Exit
		eng->Kill();
		break;
	case 1://SS
		//Mu->Discconect(1);
		break;
	case 2://SC
		//Mu->Discconect(0);
		break;
	case 3:
		Options->Show();
		break;
	}
}

UIEVENT(EventShowMenu)
{
	((CWindow*)lParam)->Show();
}

UIEVENT(EventOptions)
{
	switch (lParam)
	{
	case 0:
		Options->Hide();
		Menu->Show();
		break;
	}
}

UIEVENT(EventResChange)
{
	if (Options_Res==caller)
	{
		DWORD dwOption;
		Options_Res->GetSelected(nullptr, 0, &dwOption);
		eng->SetVideoMode(dwOption);
	}
	else if (Options_FullScreen==caller)
	{
		eng->SetFullScreen(Options_FullScreen->isChecked());
	}
}

UIEVENT(EventChatBox)
{
	if (caller== Chat||caller==ChatParty||caller==ChatGuild)
	{
		CCheckBox *pCaller = (CCheckBox *)caller;
		if (!pCaller->isChecked())
		{
			Chat->Check(true);
			ChatParty->Check(false);
			ChatGuild->Check(false);
			return;
		}
		Chat->Check(false);
		ChatParty->Check(false);
		ChatGuild->Check(false);
		pCaller->Check(true);
	}
	else if (caller == ChatSize)
	{
		switch (RichSize)
		{
		case 100:
			RichSize = 200;
			break;
		case 200:
			RichSize = 300;
			break;
		case 300:
			RichSize = 100;
			break;
		default:
			RichSize = 100;
		}
		RichBox->SetPos(0, ChatWindow->GetY() - RichSize, ChatWindow->GetWidth(), RichSize);
	}
	else if(caller == ChatAlpha)
	{
		//Size Windows Change
	}
}

UIEVENT(EventWarp)
{
	DWORD dwWarp;
	WarpMenu->GetElement(caller, nullptr, 0, &dwWarp);
	WarpGate(dwWarp);
}

UIEVENT(EventUI)
{
	switch (lParam)
	{
	case 1:
		//Inventory->Enable(false);
		if(!winInventory->Visible()) winInventory->Show();
		else winInventory->Hide();
		break;
	}
}

UIEVENT(EventKeyInGame)
{
	if(!InGame->Visible())return;

	BYTE Key = (BYTE)lParam;
	switch (Key)
	{
	case 'I':EventUI(caller, 1);
		break;
	case 'M':
		if (WarpMenu->Visible()) WarpMenu->Hide();
		else WarpMenu->Show();
		break;
	case VK_RETURN:
		if (ChatWindow->Visible())
		{
			TCHAR * pChat = szInnerChat + _tcslen(szInnerChat);
			if(_tcslen(ChatText->GetText())>0) _stprintf_s(pChat, 4096 - _tcslen(szInnerChat), TEXT("~#CFF0000FFUser: ~#CFFFFFFFF%s\r\n"), ChatText->GetText());
			ChatWindow->Hide();
			ChatText->SetText(TEXT(""));
			RichBox->innerText(szInnerChat);
		}
		else
		{
			ChatWindow->Show();
			ChatWindow->SetKeyboardFocus(ChatText);
		}
		break;
	case VK_ESCAPE:
		if (Menu->Visible())
		{
			Menu->Hide();
			GameMenu->Enable(true);
		}
		else
		{
			if (ChatWindow->Visible())
			{
				ChatWindow->Hide();
				break;
			}
			Menu->Show();
			GameMenu->Enable(false);
		}
		break;
	}
}

void SelecServerUI()
{
	ServerScreen = new CWindow(TEXT("ServerScreen"), 0, 0, Interface->GetDesktop());
	MULogo = new CImage(0, 0, TextureManager->GetTexture(TEXT("LOGO")), ServerScreen);
	Server_Deco = new CImage(0, 0, TextureManager->GetTexture(TEXT("DECORATION")), ServerScreen);
	Server_Menu = new CButton(50, 0, TextureManager->GetTexture(TEXT("MENU")), 3, ServerScreen);
	Server_Credits = new CButton(0, 0, TextureManager->GetTexture(TEXT("CREDITS")), 3, ServerScreen);
	Server_Menu->OnClick = EventShowMenu;
	Client_Copyright = new CStatic(Text.String(454), 0, 0, 0, 0, ServerScreen);
	Client_Rights = new CStatic(Text.String(455), 0, 0, 0, 0, ServerScreen);
	Client_version = new CStatic(Text.String(456), 0, 0, 0, 0, ServerScreen);

	// Login Screen
	ServerList = new CWindow(TEXT("ServerList"), 50, 150, ServerScreen);
	ServerList->Show();
	for (UINT i = 0; i < MAX_GROUPS; i++)
	{
		if(i<MAX_GROUPS/2)
			bList[i] = new CButton(0, i * 20, TextureManager->GetTexture(TEXT("CHBUTTON")), 4, ServerList);
		else
			bList[i] = new CButton(0, (i - MAX_GROUPS / 2) * 20, TextureManager->GetTexture(TEXT("CHBUTTON")), 4, ServerList);
		bList[i]->OnClick = EventServerG;
		bList[i]->SetParam(i);
		bList[i]->Hide();
		
		// 120
		ChannelList[i] = new CWindow(TEXT("Server Group"), 120, 0, ServerList);

		for (UINT j = 0; j < MAX_SERVER; j++)
		{
			bChannel[i][j] = new CButton(0, 0, TextureManager->GetTexture(TEXT("SERVERBT")), 3, ChannelList[0]);
			bChannel[i][j]->OnClick = EventServer;
			bChannel[i][j]->Hide();
		}
	}
	//bList[0] = new CButton(0, 0, TextureManager->GetTexture(TEXT("CHBUTTON")), 4, ServerList);
	bList[0]->SetText(TEXT("Server 1"));
	bList[0]->Show();

	
	//bChannel[0][0] = new CButton(0, 0, TextureManager->GetTexture(TEXT("SERVERBT")), 3, ChannelList[0]);
	//bChannel[0][0]->OnClick = EventServer;
	bChannel[0][0]->Show();
	bChannel[0][0]->SetText(TEXT("Channel 1"));

	// Login Centered
	Login = new CWindow(TEXT("Login"), 0,0, ServerScreen, TextureManager->GetTexture(TEXT("LOGINBACK")));
	OK = new CButton(180, 190, TextureManager->GetTexture(TEXT("OK")), 3, Login);
	Cancel = new CButton(240, 190, TextureManager->GetTexture(TEXT("CANCEL")), 3, Login);
	Account = new CEdit(100, 120, TextureManager->GetTexture(TEXT("EDIT")), Login);
	Password = new CEdit(100, 150, TextureManager->GetTexture(TEXT("EDIT")), Login);
	aStatic = new CStatic(TEXT("Account"), 40, 120, 150, 30, Login);
	pStatic = new CStatic(TEXT("Password"), 40, 150, 150, 30, Login);
	sStatic = new CStatic(TEXT(""), 100, 90, 150, 30, Login);
	Cancel->OnClick = EventLogin;
	OK->OnClick = EventLogin;
	Cancel->SetParam(0);
	OK->SetParam(1);
	Password->SetPassword(true);
}
void SelectCharacterUI()
{
	// SelectCharacter Screen
	CharacterSelect = new CWindow(TEXT("SelectCharacter"), 50, 0, Interface->GetDesktop());
	Create = new CButton(0, 0, TextureManager->GetTexture(TEXT("CREATE")), 4, CharacterSelect);
	csMenu = new CButton(60, 0, TextureManager->GetTexture(TEXT("MENU")), 3, CharacterSelect);
	Character_Deco = new CImage(0, 0, TextureManager->GetTexture(TEXT("DECORATION")), CharacterSelect);
	Connect = new CButton(0, 0, TextureManager->GetTexture(TEXT("CONNECT")), 4, CharacterSelect);
	Delete = new CButton(0, 0, TextureManager->GetTexture(TEXT("DELETE")), 4, CharacterSelect);
	Delete->Enable(false);
	Connect->SetParam(0);
	Create->SetParam(1);
	Create->OnClick = EventCSelect;
	Connect->OnClick = EventCSelect;
	csMenu->OnClick = EventShowMenu;

	CharacterCreate = new CWindow(TEXT("CreateCharacter"), 0, 0, Interface->GetDesktop());

	D3DXMATRIX m;
	D3DXMatrixRotationZ(&m, D3DX_PI);
	//D3DXMatrixIdentity(&m);

	for (int i = NULL; i < 6; i++)
	{
		bClass[i] = new CButton(346, i * 27, TextureManager->GetTexture(TEXT("CHBUTTON")), 4, CharacterCreate);

		bClass[i]->Show();
		//bClass[i]->SetText(szClass[i]);
		bClass[i]->SetText(Text.String(TextFileClass[i]));
		bClass[i]->OnClick = EventClassSelect;
		bClass[i]->SetParam(i);
		mClass[i] = new CUIModel(0, 50, 366, 316, 386, 386, Faces[i], CharacterCreate);

		mClass[i]->SetTransform(m);
		mClass[i]->SetBoundingBox(Faces[i]->vBounding);
		mClass[i]->SetPos(-1, -1, -1, 316);
		mClass[i]->Hide();
	}
	ccOK = new CButton(346, 346, TextureManager->GetTexture(TEXT("OK")), 3, CharacterCreate);
	ccCancel = new CButton(406, 346, TextureManager->GetTexture(TEXT("CANCEL")), 3, CharacterCreate);
	ccback = new CImage(0, 346, TextureManager->GetTexture(TEXT("CHID")), CharacterCreate);
	ccName = new CEdit(70, 360, TextureManager->GetTexture(TEXT("EDIT")), CharacterCreate);
	mClass[0]->Show();
	ccName->SetTexture(nullptr);

	ccOK->OnClick = EventCSelect;
	ccOK->SetParam(2);
	ccCancel->OnClick = EventCSelect;
	ccCancel->SetParam(3);
}
void MenuAndOptionsUI()
{
	// Menu
	Menu = new CWindow(TEXT("Menu"), 0, 0, Interface->GetDesktop(), TextureManager->GetTexture(TEXT("BACKGROUND")));
	Server_Menu->SetParam((LPARAM)Menu);
	csMenu->SetParam((LPARAM)Menu);

	int iMenu[] = { 381,382,383,385,384 };
	for (int i = NULL; i < 5; i++)
	{
		Menu_bt[i] = new CButton(0, 0, TextureManager->GetTexture(TEXT("BTNEMPTY")), 3, Menu);
		Menu_bt[i]->SetPos((Menu->GetWidth() - Menu_bt[i]->GetWidth()) / 2, (Menu_bt[i]->GetHeight() + 5)*i + 25);
		Menu_bt[i]->SetText(/*szMenu[i]*/Text.String(iMenu[i]));
		Menu_bt[i]->SetParam(i);
		Menu_bt[i]->OnClick = EventMenu;
	}

	int tmph = (Menu_bt[0]->GetHeight() + 5) * 5 + 50;
	Menu->SetPos(0,0, -1, tmph);
	Menu_dec[0] = new CImage(0, 0, TextureManager->GetTexture(TEXT("MSGBOX_TOP")), Menu);
	Menu_dec[1] = new CImage(0, Menu_dec[0]->GetHeight(), TextureManager->GetTexture(TEXT("MSGBOX_MID")), Menu);
	Menu_dec[2] = new CImage(0, 0, TextureManager->GetTexture(TEXT("MSGBOX_BOT")), Menu);

	Menu_dec[2]->SetPos(0, tmph - Menu_dec[2]->GetHeight()+5);
	Menu_dec[1]->SetPos(-1, -1, -1, Menu_dec[2]->GetY() - Menu_dec[1]->GetY());

	// Options
	Options = new CWindow(TEXT("Options"), 0, 0, Interface->GetDesktop(), TextureManager->GetTexture(TEXT("BACKGROUND")));
	Options_Dec[0] = new CImage(0, 0, TextureManager->GetTexture(TEXT("OPTIONS_TOP")), Options);
	Options_Dec[1] = new CImage(0, 0, TextureManager->GetTexture(TEXT("MSGBOX_MID")), Options);
	Options_Dec[2] = new CImage(0, 0, TextureManager->GetTexture(TEXT("MSGBOX_BOT")), Options);
	Options->SetPos(-1, -1, -1, 300);
	Options_Close = new CButton(0, 0, TextureManager->GetTexture(TEXT("CLOSE")), 3, Options);
	Options_FullScreen = new CCheckBox(20, 100, TextureManager->GetTexture(TEXT("CHKBOX")), Options);
	Options_Res = new CJumpList(20, 50, TextureManager->GetTexture(TEXT("EDIT")), TextureManager->GetTexture(TEXT("SCROLLDOWN")), 3, Options);

	Options_Dec[2]->SetPos(0, Options->GetHeight() - Options_Dec[2]->GetHeight() + 6);
	Options_Dec[1]->SetPos(0, Options_Dec[0]->GetHeight(), -1, Options->GetHeight() - Options_Dec[0]->GetHeight() - Options_Dec[2]->GetHeight() + 6);
	Options_Close->SetPos((Options->GetWidth() - Options_Close->GetWidth()) / 2, Options->GetHeight() - Options_Dec[2]->GetHeight());
	Options_Close->OnClick = EventOptions;
	Options_Close->SetParam(0);
	Options_Res->OnChange = EventResChange;
	Options_FullScreen->SetText(TEXT("Full Screen"));
	Options_FullScreen->OnChange = EventResChange;

	std::list<_tagResolution> ResDisp = eng->GetResolutionList();
	int k = 0;
	while (!ResDisp.empty())
	{
		_tagResolution res = ResDisp.front();
		Options_Res->AddElement(res.szFormat, res.Format);
		if (k == eng->GetStatus()->VideoMode) Options_Res->SetSelected(k);
		k++;
		ResDisp.pop_front();
	}
}
void InGameUI()
{
	// In Game TextureManager->GUI()
	InGame = new CWindow(TEXT("GameUI"), 0, 0, Interface->GetDesktop());
	Position = new CEdit(0, 0, TextureManager->GetTexture(TEXT("POSITION")), InGame);
	// In Game Chat
	ChatWindow = new CWindow(TEXT("Chat"), 0, 100, InGame, TextureManager->GetTexture(TEXT("CHAT")));

	Chat = new CCheckBox(0, 0, nullptr, TextureManager->GetTexture(TEXT("CHATONN")), ChatWindow);
	ChatParty = new CCheckBox(Chat->GetWidth(), 0, nullptr, TextureManager->GetTexture(TEXT("CHATONP")), ChatWindow);
	ChatGuild = new CCheckBox(ChatParty->GetX() + ChatParty->GetWidth(), 0, nullptr, TextureManager->GetTexture(TEXT("CHATONG")), ChatWindow);

	ChatWisper = new CCheckBox(ChatGuild->GetX() + ChatGuild->GetWidth() + 6, 0, nullptr, TextureManager->GetTexture(TEXT("CHATONW")), ChatWindow);
	ChatSystem = new CCheckBox(ChatWisper->GetX() + ChatWisper->GetWidth(), 0, nullptr, TextureManager->GetTexture(TEXT("CHATONS")), ChatWindow);
	ChatNormal = new CCheckBox(ChatSystem->GetX() + ChatSystem->GetWidth(), 0, nullptr, TextureManager->GetTexture(TEXT("CHATON")), ChatWindow);

	ChatFrame = new CCheckBox(ChatNormal->GetX() + ChatNormal->GetWidth() + 5, 0, nullptr, TextureManager->GetTexture(TEXT("CHATONF")), ChatWindow);
	ChatSize = new CButton(ChatFrame->GetX() + ChatFrame->GetWidth(), 0, TextureManager->GetTexture(TEXT("CHATSIZE")), 2, ChatWindow);
	ChatAlpha = new CButton(ChatSize->GetX() + ChatSize->GetWidth(), 0, TextureManager->GetTexture(TEXT("CHATALPHA")), 2, ChatWindow);

	ChatWisp = new CEdit(0, ChatSize->GetHeight() + 1, TextureManager->GetTexture(TEXT("CHATWISP")), ChatWindow);
	ChatText = new CEdit(ChatWisp->GetWidth(), ChatSize->GetHeight() + 1, TextureManager->GetTexture(TEXT("CHATMAIN")), ChatWindow);
	ChatText->SetTexture(nullptr);

	Chat->OnChange = EventChatBox;
	ChatParty->OnChange = EventChatBox;
	ChatGuild->OnChange = EventChatBox;
	ChatWisper->OnChange = EventChatBox;
	ChatSystem->OnChange = EventChatBox;
	ChatNormal->OnChange = EventChatBox;
	ChatFrame->OnChange = EventChatBox;
	ChatAlpha->OnClick = EventChatBox;
	ChatSize->OnClick = EventChatBox;

	Status = new CWindow(TEXT("StatusBar"), 0, 0, InGame);
	Status->Show();

	Imgs[0] = new CImage(0, 0, TextureManager->GetTexture(TEXT("STATS0")), Status);
	Imgs[1] = new CImage(Imgs[0]->GetX() + Imgs[0]->GetWidth(), 0, TextureManager->GetTexture(TEXT("STATS1")), Status);
	Imgs[2] = new CImage(Imgs[1]->GetX() + Imgs[1]->GetWidth(), 0, TextureManager->GetTexture(TEXT("STATS2")), Status);
	Status->SetPos(-1, -1, Imgs[2]->GetX() + Imgs[2]->GetWidth(), Imgs[2]->GetHeight());

	ChatWindow->SetPos(0, Status->GetY() - ChatWindow->GetHeight());
	RichBox = new CRichBox(0, ChatWindow->GetY() - RichSize, ChatWindow->GetWidth(), RichSize, InGame);
	RichBox->innerText(TEXT(""));
	RichBox->Show();

	Stats = new CButton(0, 0, TextureManager->GetTexture(TEXT("STATS_BT0")), 4, Status);
	int tmp = Stats->GetWidth();
	Inventory = new CButton(Status->GetWidth() - tmp * 3, 0, TextureManager->GetTexture(TEXT("STATS_BT1")), 4, Status);
	Friends = new CButton(Status->GetWidth() - tmp * 2, 0, TextureManager->GetTexture(TEXT("STATS_BT2")), 4, Status);
	GameMenu = new CButton(Status->GetWidth() - tmp * 1, 0, TextureManager->GetTexture(TEXT("STATS_BT3")), 4, Status);
	Stats->SetPos(Status->GetWidth() - tmp * 4, 0);
	Inventory->OnClick = EventUI;
	Friends->OnClick = EventUI;
	GameMenu->OnClick = EventUI;
	Stats->OnClick = EventUI;
	Stats->SetParam(0);
	Inventory->SetParam(1);
	Friends->SetParam(2);
	GameMenu->SetParam(3);

	//384
	SD = new CProgressBar(TEXT("Shield"), nullptr, TextureManager->GetTexture(TEXT("SD")), 204, 3, Status);
	HP = new CProgressBar(TEXT("Health"), nullptr, TextureManager->GetTexture(TEXT("HP")), 158, 3, Status);
	MP = new CProgressBar(TEXT("Mana"), nullptr, TextureManager->GetTexture(TEXT("MP")), 438, 3, Status);
	AG = new CProgressBar(TEXT("Stamina"), nullptr, TextureManager->GetTexture(TEXT("AG")), 420, 3, Status);
	HP->VerticalMode(true);
	MP->VerticalMode(true);
	SD->VerticalMode(true);
	AG->VerticalMode(true);
	HP->SetMax(100);
	MP->SetMax(100);
	SD->SetMax(100);
	AG->SetMax(100);
	HP->SetProgress(80);
	MP->SetProgress(80);
	SD->SetProgress(80);
	AG->SetProgress(80);

	WarpMenu = new CMenu(0, 50, 200, 150, InGame);
	WarpMenu->OnSelect = EventWarp;

	WarpMenu->Hide();
	for (int i = 0; i < MoveReq.Count(); i++)
	{
		_MoveReqBlock pBlock = *MoveReq.Get(i);
		WarpMenu->AddElement(CTString(pBlock.NameWarp), pBlock.dwGate);
	}
}

void CharacterInvUI()
{
	winInventory = new CWindow(TEXT("Inventory"), 0, 0, InGame, TextureManager->GetTexture(TEXT("BACKGROUND")));
	imgInv[0] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_TOP")), winInventory);
	imgInv[1] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_MID_L")), winInventory);
	imgInv[2] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_MID_R")), winInventory);
	imgInv[3] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_BOT")), winInventory);
	imgInv[4] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_TOP2")), winInventory);

	imgInv[1]->SetPos(0, imgInv[0]->GetHeight());
	imgInv[2]->SetPos(imgInv[0]->GetWidth()- imgInv[2]->GetWidth(), imgInv[0]->GetHeight());
	imgInv[3]->SetPos(0, imgInv[2]->GetY() + imgInv[2]->GetHeight());
	winInventory->SetPos(0, 0, imgInv[3]->GetWidth(), imgInv[3]->GetY() + imgInv[3]->GetHeight());

	int yoff = imgInv[1]->GetY()-20;

	for (int i = 0; i < 64; i++)
	{
		invBox[i] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_UNUSE")), winInventory);
		invBox[i]->SetPos((i%8)*invBox[i]->GetWidth()+10,yoff+(i/8)*invBox[i]->GetHeight()+150);
	}

	invFairy = new CImage(10, yoff, TextureManager->GetTexture(TEXT("INV_FAIRY")), winInventory);
	invWeapon[0] = new CImage(10, invFairy->GetY() + invFairy->GetHeight()-5, TextureManager->GetTexture(TEXT("INV_WEAPON_L")), winInventory);
	invGloves = new CImage(10, invWeapon[0]->GetY()+ invWeapon[0]->GetHeight()-4, TextureManager->GetTexture(TEXT("INV_GLOVES")), winInventory);
	invNeckleace = new CImage(invWeapon[0]->GetX()+ invWeapon[0]->GetWidth()-5, invWeapon[0]->GetY(), TextureManager->GetTexture(TEXT("INV_NECKLEACE")), winInventory);
	invHelm = new CImage(invNeckleace->GetWidth()+ invNeckleace->GetX()-5, invFairy->GetY(), TextureManager->GetTexture(TEXT("INV_HELM")), winInventory);
	învWings = new CImage(invHelm->GetX() + invHelm->GetWidth()+5, invHelm->GetY(), TextureManager->GetTexture(TEXT("INV_WINGS")), winInventory);
	invArmor = new CImage(invHelm->GetX(), invWeapon[0]->GetY(), TextureManager->GetTexture(TEXT("INV_ARMOR")), winInventory);
	invWeapon[1] = new CImage(invNeckleace->GetWidth() + invHelm->GetX() + invHelm->GetWidth()-10, invArmor->GetY(), TextureManager->GetTexture(TEXT("INV_WEAPON_R")), winInventory);
	invPants = new CImage(invArmor->GetX(), invGloves->GetY(), TextureManager->GetTexture(TEXT("INV_PANTS")), winInventory);
	invBoots = new CImage(invWeapon[1]->GetX(), invPants->GetY(), TextureManager->GetTexture(TEXT("INV_GLOVES")), winInventory);
	invRings[0] = new CImage(invNeckleace->GetX(), invPants->GetY(), TextureManager->GetTexture(TEXT("INV_RING")), winInventory);
	invRings[1] = new CImage(invHelm->GetX()+ invHelm->GetWidth()-5, invPants->GetY(), TextureManager->GetTexture(TEXT("INV_RING")), winInventory);

	invMoney = new CImage(10, invBox[63]->GetY()+ invBox[63]->GetHeight(), TextureManager->GetTexture(TEXT("INV_MONEY")), winInventory);
	invExit1 = new CButton(10, invMoney->GetY() + invMoney->GetHeight(), TextureManager->GetTexture(TEXT("BT_EXIT")), 2, winInventory);
	invExit1->OnClick = EventUI;
	invExit1->SetParam(1);

	invBorder[0] = new CImage(8, yoff+150, TextureManager->GetTexture(TEXT("INV_B_UP_LEFT")), winInventory);
	invBorder[1] = new CImage(winInventory->GetWidth()-8- invBorder[0]->GetWidth(), invBorder[0]->GetY(), TextureManager->GetTexture(TEXT("INV_B_UP_RIGHT")), winInventory);
	invBorder[2] = new CImage(invBorder[0]->GetX(), invBox[63]->GetY()+ invBox[63]->GetHeight()- invBorder[0]->GetHeight()+5, TextureManager->GetTexture(TEXT("INV_B_DW_LEFT")), winInventory);
	invBorder[3] = new CImage(invBorder[1]->GetX(), invBorder[2]->GetY(), TextureManager->GetTexture(TEXT("INV_B_DW_RIGHT")), winInventory);

	invBorder[4] = new CImage(invBorder[0]->GetX() + invBorder[0]->GetWidth(), invBorder[0]->GetY(), TextureManager->GetTexture(TEXT("INV_B_UP")), winInventory);
	invBorder[5] = new CImage(invBorder[1]->GetX(), invBorder[1]->GetY()+invBorder[1]->GetHeight(), TextureManager->GetTexture(TEXT("INV_B_RIGHT")), winInventory);
	invBorder[6] = new CImage(invBorder[2]->GetX() + invBorder[2]->GetWidth(), invBorder[2]->GetY(), TextureManager->GetTexture(TEXT("INV_B_DW")), winInventory);
	invBorder[7] = new CImage(invBorder[0]->GetX(), invBorder[0]->GetY() + invBorder[0]->GetHeight(), TextureManager->GetTexture(TEXT("INV_B_LEFT")), winInventory);

	invBorder[4]->SetPos(-1, -1, invBorder[1]->GetX() - invBorder[4]->GetX());
	invBorder[5]->SetPos(-1, -1, -1, invBorder[3]->GetY() - invBorder[5]->GetY());
	invBorder[6]->SetPos(-1, -1, invBorder[4]->GetWidth());
	invBorder[7]->SetPos(-1, -1, -1, invBorder[5]->GetHeight());
}
void CharacterInfoUI()
{
	winCharacter = new CWindow(TEXT("Character"), 0, 0, InGame, TextureManager->GetTexture(TEXT("BACKGROUND")));
	imgChar[0] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_TOP")), winCharacter);
	imgChar[1] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_MID_L")), winCharacter);
	imgChar[2] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_MID_R")), winCharacter);
	imgChar[3] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_BOT")), winCharacter);
	imgChar[4] = new CImage(0, 0, TextureManager->GetTexture(TEXT("INV_TOP2")), winCharacter);

	imgChar[1]->SetPos(0, imgChar[0]->GetHeight());
	imgChar[2]->SetPos(imgChar[0]->GetWidth() - imgChar[2]->GetWidth(), imgChar[0]->GetHeight());
	imgChar[3]->SetPos(0, imgChar[2]->GetY() + imgChar[2]->GetHeight());
	winCharacter->SetPos(0, 0, imgChar[3]->GetWidth(), imgChar[3]->GetY() + imgChar[3]->GetHeight());
}

void StartMuUI()
{
	eng->GUI->BindKey(VK_RETURN, EventKeyInGame);
	eng->GUI->BindKey(VK_ESCAPE, EventKeyInGame);
	eng->GUI->BindKey('Q', EventKeyInGame);
	eng->GUI->BindKey('W', EventKeyInGame);
	eng->GUI->BindKey('E', EventKeyInGame);
	eng->GUI->BindKey('R', EventKeyInGame);
	eng->GUI->BindKey('I', EventKeyInGame);
	eng->GUI->BindKey('C', EventKeyInGame);
	eng->GUI->BindKey('1', EventKeyInGame);
	eng->GUI->BindKey('2', EventKeyInGame);
	eng->GUI->BindKey('3', EventKeyInGame);
	eng->GUI->BindKey('4', EventKeyInGame);
	eng->GUI->BindKey('5', EventKeyInGame);
	eng->GUI->BindKey('6', EventKeyInGame);
	eng->GUI->BindKey('7', EventKeyInGame);
	eng->GUI->BindKey('8', EventKeyInGame);
	eng->GUI->BindKey('9', EventKeyInGame);
	eng->GUI->BindKey('0', EventKeyInGame);
	eng->GUI->BindKey('M', EventKeyInGame);
	eng->GUI->BindKey('C', EventKeyInGame);
	eng->GUI->BindKey('I', EventKeyInGame);

	for (int i = 0; i < 1000; i++)
	{
		if (szInterface[i][0] == nullptr) break;
		TextureManager->Add(szInterface[i][0], szInterface[i][1]);
	}
	for (int i = 0; i < 6; i++)
	{
		Faces[i] = new CModel(szFaces[i]);
		//Faces[i]->SetName(szClass[i]);
		Faces[i]->SetName(Text.String(TextFileClass[i]));
		eng->AddLoadQueueElement(Faces[i]);
	}
	TextureManager->Load();

	g_ScrollInfo.Top = TextureManager->GetTexture(TEXT("SCROLLTOP"));
	g_ScrollInfo.Bot = TextureManager->GetTexture(TEXT("SCROLLBOT"));
	g_ScrollInfo.Mid = TextureManager->GetTexture(TEXT("SCROLLMID"));
	g_ScrollInfo.ScrollOn = TextureManager->GetTexture(TEXT("SCROLLON"));
	g_ScrollInfo.ScrollOff = TextureManager->GetTexture(TEXT("SCROLLOFF"));

	SelecServerUI();
	SelectCharacterUI();
	InGameUI();
	CharacterInvUI();
	CharacterInfoUI();
	MenuAndOptionsUI();

	ShowServer();
}

void ResetMuUI()
{
	int bw, bh;
	eng->GetDevInfo(bw, bh);
	Server_Menu->SetPos(-1, bh - 50);
	Server_Credits->SetPos(bw - 100, bh - 50);
	Server_Deco->SetPos(bw - Server_Deco->GetWidth(), bh - Server_Deco->GetHeight());
	Character_Deco->SetPos(bw - Character_Deco->GetWidth(), bh - Character_Deco->GetHeight());
	MULogo->SetPos((bw - MULogo->GetWidth()) / 2, 20);
	Login->SetPos((bw - Login->GetWidth()) / 2, bh / 2);
	Connect->SetPos(bw - 220, 0);
	Delete->SetPos(bw - 160, 0);
	CharacterSelect->SetPos(-1, bh - 50);
	Connect->SetPos(bw - 220, -1);
	Delete->SetPos(bw - 160, -1);
	CharacterCreate->SetPos((bw - 454) / 2, (bh - 372) / 2);
	winInventory->SetPos(bw - winInventory->GetWidth(), (bh - Status->GetHeight() - winInventory->GetHeight()));
	Client_version->SetPos(0, bh - 20);
	Client_Copyright->SetPos(bw / 2 - Client_Copyright->GetWidth(), bh - 20);
	Client_Rights->SetPos(bw / 2, bh - 20);

	Status->SetPos((bw - Status->GetWidth()) / 2, bh - Status->GetHeight());
	ChatWindow->SetPos(0, Status->GetY() - ChatWindow->GetHeight());
	RichBox->SetPos(0, ChatWindow->GetY() - RichSize, ChatWindow->GetWidth(), RichSize);

	int tmph = (Menu_bt[0]->GetHeight() + 5) * 5 + 50;
	Menu->SetPos((bw - Menu->GetWidth()) / 2, (bh - tmph) / 2, -1, tmph);
	Options->SetPos((bw - Options->GetWidth()) / 2, 100);
}

void ShowServer()
{
	ServerScreen->Show();
	CharacterSelect->Hide();
	InGame->Hide();
}

void ShowCharacterSelect()
{
	ServerScreen->Hide();
	CharacterSelect->Show();
	InGame->Hide();
}

void ShowUI()
{
	ServerScreen->Hide();
	CharacterSelect->Hide();
	InGame->Show();
}