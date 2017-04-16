#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef char int8;
typedef int  int16;
int8 code musicArray[]={};
int16 code soundFrequ[]={262,};
int16 code tmr0reload[]={
		65536 -(12000000\12)\(262*2),
		65536 -(12000000\12)\(262*2),
		65536 -(12000000\12)\(262*2),
		65536 -(12000000\12)\(262*2),
		65536 -(12000000\12)\(262*2),
		65536 -(12000000\12)\(262*2),
		65536 -(12000000\12)\(262*2)
};
bit tmrFlag = 0;
bit enable  = 0;
uint8 T0RH,T0RL;
sbit beep P2^6;          // define fengmingqi io
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
	EA = 1;
	ET0 = 1;
}


void playMusic()
{
	uint8 musicCon,j;
	uint16 time,soundTime,beatTime;
for(musicCon=0;musicCon<(sizeof(musicArray[])>>1);)
  {
	while(!tmrFlag);
	tmrFlag = 0;
	if(time==beatTime)
	{
		musicCon++;
		T0RH =tmr0reload[musicArray[(musicCon-1)]]>>8;
		T0RL =tmr0reload[musicArray[(musicCon-1)]];
		beatTime  = soundFrequ[musicArray[(musicCon-1)]]*2*musicArray[musicCon];
		soundTime = (beatTime>>3)*3;
		enable = 1;
	}
	else if(time==soundTime)
	{
			enable=0;
  }
}


void delayms(int16 ms)
{
	uint8 i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<128;j++);
}

void tmr0interrupt() interrupt 0
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