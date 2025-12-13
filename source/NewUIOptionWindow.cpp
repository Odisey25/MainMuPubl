// NewUIOptionWindow.cpp: implementation of the CNewUIOptionWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "DSPlaySound.h"
#include "ZzzInterface.h"
#include "CBInterface.h"
#include "Protocol.h"
#include <Util.h>
using namespace SEASON3B;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SEASON3B::CNewUIOptionWindow::CNewUIOptionWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bAutoAttack = true;
	m_bWhisperSound = false;
	m_bSlideHelp = true;
	m_iVolumeLevel = 0;
	m_iRenderLevel = 4;
	for (int i = 0; i < eEndOnOffGrap; i++) this->OnOffGrap[i] = true;

	//==Graphics
	this->OnOffGrap[eLowSettings] = GetPrivateProfileInt("Graphics", "LowSettings", 0, ".\\config.ini");
	this->OnOffGrap[eMediumSettings] = GetPrivateProfileInt("Graphics", "MediumSettings", 0, ".\\config.ini");
	this->OnOffGrap[eHighSettings] = GetPrivateProfileInt("Graphics", "HighSettings", 1, ".\\config.ini");
	this->OnOffGrap[eGlowEffect] = GetPrivateProfileInt("Graphics", "GlowEffect", 1, ".\\config.ini");
	this->OnOffGrap[eEffectDynamic] = GetPrivateProfileInt("Graphics", "EffectDynamic", 1, ".\\config.ini");
	this->OnOffGrap[eEffectStatic] = GetPrivateProfileInt("Graphics", "EffectStatic", 1, ".\\config.ini");
	this->OnOffGrap[eBMDPlayer] = GetPrivateProfileInt("Graphics", "BMDPlayer", 1, ".\\config.ini");
	this->OnOffGrap[eBMDWings] = GetPrivateProfileInt("Graphics", "BMDWings", 1, ".\\config.ini");
	this->OnOffGrap[eBMDWeapons] = GetPrivateProfileInt("Graphics", "BMDWeapons", 1, ".\\config.ini");
	this->OnOffGrap[eBMDImg] = GetPrivateProfileInt("Graphics", "BMDImg", 1, ".\\config.ini");
	this->OnOffGrap[eBMDMonter] = GetPrivateProfileInt("Graphics", "BMDMonter", 1, ".\\config.ini");
	this->OnOffGrap[eRenderObjects] = GetPrivateProfileInt("Graphics", "RenderObjects", 1, ".\\config.ini");
	this->OnOffGrap[eRenderTerrain] = GetPrivateProfileInt("Graphics", "RenderTerrain", 1, ".\\config.ini");
	this->OnOffGrap[eExcellentEffect] = GetPrivateProfileInt("Graphics", "ExcellentEffect", 1, ".\\config.ini");
	this->OnOffGrap[eBMDZen] = GetPrivateProfileInt("Graphics", "BMDZen", 1, ".\\config.ini");
}

SEASON3B::CNewUIOptionWindow::~CNewUIOptionWindow()
{
	Release();
}

bool SEASON3B::CNewUIOptionWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_OPTION, this);
	SetPos(x, y);
	LoadImages();
	SetButtonInfo();
	Show(false);
	return true;
}

void SEASON3B::CNewUIOptionWindow::SetButtonInfo()
{
	//m_BtnClose.ChangeTextBackColor(RGBA(255,255,255,0));
	//m_BtnClose.ChangeButtonImgState( true, IMAGE_OPTION_BTN_CLOSE, true );
	//m_BtnClose.ChangeButtonInfo(m_Pos.x+68, m_Pos.y+209, 54, 30);
	//m_BtnClose.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	//m_BtnClose.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
}

void SEASON3B::CNewUIOptionWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}


