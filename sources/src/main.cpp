/** 
 *  @file 
 *  @brief The file contain the main class methodes
 *  @author wplaat
 *  @note Tab size of source code is two
 *
 *  Copyright (C) 2008-2010 PlaatSoft
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <ogcsys.h>
#include <gccore.h>
#include <gcmodplay.h> 
#include <wiiuse/wpad.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h> 
#include <stdarg.h>
#include <asndlib.h>
#include <fat.h>
#include <mxml.h>
#include <sys/dir.h>
#include <ogc/lwp_watchdog.h>	

#include "grrlib.h"
#include "General.h"
#include "Trace.h"
#include "Settings.h"
#include "HighScore.h"
#include "Sound.h"
#include "Monster.h"
#include "MonsterSpecs.h"
#include "Weapon.h"
#include "WeaponSpecs.h"
#include "Button.h"
#include "Pointer.h"
#include "Grid.h"
#include "Matrix.h"
#include "http.h"
#include "font_ttf.h" 

// -----------------------------------------------------------
// PROTOTYPES
// -----------------------------------------------------------

static u8 calculateFrameRate(void);
void checkGameOver(void);
void checkNextWave(void);
void checkPointer(void);
void checkDeadMonsters(void);
void moveMonsters(void);
void moveWeapons(void);

// -----------------------------------------------------------
// TYPEDEF
// -----------------------------------------------------------

typedef struct 
{
  // png + jpg Image index     
  GRRLIB_texImg *intro1;
  GRRLIB_texImg *intro2;
  GRRLIB_texImg *intro3;
  GRRLIB_texImg *intro4;
  
  GRRLIB_texImg *soundicon;
  
  GRRLIB_texImg *logo1;
  GRRLIB_texImg *logo2;
  GRRLIB_texImg *logo3;
  GRRLIB_texImg *logo4;
  GRRLIB_texImg *logo5;
  GRRLIB_texImg *logo6;		
  GRRLIB_texImg *logo;
  
  GRRLIB_texImg *background1;
  GRRLIB_texImg *background2;
  GRRLIB_texImg *bar;
  GRRLIB_texImg *barCursor;  
  
  GRRLIB_texImg *scrollbar;
  GRRLIB_texImg *scrollTop;
  GRRLIB_texImg *scrollMiddle;
  GRRLIB_texImg *scrollBottom;
      
  GRRLIB_texImg *button1;
  GRRLIB_texImg *buttonFocus1;  
  GRRLIB_texImg *button2;
  GRRLIB_texImg *buttonFocus2;
  GRRLIB_texImg *button3;
  GRRLIB_texImg *buttonFocus3;
  GRRLIB_texImg *button4;
  GRRLIB_texImg *buttonFocus4;
  
  GRRLIB_texImg *pointer1;
  GRRLIB_texImg *pointer2;
  GRRLIB_texImg *pointer3; 
  GRRLIB_texImg *pointer4;
} 
image;

image images;

typedef struct
{
   time_t dt;							/**< Time stamp of entry */
   char   score[MAX_LEN];			/**< Score */
   char   name[MAX_LEN];			/**< Player nickname */
   char   location[MAX_LEN];		/**< Location (Country - City) */
}
topscore;

/** topscore contains today highscore list [0..40] */
topscore todayHighScore[MAX_TODAY_HIGHSCORE+1];

/** topscore contains global highscore list [0..40] */
topscore globalHighScore[MAX_GLOBAL_HIGHSCORE+1];

// -----------------------------------------------------------
// VARIABLES
// -----------------------------------------------------------

// logo1 Image
extern const unsigned char     pic4data[];
extern int      pic4length;

// logo2 Image
extern const unsigned char     pic5data[];
extern int      pic5length;

// logo3 Image
extern const unsigned char     pic6data[];
extern int      pic6length;

// logo4 Image
extern const unsigned char     pic7data[];
extern int      pic7length;

// logo5 Image
extern const unsigned char     pic8data[];
extern int      pic8length;

// logo6 Image
extern const unsigned char     pic9data[];
extern int      pic9length;

// Background1 Image
extern const unsigned char     pic10data[];
extern int      pic10length;

// Background2 Image
extern const unsigned char     pic11data[];
extern int      pic11length;

// Background3 Image
extern const unsigned char     pic12data[];
extern int      pic12length;

// Bar Image
extern const unsigned char     pic14data[];
extern int      pic14length;

// Bar_cursor Image
extern const unsigned char     pic15data[];
extern int      pic15length;

// Sound Icon Image
extern const unsigned char     pic16data[];
extern int      pic16length;

// panel Easy Image
extern const unsigned char     pic20data[];
extern int      pic20length;

// panel Medium Image
extern const unsigned char     pic21data[];
extern int      pic21length;

// panel Hard Image
extern const unsigned char     pic22data[];
extern int      pic22length;

// Scrollbar image
extern const unsigned char     pic33data[];
extern const int      pic33length;

// ScrollTop image
extern const unsigned char     pic34data[];
extern const int      pic34length;

// ScrollMiddle image
extern const unsigned char     pic35data[];
extern const int      pic35length;

// scrollBottom image
extern const unsigned char     pic36data[];
extern const int      pic36length;

// Pointer1 Image
extern const unsigned char     pic200data[];
extern int      pic200length;

// Pointer2 Image
extern const unsigned char     pic201data[];
extern int      pic201length;

// Pointer3 Image
extern const unsigned char     pic202data[];
extern int      pic202length;

// Pointer4 Image
extern const unsigned char     pic203data[];
extern int      pic203length;

// Button1 Image
extern const unsigned char     pic600data[];
extern int      pic600length;

// Button1Focus Image
extern const unsigned char     pic601data[];
extern int      pic601length;

// Button2 Image
extern const unsigned char     pic602data[];
extern int      pic602length;

// Button2Focus Image
extern const unsigned char     pic603data[];
extern int      pic603length;

// Button3 Image
extern const unsigned char     pic604data[];
extern int      pic604length;

// Button3Focus Image
extern const unsigned char     pic605data[];
extern int      pic605length;

// Button4 Image
extern const unsigned char     pic606data[];
extern int      pic606length;

// Button4Focus Image
extern const unsigned char     pic607data[];
extern int      pic607length;

// Intro4 Image
extern const unsigned char     pic1000data[];
extern int      pic1000length;

u32          *frameBuffer[1] 	= {NULL};	/**< Video memory buffer */
GXRModeObj   *rmode 				= NULL;		/**< Video settings buffer */
Mtx          GXmodelView2D;					/**< Video mode = 2D */

Game 			 game;								/**< Game paramters */
Trace     	 *trace;								/**< Trace object */
Settings  	 *settings;							/**< Settings object */
HighScore 	 *highScore;						/**< Highscore object */
Sound      	 *sound;								/**< Sound object */
WeaponSpecs  *weaponSpecs;						/**< WeaponSpecs object */
MonsterSpecs *monsterSpecs;					/**< MonsterSpecs object */
Matrix		 *matrix;							/**< Matrix object */
Grid      	 *grids[MAX_GRIDS];				/**< Grid array */
Monster   	 *monsters[MAX_MONSTERS];		/**< Monster array */
Pointer   	 *pointers[MAX_POINTERS];		/**< Pointer array */
Weapon    	 *weapons[MAX_WEAPONS];			/**< Weapon array */
Button    	 *buttons[MAX_BUTTONS];			/**< Button array */

GRRLIB_ttfFont *myFont; 

// -----------------------------------
// Destroy METHODES
// -----------------------------------

/**
 * destroy all weapons objects.
 */
void destroyWeapons(void)
{
	const char *s_fn="destroyWeapons";
	trace->event(s_fn,0,"enter");
	
	// Destroy all Weapons
	for( int i=0; i<MAX_WEAPONS; i++)
    {
		if (weapons[i]!=NULL)
		{
			delete weapons[i];
			weapons[i]=NULL;
		}
   }

	trace->event(s_fn,0,"leave");
}

/**
 * destroy all button objects.
 */
