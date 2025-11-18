// NewUIBCustomMenu.cpp: implementation of the CNewUIBCustomMenuInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIBCustomMenu.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "MapManager.h"
#include "SkillManager.h"
#include "CharacterManager.h"
#include "ZzzOpenglUtil.h"
#include "ZzzAI.h"
#include "Util.h"
#include "CustomJewel.h"
#include "TrayMode.h"
#include "CBInterface.h"

using namespace SEASON3B;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayWinMid;
extern int DisplayWinExt;
extern int DisplayWinReal;
using namespace SEASON3B;

DWORD BLockMouseMoveClick = 0;

DWORD BButtonClickTime = 0;

extern int TargetX, TargetY;
extern MovementSkill g_MovementSkill;
extern int SelectedCharacter;


CNewUIBCustomMenuInfo::CNewUIBCustomMenuInfo()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	BLockButtonMoveWindow = false;
	BLockButtonHover = false;
	BLockMessageBox = false;

}

CNewUIBCustomMenuInfo::~CNewUIBCustomMenuInfo()
{
	Release();
}

void CNewUIBCustomMenuInfo::DrawInfoBox(float PosX, float PosY, float Width, float Height, DWORD bkcolor, int Arg6, int Arg7)
{
	float Red = (float)((bkcolor & 0xff000000) >> 24) / 255.0f;
	float Green = (float)((bkcolor & 0x00ff0000) >> 16) / 255.0f;
	float Blue = (float)((bkcolor & 0x0000ff00) >> 8) / 255.0f;
	float Alpha = (float)(bkcolor & 0x000000ff) / 255.0f;
	EnableAlphaTest();
	if (bkcolor)
	{
		glColor4f(Red, Green, Blue, Alpha);
		RenderColor(PosX, PosY, Width + 6.5, Height + 5, 0, Arg6); //central
		EndRenderColor();
	}
	float RightX = PosX - 2.0f;
	float LeftX = (PosX + Width) - 2.0f;
	float TopY = PosY - 2.0f;
	float DownY = (PosY + Height) - 2.0f;
	bool horizontal = true;
	bool vertical = true;
	//--
	float IniciarX = PosX - 2.0f;
	float w = 14.0f;
	//--
	float IniciarY = PosY - 2.0f;
	float H = 14.0f;

	EnableAlphaTest(true);
	glColor3f(1.0, 1.0, 1.0);

	while (vertical || horizontal)
	{
		if (vertical)
		{
			IniciarX += 14.0f;

			if ((IniciarX + w) > LeftX)
			{
				vertical = false;
				w = LeftX - IniciarX;
			}
			RenderImage(IMAGE_INFO_TOP_PIXEL, IniciarX, TopY, w, 14.0f); //Ngang

			RenderImage(IMAGE_INFO_BOTTOM_PIXEL, IniciarX, DownY, w, 14.0f); //Ngang 2
		}
		//-- abajo
		if (horizontal)
		{
			IniciarY += 14.0f;

			if ((IniciarY + H) > DownY)
			{
				horizontal = false;
				H = DownY - IniciarY;
			}

			RenderImage(IMAGE_INFO_RIGHT_PIXEL, LeftX, IniciarY, 14.0f, H); //Doc 2

			RenderImage(IMAGE_INFO_LEFT_PIXEL, RightX, IniciarY, 14.0f, H); // Doc 1

			
		}
	}

	RenderImage(IMAGE_INFO_TOP_LEFT, RightX, TopY, 14.0f, 14.0f); //Goc tren ben trai

	RenderImage(IMAGE_INFO_TOP_RIGHT, LeftX, TopY, 14.0f, 14.0f); //Goc tren ben phai

	RenderImage(IMAGE_INFO_BOTTOM_LEFT, RightX, DownY, 14.0f, 14.0f);  //Goc duoi trai

	RenderImage(IMAGE_INFO_BOTTOM_RIGHT, LeftX, DownY, 14.0f, 14.0f);  //Goc Duoi phai
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	EnableAlphaTest(0);
}
bool CNewUIBCustomMenuInfo::DrawWindowBG(GUIObj* gui, LPCSTR Text, ...) {
	static float v1;
	static float v2;
	static float v3;
	static float v4;
	static float v5;
	static float v6;
	static float v7;
	static float v8;
	static float v9;
	static float v10;

	//
	if (!gui->onShow)
	{
		return false;
	}
	if (GetKeyState(VK_ESCAPE) < 0)
	{

		gui->onShow = false;
	}
	if (gui->X == -1)
	{
		float ConverX = (DisplayWin / 2) - (gui->Width / 2);
		gui->X = ConverX;
		gui->Default_X = ConverX;
		gui->Max_X = ConverX + gui->Width;
		return false;
	}
	//g_ConsoleDebug->Write(MCD_ERROR, "SHOWMENUI !!");
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	//textSize tS = this->getTextSize(Buff);


	if (SEASON3B::CheckMouseIn(gui->X, gui->Y, gui->Width, gui->Height) == 1)
	{
		BLockMouseMoveClick = GetTickCount() + 500;
	}
	if (gui->allowDrag)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow)
		{

			if (SEASON3B::CheckMouseIn(gui->X, gui->Y, gui->Width, 30))
			{
				if (!gui->dragging) {
					gui->dragging = true;
					gui->curX = MouseX;
					gui->curY = MouseY;
				}
				//g_pBCustomMenuInfo->DrawMessage(1, "Check Window : %f~%f Max(%f/%f)", gui->X, gui->Y, gui->Max_X, gui->Max_Y);
			}
			if (gui->dragging)
			{
				float CalcX = gui->X + MouseX - gui->curX;
				float CalcY = gui->Y + MouseY - gui->curY;

				gui->X += MouseX - gui->curX;
				gui->Y += MouseY - gui->curY;
				gui->curX = MouseX;
				gui->curY = MouseY;
				gui->Max_X = gui->X + gui->Width;
				gui->Max_Y = gui->Y + gui->Height;

				if (CalcX > DisplayWin - gui->Width)
				{
					gui->X = DisplayWin - gui->Width;
				}
				else if (CalcX < 0)
				{
					gui->X = 0;
				}
				else if (CalcY < 0)
				{
					gui->Y = 0;
				}
				else if (CalcY > 350)
				{
					gui->Y = 350;
				}
			}
		}
		else {
			if (gui->dragging) {
				gui->dragging = false;
			}
		}
	}


	float StartX = gui->X;
	float StartY = gui->Y;
	float CuaSoW = gui->Width;
	float CuaSoH = gui->Height;

	float ScaleW = (225 / CuaSoW) / g_fScreenRate_x;
	float ScaleH = (225 / CuaSoH) / g_fScreenRate_y;

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	DrawFrame(IMAGE_BCUSTOM_WINDOW_31322, StartX, StartY, 225, 225, 0.0, 0.0, 270, 225, ScaleW, ScaleH); //khung goc ben trai tren

	//==== NGang Tren va Duoi
	for (int i = StartX + 10; i < (StartX + CuaSoW) - 10; i += 10)
	{
		RenderImage(IMAGE_BCUSTOM_WINDOW_31353, i, StartY, 10.0, 64.0, 60.0, 0); //khung goc ben trai tren
		RenderImage(IMAGE_BCUSTOM_WINDOW_31357, i, (StartY + CuaSoH) - 45.0, 10.0, 45.0, 60.0, 0); //khung goc ben trai tren
	}

	//=== Nen va thanh doc 2 trai phai
	for (int doc = StartY + 10; doc < (StartY + CuaSoH) - 10; doc += 10)
	{
		RenderImage(IMAGE_BCUSTOM_WINDOW_31355, StartX, doc, 21.0, 10, 0, 0); //Khung giua ben trai
		RenderImage(IMAGE_BCUSTOM_WINDOW_31356, (StartX + CuaSoW) - 21.0, doc, 21.0, 10, 0, 0); //Khung giua ben phai
	}
	RenderImage(IMAGE_BCUSTOM_WINDOW_31353, StartX, StartY, 60.0, 64.0, 0, 0); //khung goc ben trai tren
	RenderImage(IMAGE_BCUSTOM_WINDOW_31353, (StartX + CuaSoW) - 60.0, StartY, 60.0, 64.0, 130.0, 0); //khung goc ben phai tren

	RenderImage(IMAGE_BCUSTOM_WINDOW_31357, StartX, (StartY + CuaSoH) - 45.0, 60.0, 45.0, 0, 0); //khung goc ben trai duoi
	RenderImage(IMAGE_BCUSTOM_WINDOW_31357, (StartX + CuaSoW) - 60.0, (StartY + CuaSoH) - 45.0, 60.0, 45.0, 130.0, 0); //khung goc ben phai duoi

	//======Close
	int CloseX = StartX + CuaSoW - 33.0f;
	int CloseY = StartY;
	RenderImage(IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 36.0f, 29.0f);
	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 36, 36) == 1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gui->lastActionTime > 300 && !BLockButtonHover && !BLockMessageBox)
		{
			PlayBuffer(25, 0, 0);
			glColor3f(1.0f, 0.0f, 0.0f);
			BLockMouseMoveClick = GetTickCount() + 500;
			gui->Clear();
		}
		RenderImage(IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 36, 29, 0, 0, RGBA(255, 204, 20, 130));
		RenderTipText(CloseX + 40, CloseY + 15, "Close");
	}
	//================================================
	TextDraw(g_hFontBold, StartX, StartY + 7, 0xFFFFFFFF, 0x0, CuaSoW, 0, 3, Buff); //Buoc Vao
	DisableAlphaBlend();

	return SEASON3B::CheckMouseIn(gui->X, gui->Y, gui->Width, gui->Height);
}

