# 🚀 Asteroids — SDL2

Um jogo de asteroides feito em **C++** com **SDL2**, inspirado no clássico arcade. Desvie dos asteroides, atire neles e acumule pontos!

---

## 📸 Visão Geral

O jogador controla uma nave no centro da tela. Asteroides se movimentam quicando pelas bordas da janela. O objetivo é destruí-los com projéteis antes que colidam com a nave.

---

## 🎮 Controles

| Tecla         | Ação              |
|---------------|-------------------|
| `←` `→` `↑` `↓` | Mover a nave    |
| `Espaço`      | Atirar projétil   |

---

## ✨ Funcionalidades

- Movimentação da nave com colisão com bordas
- Asteroides com rebote dinâmico nas bordas da janela
- Sistema de projéteis com detecção de colisão
- Pontuação exibida em tempo real na tela
- Trilha sonora e efeitos sonoros via SDL_mixer
- Reset automático ao colidir com um asteroide

---

## 🛠️ Dependências

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)
- Compilador C++17 ou superior (g++, clang++)

### Instalação das dependências (Linux/Debian)

```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

---

## 📁 Arquivos necessários

Coloque os seguintes arquivos na raiz do projeto junto ao executável:

```
arial.ttf                  # Fonte utilizada para o placar
asteroidsDestroyed.wav     # Som ao destruir um asteroide
projectilSound.wav         # Som ao disparar
asteroidsMusicTheme.mp3    # Trilha sonora do jogo
```

---

## ⚙️ Compilação

```bash
g++ main.cpp -o asteroids \
    -lSDL2 -lSDL2_ttf -lSDL2_mixer \
    -std=c++17
```

> Certifique-se de que o arquivo `math-vector.h` está no mesmo diretório.

---

## ▶️ Execução

```bash
./asteroids
```

---

## 🏗️ Estrutura do Código

| Classe / Função    | Responsabilidade                                      |
|--------------------|-------------------------------------------------------|
| `Game`             | Loop principal, inicialização, renderização e eventos |
| `Player`           | Movimentação da nave e disparo de projéteis           |
| `AsteroidsClass`   | Lógica de movimento e colisão dos asteroides          |
| `ProjeteisClass`   | Movimento dos projéteis e detecção de acerto          |
| `colision()`       | Detecção de colisão AABB entre dois retângulos        |
| `getTime()`        | Tempo em segundos via SDL_GetTicks                    |

---

## 📌 Observações

- O jogo faz reset automático (posição da nave e asteroides, pontuação zerada) ao ocorrer colisão entre o jogador e um asteroide.
- Novos asteroides são gerados automaticamente quando a lista fica vazia.
- A física usa **delta time** para garantir movimentação consistente independente do FPS.

---

## 📄 Licença

Projeto acadêmico/pessoal — livre para uso e modificação.