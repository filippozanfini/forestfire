// Developed by Filippo Zanfini
// filippozanfini.com

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <math.h>

using namespace std;

#define N 800
#define M 800

enum states {EMPTY = 0, TREE = 1, FIRE = 2};

float probRand() {
    return (float) rand() / RAND_MAX;
}

bool withinGrid(int row, int col) {
    if((row < 0) || (col<0))
        return false;
    if((row >= N) || (col >= M))
        return false;

    return true;
}

bool getNeighbors(int x, int y, int** forest) {
    for(int i=x-1; i<=(x+1); i++) {
        for(int j=y-1; j<=(y+1); j++) {
            if(!(i==x && j==y)) {
                if(withinGrid(i, j) && forest[i][j] == FIRE)
                    return true;
            }
        }
    }

    return false;
}

void print(int** forest) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for(int i=0; i<N; i++) {
        for(int j=0; j<M; j++) {
            switch(forest[i][j]) {
                case EMPTY:
                    al_draw_filled_rectangle(i*5, j*5, i*5+5, j*5+5, al_map_rgb(0,0,0));
                    break;

                case TREE:
                    al_draw_filled_rectangle(i*5, j*5, i*5+5, j*5+5, al_map_rgb(34,139,34));
                    break;

                case FIRE:
                    al_draw_filled_rectangle(i*5, j*5, i*5+5, j*5+5, al_map_rgb(255,140,0));
                    break;
            }
        }
    }

    al_flip_display();
    al_rest(0.2);
}

void game(int** forest, float p, float f) {
    for(int i=0; i<N; i++) {
        for(int j=0; j<M; j++) {
            switch(forest[i][j]) {
                case EMPTY:
                    if(probRand() < p)
                        forest[i][j] = TREE;
                    break;

                case TREE:
                    if(getNeighbors(i, j, forest) || probRand() < f)
                        forest[i][j] = FIRE;
                    break;
                
                case FIRE: 
                    forest[i][j] = EMPTY;
                    break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int** forest = new int*[N];
    float p = 0.05f, f=0.005f;

    // Matrix init
    for(int i=0; i<N; i++)
        forest[i] = new int[M];

    for(int i=0; i<N; i++)
        for(int j=0; j<M; j++)
            forest[i][j] = EMPTY;

    // Allegro setup
    ALLEGRO_DISPLAY* display;
    const int displayHeight = N;
    const int displayWidth = M;

    al_init();
    display = al_create_display(displayWidth, displayHeight);
    al_init_primitives_addon();

    // Start
    while(true) {
        print(forest);
        game(forest, p, f);
    }
    
    return 0;
}