void MoveWindowsPega(int eNumWindow)
{
	static bool isDragging = false;

	if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eNumWindow].EventTick > 300)
	{
		if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, gInterface.Data[eNumWindow].Width, 30))
		{
			BLockMouseMoveClick = GetTickCount() + 500;
			if (!isDragging) {
				isDragging = true; // Bắt đầu kéo
				gInterface.Data[eNumWindow].curX = MouseX;
				gInterface.Data[eNumWindow].curY = MouseY;
			}
		}

		if (isDragging)
		{
			g_pBCustomMenuInfo->BLockButtonMoveWindow = true;
			gInterface.Data[eNumWindow].X += MouseX - gInterface.Data[eNumWindow].curX;
			gInterface.Data[eNumWindow].Y += MouseY - gInterface.Data[eNumWindow].curY;
			gInterface.Data[eNumWindow].curX = MouseX;
			gInterface.Data[eNumWindow].curY = MouseY;

		}
	}
	else
	{
		if (isDragging)
		{
			isDragging = false;
			g_pBCustomMenuInfo->BLockButtonMoveWindow = false;
		}
	}
}
void MoveWindowsPegasus(int eNumWindow)
{
	static bool isDragging = false;

	if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eNumWindow].EventTick > 300)
	{
		if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y + 15, gInterface.Data[eNumWindow].Width, 50))
		{
			BLockMouseMoveClick = GetTickCount() + 500;
			if (!isDragging) {
				isDragging = true;
				gInterface.Data[eNumWindow].curX = MouseX;
				gInterface.Data[eNumWindow].curY = MouseY;
			}
		}

		if (isDragging)
		{
			g_pBCustomMenuInfo->BLockButtonMoveWindow = true;
			gInterface.Data[eNumWindow].X += MouseX - gInterface.Data[eNumWindow].curX;
			gInterface.Data[eNumWindow].Y += MouseY - gInterface.Data[eNumWindow].curY;
			gInterface.Data[eNumWindow].curX = MouseX;
			gInterface.Data[eNumWindow].curY = MouseY;

		}
	}
	else
	{
		if (isDragging)
		{
			isDragging = false;
			g_pBCustomMenuInfo->BLockButtonMoveWindow = false;
		}
	}
}
void MoveWindows(int eNumWindow)
{
	static bool isDragging = false;

	if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eNumWindow].EventTick > 300)
	{
		if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y - 25, gInterface.Data[eNumWindow].Width, 50))
		{
			BLockMouseMoveClick = GetTickCount() + 500; 
			if (!isDragging) {
				isDragging = true; 
				gInterface.Data[eNumWindow].curX = MouseX;
				gInterface.Data[eNumWindow].curY = MouseY;
			}
		}

		if (isDragging)
		{
			g_pBCustomMenuInfo->BLockButtonMoveWindow = true;
			gInterface.Data[eNumWindow].X += MouseX - gInterface.Data[eNumWindow].curX;
			gInterface.Data[eNumWindow].Y += MouseY - gInterface.Data[eNumWindow].curY;
			gInterface.Data[eNumWindow].curX = MouseX;
			gInterface.Data[eNumWindow].curY = MouseY;

		}
	}
	else
	{
		if (isDragging)
		{
			isDragging = false; 
			g_pBCustomMenuInfo->BLockButtonMoveWindow = false;
		}
	}
}
bool CNewUIBCustomMenuInfo::gDrawMenu(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 1;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}

	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}

	float ScaleW = (CuaSoW / CuaSoW) / g_fScreenRate_x;
	float ScaleH = (CuaSoH / CuaSoH) / g_fScreenRate_y;


	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	DrawFrame(IMAGE_Frame_Menu, gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH, 0.0, 0.0, CuaSoW, CuaSoH, ScaleW, ScaleH); //khung goc ben trai tren

	DisableAlphaBlend();

	return 1;
}
bool CNewUIBCustomMenuInfo::gDrawWindowPega(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 0;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (gInterface.Data[eNumWindow].AllowMove)
	{
		MoveWindowsPega(eNumWindow);
	}


	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}

	return 1;
}
bool CNewUIBCustomMenuInfo::gDrawWindowMovePega(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 0;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (gInterface.Data[eNumWindow].AllowMove)
	{
		MoveWindowsPegasus(eNumWindow);
	}


	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}

	return 1;
}
bool CNewUIBCustomMenuInfo::gDrawWindowMove(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 0;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (gInterface.Data[eNumWindow].AllowMove)
	{
		MoveWindows(eNumWindow);
	}


	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}

	return 1;
}



bool CNewUIBCustomMenuInfo::gDrawWindowCustom(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 0;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (gInterface.Data[eNumWindow].AllowMove)
	{
		MoveWindows(eNumWindow);
	}


	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}
	float ScaleW = (225 / CuaSoW) / g_fScreenRate_x;
	float ScaleH = (225 / CuaSoH) / g_fScreenRate_y;

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	DrawFrame(IMAGE_BCUSTOM_WINDOW_31322, gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, 225, 225, 0.0, 0.0, 270, 225, ScaleW, ScaleH); //khung goc ben trai tren

	//==== NGang Tren va Duoi
	for (int i = gInterface.Data[eNumWindow].X + 10; i < (gInterface.Data[eNumWindow].X + CuaSoW) - 10; i += 10)
	{
		RenderImage(IMAGE_BCUSTOM_WINDOW_31353, i, gInterface.Data[eNumWindow].Y, 10.0, 64.0, 60.0, 0); //khung goc ben trai tren
		RenderImage(IMAGE_BCUSTOM_WINDOW_31357, i, (gInterface.Data[eNumWindow].Y + CuaSoH) - 45.0, 10.0, 45.0, 60.0, 0); //khung goc ben trai tren
	}

	//=== Nen va thanh doc 2 trai phai
	for (int doc = gInterface.Data[eNumWindow].Y + 10; doc < (gInterface.Data[eNumWindow].Y + CuaSoH) - 10; doc += 10)
	{
		RenderImage(IMAGE_BCUSTOM_WINDOW_31355, gInterface.Data[eNumWindow].X, doc, 21.0, 10, 0, 0); //Khung giua ben trai
		RenderImage(IMAGE_BCUSTOM_WINDOW_31356, (gInterface.Data[eNumWindow].X + CuaSoW) - 21.0, doc, 21.0, 10, 0, 0); //Khung giua ben phai
	}
	RenderImage(IMAGE_BCUSTOM_WINDOW_31353, gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, 60.0, 64.0, 0, 0); //khung goc ben trai tren
	RenderImage(IMAGE_BCUSTOM_WINDOW_31353, (gInterface.Data[eNumWindow].X + CuaSoW) - 60.0, gInterface.Data[eNumWindow].Y, 60.0, 64.0, 130.0, 0); //khung goc ben phai tren

	RenderImage(IMAGE_BCUSTOM_WINDOW_31357, gInterface.Data[eNumWindow].X, (gInterface.Data[eNumWindow].Y + CuaSoH) - 45.0, 60.0, 45.0, 0, 0); //khung goc ben trai duoi
	RenderImage(IMAGE_BCUSTOM_WINDOW_31357, (gInterface.Data[eNumWindow].X + CuaSoW) - 60.0, (gInterface.Data[eNumWindow].Y + CuaSoH) - 45.0, 60.0, 45.0, 130.0, 0); //khung goc ben phai duoi

	//======Close
	int CloseX = gInterface.Data[eNumWindow].X + CuaSoW - 33.0f;
	int CloseY = gInterface.Data[eNumWindow].Y;
	RenderImage(IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 36.0f, 29.0f);
	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 36, 36) == 1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eNumWindow].EventTick > 300 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow)
		{
			PlayBuffer(25, 0, 0);
			glColor3f(1.0f, 0.0f, 0.0f);
			BLockMouseMoveClick = GetTickCount() + 500;
			gInterface.Data[eNumWindow].Close();
		}
		RenderImage(IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 36, 29, 0, 0, RGBA(255, 204, 20, 130));
		RenderTipText(CloseX + 40, CloseY + 15, "Close");
	}
	//================================================
	//g_pRenderText->SetFont(g_hFontBold);
	//g_pRenderText->SetShadowText(2);
	//g_pRenderText->SetBgColor(0);
	//g_pRenderText->SetTextColor(CLRDW_DARKYELLOW);
	//g_pRenderText->RenderText(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, Buff, CuaSoW, 0, RT3_WRITE_CENTER);
	//g_pRenderText->SetShadowText(0);
	TextDraw(g_hFontBold, gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y + 10, 0xffff6cFF, 0x0, CuaSoW, 0, 3, Buff); //Buoc Vao
	DisableAlphaBlend();

	return 1;
}




