#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();

XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

class Button {
  public:
      int x;
      int y;
      int width;
      int height;

      bool contains(int touchX, int touchY){
        return touchX >= x && x + width >= touchX && touchY >= y && y + height >= touchY;
      }
};

class Selector {
  private:
      const char** options;
      int optionCount;
      int selectedOption;

  public:
      Button leftButton;
      Button rightButton;

      Selector(const char** options, int optionCount){
          this->options = options;
          this->optionCount = optionCount;
          selectedOption = 0;
      }

      void next(){
        selectedOption++;

        if(selectedOption >= optionCount){
          selectedOption = 0;
        }
      }

      void previous(){
        selectedOption++;

        if(0 > selectedOption){
          selectedOption = optionCount - 1;
        }
      }

      const char* getSelected(){
        return options[selectedOption];
      } 

      void draw(){
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);

        tft.setCursor(100, 100);
        tft.print(getSelected());

        tft.drawRect(40, 90, 50, 40, TFT_WHITE);
        tft.drawRect(230, 90, 50, 40, TFT_WHITE);

        tft.setCursor(55, 100);
        tft.print("<");

        tft.setCursor(245, 100);
        tft.print(">");
      }
};


const char* modes[] = {
  "KOFFIE 1",
  "KOFFIE 2",
  "KOFFIE 3"
};

Selector selector(modes, 3);

Button prev = {40, 90, 50, 40};
Button next = {230, 90, 50, 40};

int rotation = 1;

void setup() {
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);

  ts.begin();
  ts.setRotation(1);

  selector.draw();
}

void loop()
{
    tft.fillRect(0, 0, 320, 40, TFT_BLACK);
    tft.setCursor(10, 10);

    if (ts.touched())
    {
        TS_Point p = ts.getPoint();

        tft.print("TOUCH");
        tft.setCursor(10, 25);
        tft.print(p.x);
        tft.print("  ");
        tft.print(p.y);
    }
    else
    {
        tft.print("NO TOUCH");
    }

    delay(100);
}
