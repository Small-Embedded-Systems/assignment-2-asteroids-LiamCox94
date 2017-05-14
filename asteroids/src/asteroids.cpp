/*
LIAM COX - W15011743 - CM0506 ASSIGNMENT
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"
#include "objects.h"

/* Game state */
float elapsed_time;
int   score;
int   lives = 3;
int shields = 3;
bool gameState = false;
bool gameOver = false;

aster_t *asteroids;
Missle_t *Missles;
int frames;
int asterCount; int MissleCount;
/////////////////////////////////////////////////////////////
////////////////SHIP POSITION DATA///////////////////////////
/////////////////////////////////////////////////////////////
double 	shipCenterHor = 240;
double  shipCenterVert = 146;
double 	shipTipHor;
double 	shipTipVert;
double	shipCnrLeftHor = shipCenterHor - 7;
double 	shipCnrLeftVert = shipCenterVert + 10;
double 	shipCnrRightHor = shipCenterHor + 7;
double 	shipCnrRightVert = shipCenterVert + 10;

float Dt = 0.01f;

Ticker model, view, controller;

bool paused = true;
/////////////////////////////////////////////////////////////
////////////////INITIALISES LINKED LISTS/////////////////////
/////////////////////////////////////////////////////////////
void initialise() {
    asteroids = static_cast<aster_t*>(malloc(sizeof(aster_t)));
    asteroids->p.x = 0;asteroids->p.y =0;
    asteroids->v.x = 2;asteroids->v.y = 2;
    asteroids->next = NULL;
    
    Missles = static_cast<Missle_t*>(malloc(sizeof(Missle_t)));
    Missles->next = NULL;
}
/////////////////////////////////////////////////////////////
/////////GENERAL/CONTROLS BUFFER TIMING//////////////////////
/////////////////////////////////////////////////////////////
int main()
{
    initialise();
    init_DBuffer();
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
    paused = false;
}
