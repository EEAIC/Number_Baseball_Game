#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>



#define TFT_DC 9
#define TFT_CS 10


int R;//라운드
int CN[4];//컴퓨터 임의의 숫자
int PN[4];//사람이 물어볼 숫자
int CQ[20][4];//컴퓨터가 물어볼 숫자 [Round][4]
int Cstrike,Cball;//컴퓨터가 답할 스트라이크와 볼
int L = 0;//임시 저장 변수 
int Pstrike[20],Pball[20];//사람이 답할 스트라이크와 볼
int Tmpstrike[20],Tmpball[20];//시뮬레이션에 필요한 스트라이크와 볼
int count=0;//재시작 횟수
int H;//home


char chArr[5] = { 0 };
void RandonComputerNumber();//컴퓨터의 임의의 숫자 생성
void CheckingCorrectQuestion();//상대가 규칙에 맞는 예측을 했는지 확인하는 부분
void CheckingCorrectAnswer();//상대가 규칙에 맞는 스트라이크 볼을 적었는지 확인하는 부분

void IntToString(int *intArr, char *chArr);// 인트 -> 캐릭터

void KeyPadTouchInput(void);// 키패드를 띄우고 터치값 받아오기
void PrintKeyPad(void);
int KeyPadInput(void);
int KeyPadInputStrikeBall(void);

Adafruit_GFX_Button buttons[4];
Adafruit_GFX_Button keyButtons[12];


#define STMPE_CS 8


#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define BUTTON_X 50
#define BUTTON_Y 150
#define BUTTON_W 50
#define BUTTON_H 20
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2

char buttonlabels[12][5] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "CLR", "HOME" };
uint16_t buttoncolors[12] =  {ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_ORANGE, ILI9341_ORANGE};                            
// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
   Serial.begin(115200);
   randomSeed(analogRead(0));
  if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);}
  
  tft.begin();
 
  tft.setRotation(1);
}

