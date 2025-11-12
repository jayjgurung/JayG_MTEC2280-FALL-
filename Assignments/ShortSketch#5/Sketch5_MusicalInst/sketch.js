/*
  p5.js UI — sends key DOWN/UP to play only while held:
    D,<index>\n   on keyPressed
    U,<index>\n   on keyReleased
  'm' toggles major/minor: S,<0|1>\n
*/

let serial;
let portName = '/dev/tty.usbserial-2120'; // <-- set to your actual port
let options  = { baudRate: 9600 };

const KEYS = ['a','s','d','f','g','h','j','k'];
let isDown = Array(8).fill(false);
let keyGlow = Array(8).fill(0);

let bpm=90, mode=0, scaleType=0;
let colors=[];

function setup(){
  createCanvas(constrain(windowWidth,400,1080),
               constrain(windowHeight,400,720));
  colorMode(HSB,360,100,100,1); noStroke();
  for(let i=0;i<8;i++) colors[i]=color((i*360/8)%360,80,70,1);

  serial = new p5.SerialPort();
  serial.on('list', printList);
  serial.on('connected', serverConnected);
  serial.on('open', portOpen);
  serial.on('data', serialEvent);
  serial.on('error', serialError);
  serial.on('close', portClose);
  serial.list();
  serial.open(portName, options);
}

function draw(){
  background(0);
  drawKeyboard();
  drawTopBar();
}

function drawKeyboard(){
  const w=width/8, r=w*0.18, y=height*0.25, h=height*0.65;
  for(let i=0;i<8;i++){
    keyGlow[i]=lerp(keyGlow[i],0,0.08); //lerp = linear interpolation which gradually reduces the glow effect over time

    const base=colors[i];
    fill(hue(base),saturation(base),brightness(base)); rect(i*w,y,w,h,r);
    fill(hue(base),saturation(base),min(100,brightness(base)+30), constrain(keyGlow[i],0,1)); rect(i*w,y,w,h,r);

    fill(255); textAlign(CENTER,CENTER); textSize(min(22,w*0.22));
    text( (mode===0?noteLabel(i):chordLabel(i)), i*w+w/2, y+h/2 );

    fill(255,0.6); textAlign(CENTER,TOP); textSize(min(16,w*0.15));
    text(KEYS[i].toUpperCase(), i*w+w/2, y+10);
  }
}

function noteLabel(i){
  const major=['C','D','E','F','G','A','B','C'];
  const minor=['C','D','D#','F','G','G#','A#','C'];
  return (scaleType===0?major:minor)[i];
}
function chordLabel(i){
  const major=['C','Dm','Em','F','G','Am','B°','C'];
  const minor=['Cm','D°','Eb','Fm','Gm','Ab','Bb','Cm'];
  return (scaleType===0?major:minor)[i];
}

function drawTopBar(){
  const barH=height*0.20;
  const x=width*0.05,y=barH*0.15,w=width*0.45,h=barH*0.55,r=h*0.5;
  fill(20,10,25); rect(x,y,w,h,r);
  textAlign(LEFT,CENTER); fill(255); textSize(16);
  text('notes', x+10, y+h/2);
  textAlign(RIGHT,CENTER); text('arpeggio', x+w-10, y+h/2);
  let knobX = (mode===0)?(x+h/2):(x+w-h/2);
  fill(200,80,80); circle(knobX, y+h/2, h*0.8);

  const ew=width*0.20, eh=h, ex=width*0.70, ey=y;
  fill(210,30,30); rect(ex,ey,ew,eh,r);
  fill(255); textAlign(CENTER,CENTER); textSize(18);
  text(`${bpm} BPM`, ex+ew/2, ey+eh/2);
}

// ---------- KEY HOLD PROTOCOL ----------
function keyPressed(){
  const k = key.toLowerCase();
  if (k==='m'){ scaleType = (scaleType===0)?1:0; serial.write(`S,${scaleType}\n`); return; }

  const idx = KEYS.indexOf(k);
  if (idx>=0 && !isDown[idx]){
    isDown[idx]=true;
    serial.write(`D,${idx}\n`);   // <- key down
    keyGlow[idx]=1.0;
  }
}

function keyReleased(){
  const k = key.toLowerCase();
  const idx = KEYS.indexOf(k);
  if (idx>=0 && isDown[idx]){
    isDown[idx]=false;
    serial.write(`U,${idx}\n`);   // <- key up
  }
}

// ---------- Serial events ----------
function serialEvent(){
  let line = serial.readStringUntil('\n');
  if (!line) return;
  line = line.trim();
  const parts = line.split(',');
  if (parts.length>=2){
    if (parts[0]==='BPM'){ bpm=int(parts[1]); }
    else if (parts[0]==='MODE'){ mode=int(parts[1]); }
    else if (parts[0]==='PLAY'){
      let i=int(parts[1]); if (i>=0 && i<8) keyGlow[i]=1.0;
    }
  }
}

// ---------- Boilerplate ----------
function printList(list){ for(let i=0;i<list.length;i++) print(i+" "+list[i]); }
function serverConnected(){ print("connected"); }
function portOpen(){ print("port open"); }
function portClose(){ print("port close"); }
function serialError(err){ print('serial error: '+err); }
function windowResized(){
  resizeCanvas(constrain(windowWidth,400,1080),
               constrain(windowHeight,400,720));
}