#include <Servo.h>
#include <LiquidCrystal.h>

#define btEntrada A1
#define btSalida A2
#define pinServoEntrada 11
#define pinServoSalida 12
#define ledVerde 2
#define ledRojo 3
#define maxSpace 8
#define servoDelayTime 2000

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
  return 0;
}

void entrada(int *contador, Servo servoEntrada){
  if(*contador<maxSpace){
    Serial.println("entrada");
    *contador+=1;
    /* subirBarrera(servoEntrada); */
  }
}

void salida(int *contador, Servo servoSalida){
  if(*contador>0){
    Serial.println("salida");
    /* subirBarrera(servoSalida); */
    *contador-=1;
  }
}

void subirBarrera(Servo servo){
  servo.write(90);
  delay(2000);
  servo.write(0);
}

void imprimirCartel(int libre,int ocupado, LiquidCrystal lcd){
    lcd.setCursor(0,0); //columna y fila de la primera letra de la palabra
    lcd.print("Libre:");
    lcd.print(libre);
    lcd.setCursor(0,1);   // columna y fila
    lcd.print("Ocupado:");
    lcd.print (ocupado);
}

Servo servoEntrada;
Servo servoSalida;

LiquidCrystal lcd(8,9,4,5,6,7);

unsigned int contador;
unsigned long long tiempoServo[]={0,0};

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
  switch(leerBotones()){
  case 1:
    if(millis() > tiempoServo[0]+servoDelayTime){
      tiempoServo[0]=millis();
      entrada(&contador, servoEntrada);
    }
    break;
  case 2:
    if(millis() > tiempoServo[1]+servoDelayTime){
      tiempoServo[1]=millis();
      salida(&contador, servoSalida);
    }
    break;
  }
  setearServos(tiempoServo[0], servoEntrada);
  setearServos(tiempoServo[1], servoSalida);
  setSemaforo(contador<maxSpace);
  imprimirCartel(maxSpace-contador,contador, lcd);
}

void setearServos(unsigned long long tiempoServo, Servo servo){
  if(tiempoServo!=0 && millis() < tiempoServo+servoDelayTime){
    servo.write(90);
  }
  else{
    servo.write(0);
  }
}

