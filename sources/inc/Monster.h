/** 
 *  @file 
 *  @brief The file contain the Monster class
 *  @author wplaat
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

#ifndef MONSTER_H
#define MONSTER_H

#include "grrlib.h"

enum
{
	stateEnemyWaiting=0,
	stateEnemyMoving=1,
	stateEnemyStopped=2,
	stateEnemyDying=3,
	stateEnemyDead=4
};
	
class Monster
{
  private:
	int x;
	int targetX;
	int y;	
	int targetY;
	int alfa;
	float energy;
	float size;
	GRRLIB_texImg *image;
	int height;
	int width;
	int step;
	int delay;
	int pos;
	int index;
	bool visible;
	int grid;
	int state;
	int prevState;
	
	// Animation variables
	int frame;
	int frameCounter;
	int frameStep;
	bool moveUp;
	bool moveDown;
	bool moveRight;
	bool moveLeft;
	bool moveStop;
	
	int getFrame();
	
  public:
	// Constructor & Destructor
  	Monster();
 	~Monster();

	// Other
	void draw(int xOffset, int yOffset, float size);
	void text(void);
	void move(void);
	
	// Setters
	void setImage(GRRLIB_texImg *image);	
	void setStep(int step);
	void setDelay(int delay);
	void setEnergy(int energy);
	void setIndex(int index);
	void setGrid(int grid);	
	void setState(int state);
	void setMove(bool enabled);

	// Getters
	int getX(void);
	int getY(void);
	int getAlfa(void);
	float getSize(void);
	int getStep(void);
	int getEnergy(void);
	int getGrid(void);
	int getIndex(void);
	int getState(void);
};

#endif
