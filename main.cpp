
#include <iostream>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "Object.hpp"
#include "Matrix.hpp"
#include "math.h"

// Defining the area of the "screen"
// 150 x 40 -> (75 * 2) x 40
const int SCREEN_WIDTH  = 75;
const int SCREEN_HEIGHT = 60;
std::string Str_Screen = "";

// Declare the initial angle of rotation
int ANGLE = 35;
bool rotateX = true;
bool rotateY = true;
bool rotateZ = true;

// Declare the object(TODO: Add more objects)
Object obj;

// Declare the Projection Matrix
Matrix projected_2d(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

// Declare the Rotation Matrix
Matrix static_matrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
Matrix rotation_X(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
Matrix rotation_Y(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
Matrix rotation_Z(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

// Declare old index to save the indexes to be clear from the screen
std::vector<int> old_index;

const std::string ColorBuffer = "_.,-=+:;cba!?0123456789$W#@N";
int counter = 0;
int min_Depth = 0;
int max_Depth = 0;

// Declaration of the methods
void initialize();
void update();
void render();
void draw();
int map(int x, int output_Start, int output_End, int input_Start, int input_End);

int main(int argc, char const *argv[]) {

    if (!(argc == 3 || argc == 6)) {
        printf("This program requierd 2 params:\n  shape value:\t('cube' size) | ('sphere' radius) | ('cone' height)\nOptional:\nRotation angle: X Y Z\tbool\n\nExample: cube 10 true false true\n");
        return 1;
    }

    // Initialize the object with a specific form
    if (!strcmp(argv[1], "cube")) {
        obj.createCube(std::stoi( argv[2] ));
    }
    else if (!strcmp(argv[1], "sphere")) {
        obj.createSphere(std::stoi( argv[2] ));
    }
    else if (!strcmp(argv[1], "cone")) {
        obj.createCone(std::stoi( argv[2] ));
    }
    else {
        printf("Shape not recognized:\nUse one of the following: cube | sphere | cone\n");
        return 1;
    }

    if (argc == 6) {
        rotateX = abs(strcmp(argv[3], "false"));
        rotateY = abs(strcmp(argv[4], "false"));
        rotateZ = abs(strcmp(argv[5], "false"));
    }

    // Initialize essential variable
    initialize();

    min_Depth = obj.getMinDepth();
    max_Depth = obj.getMaxDepth();

    // Clear screen
    printf("\x1b[2J");
    // Draw cycle(Ctrl + C to stop)
    while(counter < 50) {
        // Set cursor position 0, 0
        printf("\x1b[H");

        update();
        render();
        draw();

        Sleep(100);
    }

    return 0;
}

int map(int x, int output_Start, int output_End, int input_Start, int input_End) {

    float a = ((float)(output_End - output_Start)) / ((float)(input_End - input_Start));
    float b = (float)(x - input_Start);

    int output = output_Start + (a * b);

    if (output < output_Start)
        return output_Start;
    if (output >= output_End)
        return output_End - 1;
    
    return output;
}

void initialize() {
    int desire_Capacity = ((2 * SCREEN_WIDTH) * SCREEN_HEIGHT) + SCREEN_HEIGHT;

    // Reserve a desire capacity
    Str_Screen.reserve( desire_Capacity );

    // Fill the string with white-spaces
    for (int i = 0; i < desire_Capacity; i++)
        Str_Screen.push_back(' ');

    // Put breaklines where correspond
    for (int i = 1; i <= SCREEN_HEIGHT; i++) {
        Str_Screen[(2 * SCREEN_WIDTH * i) + (i - 1)] = '\n';
    }    
}


void update() {
    counter++;

    // Clear the screen and the old_index
    for (int index : old_index) {
        Str_Screen[index] = ' ';
        Str_Screen[index + 1] = ' ';
    }
    old_index.clear();

    // Increase the rotation angle
    ANGLE += 10;
    ANGLE %= 360;

    double rad_angle = (((float)ANGLE) * M_PI) / 180.0;

    rotation_X.updateValues(
        1.0, 0.0, 0.0,
        0.0, cos(rad_angle), -sin(rad_angle),
        0.0, sin(rad_angle),  cos(rad_angle)
    );

    rotation_Y.updateValues(
        cos(rad_angle), 0.0, sin(rad_angle),
        0.0, 1.0, 0.0,
        -sin(rad_angle), 0.0, cos(rad_angle)
    );

    rotation_Z.updateValues(
        cos(rad_angle), -sin(rad_angle), 0.0,
        sin(rad_angle),  cos(rad_angle), 0.0,
        0.0, 0.0, 1.0
    );
}

// Put the corresponding character it´s belonging position
void render() {
    std::vector<std::vector<int>> objectPoints = obj.getPoints();
    int translateCenterX = (SCREEN_WIDTH * 2)  / 2;
    int translateCenterY = SCREEN_HEIGHT / 2;
    char ColorLight = ' ';

    for (std::vector<int> point : objectPoints) {
        
        std::vector<int> resultVector_Rotated = static_matrix.Matrix_Multiplication(point);

        resultVector_Rotated = (rotateY) ? rotation_Y.Matrix_Multiplication(resultVector_Rotated) : resultVector_Rotated;
        resultVector_Rotated = (rotateX) ? rotation_X.Matrix_Multiplication(resultVector_Rotated) : resultVector_Rotated;
        resultVector_Rotated = (rotateZ) ? rotation_Z.Matrix_Multiplication(resultVector_Rotated) : resultVector_Rotated;

        std::vector<int> resultVector_Projected = projected_2d.Matrix_Multiplication(resultVector_Rotated);
        
        int index = (2 * SCREEN_WIDTH * (resultVector_Projected[1] + translateCenterY)) +
            (resultVector_Projected[0] + translateCenterX) + 
            (resultVector_Projected[1] + translateCenterY);

        // std::cout << resultVector_Projected[2] << std::endl;

        // If index is out of bounds : Dont access to the str_screen
        if (index < 0 || index >= Str_Screen.capacity())
            continue;

        ColorLight = ColorBuffer[ map(resultVector_Projected[2], 0, ColorBuffer.size(), min_Depth, max_Depth) ];

        // Save the current index to "erase it" later
        old_index.push_back(index);
        Str_Screen[ index ] = ColorLight;
        Str_Screen[ index + 1 ] = ColorLight;
    }
}

void draw() {
    std::cout << Str_Screen;
}
