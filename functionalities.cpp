/* The above code is a C++ program that includes headers for functionalities and shapes, as well as the
standard library header <bits/stdc++.h>. It defines several global variables such as currentMode,
currentlyWaiting, downKeys, scoredGoal, goalCount, totalTries, mouseX, mouseY, firstTime, sphere,
and determineSphere. The program initializes some of these variables and sets the currentMode to
ADJUSTING. */
#include "functionalities.h"
#include "shapes.h"
#include <bits/stdc++.h>

using namespace std;


mode currentMode = ADJUSTING;
bool currentlyWaiting;
bool downKeys[127];
bool scoredGoal;
int goalCount, totalTries;
int mouseX, mouseY;
bool firstTime = true;
PhysicalState sphere, *determineSphere = NULL;

/**
 * The function `handleResize` adjusts the viewport and camera perspective based on the window
 * dimensions.
 * 
 * @param w The parameter `w` represents the width of the viewport or the window in pixels.
 * @param h The parameter `h` in the `handleResize` function represents the height of the viewport or
 * the window in which the OpenGL rendering is being displayed. It is used to set the height of the
 * viewport and calculate the camera perspective based on the aspect ratio of the window.
 */
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
    glFrustum(-1.0 * w / h, 1.0 * w / h, -1, 1, 2, 100);
    glMatrixMode(GL_MODELVIEW);
}

/**
 * The function overloads the subscript operator to access the x, y, or z coordinate of a 3D point.
 * 
 * @return A reference to a double value is being returned based on the index provided. The specific
 * double value returned depends on the index: 
 * - If index is 0, the x value is returned.
 * - If index is 1, the y value is returned.
 * - If index is 2, the z value is returned.
 */
double &axes::operator[](int index) {
    switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            cout << "Out of Bound Axis!" << endl;
            exit(1);
    }
}

/**
 * The function calculates the Euclidean distance between two points represented by their coordinates
 * in 3D space.
 * 
 * @param axes1 The `axes1` and `axes2` parameters in the `distanceBW` function represent two arrays of
 * double values, each containing three elements. The function calculates the Euclidean distance
 * between two points represented by these arrays in a three-dimensional space.
 * @param axes2 The `axes` type seems to represent a collection of values along different axes. In the
 * `distanceBW` function, `axes1` and `axes2` are two instances of this type, each containing values
 * along three axes. The function calculates the Euclidean distance between these two sets of axes
 * 
 * @return The function `distanceBW` calculates the Euclidean distance between two points represented
 * by the `axes` structures `axes1` and `axes2`. It iterates over the three dimensions, calculates the
 * squared difference between the corresponding coordinates, sums up these squared differences, and
 * returns the square root of the sum as the final Euclidean distance.
 */
double distanceBW(axes axes1, axes axes2) {
    double sum = 0.0;
    double sqr;
    for (int i = 0; i < 3; ++i) {
        sqr = axes1[i] - axes2[i];
        sqr *= sqr;
        sum += sqr;
    }
    return sqrt(sum);
}


/**
 * The `PhysicalState` constructor initializes the position, velocity, acceleration, time, and
 * elasticity properties to default values.
 */
PhysicalState::PhysicalState() {
    positionInitial = positionCurrent = velocityInitial = velocityCurrent = accelerationCurrent = {0.0, 0.0, 0.0};
    timePassed = 0;
    elasticity = 1.0;
}

/**
 * The function overloads the << operator to output the current position, velocity, and time passed of
 * a PhysicalState object.
 * 
 * @return The overloaded << operator function is returning a reference to an ostream object.
 */
ostream &operator<<(ostream &out, PhysicalState &p) {
    out << "Current Position : " << endl;
    for (int i = 0; i < 3; ++i) {
        out << p.positionCurrent[i] << "    ";
    }
    out << endl;
    out << "Current Velocity : " << endl;
    for (int i = 0; i < 3; ++i) {
        out << p.velocityCurrent[i] << "    ";
    }
    out << endl;
    out << "Time Passed : ";
    out << p.timePassed << endl;
 return out;
}


/**
 * The function `isItGoal` checks if a ball is within the goal area based on its current position.
 * 
 * @param ball The `isItGoal` function takes a `PhysicalState` object named `ball` as a parameter. The
 * `PhysicalState` object likely contains information about the current position of a ball in a
 * physical environment, such as its x, y, and z coordinates.
 * 
 * @return a boolean value, either true or false, based on the conditions specified in the if
 * statement.
 */
bool isItGoal(PhysicalState ball) {
    if ((ball.positionCurrent.x <= -POLE_RADIUS + POLE_LENGTH / 2) &&
        (ball.positionCurrent.x >= +POLE_RADIUS - POLE_LENGTH / 2) &&
        (ball.positionCurrent.z <= POLE_HEIGHT) && (ball.positionCurrent.y > GOAL_POST_Y))
        return true;
    else
        return false;

}

