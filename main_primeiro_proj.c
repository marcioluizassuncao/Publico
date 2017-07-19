/******************************************************************************
 *
 *            Projeto de Sistemas Microcontrolados 
 * 				Exemplo com o PIC18F4550 da Microchip
 *
 ******************************************************************************
 * Nome do Arquivo:     main.c
 * Dependencias:    	Veja a seção INCLUDES abaixo
 * Processador:       	PIC18F4550
 * Opção de Clock:		HS 20MHz (externo) - CPU:48MHz
 * Compilador:        	C18 v3.20 ou superior
 *
 * Prática:	 			Pratica 3.0 - Interrupção
 *
 * Descrição:	Usar uma função de tratamento de interrupção de alta prioridade,
 *				para tratar o evento de mudança de estado no pino RB4.
 *				O pino RB4 está conectado ao botao BOOT do Kit PICMINAS e pode
 *				ser usado para tratar uma interrupção externa.
 *				Dentro da função de tratamento da interrupção acionar o buzzer para
 *				tocar 2 bip's curtos e alternar o estado do LED_AMARELO.
 *				É importante lembrar que antes de configurar as interrupções
 *				deve-se desabilitar TODAS elas através do registrador INTCONbits.GIE
 *				(General Interrupt Enable) e elas só devem ser habilitadas novamente
 *				antes de entrar no laço infinito da função principal.
 *
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>	// Necessário para que o compilador adicione a biblioteca
						// com as definições do PIC selecionado no projeto, neste
						// caso, o modelo 18F4550.

#include <delays.h>		// Biblioteca C18 com funções de rotinas de atraso(delay)
						// por exemplo: Delay10TCYx(xxx);
#include <timers.h>
#include <usart.h>
#include <string.h>



/** D E F I N E S ************************************************************/
// LED's do Kit PICMINAS:	saida digital
//atribuição:	0 = LED desligado
//				1 = LED ligado
#define LED_VERDE 		PORTCbits.RC2
#define LED_VERMELHO 	PORTDbits.RD1
#define LED_AMARELO 	PORTDbits.RD0
#define RELE            PORTCbits.RC0

// Botões do Kit PICMINAS: entradas digitais
//atribuição:	0 = botao não pressionado
//				1 = botao pressionado
#define BOTAO_1			PORTEbits.RE1
#define BOTAO_2			PORTEbits.RE2

// O Botão de BOOT tem lógica invertida
//atribuição:	0 = botao pressionado
//				1 = botao não pressionado
#define BOTAO_BOOT		PORTBbits.RB4

// Buzina (buzzer): saida digital
//atribuição:	0 = buzzer desligado
//				1 = buzzer ligado
#define BUZZER 			PORTCbits.RC1


/** V A R I A V E I S   G L O B A I S ****************************************/
int contador_encoder=0;
int direcao=0;
int rs0=0;
int rs1=0;
int contabotao=0;
int iflag=0;

int conta2seg=0;

//timer0 contador de tempo seg. min. hora
int  contador_timer_0=0;	// faz a contagem do tempo do cronometro
unsigned char hora=0;
unsigned char minuto=0;
unsigned char segundo=0;

//controle do cooler contador de giro
 int contadorclock_old=0;
 int contadorclock1=0;


   unsigned char string[9];
/**  P R O T O T I P O S   P R I V A D O S ***********************************/
void ConfiguraSistema (void);
void Toca_Buzina(void);
void IniciaSistema(void);
/**  P R O T O T I P O S   D A S   F U N Ç Õ E S   D E   I N T E R R U P Ç Ã O*/

/** F U N C O E S ************************************************************/

/******************************************************************************
 * Funcao:        	void main(void)
 * Entrada:         Nenhuma (void)
 * Saída:          	Nenhuma (void)
 * Descrição:       Função principal do programa. O termo "main" significa
 *					principal, e isto é relacionado a esta função pois eh a
 *					mesma é a primeira a ser chamada apos o reset da PIC.
 *					Todas as outras funções sao chamadas a partir da "main".
 *****************************************************************************/
