#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <systemservice.h>
#include <orbis2d.h>
#include <orbisPad.h>
#include <orbisKeyboard.h>
#include <orbisAudio.h>
#include <modplayer.h>
#include <ps4link.h>
#include <debugnet.h>


int x=1280/2;
int y=720/2;
int w=1280/64;
int h=1280/64;
int step=10;


int64_t flipArg=0;
int R,G,B;
uint32_t color=0x80ff0000;
int flag=0;

Orbis2dConfig *conf;
OrbisPadConfig *confPad;

typedef struct OrbisGlobalConf
{
	Orbis2dConfig *conf;
	OrbisPadConfig *confPad;
	OrbisAudioConfig *confAudio;
	OrbisKeyboardConfig *confKeyboard;
	ps4LinkConfiguration *confLink;
	int orbisLinkFlag;
}OrbisGlobalConf;

OrbisGlobalConf *myConf;

void updateController()
{
	int ret;
	unsigned int buttons=0;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"Combo L2R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R1) )
		{
			debugNetPrintf(DEBUG,"Combo L1R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L1|ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"Combo L1R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
			
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R1) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R1) )
		{
			debugNetPrintf(DEBUG,"Combo L2R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_UP) || orbisPadGetButtonHold(ORBISPAD_UP))
		{
			debugNetPrintf(DEBUG,"Up pressed\n");
			
			if(y-step>=0)
			{
				y=y-step;
			}
			else
			{
				y=0;
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			
			if(y+step<conf->height-1)
			{
				y=y+step;
			}
			else
			{
				y=conf->height-1-step;
			}
		}						
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
			
			if(x+step<conf->width-1)
			{
				x=x+step;
			}
			else
			{
				x=conf->width-1-step;
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
			
			if(x-step>=0)
			{
				x=x-step;
			}
			else
			{
				x=0;
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			debugNetPrintf(DEBUG,"Triangle pressed exit\n");
			
			flag=0;
				
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			debugNetPrintf(DEBUG,"Circle pressed reset position and color red\n");
			x=1280/2;
			y=720/2;
			color=0x80ff0000;	
			orbisAudioResume(0);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed rand color\n");
			R=rand()%256;
			G=rand()%256;
			B=rand()%256;
			color=0x80000000|R<<16|G<<8|B;
			orbisAudioStop();
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
			orbisAudioPause(0);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			debugNetPrintf(DEBUG,"L1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			debugNetPrintf(DEBUG,"L2 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			debugNetPrintf(DEBUG,"R1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"R2 pressed\n");
			
		}
		
			
	}
}
void finishApp()
{
	orbisAudioFinish();
	orbisPadFinish();
	
	orbis2dFinish();
	
	ps4LinkFinish();
		
}
void initApp()
{
	int ret;

	
	//while(!ps4LinkRequestsIsConnected())
	//{
		
	//}
	debugNetPrintf(DEBUG,"[PS4LINK] Initialized and connected from pc/mac ready to receive commands\n");
	
	//hide playroom splash
	sceSystemServiceHideSplashScreen();
	
	
	ret=orbisPadInitWithConf(myConf->confPad);
	
	if(ret==1)
	{
		
	    confPad=orbisPadGetConf();
	
		ret=orbis2dInitWithConf(myConf->conf);
		
		
		if(ret==1)
		{
			conf=orbis2dGetConf();
			flag=1;
			ret=orbisAudioInitWithConf(myConf->confAudio);
			if(ret!=1)
			{
				debugNetPrintf(DEBUG,"libaudio ready\n");
				
				//ret=orbisAudioInitChannel(ORBISAUDIO_CHANNEL_MAIN,1024,48000,ORBISAUDIO_FORMAT_S16_STEREO);
			}
		}
	}
	
}
int main(int argc, char *argv[])
{
	int ret;


	uintptr_t intptr=0;
	sscanf(argv[1],"%p",&intptr);
	myConf=(OrbisGlobalConf *)intptr;
	ret=ps4LinkInitWithConf(myConf->confLink);
	if(!ret)
	{
		ps4LinkFinish();
		return 0;
	}
	initApp();
	
	Mod_Init(0);
	Mod_Load("host0:zweifeld.mod");
	Mod_Play();
    orbisAudioResume(0);
	
	
	
	
	while(flag)
	{
		//capture pad data and populate positions
		// X random color
		// O reset to center position and red color
		// /\ to exit
		// dpad move rectangle
		updateController();
				
				
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer();
				
		//default red is here press X to random color
		orbis2dDrawRectColor(x,w,y,h,color);
				
		//flush and flip
		orbis2dFinishDrawing(flipArg);
				
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
	}
	
	orbisAudioResume(0);
	Mod_End();
	//wait for current display buffer
	orbis2dStartDrawing();

	// clear with background (default white) to the current display buffer 
	orbis2dClearBuffer();
					
	//flush and flip
	orbis2dFinishDrawing(flipArg);
			
	//swap buffers
	orbis2dSwapBuffers();
	
	finishApp();
	
	myConf->orbisLinkFlag=1;


	return 0;
}