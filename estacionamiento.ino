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

void entrada(int *contador){
  if(*contador<maxSpace){
    Serial.println("entrada");
    *contador+=1;
  }
}

void salida(int *contador){
  if(*contador>0){
    Serial.println("salida");
    *contador-=1;
  }
}

void imprimirCartel(int libre,int ocupado, LiquidCrystal lcd){
    lcd.setCursor(0,0); //columna y fila de la primera letra de la palabra
    lcd.print("Libre:");
    lcd.print(libre);
    lcd.setCursor(0,1);   // columna y fila
    lcd.print("Ocupado:");
    lcd.print (ocupado);
}

void setearServos(unsigned long long tiempoServo, Servo servo){
  if(tiempoServo!=0 && millis() < tiempoServo+servoDelayTime){
    servo.write(90);
  }
  else{
    servo.write(0);
  }
}

//Instancias de los objetos de servo
Servo servoEntrada;
Servo servoSalida;
//Instancia del objeto Liquidcrystal(Para el lcd)
LiquidCrystal lcd(8,9,4,5,6,7);
//Contador lleva la cuenta de cuantos autos hay en el estacionamiento
unsigned int contador;
//tiempoServo guarda cuando se toco el boton, para poder usar millis() y no bloquear el programa
unsigned long long tiempoServo[]={0,0};

void setup(){
  //Inicio el lcd con 16 columnas y 2 filas
  lcd.begin(16,2);
  //Inicia los servos en los GPIO correspondientes
  servoEntrada.attach(pinServoEntrada);
  servoSalida.attach(pinServoSalida);
  //Utilizo INPUT_PULLUP para los botones
  pinMode(btEntrada, INPUT_PULLUP);
  pinMode(btSalida, INPUT_PULLUP);
  //Pongo como salidas los GPIOs de los leds
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo , OUTPUT);
  
  Serial.begin(115200);
}

void loop(){
  switch(leerBotones()){
  case 1:
    if(millis() > tiempoServo[0]+servoDelayTime){
      tiempoServo[0]=millis();
      entrada(&contador);
    }
    break;
  case 2:
    if(millis() > tiempoServo[1]+servoDelayTime){
      tiempoServo[1]=millis();
      salida(&contador);
    }
    break;
  }
  //Escribo ambos servos en base a tiemposervo
  setearServos(tiempoServo[0], servoEntrada);
  setearServos(tiempoServo[1], servoSalida);
  //seteo los leds
  setSemaforo(contador<maxSpace);
  //Imprimo el numero de lugares libres y ocupados en el lcd
  imprimirCartel(maxSpace-contador,contador, lcd);
}
