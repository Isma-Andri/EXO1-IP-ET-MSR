#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int main() {
// preparation du terminal à utiliser ncurses
    initscr();        
    cbreak();     // pas de buffering des inputs    
    noecho();  
          
  	curs_set(0);    // desactiver le curseur
    mainMenu();       

// termine ncurses
    endwin();         
    return 0;
}
