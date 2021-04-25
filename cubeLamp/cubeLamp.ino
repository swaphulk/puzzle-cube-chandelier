#include <FastLED.h>
#define LED_PIN     4
#define NUM_LEDS    486
CRGB leds[NUM_LEDS];
int i;
int n;
int x;
int sped = 20; // how long to wait while cycling LEDs during a twist
bool turning; // whether an LED update just happened
unsigned long previousTurn = 0; // when the last turn happened
unsigned long previousBlend = 0; // when the last LED cycled
char mode = 'w'; // current mode
bool dir = false; // in wander mode, moving away from solved, or back to it 
byte moveSize = 49; // how far wander mode can wander
byte moves[100]; // the stack of moves in wander mode
byte moveIndex = 0; // where we are in the stack
byte chosenMove; // what move to do next
char letter = ' '; // current command from bluetooth
char setting = ' '; // which setting is going to be set by the next command
int turnDelay = 1000; // how many milliseconds to wait between twists
byte inMove = 0; // if we are in the middle of a twist
byte currentMove; // what move we are currently in the middle of
byte noBacksies; // the reverse of the last move in wander mode, so we don't undo ourselves

// an RGB value for each LED, keeps track of what color the LEDs are aiming to become
byte goal[6][81][3] = {
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},},
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},},
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},},
  };
// an RGB value for each LED, keeps track of what color the LEDs currently are
byte actual[6][81][3] = {
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},},
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},},
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
  {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},},
};

// describes the cycle of LEDs around the rim of the cube's Front
int f_12[36][3] = {
  {162, 163, 164}, {165, 166, 167}, {168, 169, 170},
  {207, 208, 209}, {210, 211, 212}, {213, 214, 215},
  {216, 217, 218}, {219, 220, 221}, {222, 223, 224},
  {162+243, 163+243, 164+243}, {165+243, 166+243, 167+243}, {168+243, 169+243, 170+243},
  {207+243, 208+243, 209+243}, {210+243, 211+243, 212+243}, {213+243, 214+243, 215+243},
  {216+243, 217+243, 218+243}, {219+243, 220+243, 221+243}, {222+243, 223+243, 224+243},
  {303, 302, 297}, {304, 301, 298}, {305, 300, 299},
  {303+9, 302+9, 297+9}, {304+9, 301+9, 298+9}, {305+9, 300+9, 299+9},
  {303+9+9, 302+9+9, 297+9+9}, {304+9+9, 301+9+9, 298+9+9}, {305+9+9, 300+9+9, 299+9+9},
  {135,140,141}, {136,139,142}, {137,138,143},
  {135+9,140+9,141+9}, {136+9,139+9,142+9}, {137+9,138+9,143+9},
  {135+9+9,140+9+9,141+9+9}, {136+9+9,139+9+9,142+9+9}, {137+9+9,138+9+9,143+9+9},
};
int offset_f = 324;
// describes the cycle of LEDs around the face of the cube's Front
int f_8[36][2] = {
    {6+offset_f, 7+offset_f},   {8+offset_f, 5+offset_f},   {4+offset_f, 3+offset_f},
    {0+offset_f, 1+offset_f},   {2+offset_f, 9+offset_f}, {14+offset_f, 15+offset_f},
  {10+offset_f, 13+offset_f}, {16+offset_f, 17+offset_f}, {12+offset_f, 11+offset_f},
  {18+offset_f, 19+offset_f}, {20+offset_f, 21+offset_f}, {22+offset_f, 23+offset_f},
  {24+offset_f, 25+offset_f}, {26+offset_f, 27+offset_f}, {28+offset_f, 29+offset_f},
  {32+offset_f, 31+offset_f}, {30+offset_f, 35+offset_f}, {34+offset_f, 33+offset_f},
  {74+offset_f, 73+offset_f}, {72+offset_f, 75+offset_f}, {76+offset_f, 77+offset_f},
  {80+offset_f, 79+offset_f}, {78+offset_f, 71+offset_f}, {66+offset_f, 65+offset_f},
  {70+offset_f, 67+offset_f}, {64+offset_f, 63+offset_f}, {68+offset_f, 69+offset_f},
  {62+offset_f, 61+offset_f}, {60+offset_f, 59+offset_f}, {58+offset_f, 57+offset_f},
  {56+offset_f, 55+offset_f}, {54+offset_f, 53+offset_f}, {52+offset_f, 51+offset_f},
  {48+offset_f, 49+offset_f}, {50+offset_f, 45+offset_f}, {46+offset_f, 47+offset_f}
};

