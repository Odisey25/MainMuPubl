#include "stdafx.h"
#include "ChangeClass.h"
#include "CBInterface.h"
#include <NewUISystem.h>

cChangeClass gChangeClass;

cChangeClass::cChangeClass()
{
	this->m_Price = 0;
}

cChangeClass::~cChangeClass()
{
	
}

void cChangeClass::RecvData(PMSG_CHANGECLASS_DATA* Data)
{
	this->m_PriceType = Data->PriceType;
	this->m_Price = Data->Price;
	gInterface.Data[eCHANGINGCLASS_MAIN].OnShow ^= 1; //On/Off
}

void cChangeClass::SendChangeClass(int Type)
{
	gInterface.Data[eCHANGINGCLASS_MAIN].Close();

	XULY_CGPACKET pMsg;
	pMsg.header.set(0xFD,0x05, sizeof(pMsg));	
	pMsg.ThaoTac = Type;
	DataSend((LPBYTE)&pMsg, pMsg.header.size);
}

void cChangeClass::drawNewInterfaceClassChange()
{
	if (!gInterface.Data[eCHANGINGCLASS_MAIN].OnShow)
	{
		return;
	}

	

	float MainWidth = 540;
	float MainHeight = 245;
	float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2) + 1.0f;
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 151.0f;


	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, eCHANGINGCLASS_MAIN, "");

	
	//===Info Text
	float TextX = StartX + 15;
	float TextY = StartY + 25;
	int CountText = 0;

	if (gChangeClass.m_PriceType == 0)
	{
		
		//CustomFont.Draw((HFONT)pFontNormal, TextX, TextY+(11* 1), 0xFFAE00FF, 0x0, 0, 0, 1, "Yêu Cầu Zen:");
	}
	else if (gChangeClass.m_PriceType == 1)
	{
		//CustomFont.Draw((HFONT)pFontNormal, TextX + 250, TextY+80, 0xFFAE00FF, 0x0, 0, 0, 1, "Yêu Cầu Wcoin:");
	}
	else if (gChangeClass.m_PriceType == 2)
	{
		//CustomFont.Draw((HFONT)pFontNormal, TextX, TextY+(11* 1), 0xFFAE00FF, 0x0, 0, 0, 1, "Yêu Cầu WP:");
	}
	else if (gChangeClass.m_PriceType == 3)
	{
		//CustomFont.Draw((HFONT)pFontNormal, TextX, TextY+(11* 1), 0xFFAE00FF, 0x0, 0, 0, 1, "Yêu Cầu GP:");
	}
	else if (gChangeClass.m_PriceType == 4)
	{
		//CustomFont.Draw((HFONT)pFontNormal, TextX, TextY+(11* 1), 0xFFAE00FF, 0x0, 0, 0, 1, "Yêu Cầu RUUD:");
	}
	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);
	gInterface.DrawFormat(eYellow, StartX + 30, StartY + 12, 100, 1, "Đổi Chủng Tộc [F4]");

	gInterface.DrawFormat(eWhite, TextX, TextY + 10, 200, 1, "Yêu Cầu Để Đổi Chủng Tộc - Vui Lòng Đọc Kỹ ");

	gInterface.DrawFormat(eRed, TextX, TextY + 20, 200, 1, "Pegasus Coin : %d", this->m_Price);

	gInterface.DrawFormat(eWhite, TextX, TextY + 30, 200, 1, "Vui Lòng Tháo Hết Trang Bị Khi Đổi Chủng Tộc");

	gInterface.DrawFormat(eRed, TextX, TextY + 40, 200, 1, "Điểm Master Sẽ Được Trả Lại Sau Khi Đổi Chủng Tộc");

	gInterface.DrawFormat(eRed, TextX, TextY + 50, 200, 1, "Phải Hoàn Thành Nhiệm Vụ Nhân Vật 3");
	DisableAlphaBlend();
	
	
	char* buttonName[] = { "Pháp Sư", "Chiến Binh", "Tiên Nữ","Đấu Sĩ", "Chúa Tể", "Thuật Sĩ", "Thiết Binh"};
	for (char i = 0; i < 7; i++)
	{

		if (g_pBCustomMenuInfo->DrawButton(StartX + 35 + (i % 2) * (60 / 1.3 + 70), StartY + 100 + i / 2 * (50 / 2.0 + 5), 100, 12, buttonName[i]))
		{
		
			if(i == 0)
			{/*DW*/
				gChangeClass.SendChangeClass(0);
			}else if(i==1){/*DK*/
			
			
				gChangeClass.SendChangeClass(16);
			}else if(i==2){/*ELF*/
				
				
				gChangeClass.SendChangeClass(32);
			}else if(i==3){/*MG*/
			
			
				gChangeClass.SendChangeClass(48);
			}else if(i==4){/*DL*/
			
			
				gChangeClass.SendChangeClass(64);
			}else if(i==5){/*SUM*/
				
			
				gChangeClass.SendChangeClass(80);
			}else if(i==6)
			{/*RF*/
			
				
				gChangeClass.SendChangeClass(96);
			}
			gInterface.Data[eCHANGINGCLASS_MAIN].OnShow = false;
		}
	}
	
}

void cChangeClass::ChangeClass()
{

	if ((GetTickCount() - gInterface.Data[eCHANGINGCLASS_MAIN].EventTick) > 300)
	{
		gInterface.Data[eCHANGINGCLASS_MAIN].EventTick = GetTickCount();

		if (gInterface.Data[eCHANGINGCLASS_MAIN].OnShow)
		{
			gInterface.Data[eCHANGINGCLASS_MAIN].OnShow = 0;
			return;
		}
		XULY_CGPACKET pMsg;
		pMsg.header.set(0xD3, 0x93, sizeof(pMsg));
		pMsg.ThaoTac = 1; //
		DataSend((LPBYTE)&pMsg, pMsg.header.size);
		
		
	}

}
