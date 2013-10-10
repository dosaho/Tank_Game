#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
/* ���İO�o�n�[�J�o�� */ 
#include "audio.h"
/* �t�~�٦����U���]�w */ 
/* For sound effect: In [Project Options]->[Parameters]->[Linker] add the parameter -lwinmm */
#include "console_draw2.h"  
#include "kb_input2.h"
/* 
putASCII2()  �̫��ӰѼƤ��O�O�r���e���M�I�����C��
�i�H�ѦҤU�����C��ȹ�Ӫ�ӳ]�w�A�Q�n���C��     
   0: ��     1: �t��   2: �t��   3: �t�C 
   4: �t��   5: �t��   6: �t��   7: �L��
   8: �t��   9: �G��  10: �G��  11: �G�C
  12: �G��  13: �G��  14: �G��  15: ��   
*/ 
#define WIDTH  60
#define HEIGHT 60
#define OFFSET_X  2
#define OFFSET_Y  2 
#define NUM_KEYS 19
#define MAX_NUM_TANKS 30
#define MAX_NUM_MISSILES 100
#define REFRESH_RATE 2
int plate[HEIGHT][WIDTH] = {0};
/* �۩w�� struct   �ΨӴy�z������U���ݩ� */ 
struct t_Tank {
   int x;
   int y;
   int direction;
   int color;
   int duration;
   int last_tick;
   int shape[3][3];
   int alive;
};
typedef struct t_Tank Tank;

struct t_Missile {
   int x;   
   int y;   
   int direction;   
   int color;   
   int duration;   
   int last_tick;   
   int shape[3][3];   
   int alive;
};
typedef struct t_Missile Missile;
int TANK_SHAPE[4][9] = {
      {0, 1 ,0, 
       3, 2, 3, 
       3, 3, 3},
      {3, 3 ,0, 
       3, 2, 1, 
       3, 3, 0},
      {3, 3 ,3, 
       3, 2, 3, 
       0, 1, 0},
      {0, 3 ,3, 
       1, 2, 3, 
       0, 3, 3}};
int MISSILE_SHAPE[3][3] = {
      {0, 0 ,0},
      {0, 1 ,0},
      {0, 0 ,0}
      };
void delay(float sec); 
void putString(int x, int y, char *p, int fg_color, int bg_color);
void genTanks(Tank *tank_p, int num);
void showTanks(Tank *tank_p, int num);
int moveTanks(Tank *tank_p, int num, int tick,int pol,int pol2);
void genMissile(Missile *missile_p, Tank *tank_p);
void showMissiles(Missile *missile_p, int num); 
int moveMissiles(Missile *missile_p, int num, int tick,int num2,Tank *tank_p);
struct t_menu {
   int x;
   int y;
   int fgcolor;
   int bgcolor;
   int cursor;
   int num_options;
   int state[10];
   int type;
   char text[10][80];   
   char alt_text[10][80];   
};
typedef struct t_menu Menu;
void setMainmenu(Menu *m)
{
   int i;
   
   m->x = 10;
   m->y = 10;
   m->fgcolor = 14;
   m->bgcolor = 0;
   m->num_options = 5;  
   m->cursor = 0; 
   m->type=0;
   for (i=0; i<5; i++) {
      m->state[i] = 0;
   }
   m->state[m->cursor] = m->state[m->cursor] | 1;
   strcpy(m->text[0], "ONE PLAYER");
   strcpy(m->text[1], "TWO PLAYERS");
   strcpy(m->text[2], "START");
   strcpy(m->text[3], "EXIT");
   strcpy(m->text[4], "ABOUT");   
   strcpy(m->alt_text[0], "ONE PLAYER *");
   strcpy(m->alt_text[1], "TWO PLAYERS *");
   strcpy(m->alt_text[2], "START *");
   strcpy(m->alt_text[3], "EXIT *");
   strcpy(m->alt_text[4], "ABOUT *");   
}
int showMenu(Menu *m)
{
   int i;
   int fg, bg,play;
   char *str;
   
   for (i=0; i<m->num_options; i++) {
      
      if (!(m->state[i] & 1)) {
         bg = m->bgcolor;
         fg = m->fgcolor;
      } else {
         fg = m->bgcolor;
         bg = m->fgcolor;
      }

      if (!(m->state[i] & 2)) 
         {
         str = m->text[i];
          if(i==0)
            play=1;
         } else {
         str = m->alt_text[i];
        if(i==1)
        play=2;
      }
   
      putString(m->x+40, m->y+i, str, fg, bg);         
   }
  return play;
}
void scrollMenu(Menu *m, int diff)
{
   m->state[m->cursor] = m->state[m->cursor] & (~1);
   m->cursor = (m->cursor + diff + m->num_options) % m->num_options;
   m->state[m->cursor] = m->state[m->cursor] | 1;
}
int my_game(int h); 
void toggleMenu(Menu *m)
{
   int i;
   for (i=0; i<m->num_options; i++) {
      m->state[i] = m->state[i] & (~2);
   }
   m->state[m->cursor] = m->state[m->cursor] | 2;
}
void selectMenu(Menu *m)
{
   m->state[m->cursor] = m->state[m->cursor] | 2;
}
int timer(float sec);

