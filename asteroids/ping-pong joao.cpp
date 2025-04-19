#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>



const int widthWindow = 800;
const int heightWindow = 600;


class Raquete{
private:        //Private: não devem ser acessados diretamente, evitando manipulações incorretas durante o código (ENCAPSULAMENTO)
    float x, y; //Posição X e Y da raquete
    int altura, largura;    //largura e altura em pixels
    float velocidade;       //velocidade

public:

    //CONSTRUTOR: é um método para criar um objeto na classe Raquete. Ele inicializa com valores fornecidos
    Raquete(float startX, float startY, int w, int h, float vel) : x(startX), y(startY), largura(w), altura(h), velocidade(0.1) {}


    //métodos getters: métodos publicos que retornam valores privados.
    int getLargura() const {return largura;}
    int getAltura() const {return altura;}

    //mover a raquete
    void mover(bool moverDireita, bool moverEsquerda, int larguraTela, const float dt){
        if(moverEsquerda && x > 0){     //se ele está se movendo para a esquerda e não esta no canto esquerdo, a velocidade é diminuida e ele vai para a esquerda.
            x -= velocidade * dt;    //ele move 0.1 pixels pro lado. Então se X = 300, ele vai diminuindo 0.1 pixels
        }
        if(moverDireita && x + largura < larguraTela){      //verifica se esta se movendo para a direita e se X +  largura da raquete é menor q a tela. Por exemplo: X = 780 e a largura da raquete for 20 pixels, ele não se moverá mais para a direita
            x += velocidade * dt;
        }
    }

    //movimento da raquete superior automaticamente
    void moverAutomatica(float bolaX, int larguraTela){     //posição da bola e a largura da tela.
        float centroRaqueteSuperior = x + largura / 2;  //é o centro da raquete. ex: largura é 40, o centro é 20.
        float distancia = bolaX - centroRaqueteSuperior;  // Distância entre o centro da raquete e a bolinha

        // Ajustar a velocidade com base na distância da bolinha
        if (distancia > 0) { //se for maior que 0, está na direita, se for menor, está na esquerda
            if (distancia > 50) { //se a distancia da bola e da raquete é mais de 50 pixels, a velocidade é multiplicada por 1.5, fazendo-a chegar mais rápido
                x += velocidade * 1.5f;  // Acelerar se a bola estiver muito à direita
            } else {
                x += velocidade;  // Velocidade normal
            }
        } else if (distancia < 0) {
            if (distancia < -50) { // Acelerar para a esquerda se a bola estiver muito à esquerda
                x -= velocidade * 1.5f;
            } else {
                x -= velocidade;
            }
        }

        // Evita que a raquete ultrapasse os limites da tela
        if (x < 0) x = 0;   //se for menor que zero, ele para e so volta se mover quando é maior que 0
        if (x + largura > larguraTela) x = larguraTela - largura;   //se sair da tela pela direita, o x é o tamanho da tela - a largura da raquete, ou seja, ele não passa da tela
    }
    void setY(float novoY) {
        y = novoY;
    }

    //Desenhar a raquete 
    void desenhar(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 0, 255, 0 ,255);       //define a cor primeiramente (verde)
        SDL_Rect raquete = { static_cast<int>(x), static_cast<int>(y), largura, altura };   //desenha como um retângulo, primeiro o valor inicial x, valor inicial y, largura e altura
        SDL_RenderFillRect(renderer, &raquete);     //desenha o retângulo
    }

    //pega a posição da raquete
    float getX() const { return x;} 
    float getY() const { return y;}
};


class Bolinha{
private:    //float permite movimentos mais suaves
    float x, y; //posição
    float vx,  vy; //velocidade
    float raio;   //tamanho da bolinha
	Mix_Chunk* somColisao;

public:
    Bolinha(float startX, float startY, float velx, float vely, int r) : x(startX), y(startY), vx(0.01), vy(0.01), raio(r) {

    }

    void mover(){
        if (y + raio >= heightWindow) {
        x = 400;  // Posição de reinício
        y = 300;  // Posição de reinício
        vy = -velocidadeInicial;  // Reiniciar velocidade vertical
        vx = 0.05f;  // Reiniciar velocidade horizontal
        return;  // Não mover a bolinha após o reinício
        }

        x += vx;
        y += vy;

        if(x - raio < 0 || x + raio > widthWindow){     //se x - o tamanho da bola for menor que zero ou se o x + raio for maior que o tamanho da tela, inverte a direção
            vx = -vx;
        }

        if(y - raio < 0){       
            vy = -vy;
        }
    }

    void verificarColisaoRaquete(Raquete& raquete){
        bool colisaoHorizontal = (x + raio > raquete.getX()) && (x - raio < raquete.getX() + raquete.getLargura());     // Verifica se a borda direita da bolinha ultrapassa o lado esquerdo da raquete.
        bool colisaoVertical = (y + raio > raquete.getY()) && (y - raio < raquete.getY() + raquete.getAltura());        //Verifica se a borda esquerda da bolinha ultrapassa o lado direito da raquete.     

        if(colisaoHorizontal && colisaoVertical){
            vy = -vy;   //a bola é revertida
            vy += (vy > 0 ? 0.01f : -0.01f); // Aumenta a velocidade em 0.02f a cada colisão
            
            // Limita a velocidade para que não fique muito rápida
            if (vy > 1.0f) vy = 1.0f;  // Velocidade máxima para baixo
            if (vy < -1.0f) vy = -1.0f; // Velocidade máxima para cima

            Mix_PlayChannel(-1, somColisao, 0);
        }
    }

