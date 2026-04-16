#include "Campo.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string> // p facilitar a montagem dos caminhos das texturas

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr float PI = 3.14159265358979323846f;

Campo::Campo() {
    grassTexture = 0;
    arquibancadaTexture = 0;
    arquibancadaTextureB = 0;
    arquibancadaTextureC = 0;
    arquibancadaTextureD = 0;

    // inicialização dos ids norte, sul e oeste agora com loop para os arrays
    for(int i = 0; i < NUM_TORCEDORES; i++) {
        torcedorNorteIdleTexture[i] = 0;
        torcedorNorteComemoraTexture[i] = 0;
        torcedorSulIdleTexture[i] = 0;
        torcedorSulComemoraTexture[i] = 0;
        torcedorOesteIdleTexture[i] = 0;
        torcedorOesteComemoraTexture[i] = 0;
    }
}

void Campo::createGrassTexture() {
    const int TEX_SIZE = 128; // 128x128 pixels 
    unsigned char data[TEX_SIZE * TEX_SIZE * 3];

    // paleta de cores no carai
    unsigned char palette[4][3] = {
        { 90, 145,  65},  // verde médio base
        {105, 160,  70},  // verde mais claro
        { 75, 120,  50},  // verde escuro
        {110, 130,  55}   // verde amarelado
    };

    for (int i = 0; i < TEX_SIZE * TEX_SIZE; ++i) {
        int colorIndex = rand() % 4;
        
        data[i * 3]     = palette[colorIndex][0];
        data[i * 3 + 1] = palette[colorIndex][1];
        data[i * 3 + 2] = palette[colorIndex][2];
    }

    glGenTextures(1, &grassTexture);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    
    // enviando dados da textura para o opengll
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_SIZE, TEX_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //  - nao aplica blur ao escalajar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // rete a a rexteura ao longo do campo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// função para carregar as texturas das arquibancadas ´prra nao aguento mais tpdo dia mudando esse comentario
void Campo::loadArquibancadaTextures(const char* filepathA, const char* filepathB, const char* filepathC, const char* filepathD) {
    // arkibankada norte
    glGenTextures(1, &arquibancadaTexture);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // oia que legal tem ate função de virar
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    
    unsigned char *data = stbi_load(filepathA, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        std::cerr << "Falha ao carregar a textura da arquibancada: " << filepathA << std::endl;
    }

    // carregada textura da arquibancada sulll
    glGenTextures(1, &arquibancadaTextureB);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTextureB);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *dataB = stbi_load(filepathB, &width, &height, &nrChannels, 0);
    if (dataB) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataB);
        stbi_image_free(dataB);
    } else {
        std::cerr << "Falha ao carregar a textura da arquibancada B: " << filepathB << std::endl;
    }

    // carregada textura da arquibancada leste
    glGenTextures(1, &arquibancadaTextureC);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTextureC);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *dataC = stbi_load(filepathC, &width, &height, &nrChannels, 0);
    if (dataC) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataC);
        stbi_image_free(dataC);
    } else {
        std::cerr << "Falha ao carregar a textura da arquibancada C: " << filepathC << std::endl;
    }

    // carregada textura da arquibancada oeste
    glGenTextures(1, &arquibancadaTextureD);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTextureD);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *dataD = stbi_load(filepathD, &width, &height, &nrChannels, 0);
    if (dataD) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataD);
        stbi_image_free(dataD);
    } else {
        std::cerr << "Falha ao carregar a textura da arquibancada D: " << filepathD << std::endl;
    }
}

