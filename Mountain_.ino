/*
  MeggyJr_Stacker.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
 Moves three dots from side to side for a Stacker type game.
   
   
 
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
 
 /* 
 My idea for Stacker is to create a single array of ints, with each index corresponding to one row.
 We'll designate one of the dots (probably the center dot of the three-dot segments) 
 to be the reference point and draw the dots to either side of that dot when we 
 draw the segment. The boundaries for the reference point will be set so that 
 it won't go over. When we draw the array we'll just read the value of the index then draw each one and update
 it accordingly even if it's blank.
 */

#include <MeggyJrSimple.h>    // Required code, line 1 of 2.

int rows[9] = {1,0,0,0,0,0,0,0,0}; // The numbers in this array correspond to the column where the reference point starts.
boolean movingRight[8]; // the values in this array provide the direction each of the rows is moving in.
int level = 0; // the highest row with dots at the moment.
int win = 0;
int loose = 0;
boolean gameOver = false;

void setup()                    // run once, when the sketch starts
{
  MeggyJrSimpleSetup(); 
Serial.begin(9600);
  // Required code, line 2 of 2.
//  rows[0] = [random(6)+1]; //picks a random column in the array to be between 1-6
  InitRows(); // Just called once. It creates random directions for each row.
}

void loop()                     // run over and over again
{
  if (gameOver == false)
    play();
  else 
  {
    if (level > 7) // if you get to the top
    {
      winScreen(); //win
    }
    else
    {
      loseScreen(); //lose
    }
   NewGame(); //new game
  }
}

void play()
{
 DrawPx(0,6,White); //mountains drawn in for some scenery.
 DrawPx(0,7,White);
 DrawPx(0,4,Green);
 DrawPx(0,5,Green);
 DrawPx(0,3,Green);
 DrawArray();
 DisplaySlate();
 delay(60);
 ClearSlate();
 CheckButtonsPress();
 if (Button_A)
  {     
    if (lineUp() == true) //if dots line up
    {
     level++;    //stop dots
     rows[level]++;  //have dots appear above
     Tone_Start(9000, 50); //noise
    }
    else
    {
     level++;    //stop dots
     rows[level]++;  //have dots appear above
     gameOver = true; //end the game
    }
  }
 UpdateArray();  
} 
void DrawArray() // This draws three-dot segments and then two-dot segments.
{
  for (int i = 0; i < 3; i++) // First three rows are three-dot segments
  {
    if (rows[i] > 0) // If there is something in the row
    {
      DrawPx(rows[i]-1,i,1); // Dot to left
      DrawPx(rows[i],i,1); // Reference dot
      DrawPx(rows[i]+1,i,1); // Dot to right      
    }
  }  
    for (int i = 3; i < 8; i++) // Upper rows are two-dot segments
  {
    if (rows[i] > 0) // if there is something in the row
    {
      DrawPx(rows[i],i,1); // Reference dot
      DrawPx(rows[i]+1,i,1); // Dot to right
    }
  }   
}