bool CNewUIBCustomMenuInfo::MoveWindow(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow)
{

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 0;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (gInterface.Data[eNumWindow].AllowMove)
	{
		MoveWindows(eNumWindow);
	}
	return 1;
}

bool CNewUIBCustomMenuInfo::gDrawPegasusCongDiem(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 1;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}
	float ScaleW = (CuaSoW / CuaSoW) / g_fScreenRate_x;
	float ScaleH = (CuaSoH / CuaSoH) / g_fScreenRate_y;

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	DisableAlphaBlend();

	return 1;
}

bool CNewUIBCustomMenuInfo::gDrawPegasus(float* StartX, float* StartY, float CuaSoW, float CuaSoH, int eNumWindow, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (!gInterface.Data[eNumWindow].OnShow)
	{
		return 1;
	}
	if (!gInterface.Data[eNumWindow].FirstLoad)
	{
		gInterface.Data[eNumWindow].Width = CuaSoW;
		gInterface.Data[eNumWindow].Height = CuaSoH;
		gInterface.Data[eNumWindow].X = *StartX;
		gInterface.Data[eNumWindow].Y = *StartY;
		gInterface.Data[eNumWindow].FirstLoad = true;
	}
	else
	{
		(*StartX) = gInterface.Data[eNumWindow].X;
		(*StartY) = gInterface.Data[eNumWindow].Y;
	}


	if (SEASON3B::CheckMouseIn(gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH) == 1)
	{
		g_pBCustomMenuInfo->SetBlockCur(TRUE);
		gInterface.Data[eNumWindow].Hover = true;
	}
	else
	{
		gInterface.Data[eNumWindow].Hover = false;
	}
	float ScaleW = (CuaSoW / CuaSoW) / g_fScreenRate_x;
	float ScaleH = (CuaSoH / CuaSoH) / g_fScreenRate_y;

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	DrawFrame(IMAGE_Frame_I3, gInterface.Data[eNumWindow].X, gInterface.Data[eNumWindow].Y, CuaSoW, CuaSoH, 0.0, 0.0, CuaSoW, CuaSoH, ScaleW, ScaleH); //khung goc ben trai tren

	DisableAlphaBlend();

	return 1;
}


void CNewUIBCustomMenuInfo::RenderRanking(float PosX, float PosY, float Width, float Height, DWORD bkcolor, float bAlpha, int Arg6)
{
	EnableAlphaTest();
	float Red = (float)((bkcolor & 0xff000000) >> 24) / 255.0f;
	float Green = (float)((bkcolor & 0x00ff0000) >> 16) / 255.0f;
	float Blue = (float)((bkcolor & 0x0000ff00) >> 8) / 255.0f;
	float Alpha = (float)(bkcolor & 0x000000ff) / 255.0f;
	if (bAlpha > 0) { Alpha -= (bAlpha / 255.0f); }

	glColor4f(0.0, 0.0, 0.0, 0.4f); // Độ trong suốt 80%, nền đậm hơn
	RenderColor(PosX + 2, PosY + 1, Width - 6, Height - 6, 0.0, 0);
	EndRenderColor();
	
	
	EnableAlphaTest(0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_LEFT, PosX, PosY, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_RIGHT, (PosX + Width) - 14.f, PosY, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_LEFT, PosX, (PosY + Height) - 14.f, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_RIGHT, (PosX + Width) - 14.f, (PosY + Height) - 14.f, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_PIXEL, PosX + 6.f, PosY, (Width - 12.f), 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_RIGHT_PIXEL, (PosX + Width) - 14.f, PosY + 6.f, 14.0, (Height - 14.f));
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_PIXEL, PosX + 6.f, (PosY + Height) - 14.f, (Width - 12.f), 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_LEFT_PIXEL, PosX, (PosY + 6.f), 14.0, (Height - 14.f));
}

void CNewUIBCustomMenuInfo::RenderToolTipItem(float PosX, float PosY, float Width, float Height, DWORD bkcolor, float bAlpha, int Arg6)
{
	EnableAlphaTest();
	glColor4f(0.0, 0.0, 0.0, 0.8f); // Độ trong suốt 80%, nền đậm hơn
	RenderColor(PosX + 2, PosY + 1, Width - 6, Height - 6, 0.0, 0);
	EndRenderColor();
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_LEFT, PosX, PosY, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_RIGHT, (PosX + Width) - 14.f, PosY, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_LEFT, PosX, (PosY + Height) - 14.f, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_RIGHT, (PosX + Width) - 14.f, (PosY + Height) - 14.f, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_PIXEL, PosX + 6.f, PosY, (Width - 12.f), 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_RIGHT_PIXEL, (PosX + Width) - 14.f, PosY + 6.f, 14.0, (Height - 14.f));
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_PIXEL, PosX + 6.f, (PosY + Height) - 14.f, (Width - 12.f), 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_LEFT_PIXEL, PosX, (PosY + 6.f), 14.0, (Height - 14.f));

	EnableAlphaTest(0);

}

void SEASON3B::CNewUIBCustomMenuInfo::gBoxDropPegasus(int x, int y, int width, int height)
{
	EnableAlphaTest();
	glColor4f(0.0, 0.0, 0.0, 0.4f);
	RenderColor(x + 3.f, y + 2.f, width - 7.f, height - 7, 0.0, 0);
	EndRenderColor();

	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_LEFT, x, y, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_RIGHT, (x + width) - 14.f, y, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_LEFT, x, (y + height) - 14.f, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_RIGHT, (x + width) - 14.f, (y + height) - 14.f, 14.0, 14.0);

}

void CNewUIBCustomMenuInfo::Character(float PosX, float PosY, float Width, float Height, DWORD bkcolor, float bAlpha, int Arg6)
{
	EnableAlphaTest();
	glColor4f(0.98f, 0.98f, 0.98f, 0.1f);
	RenderColor(PosX + 3.f, PosY + 2.f, Width - 7.f, Height - 7, 0.0, 0);
	EndRenderColor();
	EnableAlphaTest(0);
}


void CNewUIBCustomMenuInfo::gInfoBox(float PosX, float PosY, float Width, float Height, DWORD bkcolor, float bAlpha, int Arg6)
{
	EnableAlphaTest();
	glColor4f(0.0, 0.0, 0.0, 0.4f);
	RenderColor(PosX + 3.f, PosY + 2.f, Width - 7.f, Height - 7, 0.0, 0);
	EndRenderColor();

	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_LEFT, PosX, PosY, 11.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_TOP_RIGHT, (PosX + Width) - 14.f, PosY, 11.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_LEFT, PosX, (PosY + Height) - 14.f, 14.0, 14.0);
	RenderImage(SEASON3B::CNewUIMuHelper::IMAGE_LIST::IMAGE_TABLE_BOTTOM_RIGHT, (PosX + Width) - 14.f, (PosY + Height) - 14.f, 14.0, 14.0);

	EnableAlphaTest(0);
}


