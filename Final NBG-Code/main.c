//
//  main.c
//  Final NBG-Code
//
//  Created by CMD on 2016. 5. 1..
//  Copyright © 2016년 CMD. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void PreemptiveAttack();//선공 파트
void LatterOffense();//후공 파트
void Help();//도움말 파트
void RandonComputerNumber();//컴퓨터의 임의의 숫자 생성
void CheckingCorrectQuestion();//상대가 규칙에 맞는 예측을 했는지 확인하는 부분
void CheckingCorrectAnswer();//상대가 규칙에 맞는 스트라이크 볼을 적었는지 확인하는 부분
void ComputerQuestion();//컴퓨터가 물어보는 부분

int CN[4];//컴퓨터 임의의 숫자
int PN[4];//사람이 물어볼 숫자
int CQ[20][4];//컴퓨터가 물어볼 숫자 [Round][4]
int R;//라운드
int Cstrike,Cball;//컴퓨터가 답할 스트라이크와 볼
int Pstrike[20],Pball[20];//사람이 답할 스트라이크와 볼
int Tmpstrike[20],Tmpball[20];//시뮬레이션에 필요한 스트라이크와 볼
int count=0;//재시작 횟수

int main()
{
    printf("============숫자야구============\n");
    int Selectmode;//모드: 선공||후공||도움말

    while (1)
    {
        printf("선공=1 후공=2 도움말=0 을 입력하세요!\n");
        scanf("%d",&Selectmode);
        if (Selectmode==1)
        {
            PreemptiveAttack();
        }
        else if (Selectmode==2)
        {
            LatterOffense();
        }
        else if (Selectmode==0)
        {
            Help();
        }
        else
        {
            printf("정해진 수 범위 내에서 입력해주세요.\n");
        }
    }
    return 0;
}

void PreemptiveAttack()//상대방 선공
{
    R=1;//Round 시작
    RandonComputerNumber();
    while(1)
    {
    CheckingCorrectQuestion();
        if (Cstrike==4)
        {
            printf("당신이 이겼습니다!\n");
            break;
        }
        else
        {
            printf("%dstrike %dball\n",Cstrike,Cball);
            printf("저의 차례입니다.\n");
            ComputerQuestion();//컴퓨터가 물어볼 숫자
            printf("%d%d%d%d\n",CQ[R][0],CQ[R][1],CQ[R][2],CQ[R][3]);
        }
    CheckingCorrectAnswer();
        if (Pstrike[R]==4)
        {
            int re;//재시작 변수
            printf("YOU LOSE!\n");
            printf("RE?\n");
            /*for (int t=0;t<=10;t++)
             {
             printf("%d\n",10-t);
             delay(1000000);
             }*///카운트 다운
            printf("저의 숫자는 %1d%1d%1d%d입니다!\n",CN[0],CN[1],CN[2],CN[3]);
            printf("다시하시려면 1을 입력하세요!\n");
            scanf("%d",&re);
            if (re==1)
            {
                count+=1;
                printf("Restart(%d)\n",count);
                break;
            }
            else
            {
                printf("GAME OVER\n");
                exit(0);
            }

        }
        R++;
        
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
    srand((unsigned int)time(NULL));
    while(i<=3)
    {
        tmp=rand()%10;
        if(CN[0]!=tmp && CN[1]!=tmp && CN[2]!=tmp && CN[3]!=tmp)
        {
            CN[i++]=tmp;
        }
    }
}
void CheckingCorrectQuestion()//상대가 규칙에 맞는 예측을 했는지 확인하는 부분

{
    Cstrike=0;
    Cball=0;
QuestionA:
    printf("상대의 수를 예측해 보세요!\n");
    scanf("%1d%1d%1d%1d",&PN[0],&PN[1],&PN[2],&PN[3]);
    for (int a=0; a<4; a++)
    {
        for (int b=a+1;b<4;b++ )
        {
            if (PN[a]==PN[b])
            {
                printf("같은 숫자가 존재합니다! 다시 입력해 주세요!\n");
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

void CheckingCorrectAnswer()//상대가 규칙에 맞는 스트라이크 볼을 적었는지 확인하는 부분
{
while(1){
    printf("스트라이크 볼의 개수를 적어주십시오!\n");
    Pstrike[R]=0;
    Pball[R]=0;
    scanf("%d %d",&Pstrike[R],&Pball[R]);
    if (Pstrike[R]>4||Pball[R]>4)
    {
        printf("정확한 값을 적어주십시오.\n");
    }
    else
    {
        break;
    }
}
    
}


void ComputerQuestion()//컴퓨터가 물어보는 부분
{
    int TR;

    if (R==1)
    {
        srand((unsigned int)time(NULL));
        int i=0;
        int tmp=0;
                CQ[1][0]=-1;
        CQ[1][1]=-2;
        CQ[1][2]=-3;
        CQ[1][3]=-4;
        while(i<=3)
        {
            tmp=rand()%10;
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
                tmp=rand()%10;
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

void  LatterOffense()//상대방 후공
{
    R=1;//Round 시작
    printf("저의 차례입니다!\n");
    RandonComputerNumber();
    while(1)
    {
    ComputerQuestion();//컴퓨터가 물어볼 숫자
    printf("%d%d%d%d\n",CQ[R][0],CQ[R][1],CQ[R][2],CQ[R][3]);
    CheckingCorrectAnswer();
    if (Pstrike[R]==4)
    {
        int re;//재시작 변수
        printf("YOU LOSE!\n");
        printf("RE?\n");
        /*for (int t=0;t<=10;t++)
         {
         printf("%d\n",10-t);
         delay(1000000);
         }*///카운트 다운
        printf("저의 숫자는 %1d%1d%1d%d입니다!\n",CN[0],CN[1],CN[2],CN[3]);
        printf("다시하시려면 1을 입력하세요!\n");
        scanf("%d",&re);
        if (re==1)
        {
            count+=1;
            printf("Restart(%d)\n",count);
            break;
        }
        else
        {
            printf("GAME OVER\n");
            exit(0);
        }
        
    }
    R++;
        CheckingCorrectQuestion();
        if (Cstrike==4)
        {
            printf("당신이 이겼습니다!\n");
            break;
        }
        else
        {
            printf("%dstrike %dball\n",Cstrike,Cball);
            printf("저의 차례입니다.\n");
            
        }
    
    }

}

void Help()//도움말 부분
{
   int ST ;//아무키나 입력부분
    printf("                 <숫자야구 게임 도움말>\n");
    printf("RULE 1. 이 숫자야구 게임은 4자리의 숫자로 하는 게임입니다.\n");
    printf("RULE 2. 각자의 숫자는 모두 달라야합니다.\n");
    printf("RULE 3. 각자리의 위치와 숫자가 같으면 1strike 입니다.\n");
    printf("        ex)1234 1289 의 경우 2strike\n");
    printf("RULE 4. 각자리의 위치는 다르지만 숫자가 같으면 1ball 입니다.\n");
    printf("        ex) 1234 8912 의 경우 2ball\n");
    printf("          이제 게임을 시작하려면 아무키나 입력하십시오!\n");
    scanf("%d\n",&ST);
}
