#include"keyboard.h"

unsigned char get_key()
{
    unsigned char row, col;
    // 矩阵键盘按键映射数组
    unsigned char keypad[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    // 遍历键盘的列
    for(col = 0; col < 4; col++)
    {
        // 设置当前列为输出低电平
        KeyData = ~(0x10 << col);

        // 遍历键盘的行
        for(row = 0; row < 4; row++)
        {
            // 检测当前行是否有输入信号
            if(!(KeyData & (1 << row)))
            {
                // 返回对应的按键值
                return keypad[row][col];
            }
        }
    }

    // 如果没有按键按下，返回0xff
    return 0xff;
}