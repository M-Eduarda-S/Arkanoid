#include <iostream>
#include <windows.h> //Biblioteca para funções do Windows
#include <time.h> //gerar numeros aleatorios
#include <cstdlib>  //pausar e limpar o terminal
#include <conio.h>
#include <fstream>  //Biblioteca arquivo

using namespace std;

#define TAM_L 25
#define TAM_C 40
#define ESTADO_QUEBRANDO 11
#define MAX_LINHAS 20
#define MAX_NOME 100

enum TipoBloco { //uma forma de enumerar coisas, ao invez de fazer vaios const int, facilita tb na hr de por na struct
    NORMAL = 2,
    FORTE1 = 3,
    FORTE2 = 4,
    VIDA = 5,
    AUMENTA_PADDLE = 6,
    DIMINUI_PADDLE = 7,
    MULTIPLICADOR_PONTOS = 8,
    INVERSOR_PONTUACAO = 9,
    PONTUACAO_EXTRA = 10
};

struct ConfiguracaoJogo {
    int velocidade; // Velocidade do jogo
    int corBola; // Cor da bola
    int corPersonagem; // Cor do personagem
    int corParede;
};


//parametro dificuldade jogo
ConfiguracaoJogo obterConfiguracao(int dificuldade) {
    switch (dificuldade) {
    case 1: // modo facil
        return { 100, 10, 5, 7 };
    case 2: // modo medio
        return { 75, 12, 11, 6 };
    case 3: // modo dificil
        return { 50, 7, 14, 4 };
    case 4: // modo automatico
        return { 100, 10, 5, 7 };
    default:
        return { 100, 10, 5, 7 }; // Padrão
    }
}

//struct bloco
struct Bloco {
    int x;
    int y;
    bool ativo; //indica se estaativo ou nao
    TipoBloco tipo; // Adiciona o tipo de bloco
};

//parametro que gera os blocos
void gerar_blocos(Bloco blocos[], int m[TAM_L][TAM_C], int fase) {
    int blocos_totais = 0;
    int bloco_especifico = 0;
    bool bloco_forte1 = false;
    bool vida = false;
    bool aumenta_paddle = false;
    bool diminui_paddle = false;
    bool multiplcador_pontos = false;
    bool inversor_pontuacao = false;
    bool pontuacao_extra = false;
    int num_blocos = 0;
    int espaco = 0;

    switch (fase) {
    case 1:
        espaco = 4; // Blocos mais próximos
        num_blocos = 50;
        break;
    case 2:
        espaco = 7; // Blocos mais espalhados
        num_blocos = 0;
        break;
    case 3:
        espaco = 9; // Blocos ainda mais espalhados
        num_blocos = 100;
        break;
    }

    while (blocos_totais < num_blocos) {
        int randX = rand() % espaco + 2; // Gera um número aleatorio de espacos (+2 para dar o espaco em cima)
        int randY = rand() % (TAM_C - 2) + 1; // Gera um número aleatório entre 1 e TAM_C-2

        // Verifica se o espaço está vazio
        if (m[randX][randY] == 0) {
            if (bloco_forte1 == false) {
                blocos[blocos_totais] = { randX, randY, true, FORTE1 }; // Define o bloco como ativo
                m[randX][randY] = 3; // Marca no mapa como valor 3
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 5) { // 10 blocos forte
                    bloco_forte1 = true;
                    bloco_especifico = 0;
                }
            }
            else if (vida == false) { // gera vida
                blocos[blocos_totais] = { randX, randY, true, VIDA }; // Define o bloco como ativo
                m[randX][randY] = 5; // Marca no mapa com valor 5
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 15) { // 2 blocos de vida
                    vida = true;
                    bloco_especifico = 0;
                }
            }
            else if (aumenta_paddle == false) { //aumenta paddle
                blocos[blocos_totais] = { randX, randY, true, AUMENTA_PADDLE }; // Define o bloco como ativo
                m[randX][randY] = 6; // Marca no mapa com valor 6
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 2) { // apenas 1 aumenta paddle
                    aumenta_paddle = true;
                    bloco_especifico = 0;
                }
            }
            else if (diminui_paddle == false) { //aumenta paddle
                blocos[blocos_totais] = { randX, randY, true, DIMINUI_PADDLE }; // Define o bloco como ativo
                m[randX][randY] = 7; // Marca no mapa com valor 7
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 1) { // apenas 1 diminui paddle
                    diminui_paddle = true;
                    bloco_especifico = 0;
                }
            }
            else if (multiplcador_pontos == false) { //aumenta paddle
                blocos[blocos_totais] = { randX, randY, true, MULTIPLICADOR_PONTOS }; // Define o bloco como ativo
                m[randX][randY] = 8; // Marca no mapa com valor 8
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 3) { // apenas 3
                    multiplcador_pontos = true;
                    bloco_especifico = 0;
                }
            }
            else if (inversor_pontuacao == false) { //aumenta paddle
                blocos[blocos_totais] = { randX, randY, true, INVERSOR_PONTUACAO }; // Define o bloco como ativo
                m[randX][randY] = 9; // Marca no mapa com valor 9
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 10) { // apenas 1 bloco
                    inversor_pontuacao = true;
                    bloco_especifico = 0;
                }
            }
            else if (pontuacao_extra == false) { //aumenta paddle
                blocos[blocos_totais] = { randX, randY, true, PONTUACAO_EXTRA }; // Define o bloco como ativo
                m[randX][randY] = 10; // Marca no mapa com valor 10
                blocos_totais++;
                bloco_especifico++;
                if (bloco_especifico == 1) { // apenas 1 bloco
                    pontuacao_extra = true;
                    bloco_especifico = 0;
                }
            }
            else { // adiciona blocos normais
                blocos[blocos_totais] = { randX, randY, true, NORMAL }; // Define o bloco como ativo
                m[randX][randY] = 2; // Marca no mapa como bloco
                blocos_totais++;

            }

        }
    }
}