void UpdateArray()
{
  for (int i = level; i < 8; i++) // This updates the reference points for everything except one case
  {
    if (rows[i] > 0) // If there is something in the row
    {
      if (rows[i] > 5 && movingRight[i] == true)
      {
        movingRight[i] = !movingRight[i]; // flips the direction
        rows[i] = 5;
      }
      else // Here's where we need to adjust for the two- and three-dot segments
      {
        if (i < 8) 
        {
          if (rows[i] < 2 && movingRight[i] == false) // if the VALUE at that index is less than 2 then it's at the edge
          {
          movingRight[i] = !movingRight[i]; // flips the direction
          rows[i] = 2;
          }      
          else 
          {
          Serial.println("Two-dot if-else reached."); // help us de-bug the game
          if (rows[i] < 1 && movingRight[i] == false) // if the VALUE at that index is less than 1 then it's at the edge
          {
            Serial.println("At edge");
            movingRight[i] = !movingRight[i]; // flips the direction
            rows[i] = 1;
          }
          }
          if (movingRight[i] == true)
          {
            Serial.println("Added one to i"); //de-bug tool
            rows[i]++;
          }
          else
          {
           Serial.println("Took one away from i"); //de-bug tool
           rows[i]--;
          }
        }
      }
    }
  }
}
void InitRows() // This basically fills the movingRight array with trues and falses
{               // We'll say false stands for moving left and true stands for moving right.
  for (int i = 0; i < 8; i++)
  {
    if (random(1) == 0) // roll a random number
      movingRight[i] = false; // if it's zero, value is false
    else
      movingRight[i] = true; // otherwise value is true
  }
}
void PrintArray()
{
  for (int i = 0; i < 8; i++)
  {
    Serial.print("i is "); //de-bug tool
    Serial.println(i);
    Serial.println(rows[i]);
    Serial.println();
  } 
}
void winScreen()   //win screen. This is what you see if you beat the game.
  {
    ClearSlate();
    DrawPx(1,0,Green);
    DrawPx(2,0,Green);
    DrawPx(3,0,Green);
    DrawPx(4,0,Green);
    DrawPx(5,0,Green);
    DrawPx(6,0,Green);
    DrawPx(7,0,Green);  
    DrawPx(2,1,White);
    DrawPx(3,1,White);
    DrawPx(4,1,White);
    DrawPx(5,1,White);
    DrawPx(3,2,Blue);
    DrawPx(5,2,Blue);
    DrawPx(4,3,Blue);
    DrawPx(4,4,Blue);
    DrawPx(5,4,Blue);
    DrawPx(3,4,Blue);    
    DrawPx(4,5,Red);
    DrawPx(0,7,Yellow);
    DrawPx(0,6,Yellow);
    DrawPx(1,6,Yellow);
    DrawPx(1,7,Yellow);         
    Tone_Start(18183, 50);    
    DisplaySlate();
    delay(500);
    ClearSlate();
    DrawPx(1,0,Green);
    DrawPx(2,0,Green);
    DrawPx(3,0,Green);
    DrawPx(4,0,Green);
    DrawPx(5,0,Green);
    DrawPx(6,0,Green);
    DrawPx(7,0,Green);  
    DrawPx(2,1,Green);
    DrawPx(3,1,Green);
    DrawPx(4,1,Green);
    DrawPx(5,1,Green);
    DrawPx(3,3,Blue);
    DrawPx(5,3,Blue);
    DrawPx(4,4,Blue);
    DrawPx(4,5,Blue);
    DrawPx(5,5,Blue);
    DrawPx(3,5,Blue);    
    DrawPx(4,6,Red);
    DrawPx(0,7,Yellow);
    DrawPx(0,6,Yellow);
    DrawPx(1,6,Yellow);
    DrawPx(1,7,Yellow);
    Tone_Start(6000, 50);
    DisplaySlate();
    delay(500);          
    ClearSlate();
    DrawPx(1,0,Green);
    DrawPx(2,0,Green);
    DrawPx(3,0,Green);
    DrawPx(4,0,Green);
    DrawPx(5,0,Green);
    DrawPx(6,0,Green);
    DrawPx(7,0,Green);  
    DrawPx(2,1,Green);
    DrawPx(3,1,Green);
    DrawPx(4,1,Green);
    DrawPx(5,1,Green);
    DrawPx(3,2,Blue);
    DrawPx(5,2,Blue);
    DrawPx(4,3,Blue);
    DrawPx(4,4,Blue);
    DrawPx(5,4,Blue);
    DrawPx(3,4,Blue);    
    DrawPx(4,5,Red);
    DrawPx(0,7,Yellow);
    DrawPx(0,6,Yellow);
    DrawPx(1,6,Yellow);
    DrawPx(1,7,Yellow);         
    Tone_Start(9000, 50);          
    DisplaySlate();
    delay(500);          
    ClearSlate();
    DrawPx(1,0,Green);
    DrawPx(2,0,Green);
    DrawPx(3,0,Green);
    DrawPx(4,0,Green);
    DrawPx(5,0,Green);
    DrawPx(6,0,Green);
    DrawPx(7,0,Green);  
    DrawPx(2,1,Green);
    DrawPx(3,1,Green);
    DrawPx(4,1,Green);
    DrawPx(5,1,Green);
    DrawPx(3,3,Blue);
    DrawPx(5,3,Blue);
    DrawPx(4,4,Blue);
    DrawPx(4,5,Blue);
    DrawPx(5,5,Blue);
    DrawPx(3,5,Blue);    
    DrawPx(4,6,Red);
    DrawPx(0,7,Yellow);
    DrawPx(0,6,Yellow);
    DrawPx(1,6,Yellow);
    DrawPx(1,7,Yellow);
    Tone_Start(6000, 50);
    DisplaySlate();
    delay(500);                    
    ClearSlate();
    DrawPx(1,0,Green);
    DrawPx(2,0,Green);
    DrawPx(3,0,Green);
    DrawPx(4,0,Green);
    DrawPx(5,0,Green);
    DrawPx(6,0,Green);
    DrawPx(7,0,Green);  
    DrawPx(2,1,Green);
    DrawPx(3,1,Green);
    DrawPx(4,1,Green);
    DrawPx(5,1,Green);
    DrawPx(3,2,Blue);
    DrawPx(5,2,Blue);
    DrawPx(4,3,Blue);
    DrawPx(4,4,Blue);
    DrawPx(5,4,Blue);
    DrawPx(3,4,Blue);    
    DrawPx(4,5,Red);
    DrawPx(0,7,Yellow);
    DrawPx(0,6,Yellow);
    DrawPx(1,6,Yellow);
    DrawPx(1,7,Yellow);
    Tone_Start(18183, 50);            
    DisplaySlate();
    delay(500);         
    ClearSlate();
    DrawPx(1,6,Green);
    DrawPx(2,6,Green);
    DrawPx(3,6,Green);
    DrawPx(1,5,Green);
    DrawPx(1,4,Green);
    DrawPx(2,4,Green);
    DrawPx(3,4,Green);  
    DrawPx(3,5,Green);      
    DrawPx(4,6,Green);
    DrawPx(4,5,Green);
    DrawPx(4,4,Green);
    DrawPx(5,4,Green);
    DrawPx(6,4,Green);
    DrawPx(6,5,Green);
    DrawPx(6,6,Green);
    DrawPx(5,6,Green);
    DrawPx(4,6,Green);      
    DrawPx(1,2,Green);
    DrawPx(1,1,Green);
    DrawPx(2,1,Green);
    DrawPx(3,1,Green);
    DrawPx(4,1,Green);
    DrawPx(5,1,Green);
    DrawPx(6,1,Green);
    DrawPx(6,2,Green);
    DrawPx(5,2,Green);
    DrawPx(4,2,Green);
    DrawPx(3,2,Green);
    DrawPx(2,2,Green);
    Tone_Start(18183, 50);
    delay(100);
    Tone_Start(15000, 50);
    delay(100);
    Tone_Start(12000, 50);
    delay(100);
    Tone_Start(9000, 50);
    delay(100);
    Tone_Start(6000, 50);
    delay(100);                    
    DisplaySlate();
    delay(1000);
}
void loseScreen()   // this is what you see when you lose the game.
{
   ClearSlate();
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(6,6,Blue);
   DrawPx(5,4,Blue);
   DrawPx(7,4,Blue);
   DrawPx(6,5,Blue);
   DrawPx(7,6,Blue);
   DrawPx(7,6,Blue);
   DrawPx(5,6,Blue);    
   DrawPx(6,7,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(3000, 50);          
   DisplaySlate();
   delay(150);
   ClearSlate();
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);          
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);                    
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(6,5,Blue);
   DrawPx(5,3,Blue);
   DrawPx(7,3,Blue);
   DrawPx(6,4,Blue);
   DrawPx(7,5,Blue);
   DrawPx(7,5,Blue);
   DrawPx(5,5,Blue);    
   DrawPx(6,6,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(5000, 50);          
   DisplaySlate();
   delay(150); 
   ClearSlate();
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);          
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);                     
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(6,4,Blue);
   DrawPx(5,2,Blue);
   DrawPx(7,2,Blue);
   DrawPx(6,3,Blue);
   DrawPx(7,4,Blue);
   DrawPx(7,4,Blue);
   DrawPx(5,4,Blue);    
   DrawPx(6,5,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(7000, 50);          
   DisplaySlate();
   delay(150);
   ClearSlate();     
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);                    
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);          
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(6,3,Blue);
   DrawPx(5,1,Blue);
   DrawPx(7,1,Blue);
   DrawPx(5,3,Blue);
   DrawPx(7,3,Blue);
   DrawPx(6,2,Blue);
   DrawPx(7,3,Blue);
   DrawPx(7,3,Blue);
   DrawPx(5,3,Blue);    
   DrawPx(6,4,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(9000, 50);   
   DisplaySlate();
   delay(150);
   ClearSlate();  
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);                  
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);   
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(5,0,Blue);
   DrawPx(7,0,Blue);
   DrawPx(6,2,Blue);
   DrawPx(5,2,Blue);
   DrawPx(7,2,Blue);
   DrawPx(6,1,Blue);
   DrawPx(7,2,Blue);
   DrawPx(7,2,Blue);
   DrawPx(5,2,Blue);    
   DrawPx(6,3,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(11000, 50);          
   DisplaySlate();
   delay(150);          
   ClearSlate();    
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);          
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);                     
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(5,1,Blue);
   DrawPx(7,1,Blue);
   DrawPx(6,1,Blue);          
   DrawPx(6,0,Blue);
   DrawPx(7,1,Blue);
   DrawPx(7,1,Blue);
   DrawPx(5,1,Blue);    
   DrawPx(6,2,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);   
   Tone_Start(13000, 50);         
   DisplaySlate();
   delay(150);
   ClearSlate();               
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);  
   DrawPx(0,0,Green); 
   DrawPx(3,2,White);
   DrawPx(4,2,White);                     
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);          
   DrawPx(6,0,Blue);
   DrawPx(5,0,Blue);
   DrawPx(7,0,Blue);          
   DrawPx(7,0,Blue);          
   DrawPx(5,0,Blue);    
   DrawPx(6,1,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(14000, 50);         
   DisplaySlate();
   delay(150);
   ClearSlate();   
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);         
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);                             
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);                       
   DrawPx(6,0,Red);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);         
   Tone_Start(16000, 50);         
   DisplaySlate();
   delay(150);
   ClearSlate(); 
   DrawPx(1,0,Green);
   DrawPx(2,0,Green);
   DrawPx(3,0,Green);
   DrawPx(4,0,Green);                 
   DrawPx(0,0,Green);
   DrawPx(3,2,White);
   DrawPx(4,2,White);          
   DrawPx(2,1,Green);
   DrawPx(3,1,Green);
   DrawPx(4,1,Green);
   DrawPx(0,7,Yellow);
   DrawPx(0,6,Yellow);
   DrawPx(1,6,Yellow);
   DrawPx(1,7,Yellow);
   Tone_Start(18000, 50);          
   DisplaySlate();
   delay(250);
   ClearSlate();        
   DrawPx(0,7,Red);
   DrawPx(1,6,Red);
   DrawPx(2,5,Red);
   DrawPx(3,4,Red);
   DrawPx(4,3,Red);
   DrawPx(5,2,Red);
   DrawPx(6,1,Red);
   DrawPx(7,0,Red);          
   DrawPx(0,0,Red);
   DrawPx(1,1,Red);
   DrawPx(2,2,Red);
   DrawPx(3,3,Red);
   DrawPx(4,4,Red);         
   DrawPx(5,5,Red);
   DrawPx(6,6,Red);
   DrawPx(7,7,Red);
   Tone_Start(18183, 1000);          
   DisplaySlate();
   delay(1000);          
   Serial.println("Game over!"); // de-bug tool
  //animates and delays for a while
}        
boolean lineUp()
{
/*
  for the row we are on rows[level]
  
  if the reference point is less than the length of the segment from the previous reference point, return true.
  Otherwise, return false.
  */
  if (level < 1) return true;   // need this for level = 0, or game will not work
  int segmentLength = 2;       // changed this to 2 so you have to get all the blocks on top of each other
  if ( rows[level] > 2 ) 
    segmentLength = 1; 
  int distance = (rows[level] - rows[level-1]);
  if (distance < 0) // if the distance is a negative number
    distance = distance * -1;   // another way of doing the absolute value 
  if (distance < segmentLength) return true; // if the length is more than the distance, then the blocks are not falling off. Allows you to increase level.
  else return false;  // if the blocks are too far off, end the game
}    
void NewGame() // new game
{  
ClearSlate();
for (int i = 0; i <= level; i++)
{
  rows[i] = 0; // resets the rows[i] so you can play again.
}
rows[0] = 1; 
level = 0;// resets level back to 0 so you can only see bottom row of blocks.
win = 0;
loose = 0;
gameOver = false;    
  DisplaySlate(); 
}    


