
#include <window.hpp>

#include <iostream>
#include <string>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Circle"

int main(int argc, char* argv[]){

  Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, (char*) WINDOW_TITLE);
  window.run();



}