void exibir_info_itens_pegos() {

    cout << "Voce coletou 1 vida adicional";
}

struct Item {
    int x, y; // Posição do item
    TipoBloco tipo; // Tipo do item (igual ao tipo do bloco)
    bool ativo = false; // Se o item está em queda
};
Item itens[10]; // Inicialização dos itens


// Função para soltar um item quando um bloco especial é quebrado
void soltarItem(int x, int y, TipoBloco tipo) {
    for (int i = 0; i < 10; i++) {
        if (!itens[i].ativo) {
            itens[i] = { x, y, tipo, true }; // Inicializa o item com a posição correta
            break;
        }
    }
}

//void pintuacao inversa item
void pontuacao_inversa(int& pontuacao) {
    //faz a separacao dos numeros por mod
    int num1 = pontuacao / 10000;
    int num2 = (pontuacao / 1000) % 10;
    int num3 = (pontuacao / 100) % 10;
    int num4 = (pontuacao / 10) % 10;
    int num5 = (pontuacao / 1) % 10;

    if (pontuacao > 10000) { //verifica o tamanho da pontuacao
        pontuacao = (num5 * 10000) + (num4 * 1000) + (num3 * 100) + (num2 * 10) + num1; //atualiza valor da pontuacao

    }
    else if (pontuacao > 1000) {
        pontuacao = (num5 * 1000) + (num4 * 100) + (num3 * 10) + num2;

    }
    else if (pontuacao > 100) {
        pontuacao = (num5 * 100) + (num4 * 10) + num3;

    }
    else if (pontuacao > 10) {
        pontuacao = (num5 * 10) + num4;

    }
    else {
        pontuacao = num5;
    }
}

// Função que aplica o efeito dos itens
void aplicarEfeitoItem(Item& item, int& vida, int& tamanhoPaddle, int& pontuacao, string& mensagemItemColetado) {
    mensagemItemColetado = " ";
    switch (item.tipo) {
    case VIDA:
        vida++;
        mensagemItemColetado = "Voce coletou 1 vida adicional!          ";
        break;
    case AUMENTA_PADDLE:
        tamanhoPaddle = min(tamanhoPaddle + 2, TAM_C - 1);
        mensagemItemColetado = "Voce coletou um aumento de paddle!      ";
        break;
    case DIMINUI_PADDLE:
        tamanhoPaddle = max(tamanhoPaddle - 2, 3);
        mensagemItemColetado = "Voce coletou uma diminuicao de paddle!  ";
        break;
    case MULTIPLICADOR_PONTOS:
        pontuacao *= 2;
        mensagemItemColetado = "Voce coletou um multiplicador de pontos!";
        break;
    case INVERSOR_PONTUACAO:
        pontuacao_inversa(pontuacao);
        mensagemItemColetado = "Voce coletou um inversor de pontuacao!  ";
        break;
    case PONTUACAO_EXTRA:
        pontuacao += 10;
        mensagemItemColetado = "Voce coletou pontos extras!             ";
        break;
    default:
        mensagemItemColetado = "";
        break;
    }
}
// Função que move os itens para baixo e aplica efeitos quando são coletados pelo paddle
void moverItens(int& y, int& vida, int& tamanhoPaddle, int& pontuacao, bool& inversorPontuacao, string& mensagemItemColetado) {
    for (int i = 0; i < 10; i++) {
        if (itens[i].ativo) {
            itens[i].x += 1;

            // Verifica se o item colidiu com o paddle
            if (itens[i].x == 23 && itens[i].y >= y && itens[i].y < y + tamanhoPaddle) {
                aplicarEfeitoItem(itens[i], vida, tamanhoPaddle, pontuacao, mensagemItemColetado);
                itens[i].ativo = false;  // Desativa o item após coleta
            }
            else if (itens[i].x >= TAM_L - 1) {
                itens[i].ativo = false;  // Desativa o item se ele saiu da tela
            }
        }
    }
}