void loop() {
  START:
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(3);
  tft.println(" NUMBER BASEBALL");
  buttons[0].initButton(&tft, 160, 60, 250, 30, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, "Attack F", 2);
  buttons[1].initButton(&tft, 160, 120, 250, 30, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, "Dffence F", 2);
  buttons[2].initButton(&tft, 160, 180, 250, 30, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, "HELP", 2);

  buttons[0].drawButton();
  buttons[1].drawButton();
  buttons[2].drawButton();

  while(1){
   if (! ctp.touched()) {
      
    }
    else{
      break;
      }
  }
 
  
 R=1;//Round 시작 
  while(1){  
    // Retrieve a point  
    TS_Point p = ctp.getPoint();
    
    tft.setRotation(1);
    if (buttons[0].contains(p.y, p.x))      {
        tft.fillScreen(ILI9341_BLACK);
        
        
        RandonComputerNumber();
        Serial.print(CN[0]);
        Serial.print(CN[1]);
        Serial.print(CN[2]);
        Serial.println(CN[3]);
        while(1){
        CheckingCorrectQuestion();
                if (Cstrike==4)
        {
            tft.print("YOU ARE WIN!\n");
            while(1){
             if (! ctp.touched()) {
      
              }
            else{
              break;
              }
             }
            goto START;
        }
        else
        {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(0, 0);
            tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
            tft.print("PREDICT MY NUMBER");
            tft.setCursor(0, 70);
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
            tft.print(Cstrike);
            tft.println("strikes     ");
            tft.print(Cball);
            tft.println("balls");
            Serial.print(Cstrike);
            Serial.print("strikes   ");
            Serial.print(Cball);
            Serial.print("balls");
             while(1)
              {
      
                 TS_Point p = ctp.getPoint();
                 if (! ctp.touched()) 
                 {
              
                  }
                  else 
                 {
                  break;
                 }
              }
            
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(0, 0);
            tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
            tft.println("MY TURN");
            delay(1000);
            ComputerQuestion();//컴퓨터가 물어볼 숫자
            tft.setCursor(0, 70);
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
            tft.print(CQ[R][0]);
            tft.print(CQ[R][1]);
            tft.print(CQ[R][2]);
            tft.println(CQ[R][3]);
            while(1)
            {
    
             TS_Point p = ctp.getPoint();
             if (! ctp.touched()) 
             {
          
              }
              else 
              {
              break;
              }
            }
           }
        CheckingCorrectAnswer();
        if (Pstrike[R]==4)
        {
            int re;//재시작 변수
            tft.println("YOU LOSE!");
            while(1)
            {
             if (! ctp.touched()) {
      
              }
            else{
              break;
              }
             }
            goto START;
            /*for (int t=0;t<=10;t++)
             {
             printf("%d\n",10-t);
             delay(1000000);
             }*///카운트 다운
            tft.print(CN[0]);
            tft.print(CN[1]);
            tft.print(CN[2]);
            tft.print(CN[3]);
            while(1);
            
            //printf("다시하시려면 1을 입력하세요!\n");
            //scanf("%d",&re);
           /* if (re==1)
            {
                count+=1;
                printf("Restart(%d)\n",count);
                break;
            }
            else
            {
                printf("GAME OVER\n");
                exit(0);
            }*/

        }
        ++R;

        delay(1000);
        tft.println(" Check!");
        }
      
      }
    if  (buttons[1].contains(p.y, p.x))
       {
        RandonComputerNumber();
        Serial.print(CN[0]);
        Serial.print(CN[1]);
        Serial.print(CN[2]);
        Serial.println(CN[3]);
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
        tft.println("it's my turn!");
        ComputerQuestion();//컴퓨터가 물어볼 숫자
        tft.setCursor(0, 70);
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
        tft.print(CQ[R][0]);
        tft.print(CQ[R][1]);
        tft.print(CQ[R][2]);
        tft.println(CQ[R][3]);
        while(1)
            {
    
             TS_Point p = ctp.getPoint();
             if (! ctp.touched()) {
          
              }
              else 
            {
              break;
            }
            }
            CheckingCorrectAnswer();
        if (Pstrike[R]==4)
        {
            int re;//재시작 변수
            tft.println("YOU LOSE!");
            while(1)
            {
             if (! ctp.touched()) {
      
              }
            else{
              break;
              }
             }
            goto START;
            /*for (int t=0;t<=10;t++)
             {
             printf("%d\n",10-t);
             delay(1000000);
             }*///카운트 다운
            tft.print(CN[0]);
            tft.print(CN[1]);
            tft.print(CN[2]);
            tft.print(CN[3]);
            while(1);
            
            //printf("다시하시려면 1을 입력하세요!\n");
            //scanf("%d",&re);
           /* if (re==1)
            {
                count+=1;
                printf("Restart(%d)\n",count);
                break;
            }
            else
            {
                printf("GAME OVER\n");
                exit(0);
            }*/

        }
        R++;

        delay(1000);
        tft.println(" Check!");
        
                while(1){
        CheckingCorrectQuestion();
                if (Cstrike==4)
        {
            tft.print("YOU WIN!\n");
            while(1){
             if (! ctp.touched()) {
      
              }
            else{
              break;
              }
             }
            goto START;
        }
        else
        {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(0, 0);
            tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
            tft.print("PREDICT MY NUMBER");
            tft.setCursor(0, 70);
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
            tft.print(Cstrike);
            tft.println("strikes     ");
            tft.print(Cball);
            tft.println("balls");
            Serial.print(Cstrike);
            Serial.print("strikes   ");
            Serial.print(Cball);
            Serial.print("balls");
             while(1)
              {
      
                 TS_Point p = ctp.getPoint();
                 if (! ctp.touched()) 
                 {
              
                  }
                  else 
                 {
                  break;
                 }
              }
            
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(0, 0);
            tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
            tft.println("MY TURN");
            delay(1000);
            ComputerQuestion();//컴퓨터가 물어볼 숫자
            tft.setCursor(0, 70);
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
            tft.print(CQ[R][0]);
            tft.print(CQ[R][1]);
            tft.print(CQ[R][2]);
            tft.println(CQ[R][3]);
            while(1)
            {
    
             TS_Point p = ctp.getPoint();
             if (! ctp.touched()) 
             {
          
              }
              else 
              {
              break;
              }
            }
           }
        CheckingCorrectAnswer();
        if (Pstrike[R]==4)
        {
            int re;//재시작 변수
            tft.println("YOU LOSE!");
            while(1)
            {
             if (! ctp.touched()) {
      
              }
            else{
            
              break;
              }
             }
             
            tft.fillScreen(ILI9341_BLACK);
            tft.print(CN[0]);
            tft.print(CN[1]);
            tft.print(CN[2]);
            tft.print(CN[3]);
            while(1)
            {
             if (! ctp.touched()) {
      
              }
            else{
            
              break;
              }
             }
            goto START;
            /*for (int t=0;t<=10;t++)
             {
             printf("%d\n",10-t);
             delay(1000000);
             }*///카운트 다운
            tft.print(CN[0]);
            tft.print(CN[1]);
            tft.print(CN[2]);
            tft.print(CN[3]);
            while(1);
            
            //printf("다시하시려면 1을 입력하세요!\n");
            //scanf("%d",&re);
           /* if (re==1)
            {
                count+=1;
                printf("Restart(%d)\n",count);
                break;
            }
            else
            {
                printf("GAME OVER\n");
                exit(0);
            }*/

        }
        R++;

        delay(1000);
        tft.println(" Check!");
        }
      
        break;
      }
     if (buttons[2].contains(p.y, p.x )){
      
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(3);
        tft.println("       Rule");
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        delay(1000);
        tft.println("This game is Played by 4 digits.");
        delay(1000);
        tft.println("");
        tft.println("Each digit isn't be same.");
        delay(1000);
        tft.println("");
        tft.println("If Each digit`s location and number are same, it's 1 strike.");
        delay(1000);
        tft.println("");
        tft.println("If Each digit`s number is same and location isn`t same, it`s 1 ball.");
        delay(1000);
        while(1)
            {
    
             TS_Point p = ctp.getPoint();
             if (! ctp.touched()) 
             {
          
              }
              else 
              {
              break;
              }
            }
        break;
     }
     
  }
}