void SEASON3B::CNewUIOptionWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIOptionWindow::UpdateMouseEvent()
{
	if (m_BtnClose.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
		return false;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 65, 15, 15))
	{
		m_bWhisperSound = !m_bWhisperSound;
	}

	// Effect level - adjusted position
	if (CheckMouseIn(m_Pos.x + 25, m_Pos.y + 167, 141, 29))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 25);
			float fValue = (5.f * x) / 141.f;
			m_iRenderLevel = (int)fValue;
		}
	}

	// Volume controller - adjusted position
	if (CheckMouseIn(m_Pos.x + 33 - 8, m_Pos.y + 380, 124 + 8, 16))
	{
		int iOldValue = m_iVolumeLevel;
		if (MouseWheel > 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel++;
			if (m_iVolumeLevel > 10)
			{
				m_iVolumeLevel = 10;
			}
		}
		else if (MouseWheel < 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel--;
			if (m_iVolumeLevel < 0)
			{
				m_iVolumeLevel = 0;
			}
		}
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 33);
			if (x < 0)
			{
				m_iVolumeLevel = 0;
			}
			else
			{
				float fValue = (10.f * x) / 124.f;
				m_iVolumeLevel = (int)fValue + 1;
			}
		}

		if (iOldValue != m_iVolumeLevel)
		{
			SetEffectVolumeLevel(m_iVolumeLevel);
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, 190, 420) == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIOptionWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderCustomFrame();
	RenderButtons();
	DisableAlphaBlend();
	return true;
}

float SEASON3B::CNewUIOptionWindow::GetLayerDepth()	//. 10.5f
{
	return 10.5f;
}

float SEASON3B::CNewUIOptionWindow::GetKeyEventOrder()	// 10.f;
{
	return 10.0f;
}

void SEASON3B::CNewUIOptionWindow::OpenningProcess()
{
	gInterface.Data[eMenu_OPTION].Open();
}

void SEASON3B::CNewUIOptionWindow::ClosingProcess()
{
	gInterface.Data[eMenu_OPTION].Close();
}

void SEASON3B::CNewUIOptionWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_button_close.tga", IMAGE_OPTION_BTN_CLOSE, GL_LINEAR);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_OPTION_FRAME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_OPTION_FRAME_DOWN, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_top.tga", IMAGE_OPTION_FRAME_UP, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_back06(L).tga", IMAGE_OPTION_FRAME_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_back06(R).tga", IMAGE_OPTION_FRAME_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_line.jpg", IMAGE_OPTION_LINE, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_point.tga", IMAGE_OPTION_POINT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_check.tga", IMAGE_OPTION_BTN_CHECK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_effect03.tga", IMAGE_OPTION_EFFECT_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_effect04.tga", IMAGE_OPTION_EFFECT_COLOR, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_VOLUME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_VOLUME_COLOR, GL_LINEAR);
}

void SEASON3B::CNewUIOptionWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_OPTION_BTN_CLOSE);
	DeleteBitmap(IMAGE_OPTION_FRAME_BACK);
	DeleteBitmap(IMAGE_OPTION_FRAME_DOWN);
	DeleteBitmap(IMAGE_OPTION_FRAME_UP);
	DeleteBitmap(IMAGE_OPTION_FRAME_LEFT);
	DeleteBitmap(IMAGE_OPTION_FRAME_RIGHT);
	DeleteBitmap(IMAGE_OPTION_LINE);
	DeleteBitmap(IMAGE_OPTION_POINT);
	DeleteBitmap(IMAGE_OPTION_BTN_CHECK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_BACK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_COLOR);
	DeleteBitmap(IMAGE_OPTION_VOLUME_BACK);
	DeleteBitmap(IMAGE_OPTION_VOLUME_COLOR);
}

void SEASON3B::CNewUIOptionWindow::RenderFrame()
{
	if (!gInterface.Data[eMenu_OPTION].OnShow)
	{
		g_pNewUISystem->Hide(INTERFACE_OPTION);
		return;
	}

	float MainWidth = 220;
	float MainHeight = 420;

	// Static variables to maintain window position
	static float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	static float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2);

	// Static variables to check resolution changes
	static int lastDisplayHeight = 0;
	static int lastDisplayWin = 0;

	// If resolution changed, reset window position
	if (lastDisplayHeight != DisplayHeight || lastDisplayWin != DisplayWin)
	{
		lastDisplayHeight = DisplayHeight;
		lastDisplayWin = DisplayWin;

		// Recalculate center position
		StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
		StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2);
	}

	// Draw movable window and update its position
	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, eMenu_OPTION, "Options");

	StartY += 5;

	// Adjust window position
	SetPos(StartX, StartY);

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	// Background boxes with adjusted positions
	g_pBCustomMenuInfo->gInfoBox(m_Pos.x + 17.0f, m_Pos.y + 50.0f, 190, 40, 0x00000060, 0, 0);    // Graphic quality

	g_pBCustomMenuInfo->gInfoBox(m_Pos.x + 17.0f, m_Pos.y + 95.0f, 190, 60, 0x00000060, 0, 0);    // Graphic effects (checkboxes)

	g_pBCustomMenuInfo->gInfoBox(m_Pos.x + 17.0f, m_Pos.y + 160.0f, 190, 40, 0x00000060, 0, 0);   // Effect level (bar)

	g_pBCustomMenuInfo->gInfoBox(m_Pos.x + 17.0f, m_Pos.y + 205.0f, 190, 90, 0x00000060, 0, 0);   // Graphic options

	g_pBCustomMenuInfo->gInfoBox(m_Pos.x + 17.0f, m_Pos.y + 300.0f, 190, 60, 0x00000060, 0, 0);   // Combat options

	g_pBCustomMenuInfo->gInfoBox(m_Pos.x + 17.0f, m_Pos.y + 365.0f, 190, 40, 0x00000060, 0, 0);   // Volume controller

	DisableAlphaBlend();
}

extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayHeightExt;
void change_resolution(int index)
{
	switch (index) //Resolution
	{
	case 0:WindowWidth = 640; WindowHeight = 400; break;
	case 1:WindowWidth = 800; WindowHeight = 500; break;
	case 2:WindowWidth = 1024; WindowHeight = 640; break;
	case 3:WindowWidth = 1280; WindowHeight = 800; break;
	case 4:WindowWidth = 1360; WindowHeight = 850; break;
	case 5:WindowWidth = 1440; WindowHeight = 900; break;
	case 6:WindowWidth = 1536; WindowHeight = 960; break;
	case 7:WindowWidth = 1680; WindowHeight = 1050; break;
	case 8:WindowWidth = 1920; WindowHeight = 1080; break;
	case 9:WindowWidth = 1920; WindowHeight = 1200; break;
	default:WindowWidth = 1920; WindowHeight = 1080; break;
	}
	g_fScreenRate_x = (float)1.25;
	g_fScreenRate_y = (float)1.25;

	DisplayWinCDepthBox = (int)WindowWidth / g_fScreenRate_y - 640;
	DisplayWin = (int)WindowWidth / g_fScreenRate_y;
	DisplayWinMid = (int)WindowWidth / g_fScreenRate_y / 2;
	DisplayWinExt = (int)((DisplayWin * 0.5) - 320.0);
	DisplayWinReal = (int)DisplayWin / 640.0;

	DisplayHeight = (int)((double)WindowHeight / g_fScreenRate_y);
	DisplayHeightExt = (int)((double)DisplayHeight - 480.0);

	RECT rc = { 0, 0, WindowWidth, WindowHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, NULL);

	rc.right -= rc.left;
	rc.bottom -= rc.top;

	SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN);

	SetWindowPos(g_hWnd, HWND_NOTOPMOST, (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
		rc.right,
		rc.bottom, SWP_SHOWWINDOW);

	BGetScreenWidth();

	HKEY key;
	RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key);

	RegSetValueEx(key, "Resolution", 0, REG_DWORD, (const BYTE*)&index, sizeof(index));

	RegCloseKey(key);
}