//subrotina menu principal
int inicio(int opcoes) {
    bool primeira_tentativa = false; //Controla se é a primeira tentativa de escolher uma opção

    do {
        if (primeira_tentativa == false) {
            cout << "Bem-vindo ao arcanoid, selecione um numero:\n\n 1- Sobre o jogo \n 2- Jogar \n 3- Criadores \n 4- placar\n 5- infos tecnicas \n 0- Sair \n";
            cin >> opcoes;
            primeira_tentativa = true; // Marca que a primeira tentativa foi realizada
        }
        else {
            system("cls"); //Limpa a tela
            cout << "Numero invalida!! Tente novamente.\n\n 1- Sobre o jogo \n 2- Jogar \n 3- Criadores \n 4- placar\n 5- infos tecnicas \n 0- Sair \n ";
            cin >> opcoes;
        }
    } while (opcoes < 0 || opcoes > 5); //Continua até uma opção válida ser escolhida

    return opcoes;
}

// subrotina menu dificuldade
int dificuldade(int opcao_dificuldade) {
    bool primeira_tentativa = false; //Controla se é a primeira tentativa de escolher uma opção

    do {
        if (primeira_tentativa == false) {
            cout << "Selecione o modo de jogo\n 0- voltar \n 1- Facil\n 2- Medio\n 3- Dificil\n 4- Automatico\n";
            cin >> opcao_dificuldade;
            primeira_tentativa = true; // Marca que a primeira tentativa foi realizada
        }
        else {
            system("cls"); //Limpa a tela
            cout << "Modo invalido!! Tente novamente.\n\n 0- voltar \n 1- Facil\n 2- Medio\n 3- Dificil\n 4- Automatico \n";
            cin >> opcao_dificuldade;
        }
    } while (opcao_dificuldade < 0 || opcao_dificuldade > 4); //Continua até uma opção válida ser escolhida

    return opcao_dificuldade;

}

//Subrotina que exibe informações sobre o jogo
void mensagem_sobre() {
    cout << "Sobre o jogo. \n\nArkanoid e um jogo classico de arcade lancado em 1986, onde o jogador controla uma barra na parte inferior da tela para rebater uma bola, para assim destruir blocos na parte superior, tendo como objetivo final e eliminar todos os blocos sem deixar a bola cair. \n\n";
}

//Subrotina que exibe os créditos do jogo
void mensagem_creditos() {
    cout << "Equipe e colaboradores do jogo:\n\nProfessor: Alex Luciano Roesler Rese \nAlunos: Maria Eduarda Santos, Thiago Moraes Ludvig, Yasmin Tarnovski Faccin. \nData: 24/10/2024 \n\nO que cada participante fez: " << endl;
    cout << "Maria Eduarda: responsavel por ajustar a colisao da bola com as paredes, cuidou da geracao e estetica do mapa" << endl;
    cout << "Thiago: responsavelpor fazer os blocos quebrarem, a bola rebate quando bate no personagem e condicao de vitoria e derrota" << endl;
    cout << "Yasmin: Ficou responsavel por fazer o menu, encrementou a pontuacao e movimentacao da bola" << endl;
}

