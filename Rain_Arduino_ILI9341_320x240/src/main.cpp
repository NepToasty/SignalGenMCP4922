#include <Arduino.h>
#include <Arduino_GFX_Library.h>
Arduino_DataBus *bus = new Arduino_SWSPI(41 /* DC */, 39 /* CS */, 13 /* SCK */, 11 /* MOSI */, 12 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 40 /* RST */);

// Display and Pixel definitions
#define DISPLAYWIDTH 240
#define DISPLAYHEIGHT 320
#define PIXEL_SIZE 10
#define FPS 240

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define BACKGROUND 0x31a6

const float pi = 3.1415927;

int matrix[DISPLAYHEIGHT / PIXEL_SIZE][DISPLAYWIDTH / PIXEL_SIZE];

/*We just hold |DISPLAYWIDTH / PIXEL_SIZE| vectores worth of rain drops
               |DISPLAYWIDTH / PIXEL_SIZE|
*/

float rainX[DISPLAYWIDTH / PIXEL_SIZE];
float rainY[DISPLAYWIDTH / PIXEL_SIZE];

float sinLUT[360];
float cosLUT[360];

float toRad(float deg) // Converts deg to rad
{
  return (deg / 360.0) * 2.0 * pi;
}

void createTrigLUT() // Computes the LUT for sin and cos
{
  for (int i = 0; i < 360; i++)
  {
    sinLUT[i] = sin(toRad(i));
    cosLUT[i] = cos(toRad(i));
  }
}

void initializeMatrix() // initilize the drawing matrix to 0's
{
  for (int y = 0; y < DISPLAYHEIGHT / PIXEL_SIZE; y++)
  {
    for (int x = 0; x < DISPLAYWIDTH / PIXEL_SIZE; x++)
    {
      matrix[y][x] = 0;
    }
  }
}

void initializeRain() // At startup we place the raindrops randomly onto the xy plane
{
  for (int i = 0; i < DISPLAYWIDTH / PIXEL_SIZE; i++)
  {
    rainX[i] = i;
    rainY[i] = random(0, DISPLAYHEIGHT / PIXEL_SIZE);
  }
}

void clearMatrix() // Clear out the matrix
{
  for (int y = 0; y < DISPLAYHEIGHT / PIXEL_SIZE; y++)
  {
    for (int x = 0; x < DISPLAYWIDTH / PIXEL_SIZE; x++)
    {
      matrix[y][x] = 0;
    }
  }
}

void draw() // Draws the points onto the serial port
{
  for (int y = 0; y < DISPLAYHEIGHT / PIXEL_SIZE; y++)
  {
    for (int x = 0; x < DISPLAYWIDTH / PIXEL_SIZE; x++)
    {
      if (matrix[y][x] == 0)
      {
        gfx->fillRect(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, BACKGROUND);
      }
      else
      {
        gfx->fillRect(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, GREEN);
      }
    }
  }
}

void updateRain(uint16_t angleDeg, uint8_t velocity) // Updates each vector //Angle in deg, Velocity
{
  while (angleDeg < 0)
    angleDeg += 360;
  while (angleDeg >= 360)
    angleDeg -= 360;

  // Moves the rain droplet down the xy direction
  float dx = sinLUT[angleDeg] * velocity;
  float dy = cosLUT[angleDeg] * velocity;

  for (int i = 0; i < DISPLAYWIDTH / PIXEL_SIZE; i++) // boundry check
  {
    rainX[i] += dx;
    rainY[i] += dy;

    if (rainY[i] >= DISPLAYHEIGHT / PIXEL_SIZE)
    {
      rainY[i] = 0;
      rainX[i] = random(0, DISPLAYWIDTH / PIXEL_SIZE);
    }

    if (rainX[i] < 0)
    {
      rainX[i] = DISPLAYWIDTH / PIXEL_SIZE - 1;
    }

    if (rainX[i] >= DISPLAYWIDTH / PIXEL_SIZE)
    {
      rainX[i] = 0;
    }
  }
}

void writeRainToMatrix() // marks the points to be drawn
{
  for (int i = 0; i < DISPLAYWIDTH / PIXEL_SIZE; i++)
  {
    int drawX = round(rainX[i]);
    int drawY = round(rainY[i]);

    if (drawX >= 0 && drawX < DISPLAYWIDTH / PIXEL_SIZE && drawY >= 0 && drawY < DISPLAYHEIGHT / PIXEL_SIZE)
    {
      matrix[drawY][drawX] = 1;
    }
  }
}

void setup()
{
  randomSeed(micros());
  createTrigLUT();
  initializeMatrix();
  initializeRain();
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);
  gfx->begin();
  gfx->fillScreen(BACKGROUND);
}

void loop()
{
  uint16_t rainAngle = 290; // 0 = straight down, positive = down-right

  clearMatrix();
  updateRain(rainAngle, 2);
  writeRainToMatrix();
  draw();

  delay(1 / FPS);
}