// carrega texturas distintas para a torcida norte, sul e oeste
void Campo::loadTorcedorTextures(const char* idleNortePath, const char* comemoraNortePath, const char* idleSulPath, const char* comemoraSulPath, const char* idleOestePath, const char* comemoraOestePath) {
    // criado os prefixos e diretorios para automatizar a leitura dos 6 torcedores
    std::string prefixos[6] = {"azul", "ray", "manu", "luca", "indo", "rosa"};
    std::string dirNorte = "assets/sprites/torcedor/norte/";
    std::string dirSul = "assets/sprites/torcedor/sul/";
    std::string dirOeste = "assets/sprites/torcedor/oeste/";

    // func lambda auxiliar para não repetir o bloco de carregamento 36 vezes
    auto carregarTextura = [](const std::string& path, GLuint& texID) {
        int width, height, nrChannels;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Falha ao carregar a textura do torcedor: " << path << std::endl;
        }
    };

    // carrega automaticamente todas as variações percorrendo os nomes
    for(int i = 0; i < NUM_TORCEDORES; ++i) {
        carregarTextura(dirNorte + prefixos[i] + "-idle.png", torcedorNorteIdleTexture[i]);
        carregarTextura(dirNorte + prefixos[i] + "-comemora.png", torcedorNorteComemoraTexture[i]);
        
        carregarTextura(dirSul + prefixos[i] + "-idle.png", torcedorSulIdleTexture[i]);
        carregarTextura(dirSul + prefixos[i] + "-comemora.png", torcedorSulComemoraTexture[i]);
        
        carregarTextura(dirOeste + prefixos[i] + "-idle.png", torcedorOesteIdleTexture[i]);
        carregarTextura(dirOeste + prefixos[i] + "-comemora.png", torcedorOesteComemoraTexture[i]);
    }
}