void destroyButtons(void)
{
	const char *s_fn="destroyButtons";
	trace->event(s_fn,0,"enter");
	
	// Destroy all Buttons
	for( int i=0; i<MAX_BUTTONS; i++)
   {
		if (buttons[i]!=NULL)
		{
			delete buttons[i];
			buttons[i]=NULL;
		}
   }	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy all monsters objects.
 */
void destroyMonsters(void)
{
	const char *s_fn="clearMonsters";
	trace->event(s_fn,0,"enter");
	
	// Clear monster array
	for( int i=0; i<MAX_MONSTERS; i++)	
	{
		if (monsters[i]!=NULL)
		{
			delete monsters[i];
			monsters[i]=NULL;
		}
	}
	trace->event(s_fn,0,"leave");
}

/**
 * destroy weaponsspecs object.
 */
void destroyMonsterSpecs(void)
{
	const char *s_fn="destroyMonsterSpecs";
	trace->event(s_fn,0,"enter");
	
	// Destroy Settings
	if (monsterSpecs!=NULL)
	{
		delete monsterSpecs;
		monsterSpecs=NULL;
	}
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy all grid matrix in memory.
 */
void destroyMatrix(void)
{
	const char *s_fn="destroyMatrx";
	trace->event(s_fn,0,"enter");
	
	// Destroy Settings
	if (matrix!=NULL)
	{
		delete matrix;
		matrix=NULL;
	}
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy all pointers objects.
 */
void destroyPointers(void)
{
	const char *s_fn="destroyPointers";
	trace->event(s_fn,0,"enter");
	
	// Destroy Pointers
	for( int i=0; i<MAX_POINTERS; i++)
	{
		if (pointers[i]!=NULL)
		{
			delete pointers[i];
			pointers[i]=NULL;
		}
	}	
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy all grids objects.
 */
void destroyGrids(void)
{
	const char *s_fn="destroyGrids";
	trace->event(s_fn,0,"enter");
	
	// Destroy Grid
	for( int i=0; i<MAX_GRIDS; i++)
	{
		if (grids[i]!=NULL)
		{
			delete grids[i];
			grids[i]=NULL;
		}
	}
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy sound object.
 */
void destroySound(void)
{
	const char *s_fn="destroySound";
	trace->event(s_fn,0,"enter");
  
	// Destroy Sound
	if (sound!=NULL)
	{
		delete sound;
		sound=NULL;
	}	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy local highscore object.
 */
void destroyHighScore(void)
{
	const char *s_fn="destroyHighScore";
	trace->event(s_fn,0,"enter");
	
	// Destroy Highscore
	if (highScore!=NULL)
	{
		delete highScore;
		highScore=NULL;
	}
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy setting object.
 */
void destroySettings(void)
{
	const char *s_fn="destroySettings";
	trace->event(s_fn,0,"enter");
	
	// Destroy Settings
	if (settings!=NULL)
	{
		delete settings;
		settings=NULL;
	}
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy weaponSpec object.
 */
void destroyWeaponSpecs(void)
{
	const char *s_fn="destroyWeaponSpecs";
	trace->event(s_fn,0,"enter");
	
	// Destroy Settings
	if (weaponSpecs!=NULL)
	{
		delete weaponSpecs;
		weaponSpecs=NULL;
	}
	
	trace->event(s_fn,0,"leave");
}

/**
 * destroy trace object.
 */
void destroyTrace(void)
{	
	// Destroy Trace
	if (trace!=NULL)
	{
		delete trace;
		trace=NULL;
	}
}

/**
 * destroy all images in memory.
 */
void destroyImages(void)
{
   const char *s_fn="destroyImages";
   trace->event(s_fn,0,"enter");

   GRRLIB_FreeTexture(images.logo1);
   GRRLIB_FreeTexture(images.logo2);
	GRRLIB_FreeTexture(images.logo3);
	GRRLIB_FreeTexture(images.logo4);
	GRRLIB_FreeTexture(images.logo5);
	GRRLIB_FreeTexture(images.logo6);
		
   GRRLIB_FreeTexture(images.background1);
   GRRLIB_FreeTexture(images.background2);
	GRRLIB_FreeTexture(images.intro4);
   
   GRRLIB_FreeTexture(images.bar);
   GRRLIB_FreeTexture(images.barCursor);
   GRRLIB_FreeTexture(images.soundicon);
		
   GRRLIB_FreeTexture(images.pointer1);
   GRRLIB_FreeTexture(images.pointer2);
   GRRLIB_FreeTexture(images.pointer3);
   GRRLIB_FreeTexture(images.pointer4);
         	
   GRRLIB_FreeTexture(images.button1);
   GRRLIB_FreeTexture(images.buttonFocus1);
   GRRLIB_FreeTexture(images.button2);
   GRRLIB_FreeTexture(images.buttonFocus2);
   GRRLIB_FreeTexture(images.button3);
   GRRLIB_FreeTexture(images.buttonFocus3);
   GRRLIB_FreeTexture(images.button4);
   GRRLIB_FreeTexture(images.buttonFocus4);
     
   trace->event(s_fn,0,"leave");
}

// -----------------------------------
// INIT METHODES
// -----------------------------------

/**
 * Initialise today highscore.
 */
void initTodayHighScore(void)
{
	const char *s_fn="initTodayHighScore";
	trace->event(s_fn,0,"enter");
   
	// Init today highscore memory
	for(int i=0; i<MAX_TODAY_HIGHSCORE; i++)
	{
		todayHighScore[i].score[0]=0x00;
		todayHighScore[i].dt=0;
		todayHighScore[i].name[0]=0x00;
		todayHighScore[i].location[0]=0x00;
	} 
	trace->event(s_fn,0,"leave [void]");
}

/**
 * Initialise global highscore.
 */
void initGlobalHighScore(void)
{
	const char *s_fn="initGlobalHighScore";	
	trace->event(s_fn,0,"enter");
    
	// Init global highscore memory
	for(int i=0; i<MAX_GLOBAL_HIGHSCORE; i++)
	{
		globalHighScore[i].score[0]=0x00;
		globalHighScore[i].dt=0;
		globalHighScore[i].name[0]=0x00;
		globalHighScore[i].location[0]=0x00;
	} 
  
	trace->event(s_fn,0,"leave [void]");
}

/**
 * Initialise images.
 */
void initImages(void)
{
	const char *s_fn="initImages";
	trace->event(s_fn,0,"enter");

   images.logo1=GRRLIB_LoadTexture( pic4data );
	GRRLIB_SetMidHandle( images.logo1, true );
	
	images.logo2=GRRLIB_LoadTexture( pic5data );
	images.logo=GRRLIB_LoadTexture( pic5data );
	GRRLIB_InitTileSet(images.logo, images.logo->w, 1, 0);
	
	images.logo3=GRRLIB_LoadTexture( pic6data );
	images.logo4=GRRLIB_LoadTexture( pic7data );
	images.logo5=GRRLIB_LoadTexture( pic8data );
	images.logo6=GRRLIB_LoadTexture( pic9data );
   
	images.background1=GRRLIB_LoadTexture( pic10data );
	images.background2=GRRLIB_LoadTexture( pic11data );
   images.intro4=GRRLIB_LoadTexture( pic1000data );
	
	images.bar=GRRLIB_LoadTexture( pic14data );
	images.barCursor=GRRLIB_LoadTexture( pic15data );
	
	images.soundicon=GRRLIB_LoadTexture( pic16data );
	GRRLIB_SetMidHandle( images.soundicon, true ); 	
	
	images.scrollbar=GRRLIB_LoadTexture(pic33data);
	images.scrollTop=GRRLIB_LoadTexture( pic34data);
	images.scrollMiddle=GRRLIB_LoadTexture( pic35data);
	images.scrollBottom=GRRLIB_LoadTexture( pic36data);
   
	images.pointer1=GRRLIB_LoadTexture( pic200data); 
	images.pointer2=GRRLIB_LoadTexture( pic201data);
	images.pointer3=GRRLIB_LoadTexture( pic202data);
	images.pointer4=GRRLIB_LoadTexture( pic203data);
      
	images.button1=GRRLIB_LoadTexture( pic600data );
	images.buttonFocus1=GRRLIB_LoadTexture( pic601data );  
	images.button2=GRRLIB_LoadTexture( pic602data );
	images.buttonFocus2=GRRLIB_LoadTexture( pic603data );  
	images.button3=GRRLIB_LoadTexture( pic604data );
	images.buttonFocus3=GRRLIB_LoadTexture( pic605data );  
	images.button4=GRRLIB_LoadTexture( pic606data );
	images.buttonFocus4=GRRLIB_LoadTexture( pic607data );  
     
	trace->event(s_fn,0,"leave [void]");
}

/**
 * Create new monster
 * @param special 
 *	(true)  Place monster in random grid 
 *	(false) Place monster in selected grid
 */
void initMonsters(bool special)
{
	const char *s_fn="initMonsters";
	trace->event(s_fn,0,"enter");
   
	int delay=1;   
   
	// Get amount of monster
	int amount=matrix->getEnemyAmount();
	  
	for( int i=0; i<amount; i++ ) 
	{		
		trace->event(s_fn,0,"Init monster [%d]",i);
	
		// Find first empty place in monster array
		int id=0;
		while (monsters[id]!=NULL) 
		{
			if (++id>=(MAX_MONSTERS-1)) 
			{
				id=(MAX_MONSTERS-1);
				break;
			}
		}
		
		monsters[id]=new Monster();	  		
		monsters[id]->setIndex(id);
		
		// Set delay between two monsters.	
		monsters[id]->setDelay(delay);		
		delay+=matrix->getEnemyDelay();
		
		// Choose randon monster type depending on wave index.
		int type = (int) (rand() % game.wave);
		monsters[id]->setImage(monsterSpecs->getImage(type));
		monsters[id]->setEnergy(monsterSpecs->getEnergy(type));
		monsters[id]->setStep(matrix->getEnemyStep());	
			
		if (special)
		{
		    // Set monster in random grid (Map Select behalvior)
			monsters[id]->setGrid((int) (rand() % MAX_GRIDS));
		}
		else
		{
			// Set monster in selected grid (Game behalvior)
			if (game.selectedMap!=-1)
			{
				monsters[id]->setGrid(game.selectedMap);
			}
		}
	}
	trace->event(s_fn,0,"leave [void]");
}

/**
 * Initialise WiiMote pointers.
 */
void initPointers(void)
{
   const char *s_fn="initPointers";
   trace->event(s_fn,0,"enter");
      
   pointers[0] = new Pointer();   
   pointers[0]->setX(320);
   pointers[0]->setY(240);
   pointers[0]->setAngle(0);
   pointers[0]->setImage(images.pointer1);
   pointers[0]->setIndex(0);
	pointers[0]->setColor(IMAGE_COLOR);

   pointers[1] = new Pointer(); 
   pointers[1]->setX(320);
   pointers[1]->setY(240);
   pointers[1]->setAngle(0);
   pointers[1]->setImage(images.pointer2);
   pointers[1]->setIndex(1);
	pointers[1]->setColor(IMAGE_COLOR);

   pointers[2] = new Pointer(); 
   pointers[2]->setX(320);
   pointers[2]->setY(240);
   pointers[2]->setAngle(0);
   pointers[2]->setImage(images.pointer3);
   pointers[2]->setIndex(2);
	pointers[2]->setColor(IMAGE_COLOR);

   pointers[3] = new Pointer(); 
   pointers[3]->setX(340);
   pointers[3]->setY(240);
   pointers[3]->setAngle(0);
   pointers[3]->setImage(images.pointer4);	
   pointers[3]->setIndex(3);
	pointers[3]->setColor(IMAGE_COLOR);
      
   trace->event(s_fn,0,"leave [void]");
}

/**
 * Init Grid 
 * @param level Level index.
 */
void initGrids(int level)
{
   const char *s_fn="initGrids";
   trace->event(s_fn,0,"enter [level=%d]",level);

	// First destroy all loaded grids, if any
   destroyGrids();
	
	switch( level )
	{
		// Easy levels
		case gameEasy:
		
			grids[0] = new Grid();
			grids[0]->setIndex(0);
			grids[0]->create(GRID1_DIRECTORY);

			grids[1] = new Grid();
			grids[1]->setIndex(1);
			grids[1]->create(GRID2_DIRECTORY);

			grids[2] = new Grid();
			grids[2]->setIndex(2);
			grids[2]->create(GRID3_DIRECTORY);

			grids[3] = new Grid();
			grids[3]->setIndex(3);
			grids[3]->create(GRID4_DIRECTORY);

			grids[4] = new Grid();
			grids[4]->setIndex(4);
			grids[4]->create(GRID5_DIRECTORY);

			grids[5] = new Grid();
			grids[5]->setIndex(5);
			grids[5]->create(GRID6_DIRECTORY);				  
			break;
				  				  
		case gameMedium: 
		
			grids[0] = new Grid();
			grids[0]->setIndex(0);
			grids[0]->create(GRID7_DIRECTORY);

			grids[1] = new Grid();
			grids[1]->setIndex(1);
			grids[1]->create(GRID8_DIRECTORY);

			grids[2] = new Grid();
			grids[2]->setIndex(2);
			grids[2]->create(GRID9_DIRECTORY);

			grids[3] = new Grid();
			grids[3]->setIndex(3);
			grids[3]->create(GRID10_DIRECTORY);

			grids[4] = new Grid();
			grids[4]->setIndex(4);
			grids[4]->create(GRID11_DIRECTORY);

			grids[5] = new Grid();
			grids[5]->setIndex(5);
			grids[5]->create(GRID12_DIRECTORY);				  
			break;
				  				  
		case gameHard: 
		
			grids[0] = new Grid();
			grids[0]->setIndex(0);
			grids[0]->create(GRID13_DIRECTORY);

			grids[1] = new Grid();
			grids[1]->setIndex(1);
			grids[1]->create(GRID14_DIRECTORY);

			grids[2] = new Grid();
			grids[2]->setIndex(2);
			grids[2]->create(GRID15_DIRECTORY);

			grids[3] = new Grid();
			grids[3]->setIndex(3);
			grids[3]->create(GRID16_DIRECTORY);

			grids[4] = new Grid();
			grids[4]->setIndex(4);
			grids[4]->create(GRID17_DIRECTORY);

			grids[5] = new Grid();
			grids[5]->setIndex(5);
			grids[5]->create(GRID18_DIRECTORY);				  
			break;
	}
	trace->event(s_fn,0,"leave [void]");
}

/**
 * Initialise new weapon with correct game parameters
 * @param x 	The x location.
 * @param y 	The y location.
 * @param id	The uniquec id number
 * @param type	The weapon type [0..5].	  
 */
void initWeapon(int x, int y, int id, int type)
{
	const char *s_fn="initWeapon";
	trace->event(s_fn,0,"enter");
   
	weapons[id]= new Weapon();	
	weapons[id]->setX(x);
	weapons[id]->setY(y);
	weapons[id]->setSelected(true);
	weapons[id]->setIndex(id);
	
	weapons[id]->setType(type);
	weapons[id]->setFireMode(weaponSpecs->getFireMode(type));
	weapons[id]->setImage(weaponSpecs->getImage(type));
	
	weapons[id]->setPower(weaponSpecs->getMinPower(type));
	weapons[id]->setRange(weaponSpecs->getMinRange(type));
	weapons[id]->setRate(weaponSpecs->getMinRate(type));
					
	weapons[id]->setMaxPower(weaponSpecs->getMaxPower(type));
	weapons[id]->setMaxRange(weaponSpecs->getMaxRange(type));
	weapons[id]->setMaxRate(weaponSpecs->getMaxRate(type));
			
	weapons[id]->setPowerPrice(weaponSpecs->getUpgradePrice(type));
	weapons[id]->setRangePrice(weaponSpecs->getUpgradePrice(type));
	weapons[id]->setRatePrice(weaponSpecs->getUpgradePrice(type));	
				
	weapons[id]->setPowerStep(weaponSpecs->getStepPower(type));
   weapons[id]->setRangeStep(weaponSpecs->getStepRange(type));
   weapons[id]->setRateStep(weaponSpecs->getStepRate(type));
	
	weapons[id]->setName("%s [%d]", 
		weaponSpecs->getName(type), weaponSpecs->getCounter(type));
		
	trace->event(s_fn,0,"leave");
}

/**
 * Init screen buttons
 */
void initButtons(void)
{
	const char *s_fn="initButtons";
	trace->event(s_fn,0,"enter");

	// First destroy existing buttons
	destroyButtons();
   
	switch( game.stateMachine )	
	{			
		case stateMainMenu:
		{
			int ypos=40;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=15;
			
			// Play Button 
			buttons[0]=new Button();
			buttons[0]->setX(440);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Play");
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(6);
			
			// HighScore Button 
			ypos+=40;
			buttons[1]=new Button();
			buttons[1]->setX(440);
			buttons[1]->setY(ypos);
			buttons[1]->setImageNormal(images.button2);
			buttons[1]->setImageFocus(images.buttonFocus2);
			buttons[1]->setLabel("High Score");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(0);
			
			// Help Button 
			ypos+=40;
			buttons[2]=new Button();
			buttons[2]->setX(440);
			buttons[2]->setY(ypos);
			buttons[2]->setImageNormal(images.button2);
			buttons[2]->setImageFocus(images.buttonFocus2);
			buttons[2]->setLabel("Help");			
			buttons[2]->setColor(IMAGE_COLOR);
			buttons[2]->setIndex(1);

			// Credits Button 
			ypos+=40;
			buttons[3]=new Button();
			buttons[3]->setX(440);
			buttons[3]->setY(ypos);
			buttons[3]->setImageNormal(images.button2);
			buttons[3]->setImageFocus(images.buttonFocus2);
			buttons[3]->setLabel("Credits");	
			buttons[3]->setColor(IMAGE_COLOR);
			buttons[3]->setIndex(2);
			
			// Release Notes Button 
			ypos+=40;
			buttons[5]=new Button();
			buttons[5]->setX(440);
			buttons[5]->setY(ypos);
			buttons[5]->setImageNormal(images.button2);
			buttons[5]->setImageFocus(images.buttonFocus2);
			buttons[5]->setLabel("Release Notes");	
			buttons[5]->setColor(IMAGE_COLOR);
			buttons[5]->setIndex(4);
			
			// Sound Settings Button 
			ypos+=40;
			buttons[4]=new Button();
			buttons[4]->setX(440);
			buttons[4]->setY(ypos);
			buttons[4]->setImageNormal(images.button2);
			buttons[4]->setImageFocus(images.buttonFocus2);
			buttons[4]->setLabel("Sound Settings");	
			buttons[4]->setColor(IMAGE_COLOR);
			buttons[4]->setIndex(3);
			
			// Game Settings Button 
			ypos+=40;
			buttons[6]=new Button();
			buttons[6]->setX(440);
			buttons[6]->setY(ypos);
			buttons[6]->setImageNormal(images.button2);
			buttons[6]->setImageFocus(images.buttonFocus2);
			buttons[6]->setLabel("Game Settings");	
			buttons[6]->setColor(IMAGE_COLOR);
			buttons[6]->setIndex(5);
			
			// Donate Button 
			ypos+=40;
			buttons[9]=new Button();
			buttons[9]->setX(440);
			buttons[9]->setY(ypos);
			buttons[9]->setImageNormal(images.button2);
			buttons[9]->setImageFocus(images.buttonFocus2);
			buttons[9]->setLabel("Donate");	
			buttons[9]->setColor(IMAGE_COLOR);
			buttons[9]->setIndex(5);
					
			// Exit HBC Button 
			ypos=400;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=38;
			
			buttons[7]=new Button();
			buttons[7]->setX(440);
			buttons[7]->setY(ypos);
			buttons[7]->setImageNormal(images.button2);
			buttons[7]->setImageFocus(images.buttonFocus2);
			buttons[7]->setLabel("Exit to HBC");	
			buttons[7]->setColor(IMAGE_COLOR);
			buttons[7]->setIndex(7);
	 
			// Reset Wii Button 
			ypos+=40;
			buttons[8]=new Button();
			buttons[8]->setX(440);
			buttons[8]->setY(ypos);
			buttons[8]->setImageNormal(images.button2);
			buttons[8]->setImageFocus(images.buttonFocus2);
			buttons[8]->setLabel("Reset Wii");	
			buttons[8]->setColor(IMAGE_COLOR);
			buttons[8]->setIndex(8);
		}
		break;
		
		
		case stateLevelMenu:
		{
			int ypos=405;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Easy Button 
			buttons[0]=new Button();
			buttons[0]->setX(40);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Easy Play");
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(6);
			
			// Medium Button 
			buttons[1]=new Button();
			buttons[1]->setX(240);
			buttons[1]->setY(ypos);
			buttons[1]->setImageNormal(images.button2);
			buttons[1]->setImageFocus(images.buttonFocus2);
			buttons[1]->setLabel("Medium Play");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(0);
			
			// Hard Button 
			buttons[2]=new Button();
			buttons[2]->setX(440);
			buttons[2]->setY(ypos);
			buttons[2]->setImageNormal(images.button2);
			buttons[2]->setImageFocus(images.buttonFocus2);
			buttons[2]->setLabel("Hard Play");			
			buttons[2]->setColor(IMAGE_COLOR);
			buttons[2]->setIndex(1);
			
			// Back Button 
			ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			buttons[3]=new Button();
			buttons[3]->setX(240);
			buttons[3]->setY(ypos);
			buttons[3]->setImageNormal(images.button2);
			buttons[3]->setImageFocus(images.buttonFocus2);
			buttons[3]->setLabel("Back" );
			buttons[3]->setColor(IMAGE_COLOR);
			buttons[3]->setIndex(6);
		}
		break;
		
		case stateMapSelectMenu:
		{	
			int ypos=255;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Map1 Button
   		buttons[0]=new Button();
			buttons[0]->setX(40);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Map1");
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
						
			// Map2 Button 
			buttons[1]=new Button();
			buttons[1]->setX(240);
			buttons[1]->setY(ypos);
			buttons[1]->setImageNormal(images.button2);
			buttons[1]->setImageFocus(images.buttonFocus2);
			buttons[1]->setLabel("Map2");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);
				
			// Map3 Button 
			buttons[2]=new Button();
			buttons[2]->setX(440);
			buttons[2]->setY(ypos);
			buttons[2]->setImageNormal(images.button2);
			buttons[2]->setImageFocus(images.buttonFocus2);
			buttons[2]->setLabel("Map3");
			buttons[2]->setColor(IMAGE_COLOR);
			buttons[2]->setIndex(2);
			
			ypos+=150;
			// Map4 Button 
			buttons[3]=new Button();
			buttons[3]->setX(40);
			buttons[3]->setY(ypos);
			buttons[3]->setImageNormal(images.button2);
			buttons[3]->setImageFocus(images.buttonFocus2);
			buttons[3]->setLabel("Map4");
			buttons[3]->setColor(IMAGE_COLOR);
			buttons[3]->setIndex(3);
			
			// Map5 Button 
			buttons[4]=new Button();
			buttons[4]->setX(240);
			buttons[4]->setY(ypos);
			buttons[4]->setImageNormal(images.button2);
			buttons[4]->setImageFocus(images.buttonFocus2);
			buttons[4]->setLabel("Map5");
			buttons[4]->setColor(IMAGE_COLOR);
			buttons[4]->setIndex(4);
			
			// Map6 Button
			buttons[5]=new Button();
			buttons[5]->setX(440);
			buttons[5]->setY(ypos);
			buttons[5]->setImageNormal(images.button2);
			buttons[5]->setImageFocus(images.buttonFocus2);
			buttons[5]->setLabel("Map6");
			buttons[5]->setColor(IMAGE_COLOR);
			buttons[5]->setIndex(5);
			
			ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Back Button 
			buttons[6]=new Button();
			buttons[6]->setX(240);
			buttons[6]->setY(ypos);
			buttons[6]->setImageNormal(images.button2);
			buttons[6]->setImageFocus(images.buttonFocus2);
			buttons[6]->setLabel("Back" );
			buttons[6]->setColor(IMAGE_COLOR);
			buttons[6]->setIndex(6);
		}
		break;	
				
		case stateLocalHighScore:
	    {
			int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Next Button
			buttons[0]=new Button();
			buttons[0]->setX(225);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Next");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
			
			// Scrollbar button 
			buttons[1]=new Button();
			buttons[1]->setX(SCROLLBAR_x);
			buttons[1]->setY(SCROLLBAR_Y_MIN);
			buttons[1]->setImageNormal(images.scrollbar);
			buttons[1]->setImageFocus(images.scrollbar);
			buttons[1]->setLabel("");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);
		}
		break;
		
		case stateTodayHighScore:
	   {
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Next Button
			buttons[0]=new Button();
			buttons[0]->setX(225);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Next");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
			
			// Scrollbar button 
			buttons[1]=new Button();
			buttons[1]->setX(SCROLLBAR_x);
			buttons[1]->setY(SCROLLBAR_Y_MIN);
			buttons[1]->setImageNormal(images.scrollbar);
			buttons[1]->setImageFocus(images.scrollbar);
			buttons[1]->setLabel("");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);
		}
		break;
		
		case stateGlobalHighScore:
	   {
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Next Button
			buttons[0]=new Button();
			buttons[0]->setX(225);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
			
			// Scrollbar button 
			buttons[1]=new Button();
			buttons[1]->setX(SCROLLBAR_x);
			buttons[1]->setY(SCROLLBAR_Y_MIN);
			buttons[1]->setImageNormal(images.scrollbar);
			buttons[1]->setImageFocus(images.scrollbar);
			buttons[1]->setLabel("");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);
		}
		break;
		
		case stateHelp1:
		{
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Next Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Next");	
			buttons[0]->setColor(IMAGE_COLOR);	
			buttons[0]->setIndex(0);
		}
		break;
		
		case stateHelp2:
		{
	    	int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Next Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Next");	
			buttons[0]->setColor(IMAGE_COLOR);	
			buttons[0]->setIndex(0);
		}
		break;
		
		case stateHelp3:
		{
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Next Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Next");	
			buttons[0]->setColor(IMAGE_COLOR);	
			buttons[0]->setIndex(0);
		}
		break;
		
		case stateHelp4:
		{
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Main Menu Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");	
			buttons[0]->setColor(IMAGE_COLOR);	
			buttons[0]->setIndex(0);
		}
		break;
		
		case stateCredits:
		{
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Main Menu Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
		}
		break;

		case stateDonate:
		{
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Main Menu Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
		}
		break;
		
		
		case stateReleaseNotes:
		{
		   int ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Main Menu Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");		
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
			
			// Scrollbar button 
			buttons[1]=new Button();
			buttons[1]->setX(SCROLLBAR_x);
			buttons[1]->setY(SCROLLBAR_Y_MIN);
			buttons[1]->setImageNormal(images.scrollbar);
			buttons[1]->setImageFocus(images.scrollbar);
			buttons[1]->setLabel("");
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);
		}
		break;
		
		case stateSoundSettings:
		{
			int ypos=195;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=35; 
	
			// Music Volume - button 
			buttons[1]=new Button();
			buttons[1]->setX(20);
			buttons[1]->setY(ypos);
			buttons[1]->setImageNormal(images.button1);
			buttons[1]->setImageFocus(images.buttonFocus1);
			buttons[1]->setLabel("-");	
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);
			
			// Music Volume + button 
			buttons[2]=new Button();
			buttons[2]->setX(540);
			buttons[2]->setY(ypos);
			buttons[2]->setImageNormal(images.button1);
			buttons[2]->setImageFocus(images.buttonFocus1);
			buttons[2]->setLabel("+");	
			buttons[2]->setColor(IMAGE_COLOR);
			buttons[2]->setIndex(2);
			
			ypos+=105;
			// Effect Volume - button 
			buttons[3]=new Button();
			buttons[3]->setX(20);
			buttons[3]->setY(ypos);
			buttons[3]->setImageNormal(images.button1);
			buttons[3]->setImageFocus(images.buttonFocus1);
			buttons[3]->setLabel("-");	
			buttons[3]->setColor(IMAGE_COLOR);
			buttons[3]->setIndex(3);
			
			// Effect Volume + button 
			buttons[4]=new Button();
			buttons[4]->setX(540);
			buttons[4]->setY(ypos);
			buttons[4]->setImageNormal(images.button1);
			buttons[4]->setImageFocus(images.buttonFocus1);
			buttons[4]->setLabel("+");	
			buttons[4]->setColor(IMAGE_COLOR);
			buttons[4]->setIndex(4);
			
			ypos+=80;
			// Music track - button 
			buttons[5]=new Button();
			buttons[5]->setX(140);
			buttons[5]->setY(ypos);
			buttons[5]->setImageNormal(images.button1);
			buttons[5]->setImageFocus(images.buttonFocus1);
			buttons[5]->setLabel("-");	
			buttons[5]->setColor(IMAGE_COLOR);
			buttons[5]->setIndex(5);
		
			// Music track + button 
			buttons[6]=new Button();
			buttons[6]->setX(420);
			buttons[6]->setY(ypos);
			buttons[6]->setImageNormal(images.button1);
			buttons[6]->setImageFocus(images.buttonFocus1);
			buttons[6]->setLabel("+");	
			buttons[6]->setColor(IMAGE_COLOR);
			buttons[6]->setIndex(6);
			
			ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Main Menu Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
		}
		break;
				
		case stateGameSettings:
	    {
			int xpos=35;			
			int ypos=150;
			
			if (rmode->xfbHeight!=MAX_VERT_PIXELS) ypos+=15; 
			
			// First letter + button 
			buttons[1]=new Button();
			buttons[1]->setX(xpos);
			buttons[1]->setY(ypos);
			buttons[1]->setImageNormal(images.button1);
			buttons[1]->setImageFocus(images.buttonFocus1);
			buttons[1]->setLabel("+");	
			buttons[1]->setColor(IMAGE_COLOR);
			buttons[1]->setIndex(1);

			// First letter - button 
			buttons[2]=new Button();
			buttons[2]->setX(xpos);
			buttons[2]->setY(ypos+145);
			buttons[2]->setImageNormal(images.button1);
			buttons[2]->setImageFocus(images.buttonFocus1);
			buttons[2]->setLabel("-");	
			buttons[2]->setColor(IMAGE_COLOR);
			buttons[2]->setIndex(2);

			// Second letter + button 
			xpos+=95;
			buttons[3]=new Button();
			buttons[3]->setX(xpos);
			buttons[3]->setY(ypos);
			buttons[3]->setImageNormal(images.button1);
			buttons[3]->setImageFocus(images.buttonFocus1);
			buttons[3]->setLabel("+");	
			buttons[3]->setColor(IMAGE_COLOR);
			buttons[3]->setIndex(3);

			// second letter - button 
			buttons[4]=new Button();
			buttons[4]->setX(xpos);
			buttons[4]->setY(ypos+145);
			buttons[4]->setImageNormal(images.button1);
			buttons[4]->setImageFocus(images.buttonFocus1);
			buttons[4]->setLabel("-");	
			buttons[4]->setColor(IMAGE_COLOR);
			buttons[4]->setIndex(4);

			// Third letter + button 
			xpos+=95;
			buttons[5]=new Button();
			buttons[5]->setX(xpos);
			buttons[5]->setY(ypos);
			buttons[5]->setImageNormal(images.button1);
			buttons[5]->setImageFocus(images.buttonFocus1);
			buttons[5]->setLabel("+");	
			buttons[5]->setColor(IMAGE_COLOR);
			buttons[5]->setIndex(5);

			// Third letter - button 
			buttons[6]=new Button();
			buttons[6]->setX(xpos);
			buttons[6]->setY(ypos+145);
			buttons[6]->setImageNormal(images.button1);
			buttons[6]->setImageFocus(images.buttonFocus1);
			buttons[6]->setLabel("-");
			buttons[6]->setColor(IMAGE_COLOR);		
			buttons[6]->setIndex(6);

			// Fourth letter + button 
			xpos+=95;
			buttons[7]=new Button();
			buttons[7]->setX(xpos);
			buttons[7]->setY(ypos);
			buttons[7]->setImageNormal(images.button1);
			buttons[7]->setImageFocus(images.buttonFocus1);
			buttons[7]->setLabel("+");	
			buttons[7]->setColor(IMAGE_COLOR);
			buttons[7]->setIndex(7);

			// Fourth letter - button 
			buttons[8]=new Button();
			buttons[8]->setX(xpos);
			buttons[8]->setY(ypos+145);
			buttons[8]->setImageNormal(images.button1);
			buttons[8]->setImageFocus(images.buttonFocus1);
			buttons[8]->setLabel("-");
			buttons[8]->setColor(IMAGE_COLOR);		
			buttons[8]->setIndex(8);
			
			// Fifth letter + button 
			xpos+=95;
			buttons[9]=new Button();
			buttons[9]->setX(xpos);
			buttons[9]->setY(ypos);
			buttons[9]->setImageNormal(images.button1);
			buttons[9]->setImageFocus(images.buttonFocus1);
			buttons[9]->setLabel("+");	
			buttons[9]->setColor(IMAGE_COLOR);
			buttons[9]->setIndex(9);

			// Fifth letter - button 
			buttons[10]=new Button();
			buttons[10]->setX(xpos);
			buttons[10]->setY(ypos+145);
			buttons[10]->setImageNormal(images.button1);
			buttons[10]->setImageFocus(images.buttonFocus1);
			buttons[10]->setLabel("-");
			buttons[10]->setColor(IMAGE_COLOR);	
			buttons[10]->setIndex(10);
			
			// Sixth letter + button 
			xpos+=95;
			buttons[11]=new Button();
			buttons[11]->setX(xpos);
			buttons[11]->setY(ypos);
			buttons[11]->setImageNormal(images.button1);
			buttons[11]->setImageFocus(images.buttonFocus1);
			buttons[11]->setLabel("+");	
			buttons[11]->setColor(IMAGE_COLOR);
			buttons[11]->setIndex(11);

			// Sixth letter - button 
			buttons[12]=new Button();
			buttons[12]->setX(xpos);
			buttons[12]->setY(ypos+145);
			buttons[12]->setImageNormal(images.button1);
			buttons[12]->setImageFocus(images.buttonFocus1);
			buttons[12]->setLabel("-");
			buttons[12]->setColor(IMAGE_COLOR);
			buttons[12]->setIndex(12);
			
			// Classic Sprites Button
			buttons[13]=new Button();
			buttons[13]->setX(60);
			buttons[13]->setY(ypos+240);
			buttons[13]->setImageNormal(images.button2);
			buttons[13]->setImageFocus(images.buttonFocus2);
			
			if (settings->getClassicSprites())
			{
				buttons[13]->setLabel("Enabled");
			}
			else
			{
				buttons[13]->setLabel("Disabled");
			} 	
			buttons[13]->setColor(IMAGE_COLOR);
			buttons[13]->setIndex(0);
			
			ypos=460;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
			// Main Menu Button
			buttons[0]=new Button();
			buttons[0]->setX(240);
			buttons[0]->setY(ypos);
			buttons[0]->setImageNormal(images.button2);
			buttons[0]->setImageFocus(images.buttonFocus2);
			buttons[0]->setLabel("Main Menu");	
			buttons[0]->setColor(IMAGE_COLOR);
			buttons[0]->setIndex(0);
		}
		break;	
		
		case stateGame:
	    {									
			int ypos=55;
			
			// New Wave launch Button
			buttons[0]=new Button();
			buttons[0]->setX(10+game.panelXOffset);
			buttons[0]->setY(ypos+game.panelYOffset);
			buttons[0]->setImageNormal(images.button3);
			buttons[0]->setImageFocus(images.buttonFocus3);
			buttons[0]->setLabel("");
			buttons[0]->setColor(IMAGE_COLOR3);
			buttons[0]->setIndex(0);
			
			// Power Upgrade Button
			ypos+=155;
			buttons[1]=new Button();
			buttons[1]->setX(10+game.panelXOffset);
			buttons[1]->setY(ypos+game.panelYOffset);
			buttons[1]->setImageNormal(images.button3);
			buttons[1]->setImageFocus(images.buttonFocus3);
			buttons[1]->setLabel("");
			buttons[1]->setColor(IMAGE_COLOR3);
			buttons[1]->setIndex(1);
			
			// Range Upgrade Button
			ypos+=45;
			buttons[2]=new Button();
			buttons[2]->setX(10+game.panelXOffset);
			buttons[2]->setY(ypos+game.panelYOffset);
			buttons[2]->setImageNormal(images.button3);
			buttons[2]->setImageFocus(images.buttonFocus3);
			buttons[2]->setLabel("");
			buttons[2]->setColor(IMAGE_COLOR3);
			buttons[2]->setIndex(2);

			// Rate Upgrade Button
			ypos+=45;
			buttons[3]=new Button();
			buttons[3]->setX(10+game.panelXOffset);
			buttons[3]->setY(ypos+game.panelYOffset);
			buttons[3]->setImageNormal(images.button3);
			buttons[3]->setImageFocus(images.buttonFocus3);
			buttons[3]->setLabel("");
			buttons[3]->setColor(IMAGE_COLOR3);
			buttons[3]->setIndex(3);
								
			// Select previous weapon Button
			ypos+=45;
			buttons[4]=new Button();
			buttons[4]->setX(10+game.panelXOffset);
			buttons[4]->setY(ypos+game.panelYOffset);
			buttons[4]->setImageNormal(images.button4);
			buttons[4]->setImageFocus(images.buttonFocus4);
			buttons[4]->setLabel("<");
			buttons[4]->setColor(IMAGE_COLOR3);
			buttons[4]->setIndex(4);

			// Select next weapon Button
			buttons[5]=new Button();
			buttons[5]->setX(66+game.panelXOffset);
			buttons[5]->setY(ypos+game.panelYOffset);
			buttons[5]->setImageNormal(images.button4);
			buttons[5]->setImageFocus(images.buttonFocus4);
			buttons[5]->setLabel(">");
			buttons[5]->setColor(IMAGE_COLOR3);
			buttons[5]->setIndex(5);
			
			// Select weapon
			buttons[6]=new Button();
			buttons[6]->setX(33+game.panelXOffset);
			buttons[6]->setY(ypos+game.panelYOffset);
			buttons[6]->setImageNormal(weaponSpecs->getImageSpecial(game.weaponType));
			buttons[6]->setImageFocus(weaponSpecs->getImageSpecial(game.weaponType));
			buttons[6]->setLabel("");			
			buttons[6]->setColor(IMAGE_COLOR);
			buttons[6]->setIndex(6);
		}
		break;
	
		case stateQuitGame:	
		{
			// Yes button 
			buttons[0]=new Button();
			buttons[0]->setX(230);
			buttons[0]->setY(270);
			buttons[0]->setImageNormal(images.button3);
			buttons[0]->setImageFocus(images.buttonFocus3);
			buttons[0]->setLabel("YES");	
			buttons[0]->setColor(IMAGE_COLOR3);
			buttons[0]->setIndex(0);
			
			// No button 
			buttons[1]=new Button();
			buttons[1]->setX(330);
			buttons[1]->setY(270);
			buttons[1]->setImageNormal(images.button3);
			buttons[1]->setImageFocus(images.buttonFocus3);
			buttons[1]->setLabel("NO");	
			buttons[1]->setColor(IMAGE_COLOR3);		
			buttons[1]->setIndex(1);
		}
		break;
		
		case stateGameOver:	
		{
			// Retry button 
			buttons[0]=new Button();
			buttons[0]->setX(230);
			buttons[0]->setY(270);
			buttons[0]->setImageNormal(images.button3);
			buttons[0]->setImageFocus(images.buttonFocus3);
			buttons[0]->setLabel("RETRY" );	
			buttons[0]->setColor(IMAGE_COLOR3);
			buttons[0]->setIndex(0);
			
			// Quit button 
			buttons[1]=new Button();
			buttons[1]->setX(330);
			buttons[1]->setY(270);
			buttons[1]->setImageNormal(images.button3);
			buttons[1]->setImageFocus(images.buttonFocus3);
			buttons[1]->setLabel("QUIT");	
			buttons[1]->setColor(IMAGE_COLOR3);		
			buttons[1]->setIndex(1);
		}
		break;
	}
	trace->event(s_fn,0,"leave [void]");
}

/**
 * Init Network Module
 */
void initNetwork(void)
{ 
   const char *s_fn="initNetwork";
   trace->event(s_fn,0,"enter");
   
   char userData1[MAX_LEN];
   char userData2[MAX_LEN];

   // Set userData1
   memset(userData1,0x00, MAX_LEN);
   sprintf(userData1,"%s=%s",PROGRAM_NAME,PROGRAM_VERSION);
		
   // Get userData2 
   memset(userData2,0x00, MAX_LEN);
   sprintf(userData2,"appl=%s",PROGRAM_NAME);
	 
   tcp_init_layer();
    
   tcp_start_thread(PROGRAM_NAME, PROGRAM_VERSION, 
			ID1, URL1, 
			ID2, URL2, 
			ID3, URL3, 
			ID4, URL4, 
			URL_TOKEN, userData1, userData2);
   
   trace->event(s_fn,0,"leave [void]");
}

/** 
  * Init game parameters
  * @param wave	The wave number
  */
void initGame(int wave)
{	
	// Init game variables
	game.score=0;
	game.cash=matrix->getStartCash();							  
	game.selectedWeapon=-1;
	game.selectedNewWeapon=false;	
	game.weaponType=0;
	
	game.wave=wave;
	matrix->setWave(game.wave);
	
	game.monsterInBase=0;
	
	// Panel position
	game.panelXOffset=20;
	game.panelYOffset=0;
	
	// Reset weapon type counter
	weaponSpecs->resetCounter();
	
	// Show New Wave text on screen
	game.alfa=MAX_ALFA;     
	
	// Cleanup previous game variables, if any....
	destroyMonsters();
	destroyWeapons();
	
	// Init build grid
	if ((game.selectedMap!=-1) && (grids[game.selectedMap]!=NULL))
	{
		grids[game.selectedMap]->initBuild();
	}
	
	// Launch first monster wave
	game.event=eventLaunch;
}		

/** 
 * Init application parameters
 */
void initApplication(void)
{
	const char *s_fn="initApplication";
	
	// Open trace module
	trace = new Trace();
	trace->open(TRACE_FILENAME);
	trace->event(s_fn,0,"enter");
	trace->event(s_fn, 0,"%s %s Started", PROGRAM_NAME, PROGRAM_VERSION);

	game.stateMachine=stateIntro1;
	game.prevStateMachine=stateNone;
	game.event=eventNone;
	game.wave1 = 0;
	game.wave2 = 0;
	game.panelXOffset = 20;
	game.angle = 0;
	game.alfa = 0;
		
   // Init Images
	initImages();
   
   // Init pointers
   initPointers();

	// Load Settings from SDCard	
	settings = new Settings();
	settings->load(SETTING_FILENAME);
	
	// Load Local Highscore from SDCard
	highScore = new HighScore();
	highScore->load(HIGHSCORE_FILENAME);
	
	// Init Sound (Start play first mod file)
	sound = new Sound();
	sound->setMusicVolume(settings->getMusicVolume());
	sound->setEffectVolume(settings->getEffectVolume());	
	
	// Init weapons Specifications
	weaponSpecs = new WeaponSpecs();

	// Init monster Specifications
	monsterSpecs = new MonsterSpecs();
	
	// Init matrix
	matrix = new Matrix();
	
	// Init Today HighScore
	initTodayHighScore();
	
	// Init Global HighScore
	initGlobalHighScore();
	
	// Init network Thread
	initNetwork();
			
	trace->event(s_fn,0,"leave");
}
				
// -----------------------------------
// DRAW METHODES
// -----------------------------------

/**
 * Draw grid on screen
 */
void drawGrid(void)
{
	if ((game.selectedMap!=-1) && (grids[game.selectedMap]!=NULL))
	{
		grids[game.selectedMap]->draw(0,0,1.0);   
	}				
}

/** 
 * Draw pointers on screen
 */
void drawPointers(void)
{
   for( int i=0; i<MAX_POINTERS; i++ ) 
   {
	  if (pointers[i]!=NULL) 
	  {
			pointers[i]->action();
			pointers[i]->draw();
	  }
   }
}

/*
** drawMonsters.
**
** Draw monsters on screen
**
** @Input: 
**    special  false   drawMonster in selected grid
** 			   true    drawMonster on several grids concurrent
*/
void drawMonsters(bool special)
{
	for( int i=0; i<MAX_MONSTERS; i++ ) 
	{
		if (monsters[i]!=NULL)
		{
			if (special)
			{
			   int ypos=140;
	         if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
				// Draw mini size monsters
				switch (monsters[i]->getGrid())
				{
					case 0: 	monsters[i]->draw(55,ypos,5.0);
								break;
							
					case 1: 	monsters[i]->draw(255,ypos,5.0);
								break;
							
					case 2: 	monsters[i]->draw(455,ypos,5.0);
								break;
							
					case 3: 	monsters[i]->draw(55,ypos+150,5.0);
								break;
							
					case 4: 	monsters[i]->draw(255,ypos+150,5.0);
								break;
							
					case 5: 	monsters[i]->draw(455,ypos+150,5.0);
								break;
				}			
			}
			else
			{
				// Draw normal size monsters
				monsters[i]->draw(0,0,1);
			}
		}
	}
}

/** 
 * Draw monsters Text on screen
 */
void drawMonstersText(void)
{
   for( int i=0; i<MAX_MONSTERS; i++ ) 
   {
		if (monsters[i]!=NULL)
		{
			monsters[i]->text();
		}
   }
}

/** 
 * Draw weapons on screen
 */
void drawWeapons(void)
{
	for( int i=0; i<MAX_WEAPONS; i++ ) 
	{
		if (weapons[i]!=NULL)
		{
			weapons[i]->draw();
		}
	}
}

/** 
 * Draw buttons on screen
 */
void drawButtons(void)
{
	for( int i=0; i<MAX_BUTTONS; i++ ) 
	{
		if (buttons[i]!=NULL)
		{
			buttons[i]->draw();
		}
	}
}

/** 
 * Draw buttons Text on screen
 */
void drawButtonsText(int offset)
{
	for( int i=0; i<MAX_BUTTONS; i++ ) 
	{
		if (buttons[i]!=NULL)
		{
			buttons[i]->text(offset);
		}
	}
}

/** 
 * Draw text on screen
 * @param x		The x location of the text.
 * @param y		The y location of the text.
 * @param type	The font type.
 * @param text The text
 * @param ...	Optional parameters
 *
 */
void drawText(int x, int y, int type, const char *text, ...)
{
	char buf[MAX_LEN];
	memset(buf,0x00,sizeof(buf));
   
	if (text!=NULL)
	{    		
		// Expend event string
		va_list list;
		va_start(list, text );
		vsprintf(buf, text, list);
   	 
		switch (type)
		{  	
			case fontWelcome: 
				GRRLIB_PrintfTTF(x, y, myFont, buf, 40, GRRLIB_WHITESMOKE); 
				break;
 
			case fontTitle: 
				if (x==0) x=320-((strlen(buf)*34)/2);  
				GRRLIB_PrintfTTF(x, y, myFont, buf, 72, GRRLIB_WHITESMOKE); 
				break;
  	   
			case fontSubTitle:
				if (x==0) x=320-((strlen(buf)*20)/2);        
				GRRLIB_PrintfTTF(x, y, myFont, buf, 30, GRRLIB_WHITESMOKE); 
				break;
	   
			case fontPanel:         
				GRRLIB_PrintfTTF(x, y, myFont, buf, 14, GRRLIB_WHITESMOKE); 
				break;
	   	   
			case fontParagraph:
				if (x==0) x=320-((strlen(buf)*10)/2);	             
				GRRLIB_PrintfTTF(x, y, myFont, buf, 24, GRRLIB_WHITESMOKE); 
				break;
	   	   
			case fontNormal:
				if (x==0) x=320-((strlen(buf)*7)/2);           
				GRRLIB_PrintfTTF(x, y, myFont, buf, 18, GRRLIB_WHITESMOKE); 
				break;
	         
			case fontNew:
				if (x==0) x=320-((strlen(buf)*8)/2);	   
				GRRLIB_PrintfTTF(x, y, myFont, buf, 22, GRRLIB_WHITESMOKE); 
				break;
	   
			case fontSmall:
				if (x==0) x=320-((strlen(buf)*10)/2); 
			   GRRLIB_PrintfTTF(x, y, myFont, buf, 10, GRRLIB_WHITESMOKE);          
				break;
	   
			case fontButton:
				if (strlen(buf)==1)
				{           
					GRRLIB_PrintfTTF(x+35, y, myFont, buf, 24, GRRLIB_WHITESMOKE); 
				}
				else
				{  
					GRRLIB_PrintfTTF(x+20, y, myFont, buf, 24, GRRLIB_WHITESMOKE); 
				}		   
				break;
				
			case fontBanner:
				GRRLIB_PrintfTTF(x, y, myFont, buf, 80, GRRLIB_RED); 
				break;
			
		}
	}
}

/** 
 * Draw game panel1
 */
void drawPanel1(void)
{
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset;
	
	// Draw panel
	GRRLIB_Rectangle(xpos, ypos, 100, 90, GRRLIB_BLACK_TRANS, 1);
}

/** 
 * Draw game panel2
 */
void drawPanel2(void)
{
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset+95;

	// Draw panel
	GRRLIB_Rectangle(xpos, ypos, 100, 70, GRRLIB_BLACK_TRANS, 1);
}

/** 
 * Draw game panel3
 */
void drawPanel3(void)
{
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset+170;

	// Draw panel
	GRRLIB_Rectangle(xpos, ypos, 100, 210, GRRLIB_BLACK_TRANS, 1);
}

/** 
 * Draw game panel4
 */
void drawPanel4(void)
{
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset+385;

	// Draw panel
	GRRLIB_Rectangle(xpos, ypos, 100, 67, GRRLIB_BLACK_TRANS, 1);
}
				
/** 
  * Draw wave text panel on screen
  */
void drawPanelText1(void)
{
	char tmp[MAX_LEN];
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset;

	// General info + control
	ypos+=7;	
	drawText(xpos+30,ypos,fontPanel,"WAVE");	
	
	ypos+=12;
	drawText(xpos+35,ypos,fontPanel,"%03d", game.wave);	
	
	ypos+=20;
	drawText(xpos+20,ypos,fontPanel,"LAUNCH");
	
	// Set button label values
	sprintf(tmp,"    %03d", game.waveCountDown/25 );
	if (buttons[0]!=NULL) buttons[0]->setLabel(tmp);	
}


/** 
 * Draw score/cash text panel on screen
 */
void drawPanelText2(void)
{
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset+95;
	
	// Score info
	ypos+=5;
	drawText(xpos+28,ypos,fontPanel,"SCORE");
	
	ypos+=12;	
	drawText(xpos+20,ypos,fontPanel,"%07d", game.score);
	
	// Cash info
	ypos+=17;
	drawText(xpos+30,ypos,fontPanel,"CASH");
	
	ypos+=12;
	drawText(xpos+20,ypos,fontPanel,"$%06d", game.cash);
}


/**
 * Draw Weapon upgrade/build text game panel on screen
 */
void drawPanelText3(void)
{
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset+170;
	
	char power[MAX_LEN];
	char range[MAX_LEN];
	char rate[MAX_LEN];
	
	// Upgrade information + control
	ypos+=5;
	drawText(xpos+18,ypos,fontPanel,"UPGRADE");

	ypos+=18;
	drawText(xpos+25,ypos,fontPanel,"POWER");
	
	ypos+=45;
	drawText(xpos+25,ypos,fontPanel,"RANGE");
	
	ypos+=45;
	drawText(xpos+32,ypos,fontPanel,"RATE");
	
	// Build information + control
	ypos+=45;
	drawText(xpos+30,ypos,fontPanel,"BUILD");
		
	if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
	{
		// Get upgrade prices of selected weapon.
		if (weapons[game.selectedWeapon]->isPowerUpgradeble())
		{
			sprintf(power,"$%d", weapons[game.selectedWeapon]->getPowerPrice() );		
		}
		else
		{
			sprintf(power,"MAX");
		}
		
		if (weapons[game.selectedWeapon]->isRangeUpgradeble())
		{
			sprintf(range,"$%d", weapons[game.selectedWeapon]->getRangePrice() );		
		}
		else
		{
			sprintf(range,"MAX");
		}
		
		if (weapons[game.selectedWeapon]->isRateUpgradeble())
		{
			sprintf(rate,"$%d", weapons[game.selectedWeapon]->getRatePrice() );
		}
		else
		{
			sprintf(rate,"MAX");
		}
	}
	else
	{
		// No weapon selected yet!
		strcpy(power,"");
		strcpy(range,"");
		strcpy(rate,"");
	}
		
	if (buttons[1]!=NULL) buttons[1]->setLabel(power);
	if (buttons[2]!=NULL) buttons[2]->setLabel(range);
	if (buttons[3]!=NULL) buttons[3]->setLabel(rate);
	
	// Set build button color depending on amount of cash
	if (buttons[6]!=NULL)
	{ 
		if	(game.cash<weaponSpecs->getPrice(game.weaponType))
		{
			// Weapon Transparent (Disable build)
			buttons[6]->setColor(IMAGE_COLOR3);
		}
		else
		{
			// Weapon Normal (Enable build)
			buttons[6]->setColor(IMAGE_COLOR);
		}
	}
}

/**
 * Draw Weapon information
 */
void drawPanelText4(void)
{	
	int xpos = game.panelXOffset;
	int ypos = game.panelYOffset+385;
	
	ypos+=4;
			
	if ( (game.selectedWeapon!=-1) && 
		  (weapons[game.selectedWeapon]!=NULL) )
	{
		drawText(xpos+8,ypos,fontPanel,"%s",
			weapons[game.selectedWeapon]->getName());
		
		ypos+=15;
		drawText(xpos+8,ypos,fontPanel, "Power");

		drawText(xpos+52,ypos+3,fontSmall,"%03d-%03d",
			weapons[game.selectedWeapon]->getPower(),
			weapons[game.selectedWeapon]->getMaxPower());
			
		ypos+=12;
		drawText(xpos+8,ypos,fontPanel, "Range");

		drawText(xpos+52,ypos+3,fontSmall, "%03d-%03d",
			weapons[game.selectedWeapon]->getRange(),
			weapons[game.selectedWeapon]->getMaxRange());
			
		ypos+=12;
		drawText(xpos+8,ypos,fontPanel, "Rate");
		
		drawText(xpos+52,ypos+3,fontSmall, "%03d-%03d",
			weapons[game.selectedWeapon]->getRate(),
			weapons[game.selectedWeapon]->getMaxRate());
	}
	else
	{	
		drawText(xpos+8,ypos,fontPanel,"%s $%d", 
			weaponSpecs->getName(game.weaponType), 
			weaponSpecs->getPrice(game.weaponType));
			
		ypos+=15;
		drawText(xpos+8,ypos,fontPanel, "Power");
				
		drawText(xpos+52,ypos+3,fontSmall, "%03d-%03d",
			weaponSpecs->getMinPower(game.weaponType),
			weaponSpecs->getMaxPower(game.weaponType));
			
		ypos+=12;
		drawText(xpos+8,ypos,fontPanel, "Range");
		
		drawText(xpos+52,ypos+3,fontSmall, "%03d-%03d",
			weaponSpecs->getMinRange(game.weaponType),
			weaponSpecs->getMaxRange(game.weaponType));
			
		ypos+=12;
		drawText(xpos+8,ypos,fontPanel, "Rate");
		
		drawText(xpos+52,ypos+3,fontSmall, "%03d-%03d",
			weaponSpecs->getMinRate(game.weaponType),
			weaponSpecs->getMaxRate(game.weaponType));
	}
}

/**
 * draw screens
 */
void drawScreen(void)
{ 	   	
	char tmp[MAX_LEN];
	
	int  ypos;
	if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos=10; else ypos=25;
		  
   switch( game.stateMachine )	
	{		   
	   case stateIntro1:
	   { 		
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR );
		  
		   // Draw game logo
		   GRRLIB_DrawImg(320, (rmode->xfbHeight/2), 
				images.logo1, 0, game.size, game.size, IMAGE_COLOR );
		   if (game.size<=MAX_SIZE) game.size+=0.05;
		  
			drawText(0, ypos, fontParagraph,  "Created by wplaat"  );
			ypos+=20;
			drawText(0, ypos, fontParagraph,  "http://www.plaatsoft.nl"  );
			
			ypos+=380;
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=28;
			
			drawText(40, ypos, fontNormal,  "This software is open source and may be copied, distributed or modified"  );
			ypos+=20;
			drawText(60, ypos, fontNormal,  "under the terms of the GNU General Public License (GPL) version 2" );

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }	   
	   break;
	   
	   case stateIntro2:
	   {
			unsigned int j;
		  
	      // Draw background
			GRRLIB_DrawImg(0,0, images.background2, 0, 1, 1, IMAGE_COLOR );

			// Draw Plaatsoft logo		 
   	   for(j=0;j<images.logo->h;j++)
			{
            GRRLIB_DrawTile(
					((640-images.logo2->w)/2)+sin(game.wave1)*50, 
					(((480-images.logo2->h)/2)-50)+j, 
					images.logo, 0, 1, 1, IMAGE_COLOR, j );
            game.wave1+=0.02;
         }
			game.wave2+=0.02;
         game.wave1=game.wave2;
		  
			ypos+=320;
			drawText(0, ypos, fontParagraph,  "Please visit my website for more information." );
			ypos+=40;
			drawText(0, ypos, fontParagraph,  "http://www.plaatsoft.nl" );

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }	   
	   break;
	   	   
		case stateIntro3:
	   { 
	      int xpos, xpos2;
			
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR );

		   // Let the logo's slided in the screen
			if (game.location<640) game.location+=4;
			
			drawText(95, ypos, fontParagraph,  "Some more Wii games developed by PlaatSoft" );

			// Draw Space Bubble logo
			ypos+=40;
			xpos=(images.logo3->w*-1)+game.location;
			xpos2=40;
			if (xpos>xpos2) xpos=xpos2;
   	   GRRLIB_DrawImg(xpos, ypos, images.logo3, 	0, 0.9, 0.9, IMAGE_COLOR );		
				
			// Draw RedSquare logo
			ypos+=images.logo3->h-5;
			xpos=640-game.location;
			xpos2=635-images.logo4->w;
			if (xpos<xpos2) xpos=xpos2;			
   	   GRRLIB_DrawImg(xpos, ypos, images.logo4, 0, 0.9, 0.9, IMAGE_COLOR );
		
			// Draw BibleQuiz logo
			ypos+=images.logo5->h-10;
			xpos=(images.logo5->w*-1)+game.location;
			xpos2=30;
			if (xpos>xpos2) xpos=xpos2;		
   	   GRRLIB_DrawImg(xpos, ypos, images.logo5, 	0, 0.9, 0.9, IMAGE_COLOR );
			
			// Draw Pong2 logo
			ypos+=images.logo5->h-10;	
			xpos=640-game.location;
			xpos2=630-images.logo6->w;
			if (xpos<xpos2) xpos=xpos2;			
   	   GRRLIB_DrawImg(xpos, ypos, images.logo6, 0, 0.9, 0.9, IMAGE_COLOR );
				
			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }	   
	   break;
		
		case stateIntro4:
	   { 
	      int ypos=185;
			
			// Draw background
			GRRLIB_DrawImg(0,0, images.intro4, 0, 1, 1, IMAGE_COLOR );

			drawText(150, ypos, fontWelcome,  "WarQuest is available" );
			ypos+=45;
			drawText(0, ypos, fontParagraph,  "visit" );
			ypos+=35;
			ypos+=35;
			drawText(200, ypos, fontParagraph,  "http://www.warquest.nl" );
			ypos+=35;
			drawText(190, ypos, fontParagraph,  "and enter the battle zone!" );
						
			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }	   
	   break;
		
		case stateMainMenu:
		{
			char *version=NULL;

			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR );
		  
			// Draw Buttons
			drawButtons();
		  
			drawText(20, ypos, fontWelcome, "%s v%s", PROGRAM_NAME, PROGRAM_VERSION );
			ypos+=40;
			drawText(20, ypos, fontParagraph, RELEASE_DATE );
	
			version=tcp_get_version();
         if ( (version!=NULL) && (strlen(version)>0) && (strcmp(version,PROGRAM_VERSION)!=0) )
         {    
				ypos+=280;
	         drawText(20, ypos, fontNew, "New version [v%s] is available.",version);
				 	
				ypos+=20;	 			 
	         drawText(20, ypos, fontNew, "Check the release notes.");			 
         }  
		  	
			// Draw Button Text labels
			drawButtonsText(0);
						
			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		
		case stateLevelMenu:
		{
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR4 );
	  	
			// Draw samples maps
			ypos=140;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
	
			GRRLIB_Rectangle(65, ypos, 120, 255, 0x93ff93ff, 1);
			GRRLIB_Rectangle(265, ypos, 120, 255, 0xffff80ff, 1);
			GRRLIB_Rectangle(465, ypos, 120, 255, 0xff8040ff, 1);
			
			// Draw some roads
			GRRLIB_Rectangle(110, ypos, 32, 255, GRRLIB_GRAY, 1);
			GRRLIB_Line(110+3, ypos, 110+3, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(110+5, ypos, 110+5, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(110+32-3, ypos, 110+32-3, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(110+32-5, ypos, 110+32-5, ypos+255, GRRLIB_WHITESMOKE);

			GRRLIB_Rectangle(310, ypos, 32, 255, GRRLIB_GRAY, 1);
			GRRLIB_Line(310+3, ypos, 310+3, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(310+5, ypos, 310+5, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(310+32-3, ypos, 310+32-3, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(310+32-5, ypos, 310+32-5, ypos+255, GRRLIB_WHITESMOKE);
						
			GRRLIB_Rectangle(510, ypos, 32, 255, GRRLIB_GRAY, 1);
			GRRLIB_Line(510+3, ypos, 510+3, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(510+5, ypos, 510+5, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(510+32-3, ypos, 510+32-3, ypos+255, GRRLIB_WHITESMOKE);
			GRRLIB_Line(510+32-5, ypos, 510+32-5, ypos+255, GRRLIB_WHITESMOKE);
			
			// Calculate frame of enemy animation
			if (monsterSpecs->getImage(0)->h==32) 
			{
				// No animation available
				game.frame=0;
			}
			else
			{
				game.frame=2;
			}
			
			// Draw	some enemies
			GRRLIB_DrawTile( 110, ypos+10, monsterSpecs->getImage(0), 
				0, 1, 1, IMAGE_COLOR, game.frame );	
			GRRLIB_DrawTile( 110, ypos+50, monsterSpecs->getImage(3), 
				0, 1, 1, IMAGE_COLOR, game.frame );

			GRRLIB_DrawTile( 310, ypos+10, monsterSpecs->getImage(0),
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 310, ypos+50, monsterSpecs->getImage(3), 
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 310, ypos+90, monsterSpecs->getImage(6), 
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 310, ypos+130, monsterSpecs->getImage(9), 
				0, 1, 1, IMAGE_COLOR, game.frame );

			GRRLIB_DrawTile( 510, ypos+10, monsterSpecs->getImage(0), 
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 510, ypos+50, monsterSpecs->getImage(3), 
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 510, ypos+90, monsterSpecs->getImage(6), 
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 510, ypos+130, monsterSpecs->getImage(9), 
				0, 1, 1, IMAGE_COLOR, game.frame );
			GRRLIB_DrawTile( 510, ypos+170, monsterSpecs->getImage(12),
				0, 1, 1, IMAGE_COLOR,game.frame );
			GRRLIB_DrawTile( 510, ypos+210, monsterSpecs->getImage(15), 
				0, 1, 1, IMAGE_COLOR,game.frame );
			
			// Draw Buttons
			drawButtons();
		  		  
			// Draw title
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos=10; else ypos=25;
	      drawText(130, ypos, fontTitle, "Level Select");	

			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		
		case stateMapSelectMenu:
		{
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR4 );
  
			// Draw samples maps
			ypos=140;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
	
			if (grids[0]!=NULL) grids[0]->draw(55,ypos,5.0); 
			if (grids[1]!=NULL) grids[1]->draw(255,ypos,5.0); 
			if (grids[2]!=NULL) grids[2]->draw(455,ypos,5.0);
			ypos+=150;
			if (grids[3]!=NULL) grids[3]->draw(55,ypos,5.0); 
			if (grids[4]!=NULL) grids[4]->draw(255,ypos,5.0); 
			if (grids[5]!=NULL) grids[5]->draw(455,ypos,5.0);
		
			// Move elements
			moveMonsters();  
		  
			// Draw mini monsters on sample maps
			drawMonsters(true);
		
			// Draw Buttons
			drawButtons();
		  
			// Check Game parameters
			checkNextWave();
			
			// cleanup dead monsters
			checkDeadMonsters();
		  
			// Draw title
			if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos=10; else ypos=25;
	      drawText(120, ypos, fontTitle, "Choose Map");	

			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
	
		case stateGame:
		{		  
			// Move elements
			moveMonsters();
			moveWeapons();
		  
			// Draw elements
			drawGrid();		  
			drawMonsters(false);
			drawWeapons();
			drawPanel1();		  
			drawPanel2();	
			drawPanel3();
			drawPanel4();	
			drawButtons();
		  
			// Check for game events
			checkPointer();
			checkDeadMonsters();
			checkNextWave();
		   checkGameOver();
			
			// Show Next Wave text
			if (game.alfa>0)
			{
				drawText(180, 210, fontBanner, "WAVE %02d", game.wave);
				game.alfa-=5;
			}
		  
			// Draw Text elements
			drawMonstersText();
			drawPanelText1();
			drawPanelText2();
			drawPanelText3();
			drawPanelText4();
			drawButtonsText(-28);
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
				
		case stateGameOver:
		{	  
			// Draw elements
			drawGrid(); 
			drawMonsters(false);
			drawWeapons();
			drawPanel2();	
	      drawButtons();
			
			// Draw Transparent Box
			GRRLIB_Rectangle(210, 210, 220, 100, GRRLIB_BLACK_TRANS, 1);
			 
			// Draw text elements
			drawMonstersText();
			drawPanelText2();
			drawButtonsText(-20);
			  
			drawText(260, 220, fontParagraph, "Game Over!");
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		
		case stateQuitGame:
		{
			// Draw elements
			drawGrid(); 
			drawMonsters(false);
			drawWeapons();
			drawPanel2();	
			drawButtons();
	
			// Draw Transparent Box
			GRRLIB_Rectangle(210, 210, 220, 100, GRRLIB_BLACK_TRANS, 1);
 
			// Draw Text elements
			drawMonstersText();
			drawPanelText2();
			drawButtonsText(-10);
	
 	      drawText(0, 220, fontParagraph, "Quit game?");	
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		
		case stateLocalHighScore:
	   {
			struct tm *local;
			int startEntry;
			int endEntry;
		  		  
			if (highScore->getAmount()<15)
			{
				startEntry=0;
				endEntry=highScore->getAmount();
			}
			else
			{
				startEntry=(((float) highScore->getAmount()-15.0)/30.0)*(float)game.scrollIndex;
				endEntry=startEntry+15;
			}
				   
         // Draw background
         GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR2 );
		  
			// Draw scrollbar
			int y=SCROLLBAR_Y_MIN;
         GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollTop, 0, 1, 1, IMAGE_COLOR );
			for (int i=0; i<10; i++) 
			{
				y+=24;
				GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollMiddle, 0, 1, 1, IMAGE_COLOR );
			}
			y+=24;
			GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollBottom, 0, 1, 1, IMAGE_COLOR );
		  		  
			// Draw buttons
	      drawButtons(); 
		   
	      // Draw title
	      drawText(80, ypos, fontTitle, "Local High Score");	

         // Show Content
         ypos+=90;
			drawText(20, ypos, fontParagraph, "TOP"  );
	      drawText(80, ypos, fontParagraph, "DATE"  );
	      drawText(270, ypos, fontParagraph, "SCORE" );
			drawText(350, ypos, fontParagraph, "NAME"  );
			drawText(440, ypos, fontParagraph, "WAVE" );
			drawText(520, ypos, fontParagraph, "MAP" );
			ypos+=10;
		  
			for (int i=startEntry; i<endEntry; i++)
	      {
				// Only show highscore entries which contain data
				if (highScore->getDate(i)!=0)
				{
					ypos+=20;  
					drawText(20, ypos, fontNormal, "%02d", i+1);
			  
					local = localtime(highScore->getDate(i));
					sprintf(tmp,"%02d-%02d-%04d %02d:%02d:%02d", 
						local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
						local->tm_hour, local->tm_min, local->tm_sec);
					drawText(80, ypos, fontNormal, tmp);
					drawText(270, ypos, fontNormal, "%05d", highScore->getScore(i));
					drawText(350, ypos, fontNormal, highScore->getName(i));
					drawText(440, ypos, fontNormal, "%02d", highScore->getWave(i));
					drawText(520, ypos, fontNormal, "%02d", highScore->getMap(i));
				}
			}	
		  
			// Draw Button Text labels
			drawButtonsText(0); 	   

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;

		case stateTodayHighScore:
	   {	      
			struct tm *local;
			int startEntry;
			int endEntry;
		  		  
			if (game.maxTodayHighScore<15)
			{
				startEntry=0;
				endEntry=game.maxTodayHighScore;
			}
			else
			{
				startEntry=(((float) game.maxTodayHighScore-15.0)/30.0)*(float)game.scrollIndex;
				endEntry=startEntry+15;
			}
		   
         // Draw background
         GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR2 );
      	     	
			// Draw scrollbar
			int y=SCROLLBAR_Y_MIN;
         GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollTop, 0, 1, 1, IMAGE_COLOR );
			for (int i=0; i<10; i++) 
			{
				y+=24;
				GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollMiddle, 0, 1, 1, IMAGE_COLOR );
			}
			y+=24;
			GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollBottom, 0, 1, 1, IMAGE_COLOR );
		  		  
	      // Draw title
	      drawText(0, ypos, fontTitle, "   Today High Score");	

         // Show Content
         ypos+=90;

			drawText(20, ypos, fontParagraph,  "TOP" );
	      drawText(80, ypos, fontParagraph,  "DATE" );
	      drawText(270, ypos, fontParagraph, "SCORE" );
			drawText(350, ypos, fontParagraph, "NAME"  );
			drawText(430, ypos, fontParagraph, "LOCATION" );
			ypos+=10;
		  
			if (todayHighScore[0].dt!=0)
			{
            for (int i=startEntry; i<endEntry; i++)
            {
					ypos+=20;  
	    
					drawText(20, ypos, fontNormal, "%02d", i+1);
			  			  
					local = localtime(&todayHighScore[i].dt);
					sprintf(tmp,"%02d-%02d-%04d %02d:%02d:%02d", 
						local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
						local->tm_hour, local->tm_min, local->tm_sec);
					drawText(80, ypos, fontNormal, tmp);
	   
					drawText(270, ypos, fontNormal, todayHighScore[i].score);			  
					drawText(350, ypos, fontNormal, todayHighScore[i].name);			  
					drawText(430, ypos, fontNormal, todayHighScore[i].location);
				}			
			}
			else
			{
		      ypos+=120;
		      drawText(0, ypos, fontParagraph, "No information available!");
				ypos+=20;
				drawText(0, ypos, fontParagraph, "Information could not be fetch from internet.");
			}
			 
         // Draw buttons
	      drawButtons(); 
		  
			// Draw Button Text labels
			drawButtonsText(0);	   

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;
	   
		case stateGlobalHighScore:
	   {	      
			struct tm *local;
			int startEntry;
			int endEntry;
		  		  
			if (game.maxGlobalHighScore<15)
			{
				startEntry=0;
				endEntry=game.maxGlobalHighScore;
			}
			else
			{
				startEntry=(((float) game.maxGlobalHighScore-15.0)/30.0)*(float)game.scrollIndex;
				endEntry=startEntry+15;
			}
		  		   
         // Draw background
         GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR2 );
	      	     	
			// Draw scrollbar
			int y=SCROLLBAR_Y_MIN;
         GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollTop, 0, 1, 1, IMAGE_COLOR );
			for (int i=0; i<10; i++) 
			{
				y+=24;
				GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollMiddle, 0, 1, 1, IMAGE_COLOR );
			}
			y+=24;
			GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollBottom, 0, 1, 1, IMAGE_COLOR );
		  		  
	      // Draw title
	      drawText(0, ypos, fontTitle, "   Global High Score");	

         // Show Content
         ypos+=90;

			drawText(20, ypos, fontParagraph,  "TOP"  );
	      drawText(80, ypos, fontParagraph,  "DATE" );
	      drawText(270, ypos, fontParagraph, "SCORE" );
			drawText(350, ypos, fontParagraph, "NAME"  );
			drawText(430, ypos, fontParagraph, "LOCATION" );
			ypos+=10;
		  
			if (globalHighScore[0].dt!=0)
			{
            for (int i=startEntry; i<endEntry; i++)
            {
					ypos+=20;  
	    
					drawText(20, ypos, fontNormal, "%02d", i+1);
			  			  
					local = localtime(&globalHighScore[i].dt);
					sprintf(tmp,"%02d-%02d-%04d %02d:%02d:%02d", 
						local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
						local->tm_hour, local->tm_min, local->tm_sec);
					drawText(80, ypos, fontNormal, tmp);
	   
					drawText(270, ypos, fontNormal, globalHighScore[i].score);			  
					drawText(350, ypos, fontNormal, globalHighScore[i].name);			  
					drawText(430, ypos, fontNormal, globalHighScore[i].location);
				}			
			}
			else
			{
		      ypos+=120;
		      drawText(0, ypos, fontParagraph, "No information available!");
				ypos+=20;
				drawText(0, ypos, fontParagraph, "Information could not be fetch from internet.");
			}
			 
         // Draw buttons
	      drawButtons(); 
		  
			// Draw Button Text labels
			drawButtonsText(0);	   

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;

	   case stateHelp1:
	   {	  
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR2 );
		 
			// Draw buttons
	      drawButtons(); 
 
			// Show title
			drawText(0, ypos, fontTitle, "Help");
			ypos+=100;
		  
			drawText(0, ypos, fontParagraph, "Wii TowerDefense is an classic 2D action game. Protect ");
			ypos+=25;
			drawText(0, ypos, fontParagraph, "your base with all kind of defense systems and kill");	
			ypos+=25;
			drawText(0, ypos, fontParagraph, "all the waves of enemies. If ten enemies reach the");		
			ypos+=25;
			drawText(0, ypos, fontParagraph, "base the game is over. Good Luck!");		

		   ypos+=60;
			drawText(0, ypos, fontParagraph, "You can only build weapons on land. It is not allowed");
			ypos+=25;
			drawText(0, ypos, fontParagraph, "to build on water, roads or other deployed weapons.");		
		
			ypos+=60;
			drawText(0, ypos, fontParagraph, "Note: The global highscore contains the Top 40 of best");
			ypos+=25;
			drawText(0, ypos, fontParagraph, "internet players. Only one entry per player is showed.");	  

			// Draw Button Text labels
			drawButtonsText(0);
	
			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
	
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		
		case stateHelp2:
	   {	  
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR2 );
			 
			// Draw buttons
	      drawButtons(); 
		   
			// Show title
			drawText(0, ypos, fontTitle, "WiiMote Control");
		  
			int xoffset=50;
	
         ypos+=100;
			drawText(60+xoffset, ypos,  fontParagraph, "Button");
			drawText(180+xoffset, ypos,  fontParagraph, "Action");
	
			ypos+=50;	  
			drawText(60+xoffset, ypos, fontParagraph, "A");
			drawText(180+xoffset, ypos, fontParagraph, "Select button on screen" ); 

			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "B");
			drawText(180+xoffset, ypos, fontParagraph, "Buy new weapon" ); 

			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "-");
			drawText(180+xoffset, ypos, fontParagraph, "Sell selected weapon" ); 
			
			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "<");
			drawText(180+xoffset, ypos, fontParagraph, "Select previous weapon type" ); 

			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, ">");
			drawText(180+xoffset, ypos, fontParagraph, "Select next weapon type" ); 	

			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "1");
			drawText(180+xoffset, ypos, fontParagraph, "Play next music track" ); 

			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "2");
			drawText(180+xoffset, ypos, fontParagraph, "Play previous music track" ); 	

			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "+");
			drawText(180+xoffset, ypos, fontParagraph, "Make screenshot" ); 		
			
			ypos+=25;	  
			drawText(60+xoffset, ypos, fontParagraph, "Home");
			drawText(180+xoffset, ypos, fontParagraph, "Quit the game" );
		  
			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;

		case stateHelp3:
	   {	  
			// Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR4 );
			 
			// Draw buttons
	      drawButtons(); 
		   
			// Show title
			drawText(170, ypos, fontTitle, "Weapons");
			
			int xoffset=30;
	
         ypos+=100;
			drawText(10+xoffset, ypos,  fontParagraph, "Icon");
			drawText(60+xoffset, ypos,  fontParagraph, "Type");
			drawText(125+xoffset, ypos, fontParagraph, "Price");
			drawText(195+xoffset, ypos, fontParagraph, "Power");
			drawText(280+xoffset, ypos, fontParagraph, "Range");
			drawText(365+xoffset, ypos, fontParagraph, "Rate");
			drawText(455+xoffset, ypos, fontParagraph, "Fire Mode");
		
			ypos+=10;
			for (int i=0; i<6; i++)
			{
				ypos+=40;
	
				// Calculate frame of weapon animation
				if (weaponSpecs->getImage(i)->h==32) 
				{
					// No animation available
					game.frame=0;
				}
				else
				{
					// Select correct frame;
					switch (game.frameCounter)
					{
						case 0:  game.frame=4+(16*0);
									break;
					 
						case 1:  game.frame=4+(16*1);
									break;
					
						case 2:  game.frame=4+(16*2);
									break;
						
						case 3:  game.frame=4+(16*3);
									break;
					}
					
					// Check if next frame must be selected
					if (game.frameDelay>14) 
					{
						game.frameDelay=0;
						game.frameCounter++;
						if (game.frameCounter>6) game.frameCounter=0;
					}
					else
					{
						game.frameDelay++;
					}
				}
		
				GRRLIB_DrawTile(10+xoffset,ypos, 
					weaponSpecs->getImage(i), 0, 1, 1, IMAGE_COLOR, game.frame );
					
				drawText(60+xoffset, ypos, fontNormal, 
					weaponSpecs->getName(i));
					
				drawText(125+xoffset, ypos, fontNormal, "$%d",
					weaponSpecs->getPrice(i));
					
				drawText(195+xoffset, ypos, fontNormal, "%03d-%03d",
					weaponSpecs->getMinPower(i), weaponSpecs->getMaxPower(i));
					
				drawText(280+xoffset, ypos, fontNormal, "%03d-%03d",
					weaponSpecs->getMinRange(i), weaponSpecs->getMaxRange(i));
					
				drawText(365+xoffset, ypos, fontNormal, "%03d-%03d",
					weaponSpecs->getMinRate(i), weaponSpecs->getMaxRate(i));
					
				drawText(455+xoffset, ypos, fontNormal, weaponSpecs->getFireModeName(i));
  			}
		  
		   ypos=420;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
	
			drawText(130, ypos, fontNormal, "Overview of upgrade range of each weapon system.");	
		
			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		
		case stateHelp4:
	   {	  
	      // Draw background
			GRRLIB_DrawImg(0,0, images.background1, 0, 1, 1, IMAGE_COLOR4 );
		 
			// Draw buttons
	      drawButtons(); 
		  		
		   // Show title
			drawText(185, ypos, fontTitle, "Enemies");
        
			ypos=120;
			int xpos=90;
		  	
			for (int i=0; i<25; i++)
			{
				// Calculate frame of enemy animation
				if (monsterSpecs->getImage(0)->h==32) 
				{
					// No animation available
					game.frame=0;
				}
				else
				{
					if (game.frameDelay>144)
					{
						switch (game.frameCounter)
						{
							case 0:  game.frame=2;
										game.frameCounter++;
										break;
					  
							case 1:  game.frame=6;
										game.frameCounter++;
										break;
						
							case 2:  game.frame=2;
										game.frameCounter++;
										break;
							
							case 3:  game.frame=10;
										game.frameCounter=0;
										break;
						}
						game.frameDelay=0;
					}
					else
					{
						game.frameDelay++;
					}
				}
			
				GRRLIB_DrawTile( xpos, ypos, 
					monsterSpecs->getImage(i), 0, 1, 1, IMAGE_COLOR, game.frame);
			
			   drawText(xpos+42, ypos, fontNormal, "%d", 
					monsterSpecs->getEnergy(i));
				
				ypos+=40;
				
				if (((i+1) % 7) == 0)
				{
					ypos=120;
					xpos+=120;
				}
			}
			
			ypos=420;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
	
   	   drawText(130, ypos, fontNormal, "Overview of energy level of each enemy type.");	
		  	   		  					  
			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
		}
		break;
		  
	   case stateCredits:
	   {  
	      // Draw background
			GRRLIB_DrawImg(0,0,images.background1, 0, 1.0, 1.0, IMAGE_COLOR2 );
		  
			// Draw buttons
	      drawButtons(); 
		  
			// Show title
			drawText(220, ypos, fontTitle, "Credits");
         ypos+=90;
		  
			// Show Content
	      drawText(0, ypos, fontParagraph, "GAME LOGIC ");
         ypos+=20;
	      drawText(0, ypos, fontNormal, "wplaat");

	      ypos+=30;
	      drawText(0, ypos, fontParagraph, "GAME GRAPHICS  ");
  	      ypos+=20;
	      drawText(0, ypos, fontNormal, "wplaat");
			ypos+=20;
	      drawText(0, ypos, fontNormal, "MLtm, shang64 and Crayon");
			ypos+=20;
	      drawText(0, ypos, fontNormal, "Appliciant (Thanks for the great animated sprites)");
		  
	      ypos+=30;
	      drawText(0, ypos, fontParagraph, "MUSIC ");
	      ypos+=20;
	      drawText(0, ypos, fontNormal, "modarchive.org  ");

	      ypos+=30;
	      drawText(0, ypos, fontParagraph, "SOUND EFFECTS  ");
	      ypos+=20;
	      drawText(0, ypos, fontNormal, "wplaat");
  
  	      ypos+=30;
	      drawText(0, ypos, fontParagraph, "TESTERS");	  
			ypos+=20;
	      drawText(0, ypos, fontNormal, "wplaat");	  
			ypos+=20;
	      drawText(0, ypos, fontNormal, "quali and Wiiman360");
			
	      ypos+=30;
	      drawText(140, ypos, fontNormal,"Greetings to everybody in the Wii homebrew scene");
		  
			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;
	   
	   case stateReleaseNotes:
	   {
	      int  i=0;
			int  len=0;
			int  lineCount=0;
			int  maxLines=0;
			char *buffer=NULL;
			char text[MAX_BUFFER_SIZE];
		  
			int startEntry;
			int endEntry;
		  		  
			// Fetch release notes from network thread
			buffer=tcp_get_releasenote();
         if (buffer!=NULL) 
			{
				strncpy(text,buffer,MAX_BUFFER_SIZE);
				len=strlen(text);
				for (i=0;i<len;i++) if (text[i]=='\n') maxLines++;
			}
		  
			// Calculate start and end line.
			if (maxLines<20)
			{
				startEntry=0;
				endEntry=maxLines;
			}
			else
			{
				startEntry=(((float) maxLines-18.0)/30.0)*(float)game.scrollIndex;
				endEntry=startEntry+20;
			}
		   
			// Draw background
			GRRLIB_DrawImg(0,0,images.background1, 0, 1.0, 1.0, IMAGE_COLOR2 );

			// Draw buttons
	      drawButtons(); 
		  
         // Draw scrollbar
			int y=SCROLLBAR_Y_MIN;
         GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollTop, 0, 1, 1, IMAGE_COLOR );
			for (i=0; i<10; i++) 
			{
				y+=24;
				GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollMiddle, 0, 1, 1, IMAGE_COLOR );
			}
			y+=24;
			GRRLIB_DrawImg(SCROLLBAR_x,y, images.scrollBottom, 0, 1, 1, IMAGE_COLOR );
		  		 
	      // Draw Title	
         drawText(100, ypos, fontTitle, "Release Notes");
         ypos+=80;
	      
			if (len!=0)
			{		  
				int startpos=0;
				for (i=0; i<len; i++)
				{
					if (text[i]=='\n') 
					{			   
						text[i]=0x00;
				 
						// Show only 19 lines on screen
						if ((lineCount++)>endEntry) break;
						if (lineCount>startEntry) 
						{
							ypos+=15;
							drawText(40, ypos, fontNormal, text+startpos);	
						}
						startpos=i+1;
					}
				}
			}
			else
			{
				ypos+=120;
				drawText(0, ypos, fontParagraph, "No information available!" );	
				ypos+=20;
				drawText(0, ypos, fontParagraph, "Information could not be fetch from internet.");
			}
		  
			// Draw Button Text labels
			drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;
	   
	   case stateSoundSettings:
	   { 
	      // Draw background
			GRRLIB_DrawImg(0,0,images.background1, 0, 1.0, 1.0, IMAGE_COLOR );
		
		   // Draw Sound icon
			int yoffset=20;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) yoffset-=20;
	      GRRLIB_DrawImg((640/2), ((480/2))+yoffset, images.soundicon, 
				game.angle, 1.4, 1.4, IMAGE_COLOR );
			if (game.angle<MAX_ANGLE) game.angle++; else game.angle=0;
			
			// Draw buttons
	      drawButtons(); 
		  		  
			// Show title
			drawText(100, ypos, fontTitle, "Sound Settings");
        
		   ypos+=140;
	      if (rmode->xfbHeight==MAX_VERT_PIXELS) ypos-=20;
			
         // Draw content	
         drawText(0, ypos, fontParagraph, "Music Volume");	
	      ypos+=20;
         GRRLIB_DrawImg(104,ypos,images.bar, 0, 1, 1, IMAGE_COLOR );
	      ypos+=10;
	      GRRLIB_DrawImg(115+(sound->getMusicVolume()*40),ypos, 
				images.barCursor, 0, 1, 1, IMAGE_COLOR );
  
         ypos+=80;
         drawText(0, ypos, fontParagraph, "Effects Volume" );
	      ypos+=20;	
	      GRRLIB_DrawImg(104,ypos, images.bar, 0, 1, 1, IMAGE_COLOR );
	      ypos+=10;
	      GRRLIB_DrawImg(115+(sound->getEffectVolume()*40),ypos,
				images.barCursor, 0, 1, 1, IMAGE_COLOR );
	
	      ypos+=70;
	      drawText(0, ypos, fontParagraph, "  Music track [%d]", sound->getMusicTrack());	
		  		  		
		   // Draw Button Text labels
		   drawButtonsText(0);

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;
	   
	   case stateGameSettings:
	   {         	
			// Draw background
			GRRLIB_DrawImg(0,0,images.background1, 0, 1.0, 1.0, IMAGE_COLOR );

			// Draw Transparent Panel
			GRRLIB_Rectangle(30, ypos+120, 570, ypos+180, GRRLIB_BLACK_TRANS_2, 1);
			
			// Draw buttons
	      drawButtons(); 
		  
			// Draw Title	
			drawText(100, ypos, fontTitle, "Game Settings");
			ypos+=90;
			
			drawText(0, ypos, fontParagraph, "User initials are used in the highscore area.");	

			// Draw initial characters
			ypos+=90;	
			int xpos=50;
			
			drawText(xpos, ypos, fontTitle, "%c", settings->getFirstChar());
			xpos+=95;
			drawText(xpos, ypos, fontTitle, "%c", settings->getSecondChar());
			xpos+=95;
			drawText(xpos, ypos, fontTitle, "%c", settings->getThirdChar());
			xpos+=95;
			drawText(xpos, ypos, fontTitle, "%c", settings->getFourthChar());
			xpos+=95;
			drawText(xpos, ypos, fontTitle, "%c", settings->getFifthChar());
			xpos+=95;
			drawText(xpos, ypos, fontTitle, "%c", settings->getSixthChar());

			ypos+=170;
			drawText(65, ypos, fontParagraph, "Classic Enemies");

			// Draw Button Text labels
			drawButtonsText(0);	

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;
		
		case stateDonate:
	   {         	
			// Draw background
			GRRLIB_DrawImg(0,0,images.background1, 0, 1.0, 1.0, IMAGE_COLOR2 );

			// Draw buttons
	      drawButtons(); 
		  
			// Draw Title	
			drawText(0, ypos, fontTitle, "Donate");
			ypos+=100;
			
			drawText(0, ypos, fontParagraph, "If you enjoy this game please send me a");
			ypos+=25;
		   drawText(0, ypos, fontParagraph, "small donation. You can make a donation ");
			ypos+=25;
		   drawText(0, ypos, fontParagraph, "online with your credit card, or paypal account.");
			ypos+=25;
		   drawText(0, ypos, fontParagraph, "Your credit card will be processed by PayPal, a");
			ypos+=25;
		   drawText(0, ypos, fontParagraph, "trusted name in secure online transactions.");
			ypos+=65;
		   drawText(0, ypos, fontParagraph, "Please visit http://www.plaatsoft.nl");			
			ypos+=25;
		   drawText(0, ypos, fontParagraph, "Click on the donate link and follow the instructions.");	
			ypos+=65;
			drawText(0, ypos, fontParagraph, "Many thanks for your support!");

			// Draw Button Text labels
			drawButtonsText(0);	

			// Draw network thread status on screen
			drawText(20, rmode->xfbHeight-38, fontSmall, "Network: %s",tcp_get_state());
			
			// Show FPS information on screen.
			drawText(20, rmode->xfbHeight-28, fontSmall, "%d fps", calculateFrameRate());
	   }
	   break;
	}
}

