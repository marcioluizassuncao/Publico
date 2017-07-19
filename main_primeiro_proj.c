/******************************************************************************
 *
 *            Projeto de Sistemas Microcontrolados 
 * 				Exemplo com o PIC18F4550 da Microchip
 *
 ******************************************************************************
 * Nome do Arquivo:     main.c
 * Dependencias:    	Veja a se��o INCLUDES abaixo
 * Processador:       	PIC18F4550
 * Op��o de Clock:		HS 20MHz (externo) - CPU:48MHz
 * Compilador:        	C18 v3.20 ou superior
 *
 * Pr�tica:	 			Pratica 3.0 - Interrup��o
 *
 * Descri��o:	Usar uma fun��o de tratamento de interrup��o de alta prioridade,
 *				para tratar o evento de mudan�a de estado no pino RB4.
 *				O pino RB4 est� conectado ao botao BOOT do Kit PICMINAS e pode
 *				ser usado para tratar uma interrup��o externa.
 *				Dentro da fun��o de tratamento da interrup��o acionar o buzzer para
 *				tocar 2 bip's curtos e alternar o estado do LED_AMARELO.
 *				� importante lembrar que antes de configurar as interrup��es
 *				deve-se desabilitar TODAS elas atrav�s do registrador INTCONbits.GIE
 *				(General Interrupt Enable) e elas s� devem ser habilitadas novamente
 *				antes de entrar no la�o infinito da fun��o principal.
 *
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>	// Necess�rio para que o compilador adicione a biblioteca
						// com as defini��es do PIC selecionado no projeto, neste
						// caso, o modelo 18F4550.

#include <delays.h>		// Biblioteca C18 com fun��es de rotinas de atraso(delay)
						// por exemplo: Delay10TCYx(xxx);
#include <timers.h>
#include <usart.h>
#include <string.h>



/** D E F I N E S ************************************************************/
// LED's do Kit PICMINAS:	saida digital
//atribui��o:	0 = LED desligado
//				1 = LED ligado
#define LED_VERDE 		PORTCbits.RC2
#define LED_VERMELHO 	PORTDbits.RD1
#define LED_AMARELO 	PORTDbits.RD0
#define RELE            PORTCbits.RC0

// Bot�es do Kit PICMINAS: entradas digitais
//atribui��o:	0 = botao n�o pressionado
//				1 = botao pressionado
#define BOTAO_1			PORTEbits.RE1
#define BOTAO_2			PORTEbits.RE2

// O Bot�o de BOOT tem l�gica invertida
//atribui��o:	0 = botao pressionado
//				1 = botao n�o pressionado
#define BOTAO_BOOT		PORTBbits.RB4

// Buzina (buzzer): saida digital
//atribui��o:	0 = buzzer desligado
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
/**  P R O T O T I P O S   D A S   F U N � � E S   D E   I N T E R R U P � � O*/

/** F U N C O E S ************************************************************/

/******************************************************************************
 * Funcao:        	void main(void)
 * Entrada:         Nenhuma (void)
 * Sa�da:          	Nenhuma (void)
 * Descri��o:       Fun��o principal do programa. O termo "main" significa
 *					principal, e isto � relacionado a esta fun��o pois eh a
 *					mesma � a primeira a ser chamada apos o reset da PIC.
 *					Todas as outras fun��es sao chamadas a partir da "main".
 *****************************************************************************/
