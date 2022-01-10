#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <SDL_gfxPrimitives.h>
#include "gtksdl.h"
//#include <SDL_ttf.h>

#include "structs.h"


#define R_ROUGE 0
#define R_VERT 1
#define R_BLEU 2
#define R_NOIR 3
#define R_ROSE 4
#define R_CYAN 5
#define R_JAUNE 6
#define R_BLANC 7

#define LAYER_BELOW_RAYS 0
#define LAYER_ABOVE_RAYS 1

#define NB_LEVELS 32

SDL_Surface *leveltiles;
SDL_Surface *sprites;
SDL_Surface *light;
SDL_Surface *player;
SDL_Surface *treasures;
SDL_Surface *chests;
SDL_Surface *monstres;
SDL_Surface *bossmages;
SDL_Surface *bigboss;
SDL_Surface *parchemin;
SDL_Surface *fragments;

SDL_Surface *screenCalcul;


int currentLevel = 0;

SDL_Event event;


GtkWidget *pSdlMain;
//GtkWidget *pSdlMiniMap;

SDL_Surface *screenMain;
//SDL_Surface *screenMiniMap;

GtkWidget *pButton, *pVbox, *pWindow, *pHbox, *butgrid, *pMenuBar, *pMenu, *pMenuItem, *pHGrid;

GtkWidget *pButtonP, *imageP, *pButtonL, *imageL, *pButtonR, *imageR, *pButtonG, *imageG, *pButtonB, *imageB, *pButtonN, *imageN;
GtkWidget *pButtonColor0, *imageColor0, *pButtonColor1, *imageColor1, *pButtonColor2, *imageColor2, *pButtonColor3, *imageColor3;
GtkWidget *pButtonDoorW, *imageDoorW, *pButtonDoorN, *imageDoorN, *pButtonDoorE, *imageDoorE, *pButtonDoorS, *imageDoorS;

GtkWidget *LabelXReceptors, *LabelYReceptors, *LabelDirReceptors;

GtkWidget *EditXRecRed, *EditYRecRed, *ComboDirRecRed;
GtkWidget *EditXRecGreen, *EditYRecGreen, *ComboDirRecGreen;
GtkWidget *EditXRecBlue, *EditYRecBlue, *ComboDirRecBlue;
GtkWidget *EditXRecBlack, *EditYRecBlack, *ComboDirRecBlack;
GtkWidget *EditXRecYellow, *EditYRecYellow, *ComboDirRecYellow;
GtkWidget *EditXRecCyan, *EditYRecCyan, *ComboDirRecCyan;
GtkWidget *EditXRecPink, *EditYRecPink, *ComboDirRecPink;
GtkWidget *EditXRecWhite, *EditYRecWhite, *ComboDirRecWhite;

GtkWidget *pButtonReceptRed, *imageReceptRed;
GtkWidget *pButtonReceptGreen, *imageReceptGreen;
GtkWidget *pButtonReceptBlue, *imageReceptBlue;
GtkWidget *pButtonReceptBlack, *imageReceptBlack;
GtkWidget *pButtonReceptYellow, *imageReceptYellow;
GtkWidget *pButtonReceptCyan, *imageReceptCyan;
GtkWidget *pButtonReceptPink, *imageReceptPink;
GtkWidget *pButtonReceptWhite, *imageReceptWhite;

GtkWidget *LabelLevelConnect;
GtkWidget *EditConnectWest, *EditConnectNorth, *EditConnectEast, *EditConnectSouth;

GtkWidget *pCheckWestRed, *pCheckWestGreen, *pCheckWestBlue, *pCheckWestBlack;
GtkWidget *pCheckWestYellow, *pCheckWestPink, *pCheckWestCyan, *pCheckWestWhite;

GtkWidget *pCheckNorthRed, *pCheckNorthGreen, *pCheckNorthBlue, *pCheckNorthBlack;
GtkWidget *pCheckNorthYellow,*pCheckNorthPink, *pCheckNorthCyan, *pCheckNorthWhite;

GtkWidget *pCheckEastRed, *pCheckEastGreen, *pCheckEastBlue, *pCheckEastBlack;
GtkWidget *pCheckEastYellow, *pCheckEastPink, *pCheckEastCyan, *pCheckEastWhite;

GtkWidget *pCheckSouthRed, *pCheckSouthGreen, *pCheckSouthBlue, *pCheckSouthBlack;
GtkWidget *pCheckSouthYellow, *pCheckSouthPink, *pCheckSouthCyan, *pCheckSouthWhite;

GtkWidget *pVboxRight, *pCodeView;

unsigned char LEVELCOLOR = 0;
unsigned int SIZE_MAP_X = 13;
unsigned int SIZE_MAP_Y = 11;
unsigned int OFFSET_X = 12;
unsigned int OFFSET_Y = 12;
unsigned int SIZE = 16;
unsigned int ZOOMFACTOR = 1;

unsigned int OFFSET_X_MINIMAP = 312;
unsigned int OFFSET_Y_MINIMAP = 160;
unsigned char SCALE_X_MINIMAP = 8;
unsigned char SCALE_Y_MINIMAP = 6;

unsigned char ALPHA_OPAQUE_DEFAULT = 150; //191
unsigned char ALPHA_TRANSPARENT_DEFAULT = 80; //62

unsigned char ALPHA_OPAQUE = ALPHA_OPAQUE_DEFAULT;
unsigned char ALPHA_TRANSPARENT = ALPHA_TRANSPARENT_DEFAULT;
unsigned char ALPHA_BLOQUE = 1;

unsigned char SPEEDUP_RAY = 1;
unsigned char SPEEDUP_WALL = 1;

unsigned char NbReceptorsLevel;
unsigned char NbDoorsLevel;
unsigned char NbBlocksLevel;
unsigned char NbTreasureLevel;
unsigned char NbObstacleLevel;
unsigned char NbMonsterLevel;
unsigned char NbBossLevel;


selection selected = JOUEUR;

Player joueur;
Light lumiere;
Wall murs[4];

Blocks* BlocRouge;
Blocks* BlocVert ;
Blocks* BlocBleu;
Blocks* BlocNoir;

Blocks* BlocksCollection=NULL;
Receptor* ReceptorCollection=NULL;
Door* DoorCollection=NULL;
Treasure* TreasureCollection=NULL;
Obstacle* ObstaclesCollection=NULL;
Monster* MonstersCollection=NULL;
Boss* BossCollection=NULL;

Minimap Map[ 33 ];


unsigned char ALPHA_VALIDE = 255;
unsigned char ALPHA_NON_VALIDE = 100;

unsigned char EPSILON = 16;

bool BlackFrag=false;
bool WhiteFrag=false;
bool RedFrag=false;
bool GreenFrag=false;
bool BlueFrag=false;


bool done = false;
bool main_GTK_Quit = false;

unsigned int x,y;

void initSDL(void);
void buildInterfaceEditor( void );
void initializeinterface( void );
void updateMainScreen( void );
void loadLevel( unsigned char numLevel );
void exitAndFree( void );
void initWalls( void );
int _sgn( int x );
int _abs( int x );
int computeLightModification( unsigned int currentCellX, unsigned int currentCellY, unsigned char* R, unsigned char* G, unsigned char* B, unsigned char* A );
void rayCast( SDL_Surface *surf, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char* Rs, unsigned char* Gs, unsigned char* Bs, unsigned char* As );
void renderWalls( SDL_Surface *surf );
void renderObstacles( SDL_Surface *surf, int passnumber );
void renderTreasures( SDL_Surface* surf );
void renderMovable( SDL_Surface *surf );
void renderPlayer( SDL_Surface *surf );
void renderLight( SDL_Surface *surf );
bool checkNextPositionMonster( unsigned int Xtarget, unsigned int Ytarget, unsigned int direction );
void updateMonsters( void );
void renderMonsters( SDL_Surface *surf );
bool checkNextPositionBoss( unsigned int Xtarget, unsigned int Ytarget, unsigned int direction );
void updateBoss( void );
void renderBoss( SDL_Surface *surf );
void drawMinimap( SDL_Surface *surf );
void drawInterface( SDL_Surface *surf );
void updateTreasures( selection what );
bool isValidMove( selection selected, unsigned char x, unsigned char y, orientations sens );
void gameMechanics(  selection what, orientations touche );
void renderReceptors( SDL_Surface* surf );
void checkReceptors( SDL_Surface* surf  );
void renderDoors( SDL_Surface* surf );
void checkDoors( SDL_Surface* surf );

uint32_t _getpixel(SDL_Surface *surface, int x, int y);
void getPixelRGB( SDL_Surface* surf, unsigned int x, unsigned int y, unsigned char *R1, unsigned char* G1, unsigned char *B1 );

gboolean time_handler(GtkWidget *widget);
void Clicked(GtkWidget*,gpointer);

void DoorWClicked(GtkWidget * pWindow,gpointer data);
void DoorNClicked(GtkWidget * pWindow,gpointer data);
void DoorEClicked(GtkWidget * pWindow,gpointer data);
void DoorSClicked(GtkWidget * pWindow,gpointer data);

void Color0Clicked(GtkWidget * pWindow,gpointer data);
void Color1Clicked(GtkWidget * pWindow,gpointer data);
void Color2Clicked(GtkWidget * pWindow,gpointer data);
void Color3Clicked(GtkWidget * pWindow,gpointer data);


void RecepRedClicked(GtkWidget * pWindow,gpointer data);
void RecepGreenClicked(GtkWidget * pWindow,gpointer data);
void RecepBlueClicked(GtkWidget * pWindow,gpointer data);
void RecepBlackClicked(GtkWidget * pWindow,gpointer data);
void RecepYellowClicked(GtkWidget * pWindow,gpointer data);
void RecepCyanClicked(GtkWidget * pWindow,gpointer data);
void RecepPinkClicked(GtkWidget * pWindow,gpointer data);
void RecepWhiteClicked(GtkWidget * pWindow,gpointer data);


void OnDestroy(GtkWidget *pWidget, gpointer pData);
void OnQuitter(GtkWidget* widget, gpointer data);
void OnAbout(GtkWidget* widget, gpointer data);

