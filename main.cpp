/* The above code is a C++ program that includes several header files, defines some boolean variables,
and initializes some variables. It seems to be related to a game or simulation as it includes
variables like `poleCollided`, `stopEverything`, `Tries`, and `Goals`. Additionally, it includes
vectors and functions related to graphics using OpenGL (GL/glut.h). The code also defines a vector
`currentTextColor` with RGBA values for text color. However, there is a syntax error with the ` */
#include <GL/glut.h>
#include <bits/stdc++.h>
#include "constants.h"
#include "functionalities.h"
#include "shapes.h"

using namespace std;


bool poleCollided[3];
bool stopEverything = false;
unsigned int Tries, Goals;
vector<float> currentTextColor = {1, 1, 1, 1};


void showScore();

/**
 * The function `updatePos` handles collisions with poles, defenders, and boundary walls, and updates
 * the position and velocity of a physical object in a simulated environment.
 * 
 * @param p The `p` parameter in the `updatePos` function represents the `PhysicalState` object that
 * contains information about the current state of the physical entity being updated. It includes
 * attributes such as `positionCurrent` (current position coordinates), `velocityCurrent` (current
 * velocity components), `timePassed`
 * @param t The parameter `t` in the `updatePos` function represents the time elapsed since the last
 * update. It is used to calculate the new position of the physical state `p` based on its current
 * velocity and any collisions or interactions that may occur during that time interval.
 */
void updatePos(PhysicalState &p, double t) {
    p.timePassed += t;

    {//Collision with Pole0
        if (p.positionCurrent.z < POLE_HEIGHT && p.positionCurrent.z > 0.0) {
            axes t = {-POLE_LENGTH / 2, GOAL_POST_Y, p.positionCurrent.z};
            if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && !poleCollided[0]) {
                poleCollided[0] = true;
                double alpha, beta, theta;
                beta = atan(p.velocityCurrent.y / p.velocityCurrent.x);
                axes vec;
                for (int i = 0; i < 3; ++i) {
                    vec[i] = t[i] - p.positionCurrent[i];
                }
                alpha = atan(vec.y / vec.x);
                theta = PI / 2.0 - beta + 2 * alpha;
                double v = p.velocityCurrent.x * p.velocityCurrent.x + p.velocityCurrent.y * p.velocityCurrent.y;
                v = sqrt(v);
                p.velocityCurrent.y = -v * cos(theta) * p.elasticity;
                p.velocityCurrent.x = v * sin(theta) * p.elasticity;
            } else if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && poleCollided[0]) {
                poleCollided[0] = false;
            }
        }
    }
    {//Collision with Pole2
        if (p.positionCurrent.z < POLE_HEIGHT && p.positionCurrent.z > 0.0) {
            axes t = {POLE_LENGTH / 2, GOAL_POST_Y, p.positionCurrent.z};
            if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && !poleCollided[2]) {
                poleCollided[2] = true;
                double alpha, beta, theta;
                beta = atan(p.velocityCurrent.y / p.velocityCurrent.x);
                axes vec;
                for (int i = 0; i < 3; ++i) {
                    vec[i] = t[i] - p.positionCurrent[i];
                }
                alpha = atan(vec.y / vec.x);
                theta = PI / 2.0 - beta + 2 * alpha;
                double v = p.velocityCurrent.x * p.velocityCurrent.x + p.velocityCurrent.y * p.velocityCurrent.y;
                v = sqrt(v);
                p.velocityCurrent.y = v * cos(theta) * p.elasticity;
                p.velocityCurrent.x = -v * sin(theta) * p.elasticity;

            } else if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && poleCollided[2]) {
                poleCollided[2] = false;
            }
        }
    }
    {//Collision with Pole1
        if (p.positionCurrent.x < POLE_LENGTH / 2 + POLE_RADIUS &&
            p.positionCurrent.x > -POLE_LENGTH / 2 - POLE_RADIUS) {
            axes t = {p.positionCurrent.x, GOAL_POST_Y, POLE_RADIUS + POLE_HEIGHT};
            if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && !poleCollided[1]) {
                poleCollided[1] = true;
                double alpha, beta, theta;
                beta = atan(p.velocityCurrent.y / p.velocityCurrent.z);
                axes vec;
                for (int i = 0; i < 3; ++i) {
                    vec[i] = t[i] - p.positionCurrent[i];
                }
                alpha = atan(vec.y / vec.z);
                theta = PI / 2.0 - beta + 2 * alpha;
                double v = p.velocityCurrent.z * p.velocityCurrent.z + p.velocityCurrent.y * p.velocityCurrent.y;
                v = sqrt(v);
                p.velocityCurrent.y = v * cos(theta) * p.elasticity;
                p.velocityCurrent.z = -v * sin(theta) * p.elasticity;
            } else if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && poleCollided[1]) {
                poleCollided[1] = false;
            }
        }
    }

    {//Collision with Defender
        if (p.positionCurrent.x < defender.state.positionCurrent.x + defender.width / 2.0 &&
            p.positionCurrent.x > defender.state.positionCurrent.x - defender.width / 2.0 &&
            p.positionCurrent.z < defender.height &&
            p.positionCurrent.y + BALL_RADIUS / 2.0 + DEFENDER_THICKNESS / 2.0 >= GOAL_POST_Y && !determineSphere) {
            p.velocityCurrent.y *= -p.elasticity;
        }
    }

    {   // Hockey puck physics (no gravity, sliding)

   // Update position only in X and Y
    p.positionCurrent.x += p.velocityCurrent.x * t;
    p.positionCurrent.y += p.velocityCurrent.y * t;

// Lock Z to ground
    p.positionCurrent.z = 0;
    p.velocityCurrent.z = 0;

// Apply friction
float friction = 0.99;
p.velocityCurrent.x *= friction;
p.velocityCurrent.y *= friction;

// Stop when very slow
if (fabs(p.velocityCurrent.x) <= 0.01) p.velocityCurrent.x = 0;
if (fabs(p.velocityCurrent.y) <= 0.01) p.velocityCurrent.y = 0;

}
    if (p.positionCurrent.y + BALL_RADIUS > 20.0 || p.positionCurrent.y - BALL_RADIUS < -20.0) {
        p.velocityCurrent.y = -p.velocityCurrent.y;
    }
    if (p.positionCurrent.x + BALL_RADIUS > 20.0 || p.positionCurrent.x - BALL_RADIUS < -20.0) {
        p.velocityCurrent.x = -p.velocityCurrent.x;
    }
}


