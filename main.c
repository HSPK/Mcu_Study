#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef char int8;
typedef int  int16;
/*uint8 code musicArray[]={0,1,0,1,5,1,	1,3,	1,2,2,1,3,1,	5,1,3,1,3,1,	3,2,5,1,	1,3,	2,1,1,1,6,1,
						 5,3,	5,2,5,1,	1,3,	1,2,2,1,3,1,	6,1,3,1,5,1,	5,1,3,1,2,1,	1,3,	2,2,5,1,
						 3,1,2,2,	2,2,3,1,	5,3,	5,1,3,1,5,1,	6,1,1,1,6,1,	6,2,3,1,	2,2,1,1,	2,2,3,1,
						 3,3,	3,2,5,1,	5,3,	0,1,3,1,3,1,	2,1,1,1,1,1,	1,2,5,1,	2,2,1,1,	3,1,2,1,1,3,0,1}; 	*/					  //δ����
/*uint8 code musicArray[]={1,2,2,2,	3,2,1,2,	1,2,2,2,	3,2,1,2,	3,2,4,2,5,2,0,2,	3,2,4,2,5,2,0,2,
						 5,1,6,1,5,1,4,1,3,2,1,2,	5,1,6,1,5,1,4,1,3,2,1,2,	2,2,8,2,1,2,0,2,	2,2,8,2,1,2,0,2};    //�ֻ�ϻ�			 */

uint8 code musicArray[]={
						0,1,0,1,5,1,	1,3,	1,2,2,1,3,1,	5,1,3,1,3,1,	3,2,5,1,	1,3,	2,1,1,1,6,1,
						 5,3,	5,2,5,1,	1,3,	1,2,2,1,3,1,	6,1,3,1,5,1,	5,1,3,1,2,1,	1,3,	2,2,5,1,
						 3,1,2,2,	2,2,3,1,	5,3,	5,1,3,1,5,1,	6,1,1,1,6,1,	6,2,3,1,	2,2,1,1,	2,2,3,1,
						 3,3,	3,2,5,1,	5,3,	0,1,3,1,3,1,	2,1,1,1,1,1,	1,2,5,1,	2,2,1,1,	3,1,2,1,1,3,0,9,
						 
						 
						 1,2,2,2,	3,2,1,2,	1,2,2,2,	3,2,1,2,	3,2,4,2,5,2,0,2,	3,2,4,2,5,2,0,2,
						 5,1,6,1,5,1,4,1,3,2,1,2,	5,1,6,1,5,1,4,1,3,2,1,2,	2,2,8,2,1,2,0,2,	2,2,8,2,1,2,0,9,


1,1,1,1,5,1,5,1,	6,1,6,1,5,2,	4,1,4,1,3,1,3,1,	2,1,2,1,1,2,	5,1,5,1,4,1,4,1,	3,1,3,1,2,2,5,1,5,1,4,1,4,1,	3,1,3,1,2,2,	1,1,1,1,5,1,5,1,	6,1,6,1,5,2,	4,1,4,1,3,1,3,1,	2,1,2,1,1,2	};
uint16 code soundFrequ[]={000,1047,1175,1319,1397,1568,1760,1976,784};
uint16 code tmr0reload[]={
      	65535,
		65536 -(12000000/12)/(1047*2),
		65536 -(12000000/12)/(1175*2),
		65536 -(12000000/12)/(1319*2),
		65536 -(12000000/12)/(1397*2),
		65536 -(12000000/12)/(1568*2),
		65536 -(12000000/12)/(1760*2),
		65536 -(12000000/12)/(1976*2),
		65536 -(12000000/12)/(784*2)
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
		
		musicScale = (musicArray[musicCon]%100);
		musicLong = (musicArray[musicCon]/100)%10);
		musicLine = (musicArray[musicCon]%1000);
		
		if (!musicScale)
		{
			enable = 0;
			T0RH = 0XD8;
			T0RL = 0XF0;
			beatTime = 25*musicLong;
			enable = 0;
		}
		else
		{

		T0RH =tmr0reload[musicScale]>>8;
		T0RL =tmr0reload[musicScale];
		beatTime  = soundFrequ[musicScale]*musicLong;
		beatTime = beatTime>>1;
		if(musicLine)
		{
			soundTime = beatTime; 
		}
		else
		{
			soundTime = ((beatTime>>3)*7);
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