/**
 * The function `backgroundMusicPlayer` plays background music in a C++ program using the `paplay`
 * command with a specified volume and sets a timer to repeat the function every 5 seconds.
 * 
 * @param _ The parameter `_` in the `backgroundMusicPlayer` function is an integer variable that is
 * not being used in the function body. It is a placeholder variable that is typically used when a
 * function requires a parameter but the parameter is not needed for the function logic. In this case,
 * the function is using
 */
void backgroundMusicPlayer(int _) {

//    if (currentMode != GOAL_ANIMATION)
    system("paplay resources/back.wav --volume 30000 &");
    glutTimerFunc(5 * 1000, backgroundMusicPlayer, 0);
}


/**
 * The function `initialiseEverything` initializes various game elements such as ground, poles, aim
 * arrow, defender, camera, sphere, power meter, and current mode with specific values and settings.
 */
void initialiseEverything() {
    ground.Type = WALL;
    axes temp;
    temp = {-20, -20, -BALL_RADIUS};
    ground.corners[0] = temp;
    temp = {-20, 20, -BALL_RADIUS};
    ground.corners[1] = temp;
    temp = {20, 20, -BALL_RADIUS};
    ground.corners[2] = temp;
    temp = {20, -20, -BALL_RADIUS};
    ground.corners[3] = temp;
    ground.color[0] = 1 / 255.0;
    ground.color[1] = 142 / 255.0;
    ground.color[2] = 14 / 255.0;


    poles[0].Type = L_POLE;
    poles[0].height = POLE_HEIGHT;
    poles[1].Type = U_POLE;
    poles[1].height = POLE_LENGTH / 2;
    poles[2].Type = R_POLE;
    poles[2].height = POLE_HEIGHT;

    temp = {0, 0, 0};
    aimArrow.start = temp;
    temp = {0, 2, 0};
    aimArrow.finish = temp;

    aimArrow.width = 0.2;


    aimArrow.zAngle = 0.0;
    aimArrow.yAngle = 0.0;
    aimArrow.length = 2.0;

    aimArrow.color[0] = 50 / 255.0;
    aimArrow.color[1] = 50 / 255.0;
    aimArrow.color[2] = 127 / 255.0;
    aimArrow.color[3] = 1.0;


    defender.color[0] = 250 / 255.0;
    defender.color[1] = 100 / 255.0;
    defender.color[2] = 100 / 255.0;
    defender.color[3] = 1.0;

    defender.width = DEFENDER_WIDTH;
    defender.height = 2.3;

    defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED;
    defender.state.positionCurrent.x = 0.0;


    sphereCamera.xAngle = -90.0f;
    sphereCamera.zAngle = 15.0f;
    sphereCamera.distance = 5.0;
    sphereCamera.distance = 10;
    for (int i = 0; i < 3; ++i) {
        sphere.positionCurrent[i] = sphere.velocityCurrent[i] = 0;
    }
    powerMeter = 0.0;
    currentMode = ADJUSTING;

    sphere.positionInitial.x = sphere.positionCurrent.x = 0.0;
    sphere.velocityCurrent[0] = sphere.velocityInitial[0] = 0;
    sphere.accelerationCurrent[2] = 0;
    toLookAt = sphere.positionCurrent;

    sphere.elasticity = BALL_ELASTICITY;
    stopEverything = false;

    if (determineSphere) {
        delete determineSphere;
        determineSphere = NULL;
    }
    system("paplay resources/whistle.wav &");
}

/**
 * The function `initialiseEverythingCallback` sets a flag to stop everything and then initializes
 * everything.
 * 
 * @param _ The parameter `_` in the `initialiseEverythingCallback` function is of type `int`. It is
 * not being used within the function, as indicated by the underscore character being used as the
 * variable name.
 */
void initialiseEverythingCallback(int _) {
    stopEverything = true;
    initialiseEverything();
}

/**
 * The function `drawGoalPost` is used to draw a goal post with three poles at specified positions.
 */
void drawGoalPost() {

    {
        glPushMatrix();
        glTranslated(GOAL_POST_X - POLE_LENGTH / 2 + POLE_RADIUS, GOAL_POST_Y + 0, 0 - BALL_RADIUS);
        poles[0].draw();
        glPopMatrix();

        glPushMatrix();
        glTranslated(GOAL_POST_X + 0, GOAL_POST_Y + 0, POLE_HEIGHT + POLE_RADIUS - BALL_RADIUS);
        poles[1].draw();
        glPopMatrix();


        glPushMatrix();
        glTranslated(GOAL_POST_X + POLE_LENGTH / 2 - POLE_RADIUS, GOAL_POST_Y + 0, 0 - BALL_RADIUS);
        poles[2].draw();
        glPopMatrix();
    }

}
/**
 * The function `drawBillboard3D` in C++ draws a 3D billboard with a metallic frame, screen, support
 * legs, and text labels for goals and tries.
 * 
 * @param text The `text` parameter in the `drawBillboard3D` function is a string that represents the
 * content or message that will be displayed on the billboard in the 3D scene.
 * @param x The `x` parameter in the `drawBillboard3D` function represents the x-coordinate of the
 * position where the 3D billboard will be drawn in the scene. It determines the horizontal position of
 * the billboard within the 3D space.
 * @param y The `y` parameter in the `drawBillboard3D` function represents the vertical position of the
 * billboard in the 3D space. It determines how high or low the billboard will be placed relative to
 * the ground or other objects in the scene. Adjusting the `y` value will move
 * @param z The `z` parameter in the `drawBillboard3D` function represents the position of the
 * billboard along the z-axis in a 3D space. This parameter determines how far forward or backward the
 * billboard will be placed relative to the viewer's perspective. Adjusting the `z` value allows
 */
