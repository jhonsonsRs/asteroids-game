#include <SDL2/SDL.h>
#include <list>
#include <random>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "math-vector.h"
using Vector = Mylib::Math::Vector<float, 2>;

const int widthWindow = 800;
const int heightWindow = 600;
const int widthPlayer = 50;
const int heightPlayer = 50;
const int widthAsteroid = 70;
const int heightAsteroid = 70;
bool playerDead = false;
int score;

// A - element 1
// B - element 2

bool collision(float Ax, float Ay, float Awidth, float Aheight, float Bx, float By, float Bwidth, float Bheight){
    if( Ay+ Aheight < By) return false;
    else if(Ay > By + Bheight) return false;
    else if(Ax + Awidth < Bx) return false;
    else if(Ax > Bx + Bwidth) return false;

    return true;    // collision occurred
}


float getTime(){
    return static_cast<float>(SDL_GetTicks())/1000.0;
}

class AsteroidsClass{
private:
    Vector position;
    Vector velocity;
    float size;
    float speed;
    SDL_Rect rect;
public:
    AsteroidsClass(const float x, const float y, const float size): position(x, y), speed(50), size(size){
        rect = { static_cast<int>(position.x), static_cast<int>(position.y), widthAsteroid, heightAsteroid};
        
        velocity.y = -speed;
        velocity.x = -speed;
    }

    void move(float dt){
        position += velocity * dt;
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        

        if(position.y < 0){
            velocity.y = -velocity.y;
            return;
        }

        if((position.y + size) >= heightWindow){
            velocity.y = -velocity.y;
            return;
        }

        if(position.x < 0){
            velocity.x = -velocity.x;
        }

        if(position.x + size > widthWindow){
            velocity.x = -velocity.x;
        }
    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    Vector getP() const {
        return position;
    }

    SDL_Rect getRect() const {
        return rect;
    }

    void setPosition(float x, float y){
        position.x = x;
        position.y = y;
    }
    

};


class ProjectileClass{
private:
    Vector position;
    Vector velocity;
    float speed;
    SDL_Rect rect;
    //Mix_Chunk* asteroidDestroy;
public:
    ProjectileClass(const float x, const float y): position(x, y), speed(100) {
        rect = { static_cast<int>(position.x), static_cast<int>(position.y), 10, 10};

        velocity.y = -speed;
    }

    Vector getPosition() const {
        return position;
    }

    void movement(float dt, std::list<AsteroidsClass>& asteroids, std::list<ProjectileClass>& projectiles){
        position += velocity * dt;
        rect.y = static_cast<int>(position.y);
        rect.x = static_cast<int>(position.x);
        

        for(auto ast_it = asteroids.begin(); ast_it != asteroids.end();){
            Vector position_o = ast_it->getP();
            SDL_Rect rectangle = ast_it->getRect();
            
            bool asteroidDestroyed = false;
            
            for(auto proj_it = projectiles.begin(); proj_it != projectiles.end();){
                Vector position_proj = proj_it->getPosition();

                if(collision(position_proj.x, position_proj.y, 10, 10, position_o.x, position_o.y, widthAsteroid, heightAsteroid) == true){
                    std::cout << "hit" << std::endl;

                    proj_it = projectiles.erase(proj_it);
                    asteroidDestroyed = true;


                    break;
                } else {
                    ++proj_it;
                }
            }

            if(asteroidDestroyed == true){
                ast_it = asteroids.erase(ast_it);
                //Mix_PlayChannel(-1, asteroidDestroy, 0);
                score++;
                std::cout << score << std::endl;
            } else {
                ++ast_it;
            }
        }
    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

};

class Player{
private:
    Vector position;
    Vector velocity;
    //Mix_Chunk* projectileSound;
    float speed;
    SDL_Rect rect;
    SDL_Renderer* renderer;

public:
    Player(const float x, const float y) : position(x, y), speed(100) {
        rect = { static_cast<int>(position.x), static_cast<int>(position.y), widthPlayer, heightPlayer};
    }

    bool spacePressed;

    void handleEvents(float dt, std::list<ProjectileClass>& projectiles, SDL_Event& event){
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_UP:
                    velocity.y = -speed;
                    break; 
                case SDLK_DOWN:
                    velocity.y = speed;
                    break;
                case SDLK_LEFT:
                    velocity.x = -speed;
                    break;
                case SDLK_RIGHT:
                    velocity.x = speed;
                    break;
                case SDLK_SPACE:
                    std::cout << "created" << std::endl;
                    projectiles.push_back(ProjectileClass(position.x + 20, position.y));
                    break;
            }
        }

        if(event.type == SDL_KEYUP){
            switch (event.key.keysym.sym){
                case SDLK_UP:
                case SDLK_DOWN:
                    velocity.y = 0;
                    break;
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    velocity.x = 0;
                    break;
            }
        }

        /*if(keys[SDL_SCANCODE_UP]){
            velocity.y = -speed;
        } else if(keys[SDL_SCANCODE_DOWN]) {
            velocity.y = speed;
        } else {
            velocity.y = 0;
        }
    
        if(keys[SDL_SCANCODE_LEFT]){
            velocity.x = -speed;
        } else if(keys[SDL_SCANCODE_RIGHT]){
            velocity.x = speed;
        } else {
            velocity.x = 0;
        }
    
        // Check if the key was pressed and not yet released
            if(keys[SDL_SCANCODE_SPACE] && !spacePressed){
                std::cout << "created" << std::endl;
                projectiles.push_back(ProjectileClass(position.x + 20, position.y));
                spacePressed = true;
                //Mix_PlayChannel(-1, projectileSound, 0);
            }

            if(!keys[SDL_SCANCODE_SPACE]){
                spacePressed = false;
            }
        */

