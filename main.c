// #include <iostream>

#include <SDL2/SDL.h> 
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// clang++ main.c -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -I/Library/Frameworks/SDL2_image.framework/Headers  -framework SDL2_image -I/Library/Frameworks/SDL2_mixer.framework/Headers -framework SDL2_mixer -I/Library/Frameworks/SDL2_ttf.framework/Headers -framework SDL2_ttf -o Cscape 

int clamp(int x,int min, int max) {
    if (x<min) {
        return min;
    }
    else if (x>max)
    {
        return max;
    }
    else {
        return x;
    }
    
}

void SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }

        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }

        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

int main(int argc, char* argv[]) {
    const int DESIRED_FPS = 60;
    const int FRAME_TARGET_TIME = 1000 / DESIRED_FPS;

    SDL_Window* window=nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        // std::cout << "no worky :( " <<
                  SDL_GetError();
     } 
    // else{
    //     std::cout << "everything works!\n";
    // }

    int playerY = 200;
    int playerVelocity = 0;

    TTF_Init();

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("Cscape",20, 20, 400,400,SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    // variables !!
    bool gameIsRunning = true;

    int lastFrameTime, currentFrameTime, deltaTime;

    bool inputMap[2] = {false,false}; // Tap,Esc

    Mix_Chunk* jump = Mix_LoadWAV("/assets/jump.mp3");
    Mix_Chunk* soundtrack = Mix_LoadWAV("/assets/soundtrack.wav");
    Mix_Chunk* start = Mix_LoadWAV("/assets/start.mp3");

    SDL_Rect topOb;
    topOb.x = 400;

    int obstacle = rand() % 100;

    int edgeCol = 255;
    bool pulseDown = false;
    int obSpeed = 5;

    bool isTitle = true;
    bool music = false;

    Mix_PlayChannel(-1, start, 0);

    SDL_Surface *game;
    SDL_Surface *title;
    title = IMG_Load("/assets/title.png");
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, title);

    int points = 0;

    int n; // for loop friend i love you n

    int amtLines = 500;
    int linePos[amtLines*2];

    TTF_Font* font = TTF_OpenFont("/assets/silkscreen.ttf", 30);
    SDL_Rect textLocation = {183,2,100,35}; 

    int bgColour[3] = {0,0,0};

    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};

    char pointsDisplay[2];

    while(gameIsRunning) {
        Uint32 frameStart = SDL_GetTicks();
        
        SDL_Event event;

        lastFrameTime = currentFrameTime;
        currentFrameTime = SDL_GetTicks();
        deltaTime = currentFrameTime - lastFrameTime;

        // check events
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                gameIsRunning= false;
            } if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    inputMap[0] = true;
                    if (isTitle == true) {
                        Mix_PlayChannel(-1, soundtrack, -1);
                    }
                    isTitle = false;
                    Mix_PlayChannel(-1, jump, 0);
                    playerVelocity = 30;
                    break;
            }
            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    inputMap[0] = false;
                    break;
                }
            }
        }
        }
        sprintf(pointsDisplay,"%i",points);
        SDL_Surface *text = TTF_RenderText_Shaded(font,pointsDisplay, white, black);

        if (isTitle == false) {
        // handle gravity
        playerVelocity -= 1;
        playerY -= playerVelocity/4;

        // handle speed change
        obSpeed = round((points+50)/10);
        
        // handle hitboxes
        if (!(0 < playerY && playerY < 400)) {
            playerY = 200;
            playerVelocity = 4;
            topOb.x = 500;
            obstacle = rand() % 350;
            points = 0;
            isTitle = true;
            Mix_HaltChannel(-1);
        }

        if (topOb.x > -75) {
            topOb.x -= obSpeed;
        } else { 
            topOb.x = 400;
            obstacle = rand() % 350;
            points += 1;
            bgColour[rand()%3] = 30;
        }
        topOb.y = obstacle;
        topOb.w = 75;
        topOb.h = 75;

        if ((playerY > obstacle && playerY < obstacle+75) && (topOb.x < 50 && topOb.x+75 > 50)) {
            playerY = 200;
            playerVelocity = 4;
            topOb.x = 500;
            obstacle = rand() % 350;
            points = 0;
            Mix_HaltChannel(-1);
            isTitle = true;
        }

        // limit gravity
        playerVelocity = clamp(playerVelocity, -30,30);
        }
        // draw stuff 
        SDL_SetRenderDrawColor(renderer,bgColour[0],bgColour[1],bgColour[2],SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        
        // pulsing edges
        if (edgeCol >= 255) {
            pulseDown = true;
        } else if (edgeCol <= 0) {
            pulseDown = false;
        }

        if (pulseDown == true) {
            edgeCol -= 5;
        } else {
            edgeCol += 5;
        }

        for (n = 0; n < 3; n++) {
            if (bgColour[n] > 0) {
                bgColour[n] -= 5;
            }
        }

        if (points < 10) {
            textLocation.w = 25;
        } else {
            textLocation.w = 50;
            textLocation.x = 175;
        }

        SDL_SetRenderDrawColor(renderer,edgeCol-edgeCol/5,edgeCol,edgeCol-(rand()%255),SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer,0,1,400,1);
        SDL_RenderDrawLine(renderer,0,399,400,399);

        // draw bg lines!!
        for (n = 0; n < amtLines+1; n += 2) {
            linePos[n] -= round(obSpeed/1.5);
            SDL_RenderDrawLine(renderer,linePos[n],linePos[n+1],linePos[n]+30,linePos[n+1]);
            if (linePos[n] < -25) {
                linePos[n] = 400+rand()%400;
                linePos[n+1] = rand()%300 +50;
            }
        }

        SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);

        SDL_RenderFillCircle(renderer,50,playerY,15);

        // Draw the obstacle!!
        SDL_SetRenderDrawColor(renderer,clamp(rand()%255,100,255),clamp(rand()%255,100,255),clamp(rand()%255,100,255),SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer,&topOb);

        SDL_RenderCopy(renderer,SDL_CreateTextureFromSurface(renderer,text),NULL,&textLocation);

        if (isTitle == true) {SDL_RenderCopy(renderer,imageTexture,NULL,NULL);}

        SDL_RenderPresent(renderer);
        
        Uint32 frameTime = SDL_GetTicks() - frameStart;\

        if (FRAME_TARGET_TIME > frameTime) {
        SDL_Delay(FRAME_TARGET_TIME - frameTime);
        }
    }

    SDL_DestroyWindow(window);
        
    SDL_Quit();
    return 0;
}