    void atualizar(float alturaTela, const float dt){
        x += vx;         
        y += vy;

		this->pos += this->vel * dt;

        if(y + raio > alturaTela){
            x = 400;
            y = 300;
            vy = -velocidadeInicial; 
            vx = 0.01f;
        }
    }

    void desenhar(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   //define a cor da bolinha
        SDL_Rect bola = { static_cast<int>(x) - raio, static_cast<int>(y) - raio, raio * 2, raio * 2 };
        SDL_RenderFillRect(renderer, &bola);   
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    void refletirVerticalmente(){
        vy = -vy;
    }
};


class Game{
public:
	struct Error {
		int code;
	};

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* backgroundMusic;
    Mix_Chunk* somColisao;
    bool isRunning;
	int width;
	int height;

    Bolinha bolinha;
    Raquete raqueteJogador;
    Raquete raqueteSuperior;

public:
    Game(const char* title, int width_, int height_) : window(nullptr), renderer(nullptr), isRunning(false), 
	    bolinha(400, 300, 5, 5, 10),
    	raqueteJogador(350, 550, 100, 20, 5),
    	raqueteSuperior(350, 0, 100, 20, 5),
		width(width_),
		height(height_)
	{    
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {  
            std::cerr << "Erro ao inicializar o SDL2: " << SDL_GetError() << std::endl;
            throw Error { 0 }; //encerra o programa
        }

        window = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_CENTERED,   
                                SDL_WINDOWPOS_CENTERED,   
                                width, height,    
                                SDL_WINDOW_SHOWN);  

        if (!window) {
            std::cerr << "Erro ao criar a janela: " << SDL_GetError() << std::endl;
            SDL_Quit();
            throw Error { 1 };
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (!renderer) { 
            std::cerr << "Erro ao criar o renderer: " << SDL_GetError() << std::endl;
            throw Error { 2 };
        }
        
        if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
            std::cerr << "Erro ao inicializar o SDL_mixer: " << Mix_GetError() << std::endl;
            SDL_Quit();
            throw Error { 3 };
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {  //22050: Frequência de amostragem (número de samples por segundo). Aqui, é definido 22050 Hz, o que é uma taxa de áudio razoável. 2: O número de canais de áudio (2 significa estéreo).
                                                                   //4096: O tamanho do buffer de áudio, ou seja, quanto de dados de áudio o sistema deve manter em memória antes de começar a processar o áudio.
            std::cerr << "Erro ao abrir o áudio: " << Mix_GetError() << std::endl;
            Mix_Quit();
            SDL_Quit();
            throw Error { 4 };
        } else { 
            std::cout << "Áudio inicializado com sucesso!" << std::endl;
        }


        somColisao = Mix_LoadWAV("pingpongSong.mp3");
        if (!somColisao) {
            std::cerr << "Som de colisão não carregado: " << Mix_GetError() << std::endl;
            throw Error {5 };
        }

        backgroundMusic = Mix_LoadMUS("bground.wav");
        if(!backgroundMusic){
            std::cerr << "Erro ao carregar a música: " << Mix_GetError() << std::endl;
            return false;
        }

        Mix_PlayMusic(backgroundMusic, -1);

        isRunning = true;
        return true;
    }

    void run(){
        while(isRunning){
            handleEvents();
            update();
            render();
        }
    }

    void handleEvents(){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                isRunning = false;
            }
        }
    }

    void update(){
        bolinha.mover(this->width, this->height);

        bolinha.atualizar(600);

        bolinha.verificarColisaoRaquete(raqueteJogador, somColisao);

        bolinha.verificarColisaoRaquete(raqueteSuperior, somColisao);

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        bool moverEsquerda =  keystates[SDL_SCANCODE_LEFT];
        bool moverDireita = keystates[SDL_SCANCODE_RIGHT];
        raqueteJogador.mover(moverDireita, moverEsquerda, 800);
        raqueteSuperior.moverAutomatica(bolinha.getX(), 800);

        raqueteJogador.setY(600 - raqueteJogador.getAltura());  // Raquete inferior no fundo da tela
        raqueteSuperior.setY(0); // Raquete superior no topo da tela
    }

    void render(){
		const SDL_Color cor_fundo = { ... };

		// setting background color
        SDL_SetRenderDrawColor(renderer, cor_fundo.r, cor_fundo.g, cor_fundo.b, cor_fundo.a);
        SDL_RenderClear(renderer);

        //desenhar objetos
        bolinha.desenhar(renderer);
        raqueteJogador.desenhar(renderer);
        raqueteSuperior.desenhar(renderer);

        SDL_RenderPresent(renderer);
    }

    void clean(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_FreeChunk(somColisao);
        Mix_FreeMusic(backgroundMusic);
        SDL_Quit();
        Mix_Quit();
    }

    ~Game() {
        clean();
    }

};



int main(int argc, char* argv[]){
    Game *game;

	try {
		game = new Game;
		game->run();
		delete game;
	}
	catch (const Game::Error& error) {
		std::cout << "morreu" << std::endl;
	}

    return 0;
}