        position += velocity * dt;
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
    }

    Vector position_player() const {
        return position;
    }

    void setPosition(float x, float y){
        position.x = x;
        position.y = y;
    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
};



class Game{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Mix_Chunk* projectileSound;
    Mix_Chunk* asteroidDestroy;
    Mix_Music* asteroidTheme;
    std::list<AsteroidsClass> asteroids;
    std::list<ProjectileClass> projectiles;
    bool isRunning;
    Player* player;
    SDL_Event event;
public: 
    Game() : window(nullptr), renderer(nullptr), font(nullptr), isRunning(false), player(nullptr) {}

    bool initialize(const char* title, int width, int height){
        if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() == -1) {  
            std::cerr << "Failed to initialize SDL2: " <<   SDL_GetError() << std::endl;
            return -1; // exit program
        }

        window = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_CENTERED,   
                                SDL_WINDOWPOS_CENTERED,   
                                width, height,    
                                SDL_WINDOW_SHOWN);  

        if (!window) {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return -1;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) { 
            std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return false;
        }

        if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
            std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {  // 22050: sample rate (samples per second). 2: audio channels (stereo).
                                                                   // 4096: audio buffer size (bytes kept in memory before playback).
            std::cerr << "Failed to open audio: " << Mix_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            Mix_Quit();
            SDL_Quit();
        } else { 
            std::cout << "Audio initialized successfully!" << std::endl;
        }

        asteroidDestroy = Mix_LoadWAV("asteroidsDestroyed.wav");
        if (!asteroidDestroy) {
            std::cerr << "Collision sound failed to load: " << Mix_GetError() << std::endl;
        }

        projectileSound = Mix_LoadWAV("projectileSound.wav");
        if (!projectileSound) {
            std::cerr << "Projectile sound failed to load: " << Mix_GetError() << std::endl;
        }

        asteroidTheme = Mix_LoadMUS("asteroidsMusicTheme.mp3");
        if(!asteroidTheme){
            std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
            return false;
        }

        Mix_PlayMusic(asteroidTheme, -1);

        font = TTF_OpenFont("arial.ttf", 24);
        if(!font){
            std::cout << "Failed to create font" << std::endl;
        }

        player = new Player(width / 2, height / 2);
        
        isRunning = true;
        return true;
     }

    void renderText(SDL_Renderer* renderer, TTF_Font* font, int score, int x, int y){
        SDL_Color textColor = {255, 255, 255, 255}; // text color
        std::string scoreText = "Score: " + std::to_string(score); // score string for on-screen display

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor); // render text to SDL surface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); // surface to texture for rendering

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h}; // position and text size
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // draw texture to screen

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
     }

    void game_reset(){
        player->setPosition(widthWindow / 2, heightWindow / 2);

        for(auto& ast : asteroids){
            for(int i = 0; i <= 5; i++){
            ast.setPosition( 70 + (rand() % (widthWindow - (70 * 2))), 70 + (rand() % (heightWindow - (70 * 2))));
            }
        }

        score = 0;

        std::cout << "game reset" << std::endl;
    }

    void run(){
    

        float previousTime = getTime();
        while(isRunning){
            float currentTime = getTime();
            float dt = currentTime - previousTime;
            previousTime = currentTime;
            while(asteroids.empty()){
                for(int i = 0; i <= 5; i++){
                    asteroids.push_back(AsteroidsClass( 80 + (rand() % (widthWindow - (80 * 2))), 80 + (rand() % (heightWindow - (80 * 2))), 70));
                }
            }
            handleEvents(dt);
            update(dt);
            render(dt);
            SDL_Delay(30);
        }
    }

    void render(float dt){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
        SDL_RenderClear(renderer);

        renderText(renderer, font, score, 50, 50);

        player->render(renderer);
        
        for(auto& projectile : projectiles){
            projectile.render(renderer);
        }

        for(auto& ast : asteroids){
            ast.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    void  update(float dt){

    }

    void handleEvents(float dt){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                isRunning = false;
            }

            player->handleEvents(dt, projectiles, event);

        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        
        
        for(auto& projectile : projectiles){
            projectile.movement(dt, asteroids, projectiles);
        }

        

        for(auto& ast : asteroids){

            ast.move(dt);
            
            Vector position_p = player->position_player();
            Vector position_o = ast.getP();

            if(collision(position_p.x, position_p.y, widthPlayer, heightPlayer, position_o.x, position_o.y, widthAsteroid, heightAsteroid) == true){
                playerDead = true;
                std::cout << "hit" << std::endl;
                game_reset();
            }
        }
        
    }

    void clean(){
        delete player;
        TTF_CloseFont(font);
        Mix_FreeChunk(projectileSound);
        Mix_FreeChunk(asteroidDestroy);
        Mix_FreeMusic(asteroidTheme); 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    ~Game(){
        clean();
    }

};

int main(int argc, char* argv[]){
    Game game;

    srand(time(0));
    if(!game.initialize("Asteroids", widthWindow, heightWindow)){
        return -1;
    }

    game.run();
    return 0;
}