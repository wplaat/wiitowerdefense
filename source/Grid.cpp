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
**  Foundation, Inc., 5 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <mxml.h>

#include "General.h"
#include "GRRLIB.h"
#include "Trace.h"
#include "Grid.h"

int maxLines;
int maxLocations;

char gridData[MAX_GRID_Y][MAX_GRID_X];

typedef struct 
{
	int x;
	int y;
}
location;

location locationList[MAX_GRID_Y*MAX_GRID_X];

extern Trace trace;

// ------------------------------
// Constructor
// ------------------------------

Grid::Grid()
{
	const char *s_fn="Grid::Grid";
	trace.event(s_fn,0,"enter");
  
	trace.event(s_fn,0,"leave [void]");  
}
	
// ------------------------------
// Destructor
// ------------------------------

Grid::~Grid()
{
	const char *s_fn="Grid::~Grid";
	trace.event(s_fn,0,"enter");

	trace.event(s_fn,0,"leave [void]");  
}

// ------------------------------
// Others
// ------------------------------

void Grid::parseGrid(void)
{
	const char *s_fn="Grid::parseGrid";
	trace.event(s_fn,0,"enter");
   
	int x,y;
	char temp[MAX_GRID_Y][MAX_GRID_X];
	boolean ready=false;
	maxLocations=0;
   
	// Make copy of grid data and store start position
	for (y=0;y<MAX_GRID_Y;y++)
	{
		for (x=0;x<MAX_GRID_X;x++)
		{
			if (gridData[y][x]=='~') 
			{
				// Replace water element with glass element
				// else monsters walk over water.
				temp[y][x]='0';
			}
			else 
			{
				// Store other grid data elements
				temp[y][x]=gridData[y][x];
			
				// Store start position	of grid
				if (temp[y][x]=='*')
				{
					locationList[maxLocations].x=x;
					locationList[maxLocations].y=y;
					maxLocations++;
					temp[y][x]='0';
				}
			}
		}
	}
   
	// Create walking plan for monsters
	while (!ready)
	{
		ready=true;
		x=locationList[maxLocations-1].x;
		y=locationList[maxLocations-1].y;
		
		if (temp[y][x+1]!='0')
		{
			locationList[maxLocations].x=(x+1);
			locationList[maxLocations].y=y;
			maxLocations++;
			if (temp[y][x+1]!='#') ready=false; else ready=true;
			if (temp[y][x+1]!='7') temp[y][x+1]='0';
		}
		else if (temp[y][x-1]!='0')
		{
			locationList[maxLocations].x=(x-1);
			locationList[maxLocations].y=y;
			maxLocations++;
			if (temp[y][x-1]!='#') ready=false; else ready=true;
			if (temp[y][x-1]!='7') temp[y][x-1]='0';
		}
		else if (temp[y+1][x]!='0')
		{
			locationList[maxLocations].x=x;
			locationList[maxLocations].y=(y+1);
			maxLocations++;
			if (temp[y+1][x]!='#') ready=false; else ready=true;
			if (temp[y+1][x]!='7') temp[y+1][x]='0';
		}
		else if (temp[y-1][x]!='0')
		{
			locationList[maxLocations].x=x;
			locationList[maxLocations].y=(y-1);
			maxLocations++;
			if (temp[y-1][x]!='#') ready=false; else ready=true;
			if (temp[y-1][x]!='7') temp[y-1][x]='0';
		}
	}
	
	trace.event(s_fn,0,"leave [maxLocations=%d]",maxLocations);
}

void Grid::loadGrid(const char* filename)
{
   const char *s_fn="Grid::initGrid";
   trace.event(s_fn,0,"enter [filename=%s]",filename);
   
   FILE *fp;
   mxml_node_t *tree =NULL;
   mxml_node_t *group;   
   const char  *pointer;

   maxLines=0;
   
   /* Load our xml file! */
   fp = fopen(filename, "r");
   if (fp!=NULL)
   {   
      tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
      fclose(fp);
   }
    
   // Read Grid Lines
   for (group = mxmlFindElement(tree, tree, "line", NULL, NULL, MXML_DESCEND);
        group != NULL;
        group = mxmlFindElement(group, tree, "line", NULL, NULL, MXML_DESCEND))
   {		 	  	  
      pointer=mxmlElementGetAttr(group,"data");
      if (pointer!=NULL) strcpy(gridData[maxLines],pointer);  
	  
	  maxLines++;
   }
   
   mxmlDelete(group);
   mxmlDelete(tree);
   trace.event(s_fn,0,"leave [maxLines=%d]",maxLines);
}

