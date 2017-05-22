void midi() {
  usbMIDI.read(); //USB MIDI receive
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note On, ch=");
  Serial.print(note, DEC);
  Serial.println();
  midiTrig(note);
}


//void OnControlChange(byte channel, byte control, byte value) {
//  Serial.print("Slider value=");
//  Serial.print(value, DEC);
//  Serial.println();
//  slideControl(value)
//}

void midiTrig(int in) {
  if (in == 40) {
    changeToRed();
  }
  else if (in == 41) {
    changeToGreen();
  }
  else if (in == 42) {
    changeToBlue();
  }
}


//void OnNoteOff(byte channel, byte note, byte velocity) {
//  Serial.print("Note Off, ch=");
//  Serial.print(channel, DEC);
//  Serial.print(", note=");
//  Serial.print(note, DEC);
//  Serial.print(", velocity=");
//  Serial.print(velocity, DEC);
//  Serial.println();
//}