//==Custom Config
void SEASON3B::CNewUIOptionWindow::RenderCustomFrame()
{
	float x, y;

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	// 1. GRAPHIC QUALITY
	x = m_Pos.x + 5;
	y = m_Pos.y + 50.f;
	g_pRenderText->RenderText(x + 30, y + 5, "Graphic Quality");

	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15, y + 15, 0xFFCC00C8, this->OnOffGrap[eLowSettings] == 1 ? TRUE : FALSE, "Low"))
	{
		this->OnOffGrap[eLowSettings] = 1;
		this->OnOffGrap[eMediumSettings] = 0;
		this->OnOffGrap[eHighSettings] = 0;
		this->OnOffGrap[eGlowEffect] = 0;
		this->OnOffGrap[eEffectDynamic] = 0;
		this->OnOffGrap[eEffectStatic] = 0;
		this->OnOffGrap[eExcellentEffect] = 0;

		char string[10];
		WritePrivateProfileStringA("Graphics", "LowSettings", "1", "./config.ini");
		WritePrivateProfileStringA("Graphics", "MediumSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "HighSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "GlowEffect", itoa(this->OnOffGrap[eGlowEffect], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectDynamic", itoa(this->OnOffGrap[eEffectDynamic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectStatic", itoa(this->OnOffGrap[eEffectStatic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "ExcellentEffect", itoa(this->OnOffGrap[eExcellentEffect], string, 10), "./config.ini");
	}

	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 70, y + 15, 0xFFCC00C8, this->OnOffGrap[eMediumSettings] == 1 ? TRUE : FALSE, "Medium"))
	{
		this->OnOffGrap[eLowSettings] = 0;
		this->OnOffGrap[eMediumSettings] = 1;
		this->OnOffGrap[eHighSettings] = 0;
		this->OnOffGrap[eGlowEffect] = 1;
		this->OnOffGrap[eEffectDynamic] = 0;
		this->OnOffGrap[eEffectStatic] = 0;
		this->OnOffGrap[eExcellentEffect] = 1;

		char string[10];
		WritePrivateProfileStringA("Graphics", "LowSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "MediumSettings", "1", "./config.ini");
		WritePrivateProfileStringA("Graphics", "HighSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "GlowEffect", itoa(this->OnOffGrap[eGlowEffect], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectDynamic", itoa(this->OnOffGrap[eEffectDynamic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectStatic", itoa(this->OnOffGrap[eEffectStatic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "ExcellentEffect", itoa(this->OnOffGrap[eExcellentEffect], string, 10), "./config.ini");
	}

	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 145, y + 15, 0xFFCC00C8, this->OnOffGrap[eHighSettings] == 1 ? TRUE : FALSE, "High"))
	{
		this->OnOffGrap[eLowSettings] = 0;
		this->OnOffGrap[eMediumSettings] = 0;
		this->OnOffGrap[eHighSettings] = 1;
		this->OnOffGrap[eGlowEffect] = 1;
		this->OnOffGrap[eEffectDynamic] = 1;
		this->OnOffGrap[eEffectStatic] = 1;
		this->OnOffGrap[eExcellentEffect] = 1;

		char string[10];
		WritePrivateProfileStringA("Graphics", "LowSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "MediumSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "HighSettings", "1", "./config.ini");
		WritePrivateProfileStringA("Graphics", "GlowEffect", itoa(this->OnOffGrap[eGlowEffect], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectDynamic", itoa(this->OnOffGrap[eEffectDynamic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectStatic", itoa(this->OnOffGrap[eEffectStatic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "ExcellentEffect", itoa(this->OnOffGrap[eExcellentEffect], string, 10), "./config.ini");
	}

	// 2. GRAPHIC EFFECTS (checkboxes)
	y = m_Pos.y + 95.f;
	g_pRenderText->RenderText(x + 30, y + 5, "Graphic Effects");

	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15, y + 15, 0xFFCC00C8, this->OnOffGrap[eGlowEffect] == 1 ? TRUE : FALSE, "Glow Effect"))
	{
		this->OnOffGrap[eGlowEffect] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eEffectDynamic] == 1 ? TRUE : FALSE, "Effect Dynamic"))
	{
		this->OnOffGrap[eEffectDynamic] ^= 1;
	}

	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15, y + 32, 0xFFCC00C8, this->OnOffGrap[eEffectStatic] == 1 ? TRUE : FALSE, "Effect Static"))
	{
		this->OnOffGrap[eEffectStatic] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 80, y + 32, 0xFFCC00C8, this->OnOffGrap[eExcellentEffect] == 1 ? TRUE : FALSE, "Excellent Effect"))
	{
		this->OnOffGrap[eExcellentEffect] ^= 1;
	}

	// 3. GRAPHIC OPTIONS (below effect level bar)
	y = m_Pos.y + 205.f;
	x = m_Pos.x + 25;
	g_pRenderText->RenderText(x + 10, y + 5, "Graphic Options");

	// First row
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDWings] == 1 ? TRUE : FALSE, "Hide Wings"))
	{
		this->OnOffGrap[eBMDWings] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDWeapons] == 1 ? TRUE : FALSE, "Hide Weapons"))
	{
		this->OnOffGrap[eBMDWeapons] ^= 1;
	}

	// Second row
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 32, 0xFFCC00C8, this->OnOffGrap[eBMDImg] == 1 ? TRUE : FALSE, "Reduce Lag"))
	{
		this->OnOffGrap[eBMDImg] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 32, 0xFFCC00C8, this->OnOffGrap[eBMDMonter] == 1 ? TRUE : FALSE, "Hide Monsters"))
	{
		this->OnOffGrap[eBMDMonter] ^= 1;
	}

	// Third row
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 49, 0xFFCC00C8, this->OnOffGrap[eRenderObjects] == 1 ? TRUE : FALSE, "Hide Terrain"))
	{
		this->OnOffGrap[eRenderObjects] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 49, 0xFFCC00C8, this->OnOffGrap[eRenderTerrain] == 1 ? TRUE : FALSE, "Hide Map"))
	{
		this->OnOffGrap[eRenderTerrain] ^= 1;
	}

	// Fourth row
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 66, 0xFFCC00C8, this->OnOffGrap[eBMDPlayer] == 1 ? TRUE : FALSE, "Hide Players"))
	{
		this->OnOffGrap[eBMDPlayer] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 66, 0xFFCC00C8, this->OnOffGrap[eBMDZen] == 1 ? TRUE : FALSE, "Hide Zen Drop"))
	{
		this->OnOffGrap[eBMDZen] ^= 1;
	}

	// 4. COMBAT OPTIONS
	y = m_Pos.y + 300.f;
	x = m_Pos.x + 25;

	g_pRenderText->RenderText(x + 10, y + 5, "Combat Options");

	// First row
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 15, 0xFFCC00C8, mShowHPBar == 1 ? TRUE : FALSE, "Show HP Bar"))
	{
		mShowHPBar ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, mShowName == 1 ? TRUE : FALSE, "Show Name"))
	{
		mShowName ^= 1;
		g_bGMObservation = mShowName;
	}

	// Second row
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 32, 0xFFCC00C8, gInterface.AutoPK == 1 ? TRUE : FALSE, "Auto PvP"))
	{
		gInterface.AutoPK ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 32, 0xFFCC00C8, mAutoHP == 1 ? TRUE : FALSE, "Auto HP"))
	{
		mAutoHP ^= 1;
		XULY_CGPACKET pMsg;
		pMsg.header.set(0xFC, 103, sizeof(pMsg));
		DataSend((BYTE*)&pMsg, pMsg.header.size);
	}

	DisableAlphaBlend();
}