static gboolean mouse_moved(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static gboolean mouse_clicked(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static gboolean key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static gboolean key_released(GtkWidget *widget, GdkEventKey *event);
//gboolean update (GtkWidget *widget, GdkFrameClock *clock, gpointer data);



int main(int argc, char **argv)
{
    initSDL();
    gtk_init(&argc, &argv);


    leveltiles = IMG_Load( "./Resources/levelsimple.png" );
    sprites = IMG_Load( "./Resources/sprites.png" );
    light = IMG_Load( "./Resources/light.png" );
    player = IMG_Load( "./Resources/player.png" );
    chests = IMG_Load( "./Resources/chests.png" );
    monstres  = IMG_Load( "./Resources/monstres.png" );
    bigboss  = IMG_Load( "./Resources/bigboss.png" );
    bossmages  = IMG_Load( "./Resources/sorcerer.png" );
    parchemin = IMG_Load( "./Resources/parchemin.png" );
    fragments = IMG_Load( "./Resources/fragments.png" );

    //leveltiles = rotozoomSurface( IMG_Load( "./Resources/levelsimple.png" ), 0, ZOOMFACTOR, SMOOTHING_OFF );
    //sprites = rotozoomSurface( IMG_Load( "./Resources/sprites.png" ), 0, ZOOMFACTOR, SMOOTHING_OFF );

    /*
        leveltiles = IMG_Load( "./Resources/levelsimple.png" );
        sprites = IMG_Load( "./Resources/sprites.png" );
        light = IMG_Load( "./Resources/light.png" );
        player = IMG_Load( "./Resources/player.png" );
        chests = IMG_Load( "./Resources/chests.png" );

        screenCalcul = SDL_CreateRGBSurface(0, 800, 600, 32, 0, 0, 0, 0);
    */

    buildInterfaceEditor();
    initializeinterface();
    //updateMainScreen();


    gtk_main();
    /*
    	done=false;
    	main_GTK_Quit = false;

    	while (!done)
        {

            gtk_main_iteration_do (FALSE);

            while (SDL_PollEvent(&event))
            {
                printf( "SDL loop" );
                switch (event.type)
                {
                case SDL_QUIT:
                        done = true;
                    break;
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;

                    if (event.key.keysym.sym == SDLK_LEFT)
                    {

                    }

                    if (event.key.keysym.sym == SDLK_RIGHT)
                    {

                    }

                    if (event.key.keysym.sym == SDLK_UP)
                    {

                    }

                    if (event.key.keysym.sym == SDLK_DOWN)
                    {

                    }

                }
                }

            }
        }
    */

    exitAndFree();

    return EXIT_SUCCESS;
}

void initializeinterface( void )
{
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(pButtonColor0), TRUE );

    initWalls();
    loadLevel( currentLevel );
}


void drawMinimap( SDL_Surface *surf )
{
       for( unsigned char k=0; k<=NB_LEVELS; k++ )
       {
              if (Map[k].visited==true)
              {
                     unsigned char R= Map[k].R;
                     unsigned char G= Map[k].G;
                     unsigned char B= Map[k].B;
                     unsigned char A= Map[k].A;

                     unsigned int lX = OFFSET_X_MINIMAP + Map[k].x*(SCALE_X_MINIMAP+2);
                     unsigned int lY = OFFSET_Y_MINIMAP + Map[k].y*(SCALE_Y_MINIMAP+2);

                     boxRGBA( surf, lX-4, lY-3, lX+4, lY+3, R, G, B, A>>1 );
                     rectangleRGBA( surf, lX-4, lY-3, lX+4, lY+3, R, G, B, 255);

                     if (Map[k].doorWest==true) rectangleRGBA( surf, lX-5, lY-1, lX-5, lY+1, 255, 255, 255, 255);
                     if (Map[k].doorEast==true) rectangleRGBA( surf, lX+5, lY-1, lX+5, lY+1, 255, 255, 255, 255);
                     if (Map[k].doorNorth==true) rectangleRGBA( surf, lX-1, lY-4, lX+1, lY-4, 255, 255, 255, 255);
                     if (Map[k].doorSouth==true) rectangleRGBA( surf, lX-1, lY+4, lX+1, lY+4, 255, 255, 255, 255);

                     if (k==currentLevel)
                     {
                            rectangleRGBA( surf, lX-4, lY-3, lX+4, lY+3, 255, 0, 255, 255);
                            rectangleRGBA( surf, lX-3, lY-2, lX+3, lY+2, 255, 0, 255, 255);
                     }

              }
       }
}

void drawInterface( SDL_Surface *surf )
{

       unsigned int midX, midY;
       SDL_Rect source, dest;

       //dsubimage( 232, 0, &parchemin, 0,0, 164, 210,  DIMAGE_NONE);

       dest.w = parchemin->w;
       dest.h = parchemin->h;
       source.w = parchemin->w;
       source.h = parchemin->h;
       dest.x = 232;
       dest.y = 0;
       source.x = 0;
       source.y = 0;
       SDL_BlitSurface( parchemin, &source, surf, &dest );

       if (WhiteFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 0, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (BlackFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 32, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 32;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (RedFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 64, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 64;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (GreenFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 96, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 96;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (BlueFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 128, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 128;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }

}

/*
void loadLevel( void )
{
    free(BlocksCollection);
    free(ReceptorCollection);
    free(DoorCollection);
    free(TreasureCollection);
    free(ObstaclesCollection);

    BlocksCollection=NULL;
    ReceptorCollection=NULL;
    DoorCollection=NULL;
    TreasureCollection=NULL;
    ObstaclesCollection=NULL;


    LEVELCOLOR = 0;
    SIZE_MAP_X=13;
    SIZE_MAP_Y=11;

    NbReceptorsLevel = 3;
    NbDoorsLevel = 1;
    NbBlocksLevel = 4;
    NbTreasureLevel = 0;
    NbObstacleLevel = 8;

    BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
    ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
    DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
    TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
    ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );

    joueur = { 7, 9, HAUT };
    lumiere = { 6, 5, 1,1,1,0, false };

    ObstaclesCollection[0] = {2,2,BLOCK_SUN};
    ObstaclesCollection[1] = {10,2,BLOCK_SUN};
    ObstaclesCollection[2] = {2,8,BLOCK_SUN};
    ObstaclesCollection[3] = {10,8,BLOCK_SUN};
    ObstaclesCollection[4] = {4,5,BLOCK_WATER};
    ObstaclesCollection[5] = {8,5,BLOCK_WATER};
    ObstaclesCollection[6] = {6,3,BLOCK_WATER};
    ObstaclesCollection[7] = {6,7,BLOCK_WATER};

    BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
    BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
    BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
    BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

    BlocRouge = &BlocksCollection[0];
    BlocVert = &BlocksCollection[1];
    BlocBleu = &BlocksCollection[2];
    BlocNoir = &BlocksCollection[3];

    ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
    ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
    ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
    // ReceptorCollection[3] = { 10, 8, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
    // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
    // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
    // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
    // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

    DoorCollection[0] = { 6,0, HAUT, false, true, 3, { R_ROUGE,R_BLEU, R_VERT }, 2}; // Door NORTH
    //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
    //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
    //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

    //TreasureCollection[0] = {2,2, T_RED, false, PIERRE_BLANCHE, 100 };
    //TreasureCollection[1] = {11,9, T_YELLOW, false, PIERRE_BLANCHE, 100 };
    //TreasureCollection[2] = {10,4, T_GREEN, false, PIERRE_BLANCHE, 100 };
    //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100 };

}
*/


void loadLevel( unsigned char numLevel )
{
    free(BlocksCollection);
    free(ReceptorCollection);
    free(DoorCollection);
    free(TreasureCollection);
    free(ObstaclesCollection);
    free(MonstersCollection);

    BlocksCollection=NULL;
    ReceptorCollection=NULL;
    DoorCollection=NULL;
    TreasureCollection=NULL;
    ObstaclesCollection=NULL;
    MonstersCollection=NULL;

    if (numLevel==0) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 16;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=0;
        Map[numLevel].y=0;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=false;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        ObstaclesCollection[0] = {4,1,BLOCK_STONE};
        ObstaclesCollection[1] = {8,1,BLOCK_STONE};
        ObstaclesCollection[2] = {1,3,BLOCK_STONE};
        ObstaclesCollection[3] = {11,3,BLOCK_STONE};
        ObstaclesCollection[4] = {1,7,BLOCK_STONE};
        ObstaclesCollection[5] = {11,7,BLOCK_STONE};
        ObstaclesCollection[6] = {4,9,BLOCK_STONE};
        ObstaclesCollection[7] = {8,9,BLOCK_STONE};

        ObstaclesCollection[8] = {5,4,BLOCK_WATER};
        ObstaclesCollection[9] = {6,4,BLOCK_WATER};
        ObstaclesCollection[10] = {7,4,BLOCK_WATER};
        ObstaclesCollection[11] = {5,5,BLOCK_WATER};
        ObstaclesCollection[12] = {7,5,BLOCK_WATER};
        ObstaclesCollection[13] = {5,6,BLOCK_WATER};
        ObstaclesCollection[14] = {6,6,BLOCK_WATER};
        ObstaclesCollection[15] = {7,6,BLOCK_WATER};


        BlocksCollection[0] = { 2, 4, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 10, 4, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 10, 6, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 2, 6, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 2, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 10, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 10, 10, HAUT, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 2, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        DoorCollection[0] = { 6,0, HAUT, false, true, 1, { R_ROUGE, -1, -1 }, 2}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
        //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
        //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

        //TreasureCollection[0] = {1,1, T_RED, false, WHITE_FRAGMENT, 100, true };
        //TreasureCollection[1] = {2,1, T_YELLOW, false, BLACK_FRAGMENT, 100, true };
        //TreasureCollection[2] = {3,1, T_GREEN, false, RED_FRAGMENT, 100, true };
        //TreasureCollection[3] = {4,1, T_BLUE, false, GREEN_FRAGMENT, 100, true };
        //TreasureCollection[4] = {5,1, T_BLUE, false, BLUE_FRAGMENT, 100, true };

        //MonstersCollection[0] = { 6, 4, 2, 10, HORIZONTAL, GAUCHE, BLOB };
        //MonstersCollection[1] = { 7, 5, 2, 8, VERTICAL, HAUT, SKELETON };
    }
    else if (numLevel==1) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 16;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-1;
        Map[numLevel].y=-1;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        ObstaclesCollection[0] = {1,1,BLOCK_STONE};
        ObstaclesCollection[1] = {11,1,BLOCK_STONE};
        ObstaclesCollection[2] = {1,9,BLOCK_STONE};
        ObstaclesCollection[3] = {11,9,BLOCK_STONE};

        ObstaclesCollection[4] = {1,2,BLOCK_WATER};
        ObstaclesCollection[5] = {2,1,BLOCK_WATER};
        ObstaclesCollection[6] = {2,2,BLOCK_WATER};

        ObstaclesCollection[7] = {10,1,BLOCK_WATER};
        ObstaclesCollection[8] = {10,2,BLOCK_WATER};
        ObstaclesCollection[9] = {11,2,BLOCK_WATER};

        ObstaclesCollection[10] = {10,9,BLOCK_WATER};
        ObstaclesCollection[11] = {10,8,BLOCK_WATER};
        ObstaclesCollection[12] = {11,8,BLOCK_WATER};

        ObstaclesCollection[13] = {1,8,BLOCK_WATER};
        ObstaclesCollection[14] = {2,8,BLOCK_WATER};
        ObstaclesCollection[15] = {2,9,BLOCK_WATER};

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 6, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        DoorCollection[0] = { 6,0, HAUT, false, true, 2, { R_ROUGE,R_BLEU, -1 }, 4}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 2, { R_VERT, R_NOIR, -1 }, 2}; // Door EAST
        //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

        TreasureCollection[0] = {4,4, T_RED, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[1] = {8,4, T_YELLOW, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[2] = {4,6, T_GREEN, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[3] = {8,6, T_BLUE, false, PIERRE_BLANCHE, 100,  true };

        MonstersCollection[0] = { 6, 3, 1, 11, HORIZONTAL, DROITE, SPIDER };
        MonstersCollection[1] = { 6, 7, 1, 11, HORIZONTAL, GAUCHE, SPIDER };
        MonstersCollection[2] = { 3, 5, 1, 9, VERTICAL, HAUT, SPIDER };
        MonstersCollection[3] = { 9, 5, 1, 9, VERTICAL, BAS, SPIDER };
        //MonstersCollection[1] = { 1, 5, 1, 9, VERTICAL, BAS, SPIDER };
        //MonstersCollection[2] = { 10, 5, 1, 9, VERTICAL, HAUT, GHOST };

    }
    else if (numLevel==2) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 2;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 24;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=0;
        Map[numLevel].y=-1;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        ObstaclesCollection[0] = {1,4,BLOCK_STONE};
        ObstaclesCollection[1] = {2,4,BLOCK_STONE};
        ObstaclesCollection[2] = {1,6,BLOCK_STONE};
        ObstaclesCollection[3] = {2,6,BLOCK_STONE};
        ObstaclesCollection[4] = {10,4,BLOCK_STONE};
        ObstaclesCollection[5] = {11,4,BLOCK_STONE};
        ObstaclesCollection[6] = {10,6,BLOCK_STONE};
        ObstaclesCollection[7] = {11,6,BLOCK_STONE};
        ObstaclesCollection[8] = {5,8,BLOCK_STONE};
        ObstaclesCollection[9] = {5,9,BLOCK_STONE};
        ObstaclesCollection[10] = {7,8,BLOCK_STONE};
        ObstaclesCollection[11] = {7,9,BLOCK_STONE};
        ObstaclesCollection[12] = {5,2,BLOCK_WATER};
        ObstaclesCollection[13] = {6,2,BLOCK_WATER};
        ObstaclesCollection[14] = {7,2,BLOCK_WATER};
        ObstaclesCollection[15] = {5,3,BLOCK_WATER};
        ObstaclesCollection[16] = {7,3,BLOCK_WATER};
        ObstaclesCollection[17] = {5,4,BLOCK_WATER};
        ObstaclesCollection[18] = {7,4,BLOCK_WATER};
        ObstaclesCollection[19] = {5,5,BLOCK_WATER};
        ObstaclesCollection[20] = {7,5,BLOCK_WATER};
        ObstaclesCollection[21] = {5,6,BLOCK_WATER};
        ObstaclesCollection[22] = {6,6,BLOCK_WATER};
        ObstaclesCollection[23] = {7,6,BLOCK_WATER};

        BlocksCollection[0] = { 2, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 3, 1, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 10, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 9, 1, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 0, 3, DROITE, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 2, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 12, 3, GAUCHE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 10, 0, BAS, 0, 0, 0, false }; // Receptor BLACK
        //ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
        //ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        //ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        //ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        //DoorCollection[0] = { 6,0, HAUT, false, true, 1, { R_ROUGE,R_BLEU, R_VERT }, 1}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 2, { R_ROUGE, R_VERT, -1 }, 1}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 2, { R_BLEU, R_NOIR, -1 }, 3}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 2, { R_ROUGE, R_BLEU, -1 }, 0}; // Door SOUTH

        TreasureCollection[0] = {1,1, T_RED, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[1] = {11,1, T_YELLOW, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[2] = {1,9, T_GREEN, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[3] = {11,9, T_BLUE, false, PIERRE_BLANCHE, 100,  true };

    }
    else if (numLevel==3) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 8;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=1;
        Map[numLevel].y=-1;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=false;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        ObstaclesCollection[0] = {2,2,BLOCK_SUN};
        ObstaclesCollection[1] = {10,2,BLOCK_SUN};
        ObstaclesCollection[2] = {2,8,BLOCK_SUN};
        ObstaclesCollection[3] = {10,8,BLOCK_SUN};
        ObstaclesCollection[4] = {4,5,BLOCK_WATER};
        ObstaclesCollection[5] = {8,5,BLOCK_WATER};
        ObstaclesCollection[6] = {6,3,BLOCK_WATER};
        ObstaclesCollection[7] = {6,7,BLOCK_WATER};

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 12, 7, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
        //ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
        //ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        //ReceptorCollection[3] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        //ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE


        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { R_ROUGE,R_BLEU, R_VERT }, 5}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, 3, -1 }, 2}; // Door WEST
        //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
        //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

        TreasureCollection[0] = {4,4, T_RED, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[1] = {8,4, T_YELLOW, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[2] = {4,6, T_GREEN, false, PIERRE_BLANCHE, 100,  true };
        TreasureCollection[3] = {8,6, T_BLUE, false, PIERRE_BLANCHE, 100,  true };

        MonstersCollection[0] = { 6, 2, 1, 11, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[1] = { 6, 8, 1, 11, HORIZONTAL, GAUCHE, SKELETON };
        MonstersCollection[2] = { 2, 5, 1, 9, VERTICAL, HAUT, SKELETON };
        MonstersCollection[3] = { 10, 5, 1, 9, VERTICAL, BAS, SKELETON };


    }
    else if (numLevel==4) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 5;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 4;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-1;
        Map[numLevel].y=-2;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 6, 1,1,1,0, false };

        //ObstaclesCollection[0] = {2,2,BLOCK_SUN};
        //ObstaclesCollection[1] = {10,2,BLOCK_SUN};
        //ObstaclesCollection[2] = {2,8,BLOCK_SUN};
        //ObstaclesCollection[3] = {10,8,BLOCK_SUN};
        ObstaclesCollection[0] = {5,6,BLOCK_WATER};
        ObstaclesCollection[1] = {7,6,BLOCK_WATER};
        ObstaclesCollection[2] = {6,5,BLOCK_WATER};
        ObstaclesCollection[3] = {6,7,BLOCK_WATER};

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 12, 8, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
        ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 255, 0, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        DoorCollection[0] = { 6,0, HAUT, false, true, 2, { 2,4, -1 }, 7}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
        //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 1, { R_NOIR, -1, -1 }, 1}; // Door SOUTH

        TreasureCollection[0] = {1,1, T_RED, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[1] = {11,1, T_YELLOW, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[2] = {11,9, T_GREEN, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[3] = {1,9, T_BLUE, false, PIERRE_BLANCHE, 100, true };

        //BossCollection[0] = { 6, 2, 2, 10, HORIZONTAL, GAUCHE, B_RED };
        //BossCollection[1] = { 2, 5, 2, 8, VERTICAL, HAUT, B_GREEN };
        //BossCollection[2] = { 6, 9, 2, 10, HORIZONTAL, GAUCHE, B_BLUE };
        //BossCollection[3] = { 10, 5, 2, 8, VERTICAL, HAUT, B_BLACK };

    }
    else if (numLevel==5) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 8;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=1;
        Map[numLevel].y=-2;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 6, 1,1,1,0, false };

        ObstaclesCollection[0] = {6,3,BLOCK_SUN};
        ObstaclesCollection[1] = {5,4,BLOCK_WATER};
        ObstaclesCollection[2] = {7,4,BLOCK_WATER};
        ObstaclesCollection[3] = {5,5,BLOCK_SUN};
        ObstaclesCollection[4] = {7,5,BLOCK_SUN};
        ObstaclesCollection[5] = {5,6,BLOCK_WATER};
        ObstaclesCollection[6] = {7,6,BLOCK_WATER};
        ObstaclesCollection[7] = {6,7,BLOCK_SUN};


        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 8, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 4, 10, HAUT, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 4, 0, BAS, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 8, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        DoorCollection[0] = { 6,0, HAUT, false, true, 2, { 0, 2, -1 }, 9}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
        //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 2, { 1, 3, -1 }, 3}; // Door SOUTH

        TreasureCollection[0] = {1,1, T_RED, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[1] = {11,1, T_YELLOW, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[2] = {11,9, T_GREEN, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[3] = {1,9, T_BLUE, false, PIERRE_BLANCHE, 100, true };

        //BossCollection[0] = { 6, 2, 2, 10, HORIZONTAL, GAUCHE, B_RED };
        //BossCollection[1] = { 2, 5, 2, 8, VERTICAL, HAUT, B_GREEN };
        //BossCollection[2] = { 6, 9, 2, 10, HORIZONTAL, GAUCHE, B_BLUE };
        //BossCollection[3] = { 10, 5, 2, 8, VERTICAL, HAUT, B_BLACK };
    }
    else if (numLevel==6) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 2;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 8;
        NbMonsterLevel = 8;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-2;
        Map[numLevel].y=-3;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 }, 11}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 0}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 1, { 3, -1, -1 }, 7}; // Door EAST
        //DoorCollection[1] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 0}; // Door SOUTH

        ObstaclesCollection[0] = {5,4,BLOCK_WATER};
        ObstaclesCollection[1] = {5,5,BLOCK_WATER};
        ObstaclesCollection[2] = {5,6,BLOCK_WATER};
        ObstaclesCollection[3] = {6,6,BLOCK_WATER};
        ObstaclesCollection[4] = {7,6,BLOCK_WATER};
        ObstaclesCollection[5] = {7,5,BLOCK_WATER};
        ObstaclesCollection[6] = {7,4,BLOCK_WATER};
        ObstaclesCollection[7] = {6,4,BLOCK_WATER};

        ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 0, 5, DROITE, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 12, 4, GAUCHE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 6, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        MonstersCollection[0] = { 5, 3, 1, 5, HORIZONTAL, GAUCHE, BLOB };
        MonstersCollection[1] = { 5, 2, 1, 5, HORIZONTAL, GAUCHE, BLOB };
        MonstersCollection[2] = { 7, 3, 7, 11, HORIZONTAL, DROITE, BLOB };
        MonstersCollection[3] = { 7, 2, 7, 11, HORIZONTAL, DROITE, BLOB };
        MonstersCollection[4] = { 5, 7, 1, 5, HORIZONTAL, GAUCHE, BLOB };
        MonstersCollection[5] = { 5, 8, 1, 5, HORIZONTAL, GAUCHE, BLOB };
        MonstersCollection[6] = { 7, 7, 7, 11, HORIZONTAL, DROITE, BLOB };
        MonstersCollection[7] = { 7, 8, 7, 11, HORIZONTAL, DROITE, BLOB };


    }
    else if (numLevel==7) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 6;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 7;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-1;
        Map[numLevel].y=-3;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 7, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        ReceptorCollection[0] = { 12, 3, GAUCHE, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 6, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 0, 3, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 11, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        //ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        ReceptorCollection[4] = { 1, 0, BAS, 0, 255, 255, false }; // Receptor CYAN
        ReceptorCollection[5] = { 11, 0, BAS, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 }, 11}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 2, { 2, 0, -1 }, 6}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 3, { 1, 5, 4 }, 8}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 4}; // Door SOUTH

        ObstaclesCollection[0] = {5,6,BLOCK_WATER};
        //ObstaclesCollection[1] = {6,6,BLOCK_WATER};
        ObstaclesCollection[1] = {7,6,BLOCK_WATER};
        ObstaclesCollection[2] = {5,7,BLOCK_WATER};
        ObstaclesCollection[3] = {7,7,BLOCK_WATER};
        ObstaclesCollection[4] = {5,8,BLOCK_WATER};
        ObstaclesCollection[5] = {6,8,BLOCK_WATER};
        ObstaclesCollection[6] = {7,8,BLOCK_WATER};

    }
    else if (numLevel==8) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 2;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 5;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 0;
        NbMonsterLevel = 0;
        NbBossLevel = 1;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=0;
        Map[numLevel].y=-3;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 2,3,4 },12}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 3, { 1, 4, 2 }, 7}; // Door WEST
        DoorCollection[2] = { 12, 5, DROITE, false, true, 3, { 0, 3, 2 }, 9}; // Door EAST
        //DoorCollection[2] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 4}; // Door SOUTH


        ReceptorCollection[0] = { 12, 8, GAUCHE, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 0, 8, DROITE, 0, 255, 0, false }; // Receptor GREEN
        //ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[2] = { 6, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        ReceptorCollection[3] = { 9, 10, HAUT, 255, 0, 255, false }; // Receptor PINK
        ReceptorCollection[4] = { 3, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE



        BossCollection[0] = { 6, 3, 2, 10, HORIZONTAL, GAUCHE, B_RED };

    }
    else if (numLevel==9) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 6;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 7;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=1;
        Map[numLevel].y=-3;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 7, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        ReceptorCollection[0] = { 3, 0, BAS, 255, 0, 255, false };  // Receptor RED
        ReceptorCollection[1] = { 6, 0, BAS, 255, 0, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 9, 0, BAS, 255, 255, 0, false }; // Receptor BLUE
        ReceptorCollection[3] = { 11, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        //ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        ReceptorCollection[4] = { 1, 0, BAS, 255, 0, 0, false }; // Receptor CYAN
        ReceptorCollection[5] = { 11, 0, BAS, 255, 0, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 }, 11}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 3, { 0, 2, 1 }, 8}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 3, { 1, 4, 5 }, 10}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 5}; // Door SOUTH

        ObstaclesCollection[0] = {5,6,BLOCK_WATER};
        ObstaclesCollection[1] = {7,6,BLOCK_WATER};
        ObstaclesCollection[2] = {5,7,BLOCK_WATER};
        ObstaclesCollection[3] = {7,7,BLOCK_WATER};
        ObstaclesCollection[4] = {5,8,BLOCK_WATER};
        ObstaclesCollection[5] = {6,8,BLOCK_WATER};
        ObstaclesCollection[6] = {7,8,BLOCK_WATER};

    }
    else if (numLevel==10) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 8;
        NbMonsterLevel = 8;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=2;
        Map[numLevel].y=-3;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 },13}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { 3, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        //DoorCollection[2] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 5}; // Door SOUTH

        ObstaclesCollection[0] = {5,4,BLOCK_WATER};
        ObstaclesCollection[1] = {5,5,BLOCK_WATER};
        ObstaclesCollection[2] = {5,6,BLOCK_WATER};
        ObstaclesCollection[3] = {6,6,BLOCK_WATER};
        ObstaclesCollection[4] = {7,6,BLOCK_WATER};
        ObstaclesCollection[5] = {7,5,BLOCK_WATER};
        ObstaclesCollection[6] = {7,4,BLOCK_WATER};
        ObstaclesCollection[7] = {6,4,BLOCK_WATER};

        ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 12, 5, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 0, 4, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 6, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        MonstersCollection[0] = { 5, 3, 1, 5, HORIZONTAL, GAUCHE, GHOST };
        MonstersCollection[1] = { 5, 2, 1, 5, HORIZONTAL, GAUCHE, GHOST };
        MonstersCollection[2] = { 7, 3, 7, 11, HORIZONTAL, DROITE, GHOST };
        MonstersCollection[3] = { 7, 2, 7, 11, HORIZONTAL, DROITE, GHOST };
        MonstersCollection[4] = { 5, 7, 1, 5, HORIZONTAL, GAUCHE, GHOST };
        MonstersCollection[5] = { 5, 8, 1, 5, HORIZONTAL, GAUCHE, GHOST };
        MonstersCollection[6] = { 7, 7, 7, 11, HORIZONTAL, DROITE, GHOST };
        MonstersCollection[7] = { 7, 8, 7, 11, HORIZONTAL, DROITE, GHOST };
    }
    else if (numLevel==11) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 24;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-2;
        Map[numLevel].y=-4;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 9, 1, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 4, 10, HAUT, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 3, 3, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 5, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE


        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 },15}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 6}; // Door SOUTH

        ObstaclesCollection[0] = {5,3,BLOCK_WATER};
        ObstaclesCollection[1] = {5,5,BLOCK_WATER};
        ObstaclesCollection[2] = {5,7,BLOCK_WATER};
        ObstaclesCollection[3] = {6,6,BLOCK_WATER};
        ObstaclesCollection[4] = {7,3,BLOCK_WATER};
        ObstaclesCollection[5] = {7,5,BLOCK_WATER};
        ObstaclesCollection[6] = {7,7,BLOCK_WATER};
        ObstaclesCollection[7] = {6,4,BLOCK_WATER};

        ObstaclesCollection[8] = {1,3,BLOCK_MOON};
        ObstaclesCollection[9] = {3,3,BLOCK_MOON};
        ObstaclesCollection[10] = {1,5,BLOCK_MOON};
        ObstaclesCollection[11] = {3,5,BLOCK_MOON};
        ObstaclesCollection[12] = {1,7,BLOCK_MOON};
        ObstaclesCollection[13] = {3,7,BLOCK_MOON};
        ObstaclesCollection[14] = {3,1,BLOCK_MOON};
        ObstaclesCollection[15] = {3,9,BLOCK_MOON};

        ObstaclesCollection[16] = {11,3,BLOCK_MOON};
        ObstaclesCollection[17] = {9,3,BLOCK_MOON};
        ObstaclesCollection[18] = {11,5,BLOCK_MOON};
        ObstaclesCollection[19] = {9,5,BLOCK_MOON};
        ObstaclesCollection[20] = {11,7,BLOCK_MOON};
        ObstaclesCollection[21] = {9,7,BLOCK_MOON};
        ObstaclesCollection[22] = {9,1,BLOCK_MOON};
        ObstaclesCollection[23] = {9,9,BLOCK_MOON};




        TreasureCollection[0] = {1,1, T_RED, false, HEART_SINGLE, 100, true };
        TreasureCollection[1] = {11,1, T_YELLOW, false, HEART_SINGLE, 100, true };
        TreasureCollection[2] = {11,9, T_GREEN, false, HEART_SINGLE, 100, true };
        TreasureCollection[3] = {1,9, T_BLUE, false, HEART_SINGLE, 100, true };

        MonstersCollection[0] = { 2, 1, 1, 9, VERTICAL, BAS, BAT };
        MonstersCollection[1] = { 10, 1, 1, 9, VERTICAL, HAUT, BAT };
        MonstersCollection[2] = { 1, 2, 1, 11, HORIZONTAL, DROITE, BAT };
        MonstersCollection[3] = { 11, 8, 1, 11, HORIZONTAL, GAUCHE, BAT };


    }
    else if (numLevel==12) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 2;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 1;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 3;
        NbObstacleLevel = 0;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=0;
        Map[numLevel].y=-4;
        Map[numLevel].R=255;
        Map[numLevel].G=0;
        Map[numLevel].B=0;
        Map[numLevel].A=255;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 0, 5, DROITE, 255, 255, 255, false }; // Receptor WHITE

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },6}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[0] = { 6, 10, BAS, false, true, 1, { 0, -1, -1 }, 8}; // Door SOUTH

        TreasureCollection[0] = {6,2, T_RED, false, RED_FRAGMENT, 1000,  true };
        TreasureCollection[1] = {1,2, T_YELLOW, false, HEART_SINGLE, 100,  true };
        TreasureCollection[2] = {11,2, T_YELLOW, false, HEART_SINGLE, 100,  true };
        //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100,  true };

    }
    else if (numLevel==13) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 24;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=2;
        Map[numLevel].y=-4;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];


        ReceptorCollection[0] = { 9, 1, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 4, 10, HAUT, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 3, 3, DROITE, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 5, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
        // ReceptorCollection[4] = { 12, 3, GAUCHE, 255, 0, 255, false }; // Receptor PINK
        // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
        // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
        // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1, 2 },19}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 10}; // Door SOUTH

        ObstaclesCollection[0] = {5,3,BLOCK_WATER};
        ObstaclesCollection[1] = {5,5,BLOCK_WATER};
        ObstaclesCollection[2] = {5,7,BLOCK_WATER};
        ObstaclesCollection[3] = {6,6,BLOCK_WATER};
        ObstaclesCollection[4] = {7,3,BLOCK_WATER};
        ObstaclesCollection[5] = {7,5,BLOCK_WATER};
        ObstaclesCollection[6] = {7,7,BLOCK_WATER};
        ObstaclesCollection[7] = {6,4,BLOCK_WATER};

        ObstaclesCollection[8] = {1,3,BLOCK_SUN};
        ObstaclesCollection[9] = {3,3,BLOCK_SUN};
        ObstaclesCollection[10] = {1,5,BLOCK_SUN};
        ObstaclesCollection[11] = {3,5,BLOCK_SUN};
        ObstaclesCollection[12] = {1,7,BLOCK_SUN};
        ObstaclesCollection[13] = {3,7,BLOCK_SUN};
        ObstaclesCollection[14] = {3,1,BLOCK_SUN};
        ObstaclesCollection[15] = {3,9,BLOCK_SUN};

        ObstaclesCollection[16] = {11,3,BLOCK_SUN};
        ObstaclesCollection[17] = {9,3,BLOCK_SUN};
        ObstaclesCollection[18] = {11,5,BLOCK_SUN};
        ObstaclesCollection[19] = {9,5,BLOCK_SUN};
        ObstaclesCollection[20] = {11,7,BLOCK_SUN};
        ObstaclesCollection[21] = {9,7,BLOCK_SUN};
        ObstaclesCollection[22] = {9,1,BLOCK_SUN};
        ObstaclesCollection[23] = {9,9,BLOCK_SUN};

        TreasureCollection[0] = {1,1, T_RED, false, HEART_SINGLE, 100, true };
        TreasureCollection[1] = {11,1, T_YELLOW, false, HEART_SINGLE, 100, true };
        TreasureCollection[2] = {11,9, T_GREEN, false, HEART_SINGLE, 100, true };
        TreasureCollection[3] = {1,9, T_BLUE, false, HEART_SINGLE, 100, true };

        MonstersCollection[0] = { 2, 1, 1, 9, VERTICAL, BAS, GHOST };
        MonstersCollection[1] = { 10, 9, 1, 9, VERTICAL, HAUT, BAT };
        MonstersCollection[2] = { 1, 2, 1, 11, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[3] = { 11, 8, 1, 11, HORIZONTAL, GAUCHE, SPIDER };
    }
    else if (numLevel==14) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 2;
        NbObstacleLevel = 20;
        NbMonsterLevel = 3;
        NbBossLevel = 1;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-3;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 }, 21}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 0}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 1, { 3, -1, -1 }, 15}; // Door EAST
        //DoorCollection[1] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 0}; // Door SOUTH

        ReceptorCollection[0] = { 1, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 11, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 11, 10, HAUT, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 1, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK

        BossCollection[0] = { 6, 2, 2, 10, HORIZONTAL, GAUCHE, B_GREEN };

        ObstaclesCollection[0] = {1,4,BLOCK_WATER};
        ObstaclesCollection[1] = {1,6,BLOCK_WATER};
        ObstaclesCollection[2] = {3,4,BLOCK_WATER};
        ObstaclesCollection[3] = {3,6,BLOCK_WATER};
        ObstaclesCollection[4] = {4,4,BLOCK_WATER};
        ObstaclesCollection[5] = {4,6,BLOCK_WATER};

        ObstaclesCollection[6] = {8,4,BLOCK_WATER};
        ObstaclesCollection[7] = {8,6,BLOCK_WATER};
        ObstaclesCollection[8] = {9,4,BLOCK_WATER};
        ObstaclesCollection[9] = {9,6,BLOCK_WATER};
        ObstaclesCollection[10] = {11,4,BLOCK_WATER};
        ObstaclesCollection[11] = {11,6,BLOCK_WATER};

        ObstaclesCollection[12] = {5,1,BLOCK_WATER};
        ObstaclesCollection[13] = {7,1,BLOCK_WATER};
        ObstaclesCollection[14] = {5,3,BLOCK_WATER};
        ObstaclesCollection[15] = {7,3,BLOCK_WATER};

        ObstaclesCollection[16] = {5,7,BLOCK_WATER};
        ObstaclesCollection[17] = {7,7,BLOCK_WATER};
        ObstaclesCollection[18] = {5,9,BLOCK_WATER};
        ObstaclesCollection[19] = {7,9,BLOCK_WATER};

        TreasureCollection[0] = {1,5, T_GREEN, false, HEART_FULL, 10000,  true };
        TreasureCollection[1] = {6,9, T_YELLOW, false, PIERRE_BLANCHE, 1000,  true };

        //MonstersCollection[0] = { 6, 2, 4, 8, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[0] = { 6, 8, 4, 8, HORIZONTAL, DROITE, BLOB };

        MonstersCollection[1] = { 2, 5, 3, 7, VERTICAL, HAUT, BAT };
        MonstersCollection[2] = { 10, 5, 3, 7, VERTICAL, BAS, GHOST };

    }
    else if (numLevel==15)  // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 7;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 12;
        NbMonsterLevel = 3;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-2;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 }, 11}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 3, { 4, 5, 6 }, 14}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 3, { 0, 1, 2 }, 16}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 11}; // Door SOUTH

        ReceptorCollection[0] = { 10, 0, BAS, 0, 0, 255, false };
        ReceptorCollection[1] = { 12, 2, GAUCHE, 255, 255, 0, false };
        ReceptorCollection[2] = { 11, 0, BAS, 255, 0, 255, false };

        ReceptorCollection[3] = { 1, 10, HAUT, 0, 0, 0, false };

        ReceptorCollection[4] = { 0, 2, DROITE, 255, 0, 255, false };
        ReceptorCollection[5] = { 1, 0, BAS, 0, 255, 255, false };
        ReceptorCollection[6] = { 3, 0, BAS, 0, 255, 0, false };


        ObstaclesCollection[0] = {6,2,BLOCK_WATER};
        ObstaclesCollection[1] = {5,3,BLOCK_WATER};
        ObstaclesCollection[2] = {7,3,BLOCK_WATER};
        ObstaclesCollection[3] = {4,4,BLOCK_WATER};
        ObstaclesCollection[4] = {8,4,BLOCK_WATER};
        ObstaclesCollection[5] = {3,5,BLOCK_WATER};
        ObstaclesCollection[6] = {9,5,BLOCK_WATER};
        ObstaclesCollection[7] = {4,6,BLOCK_WATER};
        ObstaclesCollection[8] = {8,6,BLOCK_WATER};
        ObstaclesCollection[9] = {5,7,BLOCK_WATER};
        ObstaclesCollection[10] = {7,7,BLOCK_WATER};
        ObstaclesCollection[11] = {6,8,BLOCK_WATER};

        MonstersCollection[0] = { 6, 1, 1, 11, HORIZONTAL, DROITE, GHOST };
        MonstersCollection[1] = { 1, 5, 1, 9, VERTICAL, HAUT, GHOST };
        MonstersCollection[2] = { 11, 5, 1, 9, VERTICAL, HAUT, GHOST };

    }
    else if (numLevel==16) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 9;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 8;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-1;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        ObstaclesCollection[0] = {2,2,BLOCK_SUN};
        ObstaclesCollection[1] = {10,2,BLOCK_SUN};
        ObstaclesCollection[2] = {2,8,BLOCK_SUN};
        ObstaclesCollection[3] = {10,8,BLOCK_SUN};
        ObstaclesCollection[4] = {4,5,BLOCK_WATER};
        ObstaclesCollection[5] = {8,5,BLOCK_WATER};
        ObstaclesCollection[6] = {6,3,BLOCK_WATER};
        ObstaclesCollection[7] = {6,7,BLOCK_WATER};

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 2, 8, HAUT, 0, 255, 0, false };
        ReceptorCollection[1] = { 9, 10, HAUT, 255, 0, 255, false };
        ReceptorCollection[2] = { 12, 2, GAUCHE, 0, 0, 0, false };

        ReceptorCollection[3] = { 10, 8, HAUT, 0, 0, 255, false };
        ReceptorCollection[4] = { 5, 10, HAUT, 255, 0, 0, false };
        ReceptorCollection[5] = { 8, 0, BAS, 0, 0, 0, false };

        ReceptorCollection[6] = { 2, 10, HAUT, 255, 255, 0, false };
        ReceptorCollection[7] = { 8, 10, HAUT, 255, 0, 255, false };
        ReceptorCollection[8] = { 12, 7, GAUCHE, 0, 0, 255, false };

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1, 2 }, 22}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 3, { 3, 4, 5 }, 15}; // Door WEST
        DoorCollection[2] = { 12, 5, DROITE, false, true, 3, { 6, 7, 8 }, 18}; // Door EAST
        //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

        TreasureCollection[0] = {4,4, T_RED, false, PIERRE_BLANCHE, 1000,  true };
        TreasureCollection[1] = {8,4, T_YELLOW, false, HEART_FULL, 1000,  true };
        TreasureCollection[2] = {4,6, T_GREEN, false, HEART_FULL, 1000,  true };
        TreasureCollection[3] = {8,6, T_BLUE, false, PIERRE_BLANCHE, 1000,  true };

        MonstersCollection[0] = { 6, 2, 1, 11, HORIZONTAL, DROITE, GHOST };
        MonstersCollection[1] = { 6, 8, 1, 11, HORIZONTAL, GAUCHE, GHOST };
        MonstersCollection[2] = { 2, 5, 1, 9, VERTICAL, HAUT, GHOST };
        MonstersCollection[3] = { 10, 5, 1, 9, VERTICAL, BAS, GHOST };

    }
    else if (numLevel==17) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 0;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 1;
        NbObstacleLevel = 20;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-0;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 1, 1, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 11, 1, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 11, 9, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 1, 9, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 }, 23}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 16}; // Door WEST
        DoorCollection[2] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 18}; // Door EAST
        //DoorCollection[2] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 4}; // Door SOUTH


        ObstaclesCollection[0] = {1,4,BLOCK_STONE};
        ObstaclesCollection[1] = {1,6,BLOCK_STONE};
        ObstaclesCollection[2] = {3,4,BLOCK_STONE};
        ObstaclesCollection[3] = {3,6,BLOCK_STONE};
        ObstaclesCollection[4] = {4,4,BLOCK_STONE};
        ObstaclesCollection[5] = {4,6,BLOCK_STONE};

        ObstaclesCollection[6] = {8,4,BLOCK_STONE};
        ObstaclesCollection[7] = {8,6,BLOCK_STONE};
        ObstaclesCollection[8] = {9,4,BLOCK_STONE};
        ObstaclesCollection[9] = {9,6,BLOCK_STONE};
        ObstaclesCollection[10] = {11,4,BLOCK_STONE};
        ObstaclesCollection[11] = {11,6,BLOCK_STONE};

        ObstaclesCollection[12] = {5,1,BLOCK_STONE};
        ObstaclesCollection[13] = {7,1,BLOCK_STONE};
        ObstaclesCollection[14] = {5,3,BLOCK_STONE};
        ObstaclesCollection[15] = {7,3,BLOCK_STONE};

        ObstaclesCollection[16] = {5,7,BLOCK_STONE};
        ObstaclesCollection[17] = {7,7,BLOCK_STONE};
        ObstaclesCollection[18] = {5,9,BLOCK_STONE};
        ObstaclesCollection[19] = {7,9,BLOCK_STONE};

        TreasureCollection[0] = {6,9, T_GREEN, false, HEART_FULL, 10000,  true };

        MonstersCollection[0] = { 6, 2, 4, 8, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[1] = { 6, 8, 4, 8, HORIZONTAL, DROITE, BLOB };

        MonstersCollection[2] = { 2, 5, 3, 7, VERTICAL, HAUT, BAT };
        MonstersCollection[3] = { 10, 5, 3, 7, VERTICAL, BAS, GHOST };


    }
    else if (numLevel==18) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 2;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 9;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 8;
        NbMonsterLevel = 4;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=1;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;

        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        ObstaclesCollection[0] = {2,2,BLOCK_SUN};
        ObstaclesCollection[1] = {10,2,BLOCK_SUN};
        ObstaclesCollection[2] = {2,8,BLOCK_SUN};
        ObstaclesCollection[3] = {10,8,BLOCK_SUN};
        ObstaclesCollection[4] = {4,5,BLOCK_WATER};
        ObstaclesCollection[5] = {8,5,BLOCK_WATER};
        ObstaclesCollection[6] = {6,3,BLOCK_WATER};
        ObstaclesCollection[7] = {6,7,BLOCK_WATER};

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 2, 8, HAUT, 0, 255, 0, false };
        ReceptorCollection[1] = { 9, 10, HAUT, 255, 0, 255, false };
        ReceptorCollection[2] = { 12, 2, GAUCHE, 0, 0, 0, false };

        ReceptorCollection[3] = { 10, 8, HAUT, 0, 0, 255, false };
        ReceptorCollection[4] = { 5, 10, HAUT, 255, 0, 0, false };
        ReceptorCollection[5] = { 8, 0, BAS, 0, 0, 0, false };

        ReceptorCollection[6] = { 2, 10, HAUT, 255, 255, 0, false };
        ReceptorCollection[7] = { 8, 10, HAUT, 255, 0, 255, false };
        ReceptorCollection[8] = { 12, 7, GAUCHE, 0, 0, 255, false };

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1, 2 }, 24}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 3, { 3, 4, 5 }, 17}; // Door WEST
        DoorCollection[2] = { 12, 5, DROITE, false, true, 3, { 6, 7, 8 }, 19}; // Door EAST
        //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

        TreasureCollection[0] = {4,4, T_RED, false, PIERRE_BLANCHE, 1000,  true };
        TreasureCollection[1] = {8,4, T_YELLOW, false, HEART_FULL, 1000,  true };
        TreasureCollection[2] = {4,6, T_GREEN, false, HEART_FULL, 1000,  true };
        TreasureCollection[3] = {8,6, T_BLUE, false, PIERRE_BLANCHE, 1000,  true };

        MonstersCollection[0] = { 6, 2, 1, 11, HORIZONTAL, DROITE, BLOB };
        MonstersCollection[1] = { 6, 8, 1, 11, HORIZONTAL, GAUCHE, BLOB };
        MonstersCollection[2] = { 2, 5, 1, 9, VERTICAL, HAUT, BLOB };
        MonstersCollection[3] = { 10, 5, 1, 9, VERTICAL, BAS, BLOB };

    }
    else if (numLevel==19)  // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 7;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 12;
        NbMonsterLevel = 3;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=2;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },12}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 3, { 4, 5, 6 }, 18}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 3, { 0, 1, 2 }, 20}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 13}; // Door SOUTH


        ReceptorCollection[0] = { 8, 0, BAS, 255, 0, 0, false };
        ReceptorCollection[1] = { 9, 0, BAS, 255, 255, 0, false };
        ReceptorCollection[2] = { 11, 0, BAS, 0, 255, 0, false };

        ReceptorCollection[3] = { 1, 10, HAUT, 0, 0, 0, false };

        ReceptorCollection[4] = { 0, 2, DROITE, 255, 0, 255, false };
        ReceptorCollection[5] = { 1, 0, BAS, 0, 255, 255, false };
        ReceptorCollection[6] = { 3, 0, BAS, 0, 255, 0, false };


        ObstaclesCollection[0] = {6,2,BLOCK_WATER};
        ObstaclesCollection[1] = {5,3,BLOCK_WATER};
        ObstaclesCollection[2] = {7,3,BLOCK_WATER};
        ObstaclesCollection[3] = {4,4,BLOCK_WATER};
        ObstaclesCollection[4] = {8,4,BLOCK_WATER};
        ObstaclesCollection[5] = {3,5,BLOCK_WATER};
        ObstaclesCollection[6] = {9,5,BLOCK_WATER};
        ObstaclesCollection[7] = {4,6,BLOCK_WATER};
        ObstaclesCollection[8] = {8,6,BLOCK_WATER};
        ObstaclesCollection[9] = {5,7,BLOCK_WATER};
        ObstaclesCollection[10] = {7,7,BLOCK_WATER};
        ObstaclesCollection[11] = {6,8,BLOCK_WATER};

        MonstersCollection[0] = { 6, 1, 1, 11, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[1] = { 1, 5, 1, 9, VERTICAL, HAUT, SKELETON };
        MonstersCollection[2] = { 11, 5, 1, 9, VERTICAL, HAUT, SKELETON };

    }
    else if (numLevel==20) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 4;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 2;
        NbObstacleLevel = 20;
        NbMonsterLevel = 3;
        NbBossLevel = 1;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=3;
        Map[numLevel].y=-5;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 },25}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { 3, -1, -1 }, 19}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        //DoorCollection[2] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 5}; // Door SOUTH


        ReceptorCollection[0] = { 1, 0, BAS, 255, 0, 0, false };  // Receptor RED
        ReceptorCollection[1] = { 11, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
        ReceptorCollection[2] = { 11, 10, HAUT, 0, 0, 255, false }; // Receptor BLUE
        ReceptorCollection[3] = { 1, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK

        BossCollection[0] = { 6, 2, 2, 10, HORIZONTAL, GAUCHE, B_BLUE };

        ObstaclesCollection[0] = {1,4,BLOCK_WATER};
        ObstaclesCollection[1] = {1,6,BLOCK_WATER};
        ObstaclesCollection[2] = {3,4,BLOCK_WATER};
        ObstaclesCollection[3] = {3,6,BLOCK_WATER};
        ObstaclesCollection[4] = {4,4,BLOCK_WATER};
        ObstaclesCollection[5] = {4,6,BLOCK_WATER};

        ObstaclesCollection[6] = {8,4,BLOCK_WATER};
        ObstaclesCollection[7] = {8,6,BLOCK_WATER};
        ObstaclesCollection[8] = {9,4,BLOCK_WATER};
        ObstaclesCollection[9] = {9,6,BLOCK_WATER};
        ObstaclesCollection[10] = {11,4,BLOCK_WATER};
        ObstaclesCollection[11] = {11,6,BLOCK_WATER};

        ObstaclesCollection[12] = {5,1,BLOCK_WATER};
        ObstaclesCollection[13] = {7,1,BLOCK_WATER};
        ObstaclesCollection[14] = {5,3,BLOCK_WATER};
        ObstaclesCollection[15] = {7,3,BLOCK_WATER};

        ObstaclesCollection[16] = {5,7,BLOCK_WATER};
        ObstaclesCollection[17] = {7,7,BLOCK_WATER};
        ObstaclesCollection[18] = {5,9,BLOCK_WATER};
        ObstaclesCollection[19] = {7,9,BLOCK_WATER};

        TreasureCollection[0] = {11,5, T_BLUE, false, HEART_FULL, 10000,  true };
        TreasureCollection[1] = {6,9, T_YELLOW, false, PIERRE_BLANCHE, 1000,  true };

        //MonstersCollection[0] = { 6, 2, 4, 8, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[0] = { 6, 8, 4, 8, HORIZONTAL, DROITE, SPIDER };

        MonstersCollection[1] = { 2, 5, 3, 7, VERTICAL, HAUT, SKELETON };
        MonstersCollection[2] = { 10, 5, 3, 7, VERTICAL, BAS, GHOST };
    }
    else if (numLevel==21) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 1;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 3;
        NbObstacleLevel = 0;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-3;
        Map[numLevel].y=-6;
        Map[numLevel].R=0;
        Map[numLevel].G=255;
        Map[numLevel].B=0;
        Map[numLevel].A=255;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 0, 5, DROITE, 255, 255, 255, false }; // Receptor WHITE

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },6}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[0] = { 6, 10, BAS, false, true, 1, { 0, -1, -1 }, 14}; // Door SOUTH

        TreasureCollection[0] = {6,2, T_GREEN, false, GREEN_FRAGMENT, 1000,  true };
        TreasureCollection[1] = {1,2, T_YELLOW, false, HEART_SINGLE, 100,  true };
        TreasureCollection[2] = {11,2, T_YELLOW, false, HEART_SINGLE, 100,  true };
        //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100,  true };
    }
    else if (numLevel==22) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 6;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 16;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-1;
        Map[numLevel].y=-6;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 },27}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 3, { 3,4,5 }, 16}; // Door SOUTH

        ReceptorCollection[0] = { 2, 6, DROITE, 0, 255, 255, false };
        ReceptorCollection[1] = { 2, 2, DROITE, 0, 0, 255, false };
        ReceptorCollection[2] = { 5, 2, BAS, 0, 0, 0, false };
        ReceptorCollection[3] = { 7, 2, BAS, 0, 0, 0, false };
        ReceptorCollection[4] = { 10, 2, GAUCHE, 0, 0, 255, false };
        ReceptorCollection[5] = { 10, 6, GAUCHE, 255, 0, 255, false };

        ObstaclesCollection[0] = {2,2,BLOCK_SUN};
        ObstaclesCollection[1] = {5,2,BLOCK_SUN};
        ObstaclesCollection[2] = {7,2,BLOCK_SUN};
        ObstaclesCollection[3] = {10,2,BLOCK_SUN};

        ObstaclesCollection[4] = {2,4,BLOCK_SUN};
        ObstaclesCollection[5] = {5,4,BLOCK_WATER};
        ObstaclesCollection[6] = {7,4,BLOCK_WATER};
        ObstaclesCollection[7] = {10,4,BLOCK_SUN};

        ObstaclesCollection[8] = {2,6,BLOCK_SUN};
        ObstaclesCollection[9] = {5,6,BLOCK_WATER};
        ObstaclesCollection[10] = {7,6,BLOCK_WATER};
        ObstaclesCollection[11] = {10,6,BLOCK_SUN};

        ObstaclesCollection[12] = {2,8,BLOCK_SUN};
        ObstaclesCollection[13] = {5,8,BLOCK_SUN};
        ObstaclesCollection[14] = {7,8,BLOCK_SUN};
        ObstaclesCollection[15] = {10,8,BLOCK_SUN};
    }
    else if (numLevel==23) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 6;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 5;
        NbObstacleLevel = 7;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-0;
        Map[numLevel].y=-6;
        Map[numLevel].R=0;
        Map[numLevel].G=0;
        Map[numLevel].B=0;
        Map[numLevel].A=255;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 4, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 2, 10, HAUT, 0, 0, 255, false }; // Receptor WHITE
        ReceptorCollection[1] = { 3, 10, HAUT, 0, 255, 255, false }; // Receptor WHITE
        ReceptorCollection[2] = { 5, 7, HAUT, 0, 0, 255, false }; // Receptor WHITE
        ReceptorCollection[3] = { 7, 7, HAUT, 255, 0, 0, false }; // Receptor WHITE
        ReceptorCollection[4] = { 9, 10, HAUT, 0, 0, 0, false }; // Receptor WHITE
        ReceptorCollection[5] = { 10, 10, HAUT, 255, 0, 0, false }; // Receptor WHITE


        ObstaclesCollection[0] = {5,9,BLOCK_STONE};
        ObstaclesCollection[1] = {5,8,BLOCK_WATER};
        ObstaclesCollection[2] = {5,7,BLOCK_STONE};
        ObstaclesCollection[3] = {6,7,BLOCK_WATER};
        ObstaclesCollection[4] = {7,7,BLOCK_STONE};
        ObstaclesCollection[5] = {7,8,BLOCK_WATER};
        ObstaclesCollection[6] = {7,9,BLOCK_STONE};

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0, 1, 5 }, 28}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 2, { 2, 3, -1 }, 17}; // Door SOUTH

        TreasureCollection[0] = {6,6, T_GREEN, false, BLACK_FRAGMENT, 1000,  true };
        TreasureCollection[1] = {4,6, T_YELLOW, false, HEART_SINGLE, 100,  true };
        TreasureCollection[2] = {8,6, T_YELLOW, false, HEART_SINGLE, 100,  true };
        TreasureCollection[3] = {2,6, T_BLUE, false, PIERRE_BLANCHE, 1000,  true };
        TreasureCollection[4] = {10,6, T_BLUE, false, PIERRE_BLANCHE, 1000,  true };

    }
    else if (numLevel==24) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 6;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 16;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=1;
        Map[numLevel].y=-6;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,1,2 },29}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 3, { 3,4,5 }, 18}; // Door SOUTH

        ReceptorCollection[0] = { 2, 6, DROITE, 255, 255, 0, false };
        ReceptorCollection[1] = { 2, 2, DROITE, 255, 0, 0, false };
        ReceptorCollection[2] = { 5, 2, BAS, 255, 0, 255, false };
        ReceptorCollection[3] = { 7, 2, BAS, 255, 0, 255, false };
        ReceptorCollection[4] = { 10, 2, GAUCHE, 255, 0, 0, false };
        ReceptorCollection[5] = { 10, 6, GAUCHE, 0, 0, 0, false };

        ObstaclesCollection[0] = {2,2,BLOCK_SUN};
        ObstaclesCollection[1] = {5,2,BLOCK_SUN};
        ObstaclesCollection[2] = {7,2,BLOCK_SUN};
        ObstaclesCollection[3] = {10,2,BLOCK_SUN};

        ObstaclesCollection[4] = {2,4,BLOCK_SUN};
        ObstaclesCollection[5] = {5,4,BLOCK_WATER};
        ObstaclesCollection[6] = {7,4,BLOCK_WATER};
        ObstaclesCollection[7] = {10,4,BLOCK_SUN};

        ObstaclesCollection[8] = {2,6,BLOCK_SUN};
        ObstaclesCollection[9] = {5,6,BLOCK_WATER};
        ObstaclesCollection[10] = {7,6,BLOCK_WATER};
        ObstaclesCollection[11] = {10,6,BLOCK_SUN};

        ObstaclesCollection[12] = {2,8,BLOCK_SUN};
        ObstaclesCollection[13] = {5,8,BLOCK_SUN};
        ObstaclesCollection[14] = {7,8,BLOCK_SUN};
        ObstaclesCollection[15] = {10,8,BLOCK_SUN};
    }
    else if (numLevel==25) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 2;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 1;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 3;
        NbObstacleLevel = 0;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=3;
        Map[numLevel].y=-6;
        Map[numLevel].R=0;
        Map[numLevel].G=0;
        Map[numLevel].B=255;
        Map[numLevel].A=255;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        ReceptorCollection[0] = { 0, 5, DROITE, 255, 255, 255, false }; // Receptor WHITE

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },6}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[0] = { 6, 10, BAS, false, true, 1, { 0, -1, -1 }, 20}; // Door SOUTH

        TreasureCollection[0] = {6,2, T_BLUE, false, BLUE_FRAGMENT, 1000,  true };
        TreasureCollection[1] = {1,2, T_YELLOW, false, HEART_SINGLE, 100,  true };
        TreasureCollection[2] = {11,2, T_YELLOW, false, HEART_SINGLE, 100,  true };
        //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100,  true };
    }
    else if (numLevel==26) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 3;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 12;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-2;
        Map[numLevel].y=-7;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },6}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        DoorCollection[0] = { 12, 5, DROITE, false, true, 3, { 0, 1, 2 }, 27}; // Door EAST
        //DoorCollection[0] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 14}; // Door SOUTH

        ReceptorCollection[0] = { 0, 5, DROITE, 255, 255, 255, false };
        ReceptorCollection[1] = { 2, 1, BAS, 0, 255, 255, false };
        ReceptorCollection[2] = { 2, 9, DROITE, 0, 0, 0, false };

        TreasureCollection[0] = {2, 2, T_BLUE, false, HEART_SINGLE, 100,  true };
        TreasureCollection[1] = {10,2, T_BLUE, false, HEART_SINGLE, 100,  true };
        TreasureCollection[2] = {2, 8, T_BLUE, false, HEART_SINGLE, 100,  true };
        TreasureCollection[3] = {10,8, T_BLUE, false, HEART_SINGLE, 100,  true };

        ObstaclesCollection[0] = {1,1,BLOCK_STONE};
        ObstaclesCollection[1] = {2,1,BLOCK_STONE};
        ObstaclesCollection[2] = {1,2,BLOCK_STONE};

        ObstaclesCollection[3] = {11,1,BLOCK_STONE};
        ObstaclesCollection[4] = {10,1,BLOCK_STONE};
        ObstaclesCollection[5] = {11,2,BLOCK_STONE};

        ObstaclesCollection[6] = {1,9,BLOCK_STONE};
        ObstaclesCollection[7] = {2,9,BLOCK_STONE};
        ObstaclesCollection[8] = {1,8,BLOCK_STONE};

        ObstaclesCollection[9] = {11,9,BLOCK_STONE};
        ObstaclesCollection[10] = {10,9,BLOCK_STONE};
        ObstaclesCollection[11] = {11,8,BLOCK_STONE};

    }
    else if (numLevel==27)  // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 7;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 12;
        NbMonsterLevel = 3;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-1;
        Map[numLevel].y=-7;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 }, 11}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 3, { 4, 5, 6 }, 26}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 3, { 0, 1, 2 }, 28}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 22}; // Door SOUTH



        ReceptorCollection[0] = { 10, 0, BAS, 0, 0, 255, false };
        ReceptorCollection[1] = { 12, 2, GAUCHE, 255, 255, 0, false };
        ReceptorCollection[2] = { 11, 0, BAS, 255, 0, 255, false };

        ReceptorCollection[3] = { 1, 10, HAUT, 0, 0, 0, false };

        ReceptorCollection[4] = { 0, 2, DROITE, 255, 0, 255, false };
        ReceptorCollection[5] = { 1, 0, BAS, 0, 255, 255, false };
        ReceptorCollection[6] = { 3, 0, BAS, 0, 255, 0, false };


        ObstaclesCollection[0] = {6,2,BLOCK_WATER};
        ObstaclesCollection[1] = {5,3,BLOCK_WATER};
        ObstaclesCollection[2] = {7,3,BLOCK_WATER};
        ObstaclesCollection[3] = {4,4,BLOCK_WATER};
        ObstaclesCollection[4] = {8,4,BLOCK_WATER};
        ObstaclesCollection[5] = {3,5,BLOCK_WATER};
        ObstaclesCollection[6] = {9,5,BLOCK_WATER};
        ObstaclesCollection[7] = {4,6,BLOCK_WATER};
        ObstaclesCollection[8] = {8,6,BLOCK_WATER};
        ObstaclesCollection[9] = {5,7,BLOCK_WATER};
        ObstaclesCollection[10] = {7,7,BLOCK_WATER};
        ObstaclesCollection[11] = {6,8,BLOCK_WATER};

        MonstersCollection[0] = { 6, 1, 1, 11, HORIZONTAL, DROITE, GHOST };
        MonstersCollection[1] = { 1, 5, 1, 9, VERTICAL, HAUT, GHOST };
        MonstersCollection[2] = { 11, 5, 1, 9, VERTICAL, HAUT, GHOST };

    }
    else if (numLevel==28)  // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 12;
        NbDoorsLevel = 4;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 0;
        NbMonsterLevel = 4;
        NbBossLevel = 1;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=0;
        Map[numLevel].y=-7;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 6, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 0,7,2 },31}; // Door NORTH
        DoorCollection[1] = { 0, 5, GAUCHE, false, true, 3, { 10, 1, 4 }, 27}; // Door WEST
        DoorCollection[2] = { 12, 5, DROITE, false, true, 3, { 6, 9, 11 }, 29}; // Door EAST
        DoorCollection[3] = { 6, 10, BAS, false, true, 3, { 8, 3, 5 }, 23}; // Door SOUTH


        BossCollection[0] = { 6, 5, 2, 10, HORIZONTAL, GAUCHE, B_BLACK };

        MonstersCollection[0] = { 3, 5, 1, 9, VERTICAL, HAUT, SKELETON };
        MonstersCollection[1] = { 5, 5, 1, 9, VERTICAL, BAS, SKELETON };
        MonstersCollection[2] = { 7, 5, 1, 9, VERTICAL, BAS, SKELETON };
        MonstersCollection[3] = { 9, 5, 1, 9, VERTICAL, HAUT, SKELETON };

        ReceptorCollection[0] = { 1, 0, BAS, 255, 255, 0, false };
        ReceptorCollection[1] = { 0, 6, DROITE, 255, 255, 0, false };
        ReceptorCollection[2] = { 0, 7, DROITE, 255, 255, 255, false };
        ReceptorCollection[3] = { 0, 9, DROITE, 255, 255, 255, false };
        ReceptorCollection[4] = { 2, 10, HAUT, 255, 0, 255, false };
        ReceptorCollection[5] = { 4, 10, HAUT, 255, 0, 255, false };
        ReceptorCollection[6] = { 12, 1, GAUCHE, 255, 0, 0, false };
        ReceptorCollection[7] = { 5, 0, BAS, 0, 0, 0, false };
        ReceptorCollection[8] = { 2, 0, BAS, 255, 0, 0, false };
        ReceptorCollection[9] = { 7, 0, BAS, 0, 0, 0, false };
        ReceptorCollection[10] = { 11, 0, BAS, 255, 0, 0, false };
        ReceptorCollection[11] = { 12, 7, GAUCHE, 255, 0, 0, false };

    }
    else if (numLevel==29)  // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 1;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 7;
        NbDoorsLevel = 3;
        NbBlocksLevel = 4;
        NbTreasureLevel = 0;
        NbObstacleLevel = 12;
        NbMonsterLevel = 3;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=1;
        Map[numLevel].y=-7;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=true;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },12}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 3, { 4, 5, 6 }, 28}; // Door WEST
        DoorCollection[1] = { 12, 5, DROITE, false, true, 3, { 0, 1, 2 }, 30}; // Door EAST
        DoorCollection[2] = { 6, 10, BAS, false, true, 1, { 3, -1, -1 }, 24}; // Door SOUTH


        ReceptorCollection[0] = { 8, 0, BAS, 255, 0, 0, false };
        ReceptorCollection[1] = { 9, 0, BAS, 255, 255, 0, false };
        ReceptorCollection[2] = { 11, 0, BAS, 0, 255, 0, false };

        ReceptorCollection[3] = { 1, 10, HAUT, 0, 0, 0, false };

        ReceptorCollection[4] = { 0, 2, DROITE, 255, 0, 255, false };
        ReceptorCollection[5] = { 1, 0, BAS, 0, 255, 255, false };
        ReceptorCollection[6] = { 3, 0, BAS, 0, 255, 0, false };


        ObstaclesCollection[0] = {6,2,BLOCK_WATER};
        ObstaclesCollection[1] = {5,3,BLOCK_WATER};
        ObstaclesCollection[2] = {7,3,BLOCK_WATER};
        ObstaclesCollection[3] = {4,4,BLOCK_WATER};
        ObstaclesCollection[4] = {8,4,BLOCK_WATER};
        ObstaclesCollection[5] = {3,5,BLOCK_WATER};
        ObstaclesCollection[6] = {9,5,BLOCK_WATER};
        ObstaclesCollection[7] = {4,6,BLOCK_WATER};
        ObstaclesCollection[8] = {8,6,BLOCK_WATER};
        ObstaclesCollection[9] = {5,7,BLOCK_WATER};
        ObstaclesCollection[10] = {7,7,BLOCK_WATER};
        ObstaclesCollection[11] = {6,8,BLOCK_WATER};

        MonstersCollection[0] = { 6, 1, 1, 11, HORIZONTAL, DROITE, SKELETON };
        MonstersCollection[1] = { 1, 5, 1, 9, VERTICAL, HAUT, SKELETON };
        MonstersCollection[2] = { 11, 5, 1, 9, VERTICAL, HAUT, SKELETON };

    }
    else if (numLevel==30) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 3;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 12;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=2;
        Map[numLevel].y=-7;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=true;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=false;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },12}; // Door NORTH
        DoorCollection[0] = { 0, 5, GAUCHE, false, true, 3, { 0, 1, 2 }, 29}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 30}; // Door EAST
        //DoorCollection[2] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 24}; // Door SOUTH

        ReceptorCollection[0] = { 12, 5, GAUCHE, 255, 255, 255, false };
        ReceptorCollection[1] = { 10, 1, BAS, 255, 0, 255, false };
        ReceptorCollection[2] = { 2, 9, DROITE, 0, 0, 0, false };

        TreasureCollection[0] = {2, 2, T_BLUE, false, HEART_SINGLE, 100,  true };
        TreasureCollection[1] = {10,2, T_BLUE, false, HEART_SINGLE, 100,  true };
        TreasureCollection[2] = {2, 8, T_BLUE, false, HEART_SINGLE, 100,  true };
        TreasureCollection[3] = {10,8, T_BLUE, false, HEART_SINGLE, 100,  true };

        ObstaclesCollection[0] = {1,1,BLOCK_STONE};
        ObstaclesCollection[1] = {2,1,BLOCK_STONE};
        ObstaclesCollection[2] = {1,2,BLOCK_STONE};

        ObstaclesCollection[3] = {11,1,BLOCK_STONE};
        ObstaclesCollection[4] = {10,1,BLOCK_STONE};
        ObstaclesCollection[5] = {11,2,BLOCK_STONE};

        ObstaclesCollection[6] = {1,9,BLOCK_STONE};
        ObstaclesCollection[7] = {2,9,BLOCK_STONE};
        ObstaclesCollection[8] = {1,8,BLOCK_STONE};

        ObstaclesCollection[9] = {11,9,BLOCK_STONE};
        ObstaclesCollection[10] = {10,9,BLOCK_STONE};
        ObstaclesCollection[11] = {11,8,BLOCK_STONE};
    }
    else if (numLevel==31)  // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 5;
        NbDoorsLevel = 2;
        NbBlocksLevel = 4;
        NbTreasureLevel = 4;
        NbObstacleLevel = 13;
        NbMonsterLevel = 0;
        NbBossLevel = 1;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=-0;
        Map[numLevel].y=-8;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=0;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=true;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 6, 7, 1,1,1,0, false };

        BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        DoorCollection[0] = { 6,0, HAUT, false, true, 3, { 2,3,4 },32}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[1] = { 6, 10, BAS, false, true, 2, { 0,1, -1 }, 28}; // Door SOUTH


        ReceptorCollection[0] = { 1, 1, DROITE, 255, 0, 0, false };
        ReceptorCollection[1] = { 11, 1, GAUCHE, 255, 0, 0, false };
        ReceptorCollection[2] = { 6, 3, BAS, 0, 0, 0, false }; // Receptor WHITE
        ReceptorCollection[3] = { 3, 1, BAS, 255, 0, 255, false }; // Receptor WHITE
        ReceptorCollection[4] = { 9, 1, BAS, 255, 255, 0, false }; // Receptor WHITE


        TreasureCollection[0] = {1,5, T_RED, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[1] = {11,5, T_YELLOW, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[2] = {11,7, T_GREEN, false, PIERRE_BLANCHE, 100, true };
        TreasureCollection[3] = {1,7, T_BLUE, false, PIERRE_BLANCHE, 100, true };

        ObstaclesCollection[0] = {1,1,BLOCK_STONE};
        ObstaclesCollection[1] = {3,1,BLOCK_STONE};
        ObstaclesCollection[2] = {5,1,BLOCK_STONE};

        ObstaclesCollection[3] = {7,1,BLOCK_STONE};
        ObstaclesCollection[4] = {9,1,BLOCK_STONE};
        ObstaclesCollection[5] = {11,1,BLOCK_STONE};

        ObstaclesCollection[6] = {1,9,BLOCK_STONE};
        ObstaclesCollection[7] = {3,9,BLOCK_STONE};
        ObstaclesCollection[8] = {5,9,BLOCK_STONE};

        ObstaclesCollection[9] = {7,9,BLOCK_STONE};
        ObstaclesCollection[10] = {9,9,BLOCK_STONE};
        ObstaclesCollection[11] = {11,9,BLOCK_STONE};

        ObstaclesCollection[12] = {6,3,BLOCK_STONE};


        BossCollection[0] = { 6, 4, 1, 11, HORIZONTAL, GAUCHE, BIGBOSS };
    }
    else if (numLevel==32) // VALIDATED [OK]
    {
        currentLevel = numLevel;

        LEVELCOLOR = 0;
        SIZE_MAP_X=13;
        SIZE_MAP_Y=11;

        NbReceptorsLevel = 0;
        NbDoorsLevel = 1;
        NbBlocksLevel = 4;
        NbTreasureLevel = 1;
        NbObstacleLevel = 32;
        NbMonsterLevel = 0;
        NbBossLevel = 0;

        BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
        ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
        DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
        TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
        ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
        MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
        BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

        Map[numLevel].x=0;
        Map[numLevel].y=-9;
        Map[numLevel].R=255;
        Map[numLevel].G=255;
        Map[numLevel].B=255;
        Map[numLevel].A=255;
        Map[numLevel].visited=true;
        Map[numLevel].doorWest=false;
        Map[numLevel].doorNorth=false;
        Map[numLevel].doorEast=false;
        Map[numLevel].doorSouth=true;


        joueur = { 6, 9, HAUT };
        lumiere = { 3, 5, 1,1,1,0, false };

        BlocksCollection[0] = { 2, 4, 255, 0, 0, 127, TRANSPARENT };
        BlocksCollection[1] = { 2, 5, 0, 255, 0, 127, TRANSPARENT };
        BlocksCollection[2] = { 2, 6, 0, 0, 255, 127, TRANSPARENT };
        BlocksCollection[3] = { 10, 5, 0, 0, 0, 255, OPAQUE };

        BlocRouge = &BlocksCollection[0];
        BlocVert = &BlocksCollection[1];
        BlocBleu = &BlocksCollection[2];
        BlocNoir = &BlocksCollection[3];

        //DoorCollection[0] = { 6,0, HAUT, false, true, 0, { -1,-1, -1 },6}; // Door NORTH
        //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 0, { -1, -1, -1 }, 9}; // Door WEST
        //DoorCollection[1] = { 12, 5, DROITE, false, true, 0, { -1, -1, -1 }, 10}; // Door EAST
        DoorCollection[0] = { 6, 10, BAS, false, true, 0, { -1, -1, -1 }, 31}; // Door SOUTH

        TreasureCollection[0] = {6,2, T_BLUE, false, WHITE_FRAGMENT, 10000,  true };

    ObstaclesCollection[0] = {1,1,BLOCK_STONE};
    ObstaclesCollection[1] = {2,1,BLOCK_STONE};
    ObstaclesCollection[2] = {1,2,BLOCK_STONE};

    ObstaclesCollection[3] = {11,1,BLOCK_STONE};
    ObstaclesCollection[4] = {10,1,BLOCK_STONE};
    ObstaclesCollection[5] = {11,2,BLOCK_STONE};

    ObstaclesCollection[6] = {1,9,BLOCK_STONE};
    ObstaclesCollection[7] = {2,9,BLOCK_STONE};
    ObstaclesCollection[8] = {1,8,BLOCK_STONE};

    ObstaclesCollection[9] = {11,9,BLOCK_STONE};
    ObstaclesCollection[10] = {10,9,BLOCK_STONE};
    ObstaclesCollection[11] = {11,8,BLOCK_STONE};

    ObstaclesCollection[12] = {4,1,BLOCK_WATER};
    ObstaclesCollection[13] = {4,2,BLOCK_WATER};
    ObstaclesCollection[14] = {4,3,BLOCK_WATER};
    ObstaclesCollection[15] = {4,4,BLOCK_WATER};
    ObstaclesCollection[16] = {5,4,BLOCK_WATER};
    ObstaclesCollection[17] = {5,5,BLOCK_WATER};
    ObstaclesCollection[18] = {5,6,BLOCK_WATER};
    ObstaclesCollection[19] = {5,7,BLOCK_WATER};
    ObstaclesCollection[20] = {5,8,BLOCK_WATER};
    ObstaclesCollection[21] = {5,9,BLOCK_WATER};


    ObstaclesCollection[22] = {8,1,BLOCK_WATER};
    ObstaclesCollection[23] = {8,2,BLOCK_WATER};
    ObstaclesCollection[24] = {8,3,BLOCK_WATER};
    ObstaclesCollection[25] = {8,4,BLOCK_WATER};
    ObstaclesCollection[26] = {7,4,BLOCK_WATER};
    ObstaclesCollection[27] = {7,5,BLOCK_WATER};
    ObstaclesCollection[28] = {7,6,BLOCK_WATER};
    ObstaclesCollection[29] = {7,7,BLOCK_WATER};
    ObstaclesCollection[30] = {7,8,BLOCK_WATER};
    ObstaclesCollection[31] = {7,9,BLOCK_WATER};


    }
}