void RandonComputerNumber()//컴퓨터가 가질 임의의 숫자 생성
{
    int i=0;
    int tmp=0;
    CN[0]=-1;
    CN[1]=-2;
    CN[2]=-3;
    CN[3]=-4;
    while(i<=3)
    {
        tmp=random(10);
        if(CN[0]!=tmp && CN[1]!=tmp && CN[2]!=tmp && CN[3]!=tmp)
        {
            CN[i++]=tmp;
        }
    }
    
}

void IntToString(int *intArr, char * chArr)
{
  //precondition : 인트배열값, 캐릭터배열값 존재
  //postcondition : 인트 배열안의 숫자를 문자열의 숫자로 변환해서 캐릭터 배열에 저장
  //return : 변환된 문자 배열
 
  for(int index = 0; index < 4; index++)
  {
    switch(intArr[index])
    {
      case 0:
      chArr[index] = '0';
      break;
      case 1:
      chArr[index] = '1';
      break;
      case 2:
      chArr[index] = '2';
      break;
      case 3:
      chArr[index] = '3';
      break;
      case 4:
      chArr[index] = '4';
      break;
      case 5:
      chArr[index] = '5';
      break;
      case 6:
      chArr[index] = '6';
      break;
      case 7:
      chArr[index] = '7';
      break;
      case 8:
      chArr[index] = '8';
      break;
      case 9:
      chArr[index] = '9';
      break;
    }
  }
  
}

void KeyPadTouchInput(void)
{
  //precondition : none
  //postcondition : 키패드 화면에 출력, 터치된 키패드값 받아오기
  //return : 입력받은 키패드값


  //키패드출력함수();
  KeyPadInput();
  //return 키패드값 받는 함수();

}

void PrintKeyPad(void)
{
  //precondition : none
  //postcondition : 키패드 크기 셋팅, 키패드 위치 셋팅, 키 이름 셋팅
  //return : none
  // 150 < y < 220
  // 130 < x < 380
  // height : 20, space_y : 2
  // width : 50, space_x : 10
  tft.fillScreen(ILI9341_BLACK);
  for (uint8_t row=0; row<3; row++) {
    for (uint8_t col=0; col<4; col++) {
      keyButtons[col + row*4].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, ILI9341_WHITE, buttoncolors[col+row*4], ILI9341_WHITE,
                  buttonlabels[col + row*4], BUTTON_TEXTSIZE); 
      keyButtons[col + row*4].drawButton();
    }
  }
}

