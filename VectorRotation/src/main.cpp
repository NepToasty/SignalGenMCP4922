#include <Arduino.h>
#include <math.h>

const int matrixSize = 30;
char matrix[matrixSize][matrixSize];

double x = 10.0, y = 5.0;
const double pi = 3.141592653589793;
double rotation = 0.0;

void initializeMatrix() // Initilize the Matrix with #
{
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            matrix[i][j] = '#';
        }
    }
}

void setup()
{
    initializeMatrix();
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
}

void draw() // Draws the matrix onto the serial port
{
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            Serial.print(matrix[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println();
}

void rotationMatrix(double toAddRotation) // Rotates the vector
{
    double x_1, y_1;
    double offset = matrixSize / 2.0;

    int oldX = round(x + offset);
    int oldY = round(y + offset);

    if (oldX >= 0 && oldX < matrixSize && oldY >= 0 && oldY < matrixSize) // Checks whether the vector tip is inside of the matrix
    {
        matrix[oldY][oldX] = '#';
    }
    else
    {
        Serial.println("Array out of bound");
    }

    double step = (toAddRotation / 360.0) * 2.0 * pi; // Converts deg to rad
    rotation += step;
    if (rotation > 2.0 * pi)
    {
        rotation -= 2.0 * pi;
    }

    //Rotates the vector
    x_1 = x * cos(step) - y * sin(step);
    y_1 = x * sin(step) + y * cos(step);

    x = x_1;
    y = y_1;

    int newX = round(x + offset);
    int newY = round(y + offset);

    if (newX >= 0 && newX < matrixSize && newY >= 0 && newY < matrixSize) // Checks whether the vector tip is inside of the matrix
    {
        matrix[newY][newX] = ' ';
    }
    else
    {
        Serial.println("Array out of bound");
    }

    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);
    Serial.print("rotation rad: ");
    Serial.println(rotation);
}

void flushSerial(int linesToFlush) // Flushes the serial port
{
    for (int i = 0; i < linesToFlush; i++)
    {
        Serial.println();
    }
}

void loop()
{
    delay(40);
    flushSerial(40);
    rotationMatrix(8);
    draw();
}