void main(void)
{

	ConfiguraSistema();		// Configura as portas e periféricos do PIC.
    IniciaSistema();
    
	//Desliga todos os LED's
	LED_VERDE=0;
	LED_AMARELO=0;
	LED_VERMELHO=0;
    RELE=0;

	//Desliga Buzzer
	BUZZER=0;
    
	while (1)				//laço infinito que executa o funcionamento do projeto.
	{

		
             //Delay100TCYx(40);
             //LED_VERDE = !LED_VERDE ;
             while(BOTAO_1==1){
               Delay100TCYx(10);
               LED_VERMELHO=!LED_VERMELHO; //Liga MOTOR1
               
               RELE=!RELE;
               
                 
               
             }
              while(BOTAO_2==1){
               
              LED_AMARELO=!LED_AMARELO; //Liga MOTOR2
              
             }
             if(BOTAO_BOOT==0){
                 
                 LED_VERDE=!LED_VERDE; //Liga BOMBA
             }

           

            // Delay10KTCYx(100);






	}//end while


}//end main
void IniciaSistema(void){
    LED_VERMELHO=0;
    LED_AMARELO=0;
    LED_VERDE=0;
    BUZZER=0;
    
}

/******************************************************************************
 * Funcao:         void ConfiguraSistema(void)
 * Entrada:        Nenhuma (void)
 * Saída:          Nenhuma (void)
 * Descrição:      ConfiguraSistema é a rotina de configuração principal do PIC.
 *					Seu objetivo é configurar as portas de I/O e os periféricos
 *					do microcontrolador para que os mesmos trabalhem da maneira
 *					desejada no projeto.
 *
 *****************************************************************************/
void ConfiguraSistema (void)
{
// Configura todas as 13 portas analógicas ANx (portas do conversor A/D) como pinos digitais
	ADCON1 |= 0x0F;			// Todos os pinos como digital
// No PIC18F4550 temos:
// AN0=RA0
// AN1=RA1
// AN2=RA2
// AN3=RA3
// AN4=RA5
// AN5=RE0
// AN6=RE1
// AN7=RE2
// AN8=RB2
// AN9=RB3
// AN10=RB1
// AN11=RB4
// AN12=RB0


// Configura LED's do Kit PICMINAS: saida digital
	TRISCbits.TRISC2=0; 	// RC2: saida digital - LED_VERDE
	TRISDbits.TRISD1=0; 	// RD1: saida digital - LED_VERMELHO
	TRISDbits.TRISD0=0; 	// RD0: saida digital - LED_AMARELO

// Buzina (buzzer): saida digital
	TRISCbits.TRISC1=0; 	// RC1: saida digital - BUZZER
    TRISCbits.TRISC0=0; 	// RC0: saida digital - RELE
    
// Configura Botões do Kit PICMINAS: entradas digitais
	TRISEbits.TRISE1=1; 	// RE1: entrada digital - BOTAO_1
	TRISEbits.TRISE2=1; 	// RE2: entrada digital - BOTAO_2
	TRISBbits.TRISB4=1; 	// RB4: entrada digital - BOTAO_BOOT
    
    

//configura como entradas os pinos RB0, RB1 e RB2;
        TRISBbits.RB0=1;
        TRISBbits.RB1=1;
        TRISBbits.RB2=1;
        
//porta de comando do cooler RE2 - é saida:
          TRISEbits.TRISE0=0;

// Função que configura as interrupções do sistema
	
   
    
}//end ConfiguraSistema


/******************************************************************************
 * Funcao:         void ConfiguraInterrupcao(void)
 * Entrada:        Nenhuma (void)
 * Saída:          Nenhuma (void)
 * Descrição:      Função que configura as interrupções utilizadas no projeto
 *
 *****************************************************************************/
void ConfiguraInterrupcao(void)
{

	
 
}// end ConfiguraInterrupcao

