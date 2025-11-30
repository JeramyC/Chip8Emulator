
##  CHIP-8 Overview

CHIP-8 is a small interpreted programming language developed in the late 1970s.  Due to it's simplicity and size, it makes for a great introduction into the world of emulation

- 36 Opcodes
- 4KB (4096 bytes) of memory
- 64×32 monochrome display
- Hex-based keyboard
- Delay and sound timers running at 60 Hz

Features the emulator provides:

- Full CPU implementation with fetch → decode → execute cycle
- Accurate timing using modern C++ chrono clocks
- Display rendering using SDL2 libraries
- Ability to run popular CHIP-8 games like Pong, Tetris, Space Invaders, etc.

##  Architecture Overview

### CPU

The CPU handles instruction execution and contains:

#### Registers
- **V0 – VF (16 general purpose registers)**  
  VF is also used as a flag for arithmetic operations and sprite collision.
- **I (16-bit index register)**
- **PC (program counter)** — starts at `0x200`
- **SP (stack pointer)**
- **Stack (16 entries)** — stores subroutine return addresses


### Memory Layout
```
0x000–0x1FF  Reserved (interpreter, fonts)
0x200–0xFFF  ROM instructions and data
```

### Display
- Resolution: **64 × 32 monochrome**
- Each pixel is 1-bit (on/off)
- Sprites are XOR’d onto the screen
- VF = 1 if any pixel is unset during sprite draw (collision)

### Keyboard
CHIP-8 uses a **hex keypad**:

```
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
```

Your emulator maps these to intuitive Windows keys (see key table below).

## Opcode Summary

Each opcode is 16 bits. 

Examples:

| Opcode | Meaning |
|--------|---------|
| `00E0` | Clear screen |
| `00EE` | Return from subroutine |
| `6XNN` | Set VX = NN |
| `7XNN` | Add NN to VX |
| `ANNN` | Set I = NNN |
| `DXYN` | Draw sprite |

## Timing System

CHIP-8 uses two timers running at **60 Hz**:

- **Delay Timer (DT)** — decrements to 0  
- **Sound Timer (ST)** — when > 0, a tone plays

The CPU typically runs between **500–1000 Hz**.



## Key Mapping Reference (from BytePlay KeyMapping.xlsx)

### General CHIP-8 → Windows Key Mapping

| CHIP-8 | Key |
|--------|-----|
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
| C | 4 |
| 4 | q |
| 5 | w |
| 6 | e |
| D | r |
| 7 | a |
| 8 | s |
| 9 | d |
| E | f |
| A | z |
| 0 | x |
| B | c |
| F | v |

### Game-Specific Mappings

| Game | Action | Key |
|------|--------|-----|
| Pong | Player 1 Up | 1 |
| Pong | Player 1 Down | q |
| Pong | Player 2 Up | 4 |
| Pong | Player 2 Down | r |
| Tetris | Left | w |
| Tetris | Right | e |
| Tetris | Rotate | d |
| Tetris | Down | x |