#include <GL/glut.h>
#include "shapes.h"
#include <bits/stdc++.h>
#include "functionalities.h"


void FlatSurface::draw() {

    start2DTexture(groundTexture);

    glBegin(GL_QUADS);
     glTexCoord2f(0.0, 0.0);
glVertex3f(corners[0].x, corners[0].y, corners[0].z);

glTexCoord2f(0.0, 1.0);
glVertex3f(corners[1].x, corners[1].y, corners[1].z);

glTexCoord2f(1.0, 1.0);
glVertex3f(corners[2].x, corners[2].y, corners[2].z);

glTexCoord2f(1.0, 0.0);
glVertex3f(corners[3].x, corners[3].y, corners[3].z); 

    glEnd();

    end2DTexture();

    
     start2DTexture(ads);


glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

glColor4f(1,1,1,1);  

for (int i = 0; i < 4; ++i) {

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 1);
    glVertex3f(corners[i].x, corners[i].y, corners[i].z);

    glTexCoord2f(10.0, 1);
    glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z);

    glTexCoord2f(10.0, 0);
    glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z + AD_HEIGHT);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(corners[i].x, corners[i].y, corners[i].z + AD_HEIGHT);

    glEnd();
}

end2DTexture();

}

FlatSurface ground;

void PoleSurface::draw() {
    glPushAttrib(GL_CURRENT_BIT);
    GLUquadric *quadric = gluNewQuadric();


    if (Type == U_POLE) {

        glTranslated(radius, 0.0, radius);
        glRotatef(180, 1, 0, 0);
        {


            glPushMatrix();
            glPushAttrib(GL_CURRENT_BIT);
            glColor4fv(color);
            glTranslated(-radius, 0, radius);
            glRotatef(-90, 0, 1, 0);
            gluCylinder(quadric, radius, radius, height - radius, 20, 10);
            glPopMatrix();

            glTranslated(-height - 2 * radius, 0, 0);
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            myShear();
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0, 1, 0);
            glTranslated(-radius, 0.0, 0.0);
            glColor4fv(color);
            gluCylinder(quadric, radius, radius, 2 * radius, 20, 10);
            glPopMatrix();


            glTranslated(height, 0, 0);
            glScalef(-1.0, 1.0, 1.0);
            glPushMatrix();
            glPushAttrib(GL_CURRENT_BIT);
            glColor4fv(color);
            glTranslated(-radius, 0, radius);
            glRotatef(-90, 0, 1, 0);
            gluCylinder(quadric, radius, radius, height - radius, 20, 10);
            glPopMatrix();

            glTranslated(-height - 2 * radius, 0, 0);
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            myShear();
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0, 1, 0);
            glTranslated(-radius, 0.0, 0.0);
            glColor4fv(color);
            gluCylinder(quadric, radius, radius, 2 * radius, 20, 10);
            glPopMatrix();
        }
    } else if (Type == R_POLE || Type == L_POLE) {
        if (Type == L_POLE) {
            glScalef(-1.0, 1.0, 1.0);
        }
        glRotatef(90, 0, 1, 0);
        glTranslated(0, 0, 2 * radius);
        glRotatef(180, 1, 0, 0);

        glEnd();
        {

            glPushMatrix();
            glPushAttrib(GL_CURRENT_BIT);
            glColor4fv(color);
            glTranslated(0, 0, radius);
            glRotatef(-90, 0, 1, 0);
            gluCylinder(quadric, radius, radius, height, 20, 10);
            glPopMatrix();

            glTranslated(-height - 2 * radius, 0, 0);
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            myShear();
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0, 1, 0);
            glTranslated(-radius, 0.0, 0.0);
            glColor4fv(color);
            gluCylinder(quadric, radius, radius, 2 * radius, 20, 10);
            glPopMatrix();

        }

    }
    gluDeleteQuadric(quadric);
    glPopMatrix();
    glPopAttrib();
}

PoleSurface::PoleSurface() {
    radius = POLE_RADIUS;
    color[0] = color[1] = color[2] = 1.0;
    color[3] = 1.0;
}


void FlatArrow::drawWithPoints() {
    axes vector;
    for (int i = 0; i < 3; ++i) {
        vector[i] = finish[i] - start[i];
    }
    double rot;
    rot = GRAD2DEG(acos((vector.y * vector.y) /
                        (sqrt(vector.y * vector.y) *
                         sqrt(vector.x * vector.x + vector.y * vector.y))));
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();

    glRotatef(-rot, 0, 0, 1);

    rot = GRAD2DEG(acos((vector.x * vector.x + vector.y * vector.y) /
                        (sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z) *
                         sqrt(vector.x * vector.x + vector.y * vector.y))));

    glRotatef(-rot, -vector.y, vector.x, 0);

    glTranslated(start.x, 0, start.z);
    glColor4fv(color);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, start.y, 0);
    glVertex3f(width / 2, start.y, 0);
    glVertex3f(width / 2, finish.y - 2 * width / vector.y, 0);
    glVertex3f(-width / 2, finish.y - 2 * width / vector.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-width, finish.y - 2 * width / vector.y, 0);
    glVertex3f(width, finish.y - 2 * width / vector.y, 0);
    glVertex3f(0, finish.y, 0);
    glEnd();

    glPopMatrix();
    glPopAttrib();

}

