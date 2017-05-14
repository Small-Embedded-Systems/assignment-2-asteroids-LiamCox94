/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <display.h>
#include <mbed.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"
#include "controller.h"
#include<stdio.h>
#include<stdlib.h>
////////////////VARIABLES///////////////////////////
float angle = 0.0;
double x, y;
double x1, y1;
double x2, y2;
int radius = 15;
float angle_stepsize = 0.05;
const float PI = 3.1415926f;
float vel_x;
float vel_y;
float accel_step = 1;
float accel = 100;
float collisionX;
float collisionY;
//////////////////////////////////////////////////////////
///////////ROTATES SHIP ON ALL POINTS INDEPENDENTLY///////
//////////////////////////////////////////////////////////
void shipTurn()
{
    //go through all angles from 0 to 2 * PI radians
    x = radius * cos (angle);
    y = radius * sin (angle);
    x1 = radius * cos (angle + 2.35619);
    y1 = radius * sin (angle + 2.35619);
    x2 = radius * cos (angle + 3.92699);
    y2 = radius * sin (angle + 3.92699);
    shipTipHor = x + shipCenterHor;
    shipTipVert = y + shipCenterVert;
    shipCnrLeftHor = x1 + shipCenterHor;
    shipCnrLeftVert = y1 + shipCenterVert;
    shipCnrRightHor = x2 + shipCenterHor;
    shipCnrRightVert = y2 + shipCenterVert;
    if (angle > 2 * PI){
        angle = 0.1;
    }
    if (angle < 0){
        angle = (2 * PI) - 0.1;
    }
    if (joyStickRight == true ){
        angle -= angle_stepsize;
    }
    if (joyStickLeft == true ){
        angle += angle_stepsize;
    }
    if (joyStickDown == true ){
        joyStickLeft = false;
        joyStickRight = false;
        joyStickDown = false;
    }
}
//////////////////////////////////////////////////////////
////////////SHIP IN DIRECTION IT IS FACING////////////////
//////////////////////////////////////////////////////////
void shipThrust(){
    if (accel > 7 && joyStickUp == true){
        accel -= accel_step;
    }
    if (accel < 100 && joyStickUp == false){
        accel += accel_step;
    }
    vel_x = (10 * cos (angle));
    vel_y = (10 * sin (angle));
    if (joyStickUp == true || accel != 100 ){
        shipCenterHor += (vel_x / accel);
        shipCenterVert += (vel_y / accel);
    }
    
}
//////////////////////////////////////////////////////////
///////HANDLES GAME STATE AND TRANSMITS TOO VIEW//////////
//////////////////////////////////////////////////////////
void gameInfo(){
    if(joyStickCenter == true && gameState ==  false && gameOver == false ){
        joyStickCenter = false;
        lives = 3;
        shields = 3;
        gameState = true;
    }
    if(gameOver == true){
        gameState = false;
    }
    if(joyStickCenter == true && gameOver == true){
        score = 0;
        angle = 0.0;
        shipCenterHor = 240;
        shipCenterVert = 141;
        joyStickCenter = false;
        gameOver = false;
        gameState = false;
    }
}/////////////////////////////////////////////////////////
///////HANDLES SHIP MOVING OFF SCREEN/////////////////////
//////////////////////////////////////////////////////////
void shipReposition(){
    if (shipCenterVert - 15 > 272){
        shipCenterVert =  25;
    }
    if (shipCenterVert + 10 < 19 ){
        shipCenterVert =  285;
    }
    if (shipCenterHor - 7 > 487){
        shipCenterHor =  8;
    }
    if (shipCenterHor + 7 < 0 ){
        shipCenterHor =  486;
    }
}
///////////////////////////////////////////////////////////
///////LINKED LISTS HANDLING ASTEROIDS AND MISSLES/////////
///////////////////////////////////////////////////////////
void newAster(aster_t* head) {
    if(frames % 100 == 0 && asterCount < 15 ) {
        aster_t* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = (aster_t*)malloc(sizeof(aster_t));
        current->next->p.x = randrange(16, 465);current->next->p.y =  randrange(16, 257);
        current->next->v.x = randrange(-2, 3);current->next->v.y = randrange(-2, 3);
        if(current->next->v.x == 0 && current->next->v.y == 0)
        current->next->v.x = 1;
        current->next->next = NULL;
        asterCount++;
    }
}

void updateasters(aster_t* head) {
    aster_t* current = head;
    if (current != NULL) {
        current->p.x += current->v.x;current->p.y += current->v.y;
        if(current->p.x > 496) {
            current->p.x = -16;
        }
        if(current->p.x < -16) {
            current->p.x = 496;
        }
        if(current->p.y > 290) {
            current->p.y = -20;
        }
        if(current->p.y < -20) {
            current->p.y = 290;
        }
        current = current->next;
        updateasters(current);
    }
}
void newMissle(Missle_t* head) {
    Missle_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (Missle_t*)malloc(sizeof(Missle_t));
    current->next->p.x = shipCenterHor;current->next->p.y = shipCenterVert;
    current->next->v.x = vel_x;current->next->v.y = vel_y;
    current->next->age = frames;
    current->next->next = NULL;
}

void shoot(){
    if(joyStickCenter == true ){
        newMissle(Missles);
        joyStickCenter = false;
    }
}


void updateMissles(Missle_t* head) {
    Missle_t *current = head;
    while(current != NULL) {
        current->p.x += current->v.x;current->p.y += current->v.y;
        collisionX = current->p.x;
        collisionY = current->p.y;
        current = current->next;
    }
}
//////////////////////////////////////////////////////////
///////////////SENDS DATA TO BUFFER///////////////////////
//////////////////////////////////////////////////////////
void physics(void){
    frames++;
    newAster(asteroids);
    updateasters(asteroids);
    updateMissles(Missles);
}

