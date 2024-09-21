#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <vector>  // Для использования std::vector

float angle = 0.0;  // Rotation angle for the cube

// Camera position and orientation
float camX = 0.0f, camY = 0.0f, camZ = 0.0f;  // Initial position of the camera
float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 0.0f;  // Point the camera looks at
float upX = 0.0f, upY = 0.0f, upZ = 0.0f;  // Camera's "up" direction

// Rotation angles for the camera
float angleX = 0.0f, angleY = 0.0f;
float sensitivity = 0.1f;  // Mouse sensitivity

float moveSpeed = 0.5f;  // Movement speed
float rotationSpeed = 1.0f;  // Rotation speed

int WIDTH = 800;
int HEIGHT = 600;

int centerX = WIDTH / 2;
int centerY = HEIGHT / 2;

int cursorCaptured = 1;

clock_t lastFrameTime = 0;
double frameTime = 0.0; // Время между кадрами в секундах
double fps = 0.0;       // Кадры в секунду
int frameCount = 0;     // Количество кадров

void updateFPS() {
    // Получаем текущее время
    clock_t now = clock();
    double elapsedTime = static_cast<double>(now - lastFrameTime) / CLOCKS_PER_SEC;

    frameTime = elapsedTime; // Время между кадрами в секундах

    // Обновляем количество кадров
    frameCount++;

    // Вычисляем FPS каждые 1 секунду
    static double totalElapsed = 0.0;
    totalElapsed += frameTime;
    if (totalElapsed >= 1.0) {
        fps = frameCount / totalElapsed;
        frameCount = 0;
        totalElapsed = 0.0;
    }

    // Обновляем время последнего кадра
    lastFrameTime = now;
}

void printText(float x, float y, const char* string) {
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string);
        string++;
    }
}

std::vector<GLfloat> vertices;
//std::vector<Guint> incides;

GLuint VBO;
void initGLEW() {
    // Генерация буфера
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Указание атрибутов вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void updateVertices() {
    // Изменение данных вершин в C++ (например, сдвиг по оси X)
    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] += 0.01f;  // Смещаем вершины по оси X
    }

    // Обновление данных в VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
}

double scale = 10.0;


void drawcube(double x, double y, double z) {
}

char buffer[100];
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(-1000, 1000, -500, 500, -1000, 1000);

    updateFPS();
    //std::cout << fps << std::endl;
    //printText(-100.0f, 90.0f, "HELLO WORLD!");

    gluLookAt(camX, camY, camZ, eyeX, eyeY, eyeZ, 0.0f, 1.0f, 0.0f);

    updateVertices();

    glDrawArrays(GL_QUADS, 0, vertices.size());

    eyeX = camX + sin(angleX);
    eyeY = camY - tan(angleY);
    eyeZ = camZ - cos(angleX);


    glutPostRedisplay();  // Request display update
    glutSwapBuffers();
}

void capture_mouse() {
    glutSetCursor(GLUT_CURSOR_NONE);  // Скрываем курсор
    cursorCaptured = 1;
    glutWarpPointer(centerX, centerY);  // Центрируем курсор
}

void mouse(int button, int state, int x, int y) {
    if (!cursorCaptured) {
        capture_mouse();
    }

    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {

            } else {

            }

            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {

            } else {

            }

            break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':  // Move forward
            camX += sin(angleX) * moveSpeed;
            camZ -= cos(angleX) * moveSpeed;
            break;
        case 's':  // Move backward
            camX -= sin(angleX) * moveSpeed;
            camZ += cos(angleX) * moveSpeed;
            break;
        case 'a':  // Strafe left
            camX -= cos(angleX) * moveSpeed;
            camZ -= sin(angleX) * moveSpeed;
            break;
        case 'd':  // Strafe right
            camX += cos(angleX) * moveSpeed;
            camZ += sin(angleX) * moveSpeed;
            break;
        case 27:  // Escape key to exit
            if (cursorCaptured) {
                glutSetCursor(GLUT_CURSOR_LEFT_ARROW);  // Устанавливаем курсор в нормальное состояние
                cursorCaptured = 0;
            }
    }
    //glutPostRedisplay();  // Request display update
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:  // Rotate left
            angle -= rotationSpeed * (M_PI / 180);  // Convert to radians
            break;
        case GLUT_KEY_RIGHT:  // Rotate right
            angle += rotationSpeed * (M_PI / 180);  // Convert to radians
            break;
        case GLUT_KEY_UP:  // Move up
            camY += moveSpeed;
            break;
        case GLUT_KEY_DOWN:  // Move down
            camY -= moveSpeed;
            break;
    }
    //glutPostRedisplay();  // Request display update
}

void moution(int x, int y) {
    if (cursorCaptured) {
        static int lastX = -1, lastY = -1;

        if (lastX == -1) {
            lastX = x;
            lastY = y;
        }


        // Calculate the change in mouse position
        int dx = x - lastX;
        int dy = y - lastY;

        // Adjust the rotation angles based on mouse movement
        angleX += dx * sensitivity;
        angleY += dy * sensitivity;

        // Prevent the camera from flipping over by clamping the y-angle
        if (angleY > 1.5f) angleY = 1.5f;
        if (angleY < -1.5f) angleY = -1.5f;

        lastX = x;
        lastY = y;
    }
}

