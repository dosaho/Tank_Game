#include <windows.h>
#ifndef KB_INPUT2_
#define KB_INPUT2_

extern void	initializeKeyInput(void);
extern int waitForKeyDown(float sec);
extern int getKeyEventASCII(void);
extern int getKeyEventVirtual(void);

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define VK_0			0x30   
#define VK_1			0x31   
#define VK_2			0x32   
#define VK_3			0x33   
#define VK_4			0x34   
#define VK_5			0x35   
#define VK_6			0x36   
#define VK_7			0x37   
#define VK_8			0x38   
#define VK_9			0x39   

#define VK_A			0x41   
#define VK_B			0x42   
#define VK_C			0x43   
#define VK_D			0x44   
#define VK_E			0x45  
#define VK_F			0x46   
#define VK_G			0x47   
#define VK_H			0x48   
#define VK_I			0x49  
#define VK_J			0x4A   
#define VK_K			0x4B   
#define VK_L			0x4C  
#define VK_M			0x4D   
#define VK_N			0x4E   
#define VK_O			0x4F   
#define VK_P			0x50  
#define VK_Q			0x51   
#define VK_R			0x52   
#define VK_S			0x53   
#define VK_T			0x54   
#define VK_U			0x55   
#define VK_V			0x56   
#define VK_W			0x57   
#define VK_X			0x58   
#define VK_Y			0x59   
#define VK_Z			0x5A   


#endif

/* 
底下是 已經有定義的 virtual key 名稱
參考這些名稱來判斷按鍵 

VK_LBUTTON	0x01    Left mouse button  
VK_RBUTTON	0x02    Right mouse button  
VK_CANCEL	0x03    Control-break processing  
VK_MBUTTON	0x04    Middle mouse button (three-button mouse)  
                       
VK_BACK		0x08    backspace key  
VK_TAB		0x09    tab key  
                       
VK_CLEAR	0x0C    clear key  
VK_RETURN	0x0D    enter key  
                       
VK_SHIFT	0x10    shift key  
VK_CONTROL	0x11    ctrl key  
VK_MENU		0x12    alt key  
VK_PAUSE	0x13    pause key  
VK_CAPITAL	0x14    caps lock key  
                       
VK_ESCAPE	0x1B    esc key  
VK_SPACE	0x20    spacebar  
VK_PRIOR	0x21    page up key  
VK_NEXT		0x22    page down key  
VK_END		0x23    end key  
VK_HOME		0x24    home key  
VK_LEFT		0x25    left arrow key  
VK_UP		0x26    up arrow key  
VK_RIGHT	0x27    right arrow key  
VK_DOWN		0x28    down arrow key  
VK_SELECT	0x29    select key  
VK_EXECUTE	0x2B    execute key  
VK_SNAPSHOT	0x2C    print screen key  
VK_INSERT	0x2D    ins key  
VK_DELETE	0x2E    del key  
VK_HELP		0x2F    help key  
VK_LWIN		0x5B    Left Windows key (Microsoft Natural Keyboard)  
VK_RWIN		0x5C    Right Windows key (Microsoft Natural Keyboard)  
VK_APPS		0x5D    Applications key (Microsoft Natural Keyboard)                         
VK_NUMPAD0	0x60    Numeric keypad 0 key  
VK_NUMPAD1	0x61    Numeric keypad 1 key  
VK_NUMPAD2	0x62    Numeric keypad 2 key  
VK_NUMPAD3	0x63    Numeric keypad 3 key  
VK_NUMPAD4	0x64    Numeric keypad 4 key  
VK_NUMPAD5	0x65    Numeric keypad 5 key  
VK_NUMPAD6	0x66    Numeric keypad 6 key  
VK_NUMPAD7	0x67    Numeric keypad 7 key  
VK_NUMPAD8	0x68    Numeric keypad 8 key  
VK_NUMPAD9	0x69    Numeric keypad 9 key  
VK_MULTIPLY	0x6A    Multiply key  
VK_ADD		0x6B    Add key  
VK_SEPARATOR	0x6C    Separator key  
VK_SUBTRACT	0x6D    Subtract key  
VK_DECIMAL	0x6E    Decimal key  
VK_DIVIDE	0x6F    Divide key  
VK_F1		0x70    f1 key  
VK_F2		0x71    f2 key  
VK_F3		0x72    f3 key  
VK_F4		0x73    f4 key  
VK_F5		0x74    f5 key  
VK_F6		0x75    f6 key  
VK_F7		0x76    f7 key  
VK_F8		0x77    f8 key  
VK_F9		0x78    f9 key  
VK_F10		0x79    f10 key  
VK_F11		0x7A    f11 key  
VK_F12		0x7B    f12 key  
VK_F13		0x7C    f13 key  
VK_F14		0x7D    f14 key  
VK_F15		0x7E    f15 key  
VK_F16		0x7F    f16 key  
VK_F17		0x80H    f17 key  
VK_F18		0x81H    f18 key  
VK_F19		0x82H    f19 key  
VK_F20		0x83H    f20 key  
VK_F21		0x84H    f21 key  
VK_F22		0x85H    f22 key  
VK_F23		0x86H    f23 key  
VK_F24		0x87H    f24 key  

VK_NUMLOCK	0x90    num lock key  
VK_SCROLL	0x91    scroll lock key  
                       
VK_ATTN		0xF6    Attn key 
VK_CRSEL	0xF7    CrSel key 
VK_EXSEL	0xF8    ExSel key 
VK_EREOF	0xF9    Erase EOF key 
VK_PLAY		0xFA    Play key 
VK_ZOOM		0xFB    Zoom key 
VK_NONAME	0xFC    Reserved for future use.  
VK_PA1		0xFD    PA1 key 
VK_OEM_CLEAR	0xFE    Clear key
*/ 
