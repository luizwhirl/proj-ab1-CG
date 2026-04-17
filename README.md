# Projeto AB1 Computação Gráfica

Este é o repositório do "Futebolzinho do Chiquinho", uma aplicação interativa desenvolvida em C++ com OpenGL/GLUT. O foco aqui é a renderização procedural do mundo, manipulação de matrizes de projeção, implementação de Inteligência Artificial básica, detecção de colisões e a ponte entre o paradigma Orientado a Objetos e funções procedurais.

---
## Dependências, Bibliotecas e Ferramentas 

Para a compilação, além das bibliotecas padrão do OpenGL (`-lfreeglut -lopengl32 -lglu32`), o projeto depende do **irrKlang** para a engine de áudio.

É necessário linkar a biblioteca estática durante a compilação. Um exemplo de compilação com g++ (MinGW):
```bash
g++ *.cpp ./irrKlang/lib/Win32-gcc/libirrKlang.a -o main.exe -I./irrKlang/include -lopengl32 -lglu32 -lfreeglut
```

### Importação de Imagens
Para importar as imagens para o C++, e trabalharmos com suas texturas, utilizamos a biblioteca [stb_image.h](https://github.com/nothings/stb?tab=readme-ov-file). 

Ela é uma solução leve, eficiente e single-header, o que facilita muito a integração ao projeto sem a necessidade de configurar dependências externas.

Responsável por ler os arquivos de imagem do jogo direto do disco (como os arquivos ```.png``` que contêm os sprites dos jogadores, do campo, da bola e da arquibancada) e decodificá-los em dados brutos de pixels. Esses dados são então passados para o OpenGL, que os transforma em texturas 2D para que possam ser finalmente renderizados e desenhados na tela durante o jogo.

### Outras ferramentas utilizadas
- **Aseprite**: Para criação de cenários e sprites dos jogadores/torcedores (você pode vê-los [aqui](https://drive.google.com/drive/folders/1zVDBRiEg8mDZ4Hv7Y7AT7VAMn4HnIKa7?usp=sharing))
- **Visual Studio Code**: Desenvolvimento e compilação
- **Gemini (IA)**: Dúvidas gerais sobre bibliotecas, funções, métodos, etc. 
- **Github Copilot**: Usado para a agilidade durante a escrita do código (Como adicionar comentários ágeis, sugestão da escrita, etc)

---

## Controles do Jogo

- **W, A, S, D:** Movimenta o jogador atual (se estiver com a bola, conduz; se estiver sem, anda livremente).
- **J:** Toca a bola para o aliado mais próximo.
- **K:** Chuta a bola em direção ao gol (quando com a bola) ou dá o bote/rouba a bola (quando sem a posse).
- **L:** Troca o controle para o próximo jogador do seu time.
- **Espaço:** Alternativa para roubar/fazer o rival soltar a bola.
- **Botão Esquerdo do Mouse:** Ativa/Desativa o Zoom Dinâmico da câmera.

---

## Estrutura de Diretórios e Arquivos

O código foi arquitetado para separar responsabilidades lógicas, físicas e visuais. Abaixo está a explicação do que cada módulo faz sob o capô:

**`main.cpp` (Ponto de Entrada)**
É aqui que a mágica começa. O arquivo é responsável por inicializar o contexto do GLUT, instanciar a janela do sistema operacional e registrar as funções de callback. Todas essas chamadas são delegadas imediatamente para a classe `Game`.

**`Game` (O Motor)**
Atua como o gerenciador central (Engine) do projeto. Ele controla o loop principal de renderização, framerate, a Inteligência Artificial, as colisões e o estado global da aplicação (posse de bola, times, goleiros e interações gerais).

**`Jogador` e `Goleiro` (Entidades)**
Representam os "atores" em campo. Cada jogador carrega sua própria textura, direção de olhar (W, A, S, D), posição base (para manter a formação) e lida com animações de caminhada. Os Goleiros possuem uma lógica de patrulha na linha do gol.

**`Bola` (Física e Posse)**
A classe responsável por guardar o estado da partida. Ela sabe se está rolando livre (com inércia e atrito), se está nos pés do time aliado ou sendo dominada pela IA rival.

**`PowerUp` (Itens Especiais)**
Um sistema de itens dinâmicos que surgem aleatoriamente no campo. Fornecem *Speed Boost* (bota) ou *Invencibilidade* (escudo) temporária ao jogador que os coletar.

**`Campo` e `Gol` (O Mundo 2D)**
Este módulo cuida de tudo que diz respeito à arena. Gera a textura procedural da grama, gerencia as colisões das bordas e renderiza as arquibancadas (que reagem e "comemoram" aos gols marcados).

**`Scoreboard` (A Interface/HUD)**
Responsável por renderizar a camada de interface (HUD) que fica sobreposta ao jogo. Desenha os placares do Brasil e da Alemanha.

**`InputManager` (Interação)**
O cérebro por trás dos controles. Captura os eventos de clique, teclado (press/release) e faz a conversão matemática pesada das coordenadas de Tela para o Sistema Cartesiano do OpenGL.

**`SoundManager` (Áudio)**
Gerencia os efeitos sonoros da partida, como o apito do árbitro inicial, os barulhos de chute e as explosões de comemoração da torcida durante os gols, utilizando a engine irrKlang.

---

## Conceitos de C++ Aplicados

**Separação em `.h` (Interface) e `.cpp` (Implementação)**
Os arquivos `.h` (Headers) funcionam como contratos: eles declaram quais variáveis e métodos a classe possui. Já os arquivos `.cpp` incluem seus respectivos headers e implementam a lógica de fato (matemática, renderização, regras de negócio).

**Include Guards**
Em todos os arquivos `.h` (ex: `#ifndef`, `#define`, `#endif`), utilizamos Include Guards. Como um header pode ser incluído por múltiplos `.cpp`, essas guardas garantem que o compilador leia a declaração da classe apenas uma vez, evitando erros fatais de redefinição de classe.

---

## Arquitetura do Sistema e Lógica de Implementação

### 1. Game e o Padrão Singleton
Como a biblioteca GLUT foi escrita em C puro, suas funções de callback exigem ponteiros para funções globais e não aceitam métodos de instância de classes C++ diretamente. Para resolver isso (graças aos ensinamentos do Baldoino), a classe `Game` utiliza o padrão **Singleton** (`Game::getInstance()`) e expõe métodos estáticos que servem de ponte para o GLUT.

### 2. Sistema de IA e Formação Tática (4-4-2)
O jogo possui dois times em campo posicionados em uma clássica formação 4-4-2. A Inteligência Artificial avalia a situação do jogo a cada frame:
* **Perseguição Dinâmica:** Em vez de todos os bonecos correrem atrás da bola como em um futebol de várzea, o algoritmo calcula as distâncias (via Teorema de Pitágoras) e envia apenas os dois jogadores mais próximos para dar o bote. O resto da equipe se movimenta de forma relativa à sua posição base para fechar espaços.
* **Tomada de Decisão do Rival:** Se a máquina é pressionada, ela calcula o "Score" de passe. O passe avalia a distância dos companheiros, proximidade do gol e marcação adversária, escolhendo a melhor opção para tocar a bola ou, caso livre, conduzir para finalização.

### 3. Física de Colisão
Um sistema de "Hitboxes" circulares está ativo 100% do tempo.
* **Repulsão Física:** A função `resolverColisoesJogadores()` checa a distância entre absolutamente todos os jogadores em campo. Se a distância entre dois centros for menor que o limite, o vetor de sobreposição é calculado e ambos os objetos são matematicamente "empurrados" para direções opostas, impedindo que os sprites se atravessem.

### 4. Geração Procedural do Campo
O campo não é uma imagem estática importada, mas sim gerado matematicamente na memória durante a inicialização. Um array de 128x128 escolhe tons de verde aleatórios para simular grama com filtro `GL_NEAREST`. O gramado é desenhado usando múltiplos `GL_QUADS` que modificam a cor de base para simular os cortes verticais de estádio.

### 5. Câmera Dinâmica (Zoom Inteligente)
Quando o zoom é ativado pelo mouse, a projeção ortogonal (`glOrtho`) é recalculada em tempo real para focar exatamente no calor da jogada. O sistema rastreia de quem é a "Posse" atual. Se um aliado tem a bola, foca nele; se um rival roubou, a câmera o acompanha; se a bola está livre (status 0), a câmera foca nas coordenadas x/y da própria bola.

### 6. Scoreboard e Projeção Ortogonal HUD
A interface de placar é desenhada *por cima* do mundo 2D utilizando `glPushMatrix` e `glOrtho` focado nas dimensões literais da tela. Os limites da projeção coincidem com os pixels da janela (0,0 no topo-esquerdo).