// método recebe bool
void Campo::drawArquibancada(bool isComemorando) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, arquibancadaTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    float largura = 3.5f;
    float alturaProporcional = 2.33f;

    // arquibankada norte 
    float baseNorte = 5.5f;
    float topoNorte = baseNorte + alturaProporcional;
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-largura, baseNorte);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( largura, baseNorte);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( largura, topoNorte);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-largura, topoNorte);
    glEnd();

    // manter a textura ativa para o sprite e habilitar transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor3f(1.0f, 1.0f, 1.0f);

    float tempo = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 
    // 0.2f - tamanho dos jogadores no jogo
    float tam = 0.2f; // tamanh do kwadrado 

    int numFileiras = 2;
    int numQuadradinhos = 8;

    // ajusta o X e Y inicial e final para garantir que fiquem estritamente "dentro" do concreto
    // o x original vai de -3.5 a 3.5
    //  O Y original vai de 5.5 a 7.83
    float startX = -2.8f; 
    float endX = 2.8f;    
    float startY = 6.66f; // fileira Y do di cima
    float endY = 7.0f;   // fileira Y do di cima

    // glBegin/glEnd foram estao para DENTRO do laço, pois o OpenGL exige que o glBindTexture seja chamado FORA do glBegin
    for (int row = 0; row < numFileiras; ++row) {
        for (int col = 0; col < numQuadradinhos; ++col) {
            
            // Define aleatoriamente qual torcedor é desenhado baseado numa pseudo-seed de col e row
            int tipoTorcedor = abs(row * 7 + col * 13) % NUM_TORCEDORES;
            
            // define a textura baseada no estado do timer e tipoAleatorio (sprites do norte)
            if (isComemorando) {
                glBindTexture(GL_TEXTURE_2D, torcedorNorteComemoraTexture[tipoTorcedor]);
            } else {
                glBindTexture(GL_TEXTURE_2D, torcedorNorteIdleTexture[tipoTorcedor]);
            }

            glBegin(GL_QUADS);
            // interpola linearmente a posição de acordo com a coluna e fileira
            float x = startX + col * ((endX - startX) / (numQuadradinhos - 1));
            float y = startY + row * ((endY - startY) / (numFileiras - 1));
            
            float offset = x * 3.5f + y * 2.5f; 
            
            // o pulo só acontece se isComemorando for true, do contrário ficam no chão
            float pulo = 0.0f;
            if (isComemorando) {
                pulo = (std::sin(tempo * 16.0f + offset) + 1.0f) * 0.5f * 0.12f; 
            }

            float qX = x;
            float qY = y + pulo;

            // quadradis
            // mapeamento de textura em vez de apenas vertex
            glTexCoord2f(0.0f, 0.0f); glVertex2f(qX - tam, qY - tam);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(qX + tam, qY - tam);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(qX + tam, qY + tam);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(qX - tam, qY + tam);
            glEnd();
        }
    }

    // reativa a textura para o resto do campo funcionar normal
    glEnable(GL_TEXTURE_2D);
    
    // link da arkimancada sul
    glBindTexture(GL_TEXTURE_2D, arquibancadaTextureB);
    glColor3f(1.0f, 1.0f, 1.0f);

    // arkibankada sulll
    float baseSul = -5.5f;
    float topoSul = baseSul - alturaProporcional;

    // arquibancada sulll
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-largura, topoSul);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( largura, topoSul);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( largura, baseSul);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-largura, baseSul);
    glEnd();

    // os neguinho agora pulam na sul tambem, do mesmo jeitinho que na norte
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor3f(1.0f, 1.0f, 1.0f);

    // inverte as coordenadas em Y para colocar os sprites sobre o concreto da sul
    float startY_Sul = -6.66f; // fileira Y da sul
    float endY_Sul = -7.0f;    // fileira Y da sul

    for (int row = 0; row < numFileiras; ++row) {
        for (int col = 0; col < numQuadradinhos; ++col) {
            
            // randomização baseada na seed
            int tipoTorcedor = abs(row * 11 + col * 17) % NUM_TORCEDORES;
            
            // define a textura baseada no estado do timer e tipoAleatorio (sprites do SUL agora)
            if (isComemorando) {
                glBindTexture(GL_TEXTURE_2D, torcedorSulComemoraTexture[tipoTorcedor]);
            } else {
                glBindTexture(GL_TEXTURE_2D, torcedorSulIdleTexture[tipoTorcedor]);
            }

            glBegin(GL_QUADS);
            // interpola linearmente a posição
            float x = startX + col * ((endX - startX) / (numQuadradinhos - 1));
            float y = startY_Sul + row * ((endY_Sul - startY_Sul) / (numFileiras - 1));
            
            float offset = x * 3.5f + y * 2.5f; 
            
            // mesmo pulo suave de antes (com a mesma lógica)
            float pulo = 0.0f;
            if (isComemorando) {
                pulo = (std::sin(tempo * 16.0f + offset) + 1.0f) * 0.5f * 0.12f; 
            }

            float qX = x;
            float qY = y + pulo;

            // mapeamento do sprite dos torcedores
            glTexCoord2f(0.0f, 0.0f); glVertex2f(qX - tam, qY - tam);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(qX + tam, qY - tam);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(qX + tam, qY + tam);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(qX - tam, qY + tam);
            glEnd();
        }
    }

    // reativa a textura normal para garantir que a leste/oeste apareça
    glEnable(GL_TEXTURE_2D);

    // link da arkibankada lestee
    glBindTexture(GL_TEXTURE_2D, arquibancadaTextureC);
    glColor3f(1.0f, 1.0f, 1.0f);

    // arkibankada leste conectando o topo da norte e o topo da sul exatamente
    float xInternoLeste = largura; // 3.5f
    float xExternoLeste = largura + alturaProporcional; // 3.5f + 2.33f = 5.83f (usa a mesma proporção de grossura)
    float yTopoLeste = 7.23f;
    float yBaseLeste = -7.23f;

    // arquibancada leste
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xInternoLeste, yBaseLeste); // inf esq 
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xExternoLeste, yBaseLeste); // inf dir 
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xExternoLeste, yTopoLeste); // sup dir 
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xInternoLeste, yTopoLeste); // sup esq
    glEnd();

    // os neguinho pulam na leste tambem, aproveitando as texturas do oeste de forma espelhada
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor3f(1.0f, 1.0f, 1.0f);

    float startX_Leste = 3.83f; // fileira X mais perto (concreto começa em 3.5)
    float endX_Leste = 5.03f;   // fileira X mais longe (concreto vai até 5.83)
    float startY_Lat = -3.4f;   // cobrir a lateral toda (campo de -7.23 a 7.23)
    float endY_Lat = 2.4f;
    int numFileiras_Lat = 2;    // duas fileiras na lateral tambem
    int numQuadradinhos_Lat = 12; // mais quadrados para cobrir toda extensão da lateral

    // randomização baseada na seed
    for (int row = 0; row < numFileiras_Lat; ++row) {
        for (int col = 0; col < numQuadradinhos_Lat; ++col) {
            
            // randomização baseada na seed
            int tipoTorcedor = abs(row * 19 + col * 23) % NUM_TORCEDORES;
            
            if (isComemorando) {
                glBindTexture(GL_TEXTURE_2D, torcedorOesteComemoraTexture[tipoTorcedor]);
            } else {
                glBindTexture(GL_TEXTURE_2D, torcedorOesteIdleTexture[tipoTorcedor]);
            }

            glBegin(GL_QUADS);
            float x = startX_Leste + row * ((endX_Leste - startX_Leste) / (numFileiras_Lat - 1));
            float y = startY_Lat + col * ((endY_Lat - startY_Lat) / (numQuadradinhos_Lat - 1));
            
            float offset = x * 3.5f + y * 2.5f; 
            float pulo = 0.0f;
            if (isComemorando) {
                pulo = (std::sin(tempo * 16.0f + offset) + 1.0f) * 0.5f * 0.12f; 
            }

            float qX = x;
            float qY = y + pulo;

            // mapeamento do sprite dos torcedores Leste (X INVERTIDO para flip horizontal)
            glTexCoord2f(1.0f, 0.0f); glVertex2f(qX - tam, qY - tam); // bottomLeft recebe original bottomRight
            glTexCoord2f(0.0f, 0.0f); glVertex2f(qX + tam, qY - tam); // bottomRight recebe original bottomLeft
            glTexCoord2f(0.0f, 1.0f); glVertex2f(qX + tam, qY + tam); // topRight recebe original topLeft
            glTexCoord2f(1.0f, 1.0f); glVertex2f(qX - tam, qY + tam); // topLeft recebe original topRight
            glEnd();
        }
    }

    // reativa a textura normal
    glEnable(GL_TEXTURE_2D);

    // link da arkibankada oestee
    glBindTexture(GL_TEXTURE_2D, arquibancadaTextureD);
    glColor3f(1.0f, 1.0f, 1.0f);

    // arkibankada oeste conectando o topo da norte e o topo da sul exatamente
    float xInternoOeste = -largura; // -3.5f
    float xExternoOeste = -(largura + alturaProporcional); // -5.83f
    float yTopoOeste = 7.23f;
    float yBaseOeste = -7.23f;

    // arquibancada oeste (espelhada)
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xExternoOeste, yBaseOeste); // inf esq 
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xInternoOeste, yBaseOeste); // inf dir 
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xInternoOeste, yTopoOeste); // sup dir 
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xExternoOeste, yTopoOeste); // sup esq
    glEnd();

    // os neguinho pulam na oeste tambem
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor3f(1.0f, 1.0f, 1.0f);

    float startX_Oeste = -5.16f; // fileira X mais longe (concreto vai até -5.83)
    float endX_Oeste = -3.83f;   // fileira X mais perto (concreto começa em -3.5)

    // randomização baseada na seed
    for (int row = 0; row < numFileiras_Lat; ++row) {
        for (int col = 0; col < numQuadradinhos_Lat; ++col) {
            
            // randomização baseada na seed
            int tipoTorcedor = abs(row * 29 + col * 31) % NUM_TORCEDORES;
            
            if (isComemorando) {
                glBindTexture(GL_TEXTURE_2D, torcedorOesteComemoraTexture[tipoTorcedor]);
            } else {
                glBindTexture(GL_TEXTURE_2D, torcedorOesteIdleTexture[tipoTorcedor]);
            }

            glBegin(GL_QUADS);
            float x = startX_Oeste + row * ((endX_Oeste - startX_Oeste) / (numFileiras_Lat - 1));
            float y = startY_Lat + col * ((endY_Lat - startY_Lat) / (numQuadradinhos_Lat - 1));
            
            float offset = x * 3.5f + y * 2.5f; 
            float pulo = 0.0f;
            if (isComemorando) {
                pulo = (std::sin(tempo * 16.0f + offset) + 1.0f) * 0.5f * 0.12f; 
            }

            float qX = x;
            float qY = y + pulo;

            // mapeamento normal do sprite dos torcedores oeste
            glTexCoord2f(0.0f, 0.0f); glVertex2f(qX - tam, qY - tam);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(qX + tam, qY - tam);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(qX + tam, qY + tam);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(qX - tam, qY + tam);
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);
}

