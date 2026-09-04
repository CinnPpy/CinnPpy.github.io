#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define LEFT 'a'
#define RIGHT 'd'
#define DOWN 's'
#define UP 'w'
#define CONFIRM '\n'

// reference used for time library: https://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html
// reference used for ncurses functions: https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
// reference used for other functions (stdio, string): https://www.w3schools.com/c/c_ref_stdio.php

void drawArrow(int y, int dir, int pallette){
	if (dir == 1) {
		mvaddch(y - 2, 24, '.');
		attron(COLOR_PAIR(pallette));
		mvaddch(y - 1, 24, '^');
		mvaddch(y, 24, '|');
		mvaddch(y + 1, 24, '|');
	} else if (dir == 2) {
		mvaddch(y - 2, 12, '.');
		mvprintw(y - 1, 10, "     ");
		attron(COLOR_PAIR(pallette));
        	mvprintw(y, 10, "<----");
	} else if (dir == 3) {
		mvaddch(y - 2, 36, '.');
		attron(COLOR_PAIR(pallette));
		mvaddch(y - 1, 36, '|');
		mvaddch(y, 36, '|');
		mvaddch(y + 1, 36, 'v');
	} else if (dir == 4) {
		mvaddch(y - 2, 48, '.');
		mvprintw(y - 1, 46, "     ");
		attron(COLOR_PAIR(pallette));
        	mvprintw(y, 46, "---->");
	}
	attroff(COLOR_PAIR(pallette));
}
void drawBox(int pallette){
	attron(COLOR_PAIR(pallette));
	mvprintw(46, 54, "+---+");
	mvprintw(47, 54, "|   |");
	mvprintw(48, 54, "+---+");
	attroff(COLOR_PAIR(pallette));
}


int run_game(char path[]) {

	FILE *ptr;
	printw(path);
        ptr = fopen(path, "rb");
        if (ptr == NULL) {
		printw("Failed to open file!");
		return -1;
	}

	//init functions
	curs_set(0);
	raw();
	noecho();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	start_color();

	const int header_size = 8; // defines the number of bytes for the header of the file

	fseek (ptr, 0, SEEK_END);
        int flength = ftell(ptr) - header_size;
        uint8_t header[header_size];
        uint8_t space;
        uint8_t speed;
        uint8_t song[flength];
        fseek (ptr, 0, SEEK_SET);
        fread(header, header_size, 1, ptr);
        fread(song, flength, 1, ptr);
        if (!header[0] || !header[1]) {
		return -20;
	}
	speed = header[0];
        space = header[1];
	
	// variable initilization
	int score = 0;
	int i;
	int io = 0; // i offest
	int loc;
	int penalty; // point penalty for holding direction
	int ch; // current typed character
	int xdir; // previous frame direction
	int pdir = 0; // stores direction the player is pressing
	unsigned int time = 0; // frame number; overflows eventually, but only after running for over 2 years
	int start_time; // clock time at beginning of the frame

	// pallettes
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	
	// draw lines
	mvvline(5, 12, '.', 40);
	mvvline(5, 24, '.', 40);
	mvvline(5, 36, '.', 40);
	mvvline(5, 48, '.', 40);
	
	while ((ch = getch()) != '\032') { // main loop, ends at ctrl+z
	        start_time = clock();

		// check for inputs
		if (ch != ERR) {
			if (ch == UP) {
                	       	pdir = 1;
                	} else if (ch == LEFT) {
                        	pdir = 2;
                	} else if (ch == DOWN) {
                	        pdir = 3;
                	} else if (ch == RIGHT) {
                	        pdir = 4;
                	} else {
				pdir = 0;
			}
		}
		time++;
		
		// draw stationary arrows
		drawArrow(47, 3, 1);
		drawArrow(47, 1, 1);
		drawArrow(47, 4, 1);
		drawArrow(47, 2, 1);
		drawArrow(47, pdir, 2);
		drawBox((!pdir) * 2);
		refresh();

		// calculate hold penalty
		if (pdir == xdir) {
			if (penalty < 500) {
			penalty++;
			}
		} else{
			penalty -= 50;
			if (penalty < 5) {
				penalty = 5;
			}
		}
		if (pdir == 0) {
			penalty = 5;
		}
		xdir = pdir;
		// draw falling arrows
		i = io;
		loc = (time / speed) - (i * space);
        	while(loc > 8){
			mvprintw(0, 0, "%d, %d", loc, i);
			i++;
			loc = (time / speed) - (i * space);
			drawArrow(loc, (0x07 & song[i]), 1);
		
			if (loc == 47) {
			io += 1;
				// score calculation
				if (song[i] != 0 & song[i] < 5){
					if (pdir == song[i]){
						score += (500 / (penalty / 5));
					}
					else{
						score -= 100;
					}
				}
			}
			if (i > flength) {
				nodelay(stdscr, FALSE);
				if (score < 0){
					score = 0;
				}
				return score;
			}
		}
		
		
		drawArrow(47, pdir, 2);
		// HUD
		mvprintw(43, 54, "%d", time / 60);
		mvprintw(44, 54, "score: %d", score);
		if (penalty > 5) {
		attron(COLOR_PAIR(3));
		}
		
		mvprintw(47, 55, "%d", penalty - 5);
		attroff(COLOR_PAIR(3));
		refresh();

		usleep(16666);
	}
	nodelay(stdscr, FALSE);
	return -1;
}

