// CustomWingEffect.cpp: implementation of the CCustomWingEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomWing.h"
#include "CustomItemStack.h"
#include "NewUISystem.h"

CCustomItemStack gCustomItemStack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomItemStack::CCustomItemStack() // OK
{
	this->m_ItemStack.clear();
}

CCustomItemStack::~CCustomItemStack() // OK
{

}


void CCustomItemStack::Load(ITEM_STACK_INFO* info) // OK
{
	for(int n=0;n < MAX_ITEM_STACK_INFO;n++)
	{
		if(info[n].Index < 0 || info[n].Index >= MAX_ITEM_STACK_INFO)
		{
			//continue;
			return;
		}
		
		this->m_ItemStack.insert(std::pair<int, ITEM_STACK_INFO>(info[n].Index, info[n]));

		//g_ConsoleDebug->Write(MCD_ERROR, "ItemStackSize %d ~ ItemIndex %d", this->m_ItemStack.size(), info[n].ItemIndex);
	}
}


bool CCustomItemStack::IsStackItem(DWORD ItemIndex, DWORD Level) // OK
{
	

	for (std::map<int, ITEM_STACK_INFO>::iterator it = this->m_ItemStack.begin(); it != this->m_ItemStack.end(); it++)
	{
		
		if (ItemIndex == it->second.ItemIndex && ItemIndex != 0 && Level == it->second.Level)
		{
			return true;
		}
	}
	
	return false;

}