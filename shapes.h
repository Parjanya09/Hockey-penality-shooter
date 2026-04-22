


#ifndef HOCKEY_PENALTY_SHOOTER_SHAPES_H
#define HOCKEY_PENALTY_SHOOTER_SHAPES_H
#ifndef HOCKEY_PENALTY_SHOOTER_SHAPES_H
#define HOCKEY_PENALTY_SHOOTER_SHAPES_H
#include "functionalities.h"

/*This code snippet defines an enumeration called objectType with four possible values: WALL, R_POLE, L_POLE, and U_POLE.

It also defines a class called 
RealObject
 with a public member variable color of type GLfloat array with a size of 4, and a member variable Type of type objectType.

This code suggests that objectType is used to categorize different types of objects, and 
RealObject
 is a base class for objects of different types, with Type indicating the specific type of the object. The GLfloat array color is likely used to store the color information of the object.
 */
enum objectType{WALL, R_POLE, L_POLE, U_POLE};

class RealObject{
public:
    GLfloat color[4];
    objectType Type;
};

/*FlatSurface: This class is derived from the RealObject
 class. It has an array of axes
 objects called corners and a draw() method.
 */

class FlatSurface: public RealObject{
public:
    axes corners[4];
    void draw();
};
/*PoleSurface: This class is also derived from the RealObject class. It has a constructor, an axes
 object called base, and two double variables called radius and height. It also has a draw()method.
 */
class PoleSurface: public RealObject{
public:
    PoleSurface();
    axes base;
    double radius;
    double height;
    void draw();
};
/*FlatArrow: This class has several public member variables, including width, start, finish, yAngle, zAngle, color, and length. It also has two methods called drawWithPoints() and drawWithAngles().
*/
class FlatArrow{
public:
    double width;
    axes start;
    axes finish;
    double yAngle;
    double zAngle;
    GLfloat color[4];
    double length;
    void drawWithPoints();
    void drawWithAngles();
};
/*
Defender: This class has several public member variables, including width, height, armRot, and color. It also has a 
PhysicalState object called state and two methods called acceleration() and draw()
.*/
class Defender{
public:
    double width;
    double height;
    double armRot;
    GLfloat color[4];

    PhysicalState state;
    void acceleration();
    void draw();
};

extern FlatArrow aimArrow;


extern FlatSurface ground;

extern PoleSurface poles[3];
extern RealObject allObjects[];
extern Defender defender;

#endif //HOCKEY_PENALTY_SHOOTER_SHAPES_H
#endif //HOCKEY_PENALTY_SHOOTER_SHAPES_H