void Grid::draw(void)
{
   int x;
   int y;
   int baseX=0;
   int baseY=0;
   
   // Parse
   for (y=0; y<MAX_GRID_Y; y++)
   {	
		for (x=0; x<MAX_GRID_X; x++)
		{
			switch (gridData[y][x])
			{				
				case '*': 
				case '0': 
					// Draw Grass image
					GRRLIB_DrawImg( x*32, y*32, image5, 0, 1.0, 1.0, IMAGE_COLOR );
					break;
	
				case '1':
					// Draw basic road
					GRRLIB_DrawImg( (x*32)+32, y*32, image4, 90, 1.0, 1.0, IMAGE_COLOR );
					break;	

				case '2':
					// Draw basic road
					GRRLIB_DrawImg( (x*32), (y*32), image4, 0, 1.0, 1.0, IMAGE_COLOR );
					break;	
					
				case '3':
					// Draw angle road
					GRRLIB_DrawImg( (x*32), (y*32), image3, 0, 1.0, 1.0, IMAGE_COLOR );
					break;	

				case '4':
					// Draw angle road
					GRRLIB_DrawImg( (x*32)+32, y*32, image3, 90, 1.0, 1.0, IMAGE_COLOR );
					break;	
					
				case '5':
					// Draw angle road
					GRRLIB_DrawImg( (x*32)+32, (y*32)+32, image3, 180, 1.0, 1.0, IMAGE_COLOR );
					break;	
					
				case '6':
					// Draw angle road
					GRRLIB_DrawImg( (x*32), (y*32)+32, image3, 270, 1.0, 1.0, IMAGE_COLOR );
					break;	
			
				case '7':
					// Draw Cross road
					GRRLIB_DrawImg( (x*32), (y*32), image2, 0, 1.0, 1.0, IMAGE_COLOR );
					break;	
			
				case '8':
					// Draw T-section road
					GRRLIB_DrawImg( (x*32), (y*32), image1, 0, 1.0, 1.0, IMAGE_COLOR );
					break;	

				case '9':
					// Draw T-section road
					GRRLIB_DrawImg( (x*32), (y*32), image1, 90, 1.0, 1.0, IMAGE_COLOR );
					break;
		
				case '~':
					// Draw water image 
					GRRLIB_DrawImg( (x*32), (y*32), imageWater, 0, 1.0, 1.0, IMAGE_COLOR );
					break;

				case '=':
					// Draw bridge image
					GRRLIB_DrawImg( (x*32), (y*32), imageBridge, 0, 1.0, 1.0, IMAGE_COLOR );
					break;

				case 'H':
					// Draw bridge image
					GRRLIB_DrawImg( (x*32)+32, (y*32), imageBridge, 90, 1.0, 1.0, IMAGE_COLOR );
					break;
					
				case '#':
					// Draw grass image
					GRRLIB_DrawImg( (x*32), (y*32), image5, 0, 1.0, 1.0, IMAGE_COLOR );
					
					// Store Base Position for later use!
					baseX=x;
					baseY=y;
					break;
			}
		}
		
	}
	
	// Draw base 
	GRRLIB_DrawImg( (baseX*32)-32, (baseY*32)+5, imageBase, 0, 1.0, 1.0, IMAGE_COLOR );
}

// Load grid map and parse it for monster movement.
void Grid::create(const char* filename)
{
	const char *s_fn="Grid::render";
	trace.event(s_fn,0,"enter [filename=%s]",filename);
   
    loadGrid(filename);
	parseGrid();
	
	trace.event(s_fn,0,"leave [void]");  
}
			
// ------------------------------
// Setters
// ------------------------------

void Grid::setImageRoad1(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageRoad1";
   trace.event(s_fn,0,"enter");
   
   image1 = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageRoad2(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageRoad2";
   trace.event(s_fn,0,"enter");
   
   image2 = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageRoad3(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageRoad3";
   trace.event(s_fn,0,"enter");
   
   image3 = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageRoad4(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageRoad4";
   trace.event(s_fn,0,"enter");
   
   image4 = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageRoad5(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageRoad5";
   trace.event(s_fn,0,"enter");
   
   image5 = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageBase(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageBase";
   trace.event(s_fn,0,"enter");
   
   imageBase = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageWater(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageWater";
   trace.event(s_fn,0,"enter");
   
   imageWater = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setImageBridge(GRRLIB_texImg *image)
{
   const char *s_fn="Pointer::setImageBrige";
   trace.event(s_fn,0,"enter");
   
   imageBridge = image;
   
   trace.event(s_fn,0,"leave [void]");
}

void Grid::setLevel(int level1)
{
	const char *s_fn="Grid::setLevel";
	trace.event(s_fn,0,"enter [level=%d]",level1);
	
   if ((level>=0) && (level<=MAX_LEVEL))
   {
      level = level1;
   }
   
   trace.event(s_fn,0,"leave [void]");  
}

// ------------------------------
// Getters
// ------------------------------

int Grid::getLocationX(int pos)
{
    if (pos<maxLocations)
	{
		return (locationList[pos].x*32);
	}
	return -1;
}

int Grid::getLocationY(int pos)
{
    if (pos<maxLocations)
	{
		return (locationList[pos].y*32);
	}
	return -1;
}


int Grid::getMaxLocations(void)
{
   return maxLocations;
}

// ------------------------------
// The End
// ------------------------------