//subrotina que exibe mensagem com infos tencnicas
void mensagem_infos_tecnincas() {
    cout << "=========Tipo de itens==========" << endl << endl << "-> Vida: bloco tipo ( " << char(158) << " ), jogador pode ter ate 2 vidas por fase. Pontuacao = +1" << endl << "-> Aumento do personagem: bloco tipo ( " << char(205) << " ), o jogador ao pegar o efeito, tendo como aumento 2 casas de cada lado. Pontuacao = +1" << endl << "-> Diminuicao do personagem: bloco tipo ( " << char(95) << " ), o jogador ao pegar o efeito, tendo como diminuicao 2 casas de cada lado. Pontuacao = +1" << endl;
    cout << "-> Pontos extras: bloco tipo ( " << char(42) << " ), o jogador ao pegar o efeito, recebera uma pontuacao extra. Pontuacao = +10" << endl << "-> Multiplicador de pontos: bloco tipo ( " << char(50) << " ), o jogador ao pegar o efeito, recebera uma multipliacao por 2 em seus blocos destruidos. Pontuacao = +2x" << endl << "-> Inversor de pontuacao: bloco tipo ( " << char(241) << " ), o jogador ao pegar o efeito, recebera uma inversao em sua pontuacao atual. Pontuacao = inverso" << endl << endl;
    cout << "============Pontuacao============" << endl << endl << "-> Bloco normal: pontuacao =+ 1" << endl << "-> Blocos resistentes: pontuacao =+ 3" << endl << endl;
    cout << "ATENCAO!!! ao mudar de fase, os itens pegos sao RESETADOS!!!" << endl << "Porem, sua pontuacao permanecera a mesma ate o fim do jogo. " << endl << endl;
}
void salvarPlacar(int pontuacao, int minuto, int segundo, string nome_jogador, int fase_atual) {
    time_t now = time(0);
    struct tm ltm;  // Variável para armazenar a data/hora
    localtime_s(&ltm, &now);  // Passando um ponteiro para a variável 'ltm'

    // Formata a data como string
    char data[20];
    strftime(data, sizeof(data), "%d-%m-%Y", &ltm); // Formato: dia-mes-ano

    ofstream arquivo("placar.txt", ios::app); // Abre o arquivo em modo de adição (append)
    if (arquivo.is_open()) {
        arquivo << nome_jogador << " | " << pontuacao << " | " << minuto << ":" << segundo << " | " << data << " | " << "fase: " << fase_atual << endl; // Salva a pontuação no arquivo
        arquivo.close();
    }
    else {
        cerr << "Erro ao abrir o arquivo de placar!" << endl;
    }
}

void mesclar(char linhas[][MAX_NOME], int mid, int n) {
    char temp[MAX_LINHAS][MAX_NOME];
    int i = 0, j = mid, k = 0;

    // Mescla duas metades de linhas
    while (i < mid && j < n) {
        int pontuacaoI = 0, pontuacaoJ = 0;

        // Extrai a pontuação da linha i
        for (int l = 0; linhas[i][l] != '\0'; l++) {
            if (linhas[i][l] == '|') {
                l++; // Move para o próximo caractere após '|'
                while (linhas[i][l] != '\0' && linhas[i][l] != '|') {
                    pontuacaoI = pontuacaoI * 10 + (linhas[i][l] - '0');
                    l++;
                }
                break;
            }
        }

        // Extrai a pontuação da linha j
        for (int l = 0; linhas[j][l] != '\0'; l++) {
            if (linhas[j][l] == '|') {
                l++; // Move para o próximo caractere após '|'
                while (linhas[j][l] != '\0' && linhas[j][l] != '|') {
                    pontuacaoJ = pontuacaoJ * 10 + (linhas[j][l] - '0');
                    l++;
                }
                break;
            }
        }

        // Copia a linha com maior pontuação
        if (pontuacaoI >= pontuacaoJ) {
            strcpy_s(temp[k], linhas[i]); // Copia a linha inteira
            i++;
        }
        else {
            strcpy_s(temp[k], linhas[j]); // Copia a linha inteira
            j++;
        }
        k++;
    }

    // Copia as linhas restantes
    while (i < mid) {
        strcpy_s(temp[k], linhas[i]);
        i++;
        k++;
    }
    while (j < n) {
        strcpy_s(temp[k], linhas[j]);
        j++;
        k++;
    }

    // Copia as linhas mescladas de volta para o array original
    for (i = 0; i < n; i++) {
        strcpy_s(linhas[i], temp[i]); // Cópia correta
    }
}

void ordenarPlacar(char linhas[][MAX_NOME], int n) {
    if (n <= 1) return;

    int mid = n / 2;

    ordenarPlacar(linhas, mid);
    ordenarPlacar(linhas + mid, n - mid);
    mesclar(linhas, mid, n);
}

void carregarPlacar() {
    ifstream arquivo("placar.txt");
    if (arquivo.is_open()) {
        char linhas[MAX_LINHAS][MAX_NOME];
        int contador = 0;

        cout << "===== Placar =====" << endl;
        cout << "Nome | Pontuacao | Tempo | Data | Fase " << endl;

        while (contador < MAX_LINHAS && arquivo.getline(linhas[contador], MAX_NOME)) {
            contador++;
        }
        arquivo.close();

        ordenarPlacar(linhas, contador);

        for (int i = 0; i < contador; ++i) {
            cout << linhas[i] << endl; // Exibe a linha completa
        }
    }
    else {
        cout << "Erro ao abrir o arquivo de placar!" << endl;
    }
}