// -----------------------------------
// SUPPORT METHODES
// -----------------------------------

/**
 * Enemy monsters stop moving (pause mode)
 */
void stopMonsters(void)
{
	for (int i=0; i<MAX_MONSTERS; i++)
	{
		if (monsters[i]!=NULL)
		{
			monsters[i]->setMove(false);
		}
	}
}

/**
 * Enemy monster start moving again (continue mode)
 */
void startMonsters(void)
{
	for (int i=0; i<MAX_MONSTERS; i++)
	{
		if (monsters[i]!=NULL)
		{
			monsters[i]->setMove(true);
		}
	}
}

/**
 * Parse today higshcore received my network thread
 * @param buffer	The buffer contain the raw xml message with contain the highscore.
 */
void loadTodayHighScore(char *buffer)
{
   const char *s_fn="loadTodayHighScore";
   trace->event(s_fn,0,"enter");
	
   int i;
   mxml_node_t *tree=NULL;
   mxml_node_t *data=NULL;
   const char *tmp;
   char temp[MAX_LEN];
   
   game.maxTodayHighScore=0;
   
   // Clear memory
   for(i=0; i<MAX_TODAY_HIGHSCORE; i++)
   {
		todayHighScore[i].score[0]=0x00;
		todayHighScore[i].dt=0;
		todayHighScore[i].name[0]=0x00;
		todayHighScore[i].location[0]=0x00;
   } 
	 
   // If xml data available, parse it....
   if ((buffer!=NULL) && (strlen(buffer)>0))
   {  
      tree = mxmlLoadString(NULL, buffer, MXML_NO_CALLBACK);

      for(i=0; i<MAX_TODAY_HIGHSCORE; i++)
      {		
	    sprintf(temp, "item%d", i+1);
        data = mxmlFindElement(tree, tree, temp, NULL, NULL, MXML_DESCEND);

        tmp=mxmlElementGetAttr(data,"dt");   
        if (tmp!=NULL) todayHighScore[game.maxTodayHighScore].dt=atoi(tmp); else todayHighScore[game.maxTodayHighScore].dt=0; 
		
		tmp=mxmlElementGetAttr(data,"score");   
        if (tmp!=NULL) strcpy(todayHighScore[game.maxTodayHighScore].score,tmp); else strcpy(todayHighScore[game.maxTodayHighScore].score,"");
		
        tmp=mxmlElementGetAttr(data,"name");   
        if (tmp!=NULL) strcpy(todayHighScore[game.maxTodayHighScore].name,tmp); else strcpy(todayHighScore[game.maxTodayHighScore].name,"");

		tmp=mxmlElementGetAttr(data,"location");   
        if (tmp!=NULL) strcpy(todayHighScore[game.maxTodayHighScore].location,tmp); else strcpy(todayHighScore[game.maxTodayHighScore].location,"");
		
		// Entry is valid (Keep the inforamtion)
        if (strlen(todayHighScore[game.maxTodayHighScore].score)>0) game.maxTodayHighScore++;	
      }   
      mxmlDelete(data);
      mxmlDelete(tree);
   }
   
    trace->event(s_fn,0,"leave [void]");
}

