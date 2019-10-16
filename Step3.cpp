#include <LiquidCrystal.h>                              //Biblioteca para o display LCD
#include <Stepper.h>                                    //Biblioteca para o motor de passo

#define STEPS 100
#define motorspeed 30 // set the speed of the motor to 30 RPMs


Stepper stepper(STEPS, 8, 9, 10, 11);


// =============================================================== Acerto da Pinagem ======================================================

const int fcr = 22; // Fim de curso Right
const int fcl = 24; // Fim de curso Left

// ======================================================== Protótipo das Funções Auxiliares ==============================================

void keyboardRead();
int Initialization();

// =============================================================== Variaveis Externas =====================================================

int adc_value = 0x00; // Armazena o valor digital do conversor AD

boolean right  = 0x00, butt01 = 0x00,
        up     = 0x00, butt02 = 0x00,
        down   = 0x00, butt03 = 0x00,
        left   = 0x00, butt04 = 0x00,
        select = 0x00, butt05 = 0x00;

const int resolution = 2; //Averiguar!!

// =============================================================== Variáveis Internas =====================================================

int status1;
int locus; // Destino desejado
int posicao; // Posição atual
int Max; // Distancia máxima da origem [mm]
int mode; // Modo de operação (0, 1 ou 2)

// ================================================================= HARDWARE DO LCD ======================================================

LiquidCrystal disp(8,  //RS no digital 8
                   9,  //EN no digital 9
                   4,  //D4 no digital 4
                   5,  //D5 no digital 5
                   6,  //D6 no digital 6
                   7); //D7 no digital 7

// ==================================================================== VOID SETUP ========================================================

void setup()
{
  pinMode(fcr, INPUT_PULLUP);
  pinMode(fcl, INPUT_PULLUP);
  pinMode(10, OUTPUT); //BackLight
  digitalWrite(10, HIGH);
  stepper.setSpeed(motorspeed);
  status1 = 0;

  Serial.begin(9600); // TEST PROTOBOARD

} //end setup

// ==================================================================== VOID LOOP =========================================================

void loop()
{
  if (status1 == 0) // Verifica se a inicialização ja foi feita (0 = sim e 1 = não)
  {
    Max = Initialization();
    disp.begin(16, 2);                                   //Inicializa LCD 16 x 2
    disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
    disp.print("Distancia Max:");                        //Imprime mensagem
    disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
    disp.print(Max);                                     //Imprime mensagem
    mode = 0;
    posicao = 0;
    delay(2000);
    status1 = 1;
  }
  keyboardRead();
  if (select == 0x01)
  {
    select = 0x00;
    mode = 0; //Reinicia o modo.
  }
  switch (mode)
  {
    case 0: // Nenhum modo selecionado

      disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
      disp.print("Escolha o modo: ");                        //Imprime mensagem
      disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
      disp.print("L:Manual R:Auto ");

      if (right == 0x01) {
        right = 0x00;
        disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
        disp.print("Modo Automatico ");                        //Imprime mensagem
        disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
        disp.print("Escolhido.      ");
        mode = 1;
        locus = posicao;
        delay(1000);
      }
      if (left == 0x01) {
        left = 0x00;
        disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
        disp.print("Modo Manual     ");                        //Imprime mensagem
        disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
        disp.print("Escolhido.      ");
        mode = 2;
        delay(1000);
      }
      break;

    // -----------------------------------------------------------= Modo Automático =---------------------------------------------------

    case 1: // Modo Automático
      if (right == 0x01 and digitalRead(fcr) == HIGH and locus < Max)
      {
        right = 0x00;
        locus = locus + resolution;

        disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
        disp.print("Posicao:        ");                      //Imprime mensagem
        disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
        disp.print(posicao);                                 //Imprime mensagem
        disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
        disp.print("Destino:        ");                      //Imprime mensagem
        disp.setCursor(8, 1);                                //Posiciona cursor na coluna 9, linha 1
        disp.print(locus);                                   //Imprime mensagem
      }
      else if (left == 0x01 and digitalRead(fcl) == HIGH and locus > 1)
      {
        left = 0x00;
        locus = locus - resolution;

        disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
        disp.print("Posicao:        ");                      //Imprime mensagem
        disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
        disp.print(posicao);                                 //Imprime mensagem
        disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
        disp.print("Destino:        ");                      //Imprime mensagem
        disp.setCursor(8, 1);                                //Posiciona cursor na coluna 9, linha 1
        disp.print(locus);                                   //Imprime mensagem
      }
      else if (up == 0x01)
      {
        up = 0x00;
        while (posicao < locus) // Vai para a direita
        {
          posicao = posicao + resolution;

          disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
          disp.print("Posicao:        ");                      //Imprime mensagem
          disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
          disp.print(posicao);                                 //Imprime mensagem
          disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
          disp.print("Destino:        ");                      //Imprime mensagem
          disp.setCursor(8, 1);                                //Posiciona cursor na coluna 9, linha 1
          disp.print(locus);                                   //Imprime mensagem


          //stepper.step(1); // Vai para a direita

        }
        while (posicao > locus)
        {
          posicao = posicao - resolution;

          disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
          disp.print("Posicao:        ");                      //Imprime mensagem
          disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
          disp.print(posicao);                                 //Imprime mensagem
          disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
          disp.print("Destino:        ");                      //Imprime mensagem
          disp.setCursor(8, 1);                                //Posiciona cursor na coluna 9, linha 1
          disp.print(locus);                                   //Imprime mensagem
        }
      }
     
      break;

    // -------------------------------------------------------------= Modo Manual =-----------------------------------------------------

    case 2: // Modo Manual

      //PRINTAR: Position: (posicao)
      //         Máximo : (Max)

      if (right == 0x01 and digitalRead(fcr) == HIGH and (posicao<Max))
      {
        right = 0x00;
        posicao = posicao + resolution;
        //stepper.step(1);

        disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
        disp.print("Posicao:        ");                      //Imprime mensagem
        disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
        disp.print(posicao);                                 //Imprime mensagem
        disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
        disp.print("Maximo:         ");                      //Imprime mensagem
        disp.setCursor(7, 1);                                //Posiciona cursor na coluna 8, linha 1
        disp.print(Max);                                     //Imprime mensagem

      }
      else if (left == 0x01 and digitalRead(fcl) == HIGH and (posicao>1))
      {
        left = 0x00;
        posicao = posicao - resolution;
        //stepper.step(-1);

        disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
        disp.print("Posicao:        ");                      //Imprime mensagem
        disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
        disp.print(posicao);                                 //Imprime mensagem
        disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
        disp.print("Maximo:         ");                      //Imprime mensagem
        disp.setCursor(7, 1);                                //Posiciona cursor na coluna 8, linha 1
        disp.print(Max);                                     //Imprime mensagem
      }
      break;
  }
}