void drawBillboard3D(std::string text, float x, float y, float z) {
    glPushMatrix();

    // Position
    glTranslatef(x, y, z);

    // Slight backward tilt (realistic)
    glRotatef(-5, 1, 0, 0);

    float width  = 5.0;
    float height = 2.5;

    // =====================
    // FRAME (metallic look)
    // =====================
    glDisable(GL_DEPTH_TEST);

    float thickness = 0.2;

    glColor3f(0.75, 0.75, 0.78);
    glBegin(GL_QUADS);
    glVertex3f(-width, -thickness, -height);
    glVertex3f(width, -thickness, -height);
    glVertex3f(width, thickness, height);
    glVertex3f(-width, thickness, height);
    glEnd();

    // =====================
    // SCREEN
    // =====================
    glColor3f(0.02, 0.02, 0.02);

    glBegin(GL_QUADS);
    glVertex3f(-width * 0.92, 0.01, -height * 0.75);
    glVertex3f(width * 0.92, 0.01, -height * 0.75);
    glVertex3f(width * 0.92, 0.01, height * 0.75);
    glVertex3f(-width * 0.92, 0.01, height * 0.75);
    glEnd();

    glEnable(GL_DEPTH_TEST);

    // =====================
    // SUPPORT LEGS
    // =====================
    //glEnable(GL_LIGHTING);
    glColor3f(0.7, 0.7, 0.72);

    GLUquadric* quad = gluNewQuadric();

    // Left leg
    glPushMatrix();
    glTranslatef(-width * 0.6, 0, -height);
    glRotatef(180, 1, 0, 0);
    gluCylinder(quad, 0.3, 0.3, 3.0, 20, 20);

    // base plate
    glTranslatef(0, 0, 3.0);
    gluDisk(quad, 0.0, 0.3, 20, 1);
    glPopMatrix();

    // Right leg
    glPushMatrix();
    glTranslatef(width * 0.6, 0, -height);
    glRotatef(180, 1, 0, 0);
    gluCylinder(quad, 0.3, 0.3, 3.0, 20, 20);

    // base plate
    glTranslatef(0, 0, 3.0);
    gluDisk(quad, 0.0, 0.3, 20, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);

    // =====================
    // TEXT (centered)
    // =====================
    glDisable(GL_LIGHTING);

    float lineSpacing = height * 0.6;
    float centerOffset = lineSpacing / 2.0;

    // GOALS LABEL
    glPushMatrix();
    currentTextColor = {0.1, 0.8, 0.2, 1.0};
    glTranslatef(-width * 0.7, 0.03, centerOffset);
    glScalef(0.8, 0.8, 0.8);
    writeText("GOALS", font, LEFT);
    glPopMatrix();

    // GOALS VALUE
    glPushMatrix();
    currentTextColor = {0.2, 0.9, 0.3, 1.0};
    glTranslatef(width * 0.6, 0.03, centerOffset);
    glScalef(0.8, 0.8, 0.8);
    writeText(std::to_string(Goals), font, RIGHT);
    glPopMatrix();

    // TRIES LABEL
    glPushMatrix();
    currentTextColor = {0.2, 0.5, 1.0, 1.0};
    glTranslatef(-width * 0.7, 0.03, -centerOffset);
    glScalef(0.8, 0.8, 0.8);
    writeText("TRIES", font, LEFT);
    glPopMatrix();

    // TRIES VALUE
    glPushMatrix();
    currentTextColor = {0.3, 0.6, 1.0, 1.0};
    glTranslatef(width * 0.6, 0.03, -centerOffset);
    glScalef(0.8, 0.8, 0.8);
    writeText(std::to_string(Tries), font, RIGHT);
    glPopMatrix();

    glEnable(GL_LIGHTING);

    glPopMatrix();
}