// describes the cycle of LEDs around the rim of the cube's Back
int b_12[36][3] = {
  {101, 102, 107}, {106, 100, 103}, {99, 104, 105},
  {98, 93, 92}, {97, 94, 91}, {96, 95, 90},
  {89, 84, 83}, {88, 85, 82}, {87, 86, 81}, 
  {269, 264, 263}, {268, 265, 262}, {267, 266, 261},
  {260, 255, 254}, {259, 256, 253}, {258, 257, 252},
  {251, 246, 245}, {250, 247, 244}, {249, 248, 243},
  {485, 484, 483}, {480, 481, 482}, {477, 478, 479},
  {438, 439, 440}, {435, 436, 437}, {432, 433, 434},
  {429, 430, 431}, {426, 427, 428}, {423, 424, 425},
  {240, 241, 242}, {237, 238, 239}, {234, 235, 236},
  {195, 196, 197}, {192, 193, 194}, {189, 190, 191}, 
  {186, 187, 188}, {183, 184, 185}, {180, 181, 182},
};
// describes the cycle of LEDs around the face of the cube's Back
int b_8[36][2] = {
    {6, 7},   {8, 5},   {4, 3},
    {0, 1},   {2, 9}, {14, 15},
  {10, 13}, {16, 17}, {12, 11},
  {18, 19}, {20, 21}, {22, 23},
  {24, 25}, {26, 27}, {28, 29},
  {32, 31}, {30, 35}, {34, 33},
  {74, 73}, {72, 75}, {76, 77},
  {80, 79}, {78, 71}, {66, 65},
  {70, 67}, {64, 63}, {68, 69},
  {62, 61}, {60, 59}, {58, 57},
  {56, 55}, {54, 53}, {52, 51},
  {48, 49}, {50, 45}, {46, 47}
};

// describes the cycle of LEDs around the rim of the cube's Up side
int u_12[36][3] = {
  {159, 160, 161}, {156, 157, 158}, {153, 154, 155},
  {114, 115, 116}, {111, 112, 113}, {108, 109, 110},
  {105, 106, 107}, {102, 103, 104}, {99, 100, 101},
  {26, 21, 20}, {25, 22, 19}, {18, 23, 24,},
  {17, 12, 11}, {16, 13, 10}, {15, 14, 9},
  {8, 3, 2}, {7 ,4, 1}, {0, 5, 6},
  {431, 426, 425}, {430, 427, 424}, {429, 428, 423},
  {422, 417, 416}, {421, 418, 415}, {420, 419, 414},
  {413, 408, 407}, {412, 409, 406}, {411, 410, 405},
  {402, 403, 404}, {399, 400, 401}, {396, 397, 398}, 
  {357, 358, 359}, {354, 355, 356}, {351, 352, 353}, 
  {348, 349, 350}, {345, 346, 347}, {342, 343, 344},
};
int offset_u = 162;
// describes the cycle of LEDs around the face of the cube's Up side
int u_8[36][2] = {
    {6+offset_u, 7+offset_u},   {8+offset_u, 5+offset_u},   {4+offset_u, 3+offset_u},
    {0+offset_u, 1+offset_u},   {2+offset_u, 9+offset_u}, {14+offset_u, 15+offset_u},
  {10+offset_u, 13+offset_u}, {16+offset_u, 17+offset_u}, {12+offset_u, 11+offset_u},
  {18+offset_u, 19+offset_u}, {20+offset_u, 21+offset_u}, {22+offset_u, 23+offset_u},
  {24+offset_u, 25+offset_u}, {26+offset_u, 27+offset_u}, {28+offset_u, 29+offset_u},
  {32+offset_u, 31+offset_u}, {30+offset_u, 35+offset_u}, {34+offset_u, 33+offset_u},
  {74+offset_u, 73+offset_u}, {72+offset_u, 75+offset_u}, {76+offset_u, 77+offset_u},
  {80+offset_u, 79+offset_u}, {78+offset_u, 71+offset_u}, {66+offset_u, 65+offset_u},
  {70+offset_u, 67+offset_u}, {64+offset_u, 63+offset_u}, {68+offset_u, 69+offset_u},
  {62+offset_u, 61+offset_u}, {60+offset_u, 59+offset_u}, {58+offset_u, 57+offset_u},
  {56+offset_u, 55+offset_u}, {54+offset_u, 53+offset_u}, {52+offset_u, 51+offset_u},
  {48+offset_u, 49+offset_u}, {50+offset_u, 45+offset_u}, {46+offset_u, 47+offset_u}
};

