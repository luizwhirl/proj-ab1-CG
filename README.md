# Projeto AB1 Computação Gráfica

Este é o repositório do "Futebol do Chiquinho", uma aplicação interativa desenvolvida em C++ com OpenGL/GLUT. O foco aqui é a renderização procedural do mundo, manipulação de matrizes de projeção e a ponte entre o paradigma Orientado a Objetos e funções procedurais.

---

## Estrutura de Diretórios e Arquivos

O código foi arquitetado para separar responsabilidades lógicas e visuais. Abaixo está a explicação do que cada módulo faz sob o capô:

**`main.cpp` (Ponto de Entrada)**
É aqui que a mágica começa. O arquivo é responsável por inicializar o contexto do GLUT, instanciar a janela do sistema operacional e registrar as funções de callback. Para manter o código limpo, todas essas chamadas são delegadas imediatamente para a classe `Game`.

**`Game` (O Motor)**
Atua como o gerenciador central (Engine) do projeto. Ele controla o loop principal de renderização, o framerate e o estado global da aplicação.

**`Campo` (Renderização do Mundo 2D)**
Este módulo cuida de tudo que diz respeito ao gramado. Ele gera a textura procedural da grama, desenha as faixas do campo de futebol, calcula o mapeamento de textura (UV mapping) e renderiza as linhas de marcação e áreas geométricas.

**`Scoreboard` (A Interface/HUD)**
Responsável por renderizar a camada de interface (HUD) que fica sobreposta ao jogo. Ele desenha os placares, as bandeiras e gerencia os botões interativos na tela. 
>*Nota: Os botões atuais são apenas placeholders para testar o placar e serão removidos/refeitos no futuro.*

**`InputManager` (Interação e Câmera)**
O cérebro por trás dos controles. Ele captura os eventos de clique e movimento do mouse, gerencia o estado de zoom da câmera e faz a conversão matemática pesada das coordenadas de Tela (Pixels do seu monitor) para coordenadas de Mundo (Sistema Cartesiano do OpenGL).

---

## Conceitos de C++ Aplicados


**Separação em `.h` (Interface) e `.cpp` (Implementação)**
Os arquivos `.h` (Headers) funcionam como contratos: eles declaram quais variáveis e métodos a classe possui, sem escrever a lógica. Isso acelera a compilação, pois outros módulos só precisam ler essa "planta baixa". Já os arquivos `.cpp` incluem seus respectivos headers e implementam a lógica de fato (matemática, renderização, regras de negócio).

**Include Guards**
Em todos os arquivos `.h` (ex: `#ifndef`, `#define`, `#endif`), utilizamos Include Guards. Como um header pode ser incluído por múltiplos `.cpp`, essas guardas garantem que o compilador leia a declaração da classe apenas uma vez, evitando erros fatais de redefinição de classe (violação da Regra de Definição Única).

---

## ⚙️ Arquitetura do Sistema e Lógica de Implementação

### 1. Game e o Padrão Singleton
Como a biblioteca GLUT foi escrita em C puro, suas funções de callback (como `glutDisplayFunc`) exigem ponteiros para funções globais e não aceitam métodos de instância de classes C++ diretamente. Para resolver isso (graças aos ensinamentos do Baldoino), a classe `Game` utiliza o padrão **Singleton**. Ela possui uma única instância global acessível via `Game::getInstance()` e expõe métodos estáticos que servem de ponte, chamando os métodos reais da instância e unindo o GLUT ao nosso ecossistema de Orientação a Objetos.

### 2. Geração Procedural do Campo
O campo não é uma imagem estática importada, mas sim gerado matematicamente na memória durante a inicialização.
* **Textura da Grama:** Um array de 128x128 pixels é alocado. O código escolhe aleatoriamente tons de verde para cada pixel, criando uma textura que é enviada para a VRAM usando o filtro `GL_NEAREST` para garantir um visual rígido e pixelado.
* **Efeito de Corte:** O gramado é dividido em 11 quadriláteros (`GL_QUADS`) horizontais. O código altera a cor base dos vértices (que multiplica a cor da textura) para criar o visual de faixas claras e escuras (simulando a grama cortada de estádio).
* **Mapeamento Contínuo:** A matemática do mapeamento UV (`vBase` e `vTopo`) garante que a textura se repita 15 vezes no eixo Y de forma perfeitamente contínua, sem "quebrar" visualmente na divisão dos quadriláteros.

### 3. Scoreboard e Projeção Ortogonal
A interface de placar é desenhada *por cima* do mundo 3D/2D usando técnicas de sobreposição.
* **Camada HUD:** O método salva a matriz da câmera atual (`glPushMatrix`) e reseta a projeção utilizando `glOrtho`. Os limites da projeção são configurados para coincidir exatamente com os pixels da janela do sistema (onde 0,0 é o topo-esquerdo). Após desenhar o placar em 2D, a matriz do mundo é restaurada (`glPopMatrix`).
* **Hitboxes em Tempo Real:** O placar possui botões de teste cujas colisões são validadas a cada clique do usuário. Se as coordenadas do mouse (em pixels) baterem com a área do botão, os placares (que começam no clássico 1x7) são atualizados.

### 4. InputManager e a Matemática do Zoom
* **Conversão Screen-to-World:** Monitores enxergam de (0,0) até a resolução máxima, mas o OpenGL enxerga um plano cartesiano com limites arbitrários. O método calcula o *Aspect Ratio* da janela e mapeia linearmente onde o clique ocorreu, invertendo o eixo Y (já que monitores crescem para baixo e o OpenGL para cima).
* **Câmera Dinâmica:** Quando o zoom é ativado, a projeção `glOrtho` é recalculada. O código reduz o campo de visão para 35% do tamanho original e centraliza a tela exatamente na coordenada matemática de mundo onde o cursor do mouse estava apontando.

---

## Notas sobre Áudio e Dependências

Para a compilação padrão, as bibliotecas do OpenGL (`-lfreeglut -lopengl32 -lglu32`) são suficientes.

A compilação envolve áudio habilitado, então o projeto depende da biblioteca **irrKlang**. Nesse caso, é necessário linkar a biblioteca estática durante a compilação:
`g++ *.cpp ./irrKlang/lib/Win32-gcc/libirrKlang.a -o main.exe -I./irrKlang/include -lopengl32 -lglu32 -lfreeglut`

*Atenção: O executável final exige que as bibliotecas dinâmicas `ikpMP3.dll` e `irrKlang.dll` estejam na mesma pasta para funcionar corretamente.*
