
#include "io430.h"

#define ROLETAS       3
#define TEMPOREPIQUE  35000
#define TEMPO         4000
#define LIMITE        99

//                      0    1    2    3    4    5    6    7    8    9 
unsigned char cod[] = {0x3F,0x21,0x76,0x73,0x69,0x5B,0x5F,0x31,0x7F,0x7B}; 
/*------------------------------------------------------------------------------
Nome: Acabou
Descrição: Essa função faz aparecer as letras E e G nos display de sete segmentos
fazendo com que uma letra aparendo cada letra um display, de uma maneira infinita
para indicar que o limite de pessoas foi estrapolado.
Nome:           -
Saída:          -
------------------------------------------------------------------------------*/
void acabou(void)
{
  while(1)
  {    
    P1OUT |= 0x02;                      // Liga o MOSFET Q1 assim,habilitando o Display das dezenas.
    P2OUT |= 0X5E;                      // Acende os segmentos do display das dezenas formando a letra E.(DY2)  
    __delay_cycles(TEMPO);              // Chama a função do que permite que os segmentos fiquem acesos por um periodo de tempo.
    P1OUT &= 0xFD;                      // Desabilita o displays das dezenas (DY2).
    P2OUT &= 0X80;                      // Apaga todos os segmentos acesos.
    P1OUT |= 0x01;                      // Liga o MOSFET Q2 assim, habilitando o Display das unidades.
    P2OUT |= 0X5F;                      // Acende os segmentos do display das unidades formando a letra G.(DY1)
   __delay_cycles(TEMPO);               // Chama a função do que permite que os segmentos fiquem acesos por um periodo de tempo.
    P1OUT &= 0xFE;                      // Desabilita os displays das unidades (DY1).
    P2OUT &= 0X80;                      // Apaga todos os segmentos acesos.
  }
}
/*---------------------------------------------------------------------------- 
NOME: MostraDisplay 
DESCRICAO: Apresenta em dois displays de 7 segmentos o valor da dezena e unidades
de qualquer número de 2 dígitos decimal.
ENTRADA:    valor - NÚMERO EM HEXADECIMAL QUE SERÁ MOSTRADO 
SAIDA:            - 
---------------------------------------------------------------------------- */ 
void MostraDisplay(unsigned char valor) 
{ 
    unsigned char unidade=valor,dezena=0; // Cria e inicia  as variaveis unidade que recebe o valor a ser mostrado e  a variavel dezenas.
    while (unidade > 9)                   // Pergunta se o valor presente nas unidads é maior que 9.
     { 
     dezena++;                            // Incrementa o valor das dezenas.      
     unidade -= 10;                       //Sutrai 10 do valor das unidades.
     } 
    P1OUT |= 0x02;                        // Liga o MOSFET Q1 assim,habilitando o Display das dezenas.
    P2OUT |= cod[dezena];                 // Carrega o valor presente na casa das dezenas no display.
    __delay_cycles(TEMPO);                // Chama a função do que permite que os segmentos fiquem acesos por um periodo de tempo.
    P1OUT &= 0xFD;                        // Desabilita o displays das dezenas (DY2).
    P2OUT &= 0X80;                        // Apaga todos os segmentos acesos.
    P1OUT |= 0x01;                        // Liga o MOSFET Q2 assim, habilitando o Display das unidades.
    P2OUT |= cod[unidade];                // Carrega o valor presente na casa das unidads no display.
    __delay_cycles(TEMPO);                // Chama a função do que permite que os segmentos fiquem acesos por um periodo de tempo.
    P1OUT &= 0xFE;                        // Desabilita os displays das unidades (DY1).
    P2OUT &= 0X80;                        // Apaga todos os segmentos acesos.
} 
/*------------------------------------------------------------------------------
Nome: totalRoletas
Descrição: Soma o valor de todas as roletas em uma variavel chamada total
Entrada:   int*roletas - Indica aonde inicia o vetor das roletas
saída:     unsigned int total - É a variavel que irá guardar o total de pessoas
------------------------------------------------------------------------------*/
unsigned int totalRoletas (int *roletas) 
{ 
 int cont;                                // Cria a variavel do contador
 unsigned int total=0;                    // Cria a variavel do total de pessoas
 for(cont=0; cont<ROLETAS; cont++)        // Cria um loop para somar todas as roletas
 total+=roletas[cont];                    // Adiciona totas as roletas na variavel total
 return(total);                           // Retorna a variavel com o total de pessoas da roleta.
} 