bool checkNextPositionMonster( unsigned int Xtarget, unsigned int Ytarget, unsigned int direction )
{

    for( unsigned char k=0; k<NbObstacleLevel; k++)
    {
        if (ObstaclesCollection[k].x==Xtarget  && ObstaclesCollection[k].y==Ytarget)
            return false;
    }

    //if (Xtarget>=lumiere.x-1 && Xtarget<=lumiere.x+1 && Ytarget>=lumiere.y-1 && Ytarget<=lumiere.y+1)
    if (Xtarget==lumiere.x && Ytarget==lumiere.y)
    {
        return false;
    }
    else if (Xtarget==joueur.x && Ytarget==joueur.y)
    {
    //    if (life>0) life--;
        return false;
    }
    else if (Xtarget==BlocRouge->x && Ytarget==BlocRouge->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y-1, HAUT))
            {
                BlocRouge->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y+1, BAS))
            {
                BlocRouge->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( ROUGE, BlocRouge->x-1, BlocRouge->y, GAUCHE))
            {
                BlocRouge->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( ROUGE, BlocRouge->x+1, BlocRouge->y, DROITE))
            {
                BlocRouge->x+=1;
                return true;
            }
            else return false;
        }
    }
    else if (Xtarget==BlocVert->x && Ytarget==BlocVert->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( VERT, BlocVert->x, BlocVert->y-1, HAUT))
            {
                BlocVert->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( VERT, BlocVert->x, BlocVert->y+1, BAS))
            {
                BlocVert->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( VERT, BlocVert->x-1, BlocVert->y, GAUCHE))
            {
                BlocVert->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( VERT, BlocVert->x+1, BlocVert->y, DROITE))
            {
                BlocVert->x+=1;
                return true;
            }
            else return false;
        }
    }
    else if (Xtarget==BlocBleu->x && Ytarget==BlocBleu->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( BLEU, BlocBleu->x, BlocBleu->y-1, HAUT))
            {
                BlocBleu->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( BLEU, BlocBleu->x, BlocBleu->y+1, BAS))
            {
                BlocBleu->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( BLEU, BlocBleu->x-1, BlocBleu->y, GAUCHE))
            {
                BlocBleu->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( BLEU, BlocBleu->x+1, BlocBleu->y, DROITE))
            {
                BlocBleu->x+=1;
                return true;
            }
            else return false;
        }
    }
    else if (Xtarget==BlocNoir->x && Ytarget==BlocNoir->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( NOIR, BlocNoir->x, BlocNoir->y-1, HAUT))
            {
                BlocNoir->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( NOIR, BlocNoir->x, BlocNoir->y+1, BAS))
            {
                BlocNoir->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( NOIR, BlocNoir->x-1, BlocNoir->y, GAUCHE))
            {
                BlocNoir->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( NOIR, BlocNoir->x+1, BlocNoir->y, DROITE))
            {
                BlocNoir->x+=1;
                return true;
            }
            else return false;
        }
    }

    return true;
}


