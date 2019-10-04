#define pot A0 

int i,j, pins[] = {8,9,10,11}, bottomleft, bottomright, velocidade;
float Ton = 0.5, Toff;

void setup() {
  for(i=0;i<4;i++)
  {
    pinMode(pins[i],OUTPUT); // Configura os pinos como saída
  }
  pinMode(3, INPUT); //Direita
  pinMode(4, INPUT); //Esquerda
  velocidade = 500;
  Toff = 1 - Ton;
}

void loop() {
      
  if(digitalRead(3) == HIGH && digitalRead(4) == LOW){ // Aciona o motor no sentido Horário
    for(i=0;i<4;i++){  // Intercala o as bobinas acionadas
       digitalWrite(pins[i],HIGH); // Envia um pulso de um passo
       delay(velocidade*Ton); 
       digitalWrite(pins[i],LOW);
       delay(velocidade*Toff);
    }
  }
  if(digitalRead(3) == LOW && digitalRead(4) == HIGH){ // Aciona o motor no sentido Anti-Horário
    for(i=4;i>-1;i--){ // Intercala o as bobinas acionadas
       digitalWrite(pins[i],HIGH); // Envia um pulso de um passo
       delay(velocidade*Ton);
       digitalWrite(pins[i],LOW); 
       delay(velocidade*Toff);
    }
  }
}
