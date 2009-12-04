/*  
**  Created by wplaat (www.plaatsoft.nl)
**
**  Copyright (C) 2009
**  ==================
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, version 2.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <wiiuse/wpad.h>
#include <mxml.h>

#include "General.h"
#include "GRRLIB.h"
#include "Pointer.h"
#include "Trace.h"
#include "Button.h"
  
extern Trace *trace;
extern Button *buttons[MAX_BUTTONS];
extern Game game;
extern Setting settings[MAX_SETTINGS+1]; 

boolean selectedA=false;

// ------------------------------
// Constructor 
// ------------------------------

Pointer::Pointer()
{
   const char *s_fn="Pointer::Pointer";
   trace->event(s_fn,0,"enter");
   
   x=0;
   xOffset=0;
   y=0;
   yOffset=0;
   angle=0;
   rumble=0;
   rumbleGo=false;
   
   trace->event(s_fn,0,"leave [void]");
}

// ------------------------------
// Destructor
// ------------------------------

Pointer::~Pointer()
{
  const char *s_fn="Pointer::~Pointer";
  trace->event(s_fn,0,"enter");
  
  trace->event(s_fn,0,"leave [void]");
}

// ------------------------------
// Others
// ------------------------------

void Pointer::properties(void)
{  
	char tmp[50];
	int size=12;
 
	sprintf(tmp, "x=%d", x);
	GRRLIB_Printf2(10, 10, tmp, size, COLOR_WHITESMOKE); 
	
	sprintf(tmp, "y=%d", y);
	GRRLIB_Printf2(10, 20, tmp, size, COLOR_WHITESMOKE);
}

void saveSettingFile(const char* filename)
{
    const char *s_fn="saveSettingFile";
    trace->event(s_fn,0,"enter");
	
    int i;
    mxml_node_t *xml;
    mxml_node_t *group;
    mxml_node_t *data;   
    char temp[MAX_LEN];
      
    xml = mxmlNewXML("1.0");
   
    group = mxmlNewElement(xml, "TowerDefense");
   
    for(i=0; i<MAX_SETTINGS; i++)
    {
        sprintf(temp, "entry%d", i);
        data = mxmlNewElement(group, temp);
  
        mxmlElementSetAttr(data, "key", settings[i].key);	  
 	    mxmlElementSetAttr(data, "value", settings[i].value);			  
    }
  
    /* now lets save the xml file to a file! */
    FILE *fp;
    fp = fopen(filename, "w");

    mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
   
    fclose(fp);
    mxmlDelete(data);
    mxmlDelete(group);
    mxmlDelete(xml);
   
    // Update game.name value
    if ((settings[0].value[0]!=0x00) && (settings[1].value[0]!=0x00) && (settings[2].value[0]!=0x00))
    {
      sprintf(temp,"%c%c%c",settings[0].value[0],settings[1].value[0],settings[2].value[0]);
      strcpy(game.name,temp);
    }
    trace->event(s_fn,0,"leave [void]");
}


void buttonPlus(int index)
{
   switch (index)
   {
      case 0:
         // Music volume
	     //if (musicVolume<MAX_SOUND_VOLUME) musicVolume++;   
	     //MODPlay_SetVolume( &snd1, musicVolume*MUSIC_MULTIPLER,musicVolume*MUSIC_MULTIPLER); 
		 break;
		 
	  case 1:
         // Effect volume
         //if (effectVolume<MAX_SOUND_VOLUME) effectVolume++; 
		 break;
		 
	  case 2:
	     // Next music Track
         //MODPlay_Stop(&snd1);
         //if (selectedMusic<MAX_MUSIC_TRACK) selectedMusic++; else selectedMusic=1;
	     //initMusicTrack();
		 break;
		 
	  case 3:
		  // First Character
		  if (settings[0].value[0]==0x00) 
		  {
		     settings[0].value[0]='A';
			 strcpy(settings[0].key,"FIRST_CHAR");
		  }
		  else
		  {
		     if (settings[0].value[0]<90) settings[0].value[0]++; else settings[0].value[0]='A';
		  }		
		  break;
		  
	  case 4:
		  // Second Character
		  if (settings[1].value[0]==0x00) 
		  {
		     settings[1].value[0]='A';
			 strcpy(settings[1].key,"SECOND_CHAR");
		  }
		  else
		  {
		     if (settings[1].value[0]<90) settings[1].value[0]++; else settings[1].value[0]='A';
		  }		
		  break;
		  
	  case 5:
		  // Third Character
		  if (settings[2].value[0]==0x00) 
		  {
		     settings[2].value[0]='A';
			 strcpy(settings[2].key,"THIRD_CHAR");
		  }
		  else
		  {
		     if (settings[2].value[0]<90) settings[2].value[0]++; else settings[2].value[0]='A';
		  }		
		  break;
   }
}