void updateMonsters( void )
{
    for( unsigned char k=0; k<NbMonsterLevel; k++ )
    {
        unsigned int X = MonstersCollection[k].xcur;
        unsigned int Y = MonstersCollection[k].ycur;
        unsigned char mini = MonstersCollection[k].mini;
        unsigned char maxi = MonstersCollection[k].maxi;
        unsigned int direction = MonstersCollection[k].direction;
        unsigned int sens = MonstersCollection[k].sens;

        if (direction==VERTICAL)
        {
            if (sens==BAS)
            {
                if (Y<maxi && checkNextPositionMonster( X, Y+1, BAS)==true)
                {
                    MonstersCollection[k].ycur++;
                    MonstersCollection[k].sens=BAS;
                }
                else
                {
                    MonstersCollection[k].sens=HAUT;
                }
            }
            else if (sens==HAUT)
            {
                if (Y>mini && checkNextPositionMonster( X, Y-1, HAUT)==true)
                {
                    MonstersCollection[k].ycur--;
                    MonstersCollection[k].sens=HAUT;
                }
                else
                {
                    MonstersCollection[k].sens=BAS;
                }
            }
        }
        else if (direction==HORIZONTAL)
        {

            if (sens==DROITE)
            {
                if (X<maxi && checkNextPositionMonster( X+1, Y, DROITE)==true)
                {
                    MonstersCollection[k].xcur++;
                    MonstersCollection[k].sens=DROITE;
                }
                else
                {
                    MonstersCollection[k].sens=GAUCHE;
                }
            }
            else if (sens==GAUCHE)
            {
                if (X>mini && checkNextPositionMonster( X-1, Y, GAUCHE)==true)
                {
                    MonstersCollection[k].xcur--;
                    MonstersCollection[k].sens=GAUCHE;
                }
                else
                {
                    MonstersCollection[k].sens=DROITE;
                }
            }
        }

    }
}

void renderMonsters( SDL_Surface *surf )
{
    for( unsigned char k=0; k<NbMonsterLevel; k++ )
    {
        unsigned int lX = SIZE*MonstersCollection[k].xcur+OFFSET_X;
        unsigned int lY = SIZE*MonstersCollection[k].ycur+OFFSET_Y;
        unsigned char mini = MonstersCollection[k].mini;
        unsigned char maxi = MonstersCollection[k].maxi;
        unsigned int direction = MonstersCollection[k].direction;
        unsigned int sens = MonstersCollection[k].sens;
        unsigned int type = MonstersCollection[k].kind;

        unsigned int OFFSET_X_MONSTER = 48*type;

        SDL_Rect source, dest;

        dest.w = SIZE * ZOOMFACTOR;
        dest.h = SIZE * ZOOMFACTOR;
        source.w = SIZE * ZOOMFACTOR;
        source.h = SIZE * ZOOMFACTOR;

        if (direction==VERTICAL)
        {


                              if (true)
                             {
                                lineRGBA( surf, lX+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                lineRGBA( surf, lX+1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                lineRGBA( surf, lX-1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX-1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                             }

            if (sens==BAS && MonstersCollection[k].ycur<maxi)
            {
                //                        if (frame_monster==0)
                {
                    //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,0,16,16,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 0;
                    SDL_BlitSurface( monstres, &source, surf, &dest );
                }
                /*                            else if (frame_monster==1)
                                            {
                                                   //dsubimage( lX, lY+3, &monstres, OFFSET_X_MONSTER+16,0,16,16,  DIMAGE_NONE);
                                                   dest.x = lX;
                                                   dest.y = lY+3;
                                                   source.x = OFFSET_X_MONSTER+16;
                                                   source.y = 0;
                                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                                            }
                                            else if (frame_monster==2)
                                            {
                                                   //dsubimage( lX, lY+5, &monstres, OFFSET_X_MONSTER+32,0,16,16,  DIMAGE_NONE);
                                                   dest.x = lX;
                                                   dest.y = lY+5;
                                                   source.x = OFFSET_X_MONSTER+32;
                                                   source.y = 0;
                                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                                            }
                                            else if (frame_monster==3)
                                            {
                                                   //dsubimage( lX, lY+8, &monstres, OFFSET_X_MONSTER,0,16,16,  DIMAGE_NONE);
                                                   dest.x = lX;
                                                   dest.y = lY+8;
                                                   source.x = OFFSET_X_MONSTER;
                                                   source.y = 0;
                                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                                            }
                                            else if (frame_monster==4)
                                            {
                                                   //dsubimage( lX, lY+11, &monstres, OFFSET_X_MONSTER+16,0,16,16,  DIMAGE_NONE);
                                                   dest.x = lX;
                                                   dest.y = lY+11;
                                                   source.x = OFFSET_X_MONSTER+16;
                                                   source.y = 0;
                                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                                            }
                                            else if (frame_monster==5)
                                            {
                                                   //dsubimage( lX, lY+13, &monstres, OFFSET_X_MONSTER+32,0,16,16,  DIMAGE_NONE);
                                                   dest.x = lX;
                                                   dest.y = lY+13;
                                                   source.x = OFFSET_X_MONSTER+32;
                                                   source.y = 0;
                                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                                            }
                  */
            }
            else if (sens==BAS && MonstersCollection[k].ycur==maxi)
            {
                //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,0,16,16,  DIMAGE_NONE);
                dest.x = lX;
                dest.y = lY;
                source.x = OFFSET_X_MONSTER;
                source.y = 0;
                SDL_BlitSurface( monstres, &source, surf, &dest );
            }
            else if (sens==HAUT && MonstersCollection[k].ycur>mini)
            {
                //                         if (frame_monster==0)
                {
                    //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,32,16,16,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 32;
                    SDL_BlitSurface( monstres, &source, surf, &dest );
                }
                /*                       else if (frame_monster==1)
                                       {
                                              //dsubimage( lX, lY-3, &monstres, OFFSET_X_MONSTER+16,32,16,16,  DIMAGE_NONE);
                                              dest.x = lX;
                                              dest.y = lY-3;
                                              source.x = OFFSET_X_MONSTER+16;
                                              source.y = 32;
                                              SDL_BlitSurface( monstres, &source, surf, &dest );
                                       }
                                       else if (frame_monster==2)
                                       {
                                              //dsubimage( lX, lY-5, &monstres, OFFSET_X_MONSTER+32,32,16,16,  DIMAGE_NONE);
                                              dest.x = lX;
                                              dest.y = lY-5;
                                              source.x = OFFSET_X_MONSTER+32;
                                              source.y = 32;
                                              SDL_BlitSurface( monstres, &source, surf, &dest );
                                       }
                                       else if (frame_monster==3)
                                       {
                                              //dsubimage( lX, lY-8, &monstres, OFFSET_X_MONSTER,32,16,16,  DIMAGE_NONE);
                                              dest.x = lX;
                                              dest.y = lY-8;
                                              source.x = OFFSET_X_MONSTER;
                                              source.y = 32;
                                              SDL_BlitSurface( monstres, &source, surf, &dest );
                                       }
                                       else if (frame_monster==4)
                                       {
                                              //dsubimage( lX, lY-11, &monstres, OFFSET_X_MONSTER+16,32,16,16,  DIMAGE_NONE);
                                              dest.x = lX;
                                              dest.y = lY-11;
                                              source.x = OFFSET_X_MONSTER+16;
                                              source.y = 32;
                                              SDL_BlitSurface( monstres, &source, surf, &dest );
                                       }
                                       else if (frame_monster==5)
                                       {
                                              //dsubimage( lX, lY-13, &monstres, OFFSET_X_MONSTER+32,32,16,16,  DIMAGE_NONE);
                                              dest.x = lX;
                                              dest.y = lY-13;
                                              source.x = OFFSET_X_MONSTER+32;
                                              source.y = 32;
                                              SDL_BlitSurface( monstres, &source, surf, &dest );
                                       }
                */
            }
            else if (sens==HAUT && MonstersCollection[k].ycur==mini)
            {
                //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,32,16,16,  DIMAGE_NONE);
                dest.x = lX;
                dest.y = lY;
                source.x = OFFSET_X_MONSTER;
                source.y = 32;
                SDL_BlitSurface( monstres, &source, surf, &dest );
            }
        }

        else if (direction==HORIZONTAL)
        {


                             if (true)
                             {
                                lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, 255, 0, 0, 255 );
                                lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, 255, 0, 0, 255 );
                                lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, 255, 0, 0, 255 );
                             }


            if (sens==GAUCHE && MonstersCollection[k].xcur>mini)
            {
                //                        if (frame_monster==0)
                {
                    //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,48,16,16,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 48;
                    SDL_BlitSurface( monstres, &source, surf, &dest );
                }
                /*                          else if (frame_monster==1)
                                          {
                                                 //dsubimage( lX-3, lY, &monstres, OFFSET_X_MONSTER+16,48,16,16,  DIMAGE_NONE);
                                                 dest.x = lX-3;
                                                 dest.y = lY;
                                                 source.x = OFFSET_X_MONSTER+16;
                                                 source.y = 48;
                                                 SDL_BlitSurface( monstres, &source, surf, &dest );
                                          }
                                          else if (frame_monster==2)
                                          {
                                                 //dsubimage( lX-5, lY, &monstres, OFFSET_X_MONSTER+32,48,16,16,  DIMAGE_NONE);
                                                 dest.x = lX-5;
                                                 dest.y = lY;
                                                 source.x = OFFSET_X_MONSTER+32;
                                                 source.y = 48;
                                                 SDL_BlitSurface( monstres, &source, surf, &dest );
                                          }
                                          else if (frame_monster==3)
                                          {
                                                 //dsubimage( lX-8, lY, &monstres, OFFSET_X_MONSTER,48,16,16,  DIMAGE_NONE);
                                                 dest.x = lX-8;
                                                 dest.y = lY;
                                                 source.x = OFFSET_X_MONSTER;
                                                 source.y = 48;
                                                 SDL_BlitSurface( monstres, &source, surf, &dest );
                                          }
                                          else if (frame_monster==4)
                                          {
                                                 //dsubimage( lX-11, lY, &monstres, OFFSET_X_MONSTER+16,48,16,16,  DIMAGE_NONE);
                                                 dest.x = lX-11;
                                                 dest.y = lY;
                                                 source.x = OFFSET_X_MONSTER+16;
                                                 source.y = 48;
                                                 SDL_BlitSurface( monstres, &source, surf, &dest );
                                          }
                                          else if (frame_monster==5)
                                          {
                                                 //dsubimage( lX-13, lY, &monstres, OFFSET_X_MONSTER+32,48,16,16,  DIMAGE_NONE);
                                                 dest.x = lX-13;
                                                 dest.y = lY;
                                                 source.x = OFFSET_X_MONSTER+32;
                                                 source.y = 48;
                                                 SDL_BlitSurface( monstres, &source, surf, &dest );
                                          }
                   */
            }
            else if (sens==GAUCHE && MonstersCollection[k].xcur==mini)
            {
                //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,48,16,16,  DIMAGE_NONE);
                dest.x = lX;
                dest.y = lY;
                source.x = OFFSET_X_MONSTER;
                source.y = 48;
                SDL_BlitSurface( monstres, &source, surf, &dest );
            }
            else if (sens==DROITE && MonstersCollection[k].xcur<maxi)
            {
                //                      if (frame_monster==0)
                {
                    //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,16,16,16,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 16;
                    SDL_BlitSurface( monstres, &source, surf, &dest );
                }
                /*                     else if (frame_monster==1)
                                     {
                                            //dsubimage( lX+3, lY, &monstres, OFFSET_X_MONSTER+16,16,16,16,  DIMAGE_NONE);
                                            dest.x = lX+3;
                                            dest.y = lY;
                                            source.x = OFFSET_X_MONSTER+16;
                                            source.y = 16;
                                            SDL_BlitSurface( monstres, &source, surf, &dest );
                                     }
                                     else if (frame_monster==2)
                                     {
                                            //dsubimage( lX+5, lY, &monstres, OFFSET_X_MONSTER+32,16,16,16,  DIMAGE_NONE);
                                            dest.x = lX+5;
                                            dest.y = lY;
                                            source.x = OFFSET_X_MONSTER+32;
                                            source.y = 16;
                                            SDL_BlitSurface( monstres, &source, surf, &dest );
                                     }
                                     else if (frame_monster==3)
                                     {
                                            //dsubimage( lX+8, lY, &monstres, OFFSET_X_MONSTER,16,16,16,  DIMAGE_NONE);
                                            dest.x = lX+8;
                                            dest.y = lY;
                                            source.x = OFFSET_X_MONSTER;
                                            source.y = 16;
                                            SDL_BlitSurface( monstres, &source, surf, &dest );
                                     }
                                     else if (frame_monster==4)
                                     {
                                            //dsubimage( lX+11, lY, &monstres, OFFSET_X_MONSTER+16,16,16,16,  DIMAGE_NONE);
                                            dest.x = lX+11;
                                            dest.y = lY;
                                            source.x = OFFSET_X_MONSTER+16;
                                            source.y = 16;
                                            SDL_BlitSurface( monstres, &source, surf, &dest );
                                     }
                                     else if (frame_monster==5)
                                     {
                                            //dsubimage( lX+13, lY, &monstres, OFFSET_X_MONSTER+32,16,16,16,  DIMAGE_NONE);
                                            dest.x = lX+13;
                                            dest.y = lY;
                                            source.x = OFFSET_X_MONSTER+32;
                                            source.y = 16;
                                            SDL_BlitSurface( monstres, &source, surf, &dest );
                                     }
                */
            }
            else if (sens==DROITE && MonstersCollection[k].xcur==maxi)
            {
                //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,16,16,16,  DIMAGE_NONE);
                dest.x = lX;
                dest.y = lY;
                source.x = OFFSET_X_MONSTER;
                source.y = 16;
                SDL_BlitSurface( monstres, &source, surf, &dest );
            }
        }

    }

//      frame_monster++;
//      if (frame_monster==6)
//      {
    updateMonsters();
//             frame_monster=0;
//      }

}

bool checkNextPositionBoss( unsigned int Xtarget, unsigned int Ytarget, unsigned int direction )
{

    for( unsigned char k=0; k<NbObstacleLevel; k++)
    {
        if (ObstaclesCollection[k].x==Xtarget  && ObstaclesCollection[k].y==Ytarget)
            return false;
    }

    //if (Xtarget>=lumiere.x-1 && Xtarget<=lumiere.x+1 && Ytarget>=lumiere.y-1 && Ytarget<=lumiere.y+1)
    if (Xtarget==lumiere.x && Ytarget==lumiere.y)
    {
        return false;
    }
    //     else if (Xtarget==joueur.x && Ytarget==joueur.y)
    //     {
    //            if (life>0) life--;
    //            return false;
    //     }
    else if (Xtarget==BlocRouge->x && Ytarget==BlocRouge->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y-1, HAUT))
            {
                BlocRouge->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y+1, BAS))
            {
                BlocRouge->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( ROUGE, BlocRouge->x-1, BlocRouge->y, GAUCHE))
            {
                BlocRouge->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( ROUGE, BlocRouge->x+1, BlocRouge->y, DROITE))
            {
                BlocRouge->x+=1;
                return true;
            }
            else return false;
        }
    }
    else if (Xtarget==BlocVert->x && Ytarget==BlocVert->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( ROUGE, BlocVert->x, BlocVert->y-1, HAUT))
            {
                BlocVert->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( ROUGE, BlocVert->x, BlocVert->y+1, BAS))
            {
                BlocVert->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( ROUGE, BlocVert->x-1, BlocVert->y, GAUCHE))
            {
                BlocVert->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( ROUGE, BlocVert->x+1, BlocVert->y, DROITE))
            {
                BlocVert->x+=1;
                return true;
            }
            else return false;
        }
    }
    else if (Xtarget==BlocBleu->x && Ytarget==BlocBleu->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( ROUGE, BlocBleu->x, BlocBleu->y-1, HAUT))
            {
                BlocBleu->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( ROUGE, BlocBleu->x, BlocBleu->y+1, BAS))
            {
                BlocBleu->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( ROUGE, BlocBleu->x-1, BlocBleu->y, GAUCHE))
            {
                BlocBleu->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( ROUGE, BlocBleu->x+1, BlocBleu->y, DROITE))
            {
                BlocBleu->x+=1;
                return true;
            }
            else return false;
        }
    }
    else if (Xtarget==BlocNoir->x && Ytarget==BlocNoir->y)
    {
        if (direction==HAUT)
        {
            if (isValidMove( ROUGE, BlocNoir->x, BlocNoir->y-1, HAUT))
            {
                BlocNoir->y-=1;
                return true;
            }
            else return false;
        }
        else if (direction==BAS)
        {
            if (isValidMove( ROUGE, BlocNoir->x, BlocNoir->y+1, BAS))
            {
                BlocNoir->y+=1;
                return true;
            }
            else return false;
        }
        else if (direction==GAUCHE)
        {
            if (isValidMove( ROUGE, BlocNoir->x-1, BlocNoir->y, GAUCHE))
            {
                BlocNoir->x-=1;
                return true;
            }
            else return false;
        }
        else if (direction==DROITE)
        {
            if (isValidMove( ROUGE, BlocNoir->x+1, BlocNoir->y, DROITE))
            {
                BlocNoir->x+=1;
                return true;
            }
            else return false;
        }
    }

    return true;
}

