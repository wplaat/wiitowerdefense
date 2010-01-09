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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Weapon.h" 
#include "Trace.h" 
#include "Sound.h" 

// ------------------------------
// Variables
// ------------------------------

extern GXRModeObj   *rmode;
extern Game 		game; 
extern Trace    	*trace;
extern Sound    	*sound;

// ------------------------------
// Constructor 
// ------------------------------

Weapon::Weapon()
{
   const char *s_fn="Weapon::Weapon";
   trace->event(s_fn,0,"enter");
   
   x=0;
   y=0;
   
   height=0;
   width=0;
   
   alfa=255;   
   angle=0;
   targetAngle=0;
   delay=0.0;
   selected=false;
   index=0;
   
   fireDelay=0;
   monsterX=0;
   monsterY=0;
   
   power=0;
   range=0;   
   rate=0.0;
   
   maxPower=0;
   maxRange=0;   
   maxRate=0;
   
   powerPrice=0;
   rangePrice=0;
   ratePrice=0;
      
   trace->event(s_fn,0,"leave");
}

// ------------------------------
// Destructor
// ------------------------------

Weapon::~Weapon()
{
   const char *s_fn="Weapon::~Weapon";
   trace->event(s_fn,0,"enter");
  
   trace->event(s_fn,0,"leave");
}

// ------------------------------
// Others
// ------------------------------
	
void Weapon::draw()
{
	// Draw weapon range circle
	if (selected)
	{	
		GRRLIB_Circle(x+16, y+16, range, IMAGE_COLOR3, 1);
	}
	
	// Draw fire trail 
	if (--fireDelay>0)
	{
		GRRLIB_Line( x+16, y+16, monsterX+16, monsterY+16, GRRLIB_LIGHTRED);								
	}
	
	// Draw Weapon on screen
	GRRLIB_DrawImg( x, y, image, angle, 1, 1, IMAGE_COLOR );		
	
	// Draw reload time bar
	float proc = ((rate-delay) / rate) * 21.0;
	GRRLIB_Rectangle(x+5, y+34, 20, 4, GRRLIB_BLACK, 0);
	GRRLIB_Rectangle(x+5, y+35, proc, 2, GRRLIB_GREEN, 1);
}

void Weapon::text()
{
	char tmp[5];
	int size=12;
		
	// Draw reload delay on screen
	sprintf(tmp, "%2.0f", (delay/10.0));
	GRRLIB_Printf2(x+6, y-16, tmp, size, 0x000000); 		
}

void Weapon::fire(Monster *monsters[MAX_MONSTERS])
{
	if (delay>0) 
	{
		delay--;
	}
	else
	{
		// fire
		for (int i=0; i<MAX_MONSTERS; i++)
		{			
			if (monsters[i]!=NULL)
			{
				float distance= 
					sqrt( ( (monsters[i]->getX()-x) * (monsters[i]->getX()-x) ) + 
						  ( (monsters[i]->getY()-y) * (monsters[i]->getY()-y) ) );
						  				
				if (distance<range)
				{					
					// Store monster position were fired on!
					monsterX=monsters[i]->getX();
					monsterY=monsters[i]->getY();
					fireDelay=15;
					
					switch(type)
					{
						case 0: // Gun
								sound->effect(SOUND_GUN);	
								break;
			
						case 1: // Rifle
								sound->effect(SOUND_RIFLE);	
								break;
								
						case 2: // Canon
								sound->effect(SOUND_CANON);	
								break;
					
						case 3: // Missle
								sound->effect(SOUND_MISSLE);	
								break;
								
						case 4: // Laser
								sound->effect(SOUND_LASER);	
								break;
								
						default: // Nuck
								sound->effect(SOUND_LASER);	
								break;
					}
								
					int energyLeft=monsters[i]->hit(power);
										
					if (energyLeft==0)
					{						
						delete monsters[i];
						monsters[i]=NULL;
						
						// Dead
						sound->effect(SOUND_DEAD);	
					}
					
					// Receive score and cash for shooting the monster
					game.score+=power;
					game.cash+=power;
					
					// Reset delay counter;
					delay=rate;
					break;
				}
			}
		}
	}
}

void Weapon::move(void)
{  
	angle++;
	if (angle>MAX_ANGLE) angle=0;
}

