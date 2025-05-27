#include <Arduino.h>
#include <esp_task_wdt.h>

#include "buzzer_notification.h"

const int BUZZER_PIN = 27; 

void setupBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void buzzerStart() {
    for (int x = 0; x < 2; x++) {
        digitalWrite(BUZZER_PIN, HIGH); 
        delay(200);
        digitalWrite(BUZZER_PIN, LOW); 
        delay(50);
        Serial.println("lah ini mana");
    }
}

void buzzerInference(bool status) {
    if (!status) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(300);
        digitalWrite(BUZZER_PIN, LOW); 
        Serial.println("normal");
    } else {
        esp_task_wdt_reset();
        digitalWrite(BUZZER_PIN, HIGH); 
        delay(10000); 
        digitalWrite(BUZZER_PIN, LOW); 
        Serial.println("fatigue");
    }
    
}