/**
 * Parse global higshcore received my network thread
 * @param buffer	The buffer contain the raw xml message with contain the highscore.
 */
void loadGlobalHighScore(char *buffer)
{
    const char *s_fn="loadGlobalHighScore";
    trace->event(s_fn,0,"enter");
	
   int i;
   mxml_node_t *tree=NULL;
   mxml_node_t *data=NULL;
   const char *tmp;
   char temp[MAX_LEN];
   
   game.maxGlobalHighScore=0;
   
   // Clear memory
   for(i=0; i<MAX_GLOBAL_HIGHSCORE; i++)
   {
      globalHighScore[i].score[0]=0x00;
		globalHighScore[i].dt=0;
		globalHighScore[i].name[0]=0x00;
		globalHighScore[i].location[0]=0x00;
   } 
	 
   // If xml data available, parse it....
   if ((buffer!=NULL) && (strlen(buffer)>0))
   {  
      tree = mxmlLoadString(NULL, buffer, MXML_NO_CALLBACK);

      for(i=0; i<MAX_GLOBAL_HIGHSCORE; i++)
      {		
			sprintf(temp, "item%d", i+1);
			data = mxmlFindElement(tree, tree, temp, NULL, NULL, MXML_DESCEND);

			tmp=mxmlElementGetAttr(data,"dt");   
			if (tmp!=NULL) globalHighScore[game.maxGlobalHighScore].dt=atoi(tmp); else globalHighScore[game.maxGlobalHighScore].dt=0; 
		
			tmp=mxmlElementGetAttr(data,"score");   
			if (tmp!=NULL) strcpy(globalHighScore[game.maxGlobalHighScore].score,tmp); else strcpy(globalHighScore[game.maxGlobalHighScore].score,"");
		
			tmp=mxmlElementGetAttr(data,"name");   
			if (tmp!=NULL) strcpy(globalHighScore[game.maxGlobalHighScore].name,tmp); else strcpy(globalHighScore[game.maxGlobalHighScore].name,"");

			tmp=mxmlElementGetAttr(data,"location");   
			if (tmp!=NULL) strcpy(globalHighScore[game.maxGlobalHighScore].location,tmp); else strcpy(globalHighScore[game.maxGlobalHighScore].location,"");
		
			// Entry is valid (Keep the inforamtion)
			if (strlen(globalHighScore[game.maxGlobalHighScore].score)>0) game.maxGlobalHighScore++;	
      }   
      mxmlDelete(data);
      mxmlDelete(tree);
   }
   
    trace->event(s_fn,0,"leave [void]");
}
		
