
//90s cafe, warm tones, friends, extravagant laptop bezels.
//a coffee cup as big as your head.
//if arduino was bought by a printer company is this stable?

#define BYTECODES t*(t&16384?7:5)*(3-(3&t>>9)+(3&t>>8))>>(3&-t>>(t&4096?2:16))|t>>3; 
#define PRESETAMT 2

#include "synths.h"

void setup() { 
 SETUPPERS
 lamp=true;
 LAMPLIGHT
 presets[0]=coco;
 presets[1]=echo;
 //presets[2]=bytebeats;
 FILLNOISE
 DOUBLECLK
 PRESETTER(coco)
}

void loop() {} 
