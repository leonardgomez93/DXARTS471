import processing.serial.*;
import processing.video.*;

Serial sp;
Movie myMovie;
PImage p;
PImage p2;
int buttonState;
int val = 1;
int time;
int wait = 750;


void setup() {
  time = millis();
  sp = new Serial(this, Serial.list()[1], 9600);
  myMovie = new Movie(this, "workz.MOV");
  myMovie.loop();
  myMovie.frameRate(2z500);
  fullScreen();
}

void draw() {
    //while (myPort.available() > 0) {
  //}
  if(millis() - time >= wait) {
    if(val > 1) {
      val--;
    }
    time = millis();
  }
  if(buttonState == 1 &&  val <= 14) {
     val++;
  }
  p2 = p;
  image(p2, 0, 0);
  delay(0 + (val * 50));
}


//Called every time a new frame is available to read
void movieEvent(Movie m) {
  m.read();
  p = myMovie;
  p.loadPixels();
  int dimension = (p.width * p.height);
  for (int i=0; i < dimension; i += 1) {
      float r,g,b;
      r = red (p.pixels[i]);
      g = green (p.pixels[i]);
      b = blue (p.pixels[i]);
      r = r - (val * 15);
      g = g - (val * 15);
      b = b - (val * 15);
      r = constrain(r, 0 , 255);
      g = constrain(g, 0 , 255);
      b = constrain(b, 0 , 255);
      color c = color(r,g,b); 
      p.pixels[i] = c;
  }
  p.updatePixels();
  delay(0);
}

void serialEvent(Serial p) {
   buttonState = p.read();
}