void updateBoss( void )
{
    for( unsigned char k=0; k<NbBossLevel; k++ )
    {
        unsigned int X = BossCollection[k].xcur;
        unsigned int Y = BossCollection[k].ycur;
        unsigned char mini = BossCollection[k].mini;
        unsigned char maxi = BossCollection[k].maxi;
        unsigned int direction = BossCollection[k].direction;
        unsigned int sens = BossCollection[k].sens;

        if (direction==VERTICAL)
        {
            if (sens==BAS)
            {
                if (Y<maxi && checkNextPositionMonster( X, Y+1, BAS)==true)
                {
                    BossCollection[k].ycur++;
                    BossCollection[k].sens=BAS;
                }
                else
                {
                    BossCollection[k].sens=HAUT;
                }
            }
            else if (sens==HAUT)
            {
                if (Y>mini && checkNextPositionMonster( X, Y-1, HAUT)==true)
                {
                    BossCollection[k].ycur--;
                    BossCollection[k].sens=HAUT;
                }
                else
                {
                    BossCollection[k].sens=BAS;
                }
            }
        }
        else if (direction==HORIZONTAL)
        {

            if (sens==DROITE)
            {
                if (X<maxi && checkNextPositionMonster( X+1, Y, DROITE)==true)
                {
                    BossCollection[k].xcur++;
                    BossCollection[k].sens=DROITE;
                }
                else
                {
                    BossCollection[k].sens=GAUCHE;
                }
            }
            else if (sens==GAUCHE)
            {
                if (X>mini && checkNextPositionMonster( X-1, Y, GAUCHE)==true)
                {
                    BossCollection[k].xcur--;
                    BossCollection[k].sens=GAUCHE;
                }
                else
                {
                    BossCollection[k].sens=DROITE;
                }
            }
        }

    }
}

void renderBoss( SDL_Surface *surf )
{

    for( unsigned char k=0; k<NbBossLevel; k++ )
    {
        unsigned int lX = SIZE*BossCollection[k].xcur+OFFSET_X;
        unsigned int lY = SIZE*BossCollection[k].ycur+OFFSET_Y - 2; //Boss are 18 pixel high instead of 16
        unsigned char mini = BossCollection[k].mini;
        unsigned char maxi = BossCollection[k].maxi;
        unsigned int direction = BossCollection[k].direction;
        unsigned int sens = BossCollection[k].sens;
        unsigned int type = BossCollection[k].color;



        SDL_Rect source, dest;



        if (type ==BIGBOSS)
        {
            lX -= 8;
            lY -= 14;

            dest.w = 32;
            dest.h = 32;
            source.w = 32;
            source.h = 32;

            unsigned int OFFSET_X_MONSTER = 0;

            if (direction==VERTICAL)
            {


                                  if (true)
                                 {
                                    lineRGBA( surf, lX+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, lX+1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, lX-1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX-1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                 }

                if (sens==BAS && BossCollection[k].ycur<maxi)
                {
                    //                            if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,64,32,32,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 64;
                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                    }
                    /*                            else if (frame_boss==1)   //dsubimage( lX, lY+3, &bigboss, OFFSET_X_MONSTER+32,64,32,32,  DIMAGE_NONE);
                                                {
                                                       dest.x = lX;
                                                       dest.y = lY+3;
                                                       source.x = OFFSET_X_MONSTER+32;
                                                       source.y = 64;
                                                       SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                }
                                                else if (frame_boss==2)   //dsubimage( lX, lY+5, &bigboss, OFFSET_X_MONSTER+64,64,32,32,  DIMAGE_NONE);
                                                {
                                                       dest.x = lX;
                                                       dest.y = lY+5;
                                                       source.x = OFFSET_X_MONSTER+64;
                                                       source.y = 64;
                                                       SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                }
                                                else if (frame_boss==3)   //dsubimage( lX, lY+8, &bigboss, OFFSET_X_MONSTER,64,32,32,  DIMAGE_NONE);
                                                {
                                                       dest.x = lX;
                                                       dest.y = lY+8;
                                                       source.x = OFFSET_X_MONSTER;
                                                       source.y = 64;
                                                       SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                }
                                                else if (frame_boss==4)   //dsubimage( lX, lY+11, &bigboss, OFFSET_X_MONSTER+32,64,32,32,  DIMAGE_NONE);
                                                {
                                                       dest.x = lX;
                                                       dest.y = lY+11;
                                                       source.x = OFFSET_X_MONSTER+32;
                                                       source.y = 64;
                                                       SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                }
                                                else if (frame_boss==5)   //dsubimage( lX, lY+13, &bigboss, OFFSET_X_MONSTER+64,64,32,32,  DIMAGE_NONE);
                                                {
                                                       dest.x = lX;
                                                       dest.y = lY+13;
                                                       source.x = OFFSET_X_MONSTER+64;
                                                       source.y = 64;
                                                       SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                }
                    */
                }
                else if (sens==BAS && BossCollection[k].ycur==maxi)
                {

                    //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,64,32,32,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 64;
                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                }
                else if (sens==HAUT && BossCollection[k].ycur>mini)
                {
                    //                             if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,0,32,32,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 0;
                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                    }
                    /*                              else if (frame_boss==1)   //dsubimage( lX, lY-3, &bigboss, OFFSET_X_MONSTER+32,0,32,32,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX;
                                                         dest.y = lY-3;
                                                         source.x = OFFSET_X_MONSTER+32;
                                                         source.y = 0;
                                                         SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==2)   //dsubimage( lX, lY-5, &bigboss, OFFSET_X_MONSTER+64,0,32,32,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX;
                                                         dest.y = lY-5;
                                                         source.x = OFFSET_X_MONSTER+64;
                                                         source.y = 0;
                                                         SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==3)   //dsubimage( lX, lY-8, &bigboss, OFFSET_X_MONSTER,32,0,32,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX;
                                                         dest.y = lY-8;
                                                         source.x = OFFSET_X_MONSTER;
                                                         source.y = 0;
                                                         SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==4)   //dsubimage( lX, lY-11, &bigboss, OFFSET_X_MONSTER+32,0,32,32,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX;
                                                         dest.y = lY-11;
                                                         source.x = OFFSET_X_MONSTER+32;
                                                         source.y = 0;
                                                         SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==5)   //dsubimage( lX, lY-13, &bigboss, OFFSET_X_MONSTER+64,0,32,32,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX;
                                                         dest.y = lY-13;
                                                         source.x = OFFSET_X_MONSTER+64;
                                                         source.y = 0;
                                                         SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                  }
                      */
                }
                else if (sens==HAUT && BossCollection[k].ycur==mini)
                {

                    //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,0,32,32,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 0;
                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                }
            }

            else if (direction==HORIZONTAL)
            {


                                 if (true)
                                 {
                                    lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, 255, 0, 0, 255 );
                                 }


                if (sens==GAUCHE && BossCollection[k].xcur>mini)
                {
                    //                            if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,96,32,32,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 96;
                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                    }
                    /*                             else if (frame_boss==1)   //dsubimage( lX-3, lY, &bigboss, OFFSET_X_MONSTER+32,96,32,32,  DIMAGE_NONE);
                                                 {
                                                        dest.x = lX-3;
                                                        dest.y = lY;
                                                        source.x = OFFSET_X_MONSTER+32;
                                                        source.y = 96;
                                                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                 }
                                                 else if (frame_boss==2)   //dsubimage( lX-5, lY, &bigboss, OFFSET_X_MONSTER+64,96,32,32,  DIMAGE_NONE);
                                                 {
                                                        dest.x = lX-5;
                                                        dest.y = lY;
                                                        source.x = OFFSET_X_MONSTER+64;
                                                        source.y = 96;
                                                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                 }
                                                 else if (frame_boss==3)   //dsubimage( lX-8, lY, &bigboss, OFFSET_X_MONSTER,96,32,32,  DIMAGE_NONE);
                                                 {
                                                        dest.x = lX-8;
                                                        dest.y = lY;
                                                        source.x = OFFSET_X_MONSTER;
                                                        source.y = 96;
                                                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                 }
                                                 else if (frame_boss==4)   //dsubimage( lX-11, lY, &bigboss, OFFSET_X_MONSTER+32,96,32,32,  DIMAGE_NONE);
                                                 {
                                                        dest.x = lX-11;
                                                        dest.y = lY;
                                                        source.x = OFFSET_X_MONSTER+32;
                                                        source.y = 96;
                                                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                 }
                                                 else if (frame_boss==5)   //dsubimage( lX-13, lY, &bigboss, OFFSET_X_MONSTER+64,96,32,32,  DIMAGE_NONE);
                                                 {
                                                        dest.x = lX-13;
                                                        dest.y = lY;
                                                        source.x = OFFSET_X_MONSTER+64;
                                                        source.y = 96;
                                                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                                                 }
                    */
                }
                else if (sens==GAUCHE && BossCollection[k].xcur==mini)
                {

                    //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,96,32,32,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 96;
                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                }
                else if (sens==DROITE && BossCollection[k].xcur<maxi)
                {
                    //                         if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,32,32,32,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 32;
                        SDL_BlitSurface( bigboss, &source, surf, &dest );
                    }
                    /*                         else if (frame_boss==1)   //dsubimage( lX+3, lY, &bigboss, OFFSET_X_MONSTER+32,32,32,32,  DIMAGE_NONE);
                                             {
                                                    dest.x = lX+3;
                                                    dest.y = lY;
                                                    source.x = OFFSET_X_MONSTER+32;
                                                    source.y = 32;
                                                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                                             }
                                             else if (frame_boss==2)   //dsubimage( lX+5, lY, &bigboss, OFFSET_X_MONSTER+64,32,32,32,  DIMAGE_NONE);
                                             {
                                                    dest.x = lX+5;
                                                    dest.y = lY;
                                                    source.x = OFFSET_X_MONSTER+64;
                                                    source.y = 32;
                                                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                                             }
                                             else if (frame_boss==3)   //dsubimage( lX+8, lY, &bigboss, OFFSET_X_MONSTER,32,32,32,  DIMAGE_NONE);
                                             {
                                                    dest.x = lX+8;
                                                    dest.y = lY;
                                                    source.x = OFFSET_X_MONSTER;
                                                    source.y = 32;
                                                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                                             }
                                             else if (frame_boss==4)   //dsubimage( lX+11, lY, &bigboss, OFFSET_X_MONSTER+32,32,32,32,  DIMAGE_NONE);
                                             {
                                                    dest.x = lX+11;
                                                    dest.y = lY;
                                                    source.x = OFFSET_X_MONSTER+32;
                                                    source.y = 32;
                                                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                                             }
                                             else if (frame_boss==5)   //dsubimage( lX+13, lY, &bigboss, OFFSET_X_MONSTER+64,32,32,32,  DIMAGE_NONE);
                                             {
                                                    dest.x = lX+13;
                                                    dest.y = lY;
                                                    source.x = OFFSET_X_MONSTER+64;
                                                    source.y = 32;
                                                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                                             }
                    */
                }
                else if (sens==DROITE && BossCollection[k].xcur==maxi)
                {
                    //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,32,32,32,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 32;
                    SDL_BlitSurface( bigboss, &source, surf, &dest );
                }
            }

        }
        else
        {

            dest.w = 16;
            dest.h = 18;
            source.w = 16;
            source.h = 18;

            unsigned int OFFSET_X_MONSTER = 48*type;


            if (direction==VERTICAL)
            {


                                  if (true)
                                 {
                                    lineRGBA( surf, lX+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, lX+1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, lX-1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX-1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                 }

                if (sens==BAS && BossCollection[k].ycur<maxi)
                {
                    //                           if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,36,16,18,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 36;
                        SDL_BlitSurface( bossmages, &source, surf, &dest );
                    }
                    /*                          else if (frame_boss==1)   //dsubimage( lX, lY+3, &bossmages, OFFSET_X_MONSTER+16,36,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY+3;
                                                     source.x = OFFSET_X_MONSTER+16;
                                                     source.y = 36;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==2)   //dsubimage( lX, lY+5, &bossmages, OFFSET_X_MONSTER+32,36,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY+5;
                                                     source.x = OFFSET_X_MONSTER+32;
                                                     source.y = 36;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==3)   //dsubimage( lX, lY+8, &bossmages, OFFSET_X_MONSTER,36,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY+8;
                                                     source.x = OFFSET_X_MONSTER;
                                                     source.y = 36;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==4)   //dsubimage( lX, lY+11, &bossmages, OFFSET_X_MONSTER+16,36,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY+11;
                                                     source.x = OFFSET_X_MONSTER+16;
                                                     source.y = 36;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==5)   //dsubimage( lX, lY+13, &bossmages, OFFSET_X_MONSTER+32,36,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY+13;
                                                     source.x = OFFSET_X_MONSTER+32;
                                                     source.y = 36;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                    */
                }
                else if (sens==BAS && BossCollection[k].ycur==maxi)
                {

                    //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,36,16,18,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 36;
                    SDL_BlitSurface( bossmages, &source, surf, &dest );
                }
                else if (sens==HAUT && BossCollection[k].ycur>mini)
                {
                    //                          if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,0,16,18,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 0;
                        SDL_BlitSurface( bossmages, &source, surf, &dest );
                    }
                    /*                          else if (frame_boss==1)   //dsubimage( lX, lY-3, &bossmages, OFFSET_X_MONSTER+16,0,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY-3;
                                                     source.x = OFFSET_X_MONSTER+16;
                                                     source.y = 0;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==2)   //dsubimage( lX, lY-5, &bossmages, OFFSET_X_MONSTER+32,0,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY-5;
                                                     source.x = OFFSET_X_MONSTER+32;
                                                     source.y = 0;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==3)   //dsubimage( lX, lY-8, &bossmages, OFFSET_X_MONSTER,0,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY-8;
                                                     source.x = OFFSET_X_MONSTER;
                                                     source.y = 0;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==4)   //dsubimage( lX, lY-11, &bossmages, OFFSET_X_MONSTER+16,0,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY-11;
                                                     source.x = OFFSET_X_MONSTER+16;
                                                     source.y = 0;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                                              else if (frame_boss==5)   //dsubimage( lX, lY-13, &bossmages, OFFSET_X_MONSTER+32,0,16,18,  DIMAGE_NONE);
                                              {
                                                     dest.x = lX;
                                                     dest.y = lY-13;
                                                     source.x = OFFSET_X_MONSTER+32;
                                                     source.y = 0;
                                                     SDL_BlitSurface( bossmages, &source, surf, &dest );
                                              }
                    */
                }
                else if (sens==HAUT && BossCollection[k].ycur==mini)
                {
                    //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,0,16,18,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 0;
                    SDL_BlitSurface( bossmages, &source, surf, &dest );
                }
            }

            else if (direction==HORIZONTAL)
            {


                                 if (true)
                                 {
                                    lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, 255, 0, 0, 255 );
                                    lineRGBA( surf, mini*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, 255, 0, 0, 255 );
                                 }


                if (sens==GAUCHE && BossCollection[k].xcur>mini)
                {
                    //                            if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,54,16,18,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 54;
                        SDL_BlitSurface( bossmages, &source, surf, &dest );
                    }
                    /*                              else if (frame_boss==1)   //dsubimage( lX-3, lY, &bossmages, OFFSET_X_MONSTER+16,54,16,18,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX-3;
                                                         dest.y = lY;
                                                         source.x = OFFSET_X_MONSTER+16;
                                                         source.y = 54;
                                                         SDL_BlitSurface( bossmages, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==2)   //dsubimage( lX-5, lY, &bossmages, OFFSET_X_MONSTER+32,54,16,18,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX-5;
                                                         dest.y = lY;
                                                         source.x = OFFSET_X_MONSTER+32;
                                                         source.y = 54;
                                                         SDL_BlitSurface( bossmages, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==3)   //dsubimage( lX-8, lY, &bossmages, OFFSET_X_MONSTER,54,16,18,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX-8;
                                                         dest.y = lY;
                                                         source.x = OFFSET_X_MONSTER;
                                                         source.y = 54;
                                                         SDL_BlitSurface( bossmages, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==4)   //dsubimage( lX-11, lY, &bossmages, OFFSET_X_MONSTER+16,54,16,18,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX-11;
                                                         dest.y = lY;
                                                         source.x = OFFSET_X_MONSTER+16;
                                                         source.y = 54;
                                                         SDL_BlitSurface( bossmages, &source, surf, &dest );
                                                  }
                                                  else if (frame_boss==5)   //dsubimage( lX-13, lY, &bossmages, OFFSET_X_MONSTER+32,54,16,18,  DIMAGE_NONE);
                                                  {
                                                         dest.x = lX-13;
                                                         dest.y = lY;
                                                         source.x = OFFSET_X_MONSTER+32;
                                                         source.y = 54;
                                                         SDL_BlitSurface( bossmages, &source, surf, &dest );
                                                  }
                       */
                }
                else if (sens==GAUCHE && BossCollection[k].xcur==mini)
                {
                    //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,54,16,18,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 54;
                    SDL_BlitSurface( bossmages, &source, surf, &dest );
                }
                else if (sens==DROITE && BossCollection[k].xcur<maxi)
                {
                    //                             if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,18,16,18,  DIMAGE_NONE);
                    {
                        dest.x = lX;
                        dest.y = lY;
                        source.x = OFFSET_X_MONSTER;
                        source.y = 18;
                        SDL_BlitSurface( bossmages, &source, surf, &dest );
                    }
                    /*                           else if (frame_boss==1)   //dsubimage( lX+3, lY, &bossmages, OFFSET_X_MONSTER+16,18,16,18,  DIMAGE_NONE);
                                               {
                                                      dest.x = lX+3;
                                                      dest.y = lY;
                                                      source.x = OFFSET_X_MONSTER+16;
                                                      source.y = 18;
                                                      SDL_BlitSurface( bossmages, &source, surf, &dest );
                                               }
                                               else if (frame_boss==2)   //dsubimage( lX+5, lY, &bossmages, OFFSET_X_MONSTER+32,18,16,18,  DIMAGE_NONE);
                                               {
                                                      dest.x = lX+5;
                                                      dest.y = lY;
                                                      source.x = OFFSET_X_MONSTER+32;
                                                      source.y = 18;
                                                      SDL_BlitSurface( bossmages, &source, surf, &dest );
                                               }
                                               else if (frame_boss==3)   //dsubimage( lX+8, lY, &bossmages, OFFSET_X_MONSTER,18,16,18,  DIMAGE_NONE);
                                               {
                                                      dest.x = lX+8;
                                                      dest.y = lY;
                                                      source.x = OFFSET_X_MONSTER;
                                                      source.y = 18;
                                                      SDL_BlitSurface( bossmages, &source, surf, &dest );
                                               }
                                               else if (frame_boss==4)   //dsubimage( lX+11, lY, &bossmages, OFFSET_X_MONSTER+16,18,16,18,  DIMAGE_NONE);
                                               {
                                                      dest.x = lX+11;
                                                      dest.y = lY;
                                                      source.x = OFFSET_X_MONSTER+16;
                                                      source.y = 18;
                                                      SDL_BlitSurface( bossmages, &source, surf, &dest );
                                               }
                                               else if (frame_boss==5)   //dsubimage( lX+13, lY, &bossmages, OFFSET_X_MONSTER+32,18,16,18,  DIMAGE_NONE);
                                               {
                                                      dest.x = lX+13;
                                                      dest.y = lY;
                                                      source.x = OFFSET_X_MONSTER+32;
                                                      source.y = 18;
                                                      SDL_BlitSurface( bossmages, &source, surf, &dest );
                                               }
                    */
                }
                else if (sens==DROITE && BossCollection[k].xcur==maxi)
                {
                    //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,18,16,18,  DIMAGE_NONE);
                    dest.x = lX;
                    dest.y = lY;
                    source.x = OFFSET_X_MONSTER;
                    source.y = 18;
                    SDL_BlitSurface( bossmages, &source, surf, &dest );
                }
            }
        }
    }

//      frame_boss++;
//      if (frame_boss==6)
//      {
    updateBoss();
//             frame_boss=0;
//      }
}



