/*
Integrantes:

Cássio Montenegro Marques  
Erik Calixto  
Gabriel Campos Luiz De Mendonça 
Ivone Georg
Marcos Paulo do Nascimento

Um jogo feito para avaliação do 2°Semestre da faculdade Fatec de São Caetano do Sul Antonio Russo no curso de Jogos Digitais.

Este jogo foi feito na linguagem de programação C sem a utilização de classes, mas sim métodos que nos permitiram fazer processos como
trocas de tela, definições de elementos como pesonagens e afins, e dentre outros. 

Como é uma linguagem muito rustica, tivemos apoio apenas da biblioteca BGI para a criação desses game. Então varios dos elementos foram feitos 
"na mão", usando e brincando com logicas encima de numeros e sprits.

*/
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<math.h>
#include <time.h>
#include <windows.h>

#define QtdFases 10
#define QntPlayer 10
#define QntInimigo 10


//structs
struct Define {int xi, yi, x, y, lar, alt, vX, vY, ligado;}; //largura imagem, altura imagem, posição x, posição y, largura, altura, velocidadeX, velocidadeY

struct Define 
	Player = {800, 80, 100, 300, 80, 80, 17, 0, 0},
	Fundo = {1000, 600, 0, 0, 1000, 600},
	
	Inimigo[3] = {{800, 120, rand()%100+400, 450, 80, 120, 3, 100, 0},{0, 0, rand()%100+600, 450, 80, 120, 3, 100, 0},
				 {0, 0, rand()%100+800, 450, 80, 120, 3, 100, 0}},
	
	Plataforma[5] = {{100, 50, rand()%100+50, rand()%150+255, 100, 50},{0, 0, rand()%100+250, rand()%150+255, 100, 50},
					{0, 0, rand()%100+450, rand()%150+255, 100, 50},{0, 0, rand()%100+600, rand()%150+255, 100, 50},
					{0, 0, rand()%100+810, rand()%150+255, 100, 50}},
	
	Coletavel[5] = {{60, 70,rand()%100+50, rand()%100+140,60, 70, 0, 0, 0},{0, 0,rand()%100+350, rand()%100+140,60, 70, 0, 0, 0},
					{0, 0,rand()%100+550, rand()%100+140,60, 70, 0, 0, 0},{0, 0,rand()%100+710, rand()%100+140,60, 70, 0, 0, 0},
					{0, 0,rand()%100+820, rand()%100+140,60, 70, 0, 0, 0}};
;
	
//variáveis globais
int LarTela = 1000, AltTela = 600;
int LimtePulo = 0, iTrocaI = 0,iTrocaII = 0, iTrocaIII = 0, i = 0;
bool jump = false, caindo = true,mataI = false, mataII = false, mataIII = false, GameOver = false, Direita = false, Esquerda = false;
void *Corote[QntPlayer], *CoroteR[QntPlayer], *CoroteM[QntPlayer], *CoroteRM[QntPlayer], *fase[QtdFases], *Enimy[QntInimigo], *EnimyM[QntInimigo], *Plata, *PlataM, *Moeda, *MoedaM; 
int reseta = 0;
int iFase = 0, pTroca = 0;
int point = 0, life = 100;
int posi = 0;
char tecla = 0, PONTOS[1], VIDA [1];
int BotaoLar = 210, BotaoAlt = 80;

//funções
void Menu();							//essa função mostra a tela inical, o Menu
void Creditos();						//essa função mostra os créditos na tela
void Jogo();							//essa função mostra o Jogo na tela
bool Colisao(Define A, Define B);		//essa função calcula a colisão enrtre o player e um objeto ou inimigo
int Inimigos();							//essa função cria o inimigo, sua movimentação e morte dele
int Plataformas();						//essa função cria as plataformas e colisão com elas
int Limpa();							//essa função reseta os valores da fase, velocidade do inimigo repõe os inimigos, coletáveis e plataforma com coordenadas diferentes
int Cana();								//essa função gera as canas e seu desaparecimento quando coletadas
int GeraFase();							//essa função chama as outras quando troca de tela, gerando outra fase



