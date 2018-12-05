#include <SoftwareSerial.h>
#define VS1053_RX  2 

#define VS1053_RESET 9 

//www.vlsi.fi/fileadmin/datasheets/vs1053.pdf 
#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

//http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf
#define VS1053_GM1_ELECTRIC_PIANO_2 120
#define VS1053_GM1_DRAWBAR_ORGAN 17
#define VS1053_GM1_SYNTH_VOICE 55
#define VS1053_GM1_RAIN 122



#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0
#define MIDI_PITCH_WHEEL 0xE0
#define MIDI_EFFECTS_LEVEL 0x5b
#define MIDI_NOTES_OFF 0x7b

SoftwareSerial VS1053_MIDI(0, 2); 
int buttonPin = 20;
volatile int buttonState;
int lightPin = 13;
int t1;
int t2 = 0;
int wait = 1750; 
volatile int val = 0;
boolean first = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(lightPin, OUTPUT);

  VS1053_MIDI.begin(31250);
  
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
  delay(10);

  
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetChannelBank(9, VS1053_BANK_MELODY);
  midiSetChannelBank(10, VS1053_BANK_DRUMS1);
  midiSetInstrument(0, VS1053_GM1_ELECTRIC_PIANO_2);
  midiSetInstrument(1, VS1053_GM1_SYNTH_VOICE);
  midiSetInstrument(2, VS1053_GM1_RAIN);
  midiSetInstrument(5, VS1053_GM1_DRAWBAR_ORGAN);

  
  midiSetChannelVolume(0, 127);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  t1 = millis();
  if(t1 - t2 >= wait) {
    if (val >= 45) {
      val = val - 40;
    } else if(val < 40) {
      val = 20;
    }
    t2 = t1;
  }
  if(buttonState == 1 && val <= 254) {
    val++;
    delay(20);
  }
  if(val == 20) {
    first = true;
    val = 1;
  }
  Serial.write(buttonState);
  analogWrite(lightPin, val);
  if(first) {
    midiNoteOn(5, 47, 115);
    midiNoteOn(5, 50, 115);
    midiNoteOn(5, 52, 115);
    midiNoteOn(5, 54, 115);
    first = false;
    delay(2000);
  }
  pitchBendChange(5, val / 2);
  midiNoteOn(1, 55, 125);
  if(val == 255) {
    midiNoteOn(0, 31, 105);
    delay(50);
    midiNoteOn(0, 33, 105);
    delay(75);
    midiNoteOn(0, 39, 105);
    delay(100);
    midiNoteOn(0, 42, 105);
    delay(125);
  }
}



void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
  inst --; 
  if (inst > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);  
  VS1053_MIDI.write(inst);
}

void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write(MIDI_CHAN_VOLUME);
  VS1053_MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
  VS1053_MIDI.write(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;

  VS1053_MIDI.write(MIDI_NOTE_ON | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void pitchBendChange(byte channel, int value) {
  VS1053_MIDI.write(MIDI_PITCH_WHEEL | channel);
  VS1053_MIDI.write(value);
}