void renderObstacles( SDL_Surface *surf, int passnumber )
{
    if (passnumber == LAYER_BELOW_RAYS)
    {

        for( unsigned char k=0; k<NbObstacleLevel; k++ )
        {
            unsigned int Xb = ObstaclesCollection[k].x;
            unsigned int Yb = ObstaclesCollection[k].y;
            unsigned char Tb = ObstaclesCollection[k].type;

            unsigned int lX = SIZE*Xb+OFFSET_X;
            unsigned int lY = SIZE*Yb+OFFSET_Y;

            SDL_Rect source, dest;

            dest.x = lX;
            dest.y = lY;
            dest.w = SIZE * ZOOMFACTOR;
            dest.h = SIZE * ZOOMFACTOR;

            if (Tb==BLOCK_WATER)
            {
                // dsubimage( lX, lY, &sprites, 64,112,16,16,  DIMAGE_NONE);
                source.x = 64;
                source.y = 112;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
        }
    }
    else if (passnumber == LAYER_ABOVE_RAYS)
    {

        for( unsigned char k=0; k<NbObstacleLevel; k++ )
        {
            unsigned int Xb = ObstaclesCollection[k].x;
            unsigned int Yb = ObstaclesCollection[k].y;
            unsigned char Tb = ObstaclesCollection[k].type;

            unsigned int lX = SIZE*Xb+OFFSET_X;
            unsigned int lY = SIZE*Yb+OFFSET_Y;

            SDL_Rect source, dest;

            dest.x = lX;
            dest.y = lY;
            dest.w = SIZE * ZOOMFACTOR;
            dest.h = SIZE * ZOOMFACTOR;

            if (Tb==BLOCK_STONE)
            {
                //dsubimage( lX, lY, &sprites, 0,112,16,16,  DIMAGE_NONE);
                source.x = 0;
                source.y = 112;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (Tb==BLOCK_MOON)
            {
                //dsubimage( lX, lY, &sprites, 16,112,16,16,  DIMAGE_NONE);
                source.x = 16;
                source.y = 112;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (Tb==BLOCK_WOOD)
            {
                //dsubimage( lX, lY, &sprites, 32,112,16,16,  DIMAGE_NONE);
                source.x = 32;
                source.y = 112;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (Tb==BLOCK_SUN)
            {
                //dsubimage( lX, lY, &sprites, 48,112,16,16,  DIMAGE_NONE);
                source.x = 48;
                source.y = 112;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( sprites, &source, surf, &dest );
            }

        }
    }
}

void renderTreasures( SDL_Surface* surf )
{
    for( unsigned char k=0; k<NbTreasureLevel; k++ )
    {
        unsigned int Xb = TreasureCollection[k].x;
        unsigned int Yb = TreasureCollection[k].y;
        chest Tb = TreasureCollection[k].type;
        bool Ob = TreasureCollection[k].isopen;

        unsigned int lX = SIZE*Xb+OFFSET_X;
        unsigned int lY = SIZE*Yb+OFFSET_Y;

        SDL_Rect source, dest;

        dest.x = lX;
        dest.y = lY;
        dest.w = SIZE * ZOOMFACTOR;
        dest.h = SIZE * ZOOMFACTOR;

        if (Tb==T_RED)
        {
            if (Ob==false)
            {
                //dsubimage( lX, lY, &chests, 0,0,16,16,  DIMAGE_NONE);
                source.x = 0;
                source.y = 0;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );
            }
            else
            {
                //dsubimage( lX, lY, &chests, 0,48,16,16,  DIMAGE_NONE);
                source.x = 0;
                source.y = 48;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
        }
        else if (Tb==T_YELLOW)
        {
            if (Ob==false)
            {
                //dsubimage( lX, lY, &chests, 16,0,16,16,  DIMAGE_NONE);
                source.x = 16;
                source.y = 0;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
            else
            {
                //dsubimage( lX, lY, &chests, 16,48,16,16,  DIMAGE_NONE);
                source.x = 16;
                source.y = 48;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
        }
        else if (Tb==T_GREEN)
        {
            if (Ob==false)
            {
                //dsubimage( lX, lY, &chests, 32,0,16,16,  DIMAGE_NONE);
                source.x = 32;
                source.y = 0;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
            else
            {
                //dsubimage( lX, lY, &chests, 32,48,16,16,  DIMAGE_NONE);
                source.x = 32;
                source.y = 48;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
        }
        else if (Tb==T_BLUE)
        {
            if (Ob==false)
            {
                //dsubimage( lX, lY, &chests, 48,0,16,16,  DIMAGE_NONE);
                source.x = 48;
                source.y = 0;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
            else
            {
                //dsubimage( lX, lY, &chests, 48,48,16,16,  DIMAGE_NONE);
                source.x = 48;
                source.y = 48;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;
                SDL_BlitSurface( chests, &source, surf, &dest );

            }
        }
    }

}

void renderMovable( SDL_Surface *surf )
{
    for( unsigned char k=0; k<NbBlocksLevel; k++ )
    {
        unsigned int Xb = BlocksCollection[k].x;
        unsigned int Yb = BlocksCollection[k].y;
        unsigned char Rb = BlocksCollection[k].R;
        unsigned char Gb = BlocksCollection[k].G;
        unsigned char Bb = BlocksCollection[k].B;
        //unsigned char Ab = BlocksCollection[k].A;

        unsigned int lX = SIZE*Xb+OFFSET_X;
        unsigned int lY = SIZE*Yb+OFFSET_Y;

        SDL_Rect source, dest;

        dest.x = lX;
        dest.y = lY;
        dest.w = SIZE * ZOOMFACTOR;
        dest.h = SIZE * ZOOMFACTOR;


        if (Rb==255 && Gb==0 && Bb==0)
        {
            //dsubimage( lX, lY, &sprites, 16,0,16,16,  DIMAGE_NONE);
            source.x = 16;
            source.y = 0;
            source.w = SIZE * ZOOMFACTOR;
            source.h = SIZE * ZOOMFACTOR;
            SDL_BlitSurface( sprites, &source, surf, &dest );
        }
        else if (Rb==0 && Gb==0 && Bb==255)
        {
            //dsubimage( lX, lY, &sprites, 32,0,16,16,  DIMAGE_NONE);
            source.x = 32;
            source.y = 0;
            source.w = SIZE * ZOOMFACTOR;
            source.h = SIZE * ZOOMFACTOR;
            SDL_BlitSurface( sprites, &source, surf, &dest );
        }
        else if (Rb==0 && Gb==255 && Bb==0)
        {
            //dsubimage( lX, lY, &sprites, 48,0,16,16,  DIMAGE_NONE);
            source.x = 48;
            source.y = 0;
            source.w = SIZE * ZOOMFACTOR;
            source.h = SIZE * ZOOMFACTOR;
            SDL_BlitSurface( sprites, &source, surf, &dest );
        }
        else if (Rb==0 && Gb==0 && Bb==0)
        {
            // dsubimage( lX, lY, &sprites, 64,0,16,16,  DIMAGE_NONE);
            source.x = 64;
            source.y = 0;
            source.w = SIZE * ZOOMFACTOR;
            source.h = SIZE * ZOOMFACTOR;
            SDL_BlitSurface( sprites, &source, surf, &dest );
        }
        else if (Rb==255 && Gb==255 && Bb==255)
        {
            //dsubimage( lX, lY, &sprites, 0,0,16,16,  DIMAGE_NONE);
            source.x = 0;
            source.y = 0;
            source.w = SIZE * ZOOMFACTOR;
            source.h = SIZE * ZOOMFACTOR;
            SDL_BlitSurface( sprites, &source, surf, &dest );
        }

    }

}

void renderPlayer( SDL_Surface *surf )
{
    unsigned int lX = SIZE*joueur.x*ZOOMFACTOR+OFFSET_X;
    unsigned int lY = SIZE*joueur.y*ZOOMFACTOR+OFFSET_Y;

    SDL_Rect source, dest;

    dest.x = lX;
    dest.y = lY;
    dest.w = SIZE * ZOOMFACTOR;
    dest.h = SIZE * ZOOMFACTOR;

    source.x = 0;
    source.y = 0;
    source.w = SIZE * ZOOMFACTOR;
    source.h = SIZE * ZOOMFACTOR;

    SDL_BlitSurface( player, &source, surf, &dest );
}

void renderLight( SDL_Surface *surf )
{
    unsigned int lX = SIZE*lumiere.x+OFFSET_X;
    unsigned int lY = SIZE*lumiere.y+OFFSET_Y;

    SDL_Rect source, dest;

    dest.x = lX;
    dest.y = lY;
    dest.w = SIZE * ZOOMFACTOR;
    dest.h = SIZE * ZOOMFACTOR;

    source.x = 0;
    source.y = 0;
    source.w = SIZE * ZOOMFACTOR;
    source.h = SIZE * ZOOMFACTOR;

    SDL_BlitSurface( light, &source, surf, &dest );
}

bool isValidMove( selection selected, unsigned char x, unsigned char y, orientations sens )
{
    if (selected == JOUEUR)
        for(unsigned char k=0; k<NbDoorsLevel; k++)
        {
            if ( x==DoorCollection[k].x && y==DoorCollection[k].y)
            {
                if (DoorCollection[k].isdoor && DoorCollection[k].isopen)
                {
                    loadLevel(  DoorCollection[k].nextLevelNumber );
                    joueur = { 11-10*x/12, 9-8*y/10, sens };
                    return false;
                }
            }
        }

    if (x<1 || x>SIZE_MAP_X-2 || y<1 || y>SIZE_MAP_Y-2)
    {
        return false;
    }
    else if (selected == JOUEUR)
    {
        if (x==lumiere.x && y==lumiere.y) return false;
        else if (x==BlocRouge->x && y==BlocRouge->y) return false;
        else if (x==BlocVert->x && y==BlocVert->y) return false;
        else if (x==BlocBleu->x && y==BlocBleu->y) return false;
        else if (x==BlocNoir->x && y==BlocNoir->y) return false;
        else
        {
            for (unsigned char k =0; k<NbObstacleLevel; k++)
                if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                    return false;
            return true;
        }
    }
    else if (selected == LUMIERE)
    {
        if (x==joueur.x && y==joueur.y) return false;
        else
        {
            for (unsigned char k =0; k<NbObstacleLevel; k++)
                if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                    return false;
            return true;
        }
    }
    else if (selected == ROUGE)
    {
        if (x==joueur.x && y==joueur.y) return false;
        else if (x==BlocVert->x && y==BlocVert->y) return false;
        else if (x==BlocBleu->x && y==BlocBleu->y) return false;
        else if (x==BlocNoir->x && y==BlocNoir->y) return false;
        else
        {
            for (unsigned char k =0; k<NbObstacleLevel; k++)
                if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                    return false;
            return true;
        }
    }
    else if (selected == VERT)
    {
        if (x==joueur.x && y==joueur.y) return false;
        else if (x==BlocRouge->x && y==BlocRouge->y) return false;
        else if (x==BlocBleu->x && y==BlocBleu->y) return false;
        else if (x==BlocNoir->x && y==BlocNoir->y) return false;
        else
        {
            for (unsigned char k =0; k<NbObstacleLevel; k++)
                if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                    return false;
            return true;
        }
    }
    else if (selected == BLEU)
    {
        if (x==joueur.x && y==joueur.y) return false;
        else if (x==BlocRouge->x && y==BlocRouge->y) return false;
        else if (x==BlocVert->x && y==BlocVert->y) return false;
        else if (x==BlocNoir->x && y==BlocNoir->y) return false;
        else
        {
            for (unsigned char k =0; k<NbObstacleLevel; k++)
                if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                    return false;
            return true;
        }
    }
    else if (selected == NOIR)
    {
        if (x==joueur.x && y==joueur.y) return false;
        else if (x==BlocRouge->x && y==BlocRouge->y) return false;
        else if (x==BlocVert->x && y==BlocVert->y) return false;
        else if (x==BlocBleu->x && y==BlocBleu->y) return false;
        else
        {
            for (unsigned char k =0; k<NbObstacleLevel; k++)
                if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                    return false;
            return true;
        }
    }
    return false;
}

void gameMechanics(  selection what, orientations touche )
{
    if (touche==GAUCHE)
    {
        if (what==JOUEUR && isValidMove(JOUEUR, joueur.x-1, joueur.y, touche ))
        {
            joueur.x--;
        }
        else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x-1, joueur.y, touche))
        {

        }
        else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x-1, lumiere.y, touche))     lumiere.x--;
        else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x-1, BlocRouge->y, touche ))     BlocRouge->x--;
        else if (what==VERT && isValidMove(VERT, BlocVert->x-1, BlocVert->y, touche))     BlocVert->x--;
        else if (what==BLEU && isValidMove(BLEU, BlocBleu->x-1, BlocBleu->y, touche))     BlocBleu->x--;
        else if (what==NOIR && isValidMove(NOIR, BlocNoir->x-1, BlocNoir->y, touche))     BlocNoir->x--;
    }
    if (touche==DROITE)
    {
        if (what==JOUEUR && isValidMove(JOUEUR, joueur.x+1, joueur.y, touche))
        {
            joueur.x++;
        }
        else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x+1, joueur.y, touche))
        {

        }
        else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x+1, lumiere.y, touche))     lumiere.x++;
        else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x+1, BlocRouge->y, touche))     BlocRouge->x++;
        else if (what==VERT && isValidMove(VERT, BlocVert->x+1, BlocVert->y, touche))     BlocVert->x++;
        else if (what==BLEU && isValidMove(BLEU, BlocBleu->x+1, BlocBleu->y, touche))     BlocBleu->x++;
        else if (what==NOIR && isValidMove(NOIR, BlocNoir->x+1, BlocNoir->y, touche))     BlocNoir->x++;
    }
    if (touche==HAUT)
    {
        if (what==JOUEUR && isValidMove(JOUEUR, joueur.x, joueur.y-1, touche))
        {
            joueur.y--;
        }
        else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x, joueur.y-1, touche))
        {

        }
        else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x, lumiere.y-1, touche))     lumiere.y--;
        else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x, BlocRouge->y-1, touche))     BlocRouge->y--;
        else if (what==VERT && isValidMove(VERT, BlocVert->x, BlocVert->y-1, touche))     BlocVert->y--;
        else if (what==BLEU && isValidMove(BLEU, BlocBleu->x, BlocBleu->y-1, touche))     BlocBleu->y--;
        else if (what==NOIR && isValidMove(NOIR, BlocNoir->x, BlocNoir->y-1, touche))     BlocNoir->y--;
    }
    if (touche==BAS)
    {
        if (what==JOUEUR && isValidMove(JOUEUR, joueur.x, joueur.y+1, touche))
        {
            joueur.y++;
        }
        else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x, joueur.y+1, touche))
        {

        }
        else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x, lumiere.y+1, touche))     lumiere.y++;
        else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x, BlocRouge->y+1, touche))     BlocRouge->y++;
        else if (what==VERT && isValidMove(VERT, BlocVert->x, BlocVert->y+1, touche))     BlocVert->y++;
        else if (what==BLEU && isValidMove(BLEU, BlocBleu->x, BlocBleu->y+1, touche))     BlocBleu->y++;
        else if (what==NOIR && isValidMove(NOIR, BlocNoir->x, BlocNoir->y+1, touche))     BlocNoir->y++;
    }
}

void renderReceptors( SDL_Surface* surf )
{
    for( unsigned char k=0; k<NbReceptorsLevel; k++ )
    {
        unsigned int lX = SIZE*ReceptorCollection[k].x+OFFSET_X+SIZE/2;
        unsigned int lY = SIZE*ReceptorCollection[k].y+OFFSET_Y+SIZE/2;
        unsigned char Rr = ReceptorCollection[k].R;
        unsigned char Gr = ReceptorCollection[k].G;
        unsigned char Br = ReceptorCollection[k].B;
        unsigned int direction = ReceptorCollection[k].direction;
        bool valide = ReceptorCollection[k].isvalide;

        unsigned char Rc,Gc,Bc;

        if (valide==false)
        {
            Rc=0;
            Gc=0;
            Bc=0;
        }
        else
        {
            Rc=255;
            Gc=255;
            Bc=255;
        }

        boxRGBA( surf, lX-4, lY-4, lX+4, lY+4, Rc, Gc, Bc, 255 );
        boxRGBA( surf,  lX-2, lY-2, lX+2, lY+2, Rr, Gr, Br, 255 );


        if (direction==GAUCHE)
        {
            lineRGBA( surf,  lX-4, lY-1, lX-8, lY-1, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX-4, lY, lX-8, lY, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX-4, lY+1, lX-8, lY+1, Rc, Gc, Bc, 255 );
        }
        else if (direction==DROITE)
        {
            lineRGBA( surf, lX+4, lY-1, lX+8, lY-1, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX+4, lY, lX+8, lY, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX+4, lY+1, lX+8, lY+1, Rc, Gc, Bc, 255 );
        }
        else if (direction==HAUT)
        {
            lineRGBA( surf, lX-1, lY-4, lX-1, lY-8, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX, lY-4, lX, lY-8, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX+1, lY-4, lX+1, lY-8, Rc, Gc, Bc, 255 );
        }
        else if (direction==BAS)
        {
            lineRGBA( surf, lX-1, lY+4, lX-1, lY+8, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX, lY+4, lX, lY+8, Rc, Gc, Bc, 255 );
            lineRGBA( surf, lX+1, lY+4, lX+1, lY+8, Rc, Gc, Bc, 255 );
        }

    }
}

