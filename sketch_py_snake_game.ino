#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,11,10,9,8);
/* shift regiter ds_col and d_row inputs are the data shown at output. 
If 3 clk-pulses are given to sh_col, then output data is shifted by 3 to right but the output is appeard only if a clk-pulse is also given to st_col   
*/
#define ds_col 15
#define sh_col 16
#define st_col 14
#define ds_row 17
#define start 3

int DatafromUser;
char Col[21],Row[21],move_c,move_r;
/*length of snake is originally 2 and is increased by 1 after each correct bite with max_length=20
 arrays Col and Row show the position of different parts of moving snake. At each time-step head is moved based on the user command 
 and the next part of body is moved to head and so on (the last 5 lines of code).
 Col and Row are filled by default 100 then functions Colum_data and Row_data return zero by default
 after each correct bite the j is incremented by 1 and Col and Row (already 100 by default) are replaced with correct location  
*/
int colum_data(int temp)
{
  switch(temp)
  {
   case 1: return 1;break;
   case 2: return 2; break;
   case 3: return 4; break;
   case 4: return 8; break;
   case 5: return 16; break;
   case 6: return 32; break;
   case 7: return 64; break;
   case 8: return 128; break;
   default: return 0; break;
  }
}

int row_data(int temp)
{
  switch(temp)
  {
   case 1: return 1;break;
   case 2: return 2; break;
   case 3: return 4; break;
   case 4: return 8; break;
   case 5: return 16; break;
   case 6: return 32; break;
   case 7: return 64; break;
   case 8: return 128; break;
   default: return 0; break;
  }
}

void read_serial_command()
{
  if (Serial.available()>0){

    DatafromUser = Serial.read(); 
    //left
    if (DatafromUser == '0'){  
       move_r=0;
       move_c!=-1 ? move_c=-1 : move_c=1;
    }
    //right
    if (DatafromUser == '1'){
       move_r=0;
       move_c!=1 ? move_c=1 : move_c=-1;
    }
    //down
    if (DatafromUser == '2'){  
       move_c=0;
       move_r!=-1 ? move_r=-1 : move_r=1;
    }
    //up
    if (DatafromUser == '3'){
       move_c=0;
       move_r!=1 ? move_r=1 : move_r=-1;
    }  
  }
}

void show_snake(int temp)
{
 for(int n=0;n<temp;n++)
 {
   int r,c;
  for(int k=0;k<21;k++)
  {
   int temp1=Col[k];
   c=colum_data(temp1);
   int temp2=Row[k];
   r=0xff-row_data(temp2);
    //this loop loads bit by bit the 8-bit c and r values correctly at output of shift register by giving 8 clock to sh_col
   for(int i=0;i<8;i++)
   {   
         int ds=(c & 0x01);
         digitalWrite(ds_col, ds);
         ds=(r & 0x01);
         digitalWrite(ds_row, ds);
         digitalWrite(sh_col, HIGH);
         c>>=1;//shifts c to the right c = c>>1
         r>>=1;
         digitalWrite(sh_col, LOW);
    }
    digitalWrite(st_col, HIGH);
    digitalWrite(st_col, LOW);
    read_serial_command();
    delayMicroseconds(500);
  }
 }
}

void setup()
{   
    lcd.begin(16,2);
    pinMode(ds_col, OUTPUT);
    pinMode(sh_col, OUTPUT);
    pinMode(st_col, OUTPUT);
    pinMode(ds_row, OUTPUT);
    pinMode(start, INPUT);
    digitalWrite(start, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Welcomw ece_society");
    lcd.setCursor(0,1);
    lcd.print("  Snake game    ");
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("   Press Start  ");
    lcd.setCursor(0,1);
    lcd.print("     To Play    ");
    delay(2000);
    Serial.begin(9600);
}

void loop()
{ 
  int j,k,Speed=80,score=0;
  j=k=move_c=0;
  move_r=1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Score: ");
  lcd.print(score);
  while(1)
  {
   srand(0); 
   for(int i=3;i<21;i++)
   {
     Row[i]=100;
     Col[i]=100;
   }
   Row[0]=rand()%5+2;//food position
   Col[0]=rand()%5+2;
   Row[1]=1;//snake position
   Col[1]=1;
   Row[2]=2;
   Col[2]=1;
   j=2,k=1;
   while(k==1)//wait to start
   {
    move_c=0;
    move_r=1;
    show_snake(1);
    lcd.setCursor(7,0);
    lcd.print(score);
    if(!digitalRead(start))
    {
     k=2;
     score=0;
    }
   }
  
  while(k==2)//game with 2 sates: 1) hit to wall and end  2)bite the food and restart with new food location
  {
        show_snake(Speed);
        if(Row[1]>8 || Col[1]>8 || Row[1]<0 || Col[1]<0)
        {
         Row[1]=1;
         Col[1]=1;
         k=1;
         lcd.setCursor(0,1);
         lcd.print("Game Over");
         delay(5000);
         score=0;
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Score: ");
         lcd.print(score);
        }

        if(Row[0]==Row[1]+move_r  &&  Col[0]==Col[1]+move_c)
       {
         j++;
         score=score+5;
         lcd.setCursor(7,0);
         lcd.print(score);
         Row[0]=rand()%5+2;
         Col[0]=rand()%5+2;
  }

  for(int i=j;i>1;i--)
  {
   Col[i]=Col[i-1];
   Row[i]=Row[i-1];
  }
  Col[1]=Col[2]+move_c;
  Row[1]=Row[2]+move_r;
  }
  }
}