/**
 * The function `cameraPosition` sets the camera position based on a specified point, distance, and
 * angles.
 * 
 * @param point The `point` parameter represents the coordinates of the point you want the camera to
 * look at. It consists of `x`, `y`, and `z` coordinates in 3D space.
 * @param distance The `distance` parameter in the `cameraPosition` function represents the distance
 * from the `point` in 3D space where the camera will be positioned. This distance determines how far
 * the camera will be from the point of interest.
 * @param zAngle The `zAngle` parameter represents the angle in degrees for the rotation around the
 * z-axis. It is used to calculate the new camera position based on the distance from the point along
 * with the x and z angles.
 * @param xAngle The `xAngle` parameter represents the angle in degrees for the rotation around the
 * x-axis. It is used in the `cameraPosition` function to calculate the new camera position based on
 * the distance and angles provided.
 */
void cameraPosition(axes point, double distance, double zAngle, double xAngle) {
    gluLookAt(point.x + distance * (cos(DEG2GRAD(zAngle)) * cos(DEG2GRAD(xAngle))),
              point.y + distance * (cos(DEG2GRAD(zAngle)) * sin(DEG2GRAD(xAngle))),
              point.z + distance * sin(DEG2GRAD(zAngle)), point.x, point.y, point.z, 0, 0, 1);

}

/**
 * The `camera` constructor initializes the `zAngle`, `xAngle`, and `distance` variables for a camera
 * object.
 */
camera::camera() {
    zAngle = xAngle = 0.0;
    distance = 5.0;
}

camera sphereCamera;


/**
 * The function `rainBox` draws a colored box with specified transparency using OpenGL in C++.
 * 
 * @param alpha The `alpha` parameter in the `rainBox` function is a value that controls the
 * transparency of the colors used to draw the box. It ranges from 0.0 (completely transparent) to 1.0
 * (completely opaque). By default, the `alpha` value is set
 */
void rainBox(double alpha = 0.7) {

    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.0f, 0.0, alpha); //RED
    glVertex2f(-10.0, -20.0);
    glVertex2f(10.0, -20.0);

    glColor4f(1.0f, 1.0f, 0.0, alpha); //YELLOW
    glVertex2f(10.0, -10.0);
    glVertex2f(-10.0, -10.0);

    glVertex2f(-10.0, -10.0);
    glVertex2f(10.0, -10.0);


    glColor4f(0.0f, 1.0f, 0.0, alpha); //GREEN
    glVertex2f(10.0, 0.0);
    glVertex2f(-10.0, 0.0);
    glEnd();
}

/**
 * The function myShear applies a shear transformation using a specified matrix in C++.
 */
void myShear() {
//    glRotatef(-45, 0.0, 0.0, 1.0);
    float m[] = {
            1.0, 0.0, 0.0, 0.0,
            1.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
    };
    glMultMatrixf(m);
}

double powerMeter = 0.0;

/**
 * The function `drawPowerMeter` in C++ uses OpenGL to draw a power meter with a variable power level.
 */
void drawPowerMeter() {


    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);

    glTranslatef(-10.0, -20.0 + powerMeter * 40, 0.0);

    glColor4f(0.1, 0.1, 0.1, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-10.0, -0.2);
    glVertex2f(5.0, -0.2);
    glVertex2f(5.0, 0.2);
    glVertex2f(-10.0, 0.2);
    glEnd();
    glBegin(GL_TRIANGLES);

    glVertex2f(5.0, -0.4);
    glVertex2f(8.0, 0.0);
    glVertex2f(5.0, 0.4);


    glEnd();

    glPopAttrib();
    glPopMatrix();
}


/**
 * The `drawHUD` function in C++ is responsible for rendering the Heads-Up Display (HUD) elements,
 * including instructions for gameplay and power meter display.
 */
void drawHUD() {
    glDisable(GL_LIGHTING);
    if (currentMode == HELP) {

        const char *instructions = R"INSTRUCT(
INSTRUCTIONS
You can use the mouse to look around.
Use the +/- keys for zooming in/out.

AIMING
Press the Enter/Return key to enter Aiming Mode.
Direct the arrow with the arrow keys to set up the
direction of the shot.

POWERING
Press and hold Space after aiming to power up.
Release Space to select the specified power level.
Press the ESC key (when holding down SPACE) to cancel
POWERING mode.

Press ESC key to return to the previous mode or to exit the
instructions.
Press Q at any time to exit the game.
)INSTRUCT";

        glPushMatrix();
        glRotatef(90 + sphereCamera.xAngle, 0, 0, 1);
        glRotatef(-sphereCamera.zAngle, 1, 0, 0);

        glTranslatef(0, -BALL_RADIUS, -BALL_RADIUS);

        glColor4f(0, 0, 0, 0.8);
        glBegin(GL_QUADS);
        glVertex3f(-10, 0, -5);
        glVertex3f(10, 0, -5);
        glVertex3f(10, 0, 6);
        glVertex3f(-10, 0, 6);
        glEnd();
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(0, -0.001, 9.5);

        currentTextColor = {1.0, 1.0, 1.0, 1.0};
        writeMultiLineText(instructions, font, CENTER);
        glPopMatrix();
    } else {   //HUD Render

        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(-100.0, 100.0, -100.0, 100.0);       //glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT)
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
//        glDisable(GL_CULL_FACE);

        glClear(GL_DEPTH_BUFFER_BIT);


        if (currentMode == POWERING) {   //Power Bar
            glPushMatrix();

            glTranslatef(90, 0, 0);


            glScalef(0.3, 4.0, 1.0);

            drawPowerMeter();


            rainBox();
            glScalef(1.0, -1.0, 0.0);
            rainBox();

            glPopMatrix();
        }


        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPopAttrib();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

    }
    glEnable(GL_LIGHTING);
}
/**
 * The function `updateDefenderPosition` updates the position and velocity of a defender in a game
 * based on certain conditions and modes.
 * 
 * @param _ The parameter `_` in the `updateDefenderPosition` function is of type `int`. It is not
 * being used within the function, so it seems to be a placeholder variable that is not currently
 * serving any purpose in the code snippet provided.
 */