// describes the cycle of LEDs around the rim of the cube's Down side
int d_12[36][3] = {
  {324, 325, 326}, {327, 328, 329}, {330, 331, 332},
  {369, 370, 371}, {372, 373, 374}, {375, 376, 377},
  {378, 379, 380}, {381, 382, 383}, {384, 385, 386},
  {459, 464, 465}, {460, 463, 466}, {461, 462, 467},
  {468, 473, 474}, {469, 472, 475}, {470, 471, 476},
  {477, 482, 483}, {478, 481, 484}, {479, 480, 485},
  {54, 59, 60}, {55, 58, 61}, {56, 57, 62},
  {63, 68, 69}, {64, 67, 70}, {65, 66, 71},
  {72, 77, 78}, {73, 76, 79}, {74, 75, 80},
  {81, 82, 83}, {84, 85, 86}, {87, 88, 89},
  {126, 127, 128}, {129, 130, 131}, {132, 133, 134},
  {135, 136, 137}, {138, 139, 140}, {141, 142, 143},
};
int offset_d = 243;
// describes the cycle of LEDs around the face of the cube's Down side
int d_8[36][2] = {
    {6+offset_d, 7+offset_d},   {8+offset_d, 5+offset_d},   {4+offset_d, 3+offset_d},
    {0+offset_d, 1+offset_d},   {2+offset_d, 9+offset_d}, {14+offset_d, 15+offset_d},
  {10+offset_d, 13+offset_d}, {16+offset_d, 17+offset_d}, {12+offset_d, 11+offset_d},
  {18+offset_d, 19+offset_d}, {20+offset_d, 21+offset_d}, {22+offset_d, 23+offset_d},
  {24+offset_d, 25+offset_d}, {26+offset_d, 27+offset_d}, {28+offset_d, 29+offset_d},
  {32+offset_d, 31+offset_d}, {30+offset_d, 35+offset_d}, {34+offset_d, 33+offset_d},
  {74+offset_d, 73+offset_d}, {72+offset_d, 75+offset_d}, {76+offset_d, 77+offset_d},
  {80+offset_d, 79+offset_d}, {78+offset_d, 71+offset_d}, {66+offset_d, 65+offset_d},
  {70+offset_d, 67+offset_d}, {64+offset_d, 63+offset_d}, {68+offset_d, 69+offset_d},
  {62+offset_d, 61+offset_d}, {60+offset_d, 59+offset_d}, {58+offset_d, 57+offset_d},
  {56+offset_d, 55+offset_d}, {54+offset_d, 53+offset_d}, {52+offset_d, 51+offset_d},
  {48+offset_d, 49+offset_d}, {50+offset_d, 45+offset_d}, {46+offset_d, 47+offset_d}
};

