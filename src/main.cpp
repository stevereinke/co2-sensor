#include <Arduino.h>

// sensor MHZ19B, using PWM
const int pwm_pin = 7;
// range for measurement (0-5000ppm)
const int range = 5000;

int read_co2_pwm() {
  unsigned long th;
  int ppm_pwm = 0;
  float pulse_percent;
  
  do {
    // * th → length of signal in ms, max. 1004ms
    // * timeout of function pulseIn has to be at least 1004000µs, to be sure we use 2500000µs
    th = pulseIn(pwm_pin, HIGH, 2500000) / 1000;
    // length of pulse in %
    float pulse_percent = th / 1004.0;
    // ppm values for given range
    ppm_pwm = range * pulse_percent;
  } while (th == 0);

  return ppm_pwm; 
}

void setup() {
  // define pwm pin as input
  pinMode(pwm_pin, INPUT);
  // define internl le as output
  pinMode(LED_BUILTIN, OUTPUT);
  // init serial connection
  Serial.begin(9600);
}

void loop() {
  int ppm_pwm = read_co2_pwm();
  Serial.print("PPM PWM: ");
  Serial.println(ppm_pwm);

  String quality = "nicht gesetzt";
  if(ppm_pwm > 2000) {
      quality = "rot";
  } else {
    if(ppm_pwm <= 2000 && ppm_pwm >= 1000) {
      quality = "gelb";
    } else {
      quality = "grün";
    }
  }
  Serial.print("Luftqualität: ");
  Serial.println(quality);

  // sleep for 3s
  delay(3000);
}