/**
 * Move monsters on screen
 */
void moveMonsters(void)
{
	for( int i=0; i<MAX_MONSTERS; i++ ) 
	{
		if (monsters[i]!=NULL)
		{		
			monsters[i]->move();
		}
	}
}

/** 
 * Move weapons on screen
 */
void moveWeapons(void)
{
	for( int i=0; i<MAX_WEAPONS; i++ ) 
	{
		if (weapons[i]!=NULL)
		{
			weapons[i]->move();
		}
	}
}

/**
 * Check for pointer image change
 */
void checkPointer(void)
{
	if (game.selectedNewWeapon)
	{
	   // Draw pointer image
		if (pointers[0]!=NULL)
		{ 
		   int x1=(float) pointers[0]->getXOffset()/32.0;
			int y1=(float) pointers[0]->getYOffset()/32.0;
			
			if ( (game.selectedMap!=-1) && 
				  (grids[game.selectedMap]!=NULL) &&
				  (!grids[game.selectedMap]->isBuild(x1,y1)) ) 
			{
				pointers[0]->setColor(IMAGE_COLOR);
			}
			else
			{
				pointers[0]->setColor(IMAGE_COLOR3);
			}
		}

		// Draw matrix on grid
		for (int x=4; x<MAX_GRID_X; x++)
		{
			GRRLIB_Line( x*32, 0, x*32, 528, GRRLIB_WHITE_TRANS);								
		}
		for (int y=0; y<MAX_GRID_Y; y++)
		{
			GRRLIB_Line( 128, y*32, 640, y*32, GRRLIB_WHITE_TRANS);								
		}
	}
}

