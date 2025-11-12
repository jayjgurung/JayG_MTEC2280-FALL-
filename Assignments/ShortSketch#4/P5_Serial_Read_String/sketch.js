/*
Jay J Gurung
10-04-2025

CSV per line: distance(0..100), button1, button2, pot(0..1023)
Canvas: 1920x1080
- myImage.png (center) scales 5%..100% by pot; background darker as pot→max
- myImage2.png center-x, bottom-100; distance controls y (>=90 stays bottom),
  lower distance → moves toward top; buttons move left/right by 10px WHILE HELD
-Sketch referenced from S08 Serial MultiSensor example
*/

let serial;
let portName = '/dev/tty.usbserial-120';   // <-- set your port
let options  = { baudRate: 9600 };          // <-- match Arduino baud

// [dist, btn1, btn2, pot]
let sensors = [0, 0, 0, 0];

let img1, img2;
const IMG1_PATH = 'assets/myImage.png';
const IMG2_PATH = 'assets/myImage2.png';
let img2Loaded = false;

// myImage2 position state
let img2x;                 
let img2y;                 
const bottomOffset = 100;  
const stepPx = 10;         // per-frame move while held

function preload() {
  img1 = loadImage(IMG1_PATH,
    () => console.log('myImage.png loaded:', img1.width, img1.height),
    (e) => console.error('Failed to load', IMG1_PATH, e)
  );
  img2 = loadImage(IMG2_PATH,
    () => { img2Loaded = true; console.log('myImage2.png loaded:', img2.width, img2.height); },
    (e) => console.error('Failed to load', IMG2_PATH, e)
  );
}

function setup() {
  serial = new p5.SerialPort();
  serial.on('list', printList);
  serial.on('connected', serverConnected);
  serial.on('open', portOpen);
  serial.on('data', serialEvent);
  serial.on('error', serialError);
  serial.on('close', portClose);

  serial.list();
  serial.open(portName, options);

  createCanvas(1920, 1080);
  imageMode(CENTER);
  textAlign(CENTER, CENTER);
  textSize(28);
  noStroke();

  img2x = width / 2;
  img2y = height - bottomOffset;
}

function draw() {
  const dist = constrain(Number(sensors[0]) || 0, 0, 100);
  const b1   = (Number(sensors[1]) || 0) ? 1 : 0;
  const b2   = (Number(sensors[2]) || 0) ? 1 : 0;
  const pot  = constrain(Number(sensors[3]) || 0, 0, 1023);

  // Background darker as pot increases (closer)
  const bg = int(map(pot, 0, 1023, 255, 20));
  background(bg);

  // myImage.png (center): scale 5%..100% by pot
  if (img1 && img1.width > 0) {
    const scaleFactor = map(pot, 0, 1023, 0.05, 1.0);
    image(img1, width/2, height/2, img1.width*scaleFactor, img1.height*scaleFactor);
  } else {
    fill(255 - bg); text('Loading assets/myImage.png…', width/2, height/2);
  }

  // Buttons: move while held (continuous)
  if (b1) img2x -= stepPx;   // hold button1 → move left
  if (b2) img2x += stepPx;   // hold button2 → move right

  // Constrain x after moving
  if (img2Loaded) img2x = constrain(img2x, img2.width/2, width - img2.width/2);
  else            img2x = constrain(img2x, 0, width);

  // Distance → Y: dist >= 90 => stay bottom; else move toward top
  const bottomY = height - bottomOffset;
  img2y = (dist >= 90) ? 60 : map(dist, 90, 0, 60, bottomY);

  // Draw myImage2 or debug marker
  if (img2Loaded) {
    tint(255);
    image(img2, img2x, img2y, img2.width * 0.3, img2.height * 0.3);
  } else {
    drawDebugCrosshair(img2x, img2y, 60, 255 - bg);
    fill(255 - bg); text('Loading assets/myImage2.png…', width/2, bottomY - 40);
  }

  // Debug text
  fill(255 - bg);
  text(`dist:${dist}  btn1:${b1}  btn2:${b2}  pot:${pot}`, width/2, height - 32);

  noStroke();

  // Primitive #1: ground rect near bottom
  fill(255 - bg);                  // visible against your background
  rectMode(CENTER);
  rect(width/2, height - 60, width * 0.6, 12, 2);

  // Primitive #2: center ellipse whose size follows pot
  const hudSize = map(pot, 0, 1023, 20, 160);
  fill(255 - bg, 27);
  ellipse(width/2, height/2, hudSize*5, hudSize*5);

  // Primitive #3: distance meter line across bottom
  stroke(255 - bg);
  strokeWeight(4);
  const meterX = map(constrain(dist, 0, 100), 0, 100, 80, width - 80);
  line(80, height - 20, meterX, height - 20);
  noStroke();
}

/* ---------- Serial handlers ---------- */

function printList(portList) {
  print('List of Available Serial Ports: ');
  for (let i = 0; i < portList.length; i++) print(i + ' ' + portList[i]);
}
function serverConnected() { print('CONNECTED TO SERVER'); }
function portOpen()        { print('SERIAL PORT OPEN'); }

function serialEvent() {
  const inString = serial.readStringUntil('\n');
  if (!inString) return;
  const line = inString.trim();
  if (!line) return;

  const parts = line.split(',');
  if (parts.length < 4) return;

  const d  = Number(parts[0]);
  const b1 = Number(parts[1]);
  const b2 = Number(parts[2]);
  const p  = Number(parts[3]);

  if (Number.isFinite(d))  sensors[0] = d;
  if (Number.isFinite(b1)) sensors[1] = b1;
  if (Number.isFinite(b2)) sensors[2] = b2;
  if (Number.isFinite(p))  sensors[3] = p;
}

function serialError(err) { print('ERROR: ' + err); }
function portClose()      { print('*____SERIAL PORT CLOSED'); }

/* ---------- Helpers ---------- */
function drawDebugCrosshair(x, y, size, col=255) {
  push();
  stroke(col); strokeWeight(2);
  line(x - size, y, x + size, y);
  line(x, y - size, x, y + size);
  pop();
}