/*------------------------------------------------------------------------------
Nome: ZeraRoletas
Descrição: Essa roleta zero o valor de totas as roletas presenets no vetor
Entrada:   int*roletas - Indica aonde inicia o vetor das roletas
saída:                 -
------------------------------------------------------------------------------*/
void zeraRoletas (int *roletas) 
{ 
 int cont;                               // Cria a varivel do contador
 for(cont=0; cont<ROLETAS; cont++)       // Cria um loop para carregar zero em todas as roletas.
 roletas[cont]=0;                        // Carrega zero em totasl as roletas
} 

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
    P1DIR &= 0x07;         //Configura as entradas do pino 1.
    P1DIR |= 0x03;         //Configura as saídas do pino 1.
    
    P1REN |= 0xF8;         //Habilita os resistores de PullUp/Down.
    P1OUT |= 0xF8;         //Configura os resistores para PullUp.
    
    P2DIR |= 0x7F;         //Configura as saídas do pino 2.
    
    P2SEL &= 0x00; 
    
  // Criando as roletas, zerando elas e criando a variavel do publico total. 
    unsigned int publico=0; 
    int rol[ROLETAS]; 
    zeraRoletas(rol); 

  // Começa a leitura das chaves e dos botões das roletas.
    while(1)
      {
         switch(P1IN & 0xF8)
         { 
          // Chave SW4
          case 0xD8:         
          case 0x98:
          case 0x58:
          case 0x18:
                             // Os casos anteriores verificam se a chave SW4 foi acionada em algum momento
    // considerando todas as possibilidades das chaves sell0 e sell1, aonde em qualquer situação independete do
    // das chaves sell0 e sell 1 a função da roleta que será acionada.
            
            
            __delay_cycles(TEMPOREPIQUE);               // Chama a subrotina da temporização para tratar o repique e dar um tempo para a chave estabilizar
            while(1)                                    // Cria um loop infinito aonde a unica condição de sair e quando o botão da roleta 1 não estiver apertado, para que não 
                                                        //seja contado mais de uma pessoa por vez, caso o botão seja apertado e segurado.
            {
              if ((P1IN != 0xd8)&&(P1IN != 0x98)&&(P1IN != 0x58)&&(P1IN != 0x18)) // Condição para a saida
                break;
            }
            rol[0] += 1;                                // Incrementa a roleta 1
            publico=totalRoletas(rol);                  // Soma o total de pessoas passadas na roleta
            if (publico > LIMITE)                       // Pergunta se o limite de publico foi atingido.
              acabou();                                 // Chama a função que mostra que o limite foi atingido.
            break;
                   
            
          // Chave SW3
          case 0xA8:        
          case 0x68:
          case 0x28:
          case 0xE8:  
                             // Os casos anteriores verificam se a chave SW3 foi acionada em algum momento
    // considerando todas as possibilidades das chaves sell0 e sell1, aonde em qualquer situação independete do
    // das chaves sell0 e sell 1 a função da roleta que será acionada.
            __delay_cycles(TEMPOREPIQUE);               // Chama a subrotina da temporização para tratar o repique e dar um tempo para a chave estabilizar
            
             while(1)                                   // Cria um loop infinito aonde a unica condição de sair e quando o botão da roleta 2 não estiver apertado, para que não 
                                                        //seja contado mais de uma pessoa por vez, caso o botão seja apertado e segurado.
            {
              if ((P1IN != 0xA8)&&(P1IN != 0x68)&&(P1IN != 0x28)&&(P1IN != 0xE8))// Condição para a saida
                break;          
            }
            rol[1] += 1;                                // Incrementa a roleta 2
            publico=totalRoletas(rol);                  // Soma o total de pessoas passadas na roleta
            if (publico > LIMITE)                       // Pergunta se o limite de publico foi atingido.
              acabou();                                 // Chama a função que mostra que o limite foi atingido.            
            break;
            
            
          // Chave SW2        
          case 0xB0:         
          case 0x70:
          case 0x30:
          case 0xF0:   
                             // Os casos anteriores verificam se a chave SW2 foi acionada em algum momento
    // considerando todas as possibilidades das chaves sell0 e sell1, aonde em qualquer situação independete do
    // das chaves sell0 e sell 1 a função da roleta que será acionada.
            __delay_cycles(TEMPOREPIQUE);               // Chama a subrotina da temporização para tratar o repique e dar um tempo para a chave estabilizar
            
             while(1)                                   // Cria um loop infinito aonde a unica condição de sair e quando o botão da roleta 3 não estiver apertado, para que não 
                                                        //seja contado mais de uma pessoa por vez, caso o botão seja apertado e segurado.
            {
              if ((P1IN != 0xB0)&&(P1IN != 0x70)&&(P1IN != 0x30)&&(P1IN != 0xF0))// Condição para a saida
                break;                 
            }
            rol[2] += 1;                                // Incrementa a roleta 3            
            publico=totalRoletas(rol);                  // Soma o total de pessoas passadas na roleta
            if (publico > LIMITE)                       // Pergunta se o limite de publico foi atingido.
              acabou();                                 // Chama a função que mostra que o limite foi atingido.
            break;         
         }         
         
         
         switch(P1IN&0xF8)
         {
          case 0xF8:      //Verifica se a portas Sel0 e Sell1 não foi acionada.
            MostraDisplay(publico);  // Chama a função para mostrar o publuco total 
            break;
            
          case 0xB8:      //Verifica se a portas Sel0  foi acionada e Sell1 não foi acionada.
            MostraDisplay(rol[0]); // Chama a função para mostrar o publico da roleta 1 
            break;
            
          case 0x78:     //Verifica se a portas Sel0 não foi acionada e Sell1 foi acionada .
            MostraDisplay(rol[1]);// Chama a função para mostrar o publico da roleta 2.
            break;
            
          case 0x38:     //Verifica se a portas Sell0 e Sell1 foram acionada juntas.
            MostraDisplay(rol[2]);// Chama a função para mostrar o publico da roleta 3.
            break;
         }  
        
      }
}
