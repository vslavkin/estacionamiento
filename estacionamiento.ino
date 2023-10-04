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

int leerBotones(){
  if(digitalRead(btEntrada) == LOW){
    return 1;
  }
  else if (digitalRead(btSalida) == LOW){
    return 2;
  }
  return 0;
}

bool servoAbierto(unsigned long long marcaTiempoServo){
  if(marcaTiempoServo==0){
    return false;
  }
  return millis() < marcaTiempoServo+servoDelayTime;
}

void actualizarServos(unsigned long long marcaTiempoServo, Servo servo){
  if(servoAbierto(marcaTiempoServo)){
    servo.write(90);
  }
  else{
    servo.write(0);
  }
}
void administrarComandos(unsigned long long* marcaTiempoServo, unsigned int* contador){
  switch(leerBotones()){
  case 1:
    Serial.print("tiempo");
    Serial.println((long)marcaTiempoServo[0]);
    Serial.print("abierto");
    Serial.println(millis()<marcaTiempoServo+servoDelayTime);
    if(!servoAbierto(marcaTiempoServo[0])){
      if(!sumaSaturada(*contador, 1, 0, maxSpace, contador)){
	marcaTiempoServo[0]=millis();
      }
    }
    break;
  case 2:
    if(!servoAbierto(marcaTiempoServo[1])){
      if(!sumaSaturada(*contador, -1, 0, maxSpace, contador)){
	marcaTiempoServo[1]=millis();
      }
    }
    break;
  default:
    break;
  }
}

bool sumaSaturada(int a, int b, int min, int max, int* result){
  int sum=a+b;
  if(sum < min){
    *result=min;
    return true;
  }
  else if(sum > max){
    *result=max;
    return true;
  }
  else{
    *result=sum;
    return false;
  }
}



void setSemaforo(bool estado){
  digitalWrite(ledVerde, estado);
  digitalWrite(ledRojo, !estado);
}

void imprimirCartel(int libre,int ocupado, LiquidCrystal lcd){
    lcd.setCursor(0,0); //columna y fila de la primera letra de la palabra
    lcd.print("Libre:");
    lcd.print(libre);
    lcd.setCursor(0,1);   // columna y fila
    lcd.print("Ocupado:");
    lcd.print (ocupado);
}

//Instancias de los objetos de servo
Servo servoEntrada;
Servo servoSalida;
//Instancia del objeto Liquidcrystal(Para el lcd)
LiquidCrystal lcd(8,9,4,5,6,7);
//Contador lleva la cuenta de cuantos autos hay en el estacionamiento
unsigned int contador;
//marcaTiempoServo guarda cuando se toco el boton, para poder usar millis() y no bloquear el programa
unsigned long long marcaTiempoServo[]={0,0};

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
  administrarComandos(marcaTiempoServo, &contador);
  //Escribo ambos servos en base a tiemposervo
  actualizarServos(marcaTiempoServo[0], servoEntrada);
  actualizarServos(marcaTiempoServo[1], servoSalida);
  //seteo los leds
  setSemaforo(contador<maxSpace);
  //Imprimo el numero de lugares libres y ocupados en el lcd
  imprimirCartel(maxSpace-contador,contador, lcd);
}
