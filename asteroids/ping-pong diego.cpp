#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <algorithm>

#include <my-lib/math-vector.h>

using Vector = Mylib::Math::Vector<float, 2>;

class Racket {
private:
    Vector pos;
	Vector vel;

public:
    Racket(const Vector pos_init, const Vector size)
        : pos(pos_init), vel(0.0f, 0.0f)  // Inicializa pos_y com y
    {

	}

    SDL_Rect get_rect () { return racket; }

    void mover(float hspd, int limite) {
        this->pos.y = std::clamp(pos_y + hspd, 0.0f, static_cast<float>(limite - racket.h));
        racket.y = static_cast<int>(pos_y);  // Atualiza a posição inteira para desenhar
    }

    void atualizar(const Uint8* state, const bool player, const float dt) { //state retorna 1 se a tecla tiver pressionada e 0 se não
        if (player) {
            if (state[SDL_SCANCODE_W]) { //verifica no array se w está pressionado
            	this->mover(-velocidade, altura);

				this->pos.y -= velocidade * dt;
            }
            if (state[SDL_SCANCODE_S]) {
                this->mover(velocidade, altura);
            }
        } else {
            if (state[SDL_SCANCODE_UP]) {
                mover(-velocidade, altura);
            }
            if (state[SDL_SCANCODE_DOWN]) {
                mover(velocidade, altura);
            }
        }
    }
    
    void desenhar(SDL_Renderer* renderer, SDL_Color cor) {
		SDL_Rect rect;

		rect.x = ...;
		rect.y = ...;

        SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
        SDL_RenderFillRect(renderer, &racket);
    }
};

class Ball {
private:
    Vector pos;  // Posição Y como float 
    Vector velocidade;  // Velocidade como float
	Vector size;

public:
    Ball(int x, int y, int tamanho, float velocidade)
        : velocidade(velocidade), pos_x(x), pos_y(y)  // Inicializa pos_y com y
    {   ball = { x, y, tamanho, tamanho };    } //pra compactar

    SDL_Rect get_rect () { return ball; }

    bool colision_obj (const float wspd, const float hspd, const Racket& racket_obj, const Ball& ball_obj)
    {   
        SDL_Rect racket = racket_obj.get_rect();
        SDL_Rect ball = ball_obj.get_rect();

        const int a1 = racket.x;
        const int a2 = racket.x + racket.w;
        
        const int b1 = ball.x;
        int b2 = ball.x + ball.w + wspd;

        int c1 = racket.y;
        int c2 = racket.y + racket.h;
        
        int d1 = ball.y;
        int d2 = ball.y + ball.h + hspd;
        

/*        if ((a2 < b1 or b2 < a1) || (c2 < d1 or d2 < c1)) //se nao colido
        {
            return false;
        }
        return true; */

		return !((a2 < b1 || b2 < a1) || (c2 < d1 || d2 < c1));
    }

    bool colision_lim (float spd, int medida_ball, int pos_ball, int medida_room)
    {
        return (pos_ball + medida_ball + spd >= medida_room || pos_ball + spd <= 0);
    }

    void mover(const float dt, int limite_x, int limite_y) {
		this->pos += this->velocit * dt;

		if (posição saiu da tela) ....
    }
    
    void desenhar(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball);
    }
};


class Jogo {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* music;
    Mix_Chunk* soundEffect;
    bool running;
    int largura, altura;
    SDL_Event event;

public:
    Jogo(int largura, int altura)
        : window(nullptr), renderer(nullptr), music(nullptr), soundEffect(nullptr), running(true), largura(largura), altura(altura) {}

    ~Jogo() {
        if (soundEffect) Mix_FreeChunk(soundEffect);  // Libera a memória
        if (music) Mix_FreeMusic(music);        // Libera a memória
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        Mix_CloseAudio();
        SDL_Quit();
        Mix_Quit();
    }

    bool inicializar() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
            return false;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
            std::cerr << "Erro ao inicializar SDL_Mixer: " << Mix_GetError() << std::endl;
            return false;
        }

        window = SDL_CreateWindow("Ping pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largura, altura, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
            return false;
        }

        music = Mix_LoadMUS("music.mp3");
        if (!music) {
            std::cerr << "Erro ao carregar música: " << Mix_GetError() << std::endl;
            return false;
        }

        soundEffect = Mix_LoadWAV("effect.mp3");
        if (!soundEffect) {
            std::cerr << "Erro ao carregar efeito sonoro: " << Mix_GetError() << std::endl;
            return false;
        }

        Mix_PlayMusic(music, -1);  // Tocar música em loop
        return true;
    }

    

    void executar() {

        //Criar
        Racket racket(30, 300, 20, 100, 0.2f);   // Velocidade ajustada
        Racket racket_2(largura - 50, 300, 20, 100, 0.2f); // Outra 
        Ball ball(largura / 2, altura / 2, 15, 4);

        SDL_Color blue = {0,0,255};
        SDL_Color red = {255,0,0};
        float wspd = 0.1;
        float hspd = 0.1;

		float tempo_anterior = SDL_Getticks();

        //Executando
        while (running) {
			const float tempo_atual = SDL_Getticks();
			const float dt = tempo_atual - tempo_anterior;
			tempo_anterior = tempo_atual;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            const Uint8* state = SDL_GetKeyboardState(nullptr);

			this->process_physics();
			this->check_collisions();
			this->render();
        }
    }
};

int main(int argc, char* argv[]) {
    Jogo jogo(800, 600);

    if (!jogo.inicializar()) {
        std::cerr << "Erro ao inicializar o jogo." << std::endl;
        return 1;
    }

    jogo.executar();
    return 0;
}