/******************************************************************************
 * Funcao:		void Tratamento_High_Interrupt(void)
 * Entrada:     Nenhuma (void)
 * Saída:       Nenhuma (void)
 * Descrição:   Função de tratamento das interrupções de ALTA prioridade
 *				Nessa função deve-se lembrar de fazer a seguinte lista:
 *					1- verificar qual foi a causa da interrupção, comparando
 *				os flags de cada tipo de interrupção.
 *					2- tratar a interrupção selecionada.
 *					3- limpar o flag que causou a interrupção!!! Importante
 *				para garantir que não ocorrerá uma chamada indesejada ao sair
 *				do tratamento da interrupção.
 *
 *				Ao sair dessa função é usado o retorno do tipo "retfie fast",
 *				pois esta função é declarada como ALTA prioridade com a diretiva
 *				#pragma interrupt
 *
 *****************************************************************************/
// ATENÇÃO NA SINTAXE de declaração com #pragma interrupt = Alta prioridade
#pragma interrupt Tratamento_High_Interrupt
void Tratamento_High_Interrupt(void)
{
	   

}// end Tratamento_High_Interrupt

/******************************************************************************
 * Funcao:		void Tratamento_High_Interrupt(void)
 * Entrada:     Nenhuma (void)
 * Saída:       Nenhuma (void)
 * Descrição:   Função de tratamento das interrupções de BAIXA prioridade
 *				Nessa função deve-se lembrar de fazer a seguinte lista:
 *					1- verificar qual foi a causa da interrupção, comparando
 *				os flags de cada tipo de interrupção.
 *					2- tratar a interrupção selecionada.
 *					3- limpar o flag que causou a interrupção!!! Importante
 *				para garantir que não ocorrerá uma chamada indesejada ao sair
 *				do tratamento da interrupção.
 *
 *				Ao sair dessa função é usado o retorno do tipo "retfie",
 *				pois esta função é declarada como BAIXA prioridade com a diretiva
 *				#pragma interruptlow
 *
 *****************************************************************************/
// ATENÇÃO NA SINTAXE de declaração com #pragma interruptlow = Baixa prioridade
#pragma interruptlow Tratamento_Low_Interrupt
void Tratamento_Low_Interrupt(void)
{
	//Escreva o codigo de tratamento da interrupção de baixa prioridade aqui

}//end Tratamento_Low_Interrupt



/******************************************************************************
 * Funcao:		void Toca_Buzina(void)
 * Entrada:     Nenhuma (void)
 * Saída:       Nenhuma (void)
 * Descrição:   Aciona o buzzer (RC1) com dois bip's curtos
 *
 *****************************************************************************/
void Toca_Buzina(void)
{
	BUZZER=0;			// desliga BUZZER
	BUZZER=1;			// liga BUZZER
	Delay10KTCYx(100);	// espera 50ms
	BUZZER=0;			// desliga BUZZER
}


/** V E C T O R   R E M A P P I N G ******************************************/
/*
 Seção necessária para informar ao compilador C18 onde são os novos endereços
da memória de programa que ele deve alocar as rotinas de tratamento do "reset"
do microcontrolador e das rotinas de tratamento de interrupção.

ATENÇÃO: COPIAR ESTA SEÇÃO DO CODIGO PARA TODO ARQUIVO "main.c" DOS PROJETOS QUE
UTILIZAM O BOOTLOADER PARA GRAVAÇÃO IN-CIRCUIT.

*/

// protótipo usado pelo compilador C18
extern void _startup (void);        // See c018i.c in your C18 compiler dir

// Alocação da função de tratamento do "reset" da aplicação principal
// no endereço 0x800 da memória de programa
#pragma code REMAPPED_RESET_VECTOR = 0x0001000
void _reset (void)
{
    _asm goto _startup _endasm
}

// Alocação da função de tratamento das interrupções de ALTA prioridade
// no endereço 0x808 da memória de programa.
//
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x001008
void _high_ISR (void)
{
    _asm goto Tratamento_High_Interrupt _endasm
}

// Alocação da função de tratamento das interrupções de BAIXA prioridade
// no endereço 0x818 da memória de programa
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = 0x001018
void _low_ISR (void)
{
    _asm goto Tratamento_Low_Interrupt _endasm
}

#pragma code  	// Diretiva que retorna a alocação dos endereços
				// da memória de programa para seus valores padrão

/** F I M  D A  S E Ç Ã O  D E   V E C T O R   R E M A P P I N G *************/

/** EOF main.c ***************************************************************/

