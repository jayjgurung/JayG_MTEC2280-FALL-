# MTEC2280 – Final Project Proposal

**Title:** Expanded Arpeggiated Musical Controller  
**Student:** Jay Jung Gurung  
**Date:** November 26, 2025

---

## 1. Concept Description & Inspiration

For my final project in MTEC2280, I will expand my earlier ESP32 ↔ p5.js musical interaction system into a more expressive, hardware-driven arpeggiated musical controller.

**Previous project features:**
- Play notes using the computer keyboard (sent from p5.js to ESP32)
- Switch between major and minor arpeggios
- Control tempo with a knob
- Hear sound through a piezo element

**New version goals:**
- Feature-rich, physical musical device
- Added control and input methods
- Improved audio
- Custom fabrication
- Small, self-contained controller blending physical interaction, serial communication, and sound generation on ESP32

---

## 2. Sketch of the Design

*(Say “make the sketch” if you want a visual sketch)*

**Verbal layout:**
- Compact control panel with:
  - Three rotary knobs (tempo, note length, root-note selector)
  - One slide potentiometer (volume)
  - Toggle switches for:
    - Major/Minor scale
    - Notes/Arpeggio
    - Sine/Square waveform
  - “Apply Root Note” button
  - Speaker with grill openings
  - ESP32 on custom PCB
  - All components in a custom 3D-printed enclosure

---

## 3. Materials List

**Hardware**
- ESP32-S3 or ESP32-WROVER module
- Full speaker (8Ω) + audio amplifier (PAM8302 or PAM8403)
- 3 × rotary potentiometers (10kΩ): tempo, note length, root note scroll
- 1 × slide potentiometer (10kΩ) for volume
- 3 × toggle switches: Major/Minor, Notes/Arpeggio, Sine/Square waveform
- 1 × pushbutton (Apply Root Note)
- Custom PCB, JST connectors, wires, headers
- 3D printed enclosure (PLA/ABS)
- USB cable (serial communication)

**Software**
- Arduino IDE / PlatformIO (ESP32 programming)
- p5.js (keyboard input + visualization)
- KiCad (PCB design)
- Autodesk Fusion 360 (enclosure)

**Physical Materials**
- PLA filament
- Screws (M3 or ¼-20)
- Solder & fabrication tools

---

## 4. Skills I Already Have

- Serial communication between p5.js and ESP32
- Mapping analog inputs (potentiometers)
- Digital input handling (buttons/toggles)
- Generating square-wave tones with LEDC
- Programming arpeggios and musical scales
- p5.js input handling and UI
- Basic PCB workflow and design
- Basic 3D modeling and fabrication

---

## 5. Skills I Need to Learn (Technical Research Challenge)

- Using DAC output on ESP32 for sine wave generation
- Audio amplifier integration with proper gain and wiring
- Designing, routing, and producing a manufacturable custom PCB
- Implementing note length envelopes that preserve BPM
- Expanding serial protocol to handle three rows of keyboard input
- Adding waveform switching logic in ESP32
- Designing a professional enclosure with tolerances for screws, pots, and switches

---

## 6. Precedent Research (with Short Explanations)

**1. ESP32 Tone Generation Examples**  
[Random Nerd Tutorials: ESP32 PWM](https://randomnerdtutorials.com/esp32-pwm-arduino-ide)  
*Shows method of generating tones using LEDC PWM. My project adds DAC, waveform selection, and arpeggiated logic.*

**2. DIY Arduino Synth Projects**  
[Notes and Volts](https://www.notesandvolts.com)  
*Shows how physical knobs and switches shape sound. Mine uses ESP32 + p5.js serial communication + custom PCB.*

**3. KiCad Beginner PCB Guide**  
[KiCad Discover](https://www.kicad.org/discover/)  
*Helps with schematic → PCB → fabrication workflow. My PCB integrates microcontroller logic, UI controls, and amplifier system.*

---

## 7. Week-by-Week Timeline

### 📅 Week of 11/26 — Proposal Due
- Finalize concept + feature list
- Draw initial sketches
- Begin testing all potentiometers and switches on breadboard
- Expand keyboard note list in p5.js
- Research ESP32 DAC sine wave audio

---

### 📅 Week of 12/03 — WIP 1 Due

**Hardware Goals:**
- Breadboard prototype with:
  - Tempo knob
  - Note-length knob
  - Root-note scroll knob
  - Root-note “Apply” button
  - Major/Minor toggle
  - Notes/Arpeggio toggle
  - Sine/Square waveform toggle
  - Volume slider in amplifier circuit

**Software Goals:**
- p5.js updated for three rows of keyboard input
- p5.js sends expanded note data to ESP32
- ESP32 sketch supports new input modes + settings
- Begin writing envelope logic for note length

**Fabrication Goals:**
- First pass of PCB schematic in KiCad
- Simple mock-up enclosure in Fusion

---

### 📅 Week of 12/10 — WIP 2 Due

**Hardware Goals:**
- Implement sine wave via ESP32 DAC
- Integrate amplifier + speaker
- Full prototype on breadboard
- Finalize volume slider functionality

**Software Goals:**
- Arpeggio logic for controllable note length
- Updated p5.js UI feedback
- Full input → sound chain operational

**Fabrication Goals:**
- Final PCB layout ready for order
- Final enclosure model in Fusion
- Order PCB

**Deliverables for 12/10:**
- Working prototype
- Code progress
- PCB files
- Enclosure 3D model

---

### 📅 Week of 12/17 — Final Presentation / Critique

**Hardware Goals:**
- Receive PCB and solder all components
- Assemble final enclosure

**Software Goals:**
- Final debugging
- Polish p5.js UI
- Add visualizations if time permits

**Final Deliverables:**
- Fully functional hardware musical controller
- Custom PCB + 3D enclosure
- Working modes: major/minor, notes/arpeggio, sine/square
- Expanded keyboard mapping
- Final documentation (README, video demo, images)

---