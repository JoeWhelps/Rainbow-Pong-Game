//Lab 11, Joe Whelpley
//Read the 'readme.txt' to get the program description.
//12/10/23

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "gfx.h"

// Structs in order to preserve universal variables.
typedef struct {
    double x;
    double y;
    double dx;
    double dy;
    int radius;
} Circle;

typedef struct {
    double x;
    double y;
    int length;
    int width;
} L_Block;

typedef struct {
    double x;
    double y;
    int length;
    int width;
} R_Block;

void gameplay(int, int, Circle, L_Block, R_Block);
void makeRectangle(double x, double y, int length, int width);
void colorchange();

int main()
{ // Declare variables
  int wid = 1000, ht = 600; // Screen Dimensions
  Circle cir; // calling the structs
  L_Block L_B;
  R_Block R_B;
  cir.x = wid/2;  // initial positions for al the struct variables
  cir.y = ht/2;
  cir.dx = 1.2;
  cir.dy = 1.2;
  cir.radius = 8;
  L_B.length = 75;
  L_B.width = 25;
  L_B.y = ht/2;
  L_B.x = wid/5;
  R_B.length = 75;
  R_B.width = 25;
  R_B.y =  ht/2;
  R_B.x = 4*wid/5;


  srand(time(0)); // seed generator

  gfx_open(wid, ht, "Ball Bounce Simulator");

  gameplay(wid, ht,cir, L_B, R_B); // Function that actually operates the game.

  return 0;
}
// function that mainly runs the program
void gameplay(int wid, int ht, Circle cir, L_Block L_B, R_Block R_B)
{
srand(time(0));
float speed_change= 1.05; // this determines how fast the ball increases it's speed after encountering a boundry
 while(1) {
    gfx_clear();

    if (gfx_event_waiting()){ // loop if the user enters any input
        char c = gfx_wait();
        // loop if the user clicks on the screen
        if (c ==1) {
        // New position
        cir.x = gfx_xpos();
        cir.y = gfx_ypos();


        // If mouse puts circle outside the space these if statements move it back into place.
        if(cir.x + cir.radius >= wid) cir.x = wid - (1 + cir.radius);
        if(cir.x - cir.radius <= 0) cir.x = 1+cir.radius;
        if(cir.y + cir.radius >= ht) cir.y = ht - (1+ cir.radius);
        if(cir.y - cir.radius <= 0) cir.y = 1+cir.radius;

        }
        if ((c== 's')&&(L_B.y+L_B.length/2<ht))  L_B.y += 8;    // Defines the buttons that move the boxes up and down in the game
        if ((c== 'w')&&(L_B.y-L_B.length/2>0))   L_B.y -= 8;
        if ((c== 'j')&&(R_B.y+R_B.length/2<ht))  R_B.y += 8;
        if ((c== 'i')&&(R_B.y-R_B.length/2>0))   R_B.y -= 8;

        // exit option
        if (c == 'q') break;
    }
    // make the circle
    gfx_circle(cir.x,cir.y, cir.radius);
    // make the rectangles
    makeRectangle(L_B.x, L_B.y, L_B.length, L_B.width);
    makeRectangle(R_B.x, R_B.y, R_B.length, R_B.width);

    gfx_flush();

    // update circle's position
    cir.x +=cir.dx;
    cir.y +=cir.dy;

    // Left Block collision
if (
  cir.y + cir.radius >= L_B.y - L_B.length / 2 &&
  cir.y - cir.radius <= L_B.y + L_B.length / 2
) {
  if (
    cir.x - cir.radius <= L_B.x + L_B.width / 2 &&
    cir.x + cir.radius >= L_B.x - L_B.width / 2
  ) {
          // in case the ball hits the top or bottom of the box
        if ((cir.dy > 0 && cir.y <= L_B.y - L_B.length/2) || (cir.dy < 0 && cir.y >= L_B.y+L_B.length/2)) {
        cir.dy = -cir.dy; // Reverse the y-direction
        }
        else cir.dx = -cir.dx;
        colorchange();
        cir.dx = cir.dx*speed_change;
        cir.dy = cir.dy*speed_change;

  }
}

// Right Block y collision
if (
  cir.y - cir.radius <= R_B.y + R_B.length / 2 &&
  cir.y + cir.radius >= R_B.y - R_B.length / 2
) {
  if (
    cir.x - cir.radius <= R_B.x + R_B.width / 2 &&
    cir.x + cir.radius >= R_B.x - R_B.width / 2
  ) {
          // In case the ball hits the top or bottom of the box
        if ((cir.dy > 0 && cir.y <= R_B.y - R_B.length/2) || (cir.dy < 0 && cir.y >= R_B.y+R_B.length/2)) {
        cir.dy = -cir.dy; // Reverse the y-direction
        }
        else cir.dx = -cir.dx;
        colorchange();
        cir.dx = cir.dx*speed_change;
        cir.dy = cir.dy*speed_change;
  }
}
    // ceiling or floor collision
    if ( (cir.y + cir.radius >= ht)||(cir.y -  cir.radius <= 0 )){
      cir.dy =-cir.dy;
      colorchange();
        cir.dx = cir.dx*speed_change;
        cir.dy = cir.dy*speed_change;

    }

    // wall collision detection
    if( (cir.x + cir.radius >= wid)||(cir.x - cir.radius <= 0))  {
      printf("\nGame Over, thanks for playing\n");
      printf("\nFinal Speed was %.3lf\n",sqrt(pow(cir.dx,2)+pow(cir.dy,2)));
      break;
    }
        usleep(10000);
 }
}
// Function that makes the rectangle
void makeRectangle(double x, double y, int length, int width)
{
        int left_x  = x - width/2;
        int right_x = x + width/2;
        int upper_y = y + length/2;
        int lower_y = y - length/2;

        gfx_line(left_x, lower_y, left_x, upper_y);
        gfx_line(left_x, upper_y, right_x, upper_y);
        gfx_line(right_x, upper_y, right_x, lower_y);
        gfx_line(right_x, lower_y, left_x, lower_y);
}
// Function that changes the color randomly
void colorchange()
{
                int color = rand()%(5);
                switch(color){ // switch system to randomly change the color
                        case(1):
                        gfx_color(128, 0, 128);  // Purple outline
                        break;
                        case(2):
                        gfx_color(255, 0, 0);  // Red outline
                        break;
                        case(3):
                        gfx_color(0, 0,255);  // Blue outline
                        break;
                        case(4):
                        gfx_color(0, 255, 0);  // Green outline
                        break;
                        case(5):
                        gfx_color(255, 0, 255);  // Pink outline
                        break;
                }
}