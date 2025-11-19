/*
  20x20 Pot-Controlled Pixel Drawer with Serial Handshaking

  - Two pots select a square in a 20x20 grid:
      Pot1 -> horizontal index (0..19)
      Pot2 -> vertical   index (0..19)
  - Button 1 (on Arduino) fills selected square with random color
  - Button 2 clears all squares
  - Bottom UI:
      [FILL]   (left)  shows button1 state
      (circle) center Pause/Resume (sends 'A'/'B')
      [RESET]  (right) shows button2 state
  - LED1 on Arduino = drawing mode (A)
  - LED2 on Arduino = paused (B)
*/

// 20x20 Pot-Controlled Pixel Drawer with Serial Handshaking

// ---- Serial stuff ----
let serial; 
let portName = '/dev/tty.usbserial-130'; // Windows example; on Mac something like '/dev/tty.usbmodem1101'

// incoming data from Arduino
let pot1 = 0;     // horizontal
let pot2 = 0;     // vertical
let button1 = 0;  // fill
let button2 = 0;  // reset

// previous states for edge detection
let prevButton1 = 0;
let prevButton2 = 0;

// ---- Grid setup ----
const COLS = 20;
const ROWS = 20;
let grid = [];     // grid[row][col] holds p5.Color or null
let cellW, cellH;
let selCol = 0;    // current selected column (0..19)
let selRow = 0;    // current selected row   (0..19)

// ---- Mode (A/B) ----
let isDrawing = false; // false = paused (send 'B'), true = active (send 'A')

function setup() {
  createCanvas(600, 600);
  textAlign(CENTER, CENTER);
  rectMode(CORNER);

  // leave ~100px at bottom for UI
  cellW = width / COLS;
  cellH = (height - 100) / ROWS;

  // initialize grid
  for (let r = 0; r < ROWS; r++) {
    grid[r] = [];
    for (let c = 0; c < COLS; c++) {
      grid[r][c] = null;
    }
  }

  // ---- Serial setup ----
  serial = new p5.SerialPort();
  serial.list();             // optional: prints available ports in console
  serial.open(portName);     // IMPORTANT: method is .open(), not .openPort()

  serial.on('data', serialEvent);
  serial.on('error', serialError);
}

function draw() {
  background(255);

  // ---- Handshaking: send 'A' or 'B' each frame ----
  if (serial) { // make sure serial is ready
    if (isDrawing) {
      serial.write('A'); // ask Arduino to send pot + button data
    } else {
      serial.write('B'); // tell Arduino we're paused
    }
  }

  // ---- Map pot values to grid indices ----
  let maxAdc = 4095; // Analog-to-Digital Converter max value which is 12-bit on ESP32
  selCol = constrain(floor(map(pot1, 0, maxAdc + 0.01, 0, COLS)), 0, COLS - 1); // +0.01 to avoid edge case
  selRow = constrain(floor(map(pot2, 0, maxAdc + 0.01, 0, ROWS)), 0, ROWS - 1);

  // ---- Button behavior (edge detection) ----
  // Button1 pressed → fill selected cell (only in active/drawing mode)
  if (button1 === 1 && prevButton1 === 0 && isDrawing) {
    grid[selRow][selCol] = color(random(255), random(255), random(255));
  }

  // Button2 pressed → clear grid (in either mode)
  if (button2 === 1 && prevButton2 === 0) {
    clearGrid();
  }

  prevButton1 = button1;
  prevButton2 = button2;

  // ---- Draw the pixel grid ----
  noStroke();
  for (let r = 0; r < ROWS; r++) {
    for (let c = 0; c < COLS; c++) {
      let x = c * cellW;
      let y = r * cellH;
      let col = grid[r][c];

      if (col !== null) {
        fill(col);
        rect(x, y, cellW, cellH);
      }
      // if null: leave it unfilled (black background shows through)
    }
  }

  // highlight currently selected cell with border
  stroke(0);
  strokeWeight(2);
  noFill();
  rect(selCol * cellW, selRow * cellH, cellW, cellH);// the parameters are x, y, width, height. selCol x cellW because selCol is the index of the column and cellW is the width of each cell.

  // ---- Draw bottom UI ----
  drawUI();
}

function drawUI() {
  let uiY = height - 70;
  let btnW = 130;
  let btnH = 45;
  let gap = 60;
  let centerX = width / 2;

  // FILLS button rectangle (left)
  let fillX = centerX - btnW - gap; // this positions the fill button to the left of center
  stroke(200);
  strokeWeight(button1 ? 4 : 1); // thicker when physical button pressed
  fill(button1 ? 80 : 40);
  rect(fillX, uiY, btnW, btnH, 8);
  noStroke();
  fill(255);
  text("FILL", fillX + btnW / 2, uiY + btnH / 2);

  // PAUSE/RESUME circle (center)
  let circleR = 85;
  let circleY = uiY + btnH / 2;
  stroke(200);
  strokeWeight(2);
  fill(isDrawing ? 80 : 40);
  ellipse(centerX, circleY, circleR, circleR);
  noStroke();
  fill(255);
  text(isDrawing ? "PAUSE RX" : "RESUME RX", centerX, circleY);

  // RESET button rectangle (right)
  let resetX = centerX + gap;
  stroke(200);
  strokeWeight(button2 ? 4 : 1); // thicker when reset button pressed
  fill(button2 ? 80 : 40);
  rect(resetX, uiY, btnW, btnH, 8);
  noStroke();
  fill(255);
  text("RESET", resetX + btnW / 2, uiY + btnH / 2);
}

// Mouse controls for UI (helpful for debugging )
function mousePressed() {
  let uiY = height - 70;
  let btnW = 130;
  let btnH = 45;
  let gap = 20;
  let centerX = width / 2;
  let circleR = 60;
  let circleY = uiY + btnH / 2;

  // click center circle -> toggle draw/pause mode
  let d = dist(mouseX, mouseY, centerX, circleY);
  if (d < circleR / 2) {
    isDrawing = !isDrawing;
  }

  // allow clicking rectangles to trigger FILL/RESET while testing
  let fillX = centerX - btnW - gap;
  let resetX = centerX + gap;

  // click FILL button
  if (mouseX > fillX && mouseX < fillX + btnW &&
      mouseY > uiY && mouseY < uiY + btnH) {
    grid[selRow][selCol] = color(random(255), random(255), random(255));
  }

  // click RESET button
  if (mouseX > resetX && mouseX < resetX + btnW &&
      mouseY > uiY && mouseY < uiY + btnH) {
    clearGrid();
  }
}

// Clear all cells
function clearGrid() {
  for (let r = 0; r < ROWS; r++) {
    for (let c = 0; c < COLS; c++) {
      grid[r][c] = null;
    }
  }
}

// ---- Serial callbacks ----
function serialEvent() {
  let inString = serial.readStringUntil('\n');
  if (!inString) return;

  inString = inString.trim();
  if (inString.length === 0) return;

  // In paused mode Arduino might send "PAUSED" (no commas) -> ignore
  if (inString.indexOf(',') === -1) {
    // print(inString); // uncomment to see status messages
    return;
  }

  let parts = inString.split(',');
  if (parts.length >= 4) {
    button1 = int(parts[0]);
    button2 = int(parts[1]);
    pot1    = int(parts[2]);
    pot2    = int(parts[3]);
  }
}

function serialError(err) {
  console.log('Serial Error: ', err);
}