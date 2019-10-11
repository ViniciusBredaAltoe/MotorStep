#include <Stepper.h>

#define STEPS 100
#define resolution 2 //Averiguar!!
#define motorspeed 30 // set the speed of the motor to 30 RPMs


Stepper stepper(STEPS, 8, 9, 10, 11);


const int fcr = 22; // Fim de curso Right
const int fcl = 24; // Fim de curso Left
const int botton_right = 26;
const int botton_go = 28;
const int botton_left = 30;
const int botton_mode = 32;
int locus; // Destino desejado
int posicao; // Posição atual
int cont; // Contagem de passos
int Max; // Distancia máxima da origem [mm]
int mode;



void setup() {
  pinMode(fcr, INPUT);
  pinMode(fcl, INPUT);
  pinMode(botton_right, INPUT);
  pinMode(botton_go, INPUT);
  pinMode(botton_left, INPUT);
  pinMode(botton_mode, INPUT);

  stepper.setSpeed(motorspeed);
  int cont = 0;

  Serial.begin(9600); // TEST PROTOBOARD
}



int Initialization(int Dmax)
{
  while (digitalRead(fcr) == LOW)
  {
    //stepper.step(1); // Vai para a direita
    Serial.print("Direita \n"); // TEST PROTOBOARD
    delay(2000); // TEST PROTOBOARD

  }
  while (digitalRead(fcl) == LOW)
  {
    //stepper.step(-1); // Vai para a esquerda
    cont++;
    Serial.print("Esquerda  \n"); // TEST PROTOBOARD
    delay(2000); // TEST PROTOBOARD
  }
  //Printar Cont
  Dmax = cont * resolution;
  return Dmax;
}

void loop()
{
  if (cont == 0)
  {
    Initialization(Max);
    cont = 1;
  }
  mode = 0; //Reinicia o modo
  //PRINTAR: Escolha o modo:
  //         L:Manual  R:Auto
  if (digitalRead(botton_right) == HIGH and digitalRead(botton_left) == LOW)
  {
    mode = 1;
    Serial.print("AutoDireita_  \n"); // TEST PROTOBOARD
    delay(2000); // TEST PROTOBOARD
  }
  if (digitalRead(botton_left) == HIGH and digitalRead(botton_right) == LOW)
  {
    mode = 2;
    Serial.print("ManualEsquerda_  \n"); // TEST PROTOBOARD
    delay(2000); // TEST PROTOBOARD
  }
  switch (mode)
  {
    case 0:
      break;


    // ========= AUTOMATIC MODE ===========


    case 1: //Automático
      locus = posicao;
      while (digitalRead(botton_mode) == LOW)
      {


        //PRINTAR: Locus: (locus)
        //         Position: (posicao)


        if (digitalRead(botton_go) == LOW)
        {
          if (digitalRead(botton_right) == HIGH and digitalRead(botton_left) == LOW) // Aumenta em 2mm o destino
          {
            locus = locus + resolution;
            Serial.print("DireitaA  \n"); // TEST PROTOBOARD
            delay(2000); // TEST PROTOBOARD

          }


          if (digitalRead(botton_left) == HIGH and digitalRead(botton_right) == LOW) // Diminui em 2mm o destino
          {
            locus = locus - resolution;
            Serial.print("EsquerdaA  \n"); // TEST PROTOBOARD
            delay(2000); // TEST PROTOBOARD
          }
        }

        else // Inicia o movimento para o destido selecionado
        {
          if (locus > posicao) // Vai para a direita
          {
            if (locus > Max)
            {
              //PRINT Impossible
              //      Operacion!
              locus = posicao;
            }
            else
            {
              while (digitalRead(fcr) == LOW and posicao != locus)
              {
                posicao = posicao + resolution;
                //PRINTAR: Locus: (locus)
                //         Position: (posicao)
                //stepper.step(1); // Vai para a direita
                Serial.print("Direita  \n"); // TEST PROTOBOARD
                delay(2000); // TEST PROTOBOARD

              }
            }
          }
          else // Vai para a esquerda
          {
            if (locus < 0)
            {
              //PRINT Impossible
              //      Operacion!
              locus = posicao;
            }
            else
            {
              while (digitalRead(fcl) == LOW and posicao != locus)
              {
                posicao = posicao - resolution;
                //PRINTAR: Locus: (locus)
                //         Position: (posicao)
                //stepper.step(-1); // Vai para a esquerda
                Serial.print("Esquerda  \n"); // TEST PROTOBOARD
                delay(2000); // TEST PROTOBOARD
              }
            }
          }
        }
      }
      break;


    // ================  MANUAL MODE =============

    case 2: //Manual
      while (digitalRead(botton_mode) == LOW)
      {
        //PRINTAR: Position: (posicao)
        //         Máximo : (Max)

        if (digitalRead(botton_right) == HIGH and digitalRead(botton_left) == LOW) // Anda uma resolução para a direita
        {
          posicao = posicao + resolution;
          //PRINTAR: Position: (posicao)
          //         Máximo : (Max)
          //stepper.step(1);
          Serial.print("Direita  \n"); // TEST PROTOBOARD
          delay(2000); // TEST PROTOBOARD
        }


        if (digitalRead(botton_left) == HIGH and digitalRead(botton_right) == LOW) // Anda uma resolução para a esquerda
        {
          posicao = posicao - resolution;
          //PRINTAR: Position: (posicao)
          //         Máximo : (Max)
          //stepper.step(-1);
          Serial.print("Esquerda  \n"); // TEST PROTOBOARD
          delay(2000); // TEST PROTOBOARD
        }
      }
      break;

  }
}
