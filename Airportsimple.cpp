#include <windows.h>
#include <GL/glut.h>
#include <cmath>

float busSpeed = 1.0f;
float busBoostSpeed = 4.0f;
bool busBoost = false;

bool isNight = false;

float landPlaneX = 500.0f;
float landPlaneY = 350.0f;
bool landingStarted = false;

float busX = -500.0f;
float bird2X = -600.0f;
float bird3X = -800.0f;
float birdX = -400.0f;
float birdY = 300.0f;

float radarAngle = 0.0f;
float planeX = -300.0f;
float planeY = 0.0f;
float cloudX = 0.0f;

const int CIRCLE_SEGMENTS = 40;
const float PI_F = 3.1416f;

void resetScene() {
    planeX = -300.0f;
    planeY = 0.0f;
    landPlaneX = 500.0f;
    landPlaneY = 350.0f;
    landingStarted = false;
    cloudX = 0.0f;
    busX = -500.0f;
    birdX = -400.0f;
    bird2X = -600.0f;
    bird3X = -800.0f;
    radarAngle = 0.0f;
    isNight = false;
}

void drawCircle(float cx, float cy, float r) {
    int x = 0;
    int y = r;
    int p = 1 - y;
    glBegin(GL_LINES);
    while (x <= y) {
        glVertex2i(cx - x, cy + y); glVertex2i(cx + x, cy + y);
        glVertex2i(cx - x, cy - y); glVertex2i(cx + x, cy - y);
        glVertex2i(cx - y, cy + x); glVertex2i(cx + y, cy + x);
        glVertex2i(cx - y, cy - x); glVertex2i(cx + y, cy - x);
        if (p < 0) p += 2 * x + 3;
        else { p += 2 * (x - y) + 5; y--; }
        x++;
    }
    glEnd();
}

void br(int x1, int y1, int x2, int y2) {
    float m = float(y2 - y1) / float(x2 - x1);
    int pk, x, y;
    glBegin(GL_POINTS);
    if (m >= 0 && m <= 1) {
        pk = (2 * (y2 - y1)) - (x2 - x1);
        x = x1; y = y1;
        for (int i = 0; i < x2 - x1; i++) {
            glVertex2i(x, y);
            if (pk < 0) { x++; pk += 2 * (y2 - y1); }
            else { pk += 2 * (y2 - y1) - 2 * (x2 - x1); x++; y++; }
        }
    }
    else if (m > 1) {
        pk = (2 * (x2 - x1)) - (y2 - y1);
        x = x1; y = y1;
        for (int i = 0; i < y2 - y1; i++) {
            glVertex2i(x, y);
            if (pk < 0) { y++; pk += 2 * (x2 - x1); }
            else { pk += 2 * (x2 - x1) - 2 * (y2 - y1); x++; y++; }
        }
    }
    else if (m >= -1 && m < 0) {
        pk = (2 * (y1 - y2)) + (x2 - x1);
        x = x1; y = y1;
        for (int i = 0; i < abs(x2 - x1); i++) {
            glVertex2i(x, y);
            if (pk < 0) { pk += 2 * (y1 - y2); x++; }
            else { pk += 2 * (y1 - y2) + 2 * (x2 - x1); x++; y--; }
        }
    }
    else {
        pk = (2 * (x1 - x2)) + (y2 - y1);
        x = x1; y = y1;
        for (int i = 0; i < abs(y2 - y1); i++) {
            glVertex2i(x, y);
            if (pk < 0) { pk += 2 * (x1 - x2); y--; }
            else { pk += 2 * (x1 - x2) + 2 * (y2 - y1); x++; y--; }
        }
    }
    glEnd();
}

void drawSunMoon(float x, float y, float radius) {
    if (isNight) {
        glColor3f(0.9f, 0.9f, 0.95f);
    }
    else {
        glColor3f(1.0f, 0.9f, 0.1f);
    }
    drawCircle(x, y, radius);
}

void drawCloud(float x, float y) {
    if (isNight) {
        glColor4f(1.0f, 1.0f, 0.9f, 1.0f);
    }
    else {
        glColor4f(1.0f, 1.0f, 0.7f, 1.0f);
    }
    drawCircle(x, y, 25.0f);
    drawCircle(x + 20.0f, y + 10.0f, 25.0f);
    drawCircle(x + 40.0f, y, 25.0f);
    drawCircle(x + 60.0f, y + 10.0f, 25.0f);
}

void drawRunway() {
    if (isNight) glColor3f(0.1f, 0.1f, 0.1f);
    else glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(-400.0f, -200.0f); glVertex2f(400.0f, -200.0f);
    glVertex2f(400.0f, -50.0f); glVertex2f(-400.0f, -50.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -350; i <= 350; i += 80) {
        glBegin(GL_QUADS);
        glVertex2f(i, -100.0f); glVertex2f(i + 40.0f, -100.0f);
        glVertex2f(i + 40.0f, -90.0f); glVertex2f(i, -90.0f);
        glEnd();
    }
}

