#include <Adafruit_NeoPixel.h>

// Definirea pinului pentru banda LED
#define LED_PIN 6  // Pinul unde este conectată banda LED (DAT)

// Numărul total de LED-uri pe bandă
#define NUM_LEDS 60  // Modifică numărul în funcție de banda ta

// Crearea obiectului Adafruit_NeoPixel
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variabilă care ține evidența poziției curente de oprire
int lastStoppedPosition = NUM_LEDS; // Inițial nu există LED-uri aprinse

// Funcție pentru a seta culoarea unui singur LED
void setPixelColor(int pixel, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(pixel, strip.Color(r, g, b));
}

// Funcție care mută un LED până la poziția corectă și îl lasă aprins
void movePixelAndStop(uint8_t r, uint8_t g, uint8_t b, int delayTime) {
  int stopPosition = NUM_LEDS - 1; // Default: capătul benzii
  if (lastStoppedPosition < NUM_LEDS) {
    stopPosition = lastStoppedPosition - 1; // Se oprește înaintea ultimului LED aprins
  }

  // Mutăm LED-ul curent până la poziția de oprire
  for (int i = 0; i <= stopPosition; i++) {
    // Stingem toate LED-urile care nu sunt finale
    strip.clear();

    // Aprindem toate LED-urile care au rămas aprinse
    for (int j = stopPosition + 1; j < NUM_LEDS; j++) {
      setPixelColor(j, r, g, b);
    }

    // Aprindem LED-ul curent în mișcare
    setPixelColor(i, r, g, b);
    strip.show();
    delay(delayTime);

    // Dacă LED-ul a ajuns la poziția de oprire, salvăm poziția
    if (i == stopPosition) {
      lastStoppedPosition = stopPosition; // Salvăm noua poziție finală
      break;
    }
  }
}

// Funcție care stinge LED-urile progresiv de la început spre capăt
void movePixelAndTurnOff(uint8_t r, uint8_t g, uint8_t b, int delayTime) {
  for (int i = 0; i < NUM_LEDS; i++) {
    // Stinge LED-ul curent pe poziția sa
    setPixelColor(i, 0, 0, 0);
    strip.show();
    delay(delayTime);

    // Parcurge pozițiile anterioare, aprinzându-le temporar
    for (int j = i - 1; j >= 0; j--) {
      setPixelColor(j, r, g, b); // Aprinde LED-ul anterior
      strip.show();
      delay(delayTime);

      setPixelColor(j, 0, 0, 0); // Stinge LED-ul anterior
      strip.show();
      delay(delayTime);
    }
  }
}

void setup() {
  strip.begin();         // Inițializează banda LED
  strip.show();          // Oprește toate LED-urile inițial
  strip.setBrightness(50); // Ajustează luminozitatea (0-255)
}

void loop() {
  // Culoare roșie (255, 0, 0)
  uint8_t r = 255, g = 0, b = 0;
  int delayTime = 10;  // Timpul de întârziere între pași (în milisecunde)

  // Secvență de aprindere
  movePixelAndStop(r, g, b, delayTime);

  // Dacă toate LED-urile sunt aprinse, așteaptă și apoi stinge-le
  if (lastStoppedPosition == 0) {
    delay(3000); // Menține toate LED-urile aprinse timp de 3 secunde
    movePixelAndTurnOff(r, g, b, delayTime); // Stinge LED-urile progresiv
    lastStoppedPosition = NUM_LEDS; // Resetăm poziția pentru un nou ciclu
  }
}