//verifica se todos os blocos foram quebrados
bool todos_blocos_quebrados(int m[TAM_L][TAM_C]) {
    for (int i = 0; i < TAM_L; i++) {
        for (int j = 0; j < TAM_C; j++) {
            if (m[i][j] == 2 || m[i][j] == 3 || m[i][j] == 4 || m[i][j] == 5 ||
                m[i][j] == 6 || m[i][j] == 7 || m[i][j] == 8 || m[i][j] == 9 ||
                m[i][j] == 10) { // Verifica se ainda há blocos restantes
                return false; // Se encontrar um bloco ativo, retorna false
            }
        }
    }
    return true; // Se não encontrar blocos ativos, retorna true
}

void mapa_bruto_jogo(int m[TAM_L][TAM_C]) {
    for (int i = 0; i < TAM_L;i++) {
        for (int j = 0; j < TAM_C; j++) {
            if (i == 0 || i == TAM_L - 1 || j == 0 || j == TAM_C - 1) {
                m[i][j] = 1;
            }
            else {
                m[i][j] = 0;
            }
        }
    }

}

void mapa_da_matriz(int m[TAM_L][TAM_C], int x, int y, int bolaY, int bolaX, char* personagem, int cor_personagem, int cor_bola, int cor_parede) {
    for (int i = 0; i < TAM_L; i++) {
        for (int j = 0; j < TAM_C; j++) {
            bool item_imprimido = false;

            // Verifica se algum item ativo está na posição atual
            for (int k = 0; k < 10; k++) {
                if (itens[k].ativo && itens[k].x == i && itens[k].y == j) {
                    // Exibe um caractere específico dependendo do tipo de item
                    switch (itens[k].tipo) {
                    case VIDA:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // Cor vida
                        cout << char(158); // Item de vida
                        break;
                    case AUMENTA_PADDLE:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
                        cout << char(205); // Item de aumento de paddle
                        break;
                    case DIMINUI_PADDLE:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                        cout << char(95);  // Item de diminuição de paddle
                        break;
                    case MULTIPLICADOR_PONTOS:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                        cout << char(50);  // Item multiplicador de pontos
                        break;
                    case INVERSOR_PONTUACAO:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        cout << char(241); // Item inversor de pontuação
                        break;
                    case PONTUACAO_EXTRA:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                        cout << char(42);  // Item de pontuação extra
                        break;
                    default:
                        cout << char(36);  // Caractere padrão para itens desconhecidos
                        break;
                    }
                    item_imprimido = true;
                    break;
                }
            }

            if (!item_imprimido) {
                // Exibe o restante do mapa normalmente
                if (m[i][j] == ESTADO_QUEBRANDO) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    cout << char(220);
                    Sleep(50);
                    m[i][j] = 0;
                }
                else if (i == bolaY && j == bolaX) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor_bola);
                    cout << char(79); // Bola
                }
                else if (i == x && j >= y && j < y + 10) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor_personagem);
                    cout << personagem[j - y];
                }
                else {
                    switch (m[i][j]) {
                    case 0:
                        cout << " "; // Espaço vazio
                        break;
                    case 1:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor_parede); // Cor das paredes
                        cout << char(219); // Paredes
                        break;
                    case 2:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); // Cor dos blocos
                        cout << char(207); // Blocos
                        break;
                    case 3:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Cor dos blocos fortes quebrando
                        cout << char(207);
                        break;
                    case 4:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); // Cor dos blocos fortes
                        cout << char(207);
                        break;
                    case 5:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // Bloco de vida
                        cout << char(207);
                        break;
                    case 6:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); // Bloco aumenta paddle
                        cout << char(207);
                        break;
                    case 7:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); // Bloco diminui paddle
                        cout << char(207);
                        break;
                    case 8:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // Bloco multiplicador de pontos
                        cout << char(207);
                        break;
                    case 9:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Bloco inversor de pontos
                        cout << char(207);
                        break;
                    case 10:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Bloco pontuação extra
                        cout << char(207);
                        break;
                    }
                }
            }
        }
        cout << "\n";
    }
}

// Função sobrecarregada para incluir a pontuação
void mapa_da_matriz(int pontuacao, const string& mensagemItemColetado) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "Pontos: " << pontuacao << endl; // Imprime a pontuação
    if (!mensagemItemColetado.empty()) {
        cout << mensagemItemColetado << endl; // Exibe a mensagem do item coletado
    }
}

