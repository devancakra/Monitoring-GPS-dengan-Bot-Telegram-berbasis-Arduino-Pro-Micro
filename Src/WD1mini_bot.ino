// library initialization
#include <SoftwareSerial.h> // calls a library called SoftwareSerial.h
SoftwareSerial mcuSerial(12, 13); // D6 As RX, D7 As TX -> Wemos D1 Mini to Arduino Pro Micro

String raw_data, latitude, longitude; // data with String type is used for GPS sensor purposes

// Method: setup
void setup() {
  Serial.begin(115200); // start serial communication inside the Wemos D1 Mini
  mcuSerial.begin(115200); // start serial communication to Arduino Pro Micro
}

// Method: loop
void loop() {  
  gpssensor(); // calling the gpssensor method
}

// Method: gpssensor
void gpssensor(){
  while(!mcuSerial.available()){ Serial.println("Failed to get sensor data, system tries to reconnect communication !!"); } // serial communication with the Arduino Pro Micro board failed
  raw_data = ""; // this String data type is used to store data obtained from serial communication
  while(mcuSerial.available()){ // this loop is used to read the serial communication data from the Arduino Pro Micro
    raw_data += char(mcuSerial.read()); // adds each sensor data reading into a data string named raw_data
  }  
  latitude = getValue(raw_data, ' ', 0); // this variable is used to store latitude data
  longitude = getValue(raw_data, ' ', 1); // this variable is used to store longitude data
  Serial.println("Retrieve serial data from Arduino Pro Micro board..."); // display data to the Wemos D1 Mini serial monitor
  delay(2000); // delay -> 2 second 
  Serial.println(latitude); // print latitude data on the serial monitor
  Serial.println(longitude); // longitude data on the serial monitor
  delay(1000); // delay -> 1 second 
}

// function for data parsing
String getValue(String data, char separator, int index){ // there are 3 parameters: data -> string, separator -> character, index -> integer
  int found = 0; // variable with integer type to store found data
  int strIndex[] = {0, -1}; // variable with integer type to store two index values to be used as bounds when extracting substrings from the data string. Initially, strIndex[0] is set to 0, and strIndex[1] is set to -1
  int maxIndex = data.length()-1; // variable with integer type to store the upper limit of the index
 
  for(int i=0; i<=maxIndex && found<=index; i++){ // loop to search data
    if(data.charAt(i)==separator || i==maxIndex){ // if the character at index-i in the data string is equal to a separator character or index-i has reached the end of the string then
        found++; // increases the number of values that have been found during iteration through the string
        strIndex[0] = strIndex[1]+1; // to set the initial border of the substring that will be retrieved
        strIndex[1] = (i == maxIndex) ? i+1 : i; // to set the end limit of the substring to be retrieved
    }
  } 

  // returns a value based on the condition:
  // 1. checks whether the value found is greater than the index. If this condition is true, then the function will return the substring data starting from strIndex[0] to strIndex[1]
  // 2. if the condition is false (meaning the found value is not greater than the index), then the function will return an empty string meaning no value was found at the requested index
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
