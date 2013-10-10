/*putString(OFFSET_X, OFFSET_Y-2, logo, 14, 3);

         putString(OFFSET_X, OFFSET_Y-1, str, 14, 1);
         putString(OFFSET_X+62, OFFSET_Y+6, pp, 14, 3);
         putString(OFFSET_X+67+3, OFFSET_Y+6, aa1, 14, 3);
         putString(OFFSET_X+62, OFFSET_Y+8, pp, 14, 3);
         putString(OFFSET_X+62, OFFSET_Y+10, pp, 14, 3);
         putString(OFFSET_X+62, OFFSET_Y+12, pp, 14, 3);
         putString(OFFSET_X+62, OFFSET_Y+14, pp, 14, 3);
         putString(OFFSET_X+62+5+3, OFFSET_Y+8, aa2, 14, 3);
         putString(OFFSET_X+62+5+3, OFFSET_Y+10, aa3, 14, 3);
         putString(OFFSET_X+62+5+3, OFFSET_Y+12, aa4, 14, 3);
         putString(OFFSET_X+62+5+3, OFFSET_Y+14, aa5, 14, 3);
         
         putString(OFFSET_X+62, OFFSET_Y+2, ppt, 14, 3);
         putString(OFFSET_X+67+3, OFFSET_Y+2, aa6, 14, 3);
         
         sprintf(aa1, "%4d", a1);
       sprintf(aa2, "%4d", a2);
       sprintf(aa3, "%4d", a3);
       sprintf(aa4, "%4d", a4);
       sprintf(aa5, "%4d", a5);
       sprintf(aa6, "%4d", a6);
       
       void genTanks(Tank *tank_p, int num)
{
   int i, j, k;

   for (k=0; k<num+1; k++) {
      tank_p[k].x = rand()%(WIDTH-4) + 1;
      tank_p[k].y = rand()%(HEIGHT-4) + 1;
      tank_p[k].color = rand()%15 + 1;

      tank_p[k].duration = 30 + rand()%20;
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

         /* 有百分之二十的機率會再改變方向 */ 
         /*
         if (rand()%10 > 7) {
            tank_p[k].direction = rand()%4;
         }                        
         /* 有百分之二十的機率會再改變速度 */ 
         /*
         if (rand()%10 > 7) {
            tank_p[k].duration = 30 + rand()%20;
         }                        
         */
         /*
         for (k=0; k<num; k++) {
      if (tick-tank_p[k].last_tick > tank_p[k].duration) {
         switch (tank_p[k].direction) {
            case 0:
               tank_p[k].y -= 1;
               break;
            case 1: 
               tank_p[k].x += 1;
               break;
            case 2:
               tank_p[k].y += 1;
               break;
            case 3: 
               tank_p[k].x -= 1;
               break;
         }
         
         
         
         tank_p[k].last_tick = tick;   */      
