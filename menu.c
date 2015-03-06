/* ----------------------------------------------------------------------------------------------
	Projeto Alg II - Reversi

	- Vinícius Miazaki de Moraes	  - 552461
	- Yasmin Beatriz Alves da Silva - 552518

	O programa exibe a janela de Menu Principal do jogo, com as opções de "Jogar", "Instruções",
"Sobre" e "Sair",  tudo fazendo uso de imagens, mouse e outras funções implementadas por meio da
biblioteca Allegro.h.
	Os  botões são  compostos por duas imagens:  Uma fica sempre visivel, e quando o mouse passa
sobre ela, a outra imagem é exibida, destacando o botão em questão.
	BOTÃO "JOGAR": Abre a janela com o tabuleiro e o jogo em si é iniciado.
	OBS: O jogador tem apenas 3 tentativas, caso clique 3 vezes em casas inválidas do tabuleiro, 
a vez de jogar passa para o outro jogador.
	BOTÃO "COMO JOGAR": Abre uma nova janela com as instruções do Reversi e um botão para voltar
para o Menu Principal.
	BOTÃO "SOBRE": Abre uma nova janela com algumas informações sobre o programa e um botão para
voltar para o Menu Principal.
	BOTÃO "SAIR": Encerra o programa.
	Em todas as janelas foi usado o evento para que a elas sejam fechadas quando o  "Fechar", do
topo, for clicado. Também foram usados eventos com o mouse, onde as imagens são alteradas  quando
ele passa sobre elas, para dar o "efeito de botão" e realizar uma função quando clicadas.
---------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#define HEIGHT 78.5  // altura das casas
#define WIDTH  79.25 // largura das casas
#define VERDADEIRO 1
#define FALSO      0

//Medidas da tela
const int LARGURA_TELA = 1000; //X
const int ALTURA_TELA  = 650;  //Y

//Protótipo das Funções
int  verifica_jogada (int tabuleiro[][8], int contx, int conty, int cor_jogada, int controle, int modo);
int  procuraJogadaValida (int cor_jogada, int tabuleiro[][8], int modo);
void verifica_matriz(int *nao_tem_casa_vazia, int *so_tem_peca_clara, int *so_tem_peca_escura, int tabuleiro[][8]);
void ContaPontuacao (int tabuleiro[][8], int *pont_escuro, int *pont_claro);
void imprime_pecas (int x, int y, int tabuleiro[][8], 
				ALLEGRO_BITMAP *peca_clara, 
				ALLEGRO_BITMAP *peca_escura,
				ALLEGRO_BITMAP *img_fundo_jogar,
				ALLEGRO_BITMAP *img_btn_encerrar,
				ALLEGRO_BITMAP *img_fundo_vez,
				ALLEGRO_FONT *fonte_vez);
//Bloco principal
int main(void)
{
	ALLEGRO_DISPLAY *menu 	= NULL; 			//janela do "Menu Principal", que se transforma em todas as outras
	ALLEGRO_FONT *fonte 	= NULL;			//fonte que será usada na pontuação do "Jogar"
	ALLEGRO_FONT *fonte_vez 	= NULL;			//fonte que será usada para mostrar de qual jogador é a vez

	ALLEGRO_BITMAP *img_fundo_menu 	= NULL;	//imagem do fundo do "Menu Principal"
	ALLEGRO_BITMAP *img_fundo_modo 	= NULL;	//imagem do fundo do "Modo de Jogar"
	ALLEGRO_BITMAP *img_fundo_cor 	= NULL;	//imagem do fundo do "Cor do Player"
	ALLEGRO_BITMAP *img_fundo_jogar 	= NULL;	//imagem do fundo do "Jogar"
	ALLEGRO_BITMAP *img_fundo_comojogar= NULL;	//imagem do fundo do "Como Jogar"
	ALLEGRO_BITMAP *img_fundo_sobre 	= NULL;	//imagem do fundo do "Sobre"
	ALLEGRO_BITMAP *img_btn_1player 	= NULL;	//imagem do botão "1 Player" do Modo
	ALLEGRO_BITMAP *img_btn_2player 	= NULL;	//imagem do botão "2 Player" do Modo
	ALLEGRO_BITMAP *img_btn_clara		= NULL;	//imagem do botão do "Cor do Player"
	ALLEGRO_BITMAP *img_btn_escura	= NULL;	//imagem do botão do "Cor do Player"
	ALLEGRO_BITMAP *img_btn_jogar 	= NULL;	//imagem do botão "Jogar" do Menu Principal
	ALLEGRO_BITMAP *img_btn_comojogar 	= NULL;	//imagem do botão "Como Jogar" do Menu Principal
	ALLEGRO_BITMAP *img_btn_sobre 	= NULL;	//imagem do botão "Sobre" do Menu Principal
	ALLEGRO_BITMAP *img_btn_sair 		= NULL;	//imagem do botão "Sair" do Menu Principal
	ALLEGRO_BITMAP *img_btn_voltar 	= NULL;	//imagem do botão "Voltar ao Menu Principal" do Como Jogar e Sobre
	ALLEGRO_BITMAP *img_btn_encerrar 	= NULL;	//imagem do botão "Sair" do Jogar
	ALLEGRO_BITMAP *img_peca1, *img_peca2;		//imagem das peças do tabuleiro
	ALLEGRO_BITMAP *img_jogar_fundo_direita;	//imagem do canto direito do tabuleiro
	ALLEGRO_BITMAP *img_jogar_fundo_esquerda;	//imagem do canto esquerdo do tabuleiro
	ALLEGRO_BITMAP *img_fundo_vez;			//imagem do fundo do tabuleiro onde aparece o texto da vez

	ALLEGRO_EVENT_QUEUE *eventos_menu  = NULL;	//fila de eventos do "Menu Principal"
	ALLEGRO_EVENT_QUEUE *eventos_modo  = NULL;	//fila de eventos do "Modo de Jogar"
	ALLEGRO_EVENT_QUEUE *eventos_cor	= NULL;	//fila de eventos do "Cor do Player"
	ALLEGRO_EVENT_QUEUE *eventos_jogar = NULL;	//fila de eventos do "Jogar"
	ALLEGRO_EVENT_QUEUE *eventos_comoj = NULL;	//fila de eventos do "Como Jogar"
	ALLEGRO_EVENT_QUEUE *eventos_sobre = NULL;	//fila de eventos do "Sobre"

	int tabuleiro[8][8];					//matriz das casas do tabuleiro
	int i, j;								//iteradores do for
	int cor_jogada = 2;						//armazana a cor da peça do jogador da vez
	int cont=0;							//contador do número de jogadas
	int muda=0;							//flag para mudar a vez do jogador (se igual a zero, o clique da jogada é invalido)
	int pont_escuro, pont_claro;				//contador das pontuações
	int x_1, x_2, y_1, y_2;					//auxiliares para trabalhar com as coordenadas das casas do tabuleiro na tela
	int contx, conty;						//variaveis auxiliares para trabalhar com os índices da matriz de tabuleiros
	int opc_modo=0;						//grava o modo de jogo (1 ou 2 players)
	int opc_cor, opc_corComputador;			//grava a cor da peça do jogador, no modo 1 Player
	int peca_preta = VERDADEIRO, vez_jogador;	//flags para controlar a vez dos jogadores, peca_preta se for no modo 2 players
										//e vez_jogador se for jogador vs computador
	int passou_duasvezes = FALSO;
	int vez = 0, controle = 0;
	int no_btn_jogar=0, no_btn_comojogar=0, no_btn_sobre=0, no_btn_sair=1; 	//flags
	int no_btn_voltar=1, no_btn_encerrar=1, no_btn_1player=1, no_btn_2player=1;//flags
	int no_btn_claro=1, no_btn_escuro=1;								//flags

	int nao_tem_casa_vazia = VERDADEIRO, *p_nao_tem_casa_vazia = &nao_tem_casa_vazia;
	int so_tem_peca_clara  = VERDADEIRO, *p_so_tem_peca_clara  = &so_tem_peca_clara;
	int so_tem_peca_escura = VERDADEIRO, *p_so_tem_peca_escura = &so_tem_peca_escura;
	/* flags e seus ponteiro para verificar se todas as casas do tabuleiro foram preenchidas, ou se no tabulerio só tem 
	   peças de uma ou outra cor, estas flags serão utilizadas na hora de encontrar o vencedor do jogo */

	//inicializa o tabuleiro (todas as casas vazias)
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			tabuleiro[i][j] = 0;
		}
	}

	tabuleiro[3][3] = 1;
	tabuleiro[3][4] = 2;
	tabuleiro[4][3] = 2;
	tabuleiro[4][4] = 1;


	if (!al_init()){
		fprintf(stderr, "Falha ao inicializar a Allegro.\n");
		return -1;
	}

	if (!al_init_image_addon()){
		fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
		return -1;
	}

	menu = al_create_display(LARGURA_TELA, ALTURA_TELA);
	if (!menu){
		fprintf(stderr, "Falha ao criar janela.\n");
		return -1;
 	}
 	al_set_window_position(menu, 0, 0); //define a posição da janela no monitor

	//configura o título da janela
	al_set_window_title(menu, "Reversi - Menu Principal");

	//habilita o mouse
	if (!al_install_mouse())
	{
		fprintf(stderr, "Falha ao inicializar o mouse.\n");
		al_destroy_display(menu);
		return -1;
	}

	//atribui o cursor padrão do sistema para ser usado
	if (!al_set_system_mouse_cursor(menu, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
	{
		fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
		al_destroy_display(menu);
		return -1;
	}

	//Inicialização do add-on para uso de fontes
	al_init_font_addon();

	//Inicialização do add-on para uso de fontes True Type
	if (!al_init_ttf_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
		return -1;
	}

	//Carregando o arquivo de fonte
	fonte = al_load_font("Monster AG.ttf", 80, 0);
	if (!fonte)
	{
		al_destroy_display(menu);
		fprintf(stderr, "Falha ao carregar fonte.\n");
		return -1;
	}

	//carregando imagens do Menu
	img_fundo_menu = al_load_bitmap("menu_fundo.jpg");
	if (!img_fundo_menu){
		fprintf(stderr, "Falha ao carregar a imagem de fundo do Menu Principal.\n");
		al_destroy_display(menu);
		return -1;
	}
	img_btn_jogar = al_load_bitmap("btn_jogar1.jpg");
	if (!img_btn_jogar){
		fprintf(stderr, "Falha ao carregar a imagem do botão 'Jogar'.\n");
		al_destroy_display(menu);
		return -1;
	}
	img_btn_comojogar = al_load_bitmap("btn_comojogar1.jpg");
	if (!img_btn_comojogar){
		fprintf(stderr, "Falha ao carregar a imagem do botão 'Como Jogar'.\n");
		al_destroy_display(menu);
		return -1;
	}
	img_btn_sobre = al_load_bitmap("btn_sobre1.jpg");
	if (!img_btn_sobre){
		fprintf(stderr, "Falha ao carregar a imagem do botão 'Sobre'.\n");
		al_destroy_display(menu);
		return -1;
	}
	img_btn_sair = al_load_bitmap("btn_sair1.jpg");
	if (!img_btn_sair){
		fprintf(stderr, "Falha ao carregar a imagem do botão 'Sair'.\n");
		al_destroy_display(menu);
		return -1;
	}

	eventos_menu = al_create_event_queue();
	if (!eventos_menu){
		fprintf(stderr, "Falha ao criar fila de eventos.\n");
		al_destroy_display(menu);
		return -1;
	}

	al_register_event_source(eventos_menu, al_get_display_event_source(menu));
	al_register_event_source(eventos_menu, al_get_mouse_event_source());

	al_draw_bitmap(img_fundo_menu   ,   0,   0, 0);
	al_draw_bitmap(img_btn_jogar    ,  90, 220, 0);
	al_draw_bitmap(img_btn_comojogar, 145, 330, 0);
	al_draw_bitmap(img_btn_sobre    , 145, 400, 0);
	al_draw_bitmap(img_btn_sair     , 145, 470, 0);

	al_flip_display();

	while(no_btn_sair)
	{
		ALLEGRO_EVENT evento;
		ALLEGRO_TIMEOUT timeout;
		al_init_timeout(&timeout, 0.05);

		int tem_eventos = al_wait_for_event_until(eventos_menu, &evento, &timeout);

		if(tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //verifica se o "fechar" da janela foi clicado
		{
			no_btn_sair = 0; //fará o looping ser encerrado
			break;
		}

		//EVENTOS DO BOTÃO "JOGAR"
		//verifica se o evento foi de movimentação do mouse
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			//verificamos se ele está sobre o botão
			if(evento.mouse.x >= 90 &&
			   evento.mouse.x <= 90 + al_get_bitmap_width(img_btn_jogar) &&
			   evento.mouse.y >= 220 &&
			   evento.mouse.y <= 220 + al_get_bitmap_height(img_btn_jogar) )
			{
				no_btn_jogar = 2; //está sobre o botão
			}
			else
			{
				no_btn_jogar = 1; //nao está sobre o botão
			}
		}
		//verifica se o evento foi um clique do mouse
		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			//verificar se o botão foi clicado
			if(evento.mouse.x >=  90 &&
			   evento.mouse.x <=  90 + al_get_bitmap_width(img_btn_jogar) &&
			   evento.mouse.y >= 220 &&
			   evento.mouse.y <= 220 + al_get_bitmap_height(img_btn_jogar) )
			{
				//cria a janela de "Modo" --------------------------------------------------------------------------------
				al_resize_display(menu, 572, 300); //redimensiona a janela do Menu para a Modo de Jogar
				al_set_window_title(menu, "Reversi - Modo de Jogar");

				//carregando imagens da janela
				img_fundo_modo = al_load_bitmap("modo_fundo.jpg");
				if (!img_fundo_modo)
				{
					fprintf(stderr, "Falha ao carregar a imagem de fundo do Modo.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_btn_1player = al_load_bitmap("btn_1player1.jpg");
				if (!img_btn_1player)
				{
					fprintf(stderr, "Falha ao carregar a imagem do btn 1 players.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_btn_2player = al_load_bitmap("btn_2player1.jpg");
				if (!img_btn_2player)
				{
					fprintf(stderr, "Falha ao carregar a imagem do btn 2 players\n");
					al_destroy_display(menu);
					return -1;
				}

				eventos_modo = al_create_event_queue();
				if (!eventos_modo){
					fprintf(stderr, "Falha ao criar fila de eventos.\n");
					al_destroy_display(menu);
					return -1;
				}

				al_register_event_source(eventos_modo, al_get_display_event_source(menu));
				al_register_event_source(eventos_modo, al_get_mouse_event_source());

				al_draw_bitmap(img_fundo_modo ,   0,   0, 0);
				al_draw_bitmap(img_btn_1player, 330, 160, 0);
				al_draw_bitmap(img_btn_2player, 330, 220, 0);

				al_flip_display();
				no_btn_voltar  = 1;
				no_btn_1player = 1;
				no_btn_2player = 1;

				while(no_btn_voltar)
				{
					ALLEGRO_EVENT evento_modo;
					ALLEGRO_TIMEOUT timeout_modo;
					al_init_timeout(&timeout_modo, 0.05);

					int tem_eventos_modo = al_wait_for_event_until(eventos_modo, &evento_modo, &timeout_modo);

					if(tem_eventos_modo && evento_modo.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //verifica se o "fechar" da janela foi clicado
					{
						//destoi tudo e encerra todo o programa
						al_destroy_event_queue(eventos_modo);
						al_destroy_event_queue(eventos_menu);
						al_destroy_display(menu);
						return 0;
					}

					//verifica se o evento foi de movimentação do mouse
					if (evento_modo.type == ALLEGRO_EVENT_MOUSE_AXES)
					{
						//verificamos se ele está sobre o botão
						if(evento_modo.mouse.x >= 330 &&
						   evento_modo.mouse.x <= 330 + al_get_bitmap_width(img_btn_1player) &&
						   evento_modo.mouse.y >= 160 &&
						   evento_modo.mouse.y <= 160 + al_get_bitmap_height(img_btn_1player) )
						{
							no_btn_1player = 2; //está sobre o botão
						}
						else
						{
							no_btn_1player = 1; //nao está sobre o botão
						}

						if(evento_modo.mouse.x >= 330 &&
						   evento_modo.mouse.x <= 330 + al_get_bitmap_width(img_btn_2player) &&
						   evento_modo.mouse.y >= 220 &&
						   evento_modo.mouse.y <= 220 + al_get_bitmap_height(img_btn_2player) )
						{
							no_btn_2player = 2; //está sobre o botão
						}
						else
						{
							no_btn_2player = 1; //nao está sobre o botão
						}
					}
					//verifica se o evento foi um clique do mouse
					else if (evento_modo.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
					{
						//verificar se o botão foi clicado
						if(evento_modo.mouse.x >= 330 &&
						   evento_modo.mouse.x <= 330 + al_get_bitmap_width(img_btn_1player) &&
						   evento_modo.mouse.y >= 160 &&
						   evento_modo.mouse.y <= 160 + al_get_bitmap_height(img_btn_1player) )
						{
							opc_modo = 1; //inicia o "Jogar" com 1 jogador contra o computador
							no_btn_voltar = 0; //fará o looping ser encerrado
						}

						if(evento_modo.mouse.x >= 330 &&
						   evento_modo.mouse.x <= 330 + al_get_bitmap_width(img_btn_2player) &&
						   evento_modo.mouse.y >= 220 &&
						   evento_modo.mouse.y <= 220 + al_get_bitmap_height(img_btn_2player) )
						{
							opc_modo = 2; //inicia o "Jogar" com 2 jogadores
							no_btn_voltar = 0; //fará o looping ser encerrado
						}
					}

					//rollover dos botões -----------------------------
					if(no_btn_1player==1)
						img_btn_1player = al_load_bitmap("btn_1player1.jpg"); //mouse não está sobre o botão
					else if(no_btn_1player==2)
						img_btn_1player = al_load_bitmap("btn_1player2.jpg"); //mouse está sobre o botão
					if(no_btn_2player==1)
						img_btn_2player = al_load_bitmap("btn_2player1.jpg"); //mouse não está sobre o botão
					else if(no_btn_2player==2)
						img_btn_2player = al_load_bitmap("btn_2player2.jpg"); //mouse está sobre o botão


					al_draw_bitmap(img_fundo_modo ,   0,   0, 0);
					al_draw_bitmap(img_btn_1player, 330, 160, 0);
					al_draw_bitmap(img_btn_2player, 330, 220, 0);

					al_flip_display();
				}

				al_destroy_event_queue(eventos_modo);
				//--------------------------------------------------------------------------------------------------------

				//cria a janela de "Cor" ---------------------------------------------------------------------------------
				if(opc_modo == 1) //só entra nessa janela caso o joador deseje jogar contra o computador
				{
					al_set_window_title(menu, "Reversi - Cor do Player");

					//carregando imagens da janela
					img_fundo_cor = al_load_bitmap("fundo_cor.jpg");
					if (!img_fundo_cor)
					{
						fprintf(stderr, "Falha ao carregar a imagem de fundo cor\n");
						al_destroy_display(menu);
						return -1;
					}
					img_btn_clara = al_load_bitmap("btn_clara1.jpg");
					if (!img_btn_clara)
					{
						fprintf(stderr, "Falha ao carregar a imagem do botão cor clara.\n");
						al_destroy_display(menu);
						return -1;
					}
					img_btn_escura = al_load_bitmap("btn_escura1.jpg");
					if (!img_btn_escura)
					{
						fprintf(stderr, "Falha ao carregar a imagem do botão cor escura.\n");
						al_destroy_display(menu);
						return -1;
					}

					eventos_cor = al_create_event_queue();
					if (!eventos_cor){
						fprintf(stderr, "Falha ao criar fila de eventos.\n");
						al_destroy_display(menu);
						return -1;
					}

					al_register_event_source(eventos_cor, al_get_display_event_source(menu));
					al_register_event_source(eventos_cor, al_get_mouse_event_source());

					al_draw_bitmap(img_fundo_cor ,  0,   0, 0);
					al_draw_bitmap(img_btn_clara , 40, 100, 0);
					al_draw_bitmap(img_btn_escura, 40, 160, 0);

					al_flip_display();
					no_btn_voltar = 1;
					no_btn_claro  = 1;
					no_btn_escuro = 1;

					while(no_btn_voltar)
					{
						ALLEGRO_EVENT evento_cor;
						ALLEGRO_TIMEOUT timeout_cor;
						al_init_timeout(&timeout_cor, 0.05);

						int tem_eventos_cor = al_wait_for_event_until(eventos_cor, &evento_cor, &timeout_cor);

						if(tem_eventos_cor && evento_cor.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //verifica se o "fechar" da janela foi clicado
						{
							//destoi tudo e encerra todo o programa
							al_destroy_event_queue(eventos_cor);
							al_destroy_event_queue(eventos_menu);
							al_destroy_display(menu);
							return 0;
						}

						//verifica se o evento foi de movimentação do mouse
						if (evento_cor.type == ALLEGRO_EVENT_MOUSE_AXES)
						{
							//verificamos se ele está sobre o botão
							if(evento_cor.mouse.x >= 40 &&
							   evento_cor.mouse.x <= 40 + al_get_bitmap_width(img_btn_clara) &&
							   evento_cor.mouse.y >= 100 &&
							   evento_cor.mouse.y <= 100 + al_get_bitmap_height(img_btn_clara) )
							{
								no_btn_claro = 2; //está sobre o botão
							}
							else
							{
								no_btn_claro = 1; //nao está sobre o botão
							}

							if(evento_cor.mouse.x >= 40 &&
							   evento_cor.mouse.x <= 40 + al_get_bitmap_width(img_btn_escura) &&
							   evento_cor.mouse.y >= 160 &&
							   evento_cor.mouse.y <= 160 + al_get_bitmap_height(img_btn_escura) )
							{
								no_btn_escuro = 2; //está sobre o botão
							}
							else
							{
								no_btn_escuro = 1; //nao está sobre o botão
							}
						}
						//verifica se o evento foi um clique do mouse
						else if (evento_cor.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
						{
							//verificar se o botão foi clicado
							if(evento_cor.mouse.x >= 40 &&
							   evento_cor.mouse.x <= 40 + al_get_bitmap_width(img_btn_clara) &&
							   evento_cor.mouse.y >= 100 &&
							   evento_cor.mouse.y <= 100 + al_get_bitmap_height(img_btn_clara) )
							{
								opc_cor = 2; 		//o player escolheu ser as peças de cor CLARA
								opc_corComputador = 1;
								vez_jogador = FALSO;
								no_btn_voltar = 0;	//fará o looping ser encerrado
							}

							if(evento_cor.mouse.x >= 40 &&
							   evento_cor.mouse.x <= 40 + al_get_bitmap_width(img_btn_escura) &&
							   evento_cor.mouse.y >= 160 &&
							   evento_cor.mouse.y <= 160 + al_get_bitmap_height(img_btn_escura) )
							{
								opc_cor = 1;		//o player escolheu ser as peças de cor ESCURA
								cor_jogada = 1;
								opc_corComputador = 2;
								vez_jogador = VERDADEIRO;
								no_btn_voltar = 0;	//fará o looping ser encerrado
							}
						}

						if(opc_cor == 2)
						{
							if(procuraJogadaValida(opc_corComputador, tabuleiro, opc_modo) == VERDADEIRO)
							{
								vez_jogador = VERDADEIRO; //após o computador ter iniciado a partida (por ele ser a peça preta neste caso) a vez é passada ao usuário
							}
						}						

						//rollover dos botões -----------------------------
						if(no_btn_claro==1)
							img_btn_clara = al_load_bitmap("btn_clara1.jpg"); //mouse não está sobre o botão
						else if(no_btn_claro==2)
							img_btn_clara = al_load_bitmap("btn_clara2.jpg"); //mouse está sobre o botão
						if(no_btn_escuro==1)
							img_btn_escura = al_load_bitmap("btn_escura1.jpg"); //mouse não está sobre o botão
						else if(no_btn_escuro==2)
							img_btn_escura = al_load_bitmap("btn_escura2.jpg"); //mouse está sobre o botão

						al_draw_bitmap(img_fundo_cor ,  0,   0, 0);
						al_draw_bitmap(img_btn_clara , 40, 100, 0);
						al_draw_bitmap(img_btn_escura, 40, 160, 0);

						al_flip_display();
					}
					al_destroy_event_queue(eventos_cor);
				}//FIM - if(opc_modo == 1)
				//--------------------------------------------------------------------------------------------------------

				//cria a janela de "Jogar" -------------------------------------------------------------------------------
				no_btn_encerrar=1; //flag que simboliza que a janela está aberta, senão não entra no looping depois da primeira execução
				cont = 0; 	    //inicializa o contador do número de jogadas

				al_resize_display(menu, 1100, 650); //redimensiona a janela do Menu para a Como Jogar
				al_set_window_title(menu, "Reversi - Jogar");

				//carregando a fonte
				fonte_vez = al_load_font("Monster AG.ttf", 40, 0);
				if (!fonte_vez)
				{
					al_destroy_display(menu);
					fprintf(stderr, "Falha ao carregar fonte.\n");
					return -1;
				}

				//carregando imagens da janela
				img_fundo_jogar = al_load_bitmap("jogar_fundo.jpg");
				if (!img_fundo_jogar)
				{
					fprintf(stderr, "Falha ao carregar a imagem de fundo do Jogar.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_btn_encerrar = al_load_bitmap("btn_encerrar1.png");
				if (!img_btn_encerrar)
				{
					fprintf(stderr, "Falha ao carregar a imagem de do botão Sair.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_peca1 = al_load_bitmap("bot1.png"); //peca clara
				if (!img_peca1)
				{
					fprintf(stderr, "Falha ao carregar a imagem da peça clara.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_peca2 = al_load_bitmap("bot2.png"); //peca escura
				if (!img_peca2)
				{
					fprintf(stderr, "Falha ao carregar a imagem da peça escura.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_jogar_fundo_direita = al_load_bitmap("jogar_fundo_direita.jpg");
				if (!img_jogar_fundo_direita)
				{
					fprintf(stderr, "Falha ao carregar a imagem do canto direito do tabuleiro.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_jogar_fundo_esquerda = al_load_bitmap("jogar_fundo_esquerda.jpg");
				if (!img_jogar_fundo_esquerda)
				{
					fprintf(stderr, "Falha ao carregar a imagem do canto esquerdo do tabuleiro.\n");
					al_destroy_display(menu);
					return -1;
				}
				img_fundo_vez = al_load_bitmap("fundow.png");
				if (!img_fundo_vez)
				{
					fprintf(stderr, "Falha ao carregar a imagem do canto direito\n");
					al_destroy_display(menu);
					return -1;
				}

				eventos_jogar = al_create_event_queue();
				if (!eventos_jogar)
				{
					fprintf(stderr, "Falha ao criar fila de eventos.\n");
					al_destroy_display(menu);
					return -1;
				}

				al_register_event_source(eventos_jogar, al_get_display_event_source(menu));
				al_register_event_source(eventos_jogar, al_get_mouse_event_source());

				//exibe as imagens do fundo e do botão "sair"
				al_draw_bitmap(img_fundo_jogar ,   0,   0, 0);
				al_draw_bitmap(img_btn_encerrar, 950, 595, 0);

				//exibe o texto da vez dos jogadores
				al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255),  887, 287, ALLEGRO_ALIGN_LEFT, "Vez da peca");

				//coloca as quatro peças iniciais no centro do tabuleiro
				x_1 = 237 + (3 * WIDTH);
				y_1 = 16 + (3 * HEIGHT);
				al_draw_bitmap(img_peca2, x_1, y_1, 0);

				x_1 = 237 + (4 * WIDTH);
				y_1 = 16 + (3 * HEIGHT);
				al_draw_bitmap(img_peca1, x_1, y_1, 0);

				x_1 = 237 + (3 * WIDTH);
				y_1 = 16 + (4 * HEIGHT);
				al_draw_bitmap(img_peca1, x_1, y_1, 0);

				x_1 = 237 + (4 * WIDTH);
				y_1 = 16 + (4 * HEIGHT);
				al_draw_bitmap(img_peca2, x_1, y_1, 0);

				imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);

				//exibe as pontuações zeradas
				if ((opc_modo == 1) && (opc_cor == 2))
				{
					al_draw_text(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "04"); //time escuro
					al_draw_text(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "01"); //time claro
				}
				else
				{
					al_draw_text(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "02"); //time escuro
					al_draw_text(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "02"); //time claro
				}

				//LEMBRAR:
				//opc_modo = 1 -> jogador vs computador
				//opc_modo = 2 -> jogador vs jogador
				//opc_cor  = 1 -> o jogador escolheu ser as peças de cor ESCURA
				//opc_cor  = 2 -> o jogador escolheu ser as peças de cor CLARA

				//o bloco abaixo exibe ao lado direito do tabuleiro qual a peça que seja colocada no primeiro clique do jogo
				if(opc_modo == 1)
				{
					if(opc_cor == 2 && vez_jogador == VERDADEIRO) //preça clara
					{
						al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
					}
					else if(opc_cor == 1) //peça escura
					{
						vez_jogador = VERDADEIRO;
						al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
					}
				}
				else
					al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");

				al_flip_display(); //atualiza a janela

				while(no_btn_encerrar)
				{
					ALLEGRO_EVENT evento_jogar;
					ALLEGRO_TIMEOUT timeout_jogar;
					al_init_timeout(&timeout_jogar, 0.05);

					int tem_eventos_jogar = al_wait_for_event_until(eventos_jogar, &evento_jogar, &timeout_jogar);

					if(tem_eventos_jogar && evento_jogar.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
					{
						//verifica se o "fechar" da janela foi clicado
						//destoi tudo e encerra todo o programa
						al_destroy_event_queue(eventos_jogar);
						al_destroy_event_queue(eventos_menu);
						al_destroy_display(menu);
						return 0;
					}

					//verifica se o evento foi de movimentação do mouse (para os rollovers)
					if (evento_jogar.type == ALLEGRO_EVENT_MOUSE_AXES)
					{
						//verificamos se ele está sobre o botão
						if(evento_jogar.mouse.x >= 950 &&
						   evento_jogar.mouse.x <= 950 + al_get_bitmap_width(img_btn_encerrar) &&
						   evento_jogar.mouse.y >= 595 &&
						   evento_jogar.mouse.y <= 595 + al_get_bitmap_height(img_btn_encerrar) )
						{
							no_btn_encerrar = 2; //está sobre o botão
						}
						else
						{
							no_btn_encerrar = 1; //nao está sobre o botão
						}

						//rollover do botão -----------------------------
						if(no_btn_encerrar==1)
							img_btn_encerrar = al_load_bitmap("btn_encerrar1.png"); //mouse não está sobre o botão
						else if(no_btn_encerrar==2)
							img_btn_encerrar = al_load_bitmap("btn_encerrar2.png"); //mouse está sobre o botão

						al_draw_bitmap(img_btn_encerrar, 950, 595, 0);
						al_flip_display();
					}
					//verifica se o evento foi um clique do mouse
					else if (evento_jogar.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && tem_eventos_jogar == 1)
					{
						//verificar se o botão "encerrar" foi clicado
						if(evento_jogar.mouse.x >= 950 &&
						   evento_jogar.mouse.x <= 950 + al_get_bitmap_width(img_btn_encerrar) &&
						   evento_jogar.mouse.y >= 595 &&
						   evento_jogar.mouse.y <= 595 + al_get_bitmap_height(img_btn_encerrar) )
						{
							//destroi tudo e encerra todo o programa
							al_destroy_event_queue(eventos_jogar);
							al_destroy_event_queue(eventos_menu);
							al_destroy_display(menu);
							return 0;
						}
						else	if((evento_jogar.mouse.x >= 237 && evento_jogar.mouse.x <= 865) &&
							   (evento_jogar.mouse.y >= 16 && evento_jogar.mouse.y <= 640 )) // verifica se uma posição válida do tabuleiro foi clicada
						{
							//verificação casa por casa
							for(conty = 0; conty < 8; conty++)
							{
								for(contx = 0; contx < 8; contx++)
								{
									if(evento_jogar.mouse.x >= 237 + (contx * WIDTH) && evento_jogar.mouse.x <= (237 + ((contx+1) * WIDTH)) &&
									   evento_jogar.mouse.y >= 16 + (conty * HEIGHT) && evento_jogar.mouse.y <= (16 + ((conty+1) * HEIGHT))) // coordenadas da casa (contx, conty)
									{
										//x_1 e y_1 pegam as coordenadas em pixels da casa (contx, conty) correspondente na figura do tabuleiro
										x_1 = 237 + (contx * WIDTH);
										y_1 =  16 + (conty * HEIGHT);

										//237 x 16 são as coordenadas em pixels do canto superior esquerdo do tabuleiro, ou seja, o início.

										//o If verifica se já não tem nenhuma peça na casa
										if(tabuleiro[conty][contx] == 0)
										{
											//opc_modo == 2 significa jogo com 2 players
											if(opc_modo == 2)
											{
												//estes if's verificam de quem é a vez atual para poder atribuir um valor à cor_jogada 
												if(peca_preta == VERDADEIRO)
													cor_jogada = 1; //cor jogada é a cor da peça que entrará nos for's para fazer a jogada válida (se houver)
												else if(peca_preta == FALSO)
													cor_jogada = 2;
											}

											/*Neste bloco, verifica-se se a peça pode ser colocada na posição clicada, e vira
											as peças. Para isso, é verificado todos os caminhos possíveis até os limites do
											tabuleiro, onde pode haver uma peça da mesma cor da peça do ultimo clique, caso
											encontre, o caminho contrário da busca em questão é feito, virando todas as peças
											nesse caminho, até chegar na posição do clique e a peça é colocada. Caso não
											encontre nenhuma peça da mesma cor, então a jogada nao é valido, logo nada é virado
											e a peça do clique não aparece.*/
											if(cor_jogada != 0)
											{
												muda = verifica_jogada(tabuleiro, contx, conty, cor_jogada, 1, 1);
											}

											//imprime a matriz no terminal após a jogada do usuário
											if(vez_jogador == VERDADEIRO)
											{
												printf("\n");
												for(i = 0; i < 8; i++)
												{
													for(j = 0; j < 8; j++)
													{
														fprintf(stderr, "%d ",tabuleiro[i][j]);
													}
													fprintf(stderr, "\n");
												}
												cont++; //Numero da jogada
												printf("Jogada: %d\n\n", cont);
											}

											//imprime no tabuleiro as peças correspondentes aos valores da matriz
											for(i = 0; i < 8; i++)
											{
												for(j = 0; j < 8; j++)
												{
													x_1 = 237 + (j * WIDTH);
													y_1 =  16 + (i * HEIGHT);
													if(tabuleiro[i][j]== 1)
														al_draw_bitmap(img_peca2, x_1, y_1, 0);
													if(tabuleiro[i][j]== 2)
														al_draw_bitmap(img_peca1, x_1, y_1, 0);
												}
											}
											al_flip_display();

											vez++; //conta 1 tentativa

											if(opc_modo == 2) //Testes para trocar a vez dos players no modo '2 Players'
											{
												if(muda == 0) //Se muda é igual a 0, nenhuma jogada válida aconteceu, e passa a vez do jogador
												{
													if(procuraJogadaValida(cor_jogada, tabuleiro, 0) == VERDADEIRO)
														//Se o jogador não fizer uma jogada válida em 3 tentativas, passa a vez para o outro jogador
													{
														if(vez == 3)
														{
															if(peca_preta == VERDADEIRO)
															{
																peca_preta = FALSO;
																imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
																al_flip_display();
															}
															else
															if(peca_preta == FALSO)
															{
																peca_preta = VERDADEIRO;
																imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
																al_flip_display();
															}

															vez = 0; //Zera a variavel para o proximo jogador
														}
													}
												}
												else if(muda != 0) //Se muda é diferente de 0, houve uma jogada válida, então passa a vez
												{
													imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);

													verifica_matriz(p_nao_tem_casa_vazia, p_so_tem_peca_clara, p_so_tem_peca_escura, tabuleiro);
													if(peca_preta == VERDADEIRO)
													{
														if(procuraJogadaValida(2, tabuleiro, 0) == VERDADEIRO)
														{
															peca_preta = FALSO;
															al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
															al_flip_display();
														}
														else
														{
															if(procuraJogadaValida(1, tabuleiro, 0) == FALSO)
															{
																//calcula as pontuações para ver quem venceu
																ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);
																
																//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
																al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
																al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
																al_flip_display();

																//atualiza as pontuações
																al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
																al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

																al_flip_display();

																if(nao_tem_casa_vazia == FALSO)
																{
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há mais jogadas válidas para nenhum jogador.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																}

																if(pont_claro > pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças claras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else if(pont_claro < pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças escuras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "Houve um empate.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

																al_destroy_event_queue(eventos_jogar);
																al_destroy_event_queue(eventos_menu);
																al_destroy_display(menu);
																return 0;
															}
															else
																al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há jogadas válidas para a peça clara.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
														}
													}
													else if(peca_preta == FALSO)
													{
														if(procuraJogadaValida(1, tabuleiro, 0) == VERDADEIRO)
														{
															peca_preta = VERDADEIRO;
															al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
															al_flip_display();
														}
														else
														{
															if(procuraJogadaValida(1, tabuleiro, 0) == FALSO)
															{
																//calcula as pontuações para ver quem venceu
																ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);

																//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
																al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
																al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
																al_flip_display();

																//atualiza as pontuações
																al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
																al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

																al_flip_display();

																if(nao_tem_casa_vazia == FALSO)
																{
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há mais jogadas válidas para nenhum jogador.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																}


																if(pont_claro > pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças claras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else if(pont_claro < pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças escuras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "Houve um empate.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

																al_destroy_event_queue(eventos_jogar);
																al_destroy_event_queue(eventos_menu);
																al_destroy_display(menu);
																return 0;
															}
															else
																al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há jogadas válidas para a peça escura.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);						
														}
													}
													vez  = 0; //reinicia para a proxima execução (proxima rodada)
													muda = 0; //reinicia para a proxima execução (proximo clique)
												}

												imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
												if(peca_preta == VERDADEIRO)
												{
													al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
													al_flip_display();
												}
												else
												{
													al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
													al_flip_display();
												}
											}
											else //se for pra Jogador vs Computador
											{
												if(opc_modo == 1) // Testes para trocar a vez no modo '1 Player' (Jogador vs Computador)
												{
													if(muda == 0)  // se muda é igual a 0, nenhuma jogada válida aconteceu, e passa a vez do jogador
													{
														if(procuraJogadaValida(opc_cor, tabuleiro, 0) == VERDADEIRO) // se houver uma jogada válida para o jogador, então oferece 3 tentativas
														{
															//Se o jogador não fizer uma jogada válida em 3 tentativas, passa a vez para o computador
															if(vez == 3)
															{
																if(vez_jogador == VERDADEIRO)
																{
																	vez_jogador = FALSO;
																	cor_jogada = 0;
																}
																else
																{
																	vez_jogador = VERDADEIRO;
																	cor_jogada = opc_cor;
																}
																vez = 0; //Zera a variavel para o proximo jogador
															}
														}
														else
														{
															//Este if verifica se também não tem nenhuma jogada válida para o jogador da peça oposta
															//neste caso, o jogo se encerra
															if((vez_jogador == FALSO && procuraJogadaValida(opc_cor, tabuleiro, 0) == FALSO) ||
														 	(vez_jogador == VERDADEIRO && procuraJogadaValida(opc_corComputador, tabuleiro, 0)) == FALSO)
															{
																//destoi tudo e encerra todo o programa
																al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há mais jogadas disponíveis para nenhum jogador", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);
																//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
																al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
																al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
																al_flip_display();

																//atualiza as pontuações
																al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
																al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

																al_flip_display();

																if(pont_claro > pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças claras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else if(pont_claro < pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças escuras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "Houve um empate.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

																al_destroy_event_queue(eventos_jogar);
																al_destroy_event_queue(eventos_menu);
																al_destroy_display(menu);
																return 0;
															}
															else if(opc_cor == 1) //entra no else caso não exista nenhuma jogada válida para o jogador em questão, mas existe para o jogador da peça oposta
															{
																//passa a vez para a branca, se o usuário for a preta
																imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
																al_flip_display();
															}
															else
															{
																if(opc_cor == 2) 
																{
																	//passa a vez para a preta, se o usuário for a branca
																	imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																	al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
																	al_flip_display();
																}
															}

															//passa a vez
															if(vez_jogador == VERDADEIRO)
															{
																vez_jogador = FALSO;
																cor_jogada = 0;
															}
															else
															{
																vez_jogador = VERDADEIRO;
																cor_jogada = opc_cor;
															}
														}
													}
													else // se muda é diferente de 0, houve uma jogada válida, então passa a vez direto
													{
														vez  = 0; //reinicia para a proxima execução (proxima rodada)
														muda = 0; //reinicia para a proxima execução (proximo clique)
														imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
														//passa a vez para o computador
														if(vez_jogador == VERDADEIRO)
														{
															vez_jogador = FALSO;
															cor_jogada = 0;
															if(opc_cor == 1)
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
															else if(opc_cor == 2)
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
														}
														else if(vez_jogador == FALSO)
														{
															//passa a vez para o jogador
															vez_jogador = VERDADEIRO;
															cor_jogada = opc_cor;
															if(opc_cor == 1)
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
															else	if(opc_cor == 2)
																al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
														}
													}// if(muda != 0)
				
													if(vez_jogador == FALSO)
													{

														//calcula as pontuações para ver quem venceu
														ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);
														//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
														al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
														al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
														al_flip_display();

														//atualiza as pontuações
														al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
														al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro
														al_flip_display();

														//essa parte faz a jogada do computador
														verifica_matriz(p_nao_tem_casa_vazia, p_so_tem_peca_clara, p_so_tem_peca_escura, tabuleiro);
														if((procuraJogadaValida (opc_corComputador, tabuleiro, 1) == FALSO)&&(nao_tem_casa_vazia == FALSO))
														/*  a condição do if chama a função que procura jogadas válidas para a peça do computador
															o 1 funciona como "habilitador" que indica que é o computador que está jogando */
														{	
															if(procuraJogadaValida (opc_cor, tabuleiro, 0) == VERDADEIRO)
															{

															  	al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há mais jogadas válidas para a peça do computador.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																vez_jogador = VERDADEIRO;
																muda = 0;
																cor_jogada = opc_cor;

																if(opc_cor == 1)
																	al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
																else if(opc_cor == 2)
																	al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
															}
															else
															{
																if(nao_tem_casa_vazia == FALSO)
																  	al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há mais jogadas válidas para nenhum jogador.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																//calcula as pontuações para ver quem venceu
																ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);
																//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
																al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
																al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
																al_flip_display();

																//atualiza as pontuações
																al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
																al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

																al_flip_display();

																if(pont_claro > pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças claras ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else if(pont_claro < pont_escuro)
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador das peças escura ganhou", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																else
																	al_show_native_message_box(NULL, "Reversi", "Jogo", "Houve um empate.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

																al_destroy_event_queue(eventos_jogar);
																al_destroy_event_queue(eventos_menu);
																al_destroy_display(menu);
																return 0;
															}
														}
														else
														{
															//atualiza o tabuleiro após a jogada do computador
															imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
	
															//imprime a matriz no terminal após a jogada do computador
															for(i = 0; i < 8; i++)
															{
																for(j = 0; j < 8; j++)
																{
																	fprintf(stderr, "%d ",tabuleiro[i][j]);
																}
																fprintf(stderr, "\n");
															}

															cont++; //Numero da jogada
															printf("Jogada: %d\n\n", cont);

															verifica_matriz(p_nao_tem_casa_vazia, p_so_tem_peca_clara, p_so_tem_peca_escura, tabuleiro);
											
															if(procuraJogadaValida (opc_cor, tabuleiro, 0) == VERDADEIRO)
															{
																//passa a vez para o usuário
																vez_jogador = VERDADEIRO;
																muda = 0;
																cor_jogada = opc_cor;

																imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																if(opc_cor == 1)
																	al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
																else if(opc_cor == 2)
																	al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");

															}
															else
															{
																if(nao_tem_casa_vazia == FALSO)
																{

																	//calcula as pontuações para ver quem venceu
																	ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);

																	//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
																	al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
																	al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
																	al_flip_display();

																	//atualiza as pontuações
																	al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
																	al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

																	al_flip_display();

																  	al_show_native_message_box(NULL, "Reversi", "Jogo", "Não há mais jogadas válidas para a peça do usuário", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
																	imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);
																	while(procuraJogadaValida(opc_corComputador, tabuleiro, 1) == VERDADEIRO)
																	{
																		verifica_matriz(p_nao_tem_casa_vazia, p_so_tem_peca_clara, p_so_tem_peca_escura, tabuleiro);
																		imprime_pecas (x_1, y_1, tabuleiro, img_peca1, img_peca2, img_fundo_jogar, img_btn_encerrar, img_fundo_vez, fonte_vez);

																		//calcula as pontuações para ver quem venceu
																		ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);
																		
																		//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
																		al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
																		al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
																		al_flip_display();

																		//atualiza as pontuações
																		al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
																		al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

																		al_flip_display();


																		vez_jogador = VERDADEIRO;
																		cor_jogada = opc_cor;


			
																		if(procuraJogadaValida(opc_cor, tabuleiro, 0) == VERDADEIRO)
																		{
																			al_draw_bitmap(img_fundo_vez, 927, 332, 0);
																			al_flip_display();
																			if(opc_cor == 1)
																				al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "escura");
																			else if(opc_cor == 2)
																				al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255), 933, 327, ALLEGRO_ALIGN_LEFT, "clara");
																			break;
																		}
	
																		if(nao_tem_casa_vazia == FALSO)
																			al_show_native_message_box(NULL, "Reversi", "Jogo", "Novamente não há mais jogadas válidas para a peça do usuário", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);	
																	}
																}
															}
														}
													}//FIM - if(vez_jogador == FALSO)
												}//FIM - if(opc_modo == 1)
											}//FIM - else do if(opc_modo == 2)											
										}//FIM - For's
										else
										{
											fprintf(stderr, "Jogada inválida.\n");
										}//FIM - if(tabuleiro[conty][contx] == 0)

										//PONTUAÇÃO-----------------------------------------------------------------------------
										ContaPontuacao (tabuleiro, &pont_escuro, &pont_claro);
										//--------------------------------------------------------------------------------------

										//sobrepoe as pontuações com as imagens dos cantos do tabuleiro
										al_draw_bitmap(img_jogar_fundo_esquerda,   0, 0, 0);
										al_draw_bitmap(img_jogar_fundo_direita , 894, 0, 0);
										al_flip_display();

										//atualiza as pontuações
										al_draw_textf(fonte, al_map_rgb(255, 255, 255),  70, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_escuro); //time escuro
										al_draw_textf(fonte, al_map_rgb(255, 255, 255), 950, 80, ALLEGRO_ALIGN_LEFT, "%02d", pont_claro);  //time claro

										conty = 8; // interrompe o primeiro for
										break;
									}
								}
							}
							al_flip_display();
						}

						/* Função que verifica quem venceu:
						   	A variavel 'nao_tem_casa_vazia' inicializa como VERDADEIRO, e a cada jogada é feito um for que verifica casa 
						   por casa do tabuleiro, se houver casa(s) vazia(s) seu valor muda para FALSO, e a partida continua normalmente.
							Se ao final do for o seu valor permanecer VERDADEIRO, isso fará com que o jogo se encerre mostrando quem é 
						   o vencedor. Caso contrário, ao final deste trecho do código, seu valor é novamente setado como VERDADEIRO, 
						   para prepará-la para a próxima jogada.
						   	O mesmo método é utilizado com as variaveis 'so_tem_peca_clara' e 'so_tem_peca_escura'.
						*/
						verifica_matriz(p_nao_tem_casa_vazia, p_so_tem_peca_clara, p_so_tem_peca_escura, tabuleiro);

						if(nao_tem_casa_vazia == VERDADEIRO)
						{
							//Estes ifs comparam as pontuações de cada jogador
							if(pont_claro > pont_escuro)
								al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador da peça clara ganhou!", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
							else if(pont_claro < pont_escuro)
								al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador da peça escura ganhou!", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
							else
								al_show_native_message_box(NULL, "Reversi", "Jogo", "Houve um empate.", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

							//destoi tudo e encerra todo o programa
							al_destroy_event_queue(eventos_jogar);
							al_destroy_event_queue(eventos_menu);
							al_destroy_display(menu);
							return 0;
						}
						else
						{
							if(so_tem_peca_clara == VERDADEIRO)
							{
								al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador da peça clara ganhou!", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

								//destoi tudo e encerra todo o programa
								al_destroy_event_queue(eventos_jogar);
								al_destroy_event_queue(eventos_menu);
								al_destroy_display(menu);
								return 0;
							}
							else	if(so_tem_peca_escura == VERDADEIRO)
							{
								al_show_native_message_box(NULL, "Reversi", "Jogo", "O jogador da peça escura ganhou!", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

								//destoi tudo e encerra todo o programa
								al_destroy_event_queue(eventos_jogar);
								al_destroy_event_queue(eventos_menu);
								al_destroy_display(menu);
								return 0;
							}
						}
					} //FIM - evento clique do mouse
				} //FIM - while(no_btn_encerrar)
				//FIM - cria a janela de "Jogar" -------------------------------------------------------------------------
			} //FIM - verificar se o botão foi clicado
		} //FIM - verifica se o evento foi um clique do mouse

		//EVENTOS DO BOTÃO "COMO JOGAR"
		//verifica se o evento foi de movimentação do mouse
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			//verificamos se ele está sobre o botão
			if(evento.mouse.x >= 145 &&
			   evento.mouse.x <= 145 + al_get_bitmap_width(img_btn_comojogar) &&
			   evento.mouse.y >= 330 &&
			   evento.mouse.y <= 330 + al_get_bitmap_height(img_btn_comojogar) )
			{
				no_btn_comojogar = 2; //está sobre o botão
			}
			else
			{
				no_btn_comojogar = 1; //nao está sobre o botão
			}
		}
		//verifica se o evento foi um clique do mouse
		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			//verificar se o botão foi clicado
			if(evento.mouse.x >= 145 &&
			   evento.mouse.x <= 145 + al_get_bitmap_width(img_btn_comojogar) &&
			   evento.mouse.y >= 330 &&
			   evento.mouse.y <= 330 + al_get_bitmap_height(img_btn_comojogar) )
			{
				//cria a janela de "Como Jogar" --------------------------------------------------------------------------
				al_resize_display(menu, 600, 375); //redimensiona a janela do Menu para a Como Jogar
				al_set_window_title(menu, "Reversi - Como Jogar");

				//carregando imagens da janela
				img_fundo_comojogar = al_load_bitmap("comojogar_fundo.jpg");
				if (!img_fundo_comojogar){
					fprintf(stderr, "Falha ao carregar a imagem de fundo do Como Jogar.\n");
					al_destroy_display(menu);
					return -1;
				}
				al_draw_bitmap(img_fundo_comojogar, 0, 0, 0);
				al_flip_display();

				//criando o evento
				eventos_comoj = al_create_event_queue();
				if (!eventos_comoj){
					fprintf(stderr, "Falha ao criar fila de eventos.\n");
					al_destroy_display(menu);
					return -1;
				}
				al_register_event_source(eventos_comoj, al_get_display_event_source(menu));
				al_register_event_source(eventos_comoj, al_get_mouse_event_source());

				while(1)
				{
					ALLEGRO_EVENT evento_comojogar;
					ALLEGRO_TIMEOUT timeout_comojogar;
					al_init_timeout(&timeout_comojogar, 0.05);

					int tem_eventos_comojogar = al_wait_for_event_until(eventos_comoj, &evento_comojogar, &timeout_comojogar);

					if(tem_eventos_comojogar && evento_comojogar.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //verifica se o "fechar" da janela foi clicado
					{
						//destoi tudo e encerra todo o programa
						al_destroy_event_queue(eventos_comoj);
						al_destroy_event_queue(eventos_menu);
						al_destroy_display(menu);
						return 0;
					}
				}
				//--------------------------------------------------------------------------------------------------------
			}
		}

		//EVENTOS DO BOTÃO "SOBRE"
		//verifica se o evento foi de movimentação do mouse
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			//verificamos se ele está sobre o botão
			if(evento.mouse.x >= 145 &&
			   evento.mouse.x <= 145 + al_get_bitmap_width(img_btn_sobre) &&
			   evento.mouse.y >= 400 &&
			   evento.mouse.y <= 400 + al_get_bitmap_height(img_btn_sobre) )
			{
				no_btn_sobre = 2; //está sobre o botão
			}
			else
			{
				no_btn_sobre = 1; //nao está sobre o botão
			}
		}
		//verifica se o evento foi um clique do mouse
		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			//verificar se o botão foi clicado
			if(evento.mouse.x >= 145 &&
			   evento.mouse.x <= 145 + al_get_bitmap_width(img_btn_sobre) &&
			   evento.mouse.y >= 400 &&
			   evento.mouse.y <= 400 + al_get_bitmap_height(img_btn_sobre) )
			{
				//cria a janela de "Sobre" --------------------------------------------------------------------------
				al_resize_display(menu, 600, 400); //redimensiona a janela do Menu para a Sobre
				al_set_window_title(menu, "Reversi - Sobre");

				//carregando imagens da janela
				img_fundo_sobre = al_load_bitmap("sobre_fundo.jpg");
				if (!img_fundo_sobre){
					fprintf(stderr, "Falha ao carregar a imagem de fundo do Sobre.\n");
					al_destroy_display(menu);
					return -1;
				}
				al_draw_bitmap(img_fundo_sobre, 0, 0, 0);
				al_flip_display();

				//criando o evento
				eventos_sobre = al_create_event_queue();
				if (!eventos_sobre){
					fprintf(stderr, "Falha ao criar fila de eventos.\n");
					al_destroy_display(menu);
					return -1;
				}
				al_register_event_source(eventos_sobre, al_get_display_event_source(menu));
				al_register_event_source(eventos_sobre, al_get_mouse_event_source());

				while(1)
				{
					ALLEGRO_EVENT evento_sobre;
					ALLEGRO_TIMEOUT timeout_sobre;
					al_init_timeout(&timeout_sobre, 0.05);
					int tem_eventos_sobre = al_wait_for_event_until(eventos_sobre, &evento_sobre, &timeout_sobre);

					if(tem_eventos_sobre && evento_sobre.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //verifica se o "fechar" da janela foi clicado
					{
						//destoi tudo e encerra todo o programa
						al_destroy_event_queue(eventos_sobre);
						al_destroy_event_queue(eventos_menu);
						al_destroy_display(menu);
						return 0;
					}
				}
				//--------------------------------------------------------------------------------------------------------
			}
		}

		//EVENTOS DO BOTÃO "SAIR"
		//verifica se o evento foi de movimentação do mouse
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			//verificamos se ele está sobre o botão
			if(evento.mouse.x >= 145 &&
			   evento.mouse.x <= 145 + al_get_bitmap_width(img_btn_sair) &&
			   evento.mouse.y >= 470 &&
			   evento.mouse.y <= 470 + al_get_bitmap_height(img_btn_sair) )
			{
				no_btn_sair = 2; //está sobre o botão
			}
			else
			{
				no_btn_sair = 1; //nao está sobre o botão
			}
		}
		//verifica se o evento foi um clique do mouse
		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			//verificar se o botão foi clicado
			if(evento.mouse.x >= 145 &&
			   evento.mouse.x <= 145 + al_get_bitmap_width(img_btn_sair) &&
			   evento.mouse.y >= 470 &&
			   evento.mouse.y <= 470 + al_get_bitmap_height(img_btn_sair) )
			{
				no_btn_sair = 0; //fará o looping ser encerrado
			}
		}

		//limpa a tela
		al_clear_to_color(al_map_rgb(0, 0, 0));

		//rollover dos botões -----------------------------
		if(no_btn_jogar==1)
			img_btn_jogar = al_load_bitmap("btn_jogar1.jpg"); //mouse não está sobre o botão
		else if(no_btn_jogar==2)
			img_btn_jogar = al_load_bitmap("btn_jogar2.jpg"); //mouse está sobre o botão

		if(no_btn_comojogar==1)
			img_btn_comojogar = al_load_bitmap("btn_comojogar1.jpg"); //mouse não está sobre o botão
		else if(no_btn_comojogar==2)
			img_btn_comojogar = al_load_bitmap("btn_comojogar2.jpg"); //mouse está sobre o botão

		if(no_btn_sobre==1)
			img_btn_sobre = al_load_bitmap("btn_sobre1.jpg"); //mouse não está sobre o botão
		else if(no_btn_sobre==2)
			img_btn_sobre = al_load_bitmap("btn_sobre2.jpg"); //mouse está sobre o botão

		if(no_btn_sair==1)
			img_btn_sair = al_load_bitmap("btn_sair1.jpg"); //mouse não está sobre o botão
		else if(no_btn_sair==2)
			img_btn_sair = al_load_bitmap("btn_sair2.jpg"); //mouse está sobre o botão
		//-------------------------------------------------

		al_draw_bitmap(img_fundo_menu   ,   0,   0, 0);
		al_draw_bitmap(img_btn_jogar    ,  90, 220, 0);
		al_draw_bitmap(img_btn_comojogar, 145, 330, 0);
		al_draw_bitmap(img_btn_sobre    , 145, 400, 0);
		al_draw_bitmap(img_btn_sair     , 145, 470, 0);

		al_flip_display();
	}

	al_destroy_event_queue(eventos_menu);
	al_destroy_display(menu);
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------ FUNÇÕES -----------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

int verifica_jogada(int tabuleiro[][8], int contx, int conty, int cor_jogada, int controle, int modo)
{
	//controle = 1 -> usa o contador "muda"
	//controle = 2 -> usa o retorno booleano

	int a, b, muda=0;

	//testa linha pela direita
	for(a = contx; a < 8; a++)
	{
		if(a == contx)
		{
			//nao faz nada
		}
		else if((tabuleiro[conty][a] == 0) || ((a==contx+1) && (tabuleiro[conty][a] == cor_jogada)))
		{
			break;
		}
		else	if(tabuleiro[conty][a] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[conty][a] = cor_jogada;
					a--;
				}while(a >= contx);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa linha pela esquerda
	for(a = contx; a>=0; a--)
	{
		if(a == contx)
		{
			//nao faz nada
		}
		else if((tabuleiro[conty][a] == 0) || ((a == contx-1) && (tabuleiro[conty][a] == cor_jogada)))
		{
			break;
		}
		else	if(tabuleiro[conty][a] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[conty][a] = cor_jogada;
					a++;
				}while(a <= contx);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa coluna por cima
	for(a = conty; a < 8; a++)
	{
		if(a == conty)
		{
			//nao faz nada
		}
		else if((tabuleiro[a][contx] == 0) || ((a==conty+1) && (tabuleiro[a][contx] == cor_jogada)))
		{
			break;
		}
		else if(tabuleiro[a][contx] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[a][contx] = cor_jogada;
					a--;
				} while(a >= conty);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa coluna por baixo
	for(a = conty; a>=0; a--)
		{
		if(a == conty)
		{
			//nao faz nada
		}
		else if((tabuleiro[a][contx] == 0) || ((a==conty-1) && (tabuleiro[a][contx] == cor_jogada)))
		{
			break;
		}
		else	if(tabuleiro[a][contx] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[a][contx] = cor_jogada;
					a++;
				}while(a <= conty);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa diagonal principal por cima
	for(a = conty, b = contx; a>=0 && b>=0; a--, b--)
	{
		if(a == conty)
		{
			//nao faz nada
		}
		else if((tabuleiro[a][b] == 0) || ((a == conty-1) && (tabuleiro[a][b] == cor_jogada)))
		{
			break;
		}
		else	if(tabuleiro[a][b] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[a][b] = cor_jogada;
					a++;	b++;
				}while(a <= conty);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa diagonal principal por baixo
	for(a = conty, b = contx; a < 8 && b < 8; a++, b++)
	{
		if(a == conty)
		{
			//nao faz nada
		}
		else if((tabuleiro[a][b] == 0) || ((a == conty+1)&&(tabuleiro[a][b] == cor_jogada)))
		{
			break;
		}
		else	if(tabuleiro[a][b] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[a][b] = cor_jogada;
					a--;	b--;
				}while(a >= conty);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa diagonal secundaria por cima
	for(a = conty, b = contx; a>=0 && b< 8; a--, b++)
	{
		if(a == conty)
		{
			//nao faz nada
		}
		else if((tabuleiro[a][b] == 0) || ((a == conty-1) && (tabuleiro[a][b] == cor_jogada)))
		{
			break;
		}
		else if(tabuleiro[a][b] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[a][b] = cor_jogada;
					a++;	b--;
				}while(a <= conty);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	//testa diagonal secundaria por baixo
	for(a = conty, b = contx; a < 8 && b >= 0; a++, b--)
	{
		if(a == conty)
		{
			//nao faz nada
		}
		else if((tabuleiro[a][b] == 0) || ((a == conty+1)&&(tabuleiro[a][b] == cor_jogada)))
		{
			break;
		}
		else	if(tabuleiro[a][b] == cor_jogada)
		{
			if(modo == 1)
				do{
					tabuleiro[a][b] = cor_jogada;
					a--;	b++;
				}while(a >= conty);
			if (controle == 1)
				muda++;
			else
				return VERDADEIRO;
			break;
		}
	}

	if (controle == 1)
		return muda;
	else
		return FALSO;
}

int procuraJogadaValida (int cor_jogada, int tabuleiro[][8], int modo)
{
	int muda = 0, existe_jogada = FALSO;
	int a, b, contx, conty;

	for(conty = 0; conty < 8; conty++)
	{
		for(contx = 0; contx < 8; contx++)
		{
			if(tabuleiro[conty][contx] == 0)
			{
				if (verifica_jogada(tabuleiro, contx, conty, cor_jogada, 2, modo) == VERDADEIRO)
					return VERDADEIRO;
			}
		}
	}
	return FALSO;
}

void verifica_matriz(int *nao_tem_casa_vazia, int *so_tem_peca_clara, int *so_tem_peca_escura, int tabuleiro[][8])
{
	int i, j;
	(*nao_tem_casa_vazia) = VERDADEIRO;
	(*so_tem_peca_clara)  = VERDADEIRO;
	(*so_tem_peca_escura) = VERDADEIRO;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(tabuleiro[i][j] == 0)
				(*nao_tem_casa_vazia) = FALSO;
			if(tabuleiro[i][j] == 1)
				(*so_tem_peca_clara)  = FALSO;
			if(tabuleiro[i][j] == 2)
				(*so_tem_peca_escura) = FALSO;
		}
	}
}

//Esta função conta o número de 1 e 2, para atualizar as pontuações
void ContaPontuacao (int tabuleiro[][8], int *pont_escuro, int *pont_claro)
{
	int i, j;

	*pont_escuro = 0;   //inicicliza a pontuação do time escuro
	*pont_claro  = 0;   //inicicliza a pontuação do time claro

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(tabuleiro[i][j]== 1)
				(*pont_escuro)++;
			if(tabuleiro[i][j]== 2)
				(*pont_claro)++;
		}
	}
}

void imprime_pecas (int x_1, int y_1, int tabuleiro[][8], 
				ALLEGRO_BITMAP *peca_clara, 
				ALLEGRO_BITMAP *peca_escura,
				ALLEGRO_BITMAP *img_fundo_jogar,
				ALLEGRO_BITMAP *img_btn_encerrar,
				ALLEGRO_BITMAP *img_fundo_vez,
				ALLEGRO_FONT *fonte_vez)
{
	int i, j;

	al_draw_bitmap(img_fundo_jogar ,   0,   0, 0);
	al_draw_bitmap(img_btn_encerrar, 950, 595, 0);
	//imprime toda a matriz das peças no tabuleiro
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			x_1 = 237 + (j * WIDTH);
			y_1 =  16 + (i * HEIGHT);
			if(tabuleiro[i][j]== 1)
				al_draw_bitmap(peca_escura, x_1, y_1, 0);
			if(tabuleiro[i][j]== 2)
				al_draw_bitmap(peca_clara, x_1, y_1, 0);
		}
	}
	al_draw_bitmap(img_fundo_vez, 927, 332, 0);
	al_draw_textf(fonte_vez, al_map_rgb(255, 255, 255),  887, 287, ALLEGRO_ALIGN_LEFT, "Vez da peca");
	al_flip_display();
}
