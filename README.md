# Main.6.3

Documentancion 

Para que algun cuadro desaparezca o no se vea encima de otro o ya sea una opcion de pet life party etc ya sea otra cosa se debe aplicar

g_pNewUISystem->IsVisible(SEASON3B

asi cuando se abra cualquier ventana esa desaparezca tambien asi se podria evitar cualquier cosa dupe items o similares.

ejemplo party life etc 

*int BGetScreenWidth()
{
#if(!WIDE_SCREEN)
	return GetScreenWidth();
#endif
	int iWidth = 0;

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY) &&
	     (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYSHOP_INVENTORY) &&
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_ExpandInventory)) || 
		(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER) &&
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_ExpandInventory))
		) {
		iWidth = DisplayWin - (190 * 3);
	} else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY)
		&& (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_STORAGE)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MIXINVENTORY)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADE)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYSHOP_INVENTORY)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PURCHASESHOP_INVENTORY)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYCOIN_REGISTRATION)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_ExpandInventory)
#ifdef LEM_ADD_LUCKYITEM
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND)
#endif // LEM_ADD_LUCKYITEM
			))
	{
		iWidth = DisplayWin - (190 * 2);
	}
	else if (
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY) &&
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYSHOP_INVENTORY) &&
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_ExpandInventory)
		) {
		iWidth = DisplayWin - (190 * 4);
	}
	else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)
		&& (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYQUEST)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_QUEST_PROGRESS_ETC))
		)
	{
		iWidth = DisplayWin - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)
		&& g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PET)
		)
	{
		iWidth = DisplayWin - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_REFINERY))
	{
		iWidth = DisplayWin - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCGUILDMASTER)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUARDSMAN)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_SENATUS)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GATEKEEPER)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYQUEST)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_SERVERDIVISION)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_COMMAND)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCQUEST)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GATESWITCH)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CATAPULT)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DEVILSQUARE)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_BLOODCASTLE)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GOLD_BOWMAN)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MuHelper)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GOLD_BOWMAN_LENA)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DUELWATCH)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPC_DIALOGUE)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_QUEST_PROGRESS)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_QUEST_PROGRESS_ETC)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_EMPIREGUARDIAN_NPC)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DOPPELGANGER_NPC)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GENSRANKING)
		)
	{
		iWidth = DisplayWin - 190;
	}
	else
	{
		iWidth = DisplayWin;
	}
	return iWidth;
} *

esta funcion hara que no se vea el party debajo nbinguna ventana ostruyendo la misma.
