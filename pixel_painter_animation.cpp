## 🎬 Animation Module (Free Expansion)

This expansion adds **multi-frame animation** support to Pixel Painter!

### New Features

- 🎞️ **8 Frames** – Create frame-by-frame pixel animations
- ▶️ **Playback** – Watch your animation come to life
- 🎚️ **Adjustable Speed** – 5 FPS default (modify `animationSpeed` variable)
- 💾 **Save/Load** – Export animation to `animation.dat` and import later
- ⌨️ **New Hotkeys** – Arrow keys to switch frames, SPACE to play/stop

### Additional Controls

| Key        | Action                          |
|------------|---------------------------------|
| `←` / `→`  | Previous/Next frame (when paused) |
| `SPACE`    | Play / Pause animation          |
| `S`        | Save all frames to file         |
| `L`        | Load animation from file        |
| `C`        | Clear all frames                |

### UI Additions

- **Frame selector** (right side) – Click any frame to edit
- **Play / Stop buttons** – Easy animation control

### Compile & Run

```bash
g++ -o pixel_painter_animation pixel_painter_animation.cpp -lSDL2
./pixel_painter_animation
