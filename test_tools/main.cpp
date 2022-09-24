/******************************************************************************
 * @file main.cpp
 * @author Erik Schmidthaler
 *
 * (C) Erik Schmidthaler (23.09.2022)
 */

#include <iostream>

#include <Base_Champion.h>

int main(int argc, char* argv[]){
    std::cout << "Hello Test" << std::endl;
    LDC::champion::Base_Champion a("GenericChamp", 15);
    std::cout << "Champion created" << std::endl;
    std::cout << a.test();
}