uint32_t _getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;

    case 2:
        return *(uint16_t *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(uint32_t *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void getPixelRGB( SDL_Surface* surf, unsigned int x, unsigned int y, unsigned char *R1, unsigned char* G1, unsigned char *B1 )
{

    Uint8 r,g,b;
    SDL_LockSurface( surf );
    SDL_GetRGB(_getpixel( surf, x, y), surf->format, &r, &g, &b);
    *R1=r;
    *G1=g;
    *B1=b;
    SDL_UnlockSurface( surf );
}


void checkReceptors( SDL_Surface* surf  )
{
    for( unsigned char k=0; k<NbReceptorsLevel; k++ )
    {
        unsigned int lX1=0, lX2=0, lX3=0;
        unsigned int lY1=0, lY2=0, lY3=0;

        unsigned int lX = SIZE*ReceptorCollection[k].x+OFFSET_X+SIZE/2;
        unsigned int lY = SIZE*ReceptorCollection[k].y+OFFSET_Y+SIZE/2;
        unsigned char Rr = ReceptorCollection[k].R;
        unsigned char Gr = ReceptorCollection[k].G;
        unsigned char Br = ReceptorCollection[k].B;
        unsigned int direction = ReceptorCollection[k].direction;

        if (direction==GAUCHE)
        {
            // all 7 changed to 8
            lX1=lX-7;
            lX2=lX-7;
            lX3=lX-7;
            lY1=lY-1;
            lY2=lY;
            lY3=lY+1;
        }
        else if (direction==DROITE)
        {
            lX1=lX+7;
            lX2=lX+7;
            lX3=lX+7;
            lY1=lY-1;
            lY2=lY;
            lY3=lY+1;
        }
        else if (direction==HAUT)
        {
            lX1=lX-1;
            lX2=lX;
            lX3=lX+1;
            lY1=lY-7;
            lY2=lY-7;
            lY3=lY-7;
        }
        else if (direction==BAS)
        {
            lX1=lX-1;
            lX2=lX;
            lX3=lX+1;
            lY1=lY+7;
            lY2=lY+7;
            lY3=lY+7;
        }

        unsigned char R1, G1, B1;
        unsigned char R2, G2, B2;
        unsigned char R3, G3, B3;
        unsigned char Rm, Gm, Bm;

        getPixelRGB( surf, lX1, lY1, &R1, &G1, &B1 );
        getPixelRGB( surf, lX2, lY2, &R2, &G2, &B2 );
        getPixelRGB( surf, lX3, lY3, &R3, &G3, &B3 );

        Rm = (R1+R2+R3)/3;
        Gm = (G1+G2+G3)/3;
        Bm = (B1+B2+B3)/3;

//              _boxRGBA( 310, 10+k*30, 330, 30+k*30, Rm, Gm, Bm, 255 );
//              _rectangleRGBA( 310, 10+k*30, 330, 30+k*30, 0, 0, 0, 255 );

//              _boxRGBA( 340, 10+k*30, 360, 30+k*30, Rr, Gr, Br, 255 );
//              _rectangleRGBA( 340, 10+k*30, 360, 30+k*30, 0, 0, 0, 255 );


        if (Rm>=Rr-EPSILON && Rm<=Rr+EPSILON && Gm>=Gr-EPSILON && Gm<=Gr+EPSILON && Bm>=Br-EPSILON && Bm<=Br+EPSILON)
        {
            ReceptorCollection[k].isvalide = true;
        }
        else
        {
            ReceptorCollection[k].isvalide = false;
        }
    }
}

void renderDoors( SDL_Surface* surf )
{
    for( unsigned char k=0; k<NbDoorsLevel; k++ )
    {
        unsigned int lX = SIZE*DoorCollection[k].x+OFFSET_X;
        unsigned int lY = SIZE*DoorCollection[k].y+OFFSET_Y;

        SDL_Rect source, dest;

        dest.x = lX;
        dest.y = lY;
        dest.w = SIZE * ZOOMFACTOR;
        dest.h = SIZE * ZOOMFACTOR;



        int OFFSET_Y_TILE = 16*LEVELCOLOR;

        if (DoorCollection[k].isdoor==false) // check if it is a door or a wall (draw walls)
        {
            if (DoorCollection[k].direction == HAUT)
            {
                //dsubimage( lX, lY, &sprites, 0,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                source.x = 0;
                source.y = OFFSET_Y_TILE+16;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == GAUCHE)
            {
                //dsubimage( lX, lY, &sprites, 16,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                source.x = 16;
                source.y = OFFSET_Y_TILE+16;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == BAS)
            {
                //dsubimage( lX, lY, &sprites, 32,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                source.x = 32;
                source.y = OFFSET_Y_TILE+16;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == DROITE)
            {
                //dsubimage( lX, lY, &sprites, 48,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                source.x = 48;
                source.y = OFFSET_Y_TILE+16;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
        }
        else if (DoorCollection[k].isopen==false) // check if the door is open (draw stairs)
        {
            if (DoorCollection[k].direction == HAUT)
            {
                //dsubimage( lX, lY, &sprites, 0,80,16,16,  DIMAGE_NONE);
                source.x = 0;
                source.y = 80;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == GAUCHE)
            {
                //dsubimage( lX, lY, &sprites, 16,80,16,16,  DIMAGE_NONE);
                source.x = 16;
                source.y = 80;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == BAS)
            {
                //dsubimage( lX, lY, &sprites, 32,80,16,16,  DIMAGE_NONE);
                source.x = 32;
                source.y = 80;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == DROITE)
            {
                //dsubimage( lX, lY, &sprites, 48,80,16,16,  DIMAGE_NONE);
                source.x = 48;
                source.y = 80;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
        }
        else  // check if the door is open (draw doors)
        {
            if (DoorCollection[k].direction == HAUT)
            {
                //dsubimage( lX, lY, &sprites, 0,96,16,16,  DIMAGE_NONE);
                source.x = 0;
                source.y = 96;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == GAUCHE)
            {
                //dsubimage( lX, lY, &sprites, 16,96,16,16,  DIMAGE_NONE);
                source.x = 16;
                source.y = 96;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == BAS)
            {
                //dsubimage( lX, lY, &sprites, 32,96,16,16,  DIMAGE_NONE);
                source.x = 32;
                source.y = 96;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
            else if (DoorCollection[k].direction == DROITE)
            {
                //dsubimage( lX, lY, &sprites, 48,96,16,16,  DIMAGE_NONE);
                source.x = 48;
                source.y = 96;
                source.w = SIZE * ZOOMFACTOR;
                source.h = SIZE * ZOOMFACTOR;

                SDL_BlitSurface( sprites, &source, surf, &dest );
            }
        }
    }

}


void updateTreasures( selection what )
{
       if (what==JOUEUR)
       {
              unsigned char X = joueur.x;
              unsigned char Y= joueur.y;

              for (unsigned char k=0; k<NbTreasureLevel; k++)
              {
                     if (X==TreasureCollection[k].x && Y==TreasureCollection[k].y)
                     {
                            if (TreasureCollection[k].isopen==false)
                            {
                                   //score+=TreasureCollection[k].scoreboost;
                                   TreasureCollection[k].isopen=true;

                                   if (TreasureCollection[k].inside == BLACK_FRAGMENT)  BlackFrag=true;
                                   if (TreasureCollection[k].inside == WHITE_FRAGMENT)  WhiteFrag=true;
                                   if (TreasureCollection[k].inside == RED_FRAGMENT)  RedFrag=true;
                                   if (TreasureCollection[k].inside == GREEN_FRAGMENT)  GreenFrag=true;
                                   if (TreasureCollection[k].inside == BLUE_FRAGMENT)  BlueFrag=true;

/*
                                   if (TreasureCollection[k].inside == HEART_SINGLE)
                                   {
                                          if (difficulty==0 && life<5) life++;
                                          else if (difficulty==1 && life<3) life++;
                                          else if (difficulty==2 && life<1) life++;    // this is just theorical as Life < 1 means that you loose !!
                                   }

                                   if (TreasureCollection[k].inside == HEART_FULL)
                                   {
                                          if (difficulty==0) life=5;
                                          else if (difficulty==1) life=3;
                                          else if (difficulty==2) life=1;    // this is just theorical as Life < 1 means that you loose !!
                                   }
*/
                            }
                            else
                            {
                                   TreasureCollection[k].isvisible=false;
                            }
                     }
              }
       }
}



void checkDoors( SDL_Surface* surf )
{
    for( unsigned char k=0; k<NbDoorsLevel; k++ )
    {
        if (DoorCollection[k].isdoor==true) // check if it is a door or a wall
        {
            bool temp = true;
            for(int j=0; j<DoorCollection[k].nbreceptor; j++ )
            {
                int receptorReference =  DoorCollection[k].receptors[j];
                bool currentrecept = ReceptorCollection[receptorReference].isvalide;
                Receptor* tempRecept = &ReceptorCollection[receptorReference];

                temp=temp && currentrecept;

                if (currentrecept)
                {
                    if(DoorCollection[k].direction==HAUT)
                    {
                        int X = DoorCollection[k].x*SIZE+OFFSET_X;
                        int Y = DoorCollection[k].y*SIZE-SIZE/2+OFFSET_Y;

                        if (DoorCollection[k].nbreceptor==1)
                        {
                            boxRGBA( surf, X+SIZE/2-1, Y-1, X+SIZE/2+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X+SIZE/2-2, Y-2, X+SIZE/2+2, Y+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==2)
                        {
                            boxRGBA( surf, X+SIZE/3*(j+1)-1, Y-1, X+SIZE/3*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X+SIZE/3*(j+1)-2, Y-2, X+SIZE/3*(j+1)+2, Y+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==3)
                        {
                            boxRGBA( surf, X+SIZE/4*(j+1)-1, Y-1, X+SIZE/4*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X+SIZE/4*(j+1)-2, Y-2, X+SIZE/4*(j+1)+2, Y+2, 150, 150, 150, 255);
                        }
                    }
                    else if(DoorCollection[k].direction==BAS)
                    {
                        int X = DoorCollection[k].x*SIZE+OFFSET_X;
                        int Y = DoorCollection[k].y*SIZE+3*SIZE/2+OFFSET_Y;

                        if (DoorCollection[k].nbreceptor==1)
                        {
                            boxRGBA( surf, X+SIZE/2-1, Y-1, X+SIZE/2+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X+SIZE/2-2, Y-2, X+SIZE/2+2, Y+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==2)
                        {
                            boxRGBA( surf, X+SIZE/3*(j+1)-1, Y-1, X+SIZE/3*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X+SIZE/3*(j+1)-2, Y-2, X+SIZE/3*(j+1)+2, Y+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==3)
                        {
                            boxRGBA( surf, X+SIZE/4*(j+1)-1, Y-1, X+SIZE/4*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X+SIZE/4*(j+1)-2, Y-2, X+SIZE/4*(j+1)+2, Y+2, 150, 150, 150, 255);
                        }
                    }
                    else if(DoorCollection[k].direction==GAUCHE)
                    {
                        int X = DoorCollection[k].x*SIZE-SIZE/2+OFFSET_X;
                        int Y = DoorCollection[k].y*SIZE+OFFSET_Y;

                        if (DoorCollection[k].nbreceptor==1)
                        {
                            boxRGBA( surf, X-1, Y+SIZE/2-1, X+1, Y+SIZE/2+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X-2, Y+SIZE/2-2, X+2, Y+SIZE/2+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==2)
                        {
                            boxRGBA( surf, X-1, Y+SIZE/3*(j+1)-1, X+1, Y+SIZE/3*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X-2, Y+SIZE/3*(j+1)-2, X+2, Y+SIZE/3*(j+1)+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==3)
                        {
                            boxRGBA( surf, X-1, Y+SIZE/4*(j+1)-1, X+1, Y+SIZE/4*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X-2, Y+SIZE/4*(j+1)-2, X+2, Y+SIZE/4*(j+1)+2, 150, 150, 150, 255);
                        }
                    }
                    else if(DoorCollection[k].direction==DROITE)
                    {
                        int X = DoorCollection[k].x*SIZE+3*SIZE/2+OFFSET_X;
                        int Y = DoorCollection[k].y*SIZE+OFFSET_Y;

                        if (DoorCollection[k].nbreceptor==1)
                        {
                            boxRGBA( surf, X-1, Y+SIZE/2-1, X+1, Y+SIZE/2+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X-2, Y+SIZE/2-2, X+2, Y+SIZE/2+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==2)
                        {
                            boxRGBA( surf, X-1, Y+SIZE/3*(j+1)-1, X+1, Y+SIZE/3*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X-2, Y+SIZE/3*(j+1)-2, X+2, Y+SIZE/3*(j+1)+2, 150, 150, 150, 255);
                        }
                        else if (DoorCollection[k].nbreceptor==3)
                        {
                            boxRGBA( surf, X-1, Y+SIZE/4*(j+1)-1, X+1, Y+SIZE/4*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                            rectangleRGBA( surf, X-2, Y+SIZE/4*(j+1)-2, X+2, Y+SIZE/4*(j+1)+2, 150, 150, 150, 255);
                        }
                    }
                }
            }
            DoorCollection[k].isopen=temp;
        }
    }
}


void exitAndFree( void )
{
    free(BlocksCollection);
    free(ReceptorCollection);
    free(DoorCollection);
    free(TreasureCollection);
    free(ObstaclesCollection);
    free(BossCollection);
    free(MonstersCollection);

    BlocksCollection=NULL;
    ReceptorCollection=NULL;
    DoorCollection=NULL;
    TreasureCollection=NULL;
    ObstaclesCollection=NULL;
    MonstersCollection=NULL;
    BossCollection=NULL;


    SDL_FreeSurface( leveltiles );
    SDL_FreeSurface( sprites );
    SDL_FreeSurface( light );
    SDL_FreeSurface( player );
    SDL_FreeSurface( chests );
    SDL_FreeSurface( monstres );
    SDL_FreeSurface(bigboss );
    SDL_FreeSurface( bossmages);
    SDL_FreeSurface( parchemin );
    SDL_FreeSurface( fragments );

    SDL_FreeSurface( screenCalcul );

    SDL_Quit();

}


void initWalls( void )
{
    murs[0] = { HORIZONTAL,     OFFSET_X+SIZE*1,    OFFSET_X+SIZE*(SIZE_MAP_X-1),   OFFSET_Y+SIZE };
    murs[1] = { VERTICAL,       OFFSET_Y+SIZE*1,    OFFSET_Y+SIZE*(SIZE_MAP_Y-1),   OFFSET_X+SIZE*(SIZE_MAP_X-1) };
    murs[2] = { HORIZONTAL,     OFFSET_X+SIZE*1,    OFFSET_X+SIZE*(SIZE_MAP_X-1),   OFFSET_Y+SIZE*(SIZE_MAP_Y-1) };
    murs[3] = { VERTICAL,       OFFSET_Y+SIZE*1,    OFFSET_Y+SIZE*(SIZE_MAP_Y-1),   OFFSET_X+SIZE };

//       for(unsigned char k=0; k<SIZE_MAP_X; k++)
//              dprint_opt( OFFSET_X+SIZE*k+SIZE/2, OFFSET_Y-SIZE/2, C_WHITE, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE, "%d", k );

//       for( unsigned char k=1; k<SIZE_MAP_Y; k++)
//              dprint_opt( OFFSET_X-SIZE/2, OFFSET_Y+k*SIZE+SIZE/2, C_WHITE, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE, "%d", k );
}

/* return the sign of the number given in parameter*/
/* -1 if x<0   0 if x=0 and +1 if x>0 */
int _sgn( int x )
{
    if(x>=0)
        return 1;
    else if(x<0)
        return -1;
    else
        return 0;
}

/* return the absolute value of the number given in parameter*/
int _abs( int x )
{
    if(x>=0)
        return x;
    else
        return -1*x;
}

int computeLightModification( unsigned int currentCellX, unsigned int currentCellY, unsigned char* R, unsigned char* G, unsigned char* B, unsigned char* A )
{
    if (*A!=ALPHA_BLOQUE)
    {
        for( unsigned char k=0; k<NbBlocksLevel; k++ )
        {
            if (BlocksCollection[k].x == currentCellX)
                if (BlocksCollection[k].y == currentCellY)
                {
                    if (*R!=BlocNoir->R || *G!=BlocNoir->G || *B!=BlocNoir->B || *A!=ALPHA_OPAQUE )      // We check if the light is not already a black ray
                    {
                        if (BlocksCollection[k].type == OPAQUE)
                        {
                            if (*R==lumiere.R && *G==lumiere.G && *B==lumiere.B && *A==lumiere.A)
                            {
                                *R = (*R-lumiere.R) & BlocksCollection[k].R;
                                *G = (*G-lumiere.G) & BlocksCollection[k].G;
                                *B = (*B-lumiere.B) & BlocksCollection[k].B;
                                *A = ALPHA_OPAQUE;
                            }
                            else
                            {
                                *R = *R & BlocksCollection[k].R;
                                *G = *G & BlocksCollection[k].G;
                                *B = *B & BlocksCollection[k].B;
                                *A = ALPHA_OPAQUE;
                            }
                        }
                        if (BlocksCollection[k].type == TRANSPARENT)
                        {
                            if (*R==lumiere.R && *G==lumiere.G && *B==lumiere.B && *A==lumiere.A)
                            {
                                *R = (*R-lumiere.R) | BlocksCollection[k].R;
                                *G = (*G-lumiere.G) | BlocksCollection[k].G;
                                *B = (*B-lumiere.B) | BlocksCollection[k].B;
                                *A = ALPHA_TRANSPARENT;
                            }
                            else
                            {
                                *R = *R | BlocksCollection[k].R;
                                *G = *G | BlocksCollection[k].G;
                                *B = *B | BlocksCollection[k].B;
                                *A = ALPHA_TRANSPARENT;
                            }
                        }
                    }
                }
        }

        for( unsigned char k=0; k<NbObstacleLevel; k++ )
        {
            if (ObstaclesCollection[k].type!=BLOCK_WATER)
                if (ObstaclesCollection[k].x == currentCellX)
                    if (ObstaclesCollection[k].y == currentCellY)
                    {
                        *A = ALPHA_BLOQUE;
                    }
        }

        if (*A == ALPHA_BLOQUE) return 1;
    }
    return 0;
}


void rayCast( SDL_Surface *surf, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char* Rs, unsigned char* Gs, unsigned char* Bs, unsigned char* As )
{
    // Cette fonction est un algo type BRESENHAM qui lance un rayon depuis la lumière jusqu'à chaque pixel des murs
    // Elle analyse la présence de blocs de glace de couleurs qui peuvent changer la couleur de la lumière du départ (Rs, Gs, Bs)
    // Elle dessine le point sur le mur de la bonne couleur

    // ATTENTION : toujours mettre x1 et y1 comme étant en premier et correspondant à la source de lumière !!!!


    char currentCellX = -1;
    char currentCellY = -1;

    unsigned int x,y;
    int dx,dy,swp,temp,s1,s2,p,i;

    float tempx, tempy;

    x=x1;
    y=y1;
    dx=_abs(x2-x1);
    dy=_abs(y2-y1);
    s1=_sgn(x2-x1);
    s2=_sgn(y2-y1);
    swp=0;

    if(dy>dx)
    {
        temp=dx;
        dx=dy;
        dy=temp;
        swp=1;
    }
    p=2*dy-dx;
    for(i=0; i<=dx; i+=SPEEDUP_RAY)
    {
        tempx = ((float) x - (float) OFFSET_X) / ((float) SIZE);
        tempy = ((float) y - (float) OFFSET_Y) / ((float) SIZE);
        currentCellX = (unsigned int) floor( tempx );
        currentCellY = (unsigned int) floor( tempy );

        //currentCellX = (x-OFFSET_X)/SIZE;
        //currentCellY = (y-OFFSET_Y)/SIZE;
        int result = computeLightModification( currentCellX, currentCellY, Rs, Gs, Bs, As );


        if (result==1)
        {
            if ((*Rs!=lumiere.R || *Gs!=lumiere.G || *Bs!=lumiere.B)  && *As==ALPHA_BLOQUE)
            {
                boxRGBA( surf, x-1, y-1, x+1, y+1,  *Rs, *Gs, *Bs, 255 );
            }
        }
        else if (result==0)
        {
            pixelRGBA( surf,  x, y, *Rs, *Gs, *Bs, *As );
        }


        while(p>=0)
        {
            p=p-2*dx;
            if(swp)
                x+=s1*SPEEDUP_RAY;
            else
                y+=s2*SPEEDUP_RAY;
        }
        p=p+2*dy;
        if(swp)
            y+=s2*SPEEDUP_RAY;
        else
            x+=s1*SPEEDUP_RAY;
    }
}

void renderWalls( SDL_Surface *surf )
{
    unsigned char lR = lumiere.R;
    unsigned char lG = lumiere.G;
    unsigned char lB = lumiere.B;
    unsigned char lA = lumiere.A;

    unsigned int lX = SIZE*lumiere.x + OFFSET_X + SIZE/2;
    unsigned int lY = SIZE*lumiere.y + OFFSET_Y + SIZE/2;

    //pixelRGBA( surf, lX, lY,  255,0,0,255 );

    for( unsigned char k=0; k<4; k++ )
    {
        if (murs[k].direction == HORIZONTAL)
        {
            for( unsigned int z=murs[k].mini; z<=murs[k].maxi; z+=SPEEDUP_WALL)
            {
                lR = lumiere.R;
                lG = lumiere.G;
                lB = lumiere.B;
                lA = lumiere.A;

                rayCast( surf, lX, lY, z, murs[k].fixe,  &lR, &lG, &lB, &lA );

                if (lR!=lumiere.R || lG!=lumiere.G || lB!=lumiere.B || lA!=lumiere.A )
                {
                    if(lA!=ALPHA_BLOQUE)
                    {
                        pixelRGBA( surf, z, murs[k].fixe+1,  lR,lG,lB, 255 );
                        pixelRGBA( surf, z, murs[k].fixe,    lR,lG,lB, 255 );
                        pixelRGBA( surf, z, murs[k].fixe-1,  lR,lG,lB, 255 );
                    }
                }
            }
        }

        else if (murs[k].direction == VERTICAL)
        {
            for( unsigned int z=murs[k].mini; z<=murs[k].maxi; z+=SPEEDUP_WALL)
            {
                lR = lumiere.R;
                lG = lumiere.G;
                lB = lumiere.B;
                lA = lumiere.A;

                rayCast( surf, lX, lY, murs[k].fixe, z, &lR, &lG, &lB, &lA );

                if (lR!=lumiere.R || lG!=lumiere.G || lB!=lumiere.B || lA!=lumiere.A )
                {
                    if (lA!=ALPHA_BLOQUE)
                    {
                        pixelRGBA( surf, murs[k].fixe+1, z, lR,lG,lB,255 );
                        pixelRGBA( surf, murs[k].fixe,   z, lR,lG,lB,255 );
                        pixelRGBA( surf, murs[k].fixe-1, z, lR,lG,lB,255 );
                    }
                }
            }
        }
    }
}


gboolean on_toggle_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == TRUE)
    {
        return TRUE;
    }
    return FALSE;
}


void buildInterfaceEditor( void )
{
    pButton = gtk_button_new_with_label("test");
    pVbox = gtk_vbox_new(FALSE,10);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    char title[100];
    sprintf( title, "Magic Light Level Editor v 0.9B by SlyVTT [2021/2022] - Level %d", currentLevel );
    gtk_window_set_title( GTK_WINDOW( pWindow ), title );
    gtk_window_set_icon_from_file( GTK_WINDOW( pWindow ), "./icone.png", NULL);


    pHbox = gtk_hbox_new(FALSE,5);

    //pHGrid = gtk_table_new( 1, 7, FALSE );


    butgrid = gtk_table_new( 20, 27, TRUE );

    pButtonP = gtk_toggle_button_new();
    imageP = gtk_image_new_from_file ("./images/Player.png");
    gtk_button_set_image (GTK_BUTTON (pButtonP), imageP);
    gtk_button_set_image_position (GTK_BUTTON (pButtonP), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonP, 0, 1, 0, 1);

    pButtonL = gtk_toggle_button_new();
    imageL = gtk_image_new_from_file ("./images/Light.png");
    gtk_button_set_image (GTK_BUTTON (pButtonL), imageL);
    gtk_button_set_image_position (GTK_BUTTON (pButtonL), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonL, 1, 2, 0, 1);

    pButtonR = gtk_toggle_button_new();
    imageR = gtk_image_new_from_file ("./images/BlocRed.png");
    gtk_button_set_image (GTK_BUTTON (pButtonR), imageR);
    gtk_button_set_image_position (GTK_BUTTON (pButtonR), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonR, 3, 4, 0, 1);

    pButtonG = gtk_toggle_button_new();
    imageG = gtk_image_new_from_file ("./images/BlocGreen.png");
    gtk_button_set_image (GTK_BUTTON (pButtonG), imageG);
    gtk_button_set_image_position (GTK_BUTTON (pButtonG), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonG, 4, 5, 0, 1);

    pButtonB = gtk_toggle_button_new();
    imageB = gtk_image_new_from_file ("./images/BlocBlue.png");
    gtk_button_set_image (GTK_BUTTON (pButtonB), imageB);
    gtk_button_set_image_position (GTK_BUTTON (pButtonB), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonB, 5, 6, 0, 1);

    pButtonN = gtk_toggle_button_new();
    imageN = gtk_image_new_from_file ("./images/BlocBlack.png");
    gtk_button_set_image (GTK_BUTTON (pButtonN), imageN);
    gtk_button_set_image_position (GTK_BUTTON (pButtonN), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonN, 6, 7, 0, 1);


    pButtonColor0 = gtk_radio_button_new( NULL );
    imageColor0 = gtk_image_new_from_file ("./images/ColorScheme0.png");
    gtk_button_set_image (GTK_BUTTON (pButtonColor0), imageColor0);
    gtk_button_set_image_position (GTK_BUTTON (pButtonColor0), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonColor0, 0, 2, 1, 3);
    gtk_toggle_button_set_mode( GTK_TOGGLE_BUTTON( pButtonColor0 ), FALSE );

    pButtonColor1 = gtk_radio_button_new_from_widget( GTK_RADIO_BUTTON (pButtonColor0) );
    imageColor1 = gtk_image_new_from_file ("./images/ColorScheme1.png");
    gtk_button_set_image (GTK_BUTTON (pButtonColor1), imageColor1);
    gtk_button_set_image_position (GTK_BUTTON (pButtonColor1), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonColor1, 2, 4, 1, 3);
    gtk_toggle_button_set_mode( GTK_TOGGLE_BUTTON( pButtonColor1 ), FALSE );

    pButtonColor2 = gtk_radio_button_new_from_widget( GTK_RADIO_BUTTON (pButtonColor0) );
    imageColor2 = gtk_image_new_from_file ("./images/ColorScheme2.png");
    gtk_button_set_image (GTK_BUTTON (pButtonColor2), imageColor2);
    gtk_button_set_image_position (GTK_BUTTON (pButtonColor2), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonColor2, 4, 6, 1, 3);
    gtk_toggle_button_set_mode( GTK_TOGGLE_BUTTON( pButtonColor2 ), FALSE );

    pButtonColor3 = gtk_radio_button_new_from_widget( GTK_RADIO_BUTTON (pButtonColor0) );
    imageColor3 = gtk_image_new_from_file ("./images/ColorScheme3.png");
    gtk_button_set_image (GTK_BUTTON (pButtonColor3), imageColor3);
    gtk_button_set_image_position (GTK_BUTTON (pButtonColor3), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonColor3, 6, 8, 1, 3);
    gtk_toggle_button_set_mode( GTK_TOGGLE_BUTTON( pButtonColor3 ), FALSE );


    pButtonDoorW = gtk_toggle_button_new ();
    imageDoorW = gtk_image_new_from_file ("./images/DoorWest.png");
    gtk_button_set_image (GTK_BUTTON (pButtonDoorW), imageDoorW);
    gtk_button_set_image_position (GTK_BUTTON (pButtonDoorW), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonDoorW, 1, 2, 4, 5);

    pButtonDoorN = gtk_toggle_button_new ();
    imageDoorN = gtk_image_new_from_file ("./images/DoorNorth.png");
    gtk_button_set_image (GTK_BUTTON (pButtonDoorN), imageDoorN);
    gtk_button_set_image_position (GTK_BUTTON (pButtonDoorN), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonDoorN, 2, 3, 4, 5);

    pButtonDoorE = gtk_toggle_button_new ();
    imageDoorE = gtk_image_new_from_file ("./images/DoorEast.png");
    gtk_button_set_image (GTK_BUTTON (pButtonDoorE), imageDoorE);
    gtk_button_set_image_position (GTK_BUTTON (pButtonDoorE), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonDoorE, 3, 4, 4, 5);

    pButtonDoorS = gtk_toggle_button_new ();
    imageDoorS = gtk_image_new_from_file ("./images/DoorSouth.png");
    gtk_button_set_image (GTK_BUTTON (pButtonDoorS), imageDoorS);
    gtk_button_set_image_position (GTK_BUTTON (pButtonDoorS), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonDoorS, 4, 5, 4, 5);

    LabelXReceptors = gtk_label_new( "X" );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), LabelXReceptors, 5, 6, 4, 5);
    LabelYReceptors = gtk_label_new( "Y" );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), LabelYReceptors, 6, 7, 4, 5);
    LabelDirReceptors = gtk_label_new( "Dir." );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), LabelDirReceptors, 7, 8, 4, 5);

    EditXRecRed = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecRed ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecRed, 5, 6, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecRed = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecRed ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecRed, 6, 7, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecRed = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecRed ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecRed ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecRed ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecRed ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecRed, 7, 8, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecGreen = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecGreen ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecGreen, 5, 6, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecGreen = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecGreen ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecGreen, 6, 7, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecGreen = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecGreen ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecGreen ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecGreen ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecGreen ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecGreen, 7, 8, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecBlue = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecBlue ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecBlue, 5, 6, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecBlue = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecBlue ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecBlue, 6, 7, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecBlue = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlue ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlue ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlue ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlue ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecBlue, 7, 8, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecBlack = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecBlack ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecBlack, 5, 6, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecBlack = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecBlack ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecBlack, 6, 7, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecBlack = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlack ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlack ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlack ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecBlack ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecBlack, 7, 8, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecYellow = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecYellow ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecYellow, 5, 6, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecYellow = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecYellow ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecYellow, 6, 7, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecYellow = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecYellow ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecYellow ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecYellow ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecYellow ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecYellow, 7, 8, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecCyan = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecCyan ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecCyan, 5, 6, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecCyan = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecCyan ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecCyan, 6, 7, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecCyan = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecCyan ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecCyan ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecCyan ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecCyan ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecCyan, 7, 8, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecPink = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecPink ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecPink, 5, 6, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecPink = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecPink ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecPink, 6, 7, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecPink = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecPink ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecPink ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecPink ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecPink ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecPink, 7, 8, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);

    EditXRecWhite = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditXRecWhite ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditXRecWhite, 5, 6, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditYRecWhite = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditYRecWhite ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditYRecWhite, 6, 7, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);
    ComboDirRecWhite = gtk_combo_box_new_text();
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecWhite ), "\u2190" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecWhite ), "\u2191" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecWhite ), "\u2192" );
    gtk_combo_box_append_text ( GTK_COMBO_BOX( ComboDirRecWhite ), "\u2193" );
    gtk_table_attach (GTK_TABLE (butgrid), ComboDirRecWhite, 7, 8, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);

    pButtonReceptRed = gtk_toggle_button_new ();
    imageReceptRed = gtk_image_new_from_file ("./images/ReceptorRed.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptRed), imageReceptRed);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptRed), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptRed, 0, 1, 5, 6);

    pButtonReceptGreen = gtk_toggle_button_new ();
    imageReceptGreen = gtk_image_new_from_file ("./images/ReceptorGreen.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptGreen), imageReceptGreen);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptGreen), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptGreen, 0, 1, 6, 7);

    pButtonReceptBlue = gtk_toggle_button_new ();
    imageReceptBlue = gtk_image_new_from_file ("./images/ReceptorBlue.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptBlue), imageReceptBlue);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptBlue), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptBlue, 0, 1, 7, 8);

    pButtonReceptBlack = gtk_toggle_button_new ();
    imageReceptBlack = gtk_image_new_from_file ("./images/ReceptorBlack.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptBlack), imageReceptBlack);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptBlack), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptBlack, 0, 1, 8, 9);

    pButtonReceptYellow = gtk_toggle_button_new ();
    imageReceptYellow = gtk_image_new_from_file ("./images/ReceptorYellow.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptYellow), imageReceptYellow);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptYellow), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptYellow, 0, 1, 9, 10);

    pButtonReceptCyan = gtk_toggle_button_new ();
    imageReceptCyan = gtk_image_new_from_file ("./images/ReceptorCyan.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptCyan), imageReceptCyan);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptCyan), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptCyan, 0, 1, 10, 11);

    pButtonReceptPink = gtk_toggle_button_new ();
    imageReceptPink = gtk_image_new_from_file ("./images/ReceptorPink.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptPink), imageReceptPink);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptPink), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptPink, 0, 1, 11, 12);

    pButtonReceptWhite = gtk_toggle_button_new ();
    imageReceptWhite = gtk_image_new_from_file ("./images/ReceptorWhite.png");
    gtk_button_set_image (GTK_BUTTON (pButtonReceptWhite), imageReceptWhite);
    gtk_button_set_image_position (GTK_BUTTON (pButtonReceptWhite), GTK_POS_TOP );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), pButtonReceptWhite, 0, 1, 12, 13);

    LabelLevelConnect = gtk_label_new( "\u2192Level" );
    gtk_table_attach_defaults (GTK_TABLE (butgrid), LabelLevelConnect, 0, 1, 13, 14);

    EditConnectWest = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditConnectWest ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditConnectWest, 1, 2, 13, 14, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditConnectNorth = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditConnectNorth ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditConnectNorth, 2, 3, 13, 14, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditConnectEast = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditConnectEast ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditConnectEast, 3, 4, 13, 14, GTK_SHRINK, GTK_SHRINK, 0, 0);
    EditConnectSouth = gtk_entry_new();
    gtk_entry_set_width_chars( GTK_ENTRY( EditConnectSouth ), 3 );
    gtk_table_attach (GTK_TABLE (butgrid), EditConnectSouth, 4, 5, 13, 14, GTK_SHRINK, GTK_SHRINK, 0, 0);

    pCheckWestRed = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestRed, 1, 2, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestGreen = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestGreen, 1, 2, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestBlue = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestBlue, 1, 2, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestBlack = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestBlack, 1, 2, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestYellow = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestYellow, 1, 2, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestPink = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestPink, 1, 2, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestCyan = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestCyan, 1, 2, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckWestWhite = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckWestWhite, 1, 2, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);

    pCheckNorthRed = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthRed, 2, 3, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthGreen = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthGreen, 2, 3, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthBlue = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthBlue, 2, 3, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthBlack = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthBlack, 2, 3, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthYellow = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthYellow, 2, 3, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthPink = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthPink, 2, 3, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthCyan = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthCyan, 2, 3, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckNorthWhite = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckNorthWhite, 2, 3, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);

    pCheckEastRed = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastRed, 3, 4, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastGreen = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastGreen, 3, 4, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastBlue = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastBlue, 3, 4, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastBlack = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastBlack, 3, 4, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastYellow = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastYellow, 3, 4, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastPink = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastPink, 3, 4, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastCyan = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastCyan, 3, 4, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckEastWhite = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckEastWhite, 3, 4, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);

    pCheckSouthRed = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthRed, 4, 5, 5, 6, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthGreen = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthGreen, 4, 5, 6, 7, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthBlue = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthBlue, 4, 5, 7, 8, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthBlack = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthBlack, 4, 5, 8, 9, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthYellow = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthYellow, 4, 5, 9, 10, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthPink = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthPink, 4, 5, 10, 11, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthCyan = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthCyan, 4, 5, 11, 12, GTK_SHRINK, GTK_SHRINK, 0, 0);
    pCheckSouthWhite = gtk_check_button_new( );
    gtk_table_attach (GTK_TABLE (butgrid), pCheckSouthWhite, 4, 5, 12, 13, GTK_SHRINK, GTK_SHRINK, 0, 0);


    //pSdlMiniMap = gtk_sdl_new (200, 400, 32, SDL_SWSURFACE, screenMiniMap);
    pMenuBar = gtk_menu_bar_new();


    pMenu = gtk_menu_new();
    pMenuItem = gtk_menu_item_new_with_label("Nouveau");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    pMenuItem = gtk_menu_item_new_with_label("Ouvrir");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    pMenuItem = gtk_menu_item_new_with_label("Enregistrer");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    pMenuItem = gtk_menu_item_new_with_label("Fermer");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    pMenuItem = gtk_menu_item_new_with_label("Quitter");
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnQuitter),(GtkWidget*) pWindow);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

    pMenuItem = gtk_menu_item_new_with_label("Fichier");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);


    pMenu = gtk_menu_new();
    pMenuItem = gtk_menu_item_new_with_label("À propos de...");
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnAbout),(GtkWidget*) pWindow);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    pMenuItem = gtk_menu_item_new_with_label("?");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);


    gtk_box_pack_start(GTK_BOX(pVbox), pMenuBar, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pVbox),pHbox,TRUE,TRUE,0);

    gtk_box_pack_start(GTK_BOX(pHbox),butgrid,FALSE,FALSE,0);

    pSdlMain = gtk_sdl_new (1, 1, 32, SDL_SWSURFACE, screenMain);
    //gtk_box_pack_start(GTK_BOX(pHbox),pSdlMain,TRUE,TRUE,0);
    gtk_table_attach_defaults(GTK_TABLE (butgrid), pSdlMain, 9, 27, 0, 14 );


    pVboxRight = gtk_vbox_new(FALSE,10);
    gtk_box_pack_start(GTK_BOX(pHbox),pVboxRight,TRUE,TRUE,0);

    pCodeView = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(pVboxRight),pCodeView,TRUE,TRUE,0);


    gtk_container_add(GTK_CONTAINER(pWindow),pVbox);


    gtk_widget_set_events(pSdlMain, GDK_KEY_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(pButton),"clicked",G_CALLBACK(Clicked),NULL);

    g_signal_connect(G_OBJECT(pButtonDoorW),"clicked",G_CALLBACK(DoorWClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonDoorN),"clicked",G_CALLBACK(DoorNClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonDoorE),"clicked",G_CALLBACK(DoorEClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonDoorS),"clicked",G_CALLBACK(DoorSClicked),NULL);

    g_signal_connect(G_OBJECT(pButtonColor0),"clicked",G_CALLBACK(Color0Clicked),NULL);
    g_signal_connect(G_OBJECT(pButtonColor1),"clicked",G_CALLBACK(Color1Clicked),NULL);
    g_signal_connect(G_OBJECT(pButtonColor2),"clicked",G_CALLBACK(Color2Clicked),NULL);
    g_signal_connect(G_OBJECT(pButtonColor3),"clicked",G_CALLBACK(Color3Clicked),NULL);

    g_signal_connect(G_OBJECT(pButtonReceptRed),"clicked",G_CALLBACK(RecepRedClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptGreen),"clicked",G_CALLBACK(RecepGreenClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptBlue),"clicked",G_CALLBACK(RecepBlueClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptBlack),"clicked",G_CALLBACK(RecepBlackClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptYellow),"clicked",G_CALLBACK(RecepYellowClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptCyan),"clicked",G_CALLBACK(RecepCyanClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptPink),"clicked",G_CALLBACK(RecepPinkClicked),NULL);
    g_signal_connect(G_OBJECT(pButtonReceptWhite),"clicked",G_CALLBACK(RecepWhiteClicked),NULL);

    g_signal_connect(G_OBJECT(pWindow), "motion-notify-event", G_CALLBACK(mouse_moved), NULL);
    g_signal_connect(G_OBJECT(pWindow), "button-press-event", G_CALLBACK(mouse_clicked), NULL);
    g_signal_connect(G_OBJECT(pWindow), "key_press_event", G_CALLBACK(key_pressed), NULL);
    g_signal_connect(G_OBJECT(pWindow), "key_release_event", G_CALLBACK(key_released), NULL);
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //gtk_widget_add_tick_callback(pWindow, update, NULL, NULL);

    g_timeout_add(500, (GSourceFunc) time_handler, NULL);


    gtk_widget_set_sensitive( pCheckWestRed, FALSE );
    gtk_widget_set_sensitive( pCheckNorthRed, FALSE );
    gtk_widget_set_sensitive( pCheckEastRed, FALSE );
    gtk_widget_set_sensitive( pCheckSouthRed, FALSE );
    gtk_widget_set_sensitive( EditXRecRed, FALSE );
    gtk_widget_set_sensitive( EditYRecRed, FALSE );
    gtk_widget_set_sensitive( ComboDirRecRed, FALSE );

    gtk_widget_set_sensitive( pCheckWestGreen, FALSE );
    gtk_widget_set_sensitive( pCheckNorthGreen, FALSE );
    gtk_widget_set_sensitive( pCheckEastGreen, FALSE );
    gtk_widget_set_sensitive( pCheckSouthGreen, FALSE );
    gtk_widget_set_sensitive( EditXRecGreen, FALSE );
    gtk_widget_set_sensitive( EditYRecGreen, FALSE );
    gtk_widget_set_sensitive( ComboDirRecGreen, FALSE );

    gtk_widget_set_sensitive( pCheckWestBlue, FALSE );
    gtk_widget_set_sensitive( pCheckNorthBlue, FALSE );
    gtk_widget_set_sensitive( pCheckEastBlue, FALSE );
    gtk_widget_set_sensitive( pCheckSouthBlue, FALSE );
    gtk_widget_set_sensitive( EditXRecBlue, FALSE );
    gtk_widget_set_sensitive( EditYRecBlue, FALSE );
    gtk_widget_set_sensitive( ComboDirRecBlue, FALSE );

    gtk_widget_set_sensitive( pCheckWestBlack, FALSE );
    gtk_widget_set_sensitive( pCheckNorthBlack, FALSE );
    gtk_widget_set_sensitive( pCheckEastBlack, FALSE );
    gtk_widget_set_sensitive( pCheckSouthBlack, FALSE );
    gtk_widget_set_sensitive( EditXRecBlack, FALSE );
    gtk_widget_set_sensitive( EditYRecBlack, FALSE );
    gtk_widget_set_sensitive( ComboDirRecBlack, FALSE );

    gtk_widget_set_sensitive( pCheckWestYellow, FALSE );
    gtk_widget_set_sensitive( pCheckNorthYellow, FALSE );
    gtk_widget_set_sensitive( pCheckEastYellow, FALSE );
    gtk_widget_set_sensitive( pCheckSouthYellow, FALSE );
    gtk_widget_set_sensitive( EditXRecYellow, FALSE );
    gtk_widget_set_sensitive( EditYRecYellow, FALSE );
    gtk_widget_set_sensitive( ComboDirRecYellow, FALSE );

    gtk_widget_set_sensitive( pCheckWestCyan, FALSE );
    gtk_widget_set_sensitive( pCheckNorthCyan, FALSE );
    gtk_widget_set_sensitive( pCheckEastCyan, FALSE );
    gtk_widget_set_sensitive( pCheckSouthCyan, FALSE );
    gtk_widget_set_sensitive( EditXRecCyan, FALSE );
    gtk_widget_set_sensitive( EditYRecCyan, FALSE );
    gtk_widget_set_sensitive( ComboDirRecCyan, FALSE );

    gtk_widget_set_sensitive( pCheckWestPink, FALSE );
    gtk_widget_set_sensitive( pCheckNorthPink, FALSE );
    gtk_widget_set_sensitive( pCheckEastPink, FALSE );
    gtk_widget_set_sensitive( pCheckSouthPink, FALSE );
    gtk_widget_set_sensitive( EditXRecPink, FALSE );
    gtk_widget_set_sensitive( EditYRecPink, FALSE );
    gtk_widget_set_sensitive( ComboDirRecPink, FALSE );

    gtk_widget_set_sensitive( pCheckWestWhite, FALSE );
    gtk_widget_set_sensitive( pCheckNorthWhite, FALSE );
    gtk_widget_set_sensitive( pCheckEastWhite, FALSE );
    gtk_widget_set_sensitive( pCheckSouthWhite, FALSE );
    gtk_widget_set_sensitive( EditXRecWhite, FALSE );
    gtk_widget_set_sensitive( EditYRecWhite, FALSE );
    gtk_widget_set_sensitive( ComboDirRecWhite, FALSE );

    gtk_widget_set_sensitive( EditConnectWest, FALSE );
    gtk_widget_set_sensitive( EditConnectNorth, FALSE );
    gtk_widget_set_sensitive( EditConnectEast, FALSE );
    gtk_widget_set_sensitive( EditConnectSouth, FALSE );




    gtk_widget_show_all(pWindow);
}