int Weapon::upgrade(int type)
{
	const char *s_fn="Weapon::upgrade";
	trace->event(s_fn,0,"enter [type=%d]",type);
	
	switch (type)
	{
		// Power upgrade
		case 0:	if ((game.cash>=powerPrice) && (power<maxPower))
				{
					power+=POWER_UPGRADE_STEP;
					trace->event(s_fn,0,"Weapon %d upgrade power to %d",index, power);
					game.cash-=powerPrice;
					powerPrice=powerPrice*2;					
					sound->effect(SOUND_UPGRADE);
				}
				break;

		// Range upgrade
		case 1:	if ((game.cash>=rangePrice) && (range<maxRange))
				{
					range+=RANGE_UPGRADE_STEP;
					trace->event(s_fn,0,"Weapon %d upgrade range to %d",index, range);
					game.cash-=rangePrice;
					rangePrice=rangePrice*2;
					sound->effect(SOUND_UPGRADE);
				}
				break;

		// Rate upgrade		
		case 2:	if ((game.cash>=ratePrice) && (rate>maxRate))
				{
					rate-=(RATE_UPGRADE_STEP*AVERAGE_FPS);
					trace->event(s_fn,0,"Weapon %d upgrade rate to %d",index, rate);
					game.cash-=ratePrice;
					ratePrice=ratePrice*2;
					sound->effect(SOUND_UPGRADE);
				}
				break;
	}	
	trace->event(s_fn,0,"leave [0]");
	return 0;
}

bool Weapon::onSelect(int x1, int y1)
{   
	const char *s_fn="Weapon::onSelect";
	if ( (x1>=x) && (x1<=(x+width)) && (y1>=y) && (y1<=(y+height)) )
	{      	 	
		trace->event(s_fn,0,"Weapon %d selected", index);
		
		// Click
		sound->effect(SOUND_CLICK);	
		return true;
	}
	return false;
}

// ------------------------------
// Setters 
// ------------------------------

void Weapon::setX(int x1)
{
	const char *s_fn="Weapon::setX";
	trace->event(s_fn,0,"%d",x1);
   
	if ((x1>=0) && (x1<=MAX_HORZ_PIXELS))
	{
		x = x1;
	}
}

void Weapon::setY(int y1)
{
   const char *s_fn="Weapon::setY";
   trace->event(s_fn,0,"%d",y1);
   
   if ((y1>=0) && (y1<=rmode->xfbHeight))
   {
      y = y1;
   }
}
				
void Weapon::setImage(GRRLIB_texImg *image1)
{
   const char *s_fn="Weapon::setImage";
   trace->event(s_fn,0,"data");
   
   image = image1;
   
   height=image->h;
   width=image->w;
}

void Weapon::setPower(int power1)
{
   const char *s_fn="Weapon::setPower";
   trace->event(s_fn,0,"%d",power1);
   
   power=power1;
}

void Weapon::setRange(int range1)
{
   const char *s_fn="Weapon::setRange";
   trace->event(s_fn,0,"%d",range1);
   
   range=range1;
 }

void Weapon::setRate(int rate1)
{
   const char *s_fn="Weapon::setRate";
   trace->event(s_fn,0,"%d",rate1);
   
   rate=rate1*AVERAGE_FPS;
   delay=rate;
}


void Weapon::setMaxPower(int maxPower1)
{
   const char *s_fn="Weapon::setmaxPower";
   trace->event(s_fn,0,"%d",maxPower1);
   
   maxPower=maxPower1;
}

void Weapon::setMaxRange(int maxRange1)
{
   const char *s_fn="Weapon::setMaxRange";
   trace->event(s_fn,0,"%d",maxRange1); 
   
   maxRange=maxRange1;
 }

void Weapon::setMaxRate(int maxRate1)
{
   const char *s_fn="Weapon::setMaxRate";
   trace->event(s_fn,0,"%d",maxRate1);
   
   maxRate=maxRate1*AVERAGE_FPS;
}

void Weapon::setPowerPrice(int price)
{
   const char *s_fn="Weapon::setPowerPrice";
   trace->event(s_fn,0,"%d",price);
   
   powerPrice=price;
}

void Weapon::setRangePrice(int price)
{
	const char *s_fn="Weapon::setRangePrice";
	trace->event(s_fn,0,"%d",price);
   
	rangePrice=price;
}

void Weapon::setRatePrice(int price)
{
	const char *s_fn="Weapon::setRatePrice";
	trace->event(s_fn,0,"%d",price);
	
	ratePrice=price;
}

void Weapon::setIndex(int index1)
{
	const char *s_fn="Weapon::setIndex";
	trace->event(s_fn,0,"%d",index1);
	
	index=index1;
}

void Weapon::setSelected(int selected1)
{
	selected=selected1;
}

void Weapon::setType(int type1)
{
	type=type1;
}

// ------------------------------
// Getters 
// ------------------------------

int Weapon::getPowerPrice(void)
{
	return powerPrice;
}

int Weapon::getRangePrice(void)
{
	return rangePrice;
}

int Weapon::getRatePrice(void)
{
	return ratePrice;
}
	
bool Weapon::isPowerUpgradeble(void)
{
	return power<maxPower;
}

bool Weapon::isRangeUpgradeble(void)
{
	return range<maxRange;
}

bool Weapon::isRateUpgradeble(void)
{
	return rate>maxRate;
}
// ------------------------------
// The end
// ------------------------------