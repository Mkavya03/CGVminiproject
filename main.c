#include <stdio.h>
#include<string.h>
#include <GL/glut.h>

#define MAX_CAPACITY 10

int MAX_ITEMS = 4;
int shCounter= 0;
int values[] = {10, 40, 30, 50, 20, 10};
int weights[] = {1, 3, 4, 5, 2, 2};

int dp[6 + 1][MAX_CAPACITY + 1];

float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = -5.0f;
float rotateAngleX = 0.0f;
float rotateAngleY = 0.0f;
GLfloat scaleX = 2.5f;
GLfloat scaleY = 2.5f;
GLfloat scaleZ = 1.0f;

GLfloat shearMatrix[16]={
    1.0f,0.0f,0.0f,0.0f,
    0.15f,1.0f,0.0f,0.0f,
    0.15f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f
};
GLfloat invShearMatrix[16]={
    1.0f,0.0f,0.0f,0.0f,
    -0.15f,1.0f,0.0f,0.0f,
    -0.15f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f
};


void knapsack() {
    int i, w;

    for (i = 0; i <= MAX_ITEMS; i++) {
        for (w = 0; w <= MAX_CAPACITY; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (weights[i - 1] <= w)
                dp[i][w] = (values[i - 1] + dp[i - 1][w - weights[i - 1]]) > dp[i - 1][w] ? (values[i - 1] + dp[i - 1][w - weights[i - 1]]) : dp[i - 1][w];
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
}

void drawCuboid() {
    glBegin(GL_QUADS);

    // Front face with knapsack table
    glColor3f(0.14f, 0.29f, 0.33f); // Red
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Left face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Top face
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Bottom face
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}

void drawKnapsackTable() {
   float xOffset = -0.45f;
    float yOffset = -0.45f;
    float tableWidth = 0.7f;
    float tableHeight = 0.7f;
    float tableDepth = 0.02f;

    // Draw gray background table
 /*   glColor3f(0.5f, 0.5f, 0.5f); // Gray
    glBegin(GL_QUADS);
    glVertex3f(xOffset, yOffset, 0.508f);
    glVertex3f(xOffset + tableWidth, yOffset, 0.508f);
    glVertex3f(xOffset + tableWidth, yOffset + tableHeight, 0.508f);
    glVertex3f(xOffset, yOffset + tableHeight, 0.508f);
    glEnd();*/

    // Draw knapsack table
    float cellWidth = tableWidth / MAX_ITEMS;
    float cellHeight = tableHeight / MAX_CAPACITY;

    for (int i = 0; i <= MAX_ITEMS; i++) {
        for (int j = 0; j <= MAX_CAPACITY; j++) {
            float cellX = xOffset + i * cellWidth;
            float cellY = yOffset + j * cellHeight;

            // Determine cell color based on inclusion in knapsack or final answer
            if (i == MAX_ITEMS && j == MAX_CAPACITY)
                glColor3f(1.0f, 0.0f, 0.0f); // Red (highlight final answer cell)
            else if (weights[i - 1] <= j && (values[i - 1] + dp[i - 1][j - weights[i - 1]]) > dp[i - 1][j])
                glColor3f(0.0f, 1.0f, 0.0f); // Green (cell included in knapsack)
            else
                glColor3f(1.0f, 1.0f, 1.0f); // White (regular cell)

            // Draw cell background
            glBegin(GL_QUADS);
            glVertex3f(cellX, cellY, 0.502f);
            glVertex3f(cellX + cellWidth, cellY, 0.502f);
            glVertex3f(cellX + cellWidth, cellY + cellHeight, 0.502f);
            glVertex3f(cellX, cellY + cellHeight, 0.502f);
            glEnd();

            // Draw cell border
            glColor3f(0.0f, 0.0f, 0.0f); // Black
            glLineWidth(1.0f);
            glBegin(GL_LINE_LOOP);
            glVertex3f(cellX, cellY, 0.503f);
            glVertex3f(cellX + cellWidth, cellY, 0.503f);
            glVertex3f(cellX + cellWidth, cellY + cellHeight, 0.503f);
            glVertex3f(cellX, cellY + cellHeight, 0.503f);
            glEnd();

            // Draw cell value
            glColor3f(0.0f, 0.0f, 0.0f); // Black
            char valueText[4];
            sprintf(valueText, "%d", dp[i][j]);
            float textPosX = cellX + cellWidth * 0.5f - 0.03f;
            float textPosY = cellY + cellHeight * 0.5f - 0.02f;
            glRasterPos3f(textPosX, textPosY, 0.504f);
            int len = strlen(valueText);
            for (int k = 0; k < len; k++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, valueText[k]);
            }
        }
    }
}
  //knapsack table as the face of the cube



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    if(shCounter==1){
        shCounter=0;
        glMultMatrixf(shearMatrix);
    }
    else if(shCounter == 2){
        shCounter=0;
        glMultMatrixf(invShearMatrix);
    }
    glPushMatrix();

    glTranslatef(translateX, translateY, translateZ);
    glRotatef(rotateAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateAngleY, 0.0f, 1.0f, 0.0f);
    glScalef(scaleX, scaleY, scaleZ);

    drawCuboid();
    drawKnapsackTable();

    glPopMatrix();
    // glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Translate up
            translateY += 0.1f;
            break;
        case 's': // Translate down
            translateY -= 0.1f;
            break;
        case 'a': // Translate left
            translateX -= 0.1f;
            break;
        case 'd': // Translate right
            translateX += 0.1f;
            break;
        case 'q': // Rotate counterclockwise around X-axis
            rotateAngleX += 5.0f;
            break;
        case 'e': // Rotate clockwise around X-axis
            rotateAngleX -= 5.0f;
            break;
        case 'z': // Scale up
            scaleX += 0.1f;
            scaleY += 0.1f;
            scaleZ += 0.1f;
            break;
        case 'x': // Scale down
            scaleX -= 0.1f;
            scaleY -= 0.1f;
            scaleZ -= 0.1f;
            break;
        case 'y': // Rotate counterclockwise around Y-axis
            rotateAngleY += 5.0f;
            break;
        case 'c': // Rotate clockwise around Y-axis
            rotateAngleY -= 5.0f;
            break;
         case 'r' :	scaleX = -scaleX; break;
         case 'b' : shCounter = 1;   break;
         case 'v' : shCounter = 2;   break;
    }
    glutPostRedisplay();
}

