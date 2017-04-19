#include <reg52.h>
#define mainScale 3    //c scale
#define musicSpeed 2     //music speed
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef char int8;
typedef int  int16;

uint16 code musicArray[]={
};     //   x是否连音x音长x音调x音阶
uint16 code soundFrequ[7][7]={
     // C   C#   D   D#    E     E#   F   G   G#   A   A#    B
33,37,41,46,49,55,61,            //C1  0
65,73,82,92,98,110,123,      //C     1
130,146,164,185,196,220,246,      //c      2
262,294,330,370,392,440,494,         //c1    3
523,587,659,740,784,880,987,        //   c2     4
1047,1175,1319,1480,1568,1760,1976,     //c3    5
2093,2349,2637,2960,3136,3520,3951        //c4    6
};
uint16 code tmr0reload[7][7]={50385,52023,53341,54667,55332,56446,57340,
57844,58687,59439,60102,60434,60991,61471,
61690,62112,62488,62834,62985,63264,63504,
63628,63836,64021,64185,64261,64400,64524,
64580,64685,64778,64861,64899,64968,65030,
65059,65111,65157,65199,65218,65252,65283,
65298,65324,65347,65368,65377,65394,65410
};
bit tmrFlag = 0;
bit enable  = 0;
uint8 T0RH=0xff,T0RL=0xfe;
sbit beep=P1^5;          // define fengmingqi io
void delayms(uint16 ms);
void playMusic();
void Init();

void main()
{
	Init();
	while(1)
	{
		playMusic();
		delayms(1000);
	}
}

void Init()
{
	TMOD = 0X01;      //tmr0 init 
	TH0 = T0RH;
	TL0 = T0RL;
	TR0=1;
	EA = 1;
	ET0 = 1;
}


void playMusic()
{
	uint16 musicCon=0;
	uint16 time=0,soundTime=0,beatTime=0;
	uint8 musicScale;
	uint8 musicDegree;
	uint8 musicLong;
	bit musicLine;
	for(musicCon=0;musicCon<sizeof(musicArray);)
  {
	while(!tmrFlag);
	tmrFlag = 0;
	if(time==beatTime)
	{
		musicCon++;
		time = 0;
		
		musicScale = (musicArray[musicCon]%10)-1;
		musicDegree = (musicArray[musicCon]/10%10);
		musicLong = (musicArray[musicCon]/100)%10);
		musicLine = (musicArray[musicCon]%1000);
		musicDegree = mainScale + musicDegree - 5;
		
		if (!musicScale)
		{
			enable = 0;
			T0RH = 0XD8;
			T0RL = 0XF0;
			beatTime = (50*musicLong)/musicSpeed;
			enable = 0;
		}
		else
		{
		T0RH =tmr0reload[musicDegree][musicScale]>>8;
		T0RL =tmr0reload[musicDegree][musicScale];
		beatTime  = soundFrequ[musicDegree][musicScale]*musicLong;
		beatTime = beatTime/musicSpeed;
		if(musicLine)
		{
			soundTime = beatTime; 
		}
		else
		{
			soundTime = ((beatTime>>2)*3);
		}
		enable = 1;
		}
	}
	else if(time==soundTime)
	{
		enable=0;
	}
	time++;
  }
  enable=0;
}


void delayms(uint16 ms)
{
	uint16 i;
	uint8 j;
	for(i=0;i<ms;i++)
		for(j=0;j<128;j++);
}

void tmr0interrupt() interrupt 1
{
	tmrFlag = 1;
	TH0=T0RH;
	TL0=T0RL;
	if(enable)
	{
		beep=~beep;
	}
	else
	{
		beep = 0;
	}
}