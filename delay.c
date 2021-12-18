#include "delay.h"

void atrasar(unsigned char delay){
    unsigned char i, j, k, t;
    
    //unsigned char tempMs = 25;
    /*
    for(i = 0; i < 100; i++){
        for(j = 0; j < 41; j++){
            for(k = 0; k < 3; k++);
        }
    }
    */
    
    for(t = 0; t < delay; t++){
        for(i = 0; i < 100; i++){
            for(j = 0; j < 41; j++){
                for(k = 0; k < 3; k++);
            }
        }
    }
    
}