int main () {
	int row = 0;
	int col = 0;
	initscr();
	noecho();
	keypad(stdscr, TRUE);

	FILE* charts = fopen("./charts/charts.conf", "r"); // opens config file which lists the avaliable songs. It is basically a list of filenames in the same directory seporated by newline characters
        if (charts == NULL) {
                mvprintw(0, col, "There was an error opening 'charts.conf'. no charts will be loaded");
		return 5;
	}
	char input;
	printw("Press %c and %c to navigate the menu\n%c, %c, %c, and %c correspond to up, left, right, and down in game. \npress any key to begin", LEFT, RIGHT, UP, LEFT, RIGHT, DOWN);
	while((input = getch()) != 'q'){
	        //attron(COLOR_PAIR(5));
		clear();
		getmaxyx(stdscr, row, col);

		// draw title
        	// ASCII art text generated from: https://patorjk.com/software/taag/#p=display&f=AMC%203%20Line&t=
        	mvprintw(1, col / 2 - 11, ".-. . . .-.   .-. .-. .  . .-. .-.");
        	mvprintw(2, col / 2 - 11, " |  | |  |     |  |-  |\\/| |-' | | ");
        	mvprintw(3, col / 2 - 11, " '  `-' `-'    '  `-' '  ` '   `-'");
		
		// main menu selection
        	char buffer[30];
        	if (buffer == NULL) {
        	        rewind(charts);
		}
	        if (input == RIGHT) {
                	fgets(buffer, 30, charts);
        	}
		if (input == LEFT){
			fseek(charts, -strlen(buffer), SEEK_CUR); // go to end of previous line
			char char_buf = 'a'; // buffer set to 'a' arbitrarily so that it is never initialized with value of newline
			while (!(char_buf == '\n' || ftell(charts) < 2)){ // reverses to beginning of line in the file; other implementations do not allow for dynamic file lenghts
				fseek(charts, -2, SEEK_CUR);
				char_buf = fgetc(charts);
			} 
			if (ftell(charts) < 2){ // handles first item in file
				rewind(charts);
			}
			fgets(buffer, 30, charts);
		}
		
		if (input == CONFIRM){
			char chart_path[40];
			sprintf(chart_path, "./charts/%s", buffer);
			chart_path[strlen(chart_path) - 1] = '\0'; // removes the newline character at the end of the string which causes file to not be read
			int score;
			if ((score = run_game(chart_path)) >= 0) { // the game is started; if it returns a negative value, some error occurred
				clear();
				mvprintw(10, 10, "Good Job! Your score was: %d", score);
			} else {
				clear();
				mvprintw(10, 10, "There Was An error!");
			}
		}
		// draw menu; title is drawn earlier
		mvprintw(row / 2, col / 2 - 5, "%s", buffer);
		mvaddch(row / 2, col / 3, '<');
		addch(LEFT);
		mvaddch(row / 2, col * 2 / 3, RIGHT);
		addch('>');
		refresh();
	}
	endwin();
	fclose(charts);
	return 0;

}

	

