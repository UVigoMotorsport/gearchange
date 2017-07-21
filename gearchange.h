#include <Servo.h>
#include <EEPROM.h>

void (*reset) (void) = 0;

Servo clutch;
Servo gearchange;

int MINCHANGE = 250;
int AUTO = 0;
int WANTAUTO = 0;
int CHANGESTAGE = 0;
int BLIPTIME = 250;
unsigned long BLIPDUR = 0;
int BLIPTYPE = 0;
int CLUTCHPOS = 0;
int CLUTCHREALPOS = 0;
unsigned int OFFSET = 10;
unsigned char WRITTEN = 0;
int addr_WRITTEN = OFFSET;
unsigned short int MIN = 512;
int addr_MIN = addr_WRITTEN + sizeof(WRITTEN);
unsigned short int MAX = 1023;
int addr_MAX = addr_MIN + sizeof(MIN);
//const unsigned short int DIVNUMS[3] = {512, 256, 128};
unsigned char DIVNUM = 0;
int addr_DIVNUM = addr_MAX + sizeof(MAX);
float DIV = 0;
//unsigned short int DIV = DIVNUMS[DIVNUM];
unsigned char FLAG = 0;
unsigned long int TIMELAST = 0;
unsigned char GEARCHG = 0;
unsigned long int GEARLAST = 0;
char GEARS[9] = {'N', '1', '1', '2', '3', '4', '5', '6'};
unsigned char GEAR = 0;
unsigned char OLDGEAR = 0;
unsigned char RELEASE = 0;
unsigned short int CLUTCH = MAX;
unsigned char CLUTCHSTART = 0;
unsigned long int CLUTCHLAST = 0;
unsigned long int CONFIGLAST = 0;
int CSERVOMAX = 1000;
int addr_CSERVOMAX = addr_DIVNUM + sizeof(DIVNUM);
int CSERVODEAD = 1300;
int addr_CSERVODEAD = addr_CSERVOMAX + sizeof(CSERVOMAX);
int CSERVOMIN = 2000;
int addr_CSERVOMIN = addr_CSERVODEAD + sizeof(CSERVODEAD);
float DEADPER = 0.05;
int addr_DEADPER = addr_CSERVOMIN + sizeof(CSERVOMIN);
int CREALMIN = 100;
int addr_CREALMIN = addr_CSERVOMIN + sizeof(DEADPER);
int CREALMAX = 400;
int addr_CREALMAX = addr_CREALMIN + sizeof(CREALMIN);

int SGEARMID = 1500;
int addr_SGEARMID = addr_CREALMAX + sizeof(CREALMAX);
int SGEARHALFDOWN = 1250;
int addr_SGEARHALFDOWN = addr_SGEARMID + sizeof(SGEARMID);
int SGEARHALFUP = 1750;
int addr_SGEARHALFUP = addr_SGEARHALFDOWN + sizeof(SGEARHALFDOWN);
int SGEARUP = 2000;
int addr_SGEARUP = addr_SGEARHALFUP + sizeof(SGEARHALFUP);
int SGEARDOWN = 1000;
int addr_SGEARDOWN = addr_SGEARUP + sizeof(SGEARUP);

int SPOSGEARMID = 200;
int addr_SPOSGEARMID = addr_SGEARDOWN + sizeof(SGEARDOWN);
int SPOSGEARHALFDOWN = 250;
int addr_SPOSGEARHALFDOWN = addr_SPOSGEARMID + sizeof(SPOSGEARMID);
int SPOSGEARHALFUP = 150;
int addr_SPOSGEARHALFUP = addr_SPOSGEARHALFDOWN + sizeof(SGEARHALFDOWN);
int SPOSGEARUP = 300;
int addr_SPOSGEARUP = addr_SPOSGEARHALFUP + sizeof(SPOSGEARUP);
int SPOSGEARDOWN = 100;
int addr_SPOSGEARDOWN = addr_SPOSGEARUP + sizeof(SPOSGEARUP);

int DOWNBLIPTIME = 250;
int addr_DOWNBLIPTIME = addr_SPOSGEARDOWN + sizeof(SPOSGEARDOWN);
int UPBLIPTIME = 250;
int addr_UPBLIPTIME = addr_DOWNBLIPTIME + sizeof(DOWNBLIPTIME);
int MAX2 = 0;
int addr_MAX2 = addr_UPBLIPTIME + sizeof(UPBLIPTIME);

float EXPFACTOR = 5;
int addr_EXPFACTOR = addr_MAX2 + sizeof(MAX2);

int BADCHANGE = 0;
unsigned long CHANGESTART = 0;
int CHANGESTARTED = 0;
int CURRGEARPOS = 0;
int CHANGEPOINT = 0;
int CHANGEPOS = 0;
int FUDGEPOS = 1;
unsigned long LASTNEUTSAVE = 0;

#define CLUTCHFAKEMAX 1023
#define CLUTCHFAKEMIN 0

#define SERVOMAXMAX 2500
#define SERVOMINMIN 500

#define MAXDIV 10

#define EXPCHG 1
#define CIMINMAX 2
#define CSMINMAX 4
#define GEARUP 1
#define GEARDOWN 2
#define CHANGELIMIT 1000

#define CUT 1
#define BLIP 2
#define NOBLIP 3

#define NEUTSAVEMAX 5000

#define AUTOMAX 1