void buttonMinus(int index)
{
   switch (index)   
   {
       case 0:
	      // Music volume
	      //if (musicVolume>0) musicVolume--;   
	      //MODPlay_SetVolume( &snd1, musicVolume*MUSIC_MULTIPLER,musicVolume*MUSIC_MULTIPLER);
		  break;
       
	   case 1:
          // Effect volume
          //if (effectVolume>0) effectVolume--; 
		  break;
   
       case 2:
	      // Prev music track
          //MODPlay_Stop(&snd1);
          //if (selectedMusic>1) selectedMusic--; else selectedMusic=MAX_MUSIC_TRACK;
	      //initMusicTrack();
		  break;
	
	    case 3:
		  // First Character
		  if (settings[0].value[0]==0x00) 
		  {
		     settings[0].value[0]='A';
			 strcpy(settings[0].key,"FIRST_CHAR");
		  }
		  else
		  {
		     if (settings[0].value[0]>65) settings[0].value[0]--; else settings[0].value[0]='Z';
		  }		
		  break;
		  
		case 4:
		  // Second Character
		  if (settings[1].value[0]==0x00) 
		  {
		     settings[1].value[0]='A';
			 strcpy(settings[1].key,"SECOND_CHAR");
		  }
		  else
		  {
		     if (settings[1].value[0]>65) settings[1].value[0]--; else settings[1].value[0]='Z';
		  }		
		  break;
		  
		case 5:
		  // Third Character
		  if (settings[2].value[0]==0x00) 
		  {
		     settings[2].value[0]='A';
			 strcpy(settings[2].key,"THIRD_CHAR");
		  }
		  else
		  {
		     if (settings[2].value[0]>65) settings[2].value[0]--; else settings[2].value[0]='Z';
		  }		
		  break;
   }
}


void buttonA(int x, int y)
{
  //const char *s_fn="Pointer::buttonA";

  if (selectedA) return;
  selectedA=true;
	  
  switch (game.stateMachine)
  {
     case stateIntro1:
	 {
	   game.stateMachine=stateIntro2;
	 }
	 break;

	 case stateIntro2:
	 {
	   //game.stateMachine=stateIntro3;
	   game.stateMachine=stateMenu;
	 }
	 break;
	 
	 case stateIntro3:
	 {
	   game.stateMachine=stateMenu;
	 }
	 break;
	 
	 case stateMenu:
	 {
	    if (buttons[0]->onSelect(x,y))
		{
          // Map1 button	      
		  game.stateMachine=stateGame;
		  game.selectedMap=1;
		}
		
		if (buttons[1]->onSelect(x,y))
		{
          // Map2 button	      
		  game.stateMachine=stateGame;
		  game.selectedMap=2;
		}
		
		if (buttons[2]->onSelect(x,y))
		{
          // Map3 button	      
		  game.stateMachine=stateGame;
		  game.selectedMap=3;
		}
		
		if (buttons[5]->onSelect(x,y))
		{
          // Credits button	      
		  game.stateMachine=stateCredits;
		}
		
		if (buttons[8]->onSelect(x,y))
		{
          // User Initials button	      
		  game.stateMachine=stateSettings;
		}
	 }
	 break;
   
	 case stateCredits:
     {
        if (buttons[0]->onSelect(x,y))
	    {
           // Next button	
		   game.stateMachine=stateMenu;	     
	    }
     }
	 break;
	 
   	 case stateSettings:
     { 
        if (buttons[0]->onSelect(x,y))
	    {
			// + First Character button event           
			buttonPlus(3);  
	    }
		
        if (buttons[1]->onSelect(x,y))
	    {
			// - First Character button event           
			buttonPlus(3);  
	    }
					

        if (buttons[2]->onSelect(x,y))
	    {
			// + Second Character button event           
			buttonPlus(4);  
	    }
		
        if (buttons[3]->onSelect(x,y))
	    {
			// - Second Character button event           
			buttonPlus(4);  
	    }
					    
        if (buttons[4]->onSelect(x,y))
	    {
			// + Third Character button event           
			buttonPlus(5);  
	    }
		
        if (buttons[5]->onSelect(x,y))
	    {
			// - Third Character button event           
			buttonMinus(5);  
	    }
		
        if (buttons[6]->onSelect(x,y))
	    {
           // Next button	
		   saveSettingFile(SETTING_FILENAME); 
		   game.stateMachine=stateMenu;	     
	    }
     }
	 break; 
  }
}


