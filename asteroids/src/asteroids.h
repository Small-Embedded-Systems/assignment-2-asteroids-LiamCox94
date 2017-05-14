/* Game state */

////////////////GENERAL/GAME DATA///////////////////////////
extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */
extern int 	 shields;
extern bool gameOver;
extern bool gameState;
////////////////SHIP POSITION DATA///////////////////////////
extern double 	 shipCenterHor;
extern double 	 shipCenterVert;
extern double 	 shipTipHor;
extern double 	 shipTipVert;
extern double 	 shipCnrLeftHor;
extern double 	 shipCnrLeftVert;
extern double 	 shipCnrRightHor;
extern double 	 shipCnrRightVert;
extern const float Dt; /* Time step for physics, needed for consistent motion */
////////////////LINKED LIST DATA + FRAMES///////////////////////////
extern struct aster_t *asteroids; /* array of asters / pointer to linked-list */
extern struct Missle_t *Missles;  /* array of missiles / pointer to linked-list */
extern int asterCount; extern int MissleCount;
extern int frames;


