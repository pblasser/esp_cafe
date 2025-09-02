

#include "synths.h"

void setup() { 
 SETUPPERS
 lamp=true;
 LAMPLIGHT
 presets[0]=coco;
 presets[1]=echo;
 presets[2]=bytebeats;
 FILLNOISE
 DOUBLECLK
 PRESETTER(coco)
}

void loop() {} 
