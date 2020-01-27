
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD
#include <Stepper.h> //Biblioteca para o motor de passo



#define STEPS 2050     //Number of steps for revolution
#define motorspeed 3   // set the speed of the motor to 30 RPMs





// =============================================================== Acerto da Pinagem ======================================================

#define fcr A3 // Fim de curso Right
#define fcl A2 // Fim de curso Left

// ======================================================== Protótipo das Funções Auxiliares ==============================================

void keyboardRead();
int Initialization();
void ImprimeAutomatico();
void ImprimeManual();
void Direita();
void Esquerda();

// =============================================================== Variaveis Externas =====================================================

int adc_value = 0x00; // Armazena o valor digital do conversor AD

boolean right  = 0x00, butt01 = 0x00,
up     = 0x00, butt02 = 0x00,
down   = 0x00, butt03 = 0x00,
left   = 0x00, butt04 = 0x00,
select = 0x00, butt05 = 0x00;

const float resolution = 0.053;//0.425; //5325 passos para 282mm => 282/5325 = 0.425

// =============================================================== Variáveis Internas =====================================================

int status1;
int continuaright;
int continualeft;
const int velocidadebotao = 25;
float locus; // Destino desejado
float posicao; // Posição atual
float Max; // Distancia máxima da origem [mm]
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
  pinMode(A5, OUTPUT);  //Passo
  pinMode(A4, OUTPUT);  //Sentido
  pinMode(A3, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(10, OUTPUT);  //BackLight
  digitalWrite(10, HIGH);


  status1 = 0;

  // Serial.begin(9600); // TEST PROTOBOARD

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

  keyboardRead(); // Faz a leitura dos estados dos botoes

  if (select == 0x01)
  {
    select = 0x00;
    mode = 0; //Reinicia o modo.
  }
  switch (mode)
  {
  case 0: // Nenhum modo selecionado

    disp.setCursor(0, 0);                                  //Posiciona cursor na coluna 1, linha 1
    disp.print("Escolha o modo: ");                        //Imprime mensagem
    disp.setCursor(0, 1);                                  //Posiciona cursor na coluna 1, linha 2
    disp.print("L:Manual R:Auto ");

    if (right == 0x01) {
      right = 0x00;

      disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
      disp.print("Modo Automatico ");                      //Imprime mensagem
      disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
      disp.print("Escolhido.      ");

      mode = 1;
      locus = posicao;
      delay(1000);

      ImprimeAutomatico(posicao, locus);                   //Imprime mensagem - Automatico

    }
    if (left == 0x01) {
      left = 0x00;
      disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
      disp.print("Modo Manual     ");                      //Imprime mensagem
      disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
      disp.print("Escolhido.      ");                      //Imprime mensagem - Manual

      mode = 2;
      delay(1000);

      ImprimeManual(posicao, Max);                         //Imprime mensagem - Manual

    }
    break;

    // -----------------------------------------------------------= Modo Automático =---------------------------------------------------

  case 1: // Modo Automático
    if (continuaright == 0) right = 0x00; // Se o botão right não estiver apertado, ele não precisa ir pra direita.
    if (continualeft == 0) left = 0x00;   // Se o botão left não estiver apertado, ele não precisa ir pra esquerda.

    if ((right == 0x01 or continuaright == 1) and digitalRead(fcr) == HIGH and locus < Max)
    {
      if (continuaright == 1) //delay(velocidadebotao);
        locus = locus + resolution;

      ImprimeAutomatico(posicao, locus);                         //Imprime mensagem - Automatico

    }
    else if ((left == 0x01 or continualeft == 1) and digitalRead(fcl) == HIGH and locus > 1)
    {
      if (continualeft == 1) //delay(velocidadebotao);
        locus = locus - resolution;

      ImprimeAutomatico(posicao, locus);                         //Imprime mensagem - Automatico

    }
    else if (up == 0x01)
    {
      up = 0x00;
      while (posicao < locus) // Vai para a direita
      {
        posicao = posicao + 2*resolution;

        ImprimeAutomatico(posicao, locus);                         //Imprime mensagem - Automatico

        Direita(2);

      }
      while (posicao > locus)
      {
        posicao = posicao - resolution;

        ImprimeAutomatico(posicao, locus);                         //Imprime mensagem - Automatico

        Esquerda(1);
      }
    }

    break;

    // -------------------------------------------------------------= Modo Manual =-----------------------------------------------------

  case 2: // Modo Manual
    if (continuaright == 0) right = 0x00; // Se o botão right não estiver apertado, ele não precisa ir pra direita.
    if (continualeft == 0) left = 0x00;   // Se o botão left não estiver apertado, ele não precisa ir pra esquerda.

    if ((right == 0x01 or continuaright == 1) and digitalRead(fcr) == HIGH and (posicao < Max))
    {
      if (continuaright == 1) //delay(velocidadebotao);
        posicao = posicao + resolution;

      Direita(1);


      ImprimeManual(posicao, Max);                         //Imprime mensagem - Manual

    }
    else if ((left == 0x01 or continualeft == 1) and digitalRead(fcl) == HIGH and (posicao > 1))
    {
      if (continualeft == 1) //delay(velocidadebotao);
        posicao = posicao - resolution;

      Esquerda(1);


      ImprimeManual(posicao, Max);                         //Imprime mensagem - Manual
    }
    break;
  }
}