/*----------------------------------------------------------*/

int my_game(int play) 
{    
   int IsEnding = 0,sur;
   char logo[] = "SAMPLE PROGRAM PROVIDED BY I2P09";
   char pp[]="Scores:  ";
   char ppt[]="BOMBS:  ";
   int numscore=0;
   char nscore[5];
   int i, j, k;
   FILE *fp;
   int samove=0;
   fp = fopen("map.txt", "r");
    if (fp != NULL) 
    {
        for(i = 0; i < WIDTH; i++) 
        {      
            for(j = 0; j < HEIGHT; j++) 
            {           
                fscanf(fp, "%c", &plate[j][i]); 
            }
            fscanf(fp, "%*c");  
        }                   
        fclose(fp);
    }
    for(i = 0; i < HEIGHT; i++) 
        {      
            for(j = 0; j < WIDTH; j++)           
                if(plate[i][j]==49)
                    plate[i][j]=1;    
        }                   
            
   
   
   
   
   Tank tanks[MAX_NUM_TANKS];
   int num_tanks = 5;
   
   clock_t startc;    
   int tick = 0;
   int a1=10,a2=10,a3=10,a4=10,a5=10,a6=100;
   int key_down[NUM_KEYS] = {0};
   int key_val[NUM_KEYS] = {VK_A, VK_S, VK_D, VK_F, VK_G, VK_ESCAPE, VK_Q,VK_UP,VK_DOWN,VK_RIGHT,VK_LEFT,VK_SPACE,VK_RETURN,VK_NUMPAD4,VK_NUMPAD5,VK_NUMPAD6,VK_NUMPAD8,VK_NUMPAD0};
   char aa1[5],aa2[5],aa3[5],aa4[5],aa5[5],aa6[5];
   char str[40] = {'\0'};
   int vk,pol,samove2,pol2;
   
   Missile missile[MAX_NUM_MISSILES];   
   int num_missiles = 0,pl=0;;
   
   /* �ΨӰO�����Ī� struct �}�C */ 
   Audio audio[6];
   
   /* Ū�J���ӭ����� */ 
   
 	/* �I������ */
   	/* ���Ƽ��� */

   /* �}�l����I������  */
     for (i=0; i < MAX_NUM_MISSILES; i++) {      
       missile[i].alive = 0;   
       }
   
   
   /* �Q�W���䪺����M�̩��U���a�O�@*/ 
   if(fp==NULL)
   {
       for (i = 0; i < HEIGHT; i++) 
       {
           plate[i][0] = 1;
           plate[i][WIDTH-1] = 1;
       }
       for (j = 0; j < WIDTH; j++) 
       {
           plate[0][j] = 1;
           plate[HEIGHT-1][j] = 1;
       }
    }

   
   
   /* ���Ͷü� seed    ���᭱�I�s rand() �i�H�o�줣�P���üƧǦC  */ 
   srand(time(NULL));   

   /* �Ψӭp�� */ 
   startc = clock();

   /* �Ұ���L���� */ 
   initializeKeyInput(); 
   if(play==2)
   num_tanks+=1;
   
   
   /* ���ͩZ�J */ 
   genTanks(tanks, num_tanks);
   
   


	/* �L�a�j��  �̭��]�t�F�o�ӵ{�����D�n�u�@   
   �ӥB�C���j��|���_��̷s���e����ܨ�ù��W  ���O���ʵe�@�� �C���j���s�@���e�� */
   while (!IsEnding) { 
      
      /* �ۤv�]�w���ɶ���� tick  �C 0.05 ����@�� tick   �᭱�|�Q�� tick �ƨӨM�w������ʳt�� */ 
      if ((double)(clock()-startc) > 0.05*CLOCKS_PER_SEC) {
         tick++;
         startc = clock();
      }
      samove=0;
      samove2=0;
      /* ��ثe�C�����p �� putASCII2() ��ܥX��  
         �C�g�L REFRESH_RATE �� ticks �~�|��s�@���e�� */  
      if (tick % REFRESH_RATE == 1) {
         
         for (i = 0; i < HEIGHT; i++) {
            for (j = 0; j < WIDTH; j++) {
               if (plate[i][j] == 1) {
                  putASCII2(i*2 + OFFSET_X, j + OFFSET_Y, 0xA1, 8, 0);  /* fg_color=8 �N��e�����C��O�Ǧ� */ 
                  putASCII2(i*2+1 + OFFSET_X, j + OFFSET_Y, 0xBD, 8, 0);
               } else {
                  putASCII2(i*2 + OFFSET_X, j + OFFSET_Y, ' ', 0, 0);
                  putASCII2(i*2+1 + OFFSET_X, j + OFFSET_Y, ' ', 0, 0);
               }
            }
         } 
         /* ��Z�J�e�X�� */      
         showTanks(tanks, num_tanks);
         showMissiles(missile , num_missiles);  
      
           
      /*  �� logo �� putString() ��ܥX�� 
         putString() �O�۩w�� function    ����b�S�w��m��ܦr�� 
         �o�� function ���{���X�]��b�o���ɮ׸�  �b main() ���� 
         �{���X����²�u  �u�O�b�j��̩I�s putASCII2() �Ӥw       */ 
         
         
         putString(OFFSET_X, OFFSET_Y-2, pp, 14, 3);
         sprintf(nscore, "%4d", numscore);
         putString(OFFSET_X+8, OFFSET_Y-2, nscore, 14, 3);
         
      /* ���F�n���@�s�� putASCII2() ���ʧ@���ͮĪG  
         �����n�I�s�@�� drawCmdWindow() �⤧�e�e���������e�@����ܨ�ù��W */ 
         drawCmdWindow();      
         
      } 
      /* end of if (tick%10) */ 

       
      /* ��L���� �B�z���ۤ��񪺪��p */ 
      if(waitForKeyDown(0.01)) {
         vk=getKeyEventVirtual();
            
            /* �̷ӧA�Q�n���ʧ@�ӳB�z���� */
            /* �b�o�ӨҤl�� ���U A, S, D, F, G 
            �i�H���O���� ���x�Z�J���C��  �o�X���ؤ��P���� 
            ���O�Y���ۤ��� �h�u�|�o�ͤ@���@��
            �P�ɫܦh��@�_���� ���|���ۤz�Z */ 
            switch (vk) {            
               case VK_A:            
                  tanks[0].color=(tanks[0].color+1)%15 + 1;
                  /* ������������� */ 
                  playAudio(&audio[0]);
                  if(a1!=0)
                  {
                  if (num_missiles < MAX_NUM_MISSILES) {		
                                   genMissile(&missile[num_missiles], &tanks[0]);		
                                   num_missiles++;	
                                   }
                                   a1--;
                                   }
                  break;
               case VK_S:            
                  tanks[1].color=(tanks[1].color+1)%15 + 1;
                  /* ������������� */ 
                  playAudio(&audio[1]);
                  if(a2!=0)
                  {
                  if (num_missiles < MAX_NUM_MISSILES) {		
                                   genMissile(&missile[num_missiles], &tanks[1]);		
                                   num_missiles++;	
                                   }
                                   a2--;
                                   }
                  break;
               case VK_D:            
                  
                  tanks[2].color=(tanks[2].color+1)%15 + 1;
                  /* ������������� */ 
                  playAudio(&audio[2]);
                  if(a3!=0){
                  if (num_missiles < MAX_NUM_MISSILES) {		
                                   genMissile(&missile[num_missiles], &tanks[2]);		
                                   num_missiles++;	
                                   }
                                   a3--;
                                   }
                  break;
               case VK_F:            
                  tanks[3].color=(tanks[3].color+1)%15 + 1;
                  /* ������������� */ 
                  playAudio(&audio[3]);
                  if(a4!=0){
                  if (num_missiles < MAX_NUM_MISSILES) {		
                                   genMissile(&missile[num_missiles], &tanks[3]);		
                                   num_missiles++;	
                                   }
                                   a4--;
                                   }
                  break;
               case VK_G:            
                  tanks[4].color=(tanks[4].color+1)%15 + 1;
                  /* ������������� */ 
                  playAudio(&audio[4]);
                  if(a5!=0){
                  if (num_missiles < MAX_NUM_MISSILES) {		
                                   genMissile(&missile[num_missiles], &tanks[4]);		
                                   num_missiles++;	
                                   }
                                   a5--;
                                   }
                  break;
               case VK_ESCAPE:            
                  
                  /* �Ȱ��I������ */
                  pauseAudio(&audio[5]);
                  
                  /* �Ȯɶi�J�@���J�Ҧ� */ 
                  SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), (COORD){70, 100} );
                  SetConsoleMode (GetStdHandle (STD_INPUT_HANDLE), ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT); 
                  scanf("%39s", str);
                  SetConsoleMode (GetStdHandle (STD_INPUT_HANDLE), ENABLE_LINE_INPUT); 
                  
                  /* �~�򼽩�쥻������ */ 
                  restartAudio(&audio[5]);
                  break;
               case VK_Q:          
                  IsEnding = 1;
                  break;
               case VK_UP:
                  tanks[5].y -= 1;
                  for(i=tanks[5].x;i<=tanks[5].x+2;i++)
                  {
                      for(j=tanks[5].y;j<=tanks[5].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tanks[5].y += 1;
                  pol=0;
                  break;
               case VK_DOWN:
                  tanks[5].y += 1;
                  for(i=tanks[5].x;i<=tanks[5].x+2;i++)
                  {
                      for(j=tanks[5].y;j<=tanks[5].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tanks[5].y -= 1;
                  pol=2;
                  break;
               case VK_LEFT:
                  tanks[5].x -= 1;
                  for(i=tanks[5].x;i<=tanks[5].x+2;i++)
                  {
                      for(j=tanks[5].y;j<=tanks[5].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tanks[5].x += 1;
                  pol=3;
                  break;
               case VK_RIGHT:
                  tanks[5].x += 1;
                  for(i=tanks[5].x;i<=tanks[5].x+2;i++)
                  {
                      for(j=tanks[5].y;j<=tanks[5].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tanks[5].x -= 1;
                  pol=1;
                  break;   
               case VK_SPACE:
                  
                  if(a6!=0)
                  {
                      if (num_missiles < MAX_NUM_MISSILES) 
                      {		
                          genMissile(&missile[num_missiles], &tanks[5]);		
                          num_missiles++;	
                      }
                      a6--;
                  }
                  break;
              case VK_NUMPAD0:
                  tanks[6].color=(tanks[6].color+1)%15 + 1;
                  if(a6!=0)
                  {
                      if (num_missiles < MAX_NUM_MISSILES) 
                      {		
                          genMissile(&missile[num_missiles], &tanks[6]);		
                          num_missiles++;	
                      }
                      a6--;
                  }
                  break; 
              case VK_RETURN:
                  break;
              case VK_NUMPAD8:
                  tanks[6].y -= 1;
                  for(i=tanks[6].x;i<=tanks[6].x+2;i++)
                  {
                      for(j=tanks[6].y;j<=tanks[6].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove2=1;
                      }
                  }
                  if(samove2==1)
                  tanks[6].y += 1;
                  pol2=0;
                  break;
             case VK_NUMPAD5:
                  tanks[6].y += 1;
                  for(i=tanks[6].x;i<=tanks[6].x+2;i++)
                  {
                      for(j=tanks[6].y;j<=tanks[6].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove2=1;
                      }
                  }
                  if(samove2==1)
                  tanks[6].y -= 1;
                  pol2=2;
                  break;
             case VK_NUMPAD4:
                  tanks[6].x -= 1;
                  for(i=tanks[6].x;i<=tanks[6].x+2;i++)
                  {
                      for(j=tanks[6].y;j<=tanks[6].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove2=1;
                      }
                  }
                  if(samove2==1)
                  tanks[6].x += 1;
                  pol2=3;
                  break;
             case VK_NUMPAD6:
                  tanks[6].x += 1;
                  for(i=tanks[6].x;i<=tanks[6].x+2;i++)
                  {
                      for(j=tanks[6].y;j<=tanks[6].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove2=1;
                      }
                  }
                  if(samove2==1)
                  tanks[6].x -= 1;
                  pol2=1;
                  break;  
            
            
            }
            
         
         
         /* ����q�쥻�Q���U�����A �ܦ���}�����A  �o�O���F�B�z���ۤ��񪺱��p */ 
        
                
      }
      /* ���ʩZ�J */
      moveTanks(tanks, num_tanks, tick, pol,pol2);
      pl=moveMissiles(missile , num_missiles,  tick,num_tanks,tanks);
      if(pl==10)
      {
      numscore+=10;
      pl=0;
      }
      for(sur=0;sur<num_tanks;sur++)
          {
          if(rand()%50==0&&tanks[sur].alive==0)
             {
              tanks[sur].alive=1;
              tanks[sur].x=56;
              tanks[sur].y=1;
              }
          }     
      while(num_missiles > 0) {		
                         if (missile[num_missiles-1].alive != 0)			
                         break;		
                         num_missiles--;	
                         }
                                   
   } /* while (IsEnding) */      
 

   /* �C�������e �Ȱ� 0.5 ��*/ 
   delay(0.5);   
   return 0; 
} 
/* ���{���Ȱ� sec �� */ 
void delay(float sec)
{
   clock_t startc;    
   startc = clock();
   for ( ; ; ) {	
	  if ((float)(clock()-startc)/CLOCKS_PER_SEC > sec) break;
   } 	 
} 
/* �b (x, y) �y�Ъ���m��ܦr�� p �����e  fg_color �O�e�����C��  bg_color �h�O�I�����C�� */ 
void putString(int x, int y, char *p, int fg_color, int bg_color)
{
   int i;
   for(i=0; i<strlen(p); i++) {
      putASCII2(x+i, y, p[i], fg_color, bg_color);
   }
}
/****************************************************************************************************/
void genTanks(Tank *tank_p, int num)
{
   int i, j, k,c;

   for (k=0; k<num+1; k++) {
      tank_p[k].alive=1;
      tank_p[k].x = 56;
      tank_p[k].y = 1;
      if(k==5)
      {
      tank_p[k].x=1;
      tank_p[k].y=1;
      }        
      if(k==6)
      {
      tank_p[k].x=7;
      tank_p[k].y=1;
      }   
      c=rand()%15 + 1;
      while(c<=8)
          {
          c=rand()%15 + 1;
          }
      tank_p[k].color = c;

      tank_p[k].duration = 1;
      tank_p[k].last_tick = 0;

      tank_p[k].direction = rand()%3;
      
      for (i=0; i<3; i++) {
         for (j=0; j<3; j++) {
            tank_p[k].shape[i][j] 
            = TANK_SHAPE[tank_p[k].direction][i*3+j];
         }
      }
                
   }   
   
}

void genMissile(Missile *missile_p, Tank *tank_p)
{
     missile_p->x = tank_p->x;
     missile_p->y = tank_p->y;
     missile_p->duration=1;
     missile_p->alive=1;
     missile_p->direction=tank_p->direction;
     missile_p->color=tank_p->color;
     missile_p->last_tick = 0;
     }

void showTanks(Tank *tank_p, int num)
{
   int i, j, k;
   
   for (k=0; k<num+1; k++) {  
      if(tank_p[k].alive!=0)
      {
      for (i=0; i<3; i++) {
         for (j=0; j<3; j++) {              
            switch (tank_p[k].shape[i][j]) {
               case 1:
               putASCII2((tank_p[k].x+ j)*2  + OFFSET_X, 
               tank_p[k].y + i + OFFSET_Y, 0xA1, tank_p[k].color, 0);  
               putASCII2((tank_p[k].x+ j)*2+1  + OFFSET_X, 
               tank_p[k].y + i +  OFFSET_Y, 0xBD, tank_p[k].color, 0);               
               break;
               
               case 2:
               putASCII2((tank_p[k].x+ j)*2  + OFFSET_X, 
               tank_p[k].y + i + OFFSET_Y, 0xA1, tank_p[k].color, 0);  
               putASCII2((tank_p[k].x+ j)*2+1  + OFFSET_X, 
               tank_p[k].y + i +  OFFSET_Y, 0xB4, tank_p[k].color, 0);               
               break;
               
               case 3:
               putASCII2((tank_p[k].x+ j)*2  + OFFSET_X, 
               tank_p[k].y + i + OFFSET_Y, 0xCA, tank_p[k].color, 0);  
               putASCII2((tank_p[k].x+ j)*2+1  + OFFSET_X, 
               tank_p[k].y + i +  OFFSET_Y, 0xA8, tank_p[k].color, 0);               
               break;
               
            }
         }
      }
     } 
   }

}

void showMissiles(Missile *missile_p, int num){
     int i, j, k;
   for (k=0; k<num; k++) {
     if(missile_p[k].alive!=0){  
               putASCII2((missile_p[k].x+ 1)*2  + OFFSET_X, 
               missile_p[k].y + 1 + OFFSET_Y, 0xA1, missile_p[k].color, 0);  
               putASCII2((missile_p[k].x+ 1)*2+1  + OFFSET_X, 
               missile_p[k].y + 1 +  OFFSET_Y, 0xB4, missile_p[k].color, 0);
     }
   }
}

int moveTanks(Tank *tank_p, int num, int tick,int pol,int pol2)
{
    int i, j, k,samove=0;
    if (pol==3)
    {
        tank_p[5].direction=3;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[5].shape[i][j] = TANK_SHAPE[3][i*3+j];
        }
        
     
    }
    else if(pol==0)
    {
        tank_p[5].direction=0;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[5].shape[i][j] = TANK_SHAPE[0][i*3+j];
        }
        
        
    }
    else if(pol==1)
    {
        tank_p[5].direction=1;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[5].shape[i][j] = TANK_SHAPE[1][i*3+j];
        }
        
       
    }
    else if(pol==2)
    {
        tank_p[5].direction=2;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[5].shape[i][j] = TANK_SHAPE[2][i*3+j];
        }
        
        
    }                      
   if (pol2==3)
    {
        tank_p[6].direction=3;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[6].shape[i][j] = TANK_SHAPE[3][i*3+j];
        }
        
     
    }
    else if(pol2==0)
    {
        tank_p[6].direction=0;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[6].shape[i][j] = TANK_SHAPE[0][i*3+j];
        }
        
        
    }
    else if(pol2==1)
    {
        tank_p[6].direction=1;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[6].shape[i][j] = TANK_SHAPE[1][i*3+j];
        }
        
       
    }
    else if(pol2==2)
    {
        tank_p[6].direction=2;
        for (i=0; i<3; i++) 
        {
            for (j=0; j<3; j++) 
                tank_p[6].shape[i][j] = TANK_SHAPE[2][i*3+j];
        }
    }
   for (k=0; k<num; k++) {
      if (tick-tank_p[k].last_tick > tank_p[k].duration&&k!=5) {
         switch (tank_p[k].direction) {
            case 0:
               tank_p[k].y -= 1;
               for(i=tank_p[k].x;i<=tank_p[k].x+2;i++)
                  {
                      for(j=tank_p[k].y;j<=tank_p[k].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tank_p[k].y += 1;
               break;
            case 1: 
               tank_p[k].x += 1;
               for(i=tank_p[k].x;i<=tank_p[k].x+2;i++)
                  {
                      for(j=tank_p[k].y;j<=tank_p[k].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tank_p[k].x -= 1;
               break;
            case 2:
               tank_p[k].y += 1;
               for(i=tank_p[k].x;i<=tank_p[k].x+2;i++)
                  {
                      for(j=tank_p[k].y;j<=tank_p[k].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tank_p[k].y -= 1;
               break;
            case 3: 
               tank_p[k].x -= 1;
               for(i=tank_p[k].x;i<=tank_p[k].x+2;i++)
                  {
                      for(j=tank_p[k].y;j<=tank_p[k].y+2;j++)    
                      {
                          if(plate[i][j]==1)
                          samove=1;
                      }
                  }
                  if(samove==1)
                  tank_p[k].x += 1;
               break;
         }
         
         if (tank_p[k].x < 1) {
            tank_p[k].direction = rand()%4;
            tank_p[k].x += 1;
         }
         else if (tank_p[k].x > WIDTH-4) {
            tank_p[k].direction = rand()%4;
            tank_p[k].x -= 1;
         }
         else if (tank_p[k].y < 1) {
            tank_p[k].direction = rand()%4;
            tank_p[k].y += 1;
         }
         else if (tank_p[k].y > HEIGHT-4) {
            tank_p[k].direction = rand()%4;
            tank_p[k].y -= 1;
         }
         if(samove==1)
             samove=0;
             
         /* ���ʤ����G�Q�����v�|�A���ܤ�V */ 
         if (rand()%10 > 7) {
            tank_p[k].direction = rand()%4;
         }                        
         /* ���ʤ����G�Q�����v�|�A���ܳt�� */ 
         if (rand()%10 > 7) {
            tank_p[k].duration = 1;
         }                        
         
         tank_p[k].last_tick = tick;         
      
         for (i=0; i<3; i++) {
            for (j=0; j<3; j++) {
               tank_p[k].shape[i][j] 
               = TANK_SHAPE[tank_p[k].direction][i*3+j];
            }
         }
      }             
   }   
}
int moveMissiles(Missile *missile_p, int num, int tick,int num2,Tank *tank_p)
{
    int k,pop=0,l,i,j,samove=0,pl;
    for (k=0; k<num; k++) {
      if (tick-missile_p[k].last_tick > missile_p[k].duration) {
         switch (missile_p[k].direction) {
            case 0:
               missile_p[k].y -= 1;
               break;
            case 1: 
               missile_p[k].x += 1;
               break;
            case 2:
               missile_p[k].y += 1;
               break;
            case 3: 
               missile_p[k].x -= 1;
               break;
         }
         if (missile_p[k].x < 0) {
            missile_p[k].alive=0;
         }
         else if (missile_p[k].x > WIDTH-3) {
            missile_p[k].alive=0;
         }
         else if (missile_p[k].y < 0) {
            missile_p[k].alive=0;
         }
         else if (missile_p[k].y > HEIGHT-6) {
            missile_p[k].alive=0;
         }
         else if(plate[missile_p[k].x+1][missile_p[k].y+1]==1&&missile_p[k].alive==1)
         {
             plate[missile_p[k].x+1][missile_p[k].y+1]=0;
             missile_p[k].alive=0;
         }
         for(l=0;l<=num2;l++)
         {
             
             if(missile_p[k].x==tank_p[l].x&&missile_p[k].y==tank_p[l].y)
                  {
                  tank_p[l].alive=0;
                  missile_p[k].alive=0;
                  tank_p[l].x=0;
                  tank_p[l].y=0;
                  pl=10;
                  }
         }
         
         if(missile_p[k].x&&missile_p[k].y)
         missile_p[k].last_tick = tick;
      }
    
    }
    
  return pl;
}
int timer(float sec)
{
   static clock_t startc;
   static tick;

   if ((double)(clock()-startc) > sec*CLOCKS_PER_SEC) {
      tick++;
      startc = clock();
   }
   return tick;
} 
/*----------------------------------------------------------*/
int main(void) 
{    
   int IsEnding = 0;  
   int i, j, k;
   char str[40] = {'\0'},player=0;
   int key_down[NUM_KEYS] = {0};
   int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN};
   int cur_tick, last_tick;     
   Menu mainmenu;
   for (i = 0; i < HEIGHT; i++) {
      plate[i][0] = 1;
      plate[i][WIDTH-1] = 1;
   }
   for (j = 0; j < WIDTH; j++) {
      plate[0][j] = 1;
      plate[HEIGHT-1][j] = 1;
   }
   initializeKeyInput(); 
   setMainmenu(&mainmenu);
   while (!IsEnding) { 
      cur_tick = timer(0.005);
      if (cur_tick % REFRESH_RATE == 0 && cur_tick != last_tick) {
         
         last_tick = cur_tick;
         
         clearScreen();
         
         
         putString(OFFSET_X, OFFSET_Y-1, str, 14, 1);

         player=showMenu(&mainmenu);

      /* ���F�n���@�s�� putASCII2() ���ʧ@���ͮĪG  
         �����n�I�s�@�� drawCmdWindow() �⤧�e�e���������e�@����ܨ�ù��W */ 
         drawCmdWindow();      

      } 
       for (k=0; k<NUM_KEYS; k++) {
         if(KEY_DOWN(key_val[k]) && !key_down[k]) {
            key_down[k] = 1;

            switch (key_val[k]) {            
               case VK_UP:
                  scrollMenu(&mainmenu, -1);
                  break;
               case VK_DOWN:            
                  scrollMenu(&mainmenu, +1);
                  break;
               case VK_RETURN:
                  toggleMenu(&mainmenu); 
                  break;
               case VK_ESCAPE:          
                  IsEnding = 1;
                  break;

                  
            }
            
         }
         
         /* ����q�쥻�Q���U�����A �ܦ���}�����A  �o�O���F�B�z���ۤ��񪺱��p */ 
         if(KEY_UP(key_val[k]) && key_down[k]) {
            key_down[k] = 0;    
         }
                
      }

      if ((mainmenu.state[2] & 2)) {
         /* start your game */
         mainmenu.state[2] = mainmenu.state[2] & (~2);
         my_game(player);
      }
      if ((mainmenu.state[3] & 2)) {
         IsEnding = 1;         
      }

                                   
   } /* while (IsEnding) */      
 

   /* �C�������e �Ȱ� 0.5 ��*/ 
   delay(0.5);   
   return 0; 
} 