/**
 * Check if game is over!
 */
void checkGameOver(void)
{
	if (game.monsterInBase>=MAX_MONSTER_IN_BASE)
	{
		// Too many monster in Base 
		game.stateMachine=stateGameOver; 
		game.event=eventSaveHighScore;
	}
}

/** 
 * Check enemy is real dead, if soo, destroy object
 */
void checkDeadMonsters(void)
{
	for (int i=0;i<MAX_MONSTERS;i++)
	{
		if ((monsters[i]!=NULL) && (monsters[i]->getState()==stateEnemyDead))
		{
			delete monsters[i];
			monsters[i]=NULL;
		}
	}
}

/**
 * Check if enemy wave must be launched. 
 */
void checkNextWave(void)
{		
	if (--game.waveCountDown>0)
	{
		// Check if there are any monster left in wave. If not start next wave
		for (int i=0;i<MAX_MONSTERS;i++)
		{
			if (monsters[i]!=NULL)
			{
				return;
			}
		}
	}	
	
	// If event is idle
	if (game.event==eventNone)
	{	
		// Launch new monster wave else wait one game cycle and try again
		game.event=eventLaunch;
	}
}
	
/**
 * Calculate Video Frame Rate (Indication how game engine performs)
 */
static u8 calculateFrameRate(void) 
{
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}	

