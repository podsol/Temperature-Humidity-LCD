#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPin 7
#define DHTType DHT11

DHT_Unified dht(DHTPin, DHTType);
uint32_t delayInMS;

const int RS = 12, E = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);


void setup() {

  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  delayInMS = sensor.min_delay / 1000 * 4;


  byte degree[8] {
    B11100,
    B10100,
    B11100,
    B00000,
    B00000,
    B00000,
    B00000,
  };

  lcd.createChar(0, degree);
  lcd.clear();
  
}

void loop() {
  
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
//  if (isnan(getTemp())) 
//    lcd.print(getTemp());
  lcd.setCursor(5, 1);
  lcd.write(byte(0));
  lcd.print("F");
  delay(delayInMS);
  lcd.clear();
  
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print(getHumid());
  lcd.setCursor(6, 1);
  lcd.print("%");
  delay(delayInMS);
  lcd.clear();
}



double getTemp() {
  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  
  if (isnan(event.temperature)) {
    Serial.println(event.temperature);
    return;
  } else {
    Serial.println("its a number");
    return (event.temperature * 9/5) + 32;
    
  }
}

double getHumid() {
  
  sensors_event_t event;
  dht.humidity().getEvent(&event);

  if (isnan(event.relative_humidity)) {
    Serial.println("Humidity Error!");
    return 0;
  } else {
    return event.relative_humidity;
  }
}
