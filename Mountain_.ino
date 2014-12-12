/*
  MeggyJr_Blink.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
 Blink a damned LED.
   
   
 
 Version 1.25 - 12/2/2008
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.
 http://www.evilmadscientist.com/
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
 	  
 */

 
 
 
 

/*
 * Adapted from "Blink,"  The basic Arduino example.  
 * http://www.arduino.cc/en/Tutorial/Blink
 */


#include <MeggyJrSimple.h>    // Required code, line 1 of 2.

void setup()                    // run once, when the sketch starts
{
  MeggyJrSimpleSetup();      // Required code, line 2 of 2.
}
int x = 3;
int y = 0;

int a = 2;
int b = 0;

int c = 1;
int d = 0;
boolean aA = false;
boolean bB = false;
boolean goRight = true;
int row[8] = {0,0,0,1,1,0,0,0};



void loop()                     
{
  DrawRow();
  UpdateRow();
  //Row1();
  DisplaySlate();
    delay(200);
    ClearSlate();
}

void DrawRow()
{
  for (int i = 0; i < 8; i++)
  {
    DrawPx(i,0,row[i]);
  }
}

void UpdateRow()
{
  if (goRight)
  {
    for (int i = 7; i > 0; i--)
    {
      if (row[7] != 1)
        row[i] = row[i-1];
      else goRight = false;
    }
  }
  else
  {
    for (int i = 0; i < 7; i++)
    {
      if (row[0] != 1)
      {
        row[i] = row[i+1];
        row[i+1] = 0;
      }
      else goRight = true;
    }
  }
}
