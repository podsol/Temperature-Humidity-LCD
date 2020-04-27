#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPin 7  //data pin for sensor
#define DHTType DHT11 //type of sensor you have, in this case, the dht11


DHT_Unified dht(DHTPin, DHTType);

uint32_t delayInMS;

const int RS = 12, E = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;  //pins on lcd to pins on arduino
LiquidCrystal lcd(RS, E, D4, D5, D6, D7); //connecting LCD


void setup() {

  //starting serial monitor, sensor and lcd
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);

  //used for getting the time the sensor can measure
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayInMS = sensor.min_delay / 1000 * 4;


  //degree sign because the regular one is not supported
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

  //clearing lcd to prevent problems
  lcd.clear();

}

void loop() {

  //printing temperature
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(getTemp());
  lcd.setCursor(5, 1);
  lcd.write(byte(0));
  lcd.print("F");
  delay(delayInMS);
  lcd.clear();


  //printing humidity
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print(getHumid());
  lcd.setCursor(6, 1);
  lcd.print("%");
  delay(delayInMS);
  lcd.clear();
}


// getting the current temperature
double getTemp() {
  //event for temperature
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  //checking if the sensor is connected properly
  if (isnan(event.temperature)) {
    Serial.println("Temperature Error");
    return 0;
  } else {
    return (event.temperature * 9 / 5) + 32;  //converting to farenheit (celsius default)
  }
}


//getting current humidity
double getHumid() {
  //event for humididty
  sensors_event_t event;
  dht.humidity().getEvent(&event);

  //checking if sensor is connected properly
  if (isnan(event.relative_humidity)) {
    Serial.println("Humidity Error!");
    return 0;
  } else {
    return event.relative_humidity;
  }
}