int KeyPadInput(void)
{
  //precondition : none
  //postcontidion : 터치 입력 받을 때 까지 대기, 터치 입력 받은 버튼 위치의 숫자 확인
  //return : 확인된 숫자 리
CLR:
  tft.fillScreen(ILI9341_BLACK); 
  PrintKeyPad(); 
  int CNT=0;
  L=0;     
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
  tft.print("PREDICT MY NUMBER");
  tft.setCursor(30, 70);
  while(1){
    
    TS_Point p = ctp.getPoint();
   if (! ctp.touched()) {

    }
    else if (keyButtons[3].contains(p.y, p.x))
     {
        Serial.write('1');

        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("1");
        L=(L*10)+1;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
        
      
        
     }
     else if (keyButtons[2].contains(p.y, p.x))
     {
        Serial.write('2');
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("2");
        L=(L*10)+2;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[1].contains(p.y, p.x))
     {
        Serial.write('3');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("3");
        L=(L*10)+3;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[0].contains(p.y, p.x))
     {
        Serial.write('4');
       
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("4");
        L=(L*10)+4;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     } 
     else if (keyButtons[5].contains(p.y, p.x))
     {
        Serial.write('7');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("7");
        L=(L*10)+7;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[6].contains(p.y, p.x))
     {
        Serial.write('6');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("6");
        L=(L*10)+6;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }  
     else if (keyButtons[4].contains(p.y, p.x))
     {
        Serial.write('8');
      
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("8");
        L=(L*10)+8;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[7].contains(p.y, p.x))
     {
        Serial.write('5');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("5");
        L=(L*10)+5;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
        
     }
     else if (keyButtons[8].contains(p.y, p.x))
     {
        Serial.write('home');
        
        
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }
          else if (keyButtons[9].contains(p.y, p.x))
     {
        Serial.write('HOME');
        goto CLR;
        
  
     }     else if (keyButtons[10].contains(p.y, p.x))
     {
        Serial.write('0');

        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("0");
        L=(L*10)+0;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }
          else if (keyButtons[11].contains(p.y, p.x))
     {
        Serial.write('9');

        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("9");
        L=(L*10)+9;
        CNT+=1;
        if (CNT==4)
        {
         break; 
        }
        delay(300);
     }                
  
  }
  delay(1000);
     
}

int KeyPadInputStrikeBall(void)
{
  //precondition : none
  //postcontidion : 터치 입력 받을 때 까지 대기, 터치 입력 받은 버튼 위치의 숫자 확인
  //return : 확인된 숫자 리
  CLR:
  tft.fillScreen(ILI9341_BLACK); 
  PrintKeyPad(); 
  L=0;
  int CNT=0;
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
  tft.print("Write Strike and Ball");
  tft.setCursor(30, 70);
  while(1){
    
    TS_Point p = ctp.getPoint();
   if (! ctp.touched()) {

    }
    else if (keyButtons[3].contains(p.y, p.x))
     {
        Serial.write('1');

        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("1");
        L=(L*10)+1;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
        
      
        
     }
     else if (keyButtons[2].contains(p.y, p.x))
     {
        Serial.write('2');
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("2");
        L=(L*10)+2;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[1].contains(p.y, p.x))
     {
        Serial.write('3');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("3");
        L=(L*10)+3;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[0].contains(p.y, p.x))
     {
        Serial.write('4');
       
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("4");
        L=(L*10)+4;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     } 
     else if (keyButtons[5].contains(p.y, p.x))
     {
        Serial.write('7');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("7");
        L=(L*10)+7;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[6].contains(p.y, p.x))
     {
        Serial.write('6');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("6");
        L=(L*10)+6;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }  
     else if (keyButtons[4].contains(p.y, p.x))
     {
        Serial.write('8');
      
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("8");
        L=(L*10)+8;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }
     else if (keyButtons[7].contains(p.y, p.x))
     {
        Serial.write('5');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("5");
        L=(L*10)+5;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
        
     }
     else if (keyButtons[8].contains(p.y, p.x))
     {
        Serial.write('enter');
        
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("");
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }
          else if (keyButtons[9].contains(p.y, p.x))
     {
        Serial.write('CLR');
        goto CLR;
        
  
     }     else if (keyButtons[10].contains(p.y, p.x))
     {
        Serial.write('0');

        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("0");
        L=(L*10)+0;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }
          else if (keyButtons[11].contains(p.y, p.x))
     {
        Serial.write('9');

        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
        tft.print("9");
        L=(L*10)+9;
        CNT+=1;
        if (CNT==2)
        {
         break; 
        }
        delay(300);
     }                
  
  }
  delay(1000);   
}