bool CNewUIBCustomMenuInfo::DrawButtonGUI(int IDGUID, float PosX, float PosY, float SizeW, float SizeH, int TypeButton,int Stage)
{
	EnableAlphaTest(true);
	glColor3f(1.0, 1.0, 1.0);
	bool mreturn = false;
	//=====================Button 1

	if (TypeButton == 3)
	{
		if (SEASON3B::CheckMouseIn(PosX, PosY, SizeW, SizeH) == 1)
		{
			BLockMouseMoveClick = GetTickCount() + 500;

			if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eTIME].EventTick > 500 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow)
			{
				gInterface.Data[eTIME].EventTick = GetTickCount();
				PlayBuffer(25, 0, 0);
				mreturn = true;
				RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 2);
			}
			else {
				RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 1);
			}

		}
		else {
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, 0.0);
		}
	}
	else if (TypeButton == 2)
	{
		if (SEASON3B::CheckMouseIn(PosX, PosY, SizeW, SizeH) == 1)
		{
			BLockMouseMoveClick = GetTickCount() + 500;

			if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eTIME].EventTick > 500 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow)
			{
				gInterface.Data[eTIME].EventTick = GetTickCount();
				PlayBuffer(25, 0, 0);
				mreturn = true;
				RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 1);
			}
			else {
				RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, 0.0);
			}

		}
		else {
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, 0.0);
		}
	}
	else if (TypeButton == 4)
	{
		if (Stage == 1)
		{
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 1);
			mreturn = false;
		}
	}
	else
	{
		if (SEASON3B::CheckMouseIn(PosX, PosY, SizeW, SizeH) == 1)
		{
			BLockMouseMoveClick = GetTickCount() + 500;
			if (GetTickCount() - gInterface.Data[eTIME].EventTick > 500 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow) //Click
			{
				if ((GetKeyState(VK_LBUTTON) & 0x8000))
				{
					gInterface.Data[eTIME].EventTick = GetTickCount();
					PlayBuffer(25, 0, 0);
					mreturn = true;
				}
			}
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0, 0, RGBA(255, 140, 0, 255));




		}
		else
		{
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH);
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f); //Xoa mau
	DisableAlphaBlend();
	EnableAlphaTest(0);
	return mreturn;
}
bool CNewUIBCustomMenuInfo::DrawButton(float PosX, float PosY, float SizeButton, int FontSize, char* ButtonText, float mSizeButtonW, bool IsEnter)
{
	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);
	//============Button
	float SizeButtonW = SizeButton;
	float SizeButtonH = (SizeButton * 20) / 100;

	if (mSizeButtonW != 0)
	{
		SizeButtonW = mSizeButtonW;
	}
	float StartX = PosX;
	float StartY = PosY;
	bool mreturn = false;
	float ScaleX = 0.0, ScaleY = 0.227;
	//=====================Button 1
	if (SEASON3B::CheckMouseIn(StartX, StartY, SizeButtonW, SizeButtonH) == 1 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow)
	{
		BLockMouseMoveClick = GetTickCount() + 500;
		ScaleX = ScaleY;
		if (GetTickCount() - BButtonClickTime > 500) //Click
		{
			if (GetKeyState(VK_LBUTTON) & 0x8000)
			{

				ScaleX = ScaleY * 2;
				BButtonClickTime = GetTickCount();
				PlayBuffer(25, 0, 0);
				mreturn = true;
			}
		}
	}
	if ((GetKeyState(VK_RETURN) & 0x8000) && IsEnter)
	{
		//g_pBCustomMenuInfo->CloseWindow(ChatWindow);
		PlayBuffer(25, 0, 0);
		mreturn = true;

	}
	//31563
	//RenderBitmap(31563, StartX, StartY, SizeButtonW, SizeButtonH, 0.0, 0.0, 0.64999998, 0.80000001, 1, 1, 0);

	RenderBitmap(IMAGE_BCUSTOM_WINDOW_31326, StartX, StartY, SizeButtonW, SizeButtonH, 0.0, ScaleX, 0.830, ScaleY, 1, 1, 0);
	TextDraw(g_hFontBold, StartX, StartY + (SizeButtonH / 2) - (FontSize / 2.5), 0xE6FCF7FF, 0x0, SizeButtonW, 0, 3, ButtonText); //Buoc Vao
	//=====================================
	glColor3f(1.0f, 1.0f, 1.0f); //Xoa mau
	DeleteObject(ButtonText);
	DisableAlphaBlend();
	return mreturn;
}
bool CNewUIBCustomMenuInfo::RenderCheckBoxMini(float PosX, float PosY, DWORD TextColor, bool Checkis, LPCSTR Text, ...)
{
	bool mreturn = false;
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (Checkis)
	{
		//RenderImage(BITMAP_INTERFACE_CUSTOM + 6, PosX, PosY, 15.0, 15.0, 0.0, 0.0);
		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 6, PosX, PosY, 15 / g_fScreenRate_x, 15 / g_fScreenRate_y, 0.01, 0.02999999747, 0.8799999952, 0.3700000346, 1, 1, 0);
	}
	else
	{
		//RenderImage(BITMAP_INTERFACE_CUSTOM + 6, PosX, PosY, 15.0, 15.0, 0.0, 15.0);
		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 6, PosX, PosY, 15 / g_fScreenRate_x, 15 / g_fScreenRate_y, 0.01, 0.49999999747, 0.8799999952, 0.3700000346, 1, 1, 0);
	}
	TextDraw(g_hFont, PosX + (20 / g_fScreenRate_x), PosY + 3, TextColor, 0x0, 0, 0, 1, Text);

	if (SEASON3B::CheckMouseIn(PosX, PosY, 15, 15) == 1)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eTIME].EventTick > 500)
		{
			gInterface.Data[eTIME].EventTick = GetTickCount();
			PlayBuffer(25, 0, 0);
			BButtonClickTime = GetTickCount();
			mreturn = true;
		}
	}
	return mreturn;
}
bool CNewUIBCustomMenuInfo::RenderCheckBox(float PosX, float PosY, DWORD TextColor, bool Checkis, LPCSTR Text, ...)
{
	bool mreturn = false;
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (Checkis)
	{
		RenderImage(BITMAP_INTERFACE_CUSTOM + 6, PosX, PosY, 15.0, 15.0, 0.0, 0.0);
	}
	else
	{
		RenderImage(BITMAP_INTERFACE_CUSTOM + 6, PosX, PosY, 15.0, 15.0, 0.0, 15.0);
	}
	TextDraw(g_hFont, PosX + 20, PosY + 3, TextColor, 0x0, 0, 0, 1, Text);

	if (SEASON3B::CheckMouseIn(PosX, PosY, 15, 15) == 1)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eTIME].EventTick > 500)
		{
			gInterface.Data[eTIME].EventTick = GetTickCount();
			PlayBuffer(25, 0, 0);
			BButtonClickTime = GetTickCount();
			mreturn = true;
		}
	}
	return mreturn;
}
bool CNewUIBCustomMenuInfo::RenderCheckOption(float PosX, float PosY, DWORD TextColor, bool Checkis, LPCSTR Text, ...)
{
	bool mreturn = false;
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (Checkis)
	{
		RenderImage(BITMAP_INTERFACE_CUSTOM + 7, PosX, PosY, 15.0, 15.0, 0.0, 0.0);
	}
	else
	{
		RenderImage(BITMAP_INTERFACE_CUSTOM + 7, PosX, PosY, 15.0, 15.0, 0.0, 15.0);
	}
	TextDraw(g_hFont, PosX + 20, PosY + 3, TextColor, 0x0, 0, 0, 1, Text);

	if (SEASON3B::CheckMouseIn(PosX, PosY, 15, 15) == 1)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eTIME].EventTick > 500)
		{
			gInterface.Data[eTIME].EventTick = GetTickCount();
			PlayBuffer(25, 0, 0);
			BButtonClickTime = GetTickCount();
			mreturn = true;
		}
	}
	return mreturn;
}
void CNewUIBCustomMenuInfo::RenderGroupBox(int iPosX, int iPosY, int iWidth, int iHeight, int iTitleWidth/* =60 */, int iTitleHeight/* =20  */)
{
	EnableAlphaTest();

	glColor4f(0.f, 0.f, 0.f, 0.9f);
	RenderColor(float(iPosX + 3), float(iPosY + 2), float(iTitleWidth - 8), float(iTitleHeight));
	glColor4f(0.f, 0.f, 0.f, 0.6f);
	RenderColor(float(iPosX + 3), float(iPosY + 2 + iTitleHeight), float(iWidth - 7), float(iHeight - iTitleHeight - 7));

	EndRenderColor();

	RenderImage(IMAGE_B_TABLE_TOP_LEFT, iPosX, iPosY, 14, 14);
	RenderImage(IMAGE_B_TABLE_TOP_RIGHT, iPosX + iTitleWidth - 14, iPosY, 14, 14);
	RenderImage(IMAGE_B_TABLE_TOP_RIGHT, iPosX + iWidth - 14, iPosY + iTitleHeight, 14, 14);
	RenderImage(IMAGE_B_TABLE_BOTTOM_LEFT, iPosX, iPosY + iHeight - 14, 14, 14);
	RenderImage(IMAGE_B_TABLE_BOTTOM_RIGHT, iPosX + iWidth - 14, iPosY + iHeight - 14, 14, 14);

	RenderImage(IMAGE_B_TABLE_TOP_PIXEL, iPosX + 6, iPosY, iTitleWidth - 12, 14);
	RenderImage(IMAGE_B_TABLE_RIGHT_PIXEL, iPosX + iTitleWidth - 14, iPosY + 6, 14, iTitleHeight - 6);
	RenderImage(IMAGE_B_TABLE_TOP_PIXEL, iPosX + iTitleWidth - 5, iPosY + iTitleHeight, iWidth - iTitleWidth - 6, 14);
	RenderImage(IMAGE_B_TABLE_RIGHT_PIXEL, iPosX + iWidth - 14, iPosY + iTitleHeight + 6, 14, iHeight - iTitleHeight - 14);
	RenderImage(IMAGE_B_TABLE_BOTTOM_PIXEL, iPosX + 6, iPosY + iHeight - 14, iWidth - 12, 14);
	RenderImage(IMAGE_B_TABLE_LEFT_PIXEL, iPosX, iPosY + 6, 14, iHeight - 14);
}
void CNewUIBCustomMenuInfo::RederBarOptionW(int iPosX, int iPosY, int* Vaule)
{
	if (CheckMouseIn(iPosX-8, iPosY, 124 + 8, 16))
	{
		int iOldValue = *Vaule;
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (iPosX);
			if (x < 0)
			{
				*Vaule = 0;
			}
			else
			{
				float fValue = (10.f * x) / 124.f;
				*Vaule = (int)fValue + 1;
			}
		}
	}
	int SetVaule = *Vaule;
	RenderImage(IMAGE_OPTION_B_BACK, iPosX, iPosY, 124.f, 16.f);
	if (SetVaule > 0)
	{
		RenderImage(IMAGE_OPTION_B_COLOR, iPosX, iPosY, 124.f * 0.1f * (SetVaule), 16.f);
	}
}
bool CNewUIBCustomMenuInfo::RenderInputBox(float PosX, float PosY, float W, float H, char* TextSet, CUITextInputBox*& Input, UIOPTIONS UiOption,int MaxChar, bool isPass)
{
	if (!Input)
	{
		Input = new CUITextInputBox;
		Input->Init(g_hWnd, W, H, MaxChar, isPass);
		Input->SetPosition(PosX, PosY);
		Input->SetTextColor(255, 255, 230, 230);
		Input->SetBackColor(0, 0, 0, 25);
		Input->SetFont(g_hFont);
		Input->SetState(UISTATE_NORMAL);
		Input->SetOption(UiOption);
		Input->SetText(TextSet);
	}
	else
	{
		Input->Render();
		if (SEASON3B::CheckMouseIn(PosX - 3, PosY - 2.5, W+6, H+5) == 1)
		{

			if (GetKeyState(VK_LBUTTON) & 0x8000)
			{
				Input->GiveFocus(1);
			}
		}
		return  1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------
// Create
bool CNewUIBCustomMenuInfo::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_BCUSTOMMENU, this );


	SetPos(x, y);
	LoadImages();
	Show( true );


	//this->WindowMenuCustom = new GUIObj(-1, 50, 220, 295, true);
	return true;
}