void updateDefenderPosition(int _) {

    static float increment = 2.0f;
    static int done = 0;
    defender.armRot += increment;

    if (defender.armRot > 100.0 || defender.armRot < 0.0) {
        increment *= -1;
    }

    defender.state.timePassed += 1 / 60.0;
    double t = 1 / 60.0;
    defender.acceleration();

    if (currentMode == SHOOTING) {
        if (!done) {
//            cout<<sphere.velocityCurrent.x<<endl;
            if (sphere.velocityCurrent.x < 0) {
                defender.state.velocityCurrent.x = -DEFENDER_SPEED;
                done = 1;
            } else if (sphere.velocityCurrent.x > 0) {
                defender.state.velocityCurrent.x = DEFENDER_SPEED;
                done = 1;
            } else {
                defender.state.velocityCurrent.x = 0;
                done = 1;
            }
        }
        for (int i = 0; i < 3; ++i) {
            defender.state.positionCurrent[i] =
                    defender.state.velocityCurrent[i] * t + 0.5 * defender.state.accelerationCurrent[i] * t * t +
                    defender.state.positionCurrent[i];
            defender.state.velocityCurrent[i] =
                    defender.state.velocityCurrent[i] + defender.state.accelerationCurrent[i] * t;
        }
//    if (currentMode != NONE && currentMode != GOAL_ANIMATION){
//    }
    }
    else{
        done=0;
    };
    glutTimerFunc(1000 * 1 / 60.0, updateDefenderPosition, 1 / 60.0);
}


/**
 * The function `convertToTexture` reads color data from a text file, converts it to a texture format,
 * and writes it to a binary file with a `.tx` extension.
 * 
 * @param filename The `convertToTexture` function takes a `const char*` parameter `filename`, which is
 * the name of the file to be read and converted to a texture file. The function reads the input file,
 * processes the data, and writes the texture data to a new file with a ".tx"
 * 
 * @return The function `convertToTexture` returns an integer value. If the file specified by the
 * `filename` parameter cannot be opened, the function returns -1. Otherwise, it returns 0 after
 * processing the file and writing the texture data to a new file with a ".tx" extension.
 */
int convertToTexture(const char *filename) {
    ifstream textFile(filename);
    string destination(filename);
    destination += ".tx";
    if (!textFile) {
        cout << "File cannot be opened!" << endl;
        return -1;
    }
    unsigned color;
    vector<unsigned char> text;
    int w, h;
    textFile >> w >> h;
    while (textFile >> color) {
        text.push_back((color & 0xff000000) >> 24);
        text.push_back((color & 0x00ff0000) >> 16);
        text.push_back((color & 0x0000ff00) >> 8);
        text.push_back((color & 0x000000ff) >> 0);
    }
    unsigned char *colors = &text[0];
    ofstream dest(destination.c_str(), ios::out | ios::binary);
    unsigned int size = text.size();
    dest.write((char *) colors, text.size());
    dest.write((char *) &w, sizeof(w));
    dest.write((char *) &h, sizeof(h));
    dest.close();
    return 0;
}

/**
 * The function `loadTextureFile` loads a texture from a file and creates an OpenGL texture object.
 * 
 * @param filename The `filename` parameter in the `loadTextureFile` function is a pointer to a
 * constant character array that represents the path to the texture file that you want to load as a
 * texture in your OpenGL application. This function reads the texture data from the specified file and
 * creates an OpenGL texture object from it
 * 
 * @return The function `loadTextureFile` returns a `GLuint` which represents the texture that has been
 * loaded from the file specified by the `filename` parameter.
 */
