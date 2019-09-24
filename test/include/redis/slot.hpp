#ifndef __SLOT_HPP_
#define __SLOT_HPP_
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<climits>

unsigned char slot[16384/8];

void setSlot(int num){
    slot[num/8] |= (unsigned char)(1<<num%8);
}

void _print(){
    for(int i = 0; i < 16384/8; ++i){
        if(slot[i] != 0){
            printf("slot[%d]:%x ", i, slot[i]);
        }
    }
    printf("\n");
    fflush(stdout);
}

void initSlot(){
    memset(slot, 0, sizeof(slot));
}

void max_int(){
    std::cout << "INT_MAX:" << INT_MAX << std::endl;
    std::cout << "LONG_MAX:" << LONG_MAX << std::endl;
    unsigned long end;
    std::cout << "--" <<  end << std::endl;
}

void slot_main(){
    _print();
    initSlot();
    _print();
    setSlot(0);
    setSlot(1);
    setSlot(8);
    setSlot(16);
    setSlot(5000);
    setSlot(5001);
    setSlot(5002);
    setSlot(5003);
    setSlot(5004);
    setSlot(5005);
    setSlot(5006);
    setSlot(5007);
    setSlot(1024);
    _print();
}

#endif