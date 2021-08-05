#include <iostream>
#include "compiler.h"
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

int main() {
    compiler comp;
    comp.display_menu();
    return 0;
}