void CNewUIBCustomMenuInfo::Release()
{
	UnloadImages();

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void CNewUIBCustomMenuInfo::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}
void CNewUIBCustomMenuInfo::SetBlockCur(bool Delay)
{
	if (Delay)
	{
		BLockMouseMoveClick = GetTickCount() + 300;
	}
	else
	{
		BLockMouseMoveClick = 0;
	}
	
}
bool CNewUIBCustomMenuInfo::BtnProcess()
{
	if (BLockMouseMoveClick > GetTickCount())
	{
		return true;

	}
	else 
	{
		BLockMouseMoveClick = 0;
	}
	return false;
}

bool CNewUIBCustomMenuInfo::UpdateMouseEvent()
{
	if( true == BtnProcess() )
		return false;

	
	return true;
}

bool CNewUIBCustomMenuInfo::UpdateKeyEvent()
{
	return true;
}

bool CNewUIBCustomMenuInfo::Update()
{
	return true;
}
bool CNewUIBCustomMenuInfo::UseHotKey()
{
	//if (gInterface.UseHotKey()) return false;
	return true;
}



int BuffTick = 0;
int GetSkillBuffParty(int TypeSkill)
{
	switch (TypeSkill)
	{
	case 1:
		{
			if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Fairy_Elf)
			{
				//=== khong co skill mns thi bo qua
				if (gSkillManager.FindHeroSkill((ActionSkillType)AT_SKILL_HEALING)) return AT_SKILL_HEALING;
				if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_HEAL_IMPROVED)) return MASTER_SKILL_ADD_HEAL_IMPROVED;
			}
		}
		break;
	case 2:
	{
		if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Dark_Wizard)
		{
			//=== khong co skill mns thi bo qua
			if (gSkillManager.FindHeroSkill(AT_SKILL_WIZARDDEFENSE)) return AT_SKILL_WIZARDDEFENSE;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_MANA_SHIELD_IMPROVED)) return MASTER_SKILL_ADD_MANA_SHIELD_IMPROVED;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_MANA_SHIELD_ENHANCED)) return MASTER_SKILL_ADD_MANA_SHIELD_ENHANCED;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_MANA_SHIELD_MASTERED)) return MASTER_SKILL_ADD_MANA_SHIELD_MASTERED;
		}
	}
	break;
	case 3:
	{
		if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Fairy_Elf)
		{

			if (gSkillManager.FindHeroSkill((ActionSkillType)AT_SKILL_DEFENSE)) return AT_SKILL_DEFENSE;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_GREATER_DEFENSE_IMPROVED)) return MASTER_SKILL_ADD_GREATER_DEFENSE_IMPROVED;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_GREATER_DEFENSE_ENHANCED)) return MASTER_SKILL_ADD_GREATER_DEFENSE_ENHANCED;
		}
	}
	break;
	case 4:
	{
		if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Fairy_Elf)
		{

			if (gSkillManager.FindHeroSkill((ActionSkillType)AT_SKILL_ATTACK)) return AT_SKILL_ATTACK;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_GREATER_DAMAGE_IMPROVED)) return MASTER_SKILL_ADD_GREATER_DAMAGE_IMPROVED;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_GREATER_DAMAGE_ENHANCED)) return MASTER_SKILL_ADD_GREATER_DAMAGE_ENHANCED;
		}
	}
	break;
	case 5: //Get Skill hut mau
	{
		if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Summoner)
		{
			if (gSkillManager.FindHeroSkill((ActionSkillType)AT_SKILL_ALICE_DRAINLIFE)) return AT_SKILL_ALICE_DRAINLIFE;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_DRAIN_LIFE_IMPROVED)) return MASTER_SKILL_ADD_DRAIN_LIFE_IMPROVED;
			if (gSkillManager.FindHeroSkill((ActionSkillType)MASTER_SKILL_ADD_DRAIN_LIFE_ENHANCED)) return MASTER_SKILL_ADD_DRAIN_LIFE_ENHANCED;
		}
	}
	break;
	default:
		break;
	}
	return -1;
}
bool MuHelperActiveBuff()
{
	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.AutoBuff)
	{
		for (int i = 0; i < 3; i++)
		{
			if (gSkillManager.FindHeroSkill((ActionSkillType)g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[i]) == 1 && g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[i] >0)
			{
				int iSkillType = g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[i];
				if ((g_isCharacterBuff((&Hero->Object), (eBuffState)SkillAttribute[iSkillType].Magic_Icon) == false || g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.ReBuff[i] == 0) && !Hero->SafeZone)
				{
					Hero->CurrentSkill = gSkillManager.FindHeroCurrentSkill((ActionSkillType)iSkillType);
				
					SelectedCharacter = FindCharacterIndex(Hero->Key);
					g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Stage++;		
					g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.ReBuff[i] = 1;
					return 0;
				}
			}
		}
	}
	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PartyAutoBuff) //Neu tick vao party buff
	{
		if (GetTickCount() >= g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.TimerBuffParty)

			//===Buff Party
			for (int i = 0; i < PartyNumber; i++)
			{
				if (Party[i].index >= 0 && Party[i].index < MAX_CHARACTERS_CLIENT)
				{
					CHARACTER* pChar = &CharactersClient[Party[i].index];
					OBJECT* pObj = &pChar->Object;
					int iSkillType = -1;
					if (pChar->SafeZone) continue;
					if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Dark_Wizard)
					{
						iSkillType = GetSkillBuffParty(2);
					}
					if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Fairy_Elf)
					{
						iSkillType = GetSkillBuffParty(3);
						if (iSkillType == -1 || (g_isCharacterBuff(pObj, (eBuffState)SkillAttribute[iSkillType].Magic_Icon)) == true)
						{
							iSkillType = GetSkillBuffParty(4);
						}
					}
					if (iSkillType == -1) break;
					if ((g_isCharacterBuff(pObj, (eBuffState)SkillAttribute[iSkillType].Magic_Icon)) == false)
					{

						Hero->CurrentSkill = gSkillManager.FindHeroCurrentSkill((ActionSkillType)iSkillType);
						SelectedCharacter = Party[i].index;
						g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Stage++;
						return 0;
					}
				}
			}
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.TimerBuffParty = GetTickCount() * ((g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PartyBuffTime * 60) * 1000);	
	}
	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PartyAutoHeal) //hoi mau trong nhom
	{
		//===Buff Party
		for (int i = 0; i < PartyNumber; i++)
		{
			if (Party[i].index >= 0 && Party[i].index < MAX_CHARACTERS_CLIENT)
			{
				CHARACTER* pChar = &CharactersClient[Party[i].index];
				OBJECT* pObj = &pChar->Object;
				if ((Party[i].stepHP * 10) < g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PartyHealPercent && !pChar->SafeZone)
				{
					int iSkillType = GetSkillBuffParty(1);
					if (iSkillType == -1) break;
					Hero->CurrentSkill = gSkillManager.FindHeroCurrentSkill((ActionSkillType)iSkillType);
					SelectedCharacter = Party[i].index;
					g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Stage++;
					return 0;
					
				}
			}
		}
	}
	return 1;
}
double GetDistance(CHARACTER* a2)
{
	float v3; // [esp+4h] [ebp-18h]
	float v4; // [esp+14h] [ebp-8h]
	float v5; // [esp+18h] [ebp-4h]

	if (!a2)
		return 0.0;
	v4 = (float)(Hero->PositionX - a2->PositionX);
	v5 = (float)(Hero->PositionY - a2->PositionY);
	v3 = v4 * v4 + v5 * v5;
	return sqrtf(v3);
}
double GetDistanceXY(int X,int Y, int bX, int bY)
{
	float v3; // [esp+4h] [ebp-18h]
	float v4; // [esp+14h] [ebp-8h]
	float v5; // [esp+18h] [ebp-4h]

	v4 = (float)(X - bX);
	v5 = (float)(Y - bY);
	v3 = v4 * v4 + v5 * v5;
	return sqrtf(v3);
}
int ActiveCheckMonterCount(int SkillNum)
{
	if (gSkillManager.FindHeroSkill((ActionSkillType)g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[SkillNum]) ==0)
	{
		return -1;
	}

	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillDelay[SkillNum - 1] == 1)
	{
		if ((GetTickCount() - g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.TimerDelay[SkillNum]) >= (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Delay[SkillNum - 1]*60)*1000)
		{
			return 1;
		}
	}
	else if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillCondition[SkillNum - 1] == 1)
	{
		float Distance = 0;
		int HostileEnemyNum = 0;
		for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
		{
			CHARACTER* c = &CharactersClient[i];
			OBJECT* o = &c->Object;
			if (KIND_MONSTER == o->Kind && o->Live && o->Visible && o->Alpha > 0.f && o->CurrentAction != 6)
			{
				Distance = GetDistance(c);
				//==Khoang cach rong
				if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.LongDistance)
				{
					if (Distance < g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Range[0] + 1)
					{
						HostileEnemyNum++;
					}
				}
				else
				{
					float SkillDistance = gSkillManager.GetSkillDistance(gSkillManager.FindHeroCurrentSkill((ActionSkillType)g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[SkillNum]), Hero);
					
					if (Distance < SkillDistance)
					{
						HostileEnemyNum++;
					}

				}
			}
		}
		//char zbtemp[128] = { 0 };
		//sprintf_s(zbtemp, "ActiveCheckMonterCount %d (%d)", HostileEnemyNum, g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillSubCon[SkillNum - 1]);
		//g_pChatListBox->AddText("", zbtemp, SEASON3B::TYPE_WHISPER_MESSAGE);
		//DrawMessage(1, "%d Skill Get %d (%d)  ", SkillNum - 1,HostileEnemyNum, g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillSubCon[SkillNum - 1]);
		switch (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillSubCon[SkillNum - 1])
		{
		case 3:
			if (HostileEnemyNum >= 8) return 3;
			break;
		case 2:
			if (HostileEnemyNum >= 6) return 2;
			break;
		case 1:
			if (HostileEnemyNum >= 4) return 1;
			break;
		case 0:
			if (HostileEnemyNum >= 2) return 0;
			break;
		default:
			return -1;
		}

		return -1;
	}
	return -1;
}
int GetMonter()
{
	float Distance = 0;
	for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
	{
		CHARACTER* c = &CharactersClient[i];
		OBJECT* o = &c->Object;
		if (KIND_MONSTER == o->Kind && o->Live && o->Visible && o->Alpha > 0.f && o->CurrentAction != 6)
		{
			Distance = GetDistance(c);
			//==Khoang cach rong
			if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.LongDistance)
			{
				if (Distance < g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Range[0] + 1 && CheckWall((Hero->PositionX), (Hero->PositionY), c->PositionX, c->PositionY))
				{
					return i;
				}
			}
			else
			{
				float SkillDistance = gSkillManager.GetSkillDistance(gSkillManager.FindHeroCurrentSkill((ActionSkillType)g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[0]), Hero);
				if (Distance < SkillDistance && CheckWall((Hero->PositionX), (Hero->PositionY), c->PositionX, c->PositionY))
				{
					return i;
				}

			}
		}
	}
	return -1;
}