// describes the cycle of LEDs around the rim of the cube's Right
int r_12[36][3] = {
  {216, 221, 222}, {217, 220, 223}, {218, 219, 224},
  {225, 230, 231}, {226, 229, 232}, {227, 228, 233},
  {234, 239, 240}, {235, 238, 241}, {236, 237, 242},
  {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
  {45, 46, 47}, {48, 49, 50}, {51, 52, 53},
  {54, 55, 56}, {57, 58, 59}, {60, 61, 62},
  {243, 244, 245}, {246, 247, 248}, {249, 250, 251},
  {288, 289, 290}, {291, 292, 293}, {294, 295, 296},
  {297, 298, 299}, {300, 301, 302}, {303, 304, 305},
  {378, 383, 384}, {379, 382, 385}, {380, 381, 386},
  {387, 392, 393}, {388, 391, 394}, {389, 390, 395},
  {396, 401, 402}, {397, 400, 403}, {398, 399, 404},
};
int offset_r = 405;
// describes the cycle of LEDs around the face of the cube's Right
int r_8[36][2] = {
    {6+offset_r, 7+offset_r},   {8+offset_r, 5+offset_r},   {4+offset_r, 3+offset_r},
    {0+offset_r, 1+offset_r},   {2+offset_r, 9+offset_r}, {14+offset_r, 15+offset_r},
  {10+offset_r, 13+offset_r}, {16+offset_r, 17+offset_r}, {12+offset_r, 11+offset_r},
  {18+offset_r, 19+offset_r}, {20+offset_r, 21+offset_r}, {22+offset_r, 23+offset_r},
  {24+offset_r, 25+offset_r}, {26+offset_r, 27+offset_r}, {28+offset_r, 29+offset_r},
  {32+offset_r, 31+offset_r}, {30+offset_r, 35+offset_r}, {34+offset_r, 33+offset_r},
  {74+offset_r, 73+offset_r}, {72+offset_r, 75+offset_r}, {76+offset_r, 77+offset_r},
  {80+offset_r, 79+offset_r}, {78+offset_r, 71+offset_r}, {66+offset_r, 65+offset_r},
  {70+offset_r, 67+offset_r}, {64+offset_r, 63+offset_r}, {68+offset_r, 69+offset_r},
  {62+offset_r, 61+offset_r}, {60+offset_r, 59+offset_r}, {58+offset_r, 57+offset_r},
  {56+offset_r, 55+offset_r}, {54+offset_r, 53+offset_r}, {52+offset_r, 51+offset_r},
  {48+offset_r, 49+offset_r}, {50+offset_r, 45+offset_r}, {46+offset_r, 47+offset_r}
};

// describes the cycle of LEDs around the rim of the cube's Left
int l_12[36][3] = {
  {344, 345, 350}, {343, 346, 349}, {342, 347, 348},
  {335, 336, 341}, {334, 337, 340}, {333, 338, 339},
  {326, 327, 332}, {325, 328, 331}, {324, 329, 330},
  {321, 322, 323}, {318, 319, 320}, {315, 316, 317},
  {276, 277, 278}, {273, 274, 275}, {270, 271, 272},
  {269, 268, 267}, {266, 265, 264}, {263, 262, 261},
  {78, 79, 80}, {75, 76, 77}, {72, 73, 74},
  {33, 34, 35}, {30, 31, 32}, {27, 28, 29},
  {24, 25, 26}, {21, 22, 23}, {18, 19, 20},
  {182, 183, 188}, {181, 184, 187}, {180, 185, 186},
  {173, 174, 179}, {172, 175, 178}, {171, 176, 177},
  {164, 165, 170}, {163, 166, 169}, {162, 167, 168},
};
int offset_l = 81;
// describes the cycle of LEDs around the face of the cube's Left
int l_8[36][2] = {
    {6+offset_l, 7+offset_l},   {8+offset_l, 5+offset_l},   {4+offset_l, 3+offset_l},
    {0+offset_l, 1+offset_l},   {2+offset_l, 9+offset_l}, {14+offset_l, 15+offset_l},
  {10+offset_l, 13+offset_l}, {16+offset_l, 17+offset_l}, {12+offset_l, 11+offset_l},
  {18+offset_l, 19+offset_l}, {20+offset_l, 21+offset_l}, {22+offset_l, 23+offset_l},
  {24+offset_l, 25+offset_l}, {26+offset_l, 27+offset_l}, {28+offset_l, 29+offset_l},
  {32+offset_l, 31+offset_l}, {30+offset_l, 35+offset_l}, {34+offset_l, 33+offset_l},
  {74+offset_l, 73+offset_l}, {72+offset_l, 75+offset_l}, {76+offset_l, 77+offset_l},
  {80+offset_l, 79+offset_l}, {78+offset_l, 71+offset_l}, {66+offset_l, 65+offset_l},
  {70+offset_l, 67+offset_l}, {64+offset_l, 63+offset_l}, {68+offset_l, 69+offset_l},
  {62+offset_l, 61+offset_l}, {60+offset_l, 59+offset_l}, {58+offset_l, 57+offset_l},
  {56+offset_l, 55+offset_l}, {54+offset_l, 53+offset_l}, {52+offset_l, 51+offset_l},
  {48+offset_l, 49+offset_l}, {50+offset_l, 45+offset_l}, {46+offset_l, 47+offset_l}
};

// What I think looks best for the cube colors on the LEDs that I have
byte green[3] = {250,10,0};
byte red[3] = {0,255,0};
byte blue[3] = {0,0,250};
byte white[3] = {235,255,210};
byte yellow[3] = {150,255,0};
byte orange[3] = {25,180,0};

// current lamp color for panel mode
byte lampColor[3] = {yellow[0], yellow[1], yellow[2]};

void setLampColor(byte color) {
  switch (color) {
    case '1':
      lampColor[0] = red[0]; lampColor[1] = red[1]; lampColor[2] = red[2];
      break;
    case '2':
      lampColor[0] = orange[0]; lampColor[1] = orange[1]; lampColor[2] = orange[2];
      break;
    case '3':
      lampColor[0] = yellow[0]; lampColor[1] = yellow[1]; lampColor[2] = yellow[2];
      break;
    case '4':
      lampColor[0] = green[0]; lampColor[1] = green[1]; lampColor[2] = green[2];
      break;
    case '5':
      lampColor[0] = blue[0]; lampColor[1] = blue[1]; lampColor[2] = blue[2];
      break;
    case '6':
      lampColor[0] = white[0]; lampColor[1] = white[1]; lampColor[2] = white[2];
      break;
  }  
}

// the basic cube's color layout
byte sides[6][3] = {
  {white[0], white[1], white[2]},    // 0 white  Back
  {green[0], green[1], green[2]},    // 1 green  Left
  {red[0], red[1], red[2]},          // 2 red    Up
  {orange[0], orange[1], orange[2]}, // 3 orange Down
  {yellow[0], yellow[1], yellow[2]}, // 4 yellow Front
  {blue[0], blue[1], blue[2]},       // 5 blue   Right
};

// reset the cube to the basic layout
void setSides() {
  int led;
  for (i = 0; i < 6; i++) {
    for (n = 0; n < 81; n++) {
      for (x = 0; x < 3; x++) {
        goal[i][n][x] = sides[i][x];
        actual[i][n][x] = sides[i][x];
      }
      
      led = (i * 81) + n;
      leds[led] = CRGB(sides[i][0], sides[i][1], sides[i][2]);
    }
  }
}

bool panels[6] = {1,1,0,1,0,0}; // which panels to illuminate in panel mode
// update the LEDs in each panel, either to 0 if the panel is off in panels[], or to the current lampColor
void setPanels() {
  for (i = 0; i < 6; i++) {
    for (n = 0; n < 81; n++) {
      for (x = 0; x < 3; x++) {
        goal[i][n][x] = (panels[i] ? lampColor[x] : 0);
        actual[i][n][x] = (panels[i] ? lampColor[x] : 0);
      }
      leds[(i*81)+n] = CRGB(
        (panels[i] ? lampColor[0] : 0),
        (panels[i] ? lampColor[1] : 0),
        (panels[i] ? lampColor[2] : 0)
      );
    }
  }
}

// turn on all panles
void blinding() {
  for (i = 0; i < 6; i++) {
    panels[i] = true;
  }
  setPanels();
}

// turn on half the panels
void underhead() {
  panels[0] = false;
  panels[1] = false;
  panels[2] = true;
  panels[3] = false;
  panels[4] = true;
  panels[5] = true;
  setPanels();
}

// turn on the other half of the panels
void overhead() {
  panels[0] = true;
  panels[1] = true;
  panels[2] = false;
  panels[3] = true;
  panels[4] = false;
  panels[5] = false;
  setPanels();
}

// twist the cube 
void turn(int face[36][2], int ring[36][3], bool cw = true) {
  int led;
  int led2;
  byte ringSide[36][3][3] = {
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},{{0,0,0},{0,0,0},{0,0,0}},
  };
  for (i = 0; i < 36; i++) {
    for (n = 0; n < 3; n++) {
      led = ring[i][n];
      for (x = 0; x < 3; x++) {
        ringSide[i][n][x] = actual[byte(floor(led / 81))][byte(led % 81)][x];
      }
    }
  }
  for (i = 0; i < 36; i++) {
    for (n = 0; n < 3; n++) {
      led = ring[(i+1 + (34 - (cw * 34))) % 36][n];
      for (x = 0; x < 3; x++) {
        goal[byte(floor(led / 81))][byte(led % 81)][x] = ringSide[i][n][x];
      }
    }
  }

  byte faceSide[36][2][3] = {
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},{{0,0,0},{0,0,0}},
  };
  for (i = 0; i < 36; i++) {
    for (n = 0; n < 2; n++) {
      led = face[i][n];
      for (x = 0; x < 3; x++) {
        faceSide[i][n][x] = actual[byte(floor(led / 81))][byte(led % 81)][x];
      }
    }
  }
  for (i = 0; i < 36; i++) {
    for (n = 0; n < 2; n++) {
      led = face[((i+1) + (34 - (cw * 34))) % 36][n];
      for (x = 0; x < 3; x++) {
        goal[byte(floor(led / 81))][byte(led % 81)][x] = faceSide[i][n][x];
      }
    }
  }
}

