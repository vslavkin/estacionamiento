#include <Servo.h>
#include <LiquidCrystal.h>

#define btEntrada A1
#define btSalida A2
#define pinServoEntrada 11
#define pinServoSalida 12
#define ledVerde 2
#define ledRojo 3
#define maxSpace 8

Servo servoEntrada;
Servo servoSalida;

LiquidCrystal lcd(8,9,4,5,6,7);

unsigned int contador;

void setup(){
  lcd.begin(16,2);
  servoEntrada.attach(pinServoEntrada);
  servoSalida.attach(pinServoSalida);
  pinMode(btEntrada, INPUT_PULLUP);
  pinMode(btSalida, INPUT_PULLUP);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo , OUTPUT);
  Serial.begin(115200);
}
void loop(){
  Serial.println(contador);
  switch(leerBotones()){
  case 1:
    entrada();
    break;
  case 2:
    salida();
    break;
  }
  setSemaforo(contador<maxSpace);
  imprimircartel(maxSpace-contador,contador);
}

void setSemaforo(bool estado){
  digitalWrite(ledVerde, estado);
  digitalWrite(ledRojo, !estado);
}

int leerBotones(){
  if(digitalRead(btEntrada) == LOW){
    return 1;
  }
  else if (digitalRead(btSalida) == LOW){
    return 2;
  }
}

void entrada(){
  if(contador<maxSpace){
    Serial.println("entrada");
    subirBarrera(servoEntrada);
    contador++;
  }
}

void salida(){
  if(contador>0){
    Serial.println("salida");
    subirBarrera(servoSalida);
    contador--;
  }
}

void subirBarrera(Servo servo){
  servo.write(90);
  delay(2000);
  servo.write(0);
}

void imprimircartel(int libre,int ocupado){
    lcd.setCursor(0,0); //columna y fila de la primera letra de la palabra
    lcd.print("Libre:");
    lcd.print(libre);
    lcd.setCursor(0,1);   // columna y fila
    lcd.print("Ocupado:");
    lcd.print (ocupado);
}