bool MuHelperActiveAttack()
{
	int SelectSkillNum = 0;
	int GetCount1 = ActiveCheckMonterCount(1);
	int GetCount2 = ActiveCheckMonterCount(2);

	//===Combo
	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Combo == 1)
	{
		SelectSkillNum = g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillCache;
	}
	else
	{
		if (GetCount1 != -1 || GetCount2 != -1)
		{
			SelectSkillNum = (GetCount2 > GetCount1 ? 2 : 1);
			goto SetAttackSkill;
		}
	}
SetAttackSkill:
	//DrawMessage(1, "%d ~ %d (%d)", GetCount1, GetCount2, SelectSkillNum);
	if (SelectSkillNum == -1) SelectSkillNum = 0;
	if (gSkillManager.FindHeroSkill((ActionSkillType)g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[SelectSkillNum]) == 1)
	{
		SelectedCharacter = GetMonter();

		int iSkillType = g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[SelectSkillNum];
		Hero->CurrentSkill = gSkillManager.FindHeroCurrentSkill((ActionSkillType)iSkillType);
		if (SelectedCharacter != -1)
		{
			Attacking = 2;
			//MouseRButtonPush = 1;
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Stage++;
			
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.TimerDelay[SelectSkillNum] = GetTickCount();
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillCache = (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillCache >= 2 ? 0 : g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.SkillCache + 1);
		}

	}
	return 1;
}
bool MuHelperActiveHeal()
{
	BYTE RateHP = (BYTE)((CharacterAttribute->PrintPlayer.ViewCurHP * 100) / (CharacterAttribute->PrintPlayer.ViewMaxHP));
	//==SKill Heal  , hut mau
	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.AutoHeal)
	{
		//DrawMessage(1, "Heal HP %d / %d", RateHP, g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.HealPercent);
		if (RateHP <= g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.HealPercent)
		{
			if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Fairy_Elf)
			{
				int iSkillType = GetSkillBuffParty(1); //lay skill hoi mau
				if (iSkillType != -1)
				{
					Hero->CurrentSkill = gSkillManager.FindHeroCurrentSkill((ActionSkillType)iSkillType);
					SelectedCharacter = FindCharacterIndex(Hero->Key);
					g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Stage++;
					return 0;
				}
			}
			else 	if (gCharacterManager.GetBaseClass(Hero->Class) == CNewUIBCustomMenuInfo::Summoner)
			{
				int iSkillType = GetSkillBuffParty(5); //lay skill hut maiu
				if (iSkillType != -1)
				{
					Hero->CurrentSkill = gSkillManager.FindHeroCurrentSkill((ActionSkillType)iSkillType);
					SelectedCharacter = GetMonter();
					g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Stage++;
					return 0;
				}
			}
		}
	}
	//==Dung thuoc hoi mau
	if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.AutoPotion)
	{
		if (RateHP <= g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PotPercent)
		{
			int Index = g_pMyInventory->FindHPItemIndex();

			if (Index != -1)
			{
				SendRequestUse(Index, 0);
			}
		}
	}
	return 1;
}
bool CompareStringsIgnoringSpaces(const std::string& strA, const std::string& strB)
{

	size_t lenA = strA.size();
	size_t lenB = strB.size();

	if (lenA < lenB)
	{
	//	std::cout << "CompareStringsIgnoringSpaces 1" << std::endl;
		return false;
	}
	int j = 0;
	for (int i = 0; i < lenB; ++i)
	{
		if (tolower(strB[i]) == ' ')
		{
			continue;
		}
		if (tolower(strA[i]) != tolower(strB[i]))
		{
			std::string charAsStringA(1, strA[i]);
			std::string charAsStringB(1, strB[i]);
			//DrawMessage(4, "Khac (%d)v%s ~ %s", i, charAsStringA.c_str(), charAsStringB.c_str());
			//std::cout << "CompareStringsIgnoringSpaces 2 " << charAsStringA << " ~ " << charAsStringB << std::endl;
			return false;
		}

	}

	return true;
}
bool FindItemName(const std::string& strA, const std::string& strB)
{
	for (int i = 0; i < strA.size(); ++i)
	{
		if (tolower(strA[i]) == tolower(strB[0]))
		{
			
			std::string remainingString = strA.substr(i);
			
			if (remainingString.size() >= strB.size())
			{
				if (CompareStringsIgnoringSpaces(remainingString, strB))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool PickUpItemName(ITEM* pItem)
{
	unicode::t_char szItemName[128];
	GetItemName(pItem->Type, pItem->Level, szItemName);
	for (int n = 0; n < MAX_HELPER_ITEM; n++)
	{
		if (strlen(g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.ItemList[n]) > 0)
		{
			//DrawMessage(4, "%s ~ %s", szItemName, g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.ItemList[n]);
			//== tim 2 chuoi trung nhau
			if (FindItemName(szItemName, g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.ItemList[n]))
			{
				return 1;
				break;
			}
			
		}
	}
	
	return 0;
}

int PickUpItemMuHelper()
{
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		OBJECT* o = &Items[i].Object;
		if (o->Live && o->Visible)
		{
			o->LightEnable = true;
			Vector(0.2f, 0.2f, 0.2f, o->Light);
		}
	}
	float Luminosity = 1.5f;
	float dx, dy, dl;
	OBJECT* obj = &Hero->Object;
	if (!obj) return -1;
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		OBJECT* o = &Items[i].Object;
		if (o->Live && o->Visible)
		{

			dx = obj->Position[0] - o->Position[0];
			dy = obj->Position[1] - o->Position[1];

			dl = sqrtf(dx * dx + dy * dy);
			
			if ((g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Range[1]*100) > dl)
			{
				if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickAllItem == 1)
				{
					goto SendPickUpItem;
				}
				if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickSelected == 1)
				{
					if (Items[i].Item.Type == ITEM_POTION + 15 && g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickMoney) //Zen
					{
						goto SendPickUpItem;
					}
					if ((CheckItemIsJewel(Items[i].Item.Type)) && g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickJewel) //Ngoc
					{
						goto SendPickUpItem;
					}
					if (Items[i].Item.ExtOption && g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickSet) //Set
					{
						goto SendPickUpItem;
					}
					if (Items[i].Item.Option1 && g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickExc) //Exl
					{
						goto SendPickUpItem;
					}
					if (PickUpItemName(&Items[i].Item) && g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.PickExtra) //pIck up name
					{
						goto SendPickUpItem;
					}
					 
				}
				continue;
SendPickUpItem:
				if (-1 == g_pMyInventory->FindEmptySlot(&Items[i].Item))
				{
					//DrawMessage(4, "Khong du cho nhat item %d (%f)", Items[i].Item.Type, dl);
					return -1;
				}
				//DrawMessage(1, "Find Item Type %d (%f)", Items[i].Item.Type, dl);
			    o->LightEnable = false;
			    Vector(Luminosity, Luminosity, Luminosity, o->Light);
			    return i;
				
			}
		}
	}
	return -1;
}
bool MuHelperRepairItem()
{
	for (int i = 0; i < 12; i++)
	{
		ITEM* pItem = &CharacterMachine->Equipment[i];
		if (pItem == nullptr || pItem->Type ==-1)
		{
			continue;
		}
		ITEM_ATTRIBUTE* p = &ItemAttribute[pItem->Type];
		int maxDurability = calcMaxDurability(pItem, p, pItem->Level);
		//DrawMessage(1, "Item %d Dur (%d/%d)", pItem->Type, pItem->Durability, maxDurability);
		if(pItem->Durability < maxDurability) SendRequestRepair(i, 1);
	}
	return 1;
}
extern bool LogOut;

