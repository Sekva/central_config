#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <thread>


typedef long double Num;
struct vetor;
typedef struct vetor Vetor;


struct vetor {
    Num x;
    Num y;

    static Vetor novo(Num x1, Num x2) {
        Vetor v;
        v.x = x1;
        v.y = x2;
        return v;
    }

    Num norma() { return sqrt(pow(this->x, 2) + pow(this->y, 2)); }

    Num cubo_norma() { Num c = this-> norma(); return c * c * c; }
    Num cubo_norma_i() { return 1.0 / this->cubo_norma(); }

    inline Vetor operator+(Vetor a) {
        Vetor v;
        v.x = this->x + a.x;
        v.y = this->y + a.y;
        return v;
    }

    inline Vetor operator-(Vetor a) {
        Vetor v;
        v.x = this->x - a.x;
        v.y = this->y - a.y;
        return v;
    }

    void escalar(Num l) { this->x *= l; this->y *= l; }

    void print() { std::cout << this->x << ", " << this->y << "\n"; }

};



Num N = 10;
std::vector<Vetor> pontos;



#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const float scl = 1.3;

//Screen dimension constants
const int SCREEN_WIDTH = 640 * scl;
const int SCREEN_HEIGHT = 480 * scl;

const int cx = SCREEN_WIDTH / 2;
const int cy = SCREEN_HEIGHT / 2;

bool aberto = true;
bool texto = true;


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;



void draw_circle(int x, int y, int radius, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}


void graficos() {

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("font.ttf", 11);

    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }

    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
        exit(0);
    }

    window = SDL_CreateWindow( "Server", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if ( window == nullptr ) {
         std::cout << "Failed to create window : " << SDL_GetError();
        exit(0);
     }

    renderer = SDL_CreateRenderer( window, -1, 0 );

    if ( renderer == nullptr ) {
        std::cout << "Failed to create renderer : " << SDL_GetError();
        exit(0);
    }

    SDL_RenderSetLogicalSize( renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );

    bool loop = true;
    Num scl2 = 8;

    while(loop) {

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) { aberto = loop = false; }
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_RETURN]) {
            texto = !texto;
            SDL_Delay(50);
        }

        if (state[SDL_SCANCODE_KP_PLUS]) {
            scl2 += 0.1;
        }

        if (state[SDL_SCANCODE_KP_MINUS]) {
            scl2 -= 0.1;
            if (scl2 < 0) scl2 = 0;
        }




        if (state[SDL_SCANCODE_1]) {
            std::cout << "\n";
            for(Num i = 0; i < N; i++) {
                Num n = (int)(i+1) % (int)N ;
                std:: cout << "de " << i << " ate " << n << " ";
                std:: cout << (pontos[i] - pontos[n]).norma() << "\n";
            }
            std::cout << "\n";
        }



        SDL_RenderClear(renderer);

        draw_circle(cx, cy, 2, 0, 0xff, 0x32, 1);
        SDL_SetRenderDrawColor(renderer, 0, 0x12, 0x12, 1);
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

        for(int i = 0; i < pontos.size(); i++) {
            if(texto) {

                std::string score_text = " A(" + std::to_string(i) + ") = " + std::to_string(pontos[i].x) + ", " + std::to_string(pontos[i].y);
                SDL_Color textColor = { 255, 255, 255, 0 };
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, score_text.c_str(), textColor);
                SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
                int text_width = textSurface->w;
                int text_height = textSurface->h;
                SDL_FreeSurface(textSurface);
                SDL_Rect renderQuad = { scl2 * pontos[i].x + cx, -scl2 * pontos[i].y + cy, text_width, text_height };
                SDL_RenderCopy(renderer, text, NULL, &renderQuad);
                SDL_DestroyTexture(text);

            }

            draw_circle( scl2 * pontos[i].x + cx, -scl2 * pontos[i].y + cy, 3, 0, 0x32, 0xff, 0);
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
        SDL_RenderPresent( renderer);
        SDL_Delay(16);
    }


    SDL_DestroyWindow( window );
    SDL_Quit();
}






int main() {


    for(uint8_t i = 0; i < N; i++) {

        // Aleatorio
        std::mt19937_64 rng;
        uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
        rng.seed(ss);
        std::uniform_real_distribution<double> unif(-20, 20);

        Num r1 = unif(rng);
        // consumir um tempo
        for(int j = 0; j < 17; j++) {}
        Num r2 = unif(rng);

        pontos.push_back(Vetor::novo(r1, r2));

    }

    std::thread grafico(graficos);

    SDL_Delay(500);


    Num maior;
    uint64_t it = 10000000;
    std::cout << "\n";
    std::cout << "\n";
    std::cout << it << "\n";
    for(uint64_t i = 0; i < it && aberto; i++) {
        std::cout << "\r" << i;

        for(Num j = 0; j < N; j++) {
            for(Num k = 0; k < N; k++) {
                if(k == j) { continue; }
                Vetor vv = pontos[j] - pontos[k];
                Num scl = vv.cubo_norma_i();
                vv.escalar(scl);

                pontos[j] = pontos[j] + vv;
            }
        }


        //for(int j = 0; j < N; j++) {
        //    if(pontos[j].norma() > 2000) {
        //        for(int n  = 0; n < N; n++) {
        //            pontos[n].escalar(0.995);
        //        }
        //        break;
        //    }
        //}

        //SDL_Delay(1);

    }

    std::cout << "\n";
    for(Num i = 0; i < N; i++) {

        Num n = (int)(i+1) % (int)N ;

        std:: cout << "de " << i << " ate " << n << " ";
        std:: cout << (pontos[i] - pontos[n]).norma() << "\n";
    }


    for(Num i = 0; i < N; i++) {
        pontos[i].print();
    }

    grafico.join();

    return 0;
}
