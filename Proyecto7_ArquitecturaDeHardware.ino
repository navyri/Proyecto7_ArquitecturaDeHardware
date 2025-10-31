// Proyecto 7: Contador de seguridad
// Sensor LM35, Display 7 segmentos catodo comun y LED

int sensor = A0;
const int pins[7] = {2, 3, 4, 5, 6, 7, 8};
int led = 10;
float temperatura = 0;
int limite = 20;
int umbralCambioBrusco = 3;
bool sistemaListo = false;

// Debounce / temporización
const unsigned long tiempoConfirmacionArranque = 200; // ms, necesita 2 lecturas > limite separadas
const unsigned long tiempoBloqueoPostInterrupcion = 600; // ms, espera antes de permitir nuevo arranque

// Estado del conteo
bool conteoEnCurso = false;
bool conteoInterrumpido = false;
unsigned long tiempoInterrupcion = 0;

// Numeros para display 7 segmentos (catodo comun)
const byte numbersDisplayKathode[10] = {
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

void apagarDisplay() {
  for (int i = 0; i < 7; i++) digitalWrite(pins[i], LOW);
}

void mostrarNumero(int num) {
  if (num < 0 || num > 9) return;
  byte valorNumero = numbersDisplayKathode[num];
  for (int j = 0; j < 7; j++) {
    digitalWrite(pins[j], bitRead(valorNumero, j));
  }
}

float leerTemperatura() {
  // Lectura simple del LM35 en ºC
  return (analogRead(sensor) * 5.0 * 100.0) / 1024.0;
}

bool confirmarArranque() {
  // Confirma que la temperatura esta > limite en dos lecturas separadas por tiempoConfirmacionArranque
  float t1 = leerTemperatura();
  delay(tiempoConfirmacionArranque);
  float t2 = leerTemperatura();
  return (t1 > limite && t2 > limite);
}

void setup() {
  // Configura pines y asegura que no muestre nada al encender
  for (int i = 0; i < 7; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW); // fuerza apagado desde el inicio
  }

  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
  digitalWrite(led, LOW);

  // Espera inicial para evitar que el display muestre residuos
  delay(300);
  apagarDisplay();

  // Lecturas de estabilizacion del sensor
  for (int i = 0; i < 10; i++) {
    analogRead(sensor);
    delay(50);
  }

  sistemaListo = true;
}

void loop() {
  temperatura = leerTemperatura();

  // Si hubo una interrupción reciente por normalizacion, bloquear reinicios por un tiempo corto
  if (conteoInterrumpido) {
    if (millis() - tiempoInterrupcion >= tiempoBloqueoPostInterrupcion) {
      conteoInterrumpido = false; // ya paso el tiempo de bloqueo
    }
  }

  if (sistemaListo && temperatura > limite && !conteoEnCurso && !conteoInterrumpido) {
    // Antes de arrancar el conteo, confirmar que la temperatura se mantiene alta (debounce)
    if (!confirmarArranque()) {
      // No arrancar si la segunda lectura no confirma > limite
      return;
    }

    // Inicia conteo de forma limpia
    conteoEnCurso = true;
    apagarDisplay();
    delay(150); // pequeña pausa limpia antes de mostrar 9

    float tempAnterior = temperatura;
    int i = 9; // inicia en 9

    while (i >= 0) {
      mostrarNumero(i);
      digitalWrite(led, HIGH); // LED enciende con numero visible
      delay(900);

      digitalWrite(led, LOW);  // LED apaga junto al display
      apagarDisplay();
      delay(100);

      float tempActual = leerTemperatura();

      // Si se normaliza o hay cambio brusco, interrumpe
      if (tempActual <= limite || abs(tempActual - tempAnterior) >= umbralCambioBrusco) {
        apagarDisplay();
        digitalWrite(led, LOW);

        // marca la interrupcion y registra el tiempo para bloquear reinicio inmediato
        conteoInterrumpido = true;
        tiempoInterrupcion = millis();

        conteoEnCurso = false; // salimos del conteo
        break;
      }

      tempAnterior = tempActual;
      i--;
    }

    // Si el conteo llega a 0 y sigue alta, activa alarma fija breve
    temperatura = leerTemperatura();
    if (i < 0 && temperatura > limite) {
      digitalWrite(led, HIGH); // LED encendido fijo como alarma
      delay(2000);
      digitalWrite(led, LOW);
      apagarDisplay();
    }

    conteoEnCurso = false; // libera estado por si no lo hizo antes
  } 
  else {
    // Modo normal (vigilancia): asegurar display apagado y LED apagado
    apagarDisplay();
    digitalWrite(led, LOW);
    delay(200); 
  }
}
