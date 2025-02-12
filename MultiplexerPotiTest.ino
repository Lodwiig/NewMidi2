#include <MIDIUSB.h>         // USB-MIDI Library (direkt an PC)
#include <Multiplexer4067.h> // 4067 Multiplexer Library

#define N_MUX 1  // Anzahl der Multiplexer

// Multiplexer-Pins
const int mux_s[4] = {4, 5, 6, 7};   // S0-S3 Pins
const int mux_x[N_MUX] = {A2};       // Signal-Pin des Multiplexers

// Multiplexer initialisieren
Multiplexer4067 mux[N_MUX] = { Multiplexer4067(mux_s[0], mux_s[1], mux_s[2], mux_s[3], mux_x[0]) };

int lastPotiValue = 0;  // Speichert den letzten Wert für Vergleich
const int MIDI_CC = 10; // MIDI-CC-Nummer für das Poti

void setup() {
    Serial.begin(115200);  // Debugging aktivieren
    Serial.println("MIDI Controller gestartet!");
}

void loop() {
    int midiChannel = 1; // MIDI-Kanal (1-16)

    // Nur das erste Potentiometer am Multiplexer (Pin 0)
    int value = mux[0].readChannel(0) >> 3;  // Wert auf 7 Bit (0-127) reduzieren

    if (abs(value - lastPotiValue) > 2) {  // Nur senden, wenn sich der Wert geändert hat
        sendMIDI(MIDI_CC, value, midiChannel);
        lastPotiValue = value;
    }

    delay(10);  // Kleine Pause für Stabilität
}

// 🎛 **MIDI-Control-Change senden**
void sendMIDI(byte control, byte value, byte channel) {
    midiEventPacket_t event = {0x0B, 0xB0 | (channel - 1), control, value}; 
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}
