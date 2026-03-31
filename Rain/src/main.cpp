#include <Arduino.h>

const double pi = 3.141592653589793;

const int xMatrixSize = 300;
const int yMatrixSize = 20;

int matrix[yMatrixSize][xMatrixSize];

float rainX[xMatrixSize];
float rainY[xMatrixSize];

double sinLUT[360];
double cosLUT[360];

double toRad(double deg)
{
    return (deg / 360.0) * 2.0 * pi;
}

void createTrigLUT()
{
    for (int i = 0; i < 360; i++)
    {
        sinLUT[i] = sin(toRad(i));
        cosLUT[i] = cos(toRad(i));
    }
}

void initializeMatrix()
{
    for (int y = 0; y < yMatrixSize; y++)
    {
        for (int x = 0; x < xMatrixSize; x++)
        {
            matrix[y][x] = 0;
        }
    }
}

void initializeRain()
{
    for (int i = 0; i < xMatrixSize; i++)
    {
        rainX[i] = i;
        rainY[i] = random(0, yMatrixSize);
    }
}

void clearMatrix()
{
    for (int y = 0; y < yMatrixSize; y++)
    {
        for (int x = 0; x < xMatrixSize; x++)
        {
            matrix[y][x] = 0;
        }
    }
}

void draw()
{
    for (int y = 0; y < yMatrixSize; y++)
    {
        for (int x = 0; x < xMatrixSize; x++)
        {
            if (matrix[y][x] == 0)
            {
                Serial.print(" ");
            }
            else
            {
                Serial.print("#");
            }
        }
        Serial.println();
    }
    Serial.println();
}

void updateRain(int angleDeg)
{
    while (angleDeg < 0)
        angleDeg += 360;
    while (angleDeg >= 360)
        angleDeg -= 360;

    float dx = sinLUT[angleDeg];
    float dy = cosLUT[angleDeg];

    for (int i = 0; i < xMatrixSize; i++)
    {
        rainX[i] += dx;
        rainY[i] += dy;

        if (rainY[i] >= yMatrixSize)
        {
            rainY[i] = 0;
            rainX[i] = random(0, xMatrixSize);
        }

        if (rainX[i] < 0)
        {
            rainX[i] = xMatrixSize - 1;
        }

        if (rainX[i] >= xMatrixSize)
        {
            rainX[i] = 0;
        }
    }
}

void writeRainToMatrix()
{
    for (int i = 0; i < xMatrixSize; i++)
    {
        int drawX = round(rainX[i]);
        int drawY = round(rainY[i]);

        if (drawX >= 0 && drawX < xMatrixSize && drawY >= 0 && drawY < yMatrixSize)
        {
            matrix[drawY][drawX] = 1;
        }
    }
}

void setup()
{
    while (!Serial)
    {
    }

    randomSeed(micros());

    createTrigLUT();
    initializeMatrix();
    initializeRain();
}

void loop()
{
    int rainAngle = 20; // 0 = straight down, positive = down-right

    clearMatrix();
    updateRain(rainAngle);
    writeRainToMatrix();
    draw();

    delay(20);
}