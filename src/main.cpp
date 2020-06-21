#include "ncurses_display.h"
#include "system.h"

int main() {
  System system; //Creating object of class system 
  NCursesDisplay::Display(system);	//Passing the system object to NCursesDisplay
}