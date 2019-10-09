#include <Stepper.h>

#define STEPS 100
#define resolution 2 //Averiguar!!



Stepper stepper(STEPS, 8, 9, 10, 11);


const int fcr = 22; // Fim de curso Right
const int fcl = 24; // Fim de curso Left
const int botton_right = 26;
const int botton_go = 28;
const int botton_left = 30;
const int botton_mode = 32;
int mode;
int locus;
int posicao;
int cont;
int Dmax;

void setup() {
  pinMode(botton_right, INPUT);
  pinMode(botton_go, INPUT);
  pinMode(botton_left, INPUT);
  pinMode(botton_mode, INPUT);

  stepper.setSpeed(30); // set the speed of the motor to 30 RPMs
  int cont = 0;
  while (digitalRead(fcr) == LOW)
  {
    stepper.step(1);
  }
  while (digitalRead(fcl) == LOW)
  {
    stepper.step(-1);
    cont++;
  }
  //Printar Cont
  Dmax = cont * resolution;
  cont = 0;
  posicao = 0;
}

void loop()
{
  mode = 0; //Reinicia o modo
  //PRINTAR: Escolha o modo:
  //         L:Manual  R:Auto
  if (digitalRead(botton_right == HIGH) && digitalRead(botton_right == LOW))
  {
    mode = 1;
  }
  if (digitalRead(botton_left == HIGH) && digitalRead(botton_right == LOW))
  {
    mode = 2;
  }
  switch (mode)
  {
    case 0:
      break;

    case 1: //Automático
      locus = posicao;
      while (digitalRead(botton_mode == LOW))
      {
        //PRINTAR: Locus: (locus)
        //         Position: (posicao)
        if (digitalRead(botton_right == HIGH) && digitalRead(botton_left == LOW) && digitalRead(botton_go) == LOW) // Aumenta em 2mm o destino
        {
          locus = locus + resolution;
        }
        if (digitalRead(botton_left == HIGH) && digitalRead(botton_right == LOW) && digitalRead(botton_go) == LOW) // Diminui em 2mm o destino
        {
          locus = locus - resolution;
        }
        if (digitalRead(botton_go == HIGH) && digitalRead(botton_left == LOW) && digitalRead(botton_right) == LOW) // Inicia o movimento para o destido selecionado
        {
          if (locus > posicao) // Vai para a direita
          {
            if (locus > Dmax)
            {
              //PRINT Impossible
              //      Operacion!
            }
          }
          if (locus < posicao) // Vai para a esquerda
          {
            if (locus < 0)
            {
              //PRINT Impossible
              //      Operacion!
            }
          }
        }
      }
      break;

    case 2: //Manual
      while (digitalRead(botton_mode == LOW))
      {

      }
      break;

  }
}