GLuint loadTextureFile(const char *filename) {

    GLuint texture;
    glEnable(GL_DEPTH_TEST);
    ifstream textureFile(filename, ios::ate | ios::binary);
    int size = textureFile.tellg();
    size -= 2 * sizeof(int);  //widht, height stored at the end.
    textureFile.close();
    int w, h;
    textureFile.open(filename, ios::in | ios::binary);

    unsigned char *texel = new unsigned char[size];
    textureFile.read((char *) texel, size);
    textureFile.read((char *) &w, sizeof(w));
    textureFile.read((char *) &h, sizeof(h));
    assert(w * h == size / 4);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texel[0]);
    delete[] texel;
    return texture;
}

const float chalkwidth = POLE_RADIUS;

/**
 * The function `chalkHRectangle` draws a horizontal rectangle using OpenGL with specified start and
 * end points.
 * 
 * @param start It seems like the description of the `start` parameter is missing in your message.
 * Could you please provide more information about the `start` parameter so that I can assist you
 * further with the `chalkHRectangle` function?
 * @param end The `end` parameter in the `chalkHRectangle` function represents the end point of the
 * horizontal rectangle to be drawn. It is of type `axes`, which likely contains the x and y
 * coordinates of the end point on the screen or in the coordinate system being used.
 */
void chalkHRectangle(axes start, axes end) {
    glBegin(GL_QUADS);
    glVertex3f(start.x, start.y - chalkwidth / 2.0, 0);
    glVertex3f(start.x, start.y + chalkwidth / 2.0, 0);
    glVertex3f(end.x, end.y + chalkwidth / 2.0, 0);
    glVertex3f(end.x, end.y - chalkwidth / 2.0, 0);
    glEnd();
}

/**
 * The function `chalkVRectangle` draws a rectangle using OpenGL with specified start and end points.
 * 
 * @param start The `start` parameter represents the starting point of the rectangle where you want to
 * draw the chalk line. It contains the x and y coordinates of the starting point.
 * @param end The `end` parameter represents the end point of the rectangle you want to draw. It is a
 * structure or class that contains the x and y coordinates of the endpoint.
 */
void chalkVRectangle(axes start, axes end) {
    glBegin(GL_QUADS);
    glVertex3f(start.x - chalkwidth / 2.0, start.y, 0);
    glVertex3f(start.x + chalkwidth / 2.0, start.y, 0);
    glVertex3f(end.x + chalkwidth / 2.0, end.y, 0);
    glVertex3f(end.x - chalkwidth / 2.0, end.y, 0);
    glEnd();
}

/**
 * The function `drawChalkLines` in C++ uses OpenGL to draw chalk lines on a 3D scene with specified
 * dimensions and positions.
 */
void drawChalkLines() {
    glPushAttrib(GL_LINE_WIDTH);
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();

    glColor3f(0.9, 0.9, 0.9);
    glTranslatef(0, 0, 0.0001 - BALL_RADIUS);
//    glLineWidth(POLE_RADIUS / 1.5);
    GLUquadric *quad = gluNewQuadric();
    gluDisk(quad, 0, chalkwidth / 1.5, 25, 6);
    gluDeleteQuadric(quad);
    axes start = {ground.corners[1].x + 2, GOAL_POST_Y, 0};
    axes end = {ground.corners[2].x - 2, GOAL_POST_Y, 0};
    chalkHRectangle(start, end);
    start.y = -start.y;
    end.y = -end.y;
    chalkHRectangle(start, end);
    start = {ground.corners[1].x + 2, GOAL_POST_Y + chalkwidth / 2.0, 0};
    end = {ground.corners[0].x + 2, -GOAL_POST_Y - chalkwidth / 2.0, 0};
    chalkVRectangle(start, end);
    start.x = -start.x;
    end.x = -end.x;
    chalkVRectangle(start, end);
    glPopMatrix();
    glPopAttrib();
    glPopAttrib();
}

/**
 * The function converts an image file to a texture format and then loads the texture file.
 * 
 * @param filename The `filename` parameter is a C-style string that represents the path to the image
 * file that you want to convert and load as a texture.
 * 
 * @return The function `convertAndLoadTexture` returns a `GLuint` value, which is the result of
 * loading a texture file after converting the input filename to a texture format.
 */
GLuint convertAndLoadTexture(const char *filename) {
    convertToTexture(filename);
    string dest(filename);
    dest += ".tx";
    return loadTextureFile(dest.c_str());
}

  GLuint groundTexture, defenderTexture, leftArm, rightArm, font,crowdTexture, sideTexture, ads; 

/**
 * The function `start2DTexture` sets up a 2D texture with specific parameters and options for
 * rendering in OpenGL, including handling lighting and transparency.
 * 
 * @param texture The `texture` parameter in the `start2DTexture` function is of type `GLuint` and
 * represents the ID of the 2D texture that you want to work with in OpenGL. This ID is typically
 * generated using `glGenTextures` and then bound to a texture using `glBind
 * @param lightingDisabled The `lightingDisabled` parameter is a boolean flag that indicates whether
 * lighting should be disabled when rendering the 2D texture. If `lightingDisabled` is set to `true`,
 * then lighting will be disabled; otherwise, lighting will remain enabled.
 */