void DirecaoAleatoriaBola(int& bolaDirX, int& bolaDirY, bool& primeira_vez, int& direcao_aleatoria) {
    if (primeira_vez) {
        srand(time(NULL));
        direcao_aleatoria = rand() % 4 + 1; // gera um numero aleatorio para definir a direcao

        // Condição para que a bola movimente-se aleatoriamente no início do jogo
        switch (direcao_aleatoria) {
        case 1:
            bolaDirX = 1;
            bolaDirY = -1;
            break;
        case 2:
            bolaDirX = -1;
            bolaDirY = -1;
            break;
        case 3:
            bolaDirX = 1;
            bolaDirY = 1;
            break;
        case 4:
            bolaDirX = -1;
            bolaDirY = 1;
            break;
        }

        primeira_vez = false; // Seta para false após a primeira direção ser definida
    }
}

void cronometro(int& contador_tempo, int& hora, int& minuto, int& segundo) {
    if (segundo >= 60) { // Verifica se o segundo atingiu 60
        segundo = 0; // Deve ser 0, e não 00 (número inteiro)
        minuto++;
    }
    if (minuto >= 60) { // Verifica se o minuto atingiu 60
        minuto = 0;
        hora++;
    }
    if (hora < 10) {
        cout << "0" << hora << " : ";
    }
    else {
        cout << hora << " : ";
    }

    if (minuto < 10) {
        cout << "0" << minuto << " : ";
    }
    else {
        cout << minuto << " : ";
    }

    if (segundo < 10) {
        cout << "0" << segundo << endl;
    }
    else {
        cout << segundo << endl;
    }
    if (contador_tempo <= 4) { // Atualização dos segundos
        contador_tempo++;
    }
    else {
        contador_tempo = 0;
        segundo++;
    }
}