// get the move number of the opposite move (clockwise vs counterclockwise)
byte getReverse(byte mo) {
  if (mo % 2) {
    return mo - 1;
  } else {
    return mo + 1;
  }
}

void(* resetFunc) (void) = 0; // reboot the arduino

// limit current use, so the cube doesn't melt
void setBright(char b) {
  if (b == '0') {
    FastLED.setBrightness(1);
  } else {
    FastLED.setBrightness((int((b - '1') * 18)) + 10);
  }
}

void setup() {
  Serial.begin(9600); // connect to the jdy-31 bluetooth, using pins 0 and 1

  randomSeed(analogRead(0));
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  setBright('9');

  bool beABoringLamp = true; // if set to false, the cube will start in wander mode

  if (beABoringLamp) {
    mode = '^';
    setLampColor('3');
    overhead();
  }
}

void loop() {
  if (Serial.available() &&
      Serial.peek() != 'U' && Serial.peek() != 'u' &&
      Serial.peek() != 'D' && Serial.peek() != 'd' &&
      Serial.peek() != 'L' && Serial.peek() != 'l' &&
      Serial.peek() != 'R' && Serial.peek() != 'r' &&
      Serial.peek() != 'F' && Serial.peek() != 'f' &&
      Serial.peek() != 'B' && Serial.peek() != 'b') {
    letter = byte(Serial.read());

    if (letter == '?') {
      Serial.println("  Modes:");
      Serial.println("  ^ x v - panel mode");
      Serial.println("  w - wander mode");
      Serial.println("  c - clear mode");
      Serial.println("  p - free play");
      Serial.println("  Commands:");
      Serial.println("  n - new (reboot)");
      Serial.println("  s - solve (wander mode only)");
      Serial.println("  U D L R F B u d l r f b - twist (free play mode only)");
      Serial.println("  P - panel on/off (then enter 1-6) (panel mode only)");
      Serial.println("  h - hue (then enter 1-6) (panel mode only)");
      Serial.println("  Settings:");
      Serial.println("  t - delay between twists (then enter 0-9)");
      Serial.println("  i - brightness (then enter 0-9)");
      Serial.println("  S - speed of rotation (then enter 0-9)");
      Serial.println("");
    }
    
    if (letter == 'n') {
      resetFunc();
    }
    if (letter == '^') {
      overhead();
      inMove = 0;
      mode = '^';
    }
    if (letter == 'x') {
      blinding();
      inMove = 0;
      mode = 'x';
    }
    if (letter == 'v') {
      underhead();
      inMove = 0;
      mode = 'v';
    }
    if (letter == 'w') {
      sped = 10;
      turnDelay = 1000;
      mode = 'w';
      setSides();
      inMove = 0;
      moveIndex = 0;
      dir = false;
    }
    if (letter == 's' && mode == 'w') {
      sped = 5;
      turnDelay = 100;
      mode = 's';
      if (dir == false) {
        moveIndex--;
      }
    }
    if (letter == 'c') {
      mode = 'c';
      setSides();
      inMove = 0;
    }
    
    if (letter == '0' || letter == '1' || letter == '2' || letter == '3' || letter == '4' ||
        letter == '5' || letter == '6' || letter == '7' || letter == '8' || letter == '9') {
      if (setting == 'i') {
        setBright(letter);
      }
      if (setting == 'h') {
            setLampColor(letter);
            setPanels();
      }
      if (setting == 'P') {
            byte panel = letter - '0';
            if (panel > 0 && panel < 7) {
              panels[panel - 1] = !panels[panel -  1];
              setPanels();
            }
      }
      if (setting == 'S') {
            sped = 10 + (30 * (9 - (letter - '0')));
      }
      if (setting == 't') {
            int haf = ((((letter - '0') + 3) * ((letter - '0')) + 3));
            turnDelay = haf * haf;
      }
    }
    setting = ' ';
    if (letter == 'P') {
      setting = 'P';
    }
    if (letter == 'h' && (mode == '^' || mode == 'x' || mode == 'v')) {
      setting = 'h';
    }
    if (letter == 'S') {
      setting = 'S';
    }
    if (letter == 'i') {
      setting = 'i';
    }
    if (letter == 't') {
      setting = 't';
    }

    if (letter == 'p' && mode != 's') {
      sped = 5;
      turnDelay = 100;
      if (mode == 'x' || mode == 'v' || mode == '^') {
        setSides();
        inMove = 0;
      }
      mode = 'p';
    }

    if (Serial.available() && mode != 'p' && 
      (Serial.peek() == 'U' || Serial.peek() == 'u' ||
      Serial.peek() == 'D' || Serial.peek() == 'd' ||
      Serial.peek() == 'L' || Serial.peek() == 'l' ||
      Serial.peek() == 'R' || Serial.peek() == 'r' ||
      Serial.peek() == 'F' || Serial.peek() == 'f' ||
      Serial.peek() == 'B' || Serial.peek() == 'b')
    ) {
      Serial.read();
    }
  }
    
  if (millis() > previousBlend + sped) {
    turning = false;

    if (inMove > 0) {
      turning = true;
      previousTurn = millis();
      switch (currentMove) {
        case 0: case 1: turn(f_8, f_12, currentMove % 2); break;
        case 2: case 3: turn(b_8, b_12, currentMove % 2); break;
        case 4: case 5: turn(l_8, l_12, currentMove % 2); break;
        case 6: case 7: turn(r_8, r_12, currentMove % 2); break;
        case 8: case 9: turn(u_8, u_12, currentMove % 2); break;
        case 10: case 11: turn(d_8, d_12, currentMove % 2); break;
      }    
      for (i = 0; i < 6; i++) {
        for (n = 0; n < 81; n++) {
          for (x = 0; x < 3; x++) {
            actual[i][n][x] = goal[i][n][x];
          }
          leds[(i*81)+n] = CRGB(actual[i][n][0], actual[i][n][1], actual[i][n][2]);
        }
      }
      inMove++;
      if (inMove > 9) {
        inMove = 0;
      }
    }
    
    previousBlend = millis();
    FastLED.show();
    
    if (!turning && inMove == 0) {
      if (millis() > (previousTurn + turnDelay) && mode == 'p' &&
        (Serial.peek() == 'U' || Serial.peek() == 'u' ||
        Serial.peek() == 'D' || Serial.peek() == 'd' ||
        Serial.peek() == 'L' || Serial.peek() == 'l' ||
        Serial.peek() == 'R' || Serial.peek() == 'r' ||
        Serial.peek() == 'F' || Serial.peek() == 'f' ||
        Serial.peek() == 'B' || Serial.peek() == 'b')) {
        letter = Serial.read();

        switch (letter) {
          case 'U': inMove = 1; currentMove = 9; break;
          case 'u': inMove = 1; currentMove = 8; break;
          case 'D': inMove = 1; currentMove = 11; break;
          case 'd': inMove = 1; currentMove = 10; break;
          case 'L': inMove = 1; currentMove = 5; break;
          case 'l': inMove = 1; currentMove = 4; break;
          case 'R': inMove = 1; currentMove = 7; break;
          case 'r': inMove = 1; currentMove = 6; break;
          case 'F': inMove = 1; currentMove = 1; break;
          case 'f': inMove = 1; currentMove = 0; break;
          case 'B': inMove = 1; currentMove = 3; break;
          case 'b': inMove = 1; currentMove = 2; break;
        }
      }
      
      if (millis() > (previousTurn + turnDelay) && mode == 's') {
        chosenMove = moves[moveIndex];
        dir = true;
        switch (chosenMove) {
          case 0:
          case 1: inMove = 1; currentMove = 0 + ((chosenMove + dir) % 2); break;
          case 2:
          case 3: inMove = 1; currentMove = 2 + ((chosenMove + dir) % 2); break;
          case 4:
          case 5: inMove = 1; currentMove = 4 + ((chosenMove + dir) % 2); break;
          case 6:
          case 7: inMove = 1; currentMove = 6 + ((chosenMove + dir) % 2); break;
          case 8:
          case 9: inMove = 1; currentMove = 8 + ((chosenMove + dir) % 2); break;
          case 10:
          case 11: inMove = 1; currentMove = 10 + ((chosenMove + dir) % 2); break;
        }
        if (moveIndex > 0) {
          moveIndex--;
        } else {
          mode = 'c';
        }
      }
      
      if (millis() > (previousTurn + turnDelay) && mode == 'w') {
        
        if (dir == false) {
          // make sure not to undo the last move, because it looks lame
          // get the opposite of the last move
          noBacksies = getReverse(chosenMove);
          // choose random moves until you choose one that isn't the opposite of the last move
          do {
            chosenMove = random(0, 12);
          } while (chosenMove == noBacksies);
        } else {
          chosenMove = moves[moveIndex];
        }
        
        switch (chosenMove) {
          case 0:
          case 1:
            inMove = 1;
            currentMove = 0 + ((chosenMove + dir) % 2);
            moves[moveIndex] = chosenMove;
            break;
  
          case 2:
          case 3:
            inMove = 1;
            currentMove = 2 + ((chosenMove + dir) % 2);
            moves[moveIndex] = chosenMove;
            break;
            
          case 4:
          case 5:
            inMove = 1;
            currentMove = 4 + ((chosenMove + dir) % 2);
            moves[moveIndex] = chosenMove;
            break;
            
          case 6:
          case 7:
            inMove = 1;
            currentMove = 6 + ((chosenMove + dir) % 2);
            moves[moveIndex] = chosenMove;
            break;
            
          case 8:
          case 9:
            inMove = 1;
            currentMove = 8 + ((chosenMove + dir) % 2);
            moves[moveIndex] = chosenMove;
            break;
            
          case 10:
          case 11:
            inMove = 1;
            currentMove = 10 + ((chosenMove + dir) % 2);
            moves[moveIndex] = chosenMove;
            break;
        }
  
        if (dir == false) {
          if (moveIndex < moveSize - (random(0, 10))) {
            moveIndex++;
          } else {
            dir = true;
          }
        } else {
          if (moveIndex > random(0, 5)) {
            moveIndex--;
          } else {
            dir = false;
          }
        }
      }
    }
  }
}
