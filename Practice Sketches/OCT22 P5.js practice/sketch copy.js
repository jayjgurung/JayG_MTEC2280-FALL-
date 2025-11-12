let x = 0;
let y = 200;
let n = 5;
let heightRect = 100;

function setup() { //runs once at beginning of sketch
  createCanvas(600, 600);
}

function draw() { //runs on loop at frame rate
  background(127);
  x = x + n;

  if (x > width-50 || x < 0) {
    n = n * -1;
  }
  

  fill(255, mouseX, 0);
  rect(x , y, 50, 50);
  print(x);
}

function mousePressed() {
  y +=20 ; 
}

function keyPressed() {
  if (key == 's') {
    y -=20 ; 
  }

}

