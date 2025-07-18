/**
 * @file mdds30_esp32_serial_control.ino
 * @brief Serial control for Cytron MDDS30 using ESP32 via Serial2 (UART)
 * 
 * This example shows how to control the Cytron SmartDriveDuo-30 (MDDS30) motor driver
 * in **Serial Packetized Mode** using an **ESP32**.
 * 
 * Tested with:
 * - MDDS30 DIP switch set to `11100000` (serial packetized mode)
 * - TX2 (GPIO17) connected to MDDS30 IN1
 * - GND to GND
 * 
 * @note This is adapted from the official MDDS Arduino library to work with ESP32.
 */

uint8_t headerByte = 0x55;     // Constant header byte as per protocol
uint8_t startByte = 0x80;
uint8_t addressByte, commandByte, checksum;

/**
 * @brief Arduino setup function
 */
void setup()
{
  delay(2000);  // Give time for serial devices to boot
  Serial2.begin(115200);  // UART2: TX2 on GPIO17
  delay(100);

  Serial2.write(startByte);  // Send start byte to wake up MDDS30
  delay(2000);          // Allow time for MDDS30 to initialize
}

/**
 * @brief Send motor speed command to MDDS30
 * 
 * @param boardId Board ID (usually 0 for default)
 * @param motorLSpeed Left motor speed [-100 to 100]
 * @param motorRSpeed Right motor speed [-100 to 100]
 */
void motorcontrol(uint8_t boardId, int motorLSpeed, int motorRSpeed)
{
  // --- Left Motor ---
  addressByte = boardId | 0b00000000;
  commandByte = map(motorLSpeed, -100, 100, 0, 255);
  checksum = (headerByte + addressByte + commandByte) % 256;

  Serial2.write(headerByte);
  Serial2.write(addressByte);
  Serial2.write(commandByte);
  Serial2.write(checksum);

  // --- Right Motor ---
  addressByte = boardId | 0b00001000;
  commandByte = map(motorRSpeed, 100, -100, 0, 255); // Reversed mapping for right motor
  checksum = (headerByte + addressByte + commandByte) % 256;

  Serial2.write(headerByte);
  Serial2.write(addressByte);
  Serial2.write(commandByte);
  Serial2.write(checksum);
}

/**
 * @brief Main loop
 */
void loop()
{
  // Move both motors forward at 50% speed
  motorcontrol(0, 50, 50);
  delay(1000);
}
