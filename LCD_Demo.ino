/* http://www.stm32duino.com/viewtopic.php?f=9&t=1171
	S050616
 *  
 * LCD demo. See A050616 for connections.
 * 
 * Tested on Arduino IDE 1.6.5 + Arduino_STM32.
 * Maple-mini clones.
 */ 
 
#include <LiquidCrystal.h>

// HW definitions
#define LED     PB1     // Maple-mini LED
#define BUT     PB8     // Maple-mini BUT pushbutton
#define LCD_RS  PB13
#define LCD_E   PB14
#define LCD_D4  PB12
#define LCD_D5  PB15
#define LCD_D6  PA15
#define LCD_D7  PB3
#define LCD_LED PB6     // LCD backlight led
#define LCD_V0  PA8     // LCD contrast voltage (PWM generated)

enum LcdModel {LCD2004, LCD1602};

const   uint16_t  maxPWM = 0xFFFF;        // Max value (hex) for PWM (see datasheet)
const   uint16_t  contrast = maxPWM/3;    // 33% duty cycle should be a good "default" value (V0 = Vdd/3 = 1.1V)
const   uint16_t  bklight = 0;            // 0 means full bright backlight
const   LcdModel  Lcd = LCD2004;          // LCD module type (2004 or 1602)

LiquidCrystal     lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
uint32_t          i;
boolean           flag = 0;

void setup() 
{
  // Initaialize LED and BUT button
  pinMode(LED, OUTPUT);
  pinMode(BUT, INPUT);
  
  // Wait until BUT is pressed
  digitalWrite(LED, HIGH);
  while(!digitalRead(BUT)) {}   
  digitalWrite(LED, LOW);

  // Initialize LCD and the two used PWMs (for contrast and backlight)
  lcd.begin(20, 4);
  pinMode(LCD_V0, PWM);
  pinMode(LCD_LED, PWM_OPEN_DRAIN);   // This one must be "open drain" output
  pwmWrite(LCD_V0, contrast);
  pwmWrite(LCD_LED, bklight);
}

void loop() 
{
  // Display titles
  if (Lcd == LCD2004)
  {
    lcd.print("  ARM STM32F103C8");
    lcd.setCursor(0, 1);
    lcd.print("     LCD Demo");
    delay(3000);
    lcd.clear();
    lcd.print("    Powered by:");
    lcd.setCursor(0, 1);
    lcd.print(" www.stm32duino.com");
  }
  else
  {
    lcd.print("   STM32F103C8");
    lcd.setCursor(0, 1);
    lcd.print("    LCD Demo");
    delay(3000);
    lcd.clear();
    lcd.print("   Powered by:");
    lcd.setCursor(0, 1);
    lcd.print(" stm32duino.com");
  }
  delay(3000);

  // Contrast demo
  for (i = 1; i <= 4; i++)
  {
    if (Lcd == LCD2004)
    {
      lcd.setCursor(0, 2);
      lcd.print("                 ");
      delay(500);
      lcd.setCursor(0, 2);
      lcd.print("   -> Contrast <-");
    }
    else
    {
      lcd.clear();
      //lcd.setCursor(0, 1);
      //lcd.print("              ");
      delay(500);
      //lcd.setCursor(0, );
      lcd.print(" -> Contrast <-");
    }
      delay(500);
  }
  for (i = 0; i <= maxPWM; i = i+40)
  {
    pwmWrite(LCD_V0, i);
    delay(10);
    if (Lcd == LCD2004)
    {
      lcd.setCursor(0, 3);
      lcd.print("  PWM level: ");
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("PWM level: ");
    }
    lcd.print(i);
  } 
  for (i = 0; i <= maxPWM; i = i+40)
  {
  lcd.setCursor(0, 2);
  lcd.print("                 ");
  lcd.setCursor(0, 3);
  lcd.print("                  ");
  }
  pwmWrite(LCD_V0, contrast);

  // Backlight demo
  for (i = 1; i <= 4; i++)
  {
    if (Lcd == LCD2004)
    {
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      delay(500);
      lcd.setCursor(0, 2);
      lcd.print("  ->  Backlight <-");
    }
    else
    {
      lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.print("             ");
      delay(500);
      //lcd.setCursor(0, 1);
      lcd.print(" -> Backlight <-");
    }
      delay(500);    
  }
  for (i = 0; i <= maxPWM; i = i+40)
  {
    pwmWrite(LCD_LED, i);
    delay(10);
    if (Lcd == LCD2004)
    {
      lcd.setCursor(0, 3);
      lcd.print("  PWM level: ");
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("PWM level: ");
    }
    lcd.print(i);
  } 

  // Final greetings
  lcd.clear();
  delay(1000);
  pwmWrite(LCD_LED, bklight);
  lcd.setCursor(0, 1);
  if (Lcd == LCD2004) {lcd.print("  ");};
  lcd.print("   - bye... -");
  for (i = 1; i <= 6; i++)
  {
    delay(500);
    pwmWrite(LCD_LED, flag * maxPWM);
    flag = !flag;
  }
  lcd.clear();

  // Wait until BUT is pressed to run an other loop
  digitalWrite(LED, HIGH);
  while(!digitalRead(BUT)) {}
  digitalWrite(LED, LOW);
  pwmWrite(LCD_LED, bklight);
}