void mouseMotionP(int x, int y) {
    moution(x, y);
}

void mouseMotion(int x, int y) {
    moution(x, y);
}

void timer(int value) {
}


// Функция для добавления координат вершины
void addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void addCubeVertices(float centerX, float centerY, float centerZ, float size) {
    float halfSize = size / 2.0f;

    // Координаты 8 вершин куба
    GLfloat cubeVertices[8][3] = {
        {centerX - halfSize, centerY - halfSize, centerZ - halfSize},  // Нижняя грань - левая передняя
        {centerX + halfSize, centerY - halfSize, centerZ - halfSize},  // Нижняя грань - правая передняя
        {centerX + halfSize, centerY + halfSize, centerZ - halfSize},  // Верхняя грань - правая передняя
        {centerX - halfSize, centerY + halfSize, centerZ - halfSize},  // Верхняя грань - левая передняя
        {centerX - halfSize, centerY - halfSize, centerZ + halfSize},  // Нижняя грань - левая задняя
        {centerX + halfSize, centerY - halfSize, centerZ + halfSize},  // Нижняя грань - правая задняя
        {centerX + halfSize, centerY + halfSize, centerZ + halfSize},  // Верхняя грань - правая задняя
        {centerX - halfSize, centerY + halfSize, centerZ + halfSize}   // Верхняя грань - левая задняя
    };


    // Добавляем вершины для каждой грани куба
    // Передняя грань
    addVertex(cubeVertices[0][0], cubeVertices[0][1], cubeVertices[0][2]);
    addVertex(cubeVertices[1][0], cubeVertices[1][1], cubeVertices[1][2]);
    addVertex(cubeVertices[2][0], cubeVertices[2][1], cubeVertices[2][2]);
    addVertex(cubeVertices[3][0], cubeVertices[3][1], cubeVertices[3][2]);

    // Задняя грань
    addVertex(cubeVertices[4][0], cubeVertices[4][1], cubeVertices[4][2]);
    addVertex(cubeVertices[5][0], cubeVertices[5][1], cubeVertices[5][2]);
    addVertex(cubeVertices[6][0], cubeVertices[6][1], cubeVertices[6][2]);
    addVertex(cubeVertices[7][0], cubeVertices[7][1], cubeVertices[7][2]);

    // Левая грань
    addVertex(cubeVertices[4][0], cubeVertices[4][1], cubeVertices[4][2]);
    addVertex(cubeVertices[0][0], cubeVertices[0][1], cubeVertices[0][2]);
    addVertex(cubeVertices[3][0], cubeVertices[3][1], cubeVertices[3][2]);
    addVertex(cubeVertices[7][0], cubeVertices[7][1], cubeVertices[7][2]);

    // Правая грань
    addVertex(cubeVertices[1][0], cubeVertices[1][1], cubeVertices[1][2]);
    addVertex(cubeVertices[5][0], cubeVertices[5][1], cubeVertices[5][2]);
    addVertex(cubeVertices[6][0], cubeVertices[6][1], cubeVertices[6][2]);
    addVertex(cubeVertices[2][0], cubeVertices[2][1], cubeVertices[2][2]);

    // Нижняя грань
    addVertex(cubeVertices[4][0], cubeVertices[4][1], cubeVertices[4][2]);
    addVertex(cubeVertices[5][0], cubeVertices[5][1], cubeVertices[5][2]);
    addVertex(cubeVertices[1][0], cubeVertices[1][1], cubeVertices[1][2]);
    addVertex(cubeVertices[0][0], cubeVertices[0][1], cubeVertices[0][2]);

    // Верхняя грань
    addVertex(cubeVertices[3][0], cubeVertices[3][1], cubeVertices[3][2]);
    addVertex(cubeVertices[2][0], cubeVertices[2][1], cubeVertices[2][2]);
    addVertex(cubeVertices[6][0], cubeVertices[6][1], cubeVertices[6][2]);
    addVertex(cubeVertices[7][0], cubeVertices[7][1], cubeVertices[7][2]);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Initialize GLUT
    glewExperimental = GL_TRUE;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Enable double buffering, RGB, and depth buffer
    glutInitWindowSize(500, 500);  // Set the window size
    glutCreateWindow("OpenGL Cube with GLUT");  // Create the window with a title

    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering

    if (glewInit() != GLEW_OK) {
        std::cerr << "Не удалось инициализировать GLEW" << std::endl;
        return -1;
    }

    initGLEW();

    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            addCubeVertices(i*10, 0.0f, j*10, 10.0f);
        }
    }

    glutDisplayFunc(display);  // Set the display function
    glutPassiveMotionFunc(mouseMotionP);  // Установка функции для отслеживания движения мыши
    glutMotionFunc(mouseMotion);  // Set the mouse motion function
    glutMouseFunc(mouse);  // Set the keyboard function for normal keys
    glutKeyboardFunc(keyboard);  // Set the keyboard function for normal keys
    glutSpecialFunc(specialKeys);  // Set the keyboard function for special keys
    glutTimerFunc(0, timer, 0);  // Call timer function to control animation

    capture_mouse();

    glutMainLoop();  // Start the GLUT main loop
    return 0;
}


