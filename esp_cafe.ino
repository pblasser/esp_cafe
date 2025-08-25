

#include "synths.h"

void setup() { 
 SETUPPERS
 lamp=true;
 LAMPALITY
 presets[0]=coco;
 presets[1]=echo;
 FILLNOISE
 DOUBLECLK
 PRESETTER(coco)
}

void loop() {} 