void FlatArrow::drawWithAngles() {

    axes vector;
    for (int i = 0; i < 3; ++i) {
        vector[i] = finish[i] - start[i];
    }
    double rot;
    rot = yAngle;
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();

    glRotatef(-rot, 0, 0, 1);

    glPushMatrix();


    rot = zAngle;

    glRotatef(rot, cos(DEG2GRAD(rot)), 0, 0);


    glTranslated(start.x, 0, start.z);
    glColor4fv(color);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, start.y, 0);
    glVertex3f(width / 2, start.y, 0);
    glVertex3f(width / 2, length - 2 * width / vector.y, 0);
    glVertex3f(-width / 2, length - 2 * width / vector.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-width, length - 2 * width / vector.y, 0);
    glVertex3f(width, length - 2 * width / vector.y, 0);
    glVertex3f(0, length, 0);
    glEnd();
    glPopMatrix();


    glTranslated(start.x, 0, start.z);
    glColor4f(0.0, 0.5, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, start.y, -BALL_RADIUS + 0.001f);
    glVertex3f(width / 2, start.y, -BALL_RADIUS + 0.001f);
    glVertex3f(width / 2, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glVertex3f(-width / 2, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-width, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glVertex3f(width, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glVertex3f(0, length - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glEnd();


    glPopMatrix();
    glPopAttrib();
}

void Defender::acceleration() {
    if (this->state.positionCurrent.x >= POLE_LENGTH / 2.0 - this->width / 2 - POLE_RADIUS ||
        this->state.positionCurrent.x <= -POLE_LENGTH / 2.0 + this->width / 2 + POLE_RADIUS) {
        this->state.velocityCurrent.x *= -1;
    }
}

void Defender::draw() {

    glPushMatrix();

    start2DTexture(defenderTexture);

    glTranslatef(defender.state.positionCurrent.x, GOAL_POST_Y - 0.3, (this->height) / 2 - BALL_RADIUS);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1);
    glVertex3f(-this->width / 2, 0, -this->height / 2);
    glTexCoord2f(1, 1);
    glVertex3f(this->width / 2, 0, -this->height / 2);
    glTexCoord2f(1, 0.0);
    glVertex3f(this->width / 2, 0, this->height / 2);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-this->width / 2, 0, this->height / 2);
    glEnd();

    end2DTexture();

    glPopMatrix();



    // LEFT ARM
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();

start2DTexture(leftArm);

// Move to body position
glTranslatef(defender.state.positionCurrent.x,
             GOAL_POST_Y - 0.3,
             (this->height) / 2 - BALL_RADIUS + 0.08);

glTranslatef(
    -this->width * 0.10,
    0,
    this->height / 2 - this->height * 0.23
);

// Rotate around shoulder
glRotatef(armRot, 0, 1, 0);


glBegin(GL_QUADS);

// bottom-left
glTexCoord2f(0, 1); glVertex3f(-this->width * 0.45, 0, -this->height * 0.55);

// bottom-right
glTexCoord2f(1, 1); glVertex3f(0, 0, -this->height * 0.55);

glTexCoord2f(1, 0); glVertex3f(0, 0, 0);

// top-left
glTexCoord2f(0, 0); glVertex3f(-this->width * 0.45, 0, 0);

glEnd();
end2DTexture();

glPopMatrix();



    // RIGHT ARM
    glPushMatrix();

start2DTexture(rightArm);

glTranslatef(defender.state.positionCurrent.x,
             GOAL_POST_Y - 0.3,
             (this->height) / 2 - BALL_RADIUS + 0.08);


glTranslatef(
    this->width * 0.14,
    0,
    this->height / 2 - this->height * 0.23
);

// Rotate opposite direction
glRotatef(-armRot, 0, 1, 0);

// Draw mirrored quad
glBegin(GL_QUADS);

// bottom-left
glTexCoord2f(0, 1); glVertex3f(0, 0, -this->height * 0.50);

// bottom-right
glTexCoord2f(1, 1); glVertex3f(this->width * 0.40, 0, -this->height * 0.50);

// top-right
glTexCoord2f(1, 0); glVertex3f(this->width * 0.40, 0, 0);

// ✅ top-left (pivot)
glTexCoord2f(0, 0); glVertex3f(0, 0, 0);

glEnd();

end2DTexture();

glPopMatrix();
glEnable(GL_DEPTH_TEST);
}

FlatArrow aimArrow;

PoleSurface poles[3];

Defender defender;

RealObject allObjects[] = {poles[0], poles[1], poles[2]};