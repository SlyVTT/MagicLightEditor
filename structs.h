#define NB_RECEPTORS_PER_DOOR_MAX 3

typedef enum
{
    GAUCHE = 0,
    DROITE = 1,
    HAUT = 2,
    BAS = 3,
    AUCUNE = 4,
    VERTICAL = 5,
    HORIZONTAL = 6
} orientations;

typedef struct
{
    unsigned char x;
    unsigned char y;
    orientations direction;
    unsigned char R, G, B;
    bool isvalide;
} Receptor;

typedef enum
{
    NONE = 0,
    JOUEUR = 1,
    LUMIERE = 2,
    ROUGE = 3,
    VERT = 4,
    BLEU = 5,
    NOIR = 6
} selection;


typedef enum
{
    OPAQUE = 0,
    TRANSPARENT = 1
} kind;

typedef struct
{
    unsigned char x;
    unsigned char y;
    unsigned char R,G,B,A;
    kind type;
} Blocks;

typedef struct
{
    unsigned char x;
    unsigned char y;
    orientations direction;
} Player;

typedef struct
{
    unsigned char ID;
    unsigned char x;
    unsigned char y;
    orientations direction;
} MapItem;

typedef struct
{
    unsigned char x;
    unsigned char y;
    unsigned char R,G,B,A;
    bool movable;
} Light;

typedef struct
{
    orientations direction;
    unsigned int mini;
    unsigned int maxi;
    unsigned int fixe;
} Wall;

typedef struct
{
    unsigned char x, y;
    orientations direction;
    bool isopen;
    bool isdoor;
    unsigned char nbreceptor;
    char receptors[NB_RECEPTORS_PER_DOOR_MAX];
    char nextLevelNumber;
} Door;

typedef enum
{
    RIEN = 0,
    PIERRE_NOIRE,
    PIERRE_VERTE,
    PIERRE_BLEUE,
    PIERRE_ROUGE,
    PIERRE_BLANCHE,
    LINGOT_FER,
    LINGOT_BRONZE,
    LINGOT_ARGENT,
    LINGOT_OR,
    LINGOT_PLATINE
} content;

typedef enum
{
    T_RED = 0,
    T_YELLOW,
    T_GREEN,
    T_BLUE
} chest;

typedef enum
{
    BLOCK_STONE = 0,
    BLOCK_MOON,
    BLOCK_WOOD,
    BLOCK_SUN,
    BLOCK_WATER
} nonmovable;

typedef struct
{
    unsigned char x, y;
    nonmovable type;
} Obstacle;

typedef struct
{
    unsigned char x, y;
    chest type;
    bool isopen;
    content  inside;
    unsigned int scoreboost;
} Treasure;

typedef struct
{
    char x, y;
    unsigned char R,G,B,A;
    bool visited;
} Minimap;
