#include <iostream>
#include <Magick++.h>
#include <View.hpp>
using namespace std;
using namespace Magick;

int main(){
    InitializeMagick(nullptr);
    Image image("C:/Users/jason/Documents/Code/Tucil2_13523006/assets/color.png");
    run();
    return 0;
}