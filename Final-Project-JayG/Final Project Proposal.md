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

**Week of 11/26 — Proposal Due**

- Deliverable: Submit written proposal only.
- No build or coding required this week.

Focus this week:

- Turn in proposal
- Begin light planning

---

**Week of 12/03 — WIP 1 Due**

**Hardware Goals**

- Start breadboard prototyping
  - Test potentiometers (tempo, note length, root-note scroll)
  - Test toggle switches (major/minor, notes/arpeggio, sine/square)
  - Test Apply Root Note button
  - Begin basic wiring layout planning for PCB

**Software Goals**

- Expand p5.js to recognize three rows of keyboard input
- Define a compact serial protocol for expanded note data
- Update ESP32 sketch to accept:
  - Scale mode switch
  - Waveform mode
  - Note-length input
  - Root-note selection input

**Fabrication Goals**

- Begin first draft of PCB schematic in KiCad
- Start rough 3D enclosure planning (basic measurements)

**Deliverables for WIP1**

- Breadboard test of inputs
- Updated p5.js note mapping
- Early ESP32 code structure (pseudocode acceptable)
- KiCad schematic draft (not routed)

---

### Week of 12/10 — WIP 2 Due

**Hardware Goals**

- Complete full breadboard prototype:
  - Tempo knob working
  - Note-length knob working
  - Root-note scroll + Apply button
  - Major/minor, sine/square, notes/arpeggio toggles
  - Speaker + amplifier integrated
  - Volume slider functional
  - Implement sine wave output via ESP32 DAC

**Software Goals**

- Implement note-length logic while maintaining BPM
- Integrate amplifier-safe amplitude control
- Update p5.js UI to reflect new modes and inputs
- Confirm all serial messages work with ESP32

**Fabrication Goals**

- Finalize full PCB layout (schematic + traces)
- Finalize 3D enclosure model in Fusion 360
- Order PCB from manufacturer

**Deliverables for WIP2**

- Fully functional breadboard prototype
- Working ESP32 + p5.js communication
- Ready-to-order PCB files (Gerbers)
- Final 3D enclosure model render

---

### Week of 12/17 — Final Presentation & Critique

**Hardware Goals**

- Receive and assemble PCB
- Solder all components
- Mount PCB inside 3D-printed enclosure
- Install speaker, knobs, toggles, and secure all wiring
- Perform full system test & debugging

**Software Goals**

- Finalize ESP32 code (notes, arpeggios, waveform modes, BPM)
- Finalize p5.js interface & keyboard visual feedback
- Optimize serial timing

**Final Deliverables**

- Fully assembled device
  - Custom PCB
  - 3D-printed enclosure
  - Amplified speaker output
  - Full set of knobs + toggles working
- Demonstration video
- Final code
- Documentation with images and explanations
- In-class presentation