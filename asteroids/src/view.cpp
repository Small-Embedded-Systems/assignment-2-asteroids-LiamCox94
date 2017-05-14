/* Asteroids view
*/

#include <mbed.h>
#include <MMA7455.h>
#include <LM75B.h>
#include <display.h>

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "objects.h"

Display *graphics = Display::theDisplay();

/////////////////////////////////////////////////////////////
////////////////BUFFFER FUNCTIONS////////////////////////////
/////////////////////////////////////////////////////////////
void init_DBuffer(void)
{
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}
void swap_DBuffer(void)
{
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}
/////////////////////////////////////////////////////////////
////////////////GAME STATE DISPLAY///////////////////////////
/////////////////////////////////////////////////////////////
void background(){
    graphics->fillScreen(BLACK);
    graphics->drawRect(0,0,480,272,WHITE);
    graphics->fillRect(0,0,480,20,WHITE);
}

void gameOverScreen(){
    graphics->fillScreen(BLACK);
    graphics->drawRect(0,0,480,272,WHITE);
    graphics->fillRect(0,0,480,20,WHITE);
    graphics->setTextColor(WHITE, WHITE);
    graphics->setTextSize(5);
    graphics->setCursor(100,120);
    graphics->printf("GAME OVER");
    graphics->setTextSize(3);
    graphics->setCursor(130,200);
    graphics->printf("Score = %d", score);
    graphics->setTextSize(2);
    graphics->setCursor(90,250);
    graphics->printf("Press center to restart");
}

void gameStartScreen(){
    graphics->fillScreen(BLACK);
    graphics->drawRect(0,0,480,272,WHITE);
    graphics->fillRect(0,0,480,20,WHITE);
    graphics->setTextColor(WHITE, WHITE);
    graphics->setTextSize(4);
    graphics->setCursor(130,70);
    graphics->printf("ASTEROIDS");
    graphics->setTextSize(2);
    graphics->setCursor(110,170);
    graphics->printf("Press center to start");
    graphics->setTextSize(1);
    graphics->setCursor(150,220);
    graphics->printf("LIAM COX - W15011743 - CM0506");
}
/////////////////////////////////////////////////////////////
////////////////DRAWS SHIP///////////////////////////////////
/////////////////////////////////////////////////////////////
void ship(){
    graphics->drawLine(shipTipHor,shipTipVert,shipCnrRightHor,shipCnrRightVert,WHITE); //R
    graphics->drawLine(shipCnrLeftHor,shipCnrLeftVert,shipTipHor,shipTipVert,WHITE); //B
    graphics->drawLine(shipCenterHor,shipCenterVert,shipCnrRightHor,shipCnrRightVert,WHITE);//Bottom
    graphics->drawLine(shipCnrLeftHor,shipCnrLeftVert,shipCenterHor,shipCenterVert,WHITE);	//Bottom
}

/////////////////////////////////////////////////////////////
////////////////DRAWS LIVES AND OTHER DATA///////////////////
/////////////////////////////////////////////////////////////
void gameData(){
    int i;
    for(i = 1; i <= lives; i++){
        graphics->drawLine(5 + (i *10),2,2 + (i *10),15,BLACK); //R
        graphics->drawLine(5 + (i *10),2,8 + (i *10),15,	BLACK); //G
        graphics->drawLine(2 + (i *10),15,5 + (i *10),10,BLACK); //B
        graphics->drawLine(5 + (i *10),10,8 + (i *10),15,BLACK); //B
    }
    if(lives == 0){
        gameOver = true;
    }
    graphics->setTextColor(BLACK, BLACK);
    graphics->setTextSize(1);
    graphics->setCursor(210,5);
    graphics->printf("Score = %d", score);
}
/////////////////////////////////////////////////////////////
////////////////LINKED LISTS DRAWINGS////////////////////////
/////////////////////////////////////////////////////////////
void drawMissles(Missle_t *head) {
    Missle_t* current = head;
    while (current != NULL) {
        graphics->drawCircle(current->p.x,current->p.y ,2, WHITE);
        current = current->next;
    }
}
void drawasters(aster_t *head) {
    aster_t* current = head;
    while (current != NULL) {
        graphics->drawBitmap(current->p.x,current->p.y,asteroid, 16,16, WHITE);
        current = current->next;
    }
}
void collisionasters(aster_t *head) {
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
        collisionasters(current);
    }
    
    int i;
    for(i = 1; i <= shields; i++){
        if(shields != 0){
            graphics->drawCircle(shipCenterHor,shipCenterVert,14 + (i*3),BLUE);
        }
    }
    if(shields!= 0 && current->p.x <= shipCenterHor + 15 + (i*3)&& current->p.x >= shipCenterHor - 15 - (i*3)
    && current->p.y >= shipCenterVert - 15 - (i*3) && current->p.y <= shipCenterVert + 15 + (i*3)){
        current->p.x = NULL;
        current->p.y = NULL;
        shields--;
    }
    if(current->p.x <= shipCenterHor + 6 && current->p.x >= shipCenterHor - 15
    && current->p.y >= shipCenterVert - 15 && current->p.y <= shipCenterVert + 15 && shields == 0){
        current->p.x = NULL;
        current->p.y = NULL;
        lives--;
    }
    if(current->p.x <= collisionX && current->p.x + 16 >= collisionX
    && current->p.y + 16 >= collisionY && current->p.y <= collisionX){
        current->p.x = 1000;
        current->p.y = 1000;
        score += 1;
    }
    if(collisionX <= 0 || collisionY <= 0 || collisionX >= 480 || collisionY >= 270){
        collisionX = 100;
        collisionY = 100;
    }
}

/////////////////////////////////////////////////////////////
/////////////FUNCTION USED BY BUFFER/////////////////////////
/////////////////////////////////////////////////////////////

void draw(void){
    if(gameState ==  false && gameOver == false){
        gameInfo();
        gameStartScreen();
        
    }
    if (gameState == true){
        gameInfo();
        background();
        ship();
        shipTurn();
        shipThrust();
        shoot();
        drawasters(asteroids);
        collisionasters(asteroids);
        drawMissles(Missles);
        shipReposition();
        gameData();
    }
    if (gameOver == true){
        gameInfo();
        gameOverScreen();
    }
    swap_DBuffer();
}