void start2DTexture(GLuint texture, bool lightingDisabled) {

    glPushAttrib(GL_CURRENT_BIT);
    if (lightingDisabled)
        glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    //decomm  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
///decomm glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
//    glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
//    glEnable(GL_TEXTURE_GEN_T);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0);//Replace this alpha for transparency
}

/**
 * The function `end2DTexture` disables 2D texturing and performs necessary OpenGL state changes,
 * optionally enabling lighting if disabled.
 * 
 * @param lightingDisabled The parameter `lightingDisabled` is a boolean variable that indicates
 * whether lighting is disabled or not. If `lightingDisabled` is true, then lighting will be enabled
 * using `glEnable(GL_LIGHTING)`.
 */
void end2DTexture(bool lightingDisabled) {

    if (lightingDisabled)
        glEnable(GL_LIGHTING);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopAttrib();
    glDepthMask(GL_TRUE);
}
 
/**
 * The function `drawAudience` in C++ uses OpenGL to draw a 3D audience area with back, left, and right
 * walls, applying textures and offsets for visual effects.
 */
void drawAudience() {
    float width = 25.0;
    float height = 12.0;
    float depth = 13.0;
    float eps = 1.8;   // small overlap fix to align border ads and both left and right walls
    
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

    // ================= BACK WALL =================
    start2DTexture(crowdTexture, true);

    glPushMatrix();
    glTranslatef(0, GOAL_POST_Y + depth, -BALL_RADIUS);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-width, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(width, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(width, 0, height);
    glTexCoord2f(0, 0); glVertex3f(-width, 0, height);
    glEnd();

    glPopMatrix();
    end2DTexture(true);


    // ================= LEFT WALL =================
    glPushMatrix();
    start2DTexture(sideTexture, true);

    glBegin(GL_QUADS);

   glTexCoord2f(0,1); glVertex3f(width - eps, GOAL_POST_Y + depth + eps, -BALL_RADIUS);
    glTexCoord2f(1,1); glVertex3f(width - eps, GOAL_POST_Y - depth - eps, -BALL_RADIUS);
    glTexCoord2f(1,0); glVertex3f(width - eps, GOAL_POST_Y - depth - eps, height - BALL_RADIUS);
    glTexCoord2f(0,0); glVertex3f(width - eps, GOAL_POST_Y + depth + eps, height - BALL_RADIUS);

    glEnd();

    end2DTexture(true);
    glPopMatrix();


    // ================= RIGHT WALL =================
    glPushMatrix();
    start2DTexture(sideTexture, true);

    glBegin(GL_QUADS);

     glTexCoord2f(0,1); glVertex3f(-width + eps, GOAL_POST_Y + depth + eps, -BALL_RADIUS);
    glTexCoord2f(1,1); glVertex3f(-width + eps, GOAL_POST_Y - depth - eps, -BALL_RADIUS);
    glTexCoord2f(1,0); glVertex3f(-width + eps, GOAL_POST_Y - depth - eps, height - BALL_RADIUS);
    glTexCoord2f(0,0); glVertex3f(-width + eps, GOAL_POST_Y + depth + eps, height - BALL_RADIUS);
    glEnd();

    end2DTexture(true);
    glPopMatrix();

    glDisable(GL_POLYGON_OFFSET_FILL);
}

/**
 * The function `writeMultiLineText` reads a multi-line text input, splits it into individual lines,
 * and writes each line with a specified alignment and texture.
 * 
 * @param text The `text` parameter is a string that contains multiple lines of text separated by
 * newline characters.
 * @param texture The `texture` parameter in the `writeMultiLineText` function is an integer that
 * represents the texture to be used for rendering the text. This texture could be an image or a
 * texture object that contains the characters or symbols needed for displaying the text.
 * @param align The `align` parameter in the `writeMultiLineText` function is used to specify the
 * alignment of the text within the rendering area. It is of type `alignment`, which is likely an enum
 * or a custom data type that defines different alignment options such as left, center, or right
 * alignment for
 * 
 * @return The function `writeMultiLineText` is returning a floating-point value of 0.0f.
 */
float writeMultiLineText(string text, int texture, alignment align) {
    std::stringstream iss(text);

    while (iss.good()) {
        std::string SingleLine;
        getline(iss, SingleLine, '\n');
        writeText(SingleLine, texture, align);
        glTranslated(0, 0, -1);
    }
    return 0.0f;
}

/**
 * The function `writeText` in C++ renders text on a 2D texture with specified alignment.
 * 
 * @param text The `text` parameter in the `writeText` function is a string that represents the text
 * you want to render on the specified texture.
 * @param texture The `texture` parameter in the `writeText` function is used to specify the texture ID
 * of the 2D texture that will be used for rendering the text. This texture is bound before rendering
 * the text and then unbound after rendering is complete.
 * @param align The `align` parameter in the `writeText` function specifies the alignment of the text
 * being rendered. It can have one of the following values:
 * 
 * @return The function `writeText` is returning a float value, which is calculated as `(2 * w / 128.0)
 * / h`.
 */
float writeText(string text, int texture, alignment align) {
    start2DTexture(texture);
    glColor4fv(&currentTextColor[0]);
    int w, h;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
    float translatex = (w / 128.0) / h, translatey = 0, translatez = 0;
    //    glPushMatrix();
//    glPushAttrib(GL_CURRENT_BIT);
//
//
//    glColor4fv(color);
    if (align == CENTER) {
//        float transx = text.size()*translatex/2.0;
        glTranslated(0 - ((text.size()) * translatex / 2.0), 0, 0);
    } else if (align == RIGHT) {
        float transx = text.size() * translatex;
        glTranslated(0 - ((text.size()) * translatex / 1.0), 0, 0);
    }
    for (char chr: text) {
        glBegin(GL_QUADS);
        glTexCoord2f(chr / 128.0, 1);
        glVertex3f(0, 0, 0);

        glTexCoord2f((chr + 1) / 128.0, 1);
        glVertex3f((translatex), 0, 0);

        glTexCoord2f((chr + 1) / 128.0, 0);
        glVertex3f((translatex), 0, 1);

        glTexCoord2f(chr / 128.0, 0.0);
        glVertex3f(0, 0, 1);

        glEnd();

        glTranslatef(translatex, translatey, translatez);
    }

    glEnd();

    end2DTexture();
    return (2 * w / 128.0) / h;
}


int textRotX;

/**
 * The function `rotateMsg` rotates a message by 3 degrees in the X-axis until it completes a full
 * rotation of 360 degrees.
 * 
 * @param _ The parameter `_` in the `rotateMsg` function is an integer variable that is not being used
 * within the function. It is simply there as a placeholder for an integer argument, but it is not
 * utilized in the function logic.
 */
void rotateMsg(int _) {
    textRotX = (textRotX + 3) % 360;

    if (textRotX != 0) {
        glutTimerFunc(1, rotateMsg, 0);
    }
}


/**
 * The function `showMsg` in C++ displays a message in a 3D environment based on certain conditions.
 */
void showMsg() {
    glPushMatrix();

    float col[] = {132 / 255.0, 121 / 255.0, 150 / 255.0, 0.7};
//    float col[] = {1,0,0,1};

    float distance = sphereCamera.distance - 4;

    float colin[] = {1.0, 1.0, 1.0, 0.7};
    glTranslatef(distance * (cos(DEG2GRAD(sphereCamera.zAngle)) * cos(DEG2GRAD(sphereCamera.xAngle))),
                 distance * (cos(DEG2GRAD(sphereCamera.zAngle)) * sin(DEG2GRAD(sphereCamera.xAngle))),
                 distance * sin(DEG2GRAD(sphereCamera.zAngle)));
    glTranslatef(toLookAt.x, toLookAt.y, toLookAt.z);
    glRotatef(90 + sphereCamera.xAngle + textRotX, 0, 0, 1);
    glRotatef(-sphereCamera.zAngle, 1, 0, 0);
    glScalef(0.75, 0.75, 0.75);

    bool toWrite = true;

    string msg = "MISS!";

    currentTextColor = {1.0, 0.3, 0.3, 1};
    if (determineSphere) {
        if (isItGoal(*determineSphere)) {
            msg = "GOAL!";
            currentTextColor = {0.3, 1.0, 0.3, 1};
        }
    }
    if (!determineSphere) {
        msg = "";
        toWrite = false;
    }
    if (toWrite) {
        GLUquadric *quad = gluNewQuadric();
        glPushMatrix();
        glColor4fv(col);
        glScalef(2, 0.5, 1);
        glRotatef(90, 1, 0, 0);
        gluCylinder(quad, 1, 1, 1, 40, 40);
        gluDisk(quad, 0.9, 1, 40, 40);
        glColor4fv(colin);
        gluDisk(quad, 0, 0.9, 40, 40);

        glPopMatrix();

        glPushMatrix();

        glPushMatrix();
        glColor4fv(col);
        glScalef(2, 0.5, 1);
        glTranslatef(0, -1, 0);
        glRotatef(90, 1, 0, 0);
        gluDisk(quad, 0.9, 1, 40, 40);
        glColor4fv(colin);
        gluDisk(quad, 0, 0.9, 40, 40);
        glPopMatrix();
        gluDeleteQuadric(quad);
        glPopMatrix();


        glPushMatrix();
        glTranslatef(0, .001, -0.5);
        glRotatef(180, 0, 0, 1);
        writeText(msg, font, CENTER);

        glPopMatrix();


        glPushMatrix();
        glTranslatef(0, -0.501, -0.5);
        writeText(msg, font, CENTER);
        glPopMatrix();

    }
    glPopMatrix();
}