int main() {
	//inicia tela
	initwindow(LarTela, AltTela, "Corotinho`s Epic Quest");
	
	//carrega a logo
  	setactivepage(1);
	for(i=0; i<=100; i++){
  		readimagefile(".\\Assets\\Imagens\\Comunalogo.bmp", 0, 0, Fundo.xi, Fundo.yi);
  		setvisualpage(1);
	}
	setvisualpage(2);
	
	//função onde ocorrerá tudo
	Menu();
	
	//fecha tudo
	closegraph();
	return 0;
}

void Menu() {
	bool Fim = false;
	bool btnJogarSelecionado = false; 
	bool btnCreditosSelecionado = false; 
  	bool btnSairSelecionado = false;
	int pg = 1;
  	int tam;
  	void *Tela;
  	void *btnJogar, *btnJogarSel;
  	void *btnCreditos, *btnCreditosSel;
  	void *btnSair, *btnSairSel;
  	POINT P;
	HWND janela;
  
  	//carrega e roda áudio
  	mciSendString("open .\\Assets\\Sons\\menu.mp3 type MPEGVideo alias menu", NULL, 0, 0);
  	mciSendString("play menu repeat", NULL, 0, 0);
  
  	//carrega o fundo
  	tam = imagesize(0, 0, Fundo.xi, Fundo.yi);
 	Tela = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\inicio.bmp", 0, 0, Fundo.xi, Fundo.yi);
  	getimage(0, 0, Fundo.lar, Fundo.alt, Tela);
  
  	setactivepage(1);
  	setvisualpage(1);
  	setactivepage(2);
  	
  	//carrega os botões
  	tam = imagesize(0, 0, BotaoLar-1, BotaoAlt-1);
  	btnJogar = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnjogar.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
  	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnJogar);
  	btnJogarSel = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnjogarsel.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
 	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnJogarSel);
  	btnCreditos = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnCreditos.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
  	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnCreditos);
  	btnCreditosSel = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnCreditossel.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
  	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnCreditosSel);
  	btnSair = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnsair.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
  	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnSair);
  	btnSairSel = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnsairsel.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
  	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnSairSel);
  	
  	cleardevice();
  
  	janela = GetForegroundWindow(); //pega o Handle da tela
  	while (!Fim){
    	if (pg == 1) pg = 2; else pg = 1;
    	setactivepage(pg);
    	putimage(0, 0, Tela, COPY_PUT);
    	if (btnJogarSelecionado)
     		putimage(60, 200, btnJogarSel, COPY_PUT); 
    	else
      		putimage(60, 200, btnJogar, COPY_PUT);
    	if (btnCreditosSelecionado)
      		putimage(60, 300, btnCreditosSel, COPY_PUT);
    	else
      		putimage(60, 300, btnCreditos, COPY_PUT);
    	if (btnSairSelecionado)
      		putimage(60, 400, btnSairSel, COPY_PUT);
    	else
      		putimage(60, 400, btnSair, COPY_PUT);
    
		setvisualpage(pg);
	   	 
    	if (GetCursorPos(&P)){ 		//colisão do mouse com os botões
      		if (ScreenToClient(janela, &P)){ 
  
        	btnJogarSelecionado = false; // botão jogar
        	if (60 <= P.x && P.x <= 60+BotaoLar-1 && 200 <= P.y && P.y <= 200+BotaoAlt-1) 
         	 btnJogarSelecionado = true;
          
        	btnCreditosSelecionado = false; // botão créditos
        	if (60 <= P.x && P.x <= 60+BotaoLar-1 && 300 <= P.y && P.y <= 300+BotaoAlt-1)
         	 btnCreditosSelecionado = true;

        	btnSairSelecionado = false; // botão sair
        	if (60 <= P.x && P.x <= 60+BotaoLar-1 && 400 <= P.y && P.y <= 400+BotaoAlt-1)
        	  btnSairSelecionado = true;

        	//colisão do mouse clicando em algum botão         
        	if(GetKeyState(VK_LBUTTON)&0x80){
         		//botão jogar
          		if (60 <= P.x && P.x <= 60+BotaoLar-1 && 200 <= P.y && P.y <= 200+BotaoAlt-1){
          			mciSendString("stop menu", NULL, 0, 0);
          			mciSendString("seek fundo to start", NULL, 0, 0);
          			GameOver = false; 
          			Jogo();
		 		 }
            
				//botão créditos
          		else if (60 <= P.x && P.x <= 60+BotaoLar-1 && 300 <= P.y && P.y <= 300+BotaoAlt-1)
            	Creditos(); 

         		//botão sair
          		else if (60 <= P.x && P.x <= 60+BotaoLar-1 && 400 <= P.y && P.y <= 400+BotaoAlt-1)
            		Fim = true;
        		}
    		}
    	}
	}
}