void drawAirplane(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.0f); glVertex2f(120.0f, 0.0f);
    glVertex2f(120.0f, 20.0f); glVertex2f(0.0f, 20.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 20.0f); glVertex2f(-30.0f, 40.0f); glVertex2f(0.0f, 40.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(50.0f, 20.0f); glVertex2f(90.0f, 20.0f); glVertex2f(70.0f, 50.0f);
    glEnd();
    glPopMatrix();
}

void drawAirportTerminal() {
    glColor3f(0.8f, 0.8f, 0.85f);
    glBegin(GL_QUADS);
    glVertex2f(-380.0f, -50.0f); glVertex2f(-120.0f, -50.0f);
    glVertex2f(-120.0f, 120.0f); glVertex2f(-380.0f, 120.0f);
    glEnd();
    glColor3f(0.3f, 0.3f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-390.0f, 120.0f); glVertex2f(-110.0f, 120.0f);
    glVertex2f(-100.0f, 140.0f); glVertex2f(-400.0f, 140.0f);
    glEnd();
    glColor3f(0.3f, 0.6f, 0.9f);
    for (int y = -10; y <= 90; y += 40) {
        for (int x = -360; x <= -150; x += 50) {
            glBegin(GL_QUADS);
            glVertex2f(x, (float)y); glVertex2f(x + 40.0f, (float)y);
            glVertex2f(x + 40.0f, (float)y + 30.0f); glVertex2f(x, (float)y + 30.0f);
            glEnd();
        }
    }
    glColor3f(0.1f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-265.0f, -50.0f); glVertex2f(-220.0f, -50.0f);
    glVertex2f(-220.0f, 20.0f); glVertex2f(-265.0f, 20.0f);
    glEnd();
}

void drawControlTower() {
    glColor3f(0.7f, 0.7f, 0.75f);
    glBegin(GL_QUADS);
    glVertex2f(250.0f, -50.0f); glVertex2f(300.0f, -50.0f);
    glVertex2f(300.0f, 120.0f); glVertex2f(250.0f, 120.0f);
    glEnd();
    int xLeft = 230; int xRight = 320; int yBottom = 120; int yTop = 170;
    glColor3f(0.3f, 0.6f, 0.9f);
    for (int y = yBottom; y <= yTop; y++) br(xLeft, y, xRight, y);
    br(xLeft, yBottom, xRight, yBottom); br(xLeft, yTop, xRight, yTop);
    br(xLeft, yBottom, xLeft, yTop); br(xRight, yBottom, xRight, yTop);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(230.0f, 170.0f); glVertex2f(320.0f, 170.0f); glVertex2f(275.0f, 200.0f);
    glEnd();
}

void drawLandingPlane(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.8f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.0f); glVertex2f(120.0f, 0.0f);
    glVertex2f(120.0f, 20.0f); glVertex2f(0.0f, 20.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(50.0f, 20.0f); glVertex2f(90.0f, 20.0f); glVertex2f(70.0f, 50.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(120.0f, 20.0f); glVertex2f(140.0f, 40.0f); glVertex2f(120.0f, 40.0f);
    glEnd();
    glPopMatrix();
}

void drawAirportBus(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    if (isNight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1.0f, 1.0f, 0.8f, 0.5f);
        glBegin(GL_TRIANGLES);
        glVertex2f(160.0f, 25.0f);
        glVertex2f(250.0f, 60.0f);
        glVertex2f(250.0f, -10.0f);
        glEnd();

        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(160.0f, 10.0f); glVertex2f(165.0f, 10.0f);
        glVertex2f(165.0f, 30.0f); glVertex2f(160.0f, 30.0f);
        glEnd();

        glDisable(GL_BLEND);
    }

    glColor3f(1.0f, 0.7f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f); glVertex2f(160.0f, 0.0f);
    glVertex2f(160.0f, 50.0f); glVertex2f(0.0f, 50.0f);
    glEnd();

    glColor3f(0.2f, 0.6f, 0.9f);
    for (int i = 10; i <= 130; i += 40) {
        glBegin(GL_QUADS);
        glVertex2f(i, 20.0f); glVertex2f(i + 30.0f, 20.0f);
        glVertex2f(i + 30.0f, 45.0f); glVertex2f(i, 45.0f);
        glEnd();
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(40.0f, -5.0f, 12.0f);
    drawCircle(120.0f, -5.0f, 12.0f);

    glPopMatrix();
}

void drawBird(float x, float y, float size) {
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y); glVertex2f(x - size, y + size / 2.0f);
    glVertex2f(x, y); glVertex2f(x + size, y + size / 2.0f);
    glEnd();
}

void drawMountain(float x, float y, float width, float height) {
    glColor3f(0.45f, 0.35f, 0.25f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y); glVertex2f(x + width / 2.0f, y + height); glVertex2f(x + width, y);
    glEnd();
}