void updateProgramme( void )
{
    GtkTextBuffer *buffer;
    GtkTextIter iter;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (pCodeView));

    gchar tampon[1000];
    sprintf( tampon, "%s %d;\n\n", "LEVELCOLOR =", LEVELCOLOR );
    gtk_text_buffer_set_text (buffer, tampon, -1);

    gtk_text_buffer_get_end_iter( buffer, &iter );

    sprintf( tampon, "%s %d;\n", "NbReceptorsLevel =", NbReceptorsLevel );
    gtk_text_buffer_insert( buffer, &iter, tampon, -1 );

    sprintf( tampon, "%s %d;\n", "NbDoorsLevel =", NbDoorsLevel );
    gtk_text_buffer_insert( buffer, &iter, tampon, -1 );

    sprintf( tampon, "%s %d;\n", "NbBlocksLevel =", NbBlocksLevel );
    gtk_text_buffer_insert( buffer, &iter, tampon, -1 );

    sprintf( tampon, "%s %d;\n", "NbTreasureLevel =", NbTreasureLevel );
    gtk_text_buffer_insert( buffer, &iter, tampon, -1 );

    sprintf( tampon, "%s %d;\n", "NbObstacleLevel =", NbObstacleLevel );
    gtk_text_buffer_insert( buffer, &iter, tampon, -1 );

    sprintf( tampon, "%s %d;\n", "NbMonsterLevel =", NbMonsterLevel );
    gtk_text_buffer_insert( buffer, &iter, tampon, -1 );



}

gboolean time_handler(GtkWidget *widget)
{

    updateMainScreen();
    updateProgramme();

    return TRUE;
}

void renderMap( SDL_Surface *surf )
{
    //dimage(OFFSET_X, OFFSET_Y, &mapbackgrd);
    unsigned int OFFSET_X_TILE = 0;

    OFFSET_X_TILE = 64*LEVELCOLOR*ZOOMFACTOR;

    for(unsigned char x=0; x<SIZE_MAP_X; x++)
        for( unsigned char y=0; y<SIZE_MAP_Y; y++)
        {
            unsigned int lX=OFFSET_X+x*SIZE*ZOOMFACTOR;
            unsigned int lY=OFFSET_Y+y*SIZE*ZOOMFACTOR;

            SDL_Rect source, dest;

            dest.x = lX;
            dest.y = lY;
            dest.w = SIZE * ZOOMFACTOR;
            dest.h = SIZE * ZOOMFACTOR;


            if(y==0)
            {
                if (x==0)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+0,0,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE;
                    source.y = 0;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else if (x==SIZE_MAP_X-1)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+16,0,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+SIZE*ZOOMFACTOR;
                    source.y = 0;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+32,0,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+2*SIZE*ZOOMFACTOR;
                    source.y = 0;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
            }

            else if(y==SIZE_MAP_Y-1)
            {
                if (x==0)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+0,16,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE;
                    source.y = SIZE * ZOOMFACTOR;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else if (x==SIZE_MAP_X-1)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+16,16,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+SIZE*ZOOMFACTOR;
                    source.y = SIZE * ZOOMFACTOR;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+48,16,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+3*SIZE*ZOOMFACTOR;
                    source.y = SIZE * ZOOMFACTOR;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
            }
            else
            {
                if (x==0)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+48,0,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+3*SIZE*ZOOMFACTOR;
                    source.y = 0;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else if (x==SIZE_MAP_X-1)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+32,16,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+2*SIZE*ZOOMFACTOR;
                    source.y = SIZE * ZOOMFACTOR;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else if ((x+y)%2==0)
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+0,32,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE;
                    source.y = 2*SIZE*ZOOMFACTOR;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
                else
                    //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+16,32,16,16,  DIMAGE_NONE);
                {
                    source.x = OFFSET_X_TILE+SIZE*ZOOMFACTOR;
                    source.y = 2*SIZE * ZOOMFACTOR;;
                    source.w = SIZE * ZOOMFACTOR;
                    source.h = SIZE * ZOOMFACTOR;
                    SDL_BlitSurface( leveltiles, &source, surf, &dest );
                }
            }
        }
}



void updateMainScreen( void )
{
    printf("try to create first surface\n");

    screenCalcul = SDL_CreateRGBSurface(0, 550, 500, 32, 0, 0, 0, 0);
    SDL_Surface *double_buffer = SDL_SetVideoMode(1100, 1000, 32, SDL_SWSURFACE);

    SDL_FillRect( double_buffer, NULL, 0x00000000);
    SDL_FillRect( screenCalcul, NULL, 0x00000000);
    //boxRGBA( double_buffer, 10, 10, 20, 20, 255, 0,0, 255);

    //SDL_BlitSurface(leveltiles, NULL, double_buffer, NULL );

    drawInterface( screenCalcul );
    drawMinimap( screenCalcul );

    renderMap( screenCalcul );

    checkDoors( screenCalcul);
    renderDoors( screenCalcul);

    renderObstacles( screenCalcul, LAYER_BELOW_RAYS );
    renderObstacles( screenCalcul, LAYER_ABOVE_RAYS );

    renderWalls( screenCalcul );

    checkReceptors( screenCalcul );
    renderReceptors( screenCalcul );

    renderMovable( screenCalcul );

    renderLight( screenCalcul );
    renderTreasures( screenCalcul );

    renderMonsters( screenCalcul );
    renderBoss(screenCalcul );

    renderPlayer( screenCalcul );


    SDL_BlitSurface( rotozoomSurface( screenCalcul, 0, 2, FALSE ),NULL, double_buffer, NULL );


    SDL_BlitSurface(double_buffer,NULL,screenMain,NULL);

    gtk_sdl_display_update (GTK_SDL(pSdlMain));
    SDL_FreeSurface(double_buffer);
}



void Clicked(GtkWidget * pWindow,gpointer data)
{
    updateMainScreen();
}


void Color0Clicked(GtkWidget * pWindow,gpointer data)
{
    LEVELCOLOR = 0;
    updateMainScreen( );
}

void Color1Clicked(GtkWidget * pWindow,gpointer data)
{
    LEVELCOLOR = 1;
    updateMainScreen( );
}

void Color2Clicked(GtkWidget * pWindow,gpointer data)
{
    LEVELCOLOR = 2;
    updateMainScreen( );
}

void Color3Clicked(GtkWidget * pWindow,gpointer data)
{
    LEVELCOLOR = 3;
    updateMainScreen( );
}

void DoorWClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE )
    {
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptRed) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestRed, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptGreen) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestGreen, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlue) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestBlue, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlack) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestBlack, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptYellow) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestYellow, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptPink) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestPink, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptCyan) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestCyan, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptWhite) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestWhite, TRUE );
        gtk_widget_set_sensitive( EditConnectWest, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestRed, FALSE );
        gtk_widget_set_sensitive( pCheckWestGreen, FALSE );
        gtk_widget_set_sensitive( pCheckWestBlue, FALSE );
        gtk_widget_set_sensitive( pCheckWestBlack, FALSE );
        gtk_widget_set_sensitive( pCheckWestYellow, FALSE );
        gtk_widget_set_sensitive( pCheckWestPink, FALSE );
        gtk_widget_set_sensitive( pCheckWestCyan, FALSE );
        gtk_widget_set_sensitive( pCheckWestWhite, FALSE );
        gtk_widget_set_sensitive( EditConnectWest, FALSE );
    }
}

void DoorNClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE )
    {
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptRed) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthRed, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptGreen) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthGreen, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlue) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthBlue, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlack) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthBlack, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptYellow) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthYellow, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptPink) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthPink, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptCyan) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthCyan, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptWhite) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthWhite, TRUE );
        gtk_widget_set_sensitive( EditConnectNorth, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckNorthRed, FALSE );
        gtk_widget_set_sensitive( pCheckNorthGreen, FALSE );
        gtk_widget_set_sensitive( pCheckNorthBlue, FALSE );
        gtk_widget_set_sensitive( pCheckNorthBlack, FALSE );
        gtk_widget_set_sensitive( pCheckNorthYellow, FALSE );
        gtk_widget_set_sensitive( pCheckNorthPink, FALSE );
        gtk_widget_set_sensitive( pCheckNorthCyan, FALSE );
        gtk_widget_set_sensitive( pCheckNorthWhite, FALSE );
        gtk_widget_set_sensitive( EditConnectNorth, FALSE );
    }
}

void DoorEClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE )
    {
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptRed) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastRed, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptGreen) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastGreen, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlue) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastBlue, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlack) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastBlack, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptYellow) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastYellow, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptPink) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastPink, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptCyan) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastCyan, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptWhite) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastWhite, TRUE );
        gtk_widget_set_sensitive( EditConnectEast, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckEastRed, FALSE );
        gtk_widget_set_sensitive( pCheckEastGreen, FALSE );
        gtk_widget_set_sensitive( pCheckEastBlue, FALSE );
        gtk_widget_set_sensitive( pCheckEastBlack, FALSE );
        gtk_widget_set_sensitive( pCheckEastYellow, FALSE );
        gtk_widget_set_sensitive( pCheckEastPink, FALSE );
        gtk_widget_set_sensitive( pCheckEastCyan, FALSE );
        gtk_widget_set_sensitive( pCheckEastWhite, FALSE );
        gtk_widget_set_sensitive( EditConnectEast, FALSE );
    }
}

void DoorSClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE )
    {
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptRed) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthRed, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptGreen) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthGreen, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlue) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthBlue, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlack) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthBlack, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptYellow) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthYellow, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptPink) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthPink, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptCyan) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthCyan, TRUE );
        if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptWhite) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthWhite, TRUE );
        gtk_widget_set_sensitive( EditConnectSouth, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckSouthRed, FALSE );
        gtk_widget_set_sensitive( pCheckSouthGreen, FALSE );
        gtk_widget_set_sensitive( pCheckSouthBlue, FALSE );
        gtk_widget_set_sensitive( pCheckSouthBlack, FALSE );
        gtk_widget_set_sensitive( pCheckSouthYellow, FALSE );
        gtk_widget_set_sensitive( pCheckSouthPink, FALSE );
        gtk_widget_set_sensitive( pCheckSouthCyan, FALSE );
        gtk_widget_set_sensitive( pCheckSouthWhite, FALSE );
        gtk_widget_set_sensitive( EditConnectSouth, FALSE );
    }
}

void RecepRedClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptRed) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestRed, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthRed, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastRed, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthRed, TRUE );
        gtk_widget_set_sensitive( EditXRecRed, TRUE );
        gtk_widget_set_sensitive( EditYRecRed, TRUE );
        gtk_widget_set_sensitive( ComboDirRecRed, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestRed, FALSE );
        gtk_widget_set_sensitive( pCheckNorthRed, FALSE );
        gtk_widget_set_sensitive( pCheckEastRed, FALSE );
        gtk_widget_set_sensitive( pCheckSouthRed, FALSE );
        gtk_widget_set_sensitive( EditXRecRed, FALSE );
        gtk_widget_set_sensitive( EditYRecRed, FALSE );
        gtk_widget_set_sensitive( ComboDirRecRed, FALSE );
    }
}

void RecepGreenClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptGreen) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestGreen, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthGreen, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastGreen, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthGreen, TRUE );
        gtk_widget_set_sensitive( EditXRecGreen, TRUE );
        gtk_widget_set_sensitive( EditYRecGreen, TRUE );
        gtk_widget_set_sensitive( ComboDirRecGreen, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestGreen, FALSE );
        gtk_widget_set_sensitive( pCheckNorthGreen, FALSE );
        gtk_widget_set_sensitive( pCheckEastGreen, FALSE );
        gtk_widget_set_sensitive( pCheckSouthGreen, FALSE );
        gtk_widget_set_sensitive( EditXRecGreen, FALSE );
        gtk_widget_set_sensitive( EditYRecGreen, FALSE );
        gtk_widget_set_sensitive( ComboDirRecGreen, FALSE );
    }
}

void RecepBlueClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlue) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestBlue, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthBlue, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastBlue, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthBlue, TRUE );
        gtk_widget_set_sensitive( EditXRecBlue, TRUE );
        gtk_widget_set_sensitive( EditYRecBlue, TRUE );
        gtk_widget_set_sensitive( ComboDirRecBlue, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestBlue, FALSE );
        gtk_widget_set_sensitive( pCheckNorthBlue, FALSE );
        gtk_widget_set_sensitive( pCheckEastBlue, FALSE );
        gtk_widget_set_sensitive( pCheckSouthBlue, FALSE );
        gtk_widget_set_sensitive( EditXRecBlue, FALSE );
        gtk_widget_set_sensitive( EditYRecBlue, FALSE );
        gtk_widget_set_sensitive( ComboDirRecBlue, FALSE );
    }
}

void RecepBlackClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptBlack) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestBlack, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthBlack, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastBlack, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthBlack, TRUE );
        gtk_widget_set_sensitive( EditXRecBlack, TRUE );
        gtk_widget_set_sensitive( EditYRecBlack, TRUE );
        gtk_widget_set_sensitive( ComboDirRecBlack, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestBlack, FALSE );
        gtk_widget_set_sensitive( pCheckNorthBlack, FALSE );
        gtk_widget_set_sensitive( pCheckEastBlack, FALSE );
        gtk_widget_set_sensitive( pCheckSouthBlack, FALSE );
        gtk_widget_set_sensitive( EditXRecBlack, FALSE );
        gtk_widget_set_sensitive( EditYRecBlack, FALSE );
        gtk_widget_set_sensitive( ComboDirRecBlack, FALSE );
    }
}

void RecepYellowClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptYellow) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestYellow, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthYellow, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastYellow, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthYellow, TRUE );
        gtk_widget_set_sensitive( EditXRecYellow, TRUE );
        gtk_widget_set_sensitive( EditYRecYellow, TRUE );
        gtk_widget_set_sensitive( ComboDirRecYellow, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestYellow, FALSE );
        gtk_widget_set_sensitive( pCheckNorthYellow, FALSE );
        gtk_widget_set_sensitive( pCheckEastYellow, FALSE );
        gtk_widget_set_sensitive( pCheckSouthYellow, FALSE );
        gtk_widget_set_sensitive( EditXRecYellow, FALSE );
        gtk_widget_set_sensitive( EditYRecYellow, FALSE );
        gtk_widget_set_sensitive( ComboDirRecYellow, FALSE );
    }
}

void RecepCyanClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptCyan) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestCyan, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthCyan, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastCyan, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthCyan, TRUE );
        gtk_widget_set_sensitive( EditXRecCyan, TRUE );
        gtk_widget_set_sensitive( EditYRecCyan, TRUE );
        gtk_widget_set_sensitive( ComboDirRecCyan, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestCyan, FALSE );
        gtk_widget_set_sensitive( pCheckNorthCyan, FALSE );
        gtk_widget_set_sensitive( pCheckEastCyan, FALSE );
        gtk_widget_set_sensitive( pCheckSouthCyan, FALSE );
        gtk_widget_set_sensitive( EditXRecCyan, FALSE );
        gtk_widget_set_sensitive( EditYRecCyan, FALSE );
        gtk_widget_set_sensitive( ComboDirRecCyan, FALSE );
    }
}

void RecepPinkClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptPink) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestPink, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthPink, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastPink, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthPink, TRUE );
        gtk_widget_set_sensitive( EditXRecPink, TRUE );
        gtk_widget_set_sensitive( EditYRecPink, TRUE );
        gtk_widget_set_sensitive( ComboDirRecPink, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestPink, FALSE );
        gtk_widget_set_sensitive( pCheckNorthPink, FALSE );
        gtk_widget_set_sensitive( pCheckEastPink, FALSE );
        gtk_widget_set_sensitive( pCheckSouthPink, FALSE );
        gtk_widget_set_sensitive( EditXRecPink, FALSE );
        gtk_widget_set_sensitive( EditYRecPink, FALSE );
        gtk_widget_set_sensitive( ComboDirRecPink, FALSE );
    }
}

void RecepWhiteClicked(GtkWidget * pWindow,gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(pButtonReceptWhite) ) == TRUE )
    {
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorW) ) == TRUE ) gtk_widget_set_sensitive( pCheckWestWhite, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorN) ) == TRUE ) gtk_widget_set_sensitive( pCheckNorthWhite, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorE) ) == TRUE ) gtk_widget_set_sensitive( pCheckEastWhite, TRUE );
        if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(pButtonDoorS) ) == TRUE ) gtk_widget_set_sensitive( pCheckSouthWhite, TRUE );
        gtk_widget_set_sensitive( EditXRecWhite, TRUE );
        gtk_widget_set_sensitive( EditYRecWhite, TRUE );
        gtk_widget_set_sensitive( ComboDirRecWhite, TRUE );
    }
    else
    {
        gtk_widget_set_sensitive( pCheckWestWhite, FALSE );
        gtk_widget_set_sensitive( pCheckNorthWhite, FALSE );
        gtk_widget_set_sensitive( pCheckEastWhite, FALSE );
        gtk_widget_set_sensitive( pCheckSouthWhite, FALSE );
        gtk_widget_set_sensitive( EditXRecWhite, FALSE );
        gtk_widget_set_sensitive( EditYRecWhite, FALSE );
        gtk_widget_set_sensitive( ComboDirRecWhite, FALSE );
    }
}


void initSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Erreur à l'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
    gtk_main_quit();
    done = true;
}

void OnQuitter(GtkWidget* widget, gpointer data)
{
    GtkWidget *pQuestion;

    pQuestion = gtk_message_dialog_new(GTK_WINDOW(data),
                                       GTK_DIALOG_MODAL,
                                       GTK_MESSAGE_QUESTION,
                                       GTK_BUTTONS_YES_NO,
                                       "Voulez vous vraiment\n"
                                       "quitter le programme?");

    switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
    {
    case GTK_RESPONSE_YES:
        SDL_Quit();
        gtk_main_quit();
        done = true;
        break;
    case GTK_RESPONSE_NONE:
    case GTK_RESPONSE_NO:
        gtk_widget_destroy(pQuestion);
        break;
    }
}

void OnAbout(GtkWidget* widget, gpointer data)
{
    GtkWidget *pAbout;

    pAbout = gtk_message_dialog_new(GTK_WINDOW(data),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK,
                                    "Game Level Editor for Magic Light \n"
                                    "\n"
                                    "by SlyVTT - 2021/2022 \n"
                                    "\n"
                                    "https://github.com/SlyVTT/MagicLight");

    gtk_dialog_run(GTK_DIALOG(pAbout));

    gtk_widget_destroy(pAbout);
}

static gboolean mouse_moved(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    printf( "mouse moved !! \n");
    if (event->type == GDK_MOTION_NOTIFY)
    {
        GdkEventMotion *e = (GdkEventMotion*)event;
        printf("Mouse Moved Coordinates: (%u, %u)\n", (guint)e->x, (guint)e->y);
        return TRUE;
    }
    return FALSE;
}

static gboolean mouse_clicked(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    printf( "mouse clicked !! \n");
    if (event->type == GDK_BUTTON_PRESS)
    {
        GdkEventMotion *e = (GdkEventMotion*)event;
        printf("Mouse Clicked At Coordinates: (%u, %u)\n", (guint)e->x, (guint)e->y);
        return TRUE;
    }
    return FALSE;
}


static gboolean key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{

    if (event->keyval == GDK_KEY_F1)
    {
        //is_w_pressed = TRUE;
        printf("F1 key pressed!\n");

        selected = JOUEUR;
        updateMainScreen();
        return TRUE;
    }
    if (event->keyval == GDK_KEY_F2)
    {
        //is_w_pressed = TRUE;
        printf("F2 key pressed!\n");

        selected = LUMIERE;
        updateMainScreen();
        return TRUE;
    }
    if (event->keyval == GDK_KEY_F3)
    {
        //is_w_pressed = TRUE;
        printf("F3 key pressed!\n");

        selected = ROUGE;
        updateMainScreen();
        return TRUE;
    }
    if (event->keyval == GDK_KEY_F4)
    {
        //is_w_pressed = TRUE;
        printf("F4 key pressed!\n");

        selected = VERT;
        updateMainScreen();
        return TRUE;
    }
    if (event->keyval == GDK_KEY_F5)
    {
        //is_w_pressed = TRUE;
        printf("F5 key pressed!\n");

        selected = BLEU;
        updateMainScreen();
        return TRUE;
    }
    if (event->keyval == GDK_KEY_F6)
    {
        //is_w_pressed = TRUE;
        printf("F6 key pressed!\n");

        selected = NOIR;
        updateMainScreen();
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Page_Down)
    {
        //is_w_pressed = TRUE;
        printf("P_DOWN key pressed!\n");
        if (currentLevel>0)
        {
            currentLevel--;
            loadLevel(currentLevel);
            char title[100];
            sprintf( title, "Magic Light Level Editor v 0.9B by SlyVTT [2021/2022] - Level %d", currentLevel );
            gtk_window_set_title( GTK_WINDOW( pWindow ), title );
        }
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Page_Up)
    {
        //is_w_pressed = TRUE;
        printf("P_UP key pressed!\n");
        printf("P_UP key pressed!\n");
        if (currentLevel<NB_LEVELS)
        {
            currentLevel++;
            loadLevel(currentLevel);
            char title[100];
            sprintf( title, "Magic Light Level Editor v 0.9B by SlyVTT [2021/2022] - Level %d", currentLevel );
            gtk_window_set_title( GTK_WINDOW( pWindow ), title );
        }
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Left)
    {
        //is_w_pressed = TRUE;
        printf("LEFT key pressed!\n");

        gameMechanics( selected, GAUCHE);
        updateMainScreen();
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Right)
    {
        //is_a_pressed = TRUE;
        printf("RIGHT key pressed!\n");

        gameMechanics( selected, DROITE);
        updateMainScreen();
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Up)
    {
        //is_s_pressed = TRUE;
        printf("UP key pressed!\n");

        gameMechanics( selected, HAUT);
        updateMainScreen();
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Down)
    {
        //is_d_pressed = TRUE;
        printf("DOWN key pressed!\n");

        gameMechanics( selected, BAS);
        updateMainScreen();
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_space)
    {
        //is_d_pressed = TRUE;
        printf("SPACE key pressed!\n");

        updateTreasures( selected );
        updateMainScreen();
        return TRUE;
    }
    return FALSE;
}

static gboolean key_released(GtkWidget *widget, GdkEventKey *event)
{
    /*
        if (event->keyval == GDK_KEY_w)
        {
            //is_w_pressed = FALSE;
            printf("W key released!\n");
        }
        if (event->keyval == GDK_KEY_a)
        {
            //is_a_pressed = FALSE;
            printf("A key released!\n");
        }
        if (event->keyval == GDK_KEY_s)
        {
            //is_s_pressed = FALSE;
            printf("S key released!\n");
        }
        if (event->keyval == GDK_KEY_d)
        {
            //is_d_pressed = FALSE;
            printf("D key released!\n");
        }
    */
    return FALSE;
}

/*
gboolean update (GtkWidget *widget, GdkFrameClock *clock, gpointer data)
{
    if (is_w_pressed) {
        printf("W key pressed!\n");
    }
    if (is_a_pressed) {
        printf("A key pressed!\n");
    }
    if (is_s_pressed) {
        printf("S key pressed!\n");
    }
    if (is_d_pressed) {
        printf("D key pressed!\n");
    }
    printf("updating...\n");
}
*/