// Função recursiva para contagem regressiva
void contagem_regressiva(int segundos) {
    if (segundos == 0) {
        return;
    }
    else {
        cout << endl << "Iniciando a proxima fase em " << segundos << " segundos..." << endl;
        Sleep(1000);
        system("cls");
        contagem_regressiva(segundos - 1); // chamada recursiva
    }
}
// Função template para exibir pontuação e fase
template <typename T>
void exibir_info_fase(T pontuacao, int fase) {
    system("cls");
    cout << "Parabens! Voce completou a fase " << fase << "." << endl;
    cout << "Pontuacao atual: " << pontuacao << endl << endl;
    Sleep(3000);
}

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
//INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
    //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    const int LARGURA = TAM_C; //Largura do campo de jogo
    const int ALTURA = TAM_L; //Altura do campo de jogo
    int bolaX = LARGURA / 2; //Posicao inicial da bola
    int bolaY = ALTURA / 2; //Posicao inicial da bola
    int bolaDirX = 1;  //Direcao da bola
    int bolaDirY = -1; //Direcao da bola
    int pontuacao = 0; // Variavel da pontuacao
    bool primeira_vez = true; // condicao relacionada a direcao aleatoria da bola
    int direcao_aleatoria = 0; // Direcao da bola
    bool game_over = false; //condicao do jogo
    bool vitoria = false;
    int NUM_FASES = 3;
    int fase_atual = 1; // Começamos na fase 1
    string nome_jogador;
    int vida = 4;
    int tamanhoPaddle = 10;       // Tamanho inicial do paddle
    bool inversorPontuacao = false; // Controle do inversor de pontuação
    string mensagemItemColetado;

    // mapa do jogo
    int m[TAM_L][TAM_C] = {};
    mapa_bruto_jogo(m);

    // Definindo blocos
    Bloco blocos[100];

    int x = 23, y = (LARGURA - 10) / 2; //Posição do personagem
    char tecla;
    char personagem[] = { char(205), char(205), char(205), char(205), char(205), char(205), char(205), char(205), char(205), char(205), char(205) }; //Personagem

    int opcoes = -1; // opcao menu
    int retorno_opcoes = 0; // retorno para o case
    int opcao_dificuldade = 0; // menu de dificuldade de jogo
    int retorno_opcoes_dificuldade = 0; //retorna para o case

    int segundo = 0, minuto = 0, hora = 0; // variavies do timer
    int contador_tempo = 0; //gambiarra para funcionar o segundo de forma correta

    int velocidade_jogo = 0, cor_personagem = 0, cor_bola = 0, cor_parede = 0;
    bool jogo_automatico = false;

    ConfiguracaoJogo config;
    //Loop principal do jogo
    do {
        retorno_opcoes = inicio(opcoes); //Chama a função de início para obter a opção
        system("cls");

        //Switch para tratar a opção escolhida
        switch (retorno_opcoes) {
        case 1: //Sobre o jogo
            mensagem_sobre(); // subrotina com a mensagem
            system("pause");// pausa o sistema
            system("cls");//limpa o sistema
            break;

        case 2: //Jogo
            retorno_opcoes_dificuldade = dificuldade(opcao_dificuldade);
            if (retorno_opcoes_dificuldade == 0) {
                system("cls");
                break;
            }
            if (retorno_opcoes_dificuldade == 4) {
                jogo_automatico = true;
            }

            // Inicialize aqui
            mapa_bruto_jogo(m);
            config = obterConfiguracao(retorno_opcoes_dificuldade);
            velocidade_jogo = config.velocidade;
            cor_personagem = config.corPersonagem;
            cor_bola = config.corBola;
            cor_parede = config.corParede;

            gerar_blocos(blocos, m, fase_atual); // Passa a matriz e o array de blocos

            system("cls");// limpa terminal

            //Loop do jogo
            while (game_over == false) {

                cronometro(contador_tempo, hora, minuto, segundo); // cronometro
                cout << "Vidas extras: " << vida << endl; // vida
                // Chamada para imprimir o mapa sem a pontuação
                mapa_da_matriz(m, x, y, bolaY, bolaX, personagem, cor_personagem, cor_bola, cor_parede);

                // Quando quiser mostrar a pontuação
                mapa_da_matriz(pontuacao, mensagemItemColetado);
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

                //Movimentacao do personagem
                if (jogo_automatico) {
                    int nova_y = bolaX - 5; // Nova posição do personagem
                    if (nova_y >= 0 && nova_y + 10 < LARGURA) { // Verifica se a nova posição está dentro dos limites
                        // Verifica se a nova posição não colide com uma parede
                        bool podeMover = true;
                        for (int j = nova_y; j < nova_y + 10; j++) {
                            if (m[x][j] == 1) { // 1 representa parede
                                podeMover = false;
                                break;
                            }
                        }
                        // Atualiza a posição se for válida
                        if (podeMover) {
                            y = nova_y;
                        }
                    }
                }
                else if (_kbhit()) {
                    tecla = _getch();
                    if (tecla == 'a' || tecla == 'A') {
                        if (y - 2 > 0) { //Verifica se é possivel o personagem ir para esta direcao
                            y -= 2; //Atualiza posicao do personagem
                        }
                    }
                    else if (tecla == 'd' || tecla == 'D') {
                        if (y + 12 < LARGURA) { //Verifica se é possivel o personagem ir para esta direcao
                            y += 2; //Atualiza posicao do personagme
                        }
                    }
                }


                //Direção aleatória da bola na primeira vez
                DirecaoAleatoriaBola(bolaDirX, bolaDirY, primeira_vez, direcao_aleatoria);


                //Movimentação da bola
                bolaX += bolaDirX; //Atualiza a posição X da bola
                bolaY += bolaDirY; //Atualiza a posição Y da bola

                //Verifica colisões com as paredes
                if (bolaX <= 1 || bolaX >= LARGURA - 2) {
                    bolaDirX = -bolaDirX; //Inverte a direção X da bola
                }
                if (bolaY <= 1) {
                    bolaDirY = -bolaDirY; // Inverte a direção Y da bola
                }
                //Se a bola passar do limite inferior, o jogo acaba caso o jogador nao tenha vidas
                if (bolaY >= ALTURA - 1) {
                    if (vida >= 1) {
                        vida--;
                        bolaX = LARGURA / 2; //Posicao inicial da bola
                        bolaY = ALTURA / 2; //Posicao inicial da bola
                        bolaDirX = 1;  //Direcao da bola
                        bolaDirY = -1; //Direcao da bola
                        DirecaoAleatoriaBola(bolaDirX, bolaDirY, primeira_vez, direcao_aleatoria);
                    }
                    else {
                        game_over = true;
                        cout << "Game Over! A bola passou para baixo do personagem." << endl;
                    }
                }
                switch (m[bolaY][bolaX]) {
                case 2: // Bloco normal
                    m[bolaY][bolaX] = ESTADO_QUEBRANDO;
                    bolaDirY = -bolaDirY;
                    pontuacao += 1;
                    break;

                case 3: // Bloco forte quebrando
                    m[bolaY][bolaX] -= 1;
                    bolaDirY = -bolaDirY;
                    break;

                case 4: // Bloco forte quebrado
                    m[bolaY][bolaX] = ESTADO_QUEBRANDO;
                    bolaDirY = -bolaDirY;
                    pontuacao += 3;
                    break;

                    // Blocos especiais (5 a 10), que soltam itens quando quebrados
                case 5: // Vida
                case 6: // Aumenta paddle
                case 7: // Diminui paddle
                case 8: // Multiplicador de pontos
                case 9: // Inversor de pontuação
                case 10: // Pontuação extra
                    soltarItem(bolaY, bolaX, static_cast <TipoBloco>(m[bolaY][bolaX])); // Solta o item
                    m[bolaY][bolaX] = ESTADO_QUEBRANDO;
                    bolaDirY = -bolaDirY;
                    pontuacao += 1; // Ajuste a pontuação conforme necessário para cada tipo
                    break;
                }
                moverItens(y, vida, tamanhoPaddle, pontuacao, inversorPontuacao, mensagemItemColetado);

                // Verifica possivel mudanca na direcao da bola
                if (bolaY == x - 1 && bolaX >= y && bolaX < y + 10) {
                    bolaDirY = -bolaDirY;
                    if (bolaX >= y + 10) { // Verifica se a bolinha está se aproximando do lado direito do personagem
                        if (bolaX + bolaDirX >= LARGURA - 1) { // Se a bolinha está na posição da direita do personagem e perto da parede
                            bolaDirX = -1; // Inverte a direção X
                        }
                        else {
                            int posicao_relativa = bolaX - y;
                            if (posicao_relativa < 5) {
                                bolaDirX = -1; // Se bate na parte esquerda do personagem, vai para a esquerda
                            }
                            else {
                                bolaDirX = 1; // Caso contrário, vai para a direita
                            }
                        }
                    }
                }
                Sleep(velocidade_jogo); // Use a velocidade do jogo definida

                // Verificar se todos os blocos foram destruídos
                if (todos_blocos_quebrados(m)) {
                    if (fase_atual < NUM_FASES) {
                        exibir_info_fase(pontuacao, fase_atual); // Exibe a pontuação e a fase
                        contagem_regressiva(5); // Contagem regressiva para próxima fase
                        fase_atual++; // Passa para a próxima fase
                        system("cls");

                        // Reinicia o mapa e gera novos blocos
                        mapa_bruto_jogo(m);
                        gerar_blocos(blocos, m, fase_atual); // Atualiza os blocos para a nova fase
                    }
                    else {
                        // Se todos os blocos foram quebrados e estamos na última fase
                        system("cls");
                        cout << "Parabens! Voce ganhou, com " << pontuacao << " pontos, e com " << minuto << ":" << segundo << " minutos" << endl;
                        system("pause");
                        if (jogo_automatico == false) { //verifica se nao esta falso para salvar as informacoes do jogador
                            cout << "Digite seu nome (sera visto para outros jogadores): " << endl;
                            cin >> nome_jogador;
                            salvarPlacar(pontuacao, minuto, segundo, nome_jogador, fase_atual);
                        }
                        vitoria = true;
                        game_over = true; // Finaliza o jogo
                    }
                }
            }

            system("cls"); // limpa terminal
            if (vitoria == false) {
                cout << "GAME OVER! Voce conseguiu chegar ate a fase " << fase_atual << " com " << pontuacao << " pontos, e com um tempo de " << minuto << ":" << segundo << " minutos " << endl;
                if (jogo_automatico == false) {
                    cout << "Digite seu nome (sera visto para outros jogadores): " << endl;
                    cin >> nome_jogador;
                    salvarPlacar(pontuacao, minuto, segundo, nome_jogador, fase_atual);
                }
                system("pause");// pausa o sistema
                system("cls");//limpa o sistema
            }
            /// reseta o jogo
            segundo = 0;
            minuto = 0;
            hora = 0; //
            contador_tempo = 0; //
            x = 23;
            y = (LARGURA - 10) / 2; //Posição do personagem
            bolaX = LARGURA / 2; //Posicao inicial da bola
            bolaY = ALTURA / 2; //Posicao inicial da bola
            bolaDirX = 1;  //Direcao da bola
            bolaDirY = -1; //Direcao da bola
            pontuacao = 0; // Variavel da pontuacao
            primeira_vez = true; // condicao relacionada a direcao aleatoria da bola
            direcao_aleatoria = 0; // Direcao da bola
            game_over = false; //condicao do jogo
            vitoria = false;
            jogo_automatico = false;
            fase_atual = 1;
            mapa_bruto_jogo(m);//reseta o mapa
            vida = 1;
            ///fim resete do jogo

            break;
        case 3: //Créditos
            mensagem_creditos(); // void com os creditos
            system("pause"); // pausa
            system("cls"); // limpa
            break;

        case 4: // placar
            system("cls");
            carregarPlacar(); // Chama a função para carregar e mostrar o placar
            system("pause");
            system("cls");
            break;

            break;
        case 5: //infos tecnicas
            mensagem_infos_tecnincas();
            system("pause");
            system("cls");
            break;

        case 0: //Saída
            cout << "Saindo do jogo...\n";
            break;

        default:
            break;
        }
    } while (retorno_opcoes != 0); //Continua até que o usuário escolha sair

    return 0;
}