void main(void)
{

	ConfiguraSistema();		// Configura as portas e perif�ricos do PIC.
    IniciaSistema();
    
	//Desliga todos os LED's
	LED_VERDE=0;
	LED_AMARELO=0;
	LED_VERMELHO=0;
    RELE=0;

	//Desliga Buzzer
	BUZZER=0;
    
	while (1)				//la�o infinito que executa o funcionamento do projeto.
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
 * Sa�da:          Nenhuma (void)
 * Descri��o:      ConfiguraSistema � a rotina de configura��o principal do PIC.
 *					Seu objetivo � configurar as portas de I/O e os perif�ricos
 *					do microcontrolador para que os mesmos trabalhem da maneira
 *					desejada no projeto.
 *
 *****************************************************************************/
void ConfiguraSistema (void)
{
// Configura todas as 13 portas anal�gicas ANx (portas do conversor A/D) como pinos digitais
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
    
// Configura Bot�es do Kit PICMINAS: entradas digitais
	TRISEbits.TRISE1=1; 	// RE1: entrada digital - BOTAO_1
	TRISEbits.TRISE2=1; 	// RE2: entrada digital - BOTAO_2
	TRISBbits.TRISB4=1; 	// RB4: entrada digital - BOTAO_BOOT
    
    

//configura como entradas os pinos RB0, RB1 e RB2;
        TRISBbits.RB0=1;
        TRISBbits.RB1=1;
        TRISBbits.RB2=1;
        
//porta de comando do cooler RE2 - � saida:
          TRISEbits.TRISE0=0;

// Fun��o que configura as interrup��es do sistema
	
   
    
}//end ConfiguraSistema


/******************************************************************************
 * Funcao:         void ConfiguraInterrupcao(void)
 * Entrada:        Nenhuma (void)
 * Sa�da:          Nenhuma (void)
 * Descri��o:      Fun��o que configura as interrup��es utilizadas no projeto
 *
 *****************************************************************************/
void ConfiguraInterrupcao(void)
{

	
 
}// end ConfiguraInterrupcao

/******************************************************************************
 * Funcao:		void Tratamento_High_Interrupt(void)
 * Entrada:     Nenhuma (void)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Fun��o de tratamento das interrup��es de ALTA prioridade
 *				Nessa fun��o deve-se lembrar de fazer a seguinte lista:
 *					1- verificar qual foi a causa da interrup��o, comparando
 *				os flags de cada tipo de interrup��o.
 *					2- tratar a interrup��o selecionada.
 *					3- limpar o flag que causou a interrup��o!!! Importante
 *				para garantir que n�o ocorrer� uma chamada indesejada ao sair
 *				do tratamento da interrup��o.
 *
 *				Ao sair dessa fun��o � usado o retorno do tipo "retfie fast",
 *				pois esta fun��o � declarada como ALTA prioridade com a diretiva
 *				#pragma interrupt
 *
 *****************************************************************************/
// ATEN��O NA SINTAXE de declara��o com #pragma interrupt = Alta prioridade
#pragma interrupt Tratamento_High_Interrupt
void Tratamento_High_Interrupt(void)
{
	   

}// end Tratamento_High_Interrupt

/******************************************************************************
 * Funcao:		void Tratamento_High_Interrupt(void)
 * Entrada:     Nenhuma (void)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Fun��o de tratamento das interrup��es de BAIXA prioridade
 *				Nessa fun��o deve-se lembrar de fazer a seguinte lista:
 *					1- verificar qual foi a causa da interrup��o, comparando
 *				os flags de cada tipo de interrup��o.
 *					2- tratar a interrup��o selecionada.
 *					3- limpar o flag que causou a interrup��o!!! Importante
 *				para garantir que n�o ocorrer� uma chamada indesejada ao sair
 *				do tratamento da interrup��o.
 *
 *				Ao sair dessa fun��o � usado o retorno do tipo "retfie",
 *				pois esta fun��o � declarada como BAIXA prioridade com a diretiva
 *				#pragma interruptlow
 *
 *****************************************************************************/
// ATEN��O NA SINTAXE de declara��o com #pragma interruptlow = Baixa prioridade
#pragma interruptlow Tratamento_Low_Interrupt
void Tratamento_Low_Interrupt(void)
{
	//Escreva o codigo de tratamento da interrup��o de baixa prioridade aqui

}//end Tratamento_Low_Interrupt



/******************************************************************************
 * Funcao:		void Toca_Buzina(void)
 * Entrada:     Nenhuma (void)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Aciona o buzzer (RC1) com dois bip's curtos
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
 Se��o necess�ria para informar ao compilador C18 onde s�o os novos endere�os
da mem�ria de programa que ele deve alocar as rotinas de tratamento do "reset"
do microcontrolador e das rotinas de tratamento de interrup��o.

ATEN��O: COPIAR ESTA SE��O DO CODIGO PARA TODO ARQUIVO "main.c" DOS PROJETOS QUE
UTILIZAM O BOOTLOADER PARA GRAVA��O IN-CIRCUIT.

*/

// prot�tipo usado pelo compilador C18
extern void _startup (void);        // See c018i.c in your C18 compiler dir

// Aloca��o da fun��o de tratamento do "reset" da aplica��o principal
// no endere�o 0x800 da mem�ria de programa
#pragma code REMAPPED_RESET_VECTOR = 0x0001000
void _reset (void)
{
    _asm goto _startup _endasm
}

// Aloca��o da fun��o de tratamento das interrup��es de ALTA prioridade
// no endere�o 0x808 da mem�ria de programa.
//
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x001008
void _high_ISR (void)
{
    _asm goto Tratamento_High_Interrupt _endasm
}

// Aloca��o da fun��o de tratamento das interrup��es de BAIXA prioridade
// no endere�o 0x818 da mem�ria de programa
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = 0x001018
void _low_ISR (void)
{
    _asm goto Tratamento_Low_Interrupt _endasm
}

#pragma code  	// Diretiva que retorna a aloca��o dos endere�os
				// da mem�ria de programa para seus valores padr�o

/** F I M  D A  S E � � O  D E   V E C T O R   R E M A P P I N G *************/

/** EOF main.c ***************************************************************/

