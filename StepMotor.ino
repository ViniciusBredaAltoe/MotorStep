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
int Dmax; // Distancia máxima da origem [mm]
int mode; // Modo de operação

//===== Bottons  with Debouce ===========


int fcr_CurrentState = 0;
int fcl_CurrentState = 0;
int br_CurrentState = 0;
int go_CurrentState = 0;
int bl_CurrentState = 0;
int mode_CurrentState = 0;

int fcr_lastState = 0;
int fcl_lastState = 0;
int br_lastState = 0;
int go_lastState = 0;
int bl_lastState = 0;
int mode_lastState = 0; 



void setup() {
  pinMode(fcr, INPUT);
  pinMode(fcl, INPUT);
  pinMode(botton_right, INPUT);
  pinMode(botton_go, INPUT);
  pinMode(botton_left, INPUT);
  pinMode(botton_mode, INPUT);

  stepper.setSpeed(motorspeed);
  int cont = 0;
  while (digitalRead(fcr) == LOW)
  {
    stepper.step(1); // Vai para a direita
  }
  while (digitalRead(fcl) == LOW)
  {
    stepper.step(-1); // Vai para a esquerda
    cont++;
  }
  //Printar Cont
  Dmax = cont * resolution;
  cont = 0;
  posicao = 0;
}

void loop()
{
  fcr_CurrentState = digitalRead(fcr);
  fcl_CurrentState = digitalRead(fcl);
  br_CurrentState = digitalRead(botton_right);
  go_CurrentState = digitalRead(botton_go);
  bl_CurrentState = digitalRead(botton_left);
  mode_CurrentState = digitalRead(botton_mode);

  mode = 0; //Reinicia o modo
  //PRINTAR: Escolha o modo:
  //         L:Manual  R:Auto
  if (bl_CurrentState == LOW && br_CurrentState == HIGH)
  {
    mode = 1;
  }
  if (bl_CurrentState == HIGH && br_CurrentState == LOW)
  {
    mode = 2;
  }
  switch (mode)
  {
    case 0:
      break;


    // ========= AUTOMATIC MODE ===========


    case 1: //Automático
      locus = posicao;
      while (mode_CurrentState == LOW)
      {


        //PRINTAR: Locus: (locus)
        //         Position: (posicao)


        if (go_CurrentState == LOW)
        {
          if (br_CurrentState == HIGH && bl_CurrentState == LOW) // Aumenta em 2mm o destino
          {
            locus = locus + resolution;
          }


          if (bl_CurrentState == HIGH && br_CurrentState == LOW) // Diminui em 2mm o destino
          {
            locus = locus - resolution;
          }
        }

        else // Inicia o movimento para o destido selecionado
        {
          if (locus > posicao) // Vai para a direita
          {
            if (locus > Dmax)
            {
              //PRINT Impossible
              //      Operacion!
              locus = posicao;
            }
            else
            {
              while (digitalRead(fcr) == LOW && posicao != locus)
              {
                posicao = posicao + resolution;
                //PRINTAR: Locus: (locus)
                //         Position: (posicao)
                stepper.step(1); // Vai para a direita

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
              while (digitalRead(fcl) == LOW && posicao != locus)
              {
                posicao = posicao - resolution;
                //PRINTAR: Locus: (locus)
                //         Position: (posicao)
                stepper.step(-1); // Vai para a esquerda
              }
            }
          }
        }
      }
      break;


    // ================  MANUAL MODE =============

    case 2: //Manual
      while (mode_CurrentState == LOW)
      {
        //PRINTAR: Position: (posicao)
        //         Máximo : (Dmax)

        if (br_CurrentState == HIGH && bl_CurrentState == LOW) // Anda uma resolução para a direita
        {
          posicao = posicao + resolution;
          //PRINTAR: Position: (posicao)
          //         Máximo : (Dmax)
          stepper.step(1);
        }


        if (br_CurrentState == HIGH && br_CurrentState == LOW) // Anda uma resolução para a esquerda
        {
          posicao = posicao - resolution;
          //PRINTAR: Position: (posicao)
          //         Máximo : (Dmax)
          stepper.step(-1);
        }
      }
      break;

  }
}