void Campo::drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(num_segments);
        float x = r * std::cos(theta);
        float y = r * std::sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// desenhar as linhas do campo na vertical
void Campo::drawFieldLines() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(-3.0f, -5.0f);
        glVertex2f( 3.0f, -5.0f);
        glVertex2f( 3.0f,  5.0f);
        glVertex2f(-3.0f,  5.0f);
    glEnd();

    // meio do campo
    glBegin(GL_LINES);
        glVertex2f(-3.0f, 0.0f);
        glVertex2f( 3.0f, 0.0f);
    glEnd();

    // meio do meio do campo - o círculo
    drawCircle(0.0f, 0.0f, 1.0f, 36);

    // lado dibaxo - grande area
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.5f, -5.0f);
        glVertex2f(-1.5f, -3.5f);
        glVertex2f( 1.5f, -3.5f);
        glVertex2f( 1.5f, -5.0f);
    glEnd();

    // lado dibaxo 2 - pqeuna area
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.6f, -5.0f);
        glVertex2f(-0.6f, -4.2f);
        glVertex2f( 0.6f, -4.2f);
        glVertex2f( 0.6f, -5.0f);
    glEnd();

    // lado di cimah - grande area de novo wtf 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.5f,  5.0f);
        glVertex2f(-1.5f,  3.5f);
        glVertex2f( 1.5f,  3.5f);
        glVertex2f( 1.5f,  5.0f);
    glEnd();

    // lado di xima 2 - pquenea area 
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.6f,  5.0f);
        glVertex2f(-0.6f,  4.2f);
        glVertex2f( 0.6f,  4.2f);
        glVertex2f( 0.6f,  5.0f);
    glEnd();
}