/**
 * Proces event changes
 */
void processEvent(void)
{
	const char *s_fn="processEvent";
	    
    // If no event return directly!
    if (game.event==eventNone) return;
  
	// Event state
	switch (game.event)
	{
		case eventInitEasyLevels:
			trace->event(s_fn,0,"event=eventInitEasyLevels");
			game.level=gameEasy;
			matrix->setLevel(game.level);
			initGrids(game.level);
			break;

		case eventInitMediumLevels:
			trace->event(s_fn,0,"event=eventInitMediumLevels");
			game.level=gameMedium;
			matrix->setLevel(game.level);
			initGrids(game.level);
			break;

		case eventInitHardLevels:
		   trace->event(s_fn,0,"event=eventInitHardLevels");
			game.level=gameHard;
			matrix->setLevel(game.level);
			initGrids(game.level);			
			break;
			
		case eventWeaponSelected:		
		{
			trace->event(s_fn,0,"event=eventWeaponSelected");
			
			if (game.cash>=weaponSpecs->getPrice(game.weaponType))
			{
				// Change pointer image to weapon image (For location definition)
				pointers[0]->setImage(weaponSpecs->getImageSpecial(game.weaponType));
				
				// Deselected other weapon, if any
				if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL)) 
				{
					weapons[game.selectedWeapon]->setSelected(false);
					game.selectedWeapon=-1;
				}
			}
		}
		break;
		
		case eventweaponDeployed:
		{
			trace->event(s_fn,0,"event=eventweaponDeployed");

			// First restore pointer image
			pointers[0]->setImage(images.pointer1);
			pointers[0]->setColor(IMAGE_COLOR);
				
			int x1=(float) pointers[0]->getXOffset()/32.0;
			int y1=(float) pointers[0]->getYOffset()/32.0;
				
			if ( (game.cash>=weaponSpecs->getPrice(game.weaponType)) &&
				  (game.selectedMap!=-1) &&
				  (grids[game.selectedMap]!=NULL) &&
				  (!grids[game.selectedMap]->isBuild(x1,y1) )
				)
			{
				// Find first empty place in weapons array.
				// Nothing free re-used the latest place!
				int id=0;
				while (weapons[id]!=NULL) 
				{
					if (++id>=(MAX_WEAPONS-1)) 
					{
						id=(MAX_WEAPONS-1);
						break;
					}
				}
					
				// Deselected all weapons
				for (int j=0;j<MAX_WEAPONS; j++)
				{
					if (weapons[j]!=NULL) 
					{
						weapons[j]->setSelected(false);
					}
				}
				
				if ( (game.selectedMap!=-1) && 
					  (grids[game.selectedMap]!=NULL))
				{
					// Set new location as build full.
					grids[game.selectedMap]->setBuild(x1,y1);
				}
				
				// Create new weapon
				initWeapon(x1,y1,id, game.weaponType);
				
				// Selected new weapon
				game.selectedWeapon=id;	

				// Pay for the weapon
				game.cash-=weaponSpecs->getPrice(game.weaponType);
				
				// Stored payed money in weapon calls
				weapons[id]->setTotalPrice(weaponSpecs->getPrice(game.weaponType));
			}
		}
		break;
		
		case eventWeaponNext:		
		{
			trace->event(s_fn,0,"event=eventWeaponNext");
			
			// Deselected other weapon, if any
			if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
			{
				weapons[game.selectedWeapon]->setSelected(false);
				game.selectedWeapon=-1;
			}
				
			if (game.weaponType<(MAX_WEAPON_TYPE-1)) 
			{
				game.weaponType++; 
			}
	 
	 		// Set button 
			if (game.cash>=weaponSpecs->getPrice(game.weaponType)) 
			{
				// Weapon normal
				buttons[6]->setColor(IMAGE_COLOR);
			}
			else
			{
				// Weapon Transparent (Not for sale)
				buttons[6]->setColor(IMAGE_COLOR3);
			}
			buttons[6]->setImageNormal(weaponSpecs->getImageSpecial(game.weaponType));
			buttons[6]->setImageFocus(weaponSpecs->getImageSpecial(game.weaponType));
		}
		break;

		case eventWeaponPrevious:		
		{
			trace->event(s_fn,0,"event=eventWeaponPrevious");

			// Deselected other weapon, if any
			if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
			{
				weapons[game.selectedWeapon]->setSelected(false);
				game.selectedWeapon=-1;
			}
			
			if (game.weaponType>0) 
			{
				game.weaponType--; 
			}
	
			// Set button 
			if (game.cash>=weaponSpecs->getPrice(game.weaponType))
			{
				// Weapon normal
				buttons[6]->setColor(IMAGE_COLOR);
			}
			else
			{
				// Weapon Transparent (Not for sale)
				buttons[6]->setColor(IMAGE_COLOR3);
			}
			buttons[6]->setImageNormal(weaponSpecs->getImageSpecial(game.weaponType));
			buttons[6]->setImageFocus(weaponSpecs->getImageSpecial(game.weaponType));
		}
		break;
	
		case eventWeaponSell:		
		{
			trace->event(s_fn,0,"event=eventWeaponSell");
			if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
			{
				 game.cash+=weapons[game.selectedWeapon]->getTotalPrice();
				 grids[game.selectedMap]->setUnBuild(
					weapons[game.selectedWeapon]->getX(),
					weapons[game.selectedWeapon]->getY());
				 delete weapons[game.selectedWeapon];
				 weapons[game.selectedWeapon]=NULL;
			}
		}
		break;
		
		case eventLaunch:
		{
			trace->event(s_fn,0,"event=eventLaunch");	
			
			// Count amount of empty places in enemies array.
			int count=0;
			for(int i=0;i<MAX_MONSTERS;i++)
			{
				if (monsters[i]==NULL) count++;
			}
			
			// Increase wave level
			game.wave++;
			matrix->setWave(game.wave);
										
			if ((matrix->getEnemyAmount())<count)
			{				
				// There is enough room in enemy array so add next wave
				game.waveCountDown=matrix->getWaveDelay();
				
				if (game.stateMachine==stateMapSelectMenu)
				{
					initMonsters(true);	
				}
				else
				{
					initMonsters(false);
				}
				
				// Get Bonus score and Bonus cash
				game.score+=(game.wave*225); // 200
				game.cash+=(game.wave*225); // 200

				// Show WAVE text on screen
				game.alfa=MAX_ALFA;

				// Launch sound effect
				if (game.stateMachine==stateGame) sound->effect(SOUND_LAUNCH);	
			}
			else
			{	
				trace->event(s_fn,0,"No room in enemy array wait 2 seconds and try again");
				game.waveCountDown=(2 * AVERAGE_FPS);			
				
				// Decrease wave again.
				game.wave--;
				matrix->setWave(game.wave);
			}				
		}
		break;
		
		case eventSaveHighScore:
		{
			// Calculate played mapId;
			int mapId=(game.level+1)*(game.selectedMap+1);
			
			trace->event(s_fn,0,"event=eventSaveHighScore");
			
			// Store highscore local in file.
			char name[MAX_LEN];
			sprintf(name,"%c%c%c%c%c%c",
				settings->getFirstChar(), 
				settings->getSecondChar(), 
				settings->getThirdChar(),
				settings->getFourthChar(),
				settings->getFifthChar(),
				settings->getSixthChar());
			highScore->setScore(name, game.wave, mapId, game.score);
			highScore->save(HIGHSCORE_FILENAME);
			
			// Store highscore on internet if higher then 20.000 points
			if (game.score>20000)
			{
				char tmp2[MAX_LEN];
				sprintf(tmp2,"appl=%s&version=%s&level=%d&score=%d&name=%s&map=%d&dt=%d",
					PROGRAM_NAME, 
					PROGRAM_VERSION,
					game.wave, 
					game.score, 
					name, 
					mapId,
					(int)time(NULL));
				tcp_set_state(TCP_REQUEST3a, tmp2);	
			}
		}
		break;
		
		case eventWeaponPowerUpgrade:
		{
			trace->event(s_fn,0,"event=eventWeaponPowerUpgrade");			
			
			if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
			{
				weapons[game.selectedWeapon]->upgradePower();
			}
		}
		break;
				
		case eventWeaponRangeUpgrade:
		{ 			
			trace->event(s_fn,0,"event=eventWeaponRangeUpgrade");						
			
			if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
			{
				weapons[game.selectedWeapon]->upgradeRange();
			}
		}
		break;
			
		case eventWeaponRateUpgrade:
		{ 
 			trace->event(s_fn,0,"event=eventWeaponRateUpgrade");
			
			if ((game.selectedWeapon!=-1) && (weapons[game.selectedWeapon]!=NULL))
			{
				weapons[game.selectedWeapon]->upgradeRate();
			}
		}
		break;			
	}
	
	// Event is progress, set event handler to none!
	game.event=eventNone;
}