void Creditos() {
	bool Fim = false;
	bool btnSairSelecionado = false;
  	int pg = 1;
  	int tam;
  	void *Tela;
  	void *btnSair, *btnSairSel;
  	POINT P;
  	HWND janela;
  
  	//esconde as imagens enquanto estão sendo carregadas  
  	setactivepage(1);
  	setvisualpage(1);
  	setactivepage(2);
  	
  	//carrega o fundo
 	tam = imagesize(0, 0, Fundo.xi, Fundo.yi);
  	Tela = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\creditos.bmp", 0, 0, Fundo.xi, Fundo.yi);
  	getimage(0, 0, Fundo.lar, Fundo.alt, Tela);
  
  	//carrega os botões
  	tam = imagesize(0, 0, BotaoLar-1, BotaoAlt-1);
 	btnSair = malloc(tam);
 	readimagefile(".\\Assets\\Imagens\\btnsair.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
 	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnSair);
 	btnSairSel = malloc(tam);
  	readimagefile(".\\Assets\\Imagens\\btnsairsel.bmp", 0, 0, BotaoLar-1, BotaoAlt-1);
 	getimage(0, 0, BotaoLar-1, BotaoAlt-1, btnSairSel);
 	
 	cleardevice();
  
  	janela = GetForegroundWindow(); //pega o Handle da tela
  	while (!Fim){
    	if (pg == 1) pg = 2; else pg = 1;
    	setactivepage(pg);
    	putimage(0, 0, Tela, COPY_PUT);
    	
    	if (btnSairSelecionado) putimage(700, 500, btnSairSel, COPY_PUT);
      		
    	else putimage(700, 500, btnSair, COPY_PUT);
      	
		setvisualpage(pg);
	    
    	if (GetCursorPos(&P)){ 		//colisão do mouse com os botões
      		if (ScreenToClient(janela, &P)){ 
       			btnSairSelecionado = false; //botão sair
        		if (700 <= P.x && P.x <= 700+BotaoLar-1 && 500 <= P.y && P.y <= 500+BotaoAlt-1)
          			btnSairSelecionado = true;

        		//colisão do mouse clicando em algum botão          
        		if(GetKeyState(VK_LBUTTON)&0x80){
          			if (700 <= P.x && P.x <= 700+BotaoLar-1 && 500 <= P.y && P.y <= 500+BotaoAlt-1) Fim = true;	
        		}
    		}	
    	}
	}
}

