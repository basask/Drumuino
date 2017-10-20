#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

#define LOOP_DELAY 1
#define ANALOG_CHANELS 4
#define THRESHOLD 100
#define MAX_VELOCITY 120

uint16_t peak_total[ANALOG_CHANELS];
uint8_t peak_count[ANALOG_CHANELS];
uint8_t notes[ANALOG_CHANELS];

void setup() {
    // Serial.begin(115200);

    memset(peak_total, 0, sizeof(peak_total));
    memset(peak_count, 0, sizeof(peak_count));
    memset(notes, 0, sizeof(notes));

    notes[0] = 36; // Bass Drum
    notes[1] = 40; // Snare
    notes[2] = 41; // Floor tom
    notes[3] = 46; // Open Hi-Hat
    // notes[4] = 45; // Low tom
    // notes[5] = 49; // Crash 1
}

uint8_t getVelocity(uint16_t value) {
    // rounding 1023(analogRead) int to an 127 (MIDIUSB) int velocity
    uint8_t v = value / 8;
    return v > MAX_VELOCITY ? MAX_VELOCITY : v;
}

void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    digitalWrite(LEDPIN, HIGH);
    midiEventPacket_t event = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

uint16_t evalPeak(uint8_t index, uint16_t value){
    if (value >= THRESHOLD) {
        peak_total[index] += value;
        peak_count[index] += 1;
        return 0;
    }
    if (peak_count[index] > 0) {
        uint16_t ret = (uint16_t)(peak_total[index] / peak_count[index]);
        peak_total[index] = 0;
        peak_count[index] = 0;
        return ret;
    }
    return 0;
}

void readChannels(){
    for (uint8_t i = 0; i < ANALOG_CHANELS; i++) {
        uint16_t peakVelocity = evalPeak(i, analogRead(i));
        if (peakVelocity > 0){
            noteOn(i, notes[i], getVelocity(peakVelocity));
        }
    }
}

void loop(){
    readChannels();
    delay(LOOP_DELAY);
}