bool CNewUIBCustomMenuInfo::RunningMuhelper()
{


	if (LogOut || !Hero) // Kiểm tra Hero tránh crash
	{
		return false;
	}

	if (Hero->Movement || Hero->Dead || Hero->Appear)
	{

		return false;
	}

	if (!g_pNewUISystem || !g_pNewUISystem->Get_pNewUIMuHelper())
	{
		return false;
	}

	auto* muHelper = g_pNewUISystem->Get_pNewUIMuHelper();

	if (muHelper->DataAutoMu.Stage > 1)
	{
		return false;
	}

	//=== Action MuHelper
	if (muHelper->DataAutoMu.Started)
	{
		DWORD Time = GetTickCount();

		//== Sửa đồ
		if (muHelper->DataAutoMu.RepairItem && Time > muHelper->DataAutoMu.TimerRepair)
		{
			muHelper->DataAutoMu.TimerRepair = Time + (2 * 60 * 1000);
			MuHelperRepairItem();
		}

		//== Di chuyển về vị trí cũ
		if ((Time - muHelper->DataAutoMu.TimerMove) >= (muHelper->DataAutoMu.MoveTime * 60) * 1000
			&& muHelper->DataAutoMu.OriginalPosition == 1)
		{
			muHelper->DataAutoMu.TimerMove = Time;
			if (PathFinding2(Hero->PositionX, Hero->PositionY, muHelper->DataAutoMu.StartX, muHelper->DataAutoMu.StartY, &Hero->Path, 0.0f))
			{
				Hero->MovementType = MOVEMENT_MOVE;
				Hero->Movement = true;
				SendMove(Hero, &Hero->Object);
				return false;
			}
		}

		//== Nhặt Item
		if (muHelper->DataAutoMu.SelectedItem == -1)
		{
			muHelper->DataAutoMu.SelectedItem = PickUpItemMuHelper();
		}

		if (muHelper->DataAutoMu.SelectedItem != -1)
		{
			OBJECT* o = &Hero->Object;
			Hero->MovementType = MOVEMENT_GET;
			ItemKey = muHelper->DataAutoMu.SelectedItem;
			TargetX = (int)(Items[ItemKey].Object.Position[0] / TERRAIN_SCALE);
			TargetY = (int)(Items[ItemKey].Object.Position[1] / TERRAIN_SCALE);

			if (PathFinding2(Hero->PositionX, Hero->PositionY, TargetX, TargetY, &Hero->Path))
			{
				SendMove(Hero, o);
			}
			else
			{
				Action(Hero, o, true);
				Hero->MovementType = MOVEMENT_MOVE;
			}

			muHelper->DataAutoMu.SelectedItem = -1;
			return false;
		}

		//== Hồi máu / Hút máu
		if (!MuHelperActiveHeal()) return false;

		//== Buff cá nhân & Buff nhóm
		if (!MuHelperActiveBuff()) return false;

		if (SelectedCharacter == -1 || SelectedCharacter == FindCharacterIndex(Hero->Key))
		{
			SelectedCharacter = GetMonter();
		}

		if (SelectedCharacter != -1)
		{
			CHARACTER* c = &CharactersClient[SelectedCharacter];
			OBJECT* o = &c->Object;

			if (!c || !o) return false; // Tránh truy cập NULL pointer

			float Distance = GetDistance(c);

			// Kiểm tra điều kiện di chuyển đến mục tiêu
			if (!muHelper->DataAutoMu.LongDistance &&
				(!CheckWall(Hero->PositionX, Hero->PositionY, c->PositionX, c->PositionY) ||
					GetDistanceXY(Hero->PositionX, Hero->PositionY, c->PositionX, c->PositionY) > 3))
			{
				if (PathFinding2(Hero->PositionX, Hero->PositionY, c->PositionX, c->PositionY, &Hero->Path, 3.0f))
				{
					Hero->MovementType = MOVEMENT_SKILL;
					Hero->Movement = true;
					SendMove(Hero, &Hero->Object);
				}
			}

			// Kiểm tra điều kiện tấn công
			if (o->CurrentAction == 6 || !o->Live || !o->Visible || c->Dead ||
				!CheckWall(Hero->PositionX, Hero->PositionY, c->PositionX, c->PositionY) ||
				CheckTarget(Hero))
			{
				SelectedCharacter = GetMonter();
			}

			//== Attack
			muHelper->DataAutoMu.TimerSkill = Time;
			if (!MuHelperActiveAttack()) return false;
		}
	}
	return true;
}
bool CNewUIBCustomMenuInfo::Render() //Work()
{
	gInterface.Work();
	return true;
}