void Jogo(){
	bool Fim = false;
	bool anda = false, troca = false;
	int pg = 2;
	int FPS = 60;
	int tamF, tamC, tamI, tamP, tamM;
	int gravidade = 9;
 	int vup = 30;
 	int le_arq = 0, aux = 0;
 	int Arq_Pontos[1];
  	unsigned long long gt1, gt2; 
 	char Best_Point[1];
 	char Ganhou[1];
 	char Sair[1], Restart[1], End[1];
 	FILE*arq;
 	srand(time(NULL));
  	
  	//esconde as imagens enquanto estão sendo carregadas 
  	setactivepage(1);
  	setvisualpage(1);
  	setactivepage(2);
  	
  	//carrega áudios
  	mciSendString("open .\\Assets\\Sons\\fundo.mp3 type MPEGVideo alias fundo", NULL, 0, 0);
  	
  	//toca áudio de fundo
  	mciSendString("play fundo repeat", NULL, 0, 0);
  	sndPlaySound(".\\Assets\\Sons\\FraseInicial.wav", SND_ASYNC);
  	
	//carrega imagem de fundo
  	tamF = imagesize(0, 0, Fundo.lar, Fundo.alt);
  	for(i = 0; i <= QtdFases; i++) {
    	fase[i] = malloc(tamF);
  	}
	
  	readimagefile(".\\Assets\\Fase\\fundo0.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[0]);
  	readimagefile(".\\Assets\\Fase\\fundo1.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[1]);
  	readimagefile(".\\Assets\\Fase\\fundo2.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[2]);
  	readimagefile(".\\Assets\\Fase\\fundo3.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[3]);
  	readimagefile(".\\Assets\\Fase\\fundo4.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[4]);
  	readimagefile(".\\Assets\\Fase\\fundo5.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[5]);
  	readimagefile(".\\Assets\\Fase\\fundo6.bmp", 0, 0, Fundo.xi-1, Fundo.yi-1);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[6]);
  	readimagefile(".\\Assets\\Fase\\fundo7.bmp", 0, 0, Fundo.xi, 599);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[7]);
  	readimagefile(".\\Assets\\Fase\\fundo8.bmp", 0, 0, Fundo.xi, 599);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[8]);
  	readimagefile(".\\Assets\\Fase\\fundo9.bmp", 0, 0, Fundo.xi, 599);
  	getimage(0, 0, Fundo.lar-1, Fundo.alt-1, fase[9]);
 	
  	//carrega inimigo e máscara
  	tamI = imagesize(0, 0, Inimigo[0].lar-1, Inimigo[0].alt-1);
  	for(i = 0; i<= QntInimigo; i++){
  		Enimy[i] = malloc(tamI);
  		EnimyM[i] = malloc(tamI);
	}
	
  	//Inimigo
  	readimagefile(".\\Assets\\Imagens\\Inimigo.bmp", 0, 0, Inimigo[0].xi, Inimigo[0].yi);
  	for(i = 0; i <= QntInimigo; i++){
  		getimage(Inimigo[0].lar*i, 0, Inimigo[0].lar-1+Inimigo[0].lar*i, Inimigo[0].alt-1, Enimy[i]);
  	}
  	
  	readimagefile(".\\Assets\\Imagens\\InimigoM.bmp", 0, 0, Inimigo[0].xi-1, Inimigo[0].yi-1);
  	for(i = 0; i <= QntInimigo; i++){
  		getimage(Inimigo[0].lar*i, 0, Inimigo[0].lar-1+Inimigo[0].lar*i, Inimigo[0].alt-1, EnimyM[i]);
	}
	
  	
  	//carrega personagem e máscara
  	tamC = imagesize(0, 0, Player.lar-1, Player.alt-1);
  	for(i = 0; i <= QntPlayer; i++) {
    	Corote[i] = malloc(tamC);
    	CoroteM[i] = malloc(tamC);
    	CoroteR[i] = malloc(tamC);
    	CoroteRM[i] = malloc(tamC);
  	}
  	
  	
  	//Player
  	readimagefile(".\\Assets\\Imagens\\Corotinho.bmp", 0, 0, Player.xi, Player.yi-1);
  	for(i = 0; i <= QntPlayer; i++) {
    	getimage(Player.lar*i, 0, Player.lar-1+Player.lar*i, Player.alt-1, Corote[i]);
 	}
  	readimagefile(".\\Assets\\Imagens\\CorotinhoM.bmp", 0, 0, Player.xi, Player.yi-1);
  	for(i = 0; i <= QntPlayer; i++) {
    	getimage(Player.lar*i, 0, Player.lar-1+Player.lar*i, Player.alt-1, CoroteM[i]);
    }
    readimagefile(".\\Assets\\Imagens\\CorotinhoR.bmp", 0, 0, Player.xi, Player.yi-1);
  	for(i = 0; i <= QntPlayer; i++) {
    	getimage(Player.lar*i, 0, Player.lar-1+Player.lar*i, Player.alt-1, CoroteR[i]);
    }
    readimagefile(".\\Assets\\Imagens\\CorotinhoRM.bmp", 0, 0, Player.xi, Player.yi-1);
  	for(i = 0; i <= QntPlayer; i++) {
    	getimage(Player.lar*i, 0, Player.lar-1+Player.lar*i, Player.alt-1, CoroteRM[i]);
    }
  	
  	//carrega plataforma
  	tamP = imagesize(0, 0, Plataforma[0].xi, Plataforma[0].yi);
  	Plata = malloc(tamP);
  	PlataM = malloc(tamP);
  	readimagefile(".\\Assets\\Imagens\\Plataforma.bmp", 0, 0, Plataforma[0].xi, Plataforma[0].yi);
  	getimage(0, 0, Plataforma[0].lar-1, Plataforma[0].alt-1, Plata);
  	
  	readimagefile(".\\Assets\\Imagens\\PlataformaM.bmp", 0, 0, Plataforma[0].xi, Plataforma[0].yi);
  	getimage(0, 0, Plataforma[0].lar-1, Plataforma[0].alt-1, PlataM);
  	
  	//carrega moeda
  	tamM = imagesize(0, 0, Coletavel[0].xi, Coletavel[0].yi);
  	Moeda = malloc(tamM);
  	MoedaM = malloc(tamM);
  	readimagefile(".\\Assets\\Imagens\\Cana.bmp", 0, 0, Coletavel[0].xi, Coletavel[0].yi);
  	getimage(0, 0, Coletavel[0].lar-1, Coletavel[0].alt-1, Moeda);
  	
  	readimagefile(".\\Assets\\Imagens\\CanaM.bmp", 0, 0, Coletavel[0].xi, Coletavel[0].yi);
  	getimage(0, 0, Coletavel[0].lar-1, Coletavel[0].alt-1, MoedaM);
  	
  	//FPS
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	
  	cleardevice();
  	 	
  	while(!Fim) {
  		
  		if(!GameOver){
  			
  			gt2 = GetTickCount();
  			if (gt2 - gt1 > 1000/FPS) { 
      			gt1 = gt2;
  		
    			if (pg == 1) pg = 2; else pg=1;
    			setactivepage(pg);
    			cleardevice();
    	
    			putimage(Fundo.x, Fundo.y, fase[iFase], COPY_PUT);  //imagem de fundo
    			
				//adiciona barra e textos
				setfillstyle(1, RGB(137, 222, 244));				
    			bar(0, 0, 1000, 50);
				setbkcolor(RGB(137, 222, 244));
   				setcolor(RGB(255, 255, 255));
   				settextstyle(10, 0, 5);
   				sprintf(PONTOS, "Pontuação: %d", point);
    			outtextxy(600, 7, PONTOS);
    			sprintf(VIDA, "Vida: %d", life);
   				outtextxy(50, 7, VIDA);
   				
   				//adiciona troca de fase e fim dela
    			GeraFase();										
				if (iFase == QtdFases-1){ 
					iFase = 9;
					anda = true;
				}						
					
				//faz o boneco iniciar sempre para direita
				if (posi == 0 && !Direita && !Esquerda){
					putimage(Player.x, Player.y, CoroteM[pTroca], AND_PUT);
   					putimage(Player.x, Player.y, Corote[pTroca], OR_PUT);
				}
				
				if(Direita){
					putimage(Player.x, Player.y, CoroteM[pTroca], AND_PUT);
   					putimage(Player.x, Player.y, Corote[pTroca], OR_PUT);		
				}
				if(Esquerda){
					putimage(Player.x, Player.y, CoroteRM[pTroca], AND_PUT);
   					putimage(Player.x, Player.y, CoroteR[pTroca], OR_PUT);
				}
    				
				setvisualpage(pg);
					
				//mata e cura Player
				if (life <= 0) {
					GameOver = true;			
				}
				if (life >= 100) life = 100;
				
				//troca tela
    			if (Player.x >= 980){
					iFase++;
					reseta++;
					if (reseta == 1){
						Limpa();
						reseta = 0;
					}
					Player.x = 0;
				}
			
				//colisão final direita e fim de jogo
				if (anda){
					if (Player.x >= 950) {
						GameOver = true;
					}
				}
			
				//colisão chão
				if (Player.y+Player.alt >= Fundo.alt-50) {
					caindo = false;
					Player.y = 495;
				}		
		
				//colisão teto
				if (Player.y <= 50){
					Player.y = 50;
					LimtePulo += 120;
				}
		
				//colisão esquerda
				if (Player.x <= Fundo.x){
					Player.x = Fundo.x;
				}
				
				//animação andar
				if (!caindo && troca){
					pTroca ++;
					if (pTroca >= 9){
						pTroca = 0;
					}	
				}
				else if(!caindo && !troca){
					pTroca = 0;
				}
				troca = false;
		
				//pulo, gravidade e animação pulo
				if (jump && Player.y > LimtePulo-120){
					Player.y += Player.vY;
					Player.vY = -vup;
					caindo = true;
					pTroca = 9;
				}
				else if (caindo){
					jump = false;
					Player.vY += gravidade;
					Player.y += Player.vY;
				}
				else {
					jump = false;
					Player.vY = 0;
				}
			
				//movimentação

				fflush(stdin);
				if(GetKeyState(VK_LEFT)&0x80){
					Player.x = Player.x - Player.vX;	
					troca = true;
					Esquerda = true;
					Direita = false;
				}	
	   			if(GetKeyState(VK_RIGHT)&0x80){
	    			Player.x = Player.x + Player.vX;
	    			troca = true;
	    			Direita = true;
	    			Esquerda = false;
				}
		
    			if (GetKeyState(VK_UP)&0x80  && !jump && !Player.vY){
					LimtePulo = Player.y;
					jump = true;
					tecla = 0;
					sndPlaySound(".\\Assets\\Sons\\Pulo.wav", SND_ASYNC);
				}
					
				if (GetKeyState(VK_SPACE)&0x80  && !jump && !Player.vY){
					LimtePulo = Player.y;
					jump = true;
					tecla = 0;
					sndPlaySound(".\\Assets\\Sons\\Pulo.wav", SND_ASYNC);
				}	
    			
			}
    		gt2 = GetTickCount();
		}
		
		//GameOver True
		else{		
			mciSendString("stop fundo", NULL, 0, 0);

			//lê pontos
			arq =fopen(".\\Assets\\Pontos.txt","r");
			fscanf(arq, "%d", &aux);
			Arq_Pontos[1] = aux;		
			fclose(arq);
			
			//guarda pontos
			if(point > int(Arq_Pontos[1])){
				Arq_Pontos[1] = point;
				arq = fopen(".\\Assets\\Pontos.txt","w");
				fprintf(arq,"%d\n",Arq_Pontos[1]);
				fclose(arq);
			}
			
			if (pg == 1) pg = 2; else pg=1;
    		setactivepage(pg);
    		
    		//desenha tela de game over
			setfillstyle(1, RGB(109, 98, 255));
			bar(0, 0, 1000, 600);
			setbkcolor(RGB(109, 98, 255));
    		
    		//desenha tela se venceu
    		if(iFase == 9 && Player.x >= 950){	
    			settextstyle(10, 0, 7);
    			setcolor(RGB(255, 0, 0));
    			sprintf(Ganhou, "Parabéns, você ganhou!!");
    			outtextxy(50, 250, Ganhou);
				settextstyle(10, 0, 5);
				setcolor(RGB(0, 0, 0));	
				sprintf(Sair, "Pressione S para sair");
    			outtextxy(250, 450, Sair);					
			}
			
			//desenha tela se perdeu
			else{
				settextstyle(10, 0, 7);
    			setcolor(RGB(255, 0, 0));
    			sprintf(End, "GAME OVER");
    			outtextxy(270, 250, End);
				settextstyle(10, 0, 5);
				setcolor(RGB(0, 0, 0));
				sprintf(Restart, "Pressione R para recomeçar");
    			outtextxy(185, 400, Restart);
    			sprintf(Sair, "Pressione S para sair");
    			outtextxy(185, 450, Sair);
			}
			
			//lê pontos
			arq =fopen(".\\Assets\\Pontos.txt","r");
			fscanf(arq, "%d", &aux);
			Arq_Pontos[1] = aux;		
			fclose(arq);
			
			//adiciona textos
			settextstyle(10, 0, 5);
			setcolor(RGB(0, 0, 0));
			sprintf(PONTOS, "Pontuação: %d", point);
    		outtextxy(340, 20, PONTOS);
    		sprintf(Best_Point, "Melhor Pontuação: %d", aux);
    		outtextxy(260, 150, Best_Point);

			setvisualpage(pg);
			fflush(stdin);
			
			//Restart
			if(GetKeyState(0x52)&0x80 || GetKeyState(0x72)&0x80){ //tecla R ou r
				Limpa();
				iFase = 0;
				life = 100;
				point = 0;
				Player.y = 300;
				Player.x = 100;
				caindo = true;
				Esquerda = false;
				Direita = false;  
				anda = false;
				mciSendString("seek fundo to start", NULL, 0, 0);
				mciSendString("play fundo repeat", NULL, 0, 0);
				sndPlaySound(".\\Assets\\Sons\\FraseInicial.wav", SND_ASYNC);
				GameOver = false; 
				tecla = 0;	
			}
			
			//Sair
			if(GetKeyState(0x53)&0x80 || GetKeyState(0x73)&0x80){ //tecla S ou s
				Limpa();
				iFase = 0;
				life = 100;
				point = 0;
				Player.y = 300;
				Player.x = 100;
				caindo = true;
				Esquerda = false;
				Direita = false;  
				mciSendString("seek menu to start", NULL, 0, 0);
				mciSendString("play menu repeat", NULL, 0, 0);
				Fim = true;
				tecla = 0;
			}			
		}  		
    }	
}



bool Colisao(Define A, Define B){
	bool colidiu = false;
	float distance;
	
	distance = sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
	
	if(distance < B.lar){
		colidiu = true;
	}
	return colidiu;
}

int Inimigos(){
	//adiciona inimigos, velocidade e 'mata' se sair da área
	for(i=0; i<3; i++){
		if(Inimigo[i].ligado == 0){	    					
    		Inimigo[i].x -= Inimigo[i].vX;
		}
	}	
	
	putimage(Inimigo[0].x, Inimigo[0].y, EnimyM[iTrocaI], AND_PUT);
    putimage(Inimigo[0].x, Inimigo[0].y, Enimy[iTrocaI], OR_PUT);			
    putimage(Inimigo[1].x, Inimigo[1].y, EnimyM[iTrocaII], AND_PUT);
    putimage(Inimigo[1].x, Inimigo[1].y, Enimy[iTrocaII], OR_PUT);
	putimage(Inimigo[2].x, Inimigo[2].y, EnimyM[iTrocaIII], AND_PUT);
   	putimage(Inimigo[2].x, Inimigo[2].y, Enimy[iTrocaIII], OR_PUT);
	
	//colisão inimigo1
	if(Colisao(Player, Inimigo[0])){
		if(Player.y+Player.alt >= Inimigo[0].y && Player.y<Inimigo[0].y && Player.x+40 >= Inimigo[0].x && Player.x+40 <= Inimigo[0].x+Inimigo[0].lar){
			Inimigo[0].vX = 0;
			iTrocaI = 0;
			LimtePulo -= 20;
			jump = true;
			mataI = true;	
			sndPlaySound(".\\Assets\\Sons\\DanoInimigo.wav", SND_ASYNC);		
		}
		else {
			Player.x = 10;	
			life -= 50;
			sndPlaySound(".\\Assets\\Sons\\DanoCorotinho.wav", SND_ASYNC);
		}
	}		
		if(mataI){
			Inimigo[0].y += Inimigo[0].vY*6;
			if (Inimigo[0].y >= 610) {
				mataI = false;
				Inimigo[0].vY = 0;
				Inimigo[0].ligado = 1;	
			}	
		}
		
	//colisão inimigo2	
	if (Colisao(Player, Inimigo[1])){
		if(Player.y+Player.alt >= Inimigo[1].y && Player.y<Inimigo[1].y && Player.x+40 >= Inimigo[1].x && Player.x+40 <= Inimigo[1].x+Inimigo[1].lar){
			Inimigo[1].vX = 0;
			iTrocaII = 0;
			LimtePulo -= 20;
			jump = true;
			mataII = true;	
			sndPlaySound(".\\Assets\\Sons\\DanoInimigo.wav", SND_ASYNC);		
		}
		else {
			Player.x = 10;
			life -= 50;
			sndPlaySound(".\\Assets\\Sons\\DanoCorotinho.wav", SND_ASYNC);
		}
	}	
		if(mataII){
			Inimigo[1].y += Inimigo[1].vY*6;
			if (Inimigo[1].y >= 610) {
				mataII = false;
				Inimigo[1].vY = 0;
				Inimigo[1].ligado = 1;
			}	
		}
		
	//colisão inimigo3
	if (Colisao(Player, Inimigo[2])){
		if(Player.y+Player.alt >= Inimigo[2].y && Player.y<Inimigo[2].y && Player.x+40 >= Inimigo[2].x && Player.x+40 <= Inimigo[2].x+Inimigo[2].lar){
			Inimigo[2].vX = 0;
			iTrocaIII = 0;
			LimtePulo -= 20;
			jump = true;
			mataIII = true;	
			sndPlaySound(".\\Assets\\Sons\\DanoInimigo.wav", SND_ASYNC);			
		}
		else {
			Player.x = 10;	
			life -= 50;
			sndPlaySound(".\\Assets\\Sons\\DanoCorotinho.wav", SND_ASYNC);
		}
	}	
		if(mataIII){
			Inimigo[2].y += Inimigo[2].vY*6;
			if (Inimigo[2].y >= 610) {
				mataIII = false;
				Inimigo[2].vY = 0;
				Inimigo[2].ligado = 1;
			}	
		}	
	
	//movimentação do inimigo
	if (Inimigo[0].vX != 0){
		iTrocaI++;
		if (iTrocaI >= QntInimigo) iTrocaI = 0;	
	}
	if (Inimigo[1].vX != 0){
		iTrocaII++;
		if (iTrocaII >= QntInimigo) iTrocaII = 0;
	}
	if (Inimigo[2].vX != 0){
		iTrocaIII++;
		if (iTrocaIII >= QntInimigo) iTrocaIII = 0;
	}
}

int Plataformas(){
	//gera plataformas e colisão com elas
	for(i=0; i<5; i++){
		putimage(Plataforma[i].x, Plataforma[i].y, PlataM, AND_PUT);
		putimage(Plataforma[i].x, Plataforma[i].y, Plata, OR_PUT);
		
		if(Colisao(Player, Plataforma[i])){
			if(Player.y+Player.alt >= Plataforma[i].y && Player.y < Plataforma[i].y && Player.x+40 >= Plataforma[i].x && Player.x+Player.lar-30 <= Plataforma[i].x+Plataforma[i].lar ) {
				caindo = false;
				Player.y = Plataforma[i].y-Plataforma[i].alt-25;
			}	
			else caindo = true;
								
			if (Player.y+15 < Plataforma[i].y+Plataforma[i].alt && Player.y+15 > Plataforma[i].y && Player.x+40 >= Plataforma[i].x && Player.x+Player.lar-30 <= Plataforma[i].x+Plataforma[i].lar) {
				Player.y = Plataforma[i].y+Plataforma[i].alt-15;	
				LimtePulo += LimtePulo;
			}
		}	
	}
}

int Cana(){
	//gera canas e colisão com elas
	for(i=0; i<5; i++){
		if(Coletavel[i].ligado == 0) putimage(Coletavel[i].x, Coletavel[i].y, MoedaM, AND_PUT);
		if(Coletavel[i].ligado == 0) putimage(Coletavel[i].x, Coletavel[i].y, Moeda, OR_PUT);
		
		if(Colisao(Player, Coletavel[i])){
			sndPlaySound(".\\Assets\\Sons\\PegaCana.wav", SND_ASYNC);
			Coletavel[i].y = 610;
			Coletavel[i].ligado = 1;
			point++;
			life += 10;	
		}
	}
}

int Limpa(){
	//Limpa inimigo
	for(i=0; i<3; i++){
		Inimigo[i].vX = 3;
		Inimigo[i].vY = 10;
		Inimigo[i].ligado = 0;
		Inimigo[i].y = 450;
	}
	Inimigo[0].x = rand()%100+400;
	Inimigo[1].x = rand()%100+600;
	Inimigo[2].x = rand()%100+800;
	
	//Limpa moeda e plataforma
	for(i=0; i<5; i++){
		Coletavel[i].y = rand()%100+120;
		Coletavel[i].ligado = 0;
		Plataforma[i].y = rand()%150+255;
	}
	Coletavel[0].x = rand()%100+50;
	Coletavel[1].x = rand()%100+250;
	Coletavel[2].x = rand()%100+450;
	Coletavel[3].x = rand()%100+600;
	Coletavel[4].x = rand()%100+820;
}

int GeraFase(){
		
	if(iFase == 0){
		Limpa();
	}
		
	if(iFase == 1){
		Plataformas();
		Inimigos();	
		Cana();				
	}
			
	if(iFase == 2){
		Plataformas();
		Inimigos();
		Cana();	
	}
	
	if(iFase == 3){
		Plataformas();
		Inimigos();	
		Cana();
	}
	
	if(iFase == 4){
		Plataformas();
		Inimigos();
		Cana();	
	}
	
	if(iFase == 5){				
		Plataformas();
		Inimigos();	
		Cana();
	}
	
	if(iFase == 6){				
		Plataformas();
		Inimigos();
		Cana();	
	}
	
	if(iFase == 7){				
		Plataformas();
		Inimigos();	
		Cana();
	}
	
	if(iFase == 8){				
		Plataformas();
		Inimigos();	
		Cana();
	}
	
	if(iFase == 9){		
		Plataformas();
		Inimigos();	
		Cana();
	}
}