// ================================================================== BOTOES =========================================================

void keyboardRead()
{
  adc_value = analogRead(A0);                       //Variável para leitura recebe valor AD de A0

  // --- Testa se os botões foram pressionados ---
  // Se foi pressionado, seta a respectiva flag
  if      (adc_value <  50)                    butt01 = 0x01;
  else if (adc_value > 103 && adc_value < 200) butt02 = 0x01;
  else if (adc_value > 250 && adc_value < 380) butt03 = 0x01;
  else if (adc_value > 450 && adc_value < 550) butt04 = 0x01;
  else if (adc_value > 650 && adc_value < 750) butt05 = 0x01;



  // --- Testa se os botões foram liberados ---
  //
  if (adc_value > 50 && butt01)             //Botão right solto e flag butt01 setada?
  { //Sim...
    butt01 = 0x00;                        //Limpa flag butt01
    right  = 0x01;                        //Seta flag right

  } //end right
  if (adc_value > 200 && butt02)            //Botão up solto e flag butt02 setada?
  { //Sim...
    butt02 = 0x00;                        //Limpa flag butt02
    up     = 0x01;                        //Seta flag up

  } //end up
  if (adc_value > 380 && butt03)            //Botão down solto e flag butt03 setada?
  { //Sim...
    butt03 = 0x00;                        //Limpa flag butt03
    down   = 0x01;                        //Seta flag down

  } //end down
  if (adc_value > 550 && butt04)            //Botão left solto e flag butt04 setada?
  { //Sim...
    butt04 = 0x00;                        //Limpa flag butt04
    left   = 0x01;                        //Seta flag left

  } //end left
  if (adc_value > 750 && butt05)            //Botão select solto e flag butt05 setada?
  { //Sim...
    butt05 = 0x00;                        //Limpa flag butt05
    select   = 0x01;                        //Seta flag select

  } //end select
}

// ================================================================= INICIALIZAÇÃO ========================================================

int Initialization()
{
  int Dmax;
  int cont = 0;
  while (digitalRead(fcr) == HIGH)
  {
    //stepper.step(1); // Vai para a direita
    Serial.print("Inicialização Direita \n"); // TEST PROTOBOARD
    delay(1000); // TEST PROTOBOARD

  }
  while (digitalRead(fcl) == HIGH)
  {
    //stepper.step(-1); // Vai para a esquerda
    cont++;
    Serial.print("Inicialização Esquerda  \n"); // TEST PROTOBOARD
    delay(1000); // TEST PROTOBOARD
  }
  //Printar Cont
  Dmax = cont * resolution;
  return Dmax;
}