void SEASON3B::CNewUIOptionWindow::RenderButtons()
{
	// Effect level bar - separate position
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 155, "Effect Level");

	RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x + 25, m_Pos.y + 167, 141.f, 29.f);
	if (m_iRenderLevel >= 0)
	{
		RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x + 25, m_Pos.y + 167, 141.f * 0.2f * (m_iRenderLevel + 1), 29.f);
	}

	// Volume controller - at the end and well aligned
	g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 370, "Volume");

	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 40, m_Pos.y + 380, 124.f, 16.f);
	if (m_iVolumeLevel > 0)
	{
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 40, m_Pos.y + 380, 124.f * 0.1f * (m_iVolumeLevel), 16.f);
	}
}

void SEASON3B::CNewUIOptionWindow::SetAutoAttack(bool bAuto)
{
	m_bAutoAttack = bAuto;
}

bool SEASON3B::CNewUIOptionWindow::IsAutoAttack()
{
	return m_bAutoAttack;
}

void SEASON3B::CNewUIOptionWindow::SetWhisperSound(bool bSound)
{
	m_bWhisperSound = bSound;
}

bool SEASON3B::CNewUIOptionWindow::IsWhisperSound()
{
	return m_bWhisperSound;
}

void SEASON3B::CNewUIOptionWindow::SetSlideHelp(bool bHelp)
{
	m_bSlideHelp = bHelp;
}

bool SEASON3B::CNewUIOptionWindow::IsSlideHelp()
{
	return m_bSlideHelp;
}

void SEASON3B::CNewUIOptionWindow::SetVolumeLevel(int iVolume)
{
	m_iVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetVolumeLevel()
{
	return m_iVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderLevel(int iRender)
{
	m_iRenderLevel = iRender;
}

int SEASON3B::CNewUIOptionWindow::GetRenderLevel()
{
	return m_iRenderLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderAllEffects(bool bRenderAllEffects)
{
	m_bRenderAllEffects = bRenderAllEffects;
}

bool SEASON3B::CNewUIOptionWindow::GetRenderAllEffects()
{
	return m_bRenderAllEffects;
}