/**
 * Process state Machine change
 */
void processStateMachine(void)
{
	const char *s_fn="processStateMachine";
	
	// If state is not changed return directly!
	if (game.prevStateMachine==game.stateMachine) return;
    
	// Process new state
	switch (game.stateMachine)
	{
   
		case stateIntro1:
		{
			trace->event(s_fn,0,"stateMachine=stateIntro1");
		
			// Start background music
			sound->play();
			
			game.size=0;
			game.alfa=0;
		}
		break;

		case stateIntro2:
		{
			trace->event(s_fn,0,"stateMachine=stateIntro2");
			game.alfa=0;
		}
		break;
		
		case stateIntro3:
		{
			trace->event(s_fn,0,"stateMachine=stateIntro3");
			game.location=0;
		}
		break;
		
		case stateIntro4:
		{
			trace->event(s_fn,0,"stateMachine=stateIntro4");
		}
		break;
		
		case stateMainMenu:
		{
			trace->event(s_fn,0,"stateMachine=stateMainMenu");
		
			// Init buttons
			initButtons();
		}
		break;

		case stateLevelMenu:
		{
			trace->event(s_fn,0,"stateMachine=stateLevelMenu");
			
			game.frameCounter=0;
			game.frameDelay=0;
			
			// Init buttons
			initButtons();	
		}
		break;
		
		case stateMapSelectMenu:
		{
			trace->event(s_fn,0,"stateMachine=stateMapSelectMenu");
			
			// Init buttons
			initButtons();	
						
			// Init game variables
			initGame(75);
		}
		break;
	
		case stateGame:
		{
			trace->event(s_fn,0,"stateMachine=stateGame");
	
			// Init buttons
			initButtons();	
			
			// Start monster movement
			startMonsters();
	
			if (game.prevStateMachine!=stateQuitGame)
			{
				// Init game variables
				initGame(0);
			}
		}
		break;
	
		case stateQuitGame:	
		{
			trace->event(s_fn,0,"stateMachine=stateQuitGame");
		
			// Init buttons
			initButtons();	
			
			// Stop monster movement
			stopMonsters();
		}
		break;
	
		case stateGameOver:
		{
			trace->event(s_fn,0,"stateMachine=stateGameOver");
			
			// Init buttons
			initButtons();	
			
			// Stop monster movement
			stopMonsters();
		}
		break;
	
		case stateReleaseNotes:
		{
			trace->event(s_fn,0,"stateMachine=stateReleaseNotes");
			game.scrollIndex=0;
		
			// Init buttons
			initButtons();		
		}
		break;
	
		case stateLocalHighScore:
		{
			trace->event(s_fn,0,"stateMachine=stateLocalHighScore");
			game.scrollIndex=0;
			
			// Init buttons
			initButtons();
		}
		break;
	
		case stateTodayHighScore:
		{
			trace->event(s_fn,0,"stateMachine=stateTodayHighScore");
			game.scrollIndex=0;
			
			// Fetch data for network thread
			char *buffer=NULL;
			buffer=tcp_get_today_highscore();
			loadTodayHighScore(buffer);		     
				
			// Init buttons
			initButtons();		
		}
		break;
	
		case stateGlobalHighScore:
		{
			trace->event(s_fn,0,"stateMachine=stateGlobalHighScore");
			game.scrollIndex=0;
			
			// Fetch data for network thread
			char *buffer=NULL;
			buffer=tcp_get_global_highscore();
			loadGlobalHighScore(buffer);
				
			// Init buttons
			initButtons();
		}
		break;
			
		case stateCredits:
		{
			trace->event(s_fn,0,"stateMachine=stateCredits");
			
			// Init buttons
			initButtons();	
		}
		break;
	 
		case stateSoundSettings:
		{
			trace->event(s_fn,0,"stateMachine=stateSoundSettings");
			
			// Init buttons
			initButtons();	
		}
		break;
		
		case stateHelp1:
		{
			trace->event(s_fn,0,"stateMachine=stateHelp1");
		
			// Init buttons
			initButtons();
		}
		break;
	
		case stateHelp2:
		{
			trace->event(s_fn,0,"stateMachine=stateHelp2");
			
			// Init buttons
			initButtons();
		}
		break;
		
		case stateHelp3:
		{
			trace->event(s_fn,0,"stateMachine=stateHelp3");
			
			game.frameCounter=0;
			game.frameDelay=0;
	
			// Init buttons
			initButtons();
		}
		break;
		
		case stateHelp4:
		{
			trace->event(s_fn,0,"stateMachine=stateHelp4");
			
			game.frameCounter=0;
			game.frameDelay=0;
			
			// Init buttons
			initButtons();
		}
		break;
	   	
		case stateGameSettings:
		{
			trace->event(s_fn,0,"stateMachine=stateGameSettings");
			
			// Init buttons
			initButtons();
		}
		break;
		
		case stateDonate:
		{
			trace->event(s_fn,0,"stateMachine=stateDonate");
			
			// Init buttons
			initButtons();
		}
		break;
		
	}
  
	// Store state
	game.prevStateMachine=game.stateMachine;
}

// -----------------------------------
// main
// -----------------------------------

/**
 * Start point of game.
 */
int main(void)
{
   const char *s_fn="main";
	
	// Init video layer
   VIDEO_Init();
	
	// Init wiimote layer
   WPAD_Init();
	
	// Wiimote is shutdown after 300 seconds of innactivity.
   WPAD_SetIdleTimeout(300); 
   WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
  
	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);
	
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	
    // Init Fat File system
    fatInitDefault();

	// Init Game variables and objects
	initApplication();
			  			
    // Init GRRLib graphics library
    GRRLIB_Init();
        	
	// Init FreeType font 
	myFont = GRRLIB_LoadTTF(font_ttf, font_ttf_size); 
			
    // To have a cool effect anti-aliasing is turned on
    GRRLIB_Settings.antialias = true;

    // Black background
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
	 
    GRRLIB_Render();

    // Seed the random-number generator with current time so that
    // the numbers will be different every time we run.
    srand(time(NULL));
	
	// Repeat forever
    while( game.stateMachine!=stateQuit )
	{			
		// draw Screen
		drawScreen();

		// Draw Wii Motion Pointers
		drawPointers();
			
		// Render screen
		GRRLIB_Render();
		
		// Process event machine
		processEvent();
		
		// Process state machine
		processStateMachine();
	}

	GRRLIB_FreeTTF(myFont);
	GRRLIB_Exit();
	
	// Stop network thread
	tcp_stop_thread();
		
	// Destroy all created objects
	destroyImages();
	destroyWeapons();
	destroyWeaponSpecs();
	destroyButtons();
	destroyMonsters();
	destroyMonsterSpecs();
	destroyMatrix();
	destroyPointers();
	destroySound();
	destroyHighScore();
	destroySettings();
	destroyGrids();
	
	// Trace last line
	trace->event(s_fn, 0,"%s %s Leaving", PROGRAM_NAME, PROGRAM_VERSION);
	
	// Destroy trace class
	destroyTrace();
	
	// Exit to loader
	exit(0);
}

// ------------------------------
// The end
// ------------------------------