void Pointer::draw(void)
{   
    const char *s_fn="Pointer::draw";

	// Scan for button events
	WPAD_SetVRes(index, 640, 480);
	WPAD_ScanPads();
			
	u32 wpaddown = WPAD_ButtonsDown(index);
	u32 wpadup   = WPAD_ButtonsUp(index);
	u32 wpadheld = WPAD_ButtonsHeld(index);


	// Scan for ir events 
	WPAD_IR(index, &ir); 
	x=ir.sx-WSP_POINTER_X;
	xOffset=x+IR_X_OFFSET;
	y=ir.sy-WSP_POINTER_Y;
	yOffset=y+IR_Y_OFFSET;
	angle=ir.angle;
	
	if (wpaddown & BUTTON_A) buttonA(xOffset,yOffset); 
	if (wpadup & BUTTON_A) selectedA=false;

	// Scan for button events
	if (wpaddown & WPAD_BUTTON_HOME) 
	{
	  if (game.stateMachine==stateMenu)
	  {
		trace->event(s_fn,0,"Home button pressed");
		game.stateMachine=stateQuit;
	  }
	  else
	  {
		game.stateMachine=stateMenu;
	  }
	}
	
	 // Make screenshot 
    if (wpadheld & BUTTON_PLUS)
    {
		char filename[MAX_LEN];
		struct tm *level;	   
		time_t dt=time(NULL);
		level = localtime(&dt);
		sprintf(filename,"%sTowerDefense-%04d%02d%02d%02d%02d%02d.png", GAME_DIRECTORY, level->tm_year+1900,level->tm_mon+1, level->tm_mday,  level->tm_hour, level->tm_min, level->tm_sec);		  
        GRRLIB_ScrShot(filename);	
	}

	if (rumble>0) 
	{
		rumble--;
		WPAD_Rumble(index,1); 
	}
	else 
	{
		WPAD_Rumble(index,0);
	}
				
    // Draw Pointer on screen
    GRRLIB_DrawImg( x, y, image, angle, 1.0, 1.0, IMAGE_COLOR );		
}

// ------------------------------
// Setters and getters 
// ------------------------------

void Pointer::setIndex(int index1)
{
   const char *s_fn="Pointer::setIndex";
   trace->event(s_fn,0,"enter [index=%d]",index1);
  
   index = index1;
   
   trace->event(s_fn,0,"leave [void]");
}

void Pointer::setX(int x1)
{
   const char *s_fn="Pointer::setX";
   trace->event(s_fn,0,"enter [x=%d]",x1);
   
   if ((x1>=0) && (x1<=MAX_HORZ_PIXELS))
   {
      x = x1;
   }
   
   trace->event(s_fn,0,"leave [void]");
}

void Pointer::setY(int y1)
{
   const char *s_fn="Pointer::setY";
   trace->event(s_fn,0,"enter [y=%d]",y1);
   
   if ((y1>=0) && (y1<=MAX_VERT_PIXELS))
   {
      y = y1;
   }
   
   trace->event(s_fn,0,"leave [void]");
}

void Pointer::setAngle(int angle1)
{
   const char *s_fn="Pointer::setAngle";
   trace->event(s_fn,0,"enter [angle=%d]",angle1);
   
   if ((angle1>=0) && (angle1<=MAX_ANGLE))
   {
      angle=angle1;
   }
   
   trace->event(s_fn,0,"leave [void]");
} 

void Pointer::setImage(GRRLIB_texImg *image1)
{
   const char *s_fn="Pointer::setImage";
   trace->event(s_fn,0,"enter");
   
   image = image1;
   
   trace->event(s_fn,0,"leave [void]");
}

void Pointer::setRumble(int rumble1)
{
   const char *s_fn="Pointer::setRumble";
   trace->event(s_fn,0,"enter [rumble=%d]",rumble1);
   
   rumble=rumble1;

   trace->event(s_fn,0,"leave [void]");
}

int Pointer::getX()
{
	return xOffset;
}

int Pointer::getY()
{
	return yOffset;
}
	
// ------------------------------
// The End
// ------------------------------