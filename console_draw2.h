#include <windows.h>
#ifndef CONSOLE_DRAW2_
#define CONSOLE_DRAW2_
#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200
extern void putASCII2( int x, int y, int ch, int fgcolor, int bgcolor );
extern void drawCmdWindow();
extern void clearScreen();
#endif
/* 
putASCII2()  �̫��ӰѼƤ��O�O�r���e���M�I�����C��
�i�H�ѦҤU�����C��ȹ�Ӫ�ӳ]�w�A�Q�n���C��  
   
   0: ��     1: �t��   2: �t��   3: �t�C 
   4: �t��   5: �t��   6: �t��   7: �L��
   8: �t��   9: �G��  10: �G��  11: �G�C
  12: �G��  13: �G��  14: �G��  15: ��   
*/ 
