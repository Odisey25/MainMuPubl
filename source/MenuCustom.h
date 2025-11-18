#pragma once

enum ButtonMenuCustom
{
    eButtonEventTime,
    eButtonVipShop,
    eButtonRanking,
    eButtonChangeClass,
    eButtonMarKet,
    eButtonDanhHieu,
    eButtonNapGame,
    eButtonJewelBank,

    eButtonMaxValue,
};

class cCustomMenu
{
public:
    cCustomMenu();
    ~cCustomMenu();
    void GetCountButton();
    void ActionButton(int TypeButton);
    void Draw();

    // Khai báo phương thức ToolTipPegasus trong class
    void ToolTipPegasus(int sx, int sy, const char* Text);

    void OpenCustomMenu();
};

// Khai báo đối tượng toàn cục cCustomMenu
extern cCustomMenu gBCustomMenu;
