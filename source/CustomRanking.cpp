#include "stdafx.h"
#include "CustomRanking.h"
#include "Protocol.h"
#include "CustomMessage.h"
#include "CBInterface.h"
#include "Util.h"
#include "Protect.h"
#include "Other.h"
#include "NewUISystem.h"
#include "UIControls.h"
#include "wsclientinline.h"
#include "CharacterManager.h"
#include "NewUIMyInventory.h"

int NextRanking = 0;
int PrevRanking = 0;

CustomRanking gCustomRanking;

CustomRanking::CustomRanking() // OK
{
	this->ClearCustomRanking();


}

CustomRanking::~CustomRanking() // OK
{

}
void CustomRanking::ClearCustomRanking() // OK
{
	for (int n = 0; n < MAX_RANK; n++)
	{
		memset(gCustomRanking[n].Value1, 0, sizeof(gCustomRanking[n].Value1));
		memset(gCustomRanking[n].Value2, 0, sizeof(gCustomRanking[n].Value2));
		memset(gCustomRanking[n].Value3, 0, sizeof(gCustomRanking[n].Value3));
		memset(gCustomRanking[n].Value4, 0, sizeof(gCustomRanking[n].Value4));

	}
	this->RankEnable = 0;

}
void CustomRanking::GCReqRanking(CustomRanking::PMSG_CUSTOM_RANKING_RECV* lpMsg) // OK
{
	memcpy(this->rankname, lpMsg->rankname, sizeof(this->rankname));
	memcpy(this->col1, lpMsg->col1, sizeof(this->col1));
	memcpy(this->col2, lpMsg->col2, sizeof(this->col2));
	memcpy(this->col3, lpMsg->col3, sizeof(this->col3));
	memcpy(this->col4, lpMsg->col4, sizeof(this->col4));

	this->RankEnable = 1;


	mListRanking.clear();

	this->count = lpMsg->count;

	for (int n = 0; n < lpMsg->count; n++)
	{
		CUSTOM_RANKING_DATA* lpInfo = (CUSTOM_RANKING_DATA*)(((BYTE*)lpMsg) + sizeof(PMSG_CUSTOM_RANKING_RECV) + (sizeof(CUSTOM_RANKING_DATA) * n));

		memcpy(gCustomRanking[n].Value1, lpInfo->Value1, sizeof(gCustomRanking[n].Value1));
		memcpy(gCustomRanking[n].Value2, lpInfo->Value2, sizeof(gCustomRanking[n].Value2));
		memcpy(gCustomRanking[n].Value3, lpInfo->Value3, sizeof(gCustomRanking[n].Value3));
		memcpy(gCustomRanking[n].Value4, lpInfo->Value4, sizeof(gCustomRanking[n].Value4));

	}
}

void CustomRanking::OpenWindow()
{
	if ((GetTickCount() - gInterface.Data[eRankPANEL_MAIN].EventTick) < 1500)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow = false;
		return;
	}

	gInterface.Data[eRankPANEL_MAIN].EventTick = GetTickCount();

	if (gInterface.Data[eRankPANEL_MAIN].OnShow == true)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow = false;

		return;
	}

	this->ClearCustomRanking();

	this->RankingCount = 0;

	this->SelectBarRankingNumber = 0;

	PMSG_CUSTOM_RANKING_COUNT_SEND pMsg1;

	pMsg1.header.set(0xF3, 0xE7, sizeof(pMsg1));

	DataSend((BYTE*)&pMsg1, pMsg1.header.size);

	PMSG_CUSTOM_RANKING_SEND pMsg;

	pMsg.header.set(0xF3, 0xE6, sizeof(pMsg));

	this->RankingType = 0;

	pMsg.type = 0;

	DataSend((BYTE*)&pMsg, pMsg.header.size);

	gInterface.Data[eRankPANEL_MAIN].OnShow = true;

}


void CustomRanking::RankNameRev(PMSG_RANK_INFO* lpMsg)
{
	this->ranknamecount = 0;
	memset(this->m_RankInfo, 0, sizeof(this->m_RankInfo));
	for (int i = 0; i < MAX_RANK; i++)
	{

		memcpy(this->m_RankInfo[i].NameRank, lpMsg->Info[i].NameRank, 32);
		if (strlen(this->m_RankInfo[i].NameRank) != 0)
		{
			this->ranknamecount++;
		}
	}
}