void CheckingCorrectQuestion()//상대가 규칙에 맞는 예측을 했는지 확인하는 부분
{
    Cstrike=0;
    Cball=0;
QuestionA:
    
    KeyPadTouchInput();
    for (int i=3;i>=0;i--){
      PN[i]=L%10;
      L/=10;
      }
    Serial.print(PN[0]);
    Serial.print(PN[1]);
    Serial.println(PN[2]);
    Serial.println(PN[3]);
    for (int a=0; a<4; a++)
    {
        for (int b=a+1;b<4;b++ )
        {
            if (PN[a]==PN[b])
            {
                tft.fillScreen(ILI9341_BLACK);
                tft.setCursor(0, 0);
                tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
                tft.print("PREDICT MY NUMBER");
                tft.setCursor(30, 70);
                tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
                tft.print("Same Numbers are Existed");
                delay(1000);
                tft.fillScreen(ILI9341_BLACK);
                goto QuestionA;
            }
        }
    }
    for (int h=0;h<=3;h++)
    {
        for (int k=0;k<=3;k++)
        {
            if(PN[h]==CN[k]&&k==h)
            {
                Cstrike+=1;
            }
            else if(PN[h]==CN[k]&&k!=h)
            {
                Cball+=1;
            }
        }
    }
}    

void ComputerQuestion()//컴퓨터가 물어보는 부분
{
    int TR;

    if (R==1)
    {
        
        int i=0;
        int tmp=0;
                CQ[1][0]=-1;
        CQ[1][1]=-2;
        CQ[1][2]=-3;
        CQ[1][3]=-4;
        while(i<=3)
        {
            tmp=random(10);
            if(CQ[1][0]!=tmp && CQ[1][1]!=tmp && CQ[1][2]!=tmp && CQ[1][3]!=tmp)
            {
                CQ[1][i++]=tmp;
            }
        }
    }
    else
    {
        int TempC;
        do
        {
            int i=0;
            int tmp=0;
            CQ[R][0]=-1;
            CQ[R][1]=-2;
            CQ[R][2]=-3;
            CQ[R][3]=-4;
            for(int TR=1;TR<=R;TR++)
            {
                Tmpstrike[TR]=0;
                Tmpball[TR]=0;
            }

            while(i<=3)
            {
                tmp=random(10);
                if(CQ[R][0]!=tmp && CQ[R][1]!=tmp && CQ[R][2]!=tmp && CQ[R][3]!=tmp)
                {
                    CQ[R][i++]=tmp;
                }
            }
            for (TR=1;TR<=R;TR++)//int TR 임시로 비교하기 위한 라운드
            {
                for (int h=0;h<=3;h++)
                {
                    for (int k=0;k<=3;k++)
                    {
                        if(CQ[TR][h]==CQ[R][k]&&k==h)
                        {
                            Tmpstrike[TR]+=1;
                        }
                        else if(CQ[TR][h]==CQ[R][k]&&k!=h)
                        {
                            Tmpball[TR]+=1;
                        }
                    }
                }

            }
            TempC=1;
            for (TR=1;TR<=R;TR++)
            {
                if((Tmpstrike[TR]==Pstrike[TR])&&(Tmpball[TR]==Pball[TR]))
                {
                    TempC+=1;
                }
            }
            
            
        
        
        }
        while(TempC!=R);
    }
    
}

void CheckingCorrectAnswer()//상대가 규칙에 맞는 스트라이크 볼을 적었는지 확인하는 부분
{
while(1){
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    Pstrike[R]=0;
    Pball[R]=0;
    
   
    KeyPadInputStrikeBall();

    Pstrike[R]=L/10;
    Pball[R]=L%10;
    
    if (Pstrike[R]>4||Pball[R]>4||(Pstrike[R]+Pball[R])>4)
    {
      
        tft.setCursor(20, 100);
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.print("Write Correct Answer");
        delay(1000);
    }
    else
    {
        break;
    }
}
    
}