float CNewUIBCustomMenuInfo::GetLayerDepth()
{
	return 9.3f;
}

void CNewUIBCustomMenuInfo::OpenningProcess()
{
	
}

void CNewUIBCustomMenuInfo::ClosingProcess()
{
	
}

void CNewUIBCustomMenuInfo::BindObject(short ObjectID, char* Patch, float Width, float Height, float X, float Y)
{
	int ModelID = BITMAP_NONAMED_TEXTURES_NEW_LOAD + gInterface.IndexCustomTextture++;
	LoadBitmap(Patch, ModelID, GL_LINEAR, GL_REPEAT, 1, 0);
	gInterface.Data[ObjectID].EventTick = 0;
	gInterface.Data[ObjectID].OnClick = false;
	gInterface.Data[ObjectID].OnShow = false;
	gInterface.Data[ObjectID].ModelID = ModelID;
	gInterface.Data[ObjectID].Width = Width;
	gInterface.Data[ObjectID].Height = Height;
	gInterface.Data[ObjectID].X = X;
	gInterface.Data[ObjectID].Y = Y;
	gInterface.Data[ObjectID].MaxX = X + Width;
	gInterface.Data[ObjectID].MaxY = Y + Height;
	gInterface.Data[ObjectID].Attribute = 0;
}
void CNewUIBCustomMenuInfo::DrawGUI(short ObjectID, float PosX, float PosY, bool SetPos)
{
	if(gInterface.Data[ObjectID].X == -1 || gInterface.Data[ObjectID].Y == -1 )
	{
		gInterface.Data[ObjectID].X = PosX;
		gInterface.Data[ObjectID].Y = PosY;
		gInterface.Data[ObjectID].MaxX = PosX +gInterface.Data[ObjectID].Width;
		gInterface.Data[ObjectID].MaxY = PosY +gInterface.Data[ObjectID].Height;
	}

	RenderImage(gInterface.Data[ObjectID].ModelID, PosX, PosY,gInterface.Data[ObjectID].Width, gInterface.Data[ObjectID].Height);
}
void CNewUIBCustomMenuInfo::LoadImages()
{
	//MAx IMAGE_HERO_POSITION_INFO_BASE_WINDOW +5;
	LoadBitmap("Interface\\GFx\\Character\\Frame_I3.tga", IMAGE_Frame_I3, GL_LINEAR);
	LoadBitmap("Interface\\GFx\\MacroUI\\Pegasus_Menu.tga", IMAGE_Frame_Menu, GL_LINEAR);
	LoadBitmap("Interface\\GFx\\MacroUI\\Pegasus_Map.tga", Pegasus_Map, GL_LINEAR);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_BCUSTOM_WINDOW_31322, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back01.tga", IMAGE_BCUSTOM_WINDOW_31353, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_BCUSTOM_WINDOW_31357, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_BCUSTOM_WINDOW_31355, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_BCUSTOM_WINDOW_31356, GL_LINEAR);
	LoadBitmap("Interface\\mini_map_ui_cancel.tga", IMAGE_BCUSTOM_WINDOW_32453, GL_LINEAR);
	LoadBitmap("Interface\\newui_btn_empty.tga", IMAGE_BCUSTOM_WINDOW_31326, GL_LINEAR);
	LoadBitmap("Custom\\Maps\\CRadar.tga", BITMAP_INTERFACE_CUSTOM + 2, GL_LINEAR);
	LoadBitmap("Custom\\Maps\\map_radar.tga", BITMAP_INTERFACE_CUSTOM + 4, GL_LINEAR);
	LoadBitmap("Custom\\Maps\\background-tcp.tga", BITMAP_INTERFACE_CUSTOM + 5, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_check.tga", BITMAP_INTERFACE_CUSTOM+6, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_OptionButton.tga", BITMAP_INTERFACE_CUSTOM+7, GL_LINEAR);
	LoadBitmap("Interface\\InGameShop\\ingame_Bt_page_L.tga", ButtonPageL, GL_LINEAR);
	LoadBitmap("Interface\\InGameShop\\ingame_Bt_page_R.tga", ButtonPageR, GL_LINEAR);


	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_B_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_B_COLOR, GL_LINEAR);

	LoadBitmap("Interface\\newui_item_table01(L).tga", IMAGE_B_TABLE_TOP_LEFT);
	LoadBitmap("Interface\\newui_item_table01(R).tga", IMAGE_B_TABLE_TOP_RIGHT);
	LoadBitmap("Interface\\newui_item_table02(L).tga", IMAGE_B_TABLE_BOTTOM_LEFT);
	LoadBitmap("Interface\\newui_item_table02(R).tga", IMAGE_B_TABLE_BOTTOM_RIGHT);
	LoadBitmap("Interface\\newui_item_table03(Up).tga", IMAGE_B_TABLE_TOP_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(Dw).tga", IMAGE_B_TABLE_BOTTOM_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(L).tga", IMAGE_B_TABLE_LEFT_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(R).tga", IMAGE_B_TABLE_RIGHT_PIXEL);


	LoadBitmap("Interface\\Pegasus\\RenderHP.tga", IMAGE_HP_BG);
	LoadBitmap("Interface\\Pegasus\\HPBAR.tga", IMAGE_HP_HP);
	LoadBitmap("Interface\\Pegasus\\HPBARSD.tga", IMAGE_HP_SD);
	LoadBitmap("Interface\\Pegasus\\HPBARPLAYER.tga", IMAGE_HP_PLAYER);
	//==Custom
	gInterface.BLoadImage(); //load img

}

void CNewUIBCustomMenuInfo::UnloadImages()
{
	DeleteBitmap(ButtonPageL);
	DeleteBitmap(ButtonPageR);
	DeleteBitmap(IMAGE_HP_BG);
	DeleteBitmap(IMAGE_HP_HP);
	DeleteBitmap(IMAGE_HP_SD);
	DeleteBitmap(IMAGE_HP_PLAYER);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_31322);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_31353);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_31357);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_31355);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_31356);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_32453);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW_31326);
	DeleteBitmap(IMAGE_BCUSTOM_WINDOW);
	DeleteBitmap(BITMAP_INTERFACE_CUSTOM + 2);
	DeleteBitmap(BITMAP_INTERFACE_CUSTOM + 4);
	DeleteBitmap(BITMAP_INTERFACE_CUSTOM + 5);
	DeleteBitmap(BITMAP_INTERFACE_CUSTOM + 6);

	DeleteBitmap(IMAGE_B_TABLE_RIGHT_PIXEL);
	DeleteBitmap(IMAGE_B_TABLE_LEFT_PIXEL);
	DeleteBitmap(IMAGE_B_TABLE_BOTTOM_PIXEL);
	DeleteBitmap(IMAGE_B_TABLE_TOP_PIXEL);
	DeleteBitmap(IMAGE_B_TABLE_BOTTOM_RIGHT);
	DeleteBitmap(IMAGE_B_TABLE_BOTTOM_LEFT);
	DeleteBitmap(IMAGE_B_TABLE_TOP_RIGHT);
	DeleteBitmap(IMAGE_B_TABLE_TOP_LEFT);

	DeleteBitmap(IMAGE_OPTION_B_BACK);
	DeleteBitmap(IMAGE_OPTION_B_COLOR);
	//Clear IMG
	for (int i = 0; i < gInterface.IndexCustomTextture; i++)
	{
		DeleteBitmap(BITMAP_NONAMED_TEXTURES_NEW_LOAD + i);
	}
}