void CustomRanking::GCReqRankingCount(PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg) // OK
{

	this->RankingCount = (lpMsg->count > 0) ? lpMsg->count : 0;
}

int PageBank = 0;
int MaxPerPageBank = 10;

static void BPagePrevAction(LPVOID pClass)
{
	if (GetTickCount() < gInterface.Data[eRankPANEL_MAIN].EventTick + 300) return;
	gInterface.Data[eRankPANEL_MAIN].EventTick = GetTickCount();

	if (PageBank > 0)
	{
		PageBank--;
	}
}
static void BPageNextAction(LPVOID pClass)
{
	if (GetTickCount() < gInterface.Data[eRankPANEL_MAIN].EventTick + 300) return;
	gInterface.Data[eRankPANEL_MAIN].EventTick = GetTickCount();

	if (PageBank < (gCustomRanking.count / MaxPerPageBank) - 1)
	{
		PageBank++;
	}
}


bool top1RequestSent = true;
void CustomRanking::DrawRankPanelWindow()
{
	if (!gInterface.Data[eRankPANEL_MAIN].OnShow)
	{
		return;
	}

	if (gInterface.CheckWindow(Interface::Inventory)
		|| gInterface.CheckWindow(Interface::Character) //-- 13 & 16
		|| gInterface.CheckWindow(Interface::Warehouse) //-- 8
		|| gInterface.CheckWindow(Interface::Shop) //-- 12
		|| gInterface.CheckWindow(Interface::ChaosBox) //-- 9
		|| gInterface.CheckWindow(Interface::Trade) //-- 7
		|| gInterface.CheckWindow(Interface::Store) //-- 14
		|| gInterface.CheckWindow(Interface::OtherStore) //-- 15
		|| gInterface.CheckWindow(Interface::LuckyCoin1) //-- 60
		|| gInterface.CheckWindow(Interface::NPC_ChaosMix)
		|| gInterface.CheckWindow(Interface::MoveList)

		)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow = false;
		return;
	}

	if (!gInterface.Data[eRankPANEL_MAIN].OnShow)
	{
		top1RequestSent = true;
		return;
	}

	// CORREÇÃO: Ajuste de dimensões e posicionamento da janela
	float MainWidth = 350.0f;
	float MainHeight = 280.0f;
	float StartY = ((MAX_WIN_HEIGHT - 51) / 2.0f) - (MainHeight / 2.0f);
	float StartX = (MAX_WIN_WIDTH / 2.0f) - (MainWidth / 2.0f);

	// CORREÇÃO: Chamada correta da função com bloco if
	if (g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, eRankPANEL_MAIN, this->rankname))
	{
		EnableAlphaTest();
		glColor3f(1.0, 1.0, 1.0);
		gInterface.DrawFormat(eYellow, StartX + 37.0f, StartY + 13.0f, 100, 1, "Ranking [F3]");

		// CORREÇÃO: Posição do botão de fechar ajustada
		int CloseX = StartX + MainWidth - 35.0f;
		int CloseY = StartY + 3.0f;

		RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30.0f, 30.0f);

		if (SEASON3B::CheckMouseIn(CloseX, CloseY, 30, 30) == 1)
		{
			if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eRankPANEL_MAIN].EventTick > 300)
			{
				gInterface.Data[eRankPANEL_MAIN].Close();
			}
			RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30, 30, 0, 0, RGBA(255, 255, 255, 255));
		}

		DisableAlphaBlend();
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			gInterface.Data[eRankPANEL_MAIN].OnShow ^= 1;
			PlayBuffer(SOUND_CLICK01);
		}

		{
			DWORD Color = eGray100;

			if (this->RankEnable == 1)
			{
				EnableAlphaTest();
				glColor3f(1.0, 1.0, 1.0);

				// CORREÇÃO: Cabeçalhos das colunas com posicionamento ajustado
				gInterface.DrawFormat(eWhite, StartX + 25.0f, StartY + 40.0f, 110, 1, "Top");
				gInterface.DrawFormat(eWhite, StartX + 60.0f, StartY + 40.0f, 110, 1, "%s", this->col1);
				gInterface.DrawFormat(eWhite, StartX + 140.0f, StartY + 40.0f, 110, 1, "%s", this->col2);
				gInterface.DrawFormat(eWhite, StartX + 210.0f, StartY + 40.0f, 110, 1, "%s", this->col3);
				gInterface.DrawFormat(eWhite, StartX + 280.0f, StartY + 40.0f, 110, 1, "%s", this->col4);
				DisableAlphaBlend();

				char* name = (char*)(CharacterAttribute->Name);

				for (int n = 0, i = (PageBank * MaxPerPageBank); n < MaxPerPageBank && i < this->count; i++)
				{
					Color = eWhite;

					if (i >= 0)
					{
						EnableAlphaTest();
						glColor3f(1.0, 1.0, 1.0);

						if (i < 3)
						{
							Color = eYellow;
						}
						else
						{
							Color = eWhite;
						}

						gInterface.DrawFormat(Color, StartX + 28.0f, StartY + 60.0f + (20 * n), 52, 2, "%d", (i + 1));
						DisableAlphaBlend();
					}

					if (strcmp(name, this->gCustomRanking[i].Value1) == 0)
					{
						Color = eGold;
					}

					// CORREÇÃO: Renderização de fundo da linha ajustada
					g_pBCustomMenuInfo->RenderRanking(StartX + 16.0f, StartY + 55.0f + (20 * n), 330, 22, 0x00000096, 0, 0);

					if (SEASON3B::CheckMouseIn(StartX + 16.0f, StartY + 55.0f + (20 * n), 330, 22))
					{
						FrameTarget(StartX + 17.0f, StartY + 55.0f + (20 * n), 326, 17, RGBA(31, 144, 255, 0xff));
					}

					EnableAlphaTest();
					glColor3f(1.0, 1.0, 1.0);

					// CORREÇÃO: Dados das colunas com posicionamento ajustado
					gInterface.DrawFormat(Color, StartX + 60.0f, StartY + 60.0f + (20 * n), 110, 1, "%s", this->gCustomRanking[i].Value1);
					int value = atoi(this->gCustomRanking[i].Value2);
					gInterface.DrawFormat(eWhite, StartX + 140.0f, StartY + 60.0f + (20 * n), 110, 1, "%s", gInterface.NumberFormat(value));
					gInterface.DrawFormat(eWhite, StartX + 210.0f, StartY + 60.0f + (20 * n), 110, 1, "%s", this->gCustomRanking[i].Value3);
					gInterface.DrawFormat(eWhite, StartX + 280.0f, StartY + 60.0f + (20 * n), 110, 1, "%s", this->gCustomRanking[i].Value4);
					DisableAlphaBlend();

					// Renderização de ícone se houver
					if (atoi(this->gCustomRanking[i].Value3) > 0)
					{
						EnableAlphaTest();
						glColor3f(1.0, 1.0, 1.0);
						RenderBitmap(150000 + atoi(this->gCustomRanking[i].Value3), StartX + 10.0f, StartY + 28.0f + (20 * n), 170.0f, 120.0f, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
						DisableAlphaBlend();
					}

					n++;
				}

				// CORREÇÃO: Controles de paginação centralizados
				float CenterX = StartX + (MainWidth / 2.0f);
				float CenterY = StartY + MainHeight - 25.0f;

				EnableAlphaTest();
				glColor3f(1.0, 1.0, 1.0);

				int totalPages = (this->count + MaxPerPageBank - 1) / MaxPerPageBank;
				if (totalPages < 1) totalPages = 1;

				TextDraw(g_hFontBold, CenterX - 15.0f, CenterY, 0xffffffff, 0x0, 60, 0, 3, "%d / %d", PageBank + 1, totalPages);
				DisableAlphaBlend();

				if (g_pBCustomMenuInfo->DrawButtonGUI(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::ButtonPageL, CenterX - 60.0f, CenterY, 20, 22, 3))
				{
					BPagePrevAction(NULL);
				}

				if (g_pBCustomMenuInfo->DrawButtonGUI(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::ButtonPageR, CenterX + 40.0f, CenterY, 20, 22, 3))
				{
					BPageNextAction(NULL);
				}
			}
		}
	}
}