#  Hockey Penalty Shooter (OpenGL)

A 3D interactive hockey penalty shooting simulation built using **C++ and OpenGL**, featuring realistic puck physics, animated defender movement, textured environment, and dynamic camera controls.

---

##  Features

*  **Physics Engine**

  * 2D puck motion with friction-based decay
  * Collision detection with boundaries and goalposts
  * Goal detection logic

*  **Animated Defender**

  * Rotating arms with correct pivot-based motion
  * Collision interaction with puck

*  **Aiming & Shooting System**

  * Adjustable direction and power
  * Visual aim arrow

*  **Dynamic Camera**

  * Orbit camera using spherical coordinates
  * Mouse-controlled rotation and zoom

*  **Textured Environment**

  * Ice ground, audience walls, and player textures
  * Billboard scoreboard with real-time updates

*  **Score System**

  * Tracks **Goals** and **Tries**
  * Displayed on 3D billboard

*  **Lighting System**

  * Multiple light sources (ambient + diffuse)
  * Realistic shading on objects

---

##  Project Structure

```
├── main.cpp              # Main loop, rendering pipeline, input handling
├── shapes.cpp           # Object geometry (defender, poles, ground)
├── functionalities.cpp  # Camera, textures, HUD, utilities
├── textures/            # Image assets (ground, defender, arms, etc.)
├── README.md
```

---

##  Requirements

* C++ Compiler (g++ recommended)
* OpenGL
* GLUT / freeGLUT
* GLU

---

##  Installation & Run

###  Linux (Ubuntu)

```bash
g++ main.cpp shapes.cpp functionalities.cpp -lGL -lGLU -lglut -o hockey
./hockey
```

---

###  Windows (MinGW)

Make sure you have:

* `freeglut`
* OpenGL libraries linked

```bash
g++ main.cpp shapes.cpp functionalities.cpp -lfreeglut -lglu32 -lopengl32 -o hockey.exe
hockey.exe
```

---

##  Controls

| Action        | Key            |
| ------------- | -------------- |
| Adjust camera | Mouse movement |
| Zoom in/out   | `+` / `-`      |
| Aim direction | Arrow keys     |
| Power control | Spacebar       |
| Shoot         | Enter          |

---

##  Physics Model

* Motion constrained to **x–y plane**
* Friction applied per frame:

  ```cpp
  velocity *= 0.99;
  ```
* Small velocity threshold stops motion
* Boundary collisions handled using **velocity reflection**

---

##  Camera System

Implemented using `gluLookAt`:

* **VRP** → Ball position
* **VPN** → Direction from camera to ball
* **VRC** → Camera coordinate system

Camera moves on a **spherical orbit** around the puck.

---

##  Rendering Pipeline

```
Reset → Camera → Lighting → Clear → Draw Objects → HUD → Swap Buffers
```

Includes:

* Depth testing for correct occlusion
* Texture mapping using `glTexCoord2f`
* Matrix transformations (`translate`, `rotate`, `scale`)

---

##  Screenshots

<img width="900" height="507" alt="image" src="https://github.com/user-attachments/assets/412223b9-17c6-4c84-ab05-85ab082fe58c" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/baf65cd3-aa22-40ab-9e15-1d8b04476acf" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/75314dbe-596a-46be-b889-4434cac8a7dd" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/fdcadb0f-0a46-4111-9b2b-e639072d7b17" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/b1e8d4b9-35a5-4581-8628-40747b4f7f3f" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/af11f11b-e3e8-40e4-9bab-d6ceda330ff9" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/b3e99601-7b4b-4bfd-8c1c-1ecef3468473" />

<img width="900" height="506" alt="image" src="https://github.com/user-attachments/assets/986851e6-f4b0-401e-9b62-fddaca2b2d29" />

<img width="900" height="507" alt="image" src="https://github.com/user-attachments/assets/d7f7012a-c369-44ce-998e-8fc24d3068cf" />



---

##  Future Improvements

* Shadow rendering
* Sound effects
* Advanced collision physics
* AI-based goalkeeper movement

---

##  Author

* GitHub: https://github.com/Parjanya09

---