/**
 * The function `renderBitmapString` renders a string of characters using a specified font at a given
 * position in a 3D space.
 * 
 * @param x The `x` parameter in the `renderBitmapString` function represents the x-coordinate where
 * the bitmap string will be rendered on the screen.
 * @param y The `y` parameter in the `renderBitmapString` function represents the y-coordinate where
 * the text will be rendered on the screen. It determines the vertical position of the text within the
 * 3D space.
 * @param z The 'z' parameter in the function `renderBitmapString` represents the depth or distance of
 * the text from the viewer in a 3D space. It is used to specify the position of the text along the
 * z-axis.
 * @param font The `font` parameter in the `renderBitmapString` function is a pointer to the font style
 * that you want to use for rendering the text. It is typically one of the predefined fonts available
 * in the GLUT library, such as `GLUT_BITMAP_8_BY_13`, `GLUT_BITMAP
 * @param string The `string` parameter in the `renderBitmapString` function is a pointer to a
 * character array (or string) that contains the text you want to render on the screen. It is the text
 * that will be displayed using the specified font at the specified position (x, y, z) on
 */
void renderBitmapString(
        float x,
        float y,
        float z,
        void *font,
        char *string) {

    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

/**
 * The updatePosCallBack function updates the position of a sphere at a constant frame rate while in
 * shooting mode.
 * 
 * @param _ The parameter "_" in the function "updatePosCallBack" is an integer variable that is not
 * being used within the function. It is a placeholder variable that is commonly used when a function
 * requires a parameter but the parameter is not needed for the logic inside the function.
 */
void updatePosCallBack(int _) {
    if (currentMode != SHOOTING && currentlyWaiting) {
        currentMode = SHOOTING;
        currentlyWaiting = false;
    }
    float fps = 60;
    if (currentMode == SHOOTING) {
        updatePos(sphere, 1.0 / fps);
        glutTimerFunc(100 / fps, updatePosCallBack, 0);
    }
}


/**
 * The `draw()` function in C++ sets up the drawing perspective, handles lighting, and renders various
 * elements such as a cylinder, audience, goal post, and HUD in a 3D scene.
 */
void draw() {
    glLoadIdentity(); //Reset the drawing perspective
    cameraPosition(toLookAt, sphereCamera.distance, sphereCamera.zAngle, sphereCamera.xAngle);
    if (firstTime) {
        glutWarpPointer(WIDTH / 2, HEIGHT);
        firstTime = false;
    }
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 0.7f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, -100.0f, 100.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    GLfloat lightColor1[] = {0.3f, 0.3f, 0.1f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, -1.0f, -1.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor1);


    GLfloat lightColor2[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos2[] = {0.0f, 100.0f, 0.10f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

glPushMatrix();
glColor3f(0.3, 0.3, 0.3);

float puckHeight = BALL_RADIUS * 0.4;

glTranslatef(
    sphere.positionCurrent.x,
    sphere.positionCurrent.y,
    sphere.positionCurrent.z - BALL_RADIUS
);

GLUquadric* quad = gluNewQuadric();

gluCylinder(quad, BALL_RADIUS, BALL_RADIUS, puckHeight, 30, 10);

// bottom
gluDisk(quad, 0, BALL_RADIUS, 30, 1);

// top
glTranslatef(0, 0, puckHeight);
gluDisk(quad, 0, BALL_RADIUS, 30, 1);

gluDeleteQuadric(quad);
glPopMatrix();
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1.0, 1.0, 1.0, 0.7);
    

    glPopAttrib();
    glPopMatrix();

    if (currentMode == POWERING || currentMode == AIMING) {
        aimArrow.drawWithAngles();
    }
    drawChalkLines();

    drawAudience();
    ground.draw();
    drawBillboard3D("", 0, 20.0, 4.5);
    drawGoalPost();
    defender.draw();

    showMsg();
        std::string msg = "";

if (determineSphere) {
    if (isItGoal(*determineSphere))
        msg = "GOAL!";
    else
        msg = "MISS!";
}

    drawHUD();

    glutSwapBuffers();
    glutPostRedisplay();
}

/**
 * The function `showScore` in C++ renders a visual display of goals and tries with corresponding
 * colors and text positions.
 */
void showScore() {
    glPushMatrix();
    glTranslatef(0, GOAL_POST_Y, POLE_HEIGHT + BALL_RADIUS);

    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(-POLE_LENGTH / 2.0 - BALL_RADIUS / 2.0, 0.0001, 0);
    glColor4f(125 / 255.0, 178 / 255.0, 209 / 255.0, 0.5);
    glScalef(POLE_LENGTH + BALL_RADIUS, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-POLE_LENGTH / 2.0 - BALL_RADIUS / 2.0, 0.0001, 1);
    glColor4f(178 / 255.0, 255 / 255.0, 215 / 255.0, 0.5);
    glScalef(POLE_LENGTH + BALL_RADIUS, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, 0);
    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);

    glPushMatrix();
    currentTextColor = {24 / 255.0, 163 / 255.0, 24 / 255.0, 1.0};
    glTranslatef(-POLE_LENGTH / 2, 0, 1);
//    glScalef(FONT_SIZE,FONT_SIZE,1.0);
    writeText("GOALS", font, LEFT);
    glPopMatrix();

    glPushMatrix();
    currentTextColor = {32 / 255.0, 140 / 255.0, 32 / 255.0, 1.0};
    glTranslatef(POLE_LENGTH / 2, 0, 1);
//    glScalef(FONT_SIZE,FONT_SIZE,FONT_SIZE);
    writeText(to_string(Goals), font, RIGHT);
    glPopMatrix();

    glPushMatrix();
    currentTextColor = {27 / 255.0, 92 / 255.0, 145 / 255.0, 1.0};
    glTranslatef(-POLE_LENGTH / 2, 0, 0);
//    glScalef(FONT_SIZE,FONT_SIZE,1.0);
    writeText("TRIES", font, LEFT);
    glPopMatrix();

    glPushMatrix();
    currentTextColor = {0.1, 0.1, 1.0, 1.0};
    glTranslatef(POLE_LENGTH / 2, 0, 0);
//    glScalef(FONT_SIZE,FONT_SIZE,FONT_SIZE);
    writeText(to_string(Tries), font, RIGHT);
    glPopMatrix();


    glPopMatrix();
}

/**
 * The function `incrementPowerMeter` updates the `powerMeter` value based on certain conditions and
 * schedules itself to be called again after a specific time interval if certain conditions are met.
 * 
 * @param _ The parameter `_` in the `incrementPowerMeter` function is not being used within the
 * function body. It is simply a placeholder for an integer parameter that is not being utilized in the
 * function logic.
 */
void incrementPowerMeter(int _) {
    static int up = 1;
    if (powerMeter >1.0 || powerMeter < 0.0) {
        up *= -1;
    }
    if (!currentlyWaiting && currentMode == POWERING) {
        powerMeter += up * 0.015;
        glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
    }
}

/**
 * The function `handleKeypress` in C++ handles key presses for different modes and adjusts camera
 * distance accordingly.
 * 
 * @param key The `key` parameter in the `handleKeypress` function represents the key that was pressed
 * on the keyboard. It is of type `unsigned char`, which means it can hold ASCII values of characters.
 * When a key is pressed, the corresponding ASCII value is passed to this function as the `key
 * @param x The parameter `x` in the `handleKeypress` function represents the current x-coordinate of
 * the mouse cursor when the key press event occurred. It is used to track the position of the mouse
 * along the horizontal axis at the time of the key press event.
 * @param y The parameter `y` in the `handleKeypress` function represents the current y-coordinate of
 * the mouse cursor when the key was pressed. It is used to track the position of the mouse along the
 * vertical axis.
 */
void handleKeypress(unsigned char key, //The key that was pressed
                    int x, int y) {    //The current mouse coordinates
    if (currentMode != HELP) {
        switch (key) {
            case '+':
                sphereCamera.distance -= 0.1f;
                sphereCamera.distance += (sphereCamera.distance < MIN_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
//            cout<<sphereCamera.distance<<endl;

                break;
            case '-':
                sphereCamera.distance += 0.1f;
                sphereCamera.distance -= (sphereCamera.distance > MAX_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
//            cout<<sphereCamera.distance<<endl;
                break;
        }
    } else {
        if (key == 27) {
            currentMode = ADJUSTING;
        }
    }
    downKeys[key] = true;
    if (currentMode == ADJUSTING) {
        switch (key) {
            case '\r':
                currentMode = AIMING;
                break;
            case EXIT_KEY: //Escape key
                exit(0); //Exit the program
        }
    }
    if (currentMode == AIMING) {
        switch (key) {
            case ' ':
                currentMode = POWERING;
                glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
                break;
            case 27: //Escape key
                currentMode = ADJUSTING;
        }
    }

}

/**
 * The `idle` function in C++ handles various game modes and actions such as powering up the ball,
 * shooting, and determining goal scores.
 */
void idle() {
    if (!currentlyWaiting) {
        if (currentMode == POWERING && !downKeys[' ']) {
            sphere.velocityCurrent[0] = sphere.velocityInitial[0] =
                    cos(DEG2GRAD(aimArrow.zAngle)) * sin(DEG2GRAD(aimArrow.yAngle)) * BALL_MAX_SPEED * powerMeter;
            sphere.velocityCurrent[1] = sphere.velocityInitial[1] =
                    cos(DEG2GRAD(aimArrow.zAngle)) * cos(DEG2GRAD(aimArrow.yAngle)) * BALL_MAX_SPEED * powerMeter;
            sphere.velocityCurrent[2] = sphere.velocityInitial[2] =
                    sin(DEG2GRAD(aimArrow.zAngle)) * BALL_MAX_SPEED * powerMeter + BALL_MIN_SPEED;
            if (powerMeter >= 1.0)
                powerMeter = 1.0;
            glutTimerFunc(10, updatePosCallBack, 0);
            currentlyWaiting = true;
        }
        if (currentMode == POWERING && downKeys[27]) {
            currentMode = AIMING;
            powerMeter = 0.0;
        }
        if (currentMode == SHOOTING) {
            if (sphere.positionCurrent.y <= GOAL_POST_Y)
                toLookAt = sphere.positionCurrent;
        }
        if (currentMode == SHOOTING) {
            if (sphere.positionCurrent.y > GOAL_POST_Y || sphere.velocityCurrent.y <= 0) {
                if (!determineSphere && !stopEverything) {
                    determineSphere = new PhysicalState;
                    *determineSphere = sphere;

                    scoredGoal = isItGoal(*determineSphere);
                    if (scoredGoal) {
                        Goals++;
                        system("paplay resources/goal.wav&");
                    }

                    rotateMsg(0);
                    glutTimerFunc(1000 * RESET_TIME, initialiseEverythingCallback, 0);
                    Tries++;
                }
            }
        }
        if (currentMode == POWERING) {
        }
    }
    glutPostRedisplay();
}

axes toLookAt;

/**
 * The function `handleUpKeypress` sets the value corresponding to the given key in the `downKeys`
 * array to false.
 * 
 * @param key The `key` parameter represents the ASCII value of the key that was pressed.
 * @param x The parameter `x` in the `handleUpKeypress` function represents the x-coordinate of the
 * mouse cursor when the keypress event occurred.
 * @param y The parameter `y` in the `handleUpKeypress` function represents the y-coordinate of the
 * mouse pointer when the keypress event occurred.
 */
void handleUpKeypress(unsigned char key, int x, int y) {
    downKeys[key] = false;
}

/**
 * The function `handleSpecialKeypress` adjusts the angles of an aim arrow based on specific key
 * presses when in aiming mode.
 * 
 * @param key The `key` parameter in the `handleSpecialKeypress` function represents the special key
 * that was pressed. In this case, it is an integer value that corresponds to a special key on the
 * keyboard, such as `GLUT_KEY_UP`, `GLUT_KEY_DOWN`, `GLUT_KEY_LEFT
 * @param x The `x` parameter in the `handleSpecialKeypress` function represents the x-coordinate of
 * the mouse cursor when the special key was pressed. It is a parameter that is commonly used in GLUT
 * (OpenGL Utility Toolkit) functions to provide the current x-coordinate of the mouse cursor within
 * the window when a
 * @param y The parameter `y` in the `handleSpecialKeypress` function represents the y-coordinate of
 * the mouse cursor when the special key press event is triggered. It is used in conjunction with the
 * `x` parameter to determine the position of the mouse cursor on the screen when a special key is
 * pressed.
 */
void handleSpecialKeypress(int key, int x, int y) {

    if (currentMode == AIMING) {

        const float increment=2.0f;
        switch (key) {
            case GLUT_KEY_UP:
                aimArrow.zAngle += (aimArrow.zAngle>50?0:increment);
                break;
            case GLUT_KEY_DOWN:
                aimArrow.zAngle -= (aimArrow.zAngle<0.01?0:increment);
                break;
            case GLUT_KEY_LEFT:
                aimArrow.yAngle -= (aimArrow.yAngle<-60?0:increment);
                break;
            case GLUT_KEY_RIGHT:
                aimArrow.yAngle += (aimArrow.yAngle>60?0:increment);
                break;
        }
    }
}

template<typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
 * The function `handlePassiveMouse` adjusts the angles of a camera based on mouse movement, but only
 * if the current mode is not set to `HELP`.
 * 
 * @param x The `x` parameter represents the x-coordinate of the mouse cursor on the screen.
 * @param y The `y` parameter in the `handlePassiveMouse` function represents the vertical position of
 * the mouse cursor on the screen. It is used to calculate the `sphereCamera.zAngle` based on the
 * height of the window and the position of the cursor.
 */
void handlePassiveMouse(int x, int y) {
//    if (currentMode == ADJUSTING) {
    if (currentMode != HELP) {
        sphereCamera.xAngle = -90 + (x - WIDTH / 2) * 90 / WIDTH;
        sphereCamera.zAngle = 45 + -1 * (y) * 30 / HEIGHT;
    }
}

/**
 * The function `myInit` sets up various OpenGL settings and enables features like depth testing,
 * lighting, and blending.
 */
void myInit(void) {
    glClearColor(137 / 255.0, 206 / 255.0, 255 / 255.0, 0);
//    glOrtho(0, WIDTH, 0, HEIGHT, 0, 500);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LIGHTING); //Enable lighting

    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_LIGHT2); //Enable light #2
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH);
    backgroundMusicPlayer(0);
    updateDefenderPosition(0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_MULTISAMPLE);

}

/**
 * The main function initializes various components and sets up a GLUT window for a graphics
 * application.
 * 
 * @param argc The `argc` parameter in the `main` function stands for "argument count" and represents
 * the number of arguments passed to the program when it is executed from the command line. It includes
 * the name of the program itself as the first argument.
 * @param argv The `argv` parameter in the `main` function is an array of strings that represent the
 * command-line arguments passed to the program when it is executed. Each element in the `argv` array
 * is a null-terminated C-string, where `argv[0]` typically holds the name of the
 * 
 * @return The main function is returning an integer value of 0.
 */
int main(int argc, char *argv[]) {
    initialiseEverything();
    currentMode = HELP;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    glutFullScreen();
    glutReshapeFunc(handleResize);
    glutIdleFunc(idle);
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleUpKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutPassiveMotionFunc(handlePassiveMouse);
    groundTexture = convertAndLoadTexture("resources/ice1.txt");
    defenderTexture = convertAndLoadTexture("resources/defender.txt");
    crowdTexture = convertAndLoadTexture("resources/side.txt");
    sideTexture  = convertAndLoadTexture("resources/side.txt"); 
    font = convertAndLoadTexture("resources/fonts/Ubuntu Mono Nerd Font Complete Mono.txt");
    ads = convertAndLoadTexture("resources/ads.txt");
    leftArm = convertAndLoadTexture("resources/left_arm.txt");
    rightArm = convertAndLoadTexture("resources/right_arm.txt");
    glutMouseFunc(NULL);
    glutDisplayFunc(draw);
    myInit();
    glutMainLoop();

    return 0;
}