// bool recebida e passada adiante
void Campo::draw(bool isComemorando) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    
    // esse repeats via definr quantas vezes a etxtura da grama vai se repetir
    float repeats = 15.0f; 

    // lógica de faixas
    int numFaixas = 11; // 11 faixas distribuidas ao longo (nisso bem distribuído de -5.5 a 5.5)
    float campoInicioY = -5.5f;
    float campoFimY = 5.5f;
    float alturaFaixa = (campoFimY - campoInicioY) / numFaixas;

    glBegin(GL_QUADS);
    for (int i = 0; i < numFaixas; ++i) {
        float yBase = campoInicioY + i * alturaFaixa;
        float yTopo = yBase + alturaFaixa;

        // altaernancia de cores de casa lado
        if (i % 2 == 0) {
            glColor3f(1.0f, 1.0f, 1.0f); // tim base da textura - claro
        } else {
            glColor3f(0.85f, 0.90f, 0.85f); // multiplika a textura por um tom mais escuro
        }

        // calkuloda coordenada UV da textura para que ela não quebre na divisa das faixas
        float vBase = (yBase - campoInicioY) / (campoFimY - campoInicioY) * repeats;
        float vTopo = (yTopo - campoInicioY) / (campoFimY - campoInicioY) * repeats;

        // faixa horizontal
        glTexCoord2f(0.0f, vBase);       glVertex2f(-3.5f, yBase);
        glTexCoord2f(repeats, vBase);    glVertex2f( 3.5f, yBase);
        glTexCoord2f(repeats, vTopo);    glVertex2f( 3.5f, yTopo);
        glTexCoord2f(0.0f, vTopo);       glVertex2f(-3.5f, yTopo);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    drawFieldLines();

    // Desenhando a arquibancada por baixo
    // propaga o estado para a arquibancada
    drawArquibancada(isComemorando);
}

void Campo::resolverColisaoLimites(float& posX, float& posY, float raio) {
    float limiteX = 3.5f; // limites laterais do gramado verde
    float limiteY = 5.5f; // limites superior e inferior do gramado

    // Colisão com as laterais (X)
    if (posX - raio < -limiteX) {
        posX = -limiteX + raio;
    } else if (posX + raio > limiteX) {
        posX = limiteX - raio;
    }

    // Colisão com as linhas de fundo (Y)
    if (posY - raio < -limiteY) {
        posY = -limiteY + raio;
    } else if (posY + raio > limiteY) {
        posY = limiteY - raio;
    }
}