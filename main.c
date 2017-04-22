#include <reg52.h>
#define mainScale 5    //c scale
     //music speed
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef char int8;
typedef int  int16;
 /*
uint16 code musicArray[]={150,151,152,153,151,	151,152,153,151,	153,154,255,	153,154,255,	155,156,155,154,
						  253,251,		155,156,155,154,	253,251,	252,245,251,250,	252,245,251,150,150
}; 			*/							 
    //   lianzyin long diao  scale
uint16 code musicArray[]={350,161,162,	263,163,163,163,	263,163,162,	163,162,261,	150,153,153,155,	361,161,	261,161,157,	261,157,156,	150,156,153,155,	
						  356,156,	256,255,156,155,	454,151,152,	153,155,155,156,	355,153,	152,155,156,1155,	255,161,162,	363,163,	263,262,	163,162,261,	
						  150,153,153,155,	361,161,	261,257,	161,157,256,	150,156,153,155,
						  356,156,	256,255,	156,155,	454,	151,152,	153,155,155,153,	252,252,451,450,		///////1
						  253,152,1153,		253,152,151,		1155,155,156,1155,	255,153,155,
 						  156,156,156,156,		256,153,1155,	455,350,153,	156,156,156,155,	256,156,161,	
						  155,155,155,156,		353,151,	152,152,152,152,	252,152,151,	1152,253,1152,452,
						  152,152,152,152,		252,152,151,	1152,253,1151,	351,151,	161,161,161,161,	1261,162,161,	156,255,156,	353,151,
						  352,151,	252,1152,153,	452,	350,151,	161,161,161,161,	1161,262,161,	156,255,156,
						  361,155,	356,155,	252,261,	662,	    //shuochukou
						  161,162,	163,162,161,1162,	263,162,161,	162,163,162,161,	150,153,153,155,
						  156,155,156,161,	255,1155,156,	453,	150,153,152,153,	255,156,1155,	155,155,153,155,	256,161,1156,	156,1156,155,156,
						  361,1161,	256,256,	662,	161,162,      //1
						  363,1162,262,256,		461,450,
						  363,1162,	262,256,	461,	450,	363,1162,	462,	450,456,
						  461,450,450,450	
						  	};	  
uint16 code soundFrequ[7][7]={
  // C     D   E     F   G   A   B
33,37,41,46,49,55,61,            //C1  0
65,73,82,92,98,110,123,      //C     1
130,146,164,185,196,220,246,      //c      2
262,294,330,370,392,440,494,         //c1    3		  d
523,587,659,740,784,880,987,        //   c2     4	 e
1047,1175,1319,1480,1568,1760,1976,     //c3    5	   f
2093,2349,2637,2960,3136,3520,3951        //c4    6		g
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
	uint8 musicSpeed=10;
	uint16 time=0,soundTime=0,beatTime=0;
	uint8 musicScale;
	uint8 musicDegree;
	uint8 musicLong;
	bit musicLine;
	for(musicCon=0;musicCon<(sizeof(musicArray)>>1);)
  {
	while(!tmrFlag);
   		tmrFlag = 0;
	if(time==beatTime)
	{
		time = 0;
	
		musicScale = (musicArray[musicCon]%10);
		musicDegree = (musicArray[musicCon]/10%10);
		musicLong = (musicArray[musicCon]/100)%10;
		musicLine = (musicArray[musicCon]/1000);
		musicDegree = mainScale + musicDegree - 5;
		if (!musicScale)
		{
			enable = 0;
			T0RH = 0XD8;
			T0RL = 0XF0;
			beatTime = ((25*musicLong)/10)*musicSpeed;     //weizhi
			enable = 0;
		}
		else
		{
		T0RH =tmr0reload[musicDegree][musicScale-1]>>8;
		T0RL =tmr0reload[musicDegree][musicScale-1];
		beatTime  = (soundFrequ[musicDegree][musicScale-1]*musicLong)>>1;
		beatTime = (beatTime/10)*musicSpeed;
		soundTime = ((beatTime>>2)*3);
		enable = 1;
		}
		musicCon++;
	}
	else if(time==soundTime)
	{
		enable=musicLine;
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