# Projeto AB1 Computação Gráfica: Futibas da resenha
---

### sem muito papinho, pro audio use o comando 
```
g++ *.cpp ./irrKlang/lib/Win32-gcc/libirrKlang.a -o main.exe -I./irrKlang/include -lopengl32 -lglu32 -lfreeglut
```
se quiser rodar com audio.
Se garanta que ipkMP3.dll e irrKlang.dll (ambos em irrKlang\bin\win32-gcc\) estejam no mesmo diretório que main.exe.
#### Ah indiano mas cade o readme bonitinho???
![eu](https://www.zapfigurinhas.com/img/webp/iTixSg2.webp)


## Estrutura de Diretórios e Arquivos

O código atualmente tá dividido nos seguintes módulos principais:

* **`main.cpp`**: Ponto de entrada da aplicação. Inicializa o contexto do GLUT, cria a janela e registra as funções de callback delegando-as para a classe `Game`.
* **`Game (.h/.cpp)`**: Gerenciador central (Engine). Controla o loop principal de renderização e o estado global da aplicação.
* **`Campo (.h/.cpp)`**: Responsável por toda a renderização do mundo 2D (textura do gramado, linhas de marcação, áreas geométricas).
* **`Scoreboard (.h/.cpp)`**: Responsável pela renderização do HUD, ele renderiza placares e lida com os "botões" interativos na tela.
    > PS: Esses botões são só um placeholder pra gente testar o placar, eles depois vao pros caraio
* **`InputManager (.h/.cpp)`**: Lida com eventos de mouse, estado de zoom e a conversão crítica de coordenadas de Tela (Pixels) para coordenadas de Mundo (OpenGL).

#### Arquivos de Cabeçalho (`.h`) vs. Código-Fonte (`.cpp`)

Eu não sei se precisa explicar isso, mas achei interessante incluir: Quase toda classe do projeto possui um par de arquivos: um `.h` (Header) e um `.cpp` (Source). 

Essa divisão existe por motivos arquiteturais e de como o compilador do C++ funciona:

* **Arquivos `.h` (A Interface / O "O Que"):** Os arquivos de cabeçalho contêm apenas as **declarações**. Eles funcionam como um "contrato" ou um sumário da classe. Neles, definimos quais variáveis a classe possui e quais métodos ela disponibiliza (suas assinaturas), mas sem escrever a lógica interna deles. 
  * *Vantagem:* Quando outro arquivo (como o `main.cpp`) precisa usar a classe `Game`, ele só precisa fazer o `#include "Game.h"`. O compilador apenas lê a "planta baixa" da classe para saber quanto de memória alocar e quais métodos existem, acelerando muito a compilação de projetos grandes.

* **Arquivos `.cpp` (A Implementação / O "Como"):** Uma vez que a gente tira isso da frente, a gente já pode começar a brincar: O arquivo `.cpp` faz o `#include` do seu `.h` relativo e escreve o corpo dos métodos. É onde a lógica de renderização, matemática e regras de negócio estão programadas.

* **Include Guards (`#ifndef`, `#define`, `#endif`):**
  Em todos os nossos arquivos `.h` (como no `InputManager.h`), você vai ver essas  três linhas encapsulando o código. Essas porras são os Include Guardsdo código . Como um `.h` pode ser incluído por vários arquivos `.cpp` diferentes, essas guardas garantem que o compilador leia a declaração da classe apenas **uma vez** por ciclo de compilação, prevenindo erros fatais de "redefinição de classe" (violação da Regra de Definição Única / ODR).

---

## Arquitetura do Sistema e Lógica de Implementação

### 1. Motor Principal: `Game` (Padrão Singleton)
A classe `Game` é a nossa classe principal, já que é ela a orquestradora do projeto. 
* **Probleminha do GLUT com C++:** Como o  GLUT é uma biblioteca escrita em C puro, então as funções de callback (como `glutDisplayFunc`) esperam ponteiros de funções globais ou estáticas, e não acietam métodos de instância de uma classe (pois métodos de instância possuem um parâmetro implícito `this`).
* **A Solução:** Como a gente aprendeu com Baldoino, `Game` foi implementada usando o padrão **Singleton** (acessível via `Game::getInstance()`). Ela expõe métodos estáticos (ex: `Game::displayCallback()`) que simplesmente chamam os métodos da instância única, servindo como uma ponte entre o mundo procedural do GLUT e o ecossistema do POO.

### 2. Renderização do Mundo: `Campo`
Como a gente tem que desenhar o campo, então ele uma imagem importada. Ele é gerado proceduralmente no momento da inicialização.
* **Textura Procedural da Grama:** O método `createGrassTexture()` aloca um array de 128x128 pixels em memória. Ele itera por cada pixel, escolhendo aleatoriamente (`rand() % 4`) uma entre quatro cores de uma paleta de tons de verde. Essa textura é então enviada para a VRAM via `glTexImage2D` usando filtros `GL_NEAREST` para manter o aspecto "pixelado/rígido" e configurada para se repetir (`GL_REPEAT`).
* **Efeito "Corte de Grama":** Para simular aquele efeito de gramado de estádio cortado em faixas, o campo é desenhado utilizando 11 quadriláteros (`GL_QUADS`) horizontais empilhados. Alterna-se as cores dos vértices (`glColor3f` atuando como multiplicador da textura): faixas pares recebem cor branca (1.0) mantendo a textura original, faixas ímpares recebem uma cor ligeiramente escurecida (0.85, 0.90, 0.85).
* **Mapeamento UV Contínuo:** As coordenadas `vBase` e `vTopo` da textura são calculadas matematicamente para que a textura não "quebre" na transição entre uma faixa e outra, repetindo-se exatamente 15 vezes (`repeats = 15.0f`) ao longo do eixo Y.

### 3. Interface de Usuário: `Scoreboard` (HUD Overlay)
A interface é uma camada 2D desenhada *por cima* do campo 3D/ortogonal.
* **Técnica de Projeção HUD:** O método `beginHUD()` salva a matriz atual (`glPushMatrix`) e reseta a projeção da câmera via `glOrtho(0, winW, winH, 0, -1.0, 1.0)`. Isso transforma o sistema de coordenadas temporariamente para coincidir com os **pixels exatos da janela**, onde (0,0) é o topo-esquerdo. Após renderizar placares e bandeiras feitas com `GL_QUADS`, o `endHUD()` restaura a matriz (`glPopMatrix`), devolvendo a configuração da câmera do mundo do jogo.
* **Interatividade (Hitboxes):** O placar possui botões de teste cujas colisões são checadas em tempo real pelas funções `checkButtonLeftClick` e `checkButtonRightClick`. Quando o usuário clica, se as coordenadas X e Y do mouse (em pixels) estiverem dentro das "caixas delimitadoras" dos botões, os valores `scoreLeft` ou `scoreRight` (que iniciam no icônico 1x7) são incrementados.

### 4. Interação e Sistema de Câmera: `InputManager` e Zoom
> **NOTA:** O sistema de câmera vai mudar, então considere essa explicação abaixo somente para como está funcionando HOJE ()
  
O sistema de zoom é a feature mais complexa matematicamente. Ele permite clicar em um ponto do campo e ampliar a visão exatamente centrada naquele local do clique.
* **Screen-to-World Space:** A janela do Windows/Linux entende cliques em "pixels" de (0,0) até (600, 800). O OpenGL enxerga um mundo ortogonal de -size até +size. O método `updateMouseWorldCoords()` recalcula a relação de aspecto (Aspect Ratio) e mapeia linearmente a posição X do clique (em %) para o range delimitado do campo. O eixo Y é invertido manualmente, pois a janela do SO cresce para baixo, enquanto o plano cartesiano clássico do OpenGL cresce para cima.
* **Lógica da Câmera (glOrtho):** No método `Game::setupCamera()`, se a flag `isZoomed` do `InputManager` for verdadeira, a câmera recalcula os limites `left`, `right`, `bottom`, e `top`. Ela pega as coordenadas de mundo salvas no passo anterior, calcula um campo de visão reduzido (fator de 0.35f, ou seja, 35% do tamanho original) e centraliza a projeção de exibição ao redor da coordenada exata onde o mouse estava mirando.

---
## 4. E os .png? 

Para carregar arquivos de imagem e transformá-los em texturas no OpenGL sem depender de bibliotecas pesadas e difíceis de configurar, nós estamos usando o [stb_image.h](https://github.com/nothings/stb). Essa biblioteca é de público, muito leve e prática.

Exemplo no nosso código:

### 1. Inclusão e Implementação
O `stb_image` precisa ser "instanciado" em apenas **um** arquivo `.cpp` do projeto. E a gente faz isso definindo a macro `STB_IMAGE_IMPLEMENTATION` antes do include. Isso vai  instruir o compilador a transformar o header em código-fonte executável naquele exato local.

```cpp
// Transforma o header em um arquivo .cpp com a implementação
// IMPORTANTE: Só precisa declarar isso em UM arquivo fonte do projeto
// Então uma vez que já tá declarado no campo, tá feito
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 
```

### 2. Passando a imagem da RAM para a VRAM (placa de vidro)

A resenha aqui tá no método `loadArquibancadaTexture`. Já que o fluxo envolve ler a imagem do disco, converter as coordendas (que pra tu que não sabe, o openGL lê diferente o eixo Y), transferir os pixels para a VRAM e liberar a memória da RAM pra gente evitar memory leak (porque aparentemente isso pode MESMO acontecer).

```cpp
void Campo::loadArquibancadaTexture(const char* filepath) {
    // Gera a textura e faz o bind dela no openGL
    glGenTextures(1, &arquibancadaTexture);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTexture);

    // [...]

    int width, height, nrChannels;
    
    // DEtALHE DOIDO:
    // as imagens normais (png, jpg, dbz, etc) têm a origem (0,0) no canto SUPERIOR esquerdo
    // mas aparentemente o ppenGL espera que o pixel (0,0) seja no canto INFERIOR esquerdo
    // entao essa funçãozinha inverte a imagem verticalmente na hora do carregamento para corrigir isso
    stbi_set_flip_vertically_on_load(true); 
    
    // Lê o arquivo do disco para a memória ram
    // o stb_load preenche a largura, altura e o número de canais (nrChannels)
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    
    if (data) { 
        // isso aqu descobre dinamicamente se a imagem tem canal Alpha (pra transparência)
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        
        // envia os pixels da memória ram para a memória de vídeo
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        // limpa a memória ram
        // como o openGL já tem uma cópia da imagem lá na gpu, a gente já não precisamos mais 
        // segurar esses dados aqui. 
        // esse bagulhete aqui embaixo evita o memory leak
        stbi_image_free(data); // <<<< esse. É bom tu botar ai  
        
    } else {
        std::cerr << "Falha ao carregar a textura: " << filepath << std::endl;
    }
}
```
---

## Compilação (Exemplo com GCC/MinGW no Windows)

Tenha certeza que vocÇe tá no diretório correto (pasta `proj-ab1-CG/`) via terminal e execute a linkagem de todos os módulos de classe:

```bash
g++ *.cpp -o main.exe -lfreeglut -lopengl32 -lglu32

e abra o bicho com

./main.exe 