void drawTree(float x, float y) {
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - 5.0f, y); glVertex2f(x + 5.0f, y);
    glVertex2f(x + 5.0f, y + 25.0f); glVertex2f(x - 5.0f, y + 25.0f);
    glEnd();
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 20.0f, y + 25.0f); glVertex2f(x + 20.0f, y + 25.0f);
    glVertex2f(x, y + 60.0f);
    glEnd();
}

void drawFence(float startX, float endX, float y) {
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINES);
    glVertex2f(startX, y + 40.0f); glVertex2f(endX, y + 40.0f);
    glEnd();
    for (float x = startX; x <= endX; x += 20.0f) {
        glBegin(GL_LINES);
        glVertex2f(x, y); glVertex2f(x, y + 40.0f);
        glEnd();
    }
}

void drawRadar(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(radarAngle, 0.0f, 0.0f, 1.0f);
    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-50.0f, -5.0f); glVertex2f(50.0f, -5.0f);
    glVertex2f(50.0f, 5.0f); glVertex2f(-50.0f, 5.0f);
    glEnd();
    glPopMatrix();
}

void drawRadarBase(float x, float y) {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x - 20.0f, y); glVertex2f(x + 20.0f, y);
    glVertex2f(x + 20.0f, y + 20.0f); glVertex2f(x - 20.0f, y + 20.0f);
    glEnd();
}

void drawGlow(float x, float y, float r) {
    if (isNight) glColor4f(1.0f, 1.0f, 1.0f, 0.15f);
    else glColor4f(1.0f, 0.9f, 0.1f, 0.25f);
    drawCircle(x, y, r + 20.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (isNight) glColor3f(0.05f, 0.05f, 0.2f);
    else glColor3f(0.5f, 0.8f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(-400.0f, 400.0f); glVertex2f(400.0f, 400.0f);
    glVertex2f(400.0f, -150.0f); glVertex2f(-400.0f, -150.0f);
    glEnd();

    drawMountain(-400.0f, -50.0f, 250.0f, 200.0f);
    drawMountain(-200.0f, -50.0f, 300.0f, 220.0f);
    drawMountain(100.0f, -50.0f, 250.0f, 180.0f);

    for (int i = -350; i <= 350; i += 70) drawTree(i, -50.0f);

    drawFence(-400.0f, 400.0f, -80.0f);
    drawRunway();
    drawAirportTerminal();
    drawControlTower();

    drawSunMoon(300.0f, 300.0f, 25.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawGlow(300.0f, 300.0f, 25.0f);
    drawCloud(cloudX, 250.0f);
    drawCloud(cloudX + 150.0f, 300.0f);
    drawCloud(cloudX - 200.0f, 270.0f);
    glDisable(GL_BLEND);

    if (!isNight) {
        drawBird(bird2X, birdY + 40.0f, 25.0f);
        drawBird(bird3X, birdY + 20.0f, 18.0f);
        drawBird(birdX, birdY, 20.0f);
    }

    drawRadarBase(275.0f, 200.0f);
    drawRadar(275.0f, 220.0f);
    drawAirportBus(busX, -40.0f);

    if (landingStarted) drawLandingPlane(landPlaneX, landPlaneY);
    drawAirplane(planeX, planeY);

    glutSwapBuffers();
}

void update(int value) {
    cloudX += 0.5f;
    if (cloudX > 500.0f) cloudX = -500.0f;
    if (planeX < 200.0f) planeX += 1.5f;
    else { planeY += 1.0f; planeX += 1.0f; }
    if (landingStarted) {
        landPlaneX -= 2.0f; landPlaneY -= 1.2f;
        if (landPlaneY < -70.0f) landPlaneY = -70.0f;
    }
    busX += busSpeed;
    if (busX > 500.0f) busX = -550.0f;
    birdX += 1.2f; if (birdX > 500.0f) birdX = -500.0f;
    bird2X += 1.5f; if (bird2X > 550.0f) bird2X = -600.0f;
    bird3X += 1.0f; if (bird3X > 530.0f) bird3X = -800.0f;
    radarAngle += 3.0f;
    if (radarAngle > 360.0f) radarAngle -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    if (key == 'l' || key == 'L') {
        landingStarted = true;
        landPlaneX = 500.0f; landPlaneY = 350.0f;
    }
    else if (key == 'r' || key == 'R') resetScene();
    else if (key == 'n' || key == 'N') isNight = !isNight;
    else if (key == 27) exit(0);
    else if (key == ' ') busSpeed = busBoostSpeed;
}

void keyboardUp(unsigned char key, int, int) {
    if (key == ' ') busSpeed = 1.0f;
}

void init() {
    gluOrtho2D(-400, 400, -200, 400);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 700);
    glutCreateWindow("2D Airport Runway (Night Mode Enabled)");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