void processMenu(int value)
{
	if(value==100) exit(0);
}

void processVal(int value)
{

}

void ValOfItem1(int value)
{
	switch(value)
	{
		case 20: values[0]=20; knapsack(); break;
		case 50: values[0]=50; knapsack(); break;
	}
	glutPostRedisplay();
}

void noOfItems(int value)
{
	switch(value)
	{
		//case 2:	MAX_ITEMS = 2; break;
		case 3:	MAX_ITEMS = 3; break;
		case 4:	MAX_ITEMS = 4; break;
		case 5:	MAX_ITEMS = 5; break;
        case 6:	MAX_ITEMS = 6; break;
	}
	glutPostRedisplay();
}

void createMenu()
{
	int Item1ID = glutCreateMenu(ValOfItem1);
	glutAddMenuEntry("20",20);
	glutAddMenuEntry("50",50);

	int ValID = glutCreateMenu(processVal);
	glutAddSubMenu("Item 1",Item1ID);
	
	
	int noiID = glutCreateMenu(noOfItems);
	//glutAddMenuEntry("2",2);
	glutAddMenuEntry("3",3);
	glutAddMenuEntry("4",4);
	glutAddMenuEntry("5",5);
	glutAddMenuEntry("6",6);
	glutAddMenuEntry("7",7);
	
	
	glutCreateMenu(processMenu);
	glutAddSubMenu("Change Val",ValID);
	glutAddSubMenu("Change NOI",noiID);
	//glutAddSubMenu("Change Weight",WeightID);
	glutAddMenuEntry("Quit",100);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Knapsack 3D");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glLoadIdentity();
    glutDisplayFunc(display);
    createMenu();
    glutKeyboardFunc(keyboard);
    knapsack();
    glutMainLoop();

    return 0;
}
