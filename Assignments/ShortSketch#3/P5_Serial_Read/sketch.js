/*
P5.JS SERIAL READ — PNG SCALE VERSION
- JAY J GURUNG
- 10-04-2025

- Reads 8-bit value (0–255) from serial (inData).
- Scales a centered PNG from 5% to 100% of its original size based on inData.
- Background gets darker as image gets "closer" (higher inData).

- CODE REFERENCED FROM EXAMPLE S07 SKETCH.JS

Closely follows the structure of the provided reference sketch.
*/

let serial; // instance of p5.SerialPort
let portName = '/dev/tty.usbserial-120'; // <-- change to your serial port
let options  = { baudRate: 9600 };          // <-- match your Arduino baud (115200 if you used that)

let inData = 0; // incoming serial data (0–255)
let img;        // the PNG image
const IMG_PATH = 'assets/myImage.png'; // <-- put your image here as instructed

function preload() {
  // Load image before setup/draw
  img = loadImage(IMG_PATH);
}

function setup() {
  // P5 Serial Setup
  serial = new p5.SerialPort();
  serial.on('list', printList);
  serial.on('connected', serverConnected);
  serial.on('open', portOpen);
  serial.on('data', serialEvent);
  serial.on('error', serialError);
  serial.on('close', portClose);

  serial.list();
  serial.open(portName, options);

  // Typical p5.js setup
  createCanvas(1920, 1080); // required size
  imageMode(CENTER);        // draw image centered around its x,y
  textSize(28);
  textAlign(CENTER, CENTER);
  noStroke();
}

function draw() {
  // Map inData so that higher values (closer) produce darker background.
  // inData: 0 (far) -> bright background (255)
  // inData: 255 (close) -> dark background (20)
  const bg = int(map(inData || 0, 0, 255, 255, 20));
  background(bg, 64);

  // Scale factor from 5% to 100% of original image size
  const scaleFactor = map(inData || 0, 0, 255, 0.05, 1.0);

  // Draw the image centered at canvas middle
  const drawW = img.width  * scaleFactor;
  const drawH = img.height * scaleFactor;
  image(img, width / 2, height / 2, drawW, drawH);

  // Optional on-screen readout
  fill(255 - bg); // auto-contrast text
  text(`inData: ${inData}`, width / 2, height - 40);
}

/* ====== Serial Event Handlers (same flavor as reference) ====== */

function printList(portList) {
  print('List of Available Serial Ports: ');
  for (let i = 0; i < portList.length; i++) {
    print(i + ' ' + portList[i]);
  }
}

function serverConnected() {
  print('CONNECTED TO SERVER');
}

function portOpen() {
  print('SERIAL PORT OPEN');
}

function serialEvent() {
  // Store incoming data as a number (expecting a single byte 0–255)
  // If your Arduino sends lines, you could read a string and parseInt it.
  // This direct Number(read()) mirrors your reference.
  const incoming = serial.read();
  if (incoming !== -1 && incoming !== undefined) {
    inData = Number(incoming);
  }
}

function serialError(err) {
  print('ERROR: ' + err);
}

function portClose() {
  print('*____SERIAL PORT CLOSED');
}