// ============================================================ IMPRIME AUTOMATICO ===================================================

void ImprimeAutomatico(int posicao, int locus)
{
  disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("Posicao:        ");                      //Imprime mensagem
  disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
  disp.print(posicao);                                 //Imprime mensagem
  disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
  disp.print("Destino:        ");                      //Imprime mensagem
  disp.setCursor(8, 1);                                //Posiciona cursor na coluna 9, linha 1
  disp.print(locus);                                   //Imprime mensagem
}

// ============================================================== IMPRIME MANUAL =====================================================

void ImprimeManual(int posicao, int Max)
{
  disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("Posicao:        ");                      //Imprime mensagem
  disp.setCursor(8, 0);                                //Posiciona cursor na coluna 9, linha 1
  disp.print(posicao);                                 //Imprime mensagem
  disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
  disp.print("Maximo:         ");                      //Imprime mensagem
  disp.setCursor(7, 1);                                //Posiciona cursor na coluna 8, linha 1
  disp.print(Max);
}

// ================================================================== BOTOES =========================================================

void keyboardRead()
{
  adc_value = analogRead(A0);                       //Variável para leitura recebe valor AD de A0

    // --- Testa se os botões foram pressionados ---

  // Se foi pressionado, seta a respectiva flag
  if      (adc_value <  75)
  {

    butt01 = 0x01;
    continuaright = 1;
  }
  else if (adc_value > 103 && adc_value < 200) butt02 = 0x01;
  else if (adc_value > 250 && adc_value < 380) butt03 = 0x01;
  else if (adc_value > 450 && adc_value < 550)
  {
    butt04 = 0x01;
    continualeft = 1;
  }
  else if (adc_value > 650 && adc_value < 750) butt05 = 0x01;

  // --- Testa se os botões foram liberados --- & --- Testa se Left ou Right continuam pressionados ---
  if (adc_value > 75 && butt01)             //Botão right solto e flag butt01 setada?
  { //Sim...
    butt01 = 0x00;                        //Limpa flag butt01
    right  = 0x01;                        //Seta flag right
    continuaright = 0;

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
    continualeft = 0;

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
  //int Dmax;
  //int cont = 0;
  //while (digitalRead(fcr) == HIGH)
  //{
  //  Direita(1);

  //}
  while (digitalRead(fcl) == HIGH)
  {
    Esquerda(1);
  //  cont++;
  }
  //Dmax = cont * resolution;
  //return Dmax;
  return 282;
}

// ================================================================= DIREITA ========================================================

void Direita(int passos)
{
  delay(1);
  digitalWrite(A4, HIGH);
  delay(1);

  for (int i = 0; i<passos; i++)
  {
    digitalWrite(A5, HIGH);   
    //delay(1);               
    digitalWrite(A5, LOW);    
    //delay(1);               
  }
}

// ================================================================= ESQUERDA ========================================================

void Esquerda(int passos)
{
  delay(1);
  digitalWrite(A4, LOW);
  delay(1);

  for (int i = 0; i<passos; i++)
  {
    digitalWrite(A5, HIGH);   
    delay(1);               
    digitalWrite(A5, LOW);    
    delay(1);               
  }
}


