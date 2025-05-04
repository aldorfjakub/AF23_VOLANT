#include "SerialLinkDriver.h"
#include "Arduino.h"

// Add debug control - can disable all debug prints with one change
#define SERIAL_LINK_DEBUG true

// Debug print macro
#define DEBUG_PRINT(x)          \
    if (SERIAL_LINK_DEBUG)      \
    {                           \
        Serial.print("[SLD] "); \
        Serial.println(x);      \
    }
#define DEBUG_PRINTF(format, ...)           \
    if (SERIAL_LINK_DEBUG)                  \
    {                                       \
        Serial.print("[SLD] ");             \
        Serial.printf(format, __VA_ARGS__); \
        Serial.println();                   \
    }

// Standardized packet types to match steeringWheel module
#define START_BYTE 0xAA
#define ACK_PACKET_TYPE 0x06
#define BATTERY_PACKET_TYPE 0x03
#define DASH_PACKET_TYPE 0x02
#define HELLO_PACKET_TYPE 0x00
#define SPEED_PACKET_TYPE 0x04
#define MODE_PACKET_TYPE 0x05
#define BREAK_PACKET_TYPE 0x07
#define TEMP_PACKET_TYPE 0x08
#define RIDE_STATS_PACKET_TYPE 0x09
#define LED_SPOILER_PACKET_TYPE 0x10


SerialLinkDriver::SerialLinkDriver() {}

void SerialLinkDriver::init()
{
    DEBUG_PRINT("Initializing SerialLinkDriver...");

    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.setFIFOSize(64);
    Serial1.begin(115200);

    DEBUG_PRINT("Waiting for Serial1...");

    while (!Serial1)
    {
        if (millis() > 5000)
        {
            DEBUG_PRINT("Serial1 timeout after 5 seconds");
            break;
        }
    }

    DEBUG_PRINT("Sending initial HELLO packet");
    sendPacket(HELLO_PACKET_TYPE, nullptr, 0);

    isConnected = waitForAck();
    DEBUG_PRINTF("Initial connection result: %s", isConnected ? "CONNECTED" : "FAILED");
}

void SerialLinkDriver::update()
{
    static unsigned long lastAttempt = 0;
    unsigned long now = millis();

    // Limit reconnection attempts to once per second
    if (!isConnected && (now - lastAttempt > 1000))
    {
        DEBUG_PRINT("Not connected. Sending hello packet...");
        sendPacket(HELLO_PACKET_TYPE, nullptr, 0);
        isConnected = waitForAck();
        DEBUG_PRINTF("Connection attempt result: %s", isConnected ? "CONNECTED" : "FAILED");
        lastAttempt = now;
    }

    if (isConnected)
    {
        bool packetReceived = readPacket();
        if (packetReceived)
        {
            DEBUG_PRINT("Successfully processed a packet");
        }
    }
}

void SerialLinkDriver::getBatteryData()
{
    if (isConnected)
    {
        DEBUG_PRINT("Requesting battery data");
        sendPacket(BATTERY_PACKET_TYPE, nullptr, 0);
    }
    else
    {
        DEBUG_PRINT("Cannot request battery data - not connected");
    }
}

void SerialLinkDriver::getDashboardData()
{
    if (isConnected)
    {
        DEBUG_PRINT("Requesting dashboard data");
        sendPacket(DASH_PACKET_TYPE, nullptr, 0);
    }
    else
    {
        DEBUG_PRINT("Cannot request dashboard data - not connected");
    }
}

void SerialLinkDriver::getTempData()
{
    if (isConnected)
    {
        DEBUG_PRINT("Requesting temperature data");
        sendPacket(TEMP_PACKET_TYPE, nullptr, 0);
    }
    else
    {
        DEBUG_PRINT("Cannot request temperature data - not connected");
    }
}

bool SerialLinkDriver::readPacket()
{
    static uint8_t buffer[256];
    static size_t bufferIndex = 0;
    bool packetProcessed = false;

    // Debug bytes available
    if (Serial1.available() > 0)
    {
        DEBUG_PRINTF("Serial1 has %d bytes available", Serial1.available());
    }

    while (Serial1.available() > 0)
    {
        uint8_t incomingByte = Serial1.read();

        // Start of a new packet
        if (bufferIndex == 0 && incomingByte == START_BYTE)
        {
            DEBUG_PRINT("Found START_BYTE");
            buffer[bufferIndex++] = incomingByte;
        }
        // Continue adding bytes to the buffer if we've already started a packet
        else if (bufferIndex > 0)
        {
            buffer[bufferIndex++] = incomingByte;

            // Prevent buffer overflow
            if (bufferIndex >= 256)
            {
                DEBUG_PRINT("Buffer overflow! Resetting buffer");
                bufferIndex = 0;
                return false;
            }

            // Once we've read the header + timestamp + data length byte
            if (bufferIndex == 7)
            {
                // Read the data length
                uint8_t dataLength = buffer[6];
                DEBUG_PRINTF("Packet header received. Data length: %d", dataLength);

                // Make sure the data length is reasonable
                if (dataLength > 200)
                {
                    DEBUG_PRINTF("Invalid data length: %d. Resetting buffer", dataLength);
                    bufferIndex = 0;
                    return false;
                }
            }

            // Check if we have a complete packet
            if (bufferIndex >= 8) // We have at least the header and length byte
            {
                uint8_t dataLength = buffer[6];
                size_t expectedPacketLength = 8 + dataLength; // header(7) + data + CRC(1)
                DEBUG_PRINTF("Buffer Index: %d", bufferIndex);

                // If we have read the complete packet
                if (bufferIndex >= expectedPacketLength)
                {
                    DEBUG_PRINT("Complete packet received");

                    // Calculate CRC
                    uint8_t calculatedCRC = calculateCRC(buffer, bufferIndex - 1);
                    uint8_t receivedCRC = buffer[bufferIndex - 1];

                    DEBUG_PRINT("--- Packet Received ---");
                    DEBUG_PRINT("Raw Bytes (HEX): ");
                    String hexData = "";
                    for (size_t i = 0; i < bufferIndex; i++)
                    {
                        char hex[4];
                        sprintf(hex, "%02X ", buffer[i]);
                        hexData += hex;
                    }
                    DEBUG_PRINT(hexData);

                    DEBUG_PRINTF("Packet Type: 0x%02X", buffer[1]);
                    DEBUG_PRINTF("Data Length: %d", buffer[6]);
                    DEBUG_PRINTF("Calculated CRC: 0x%02X", calculatedCRC);
                    DEBUG_PRINTF("Received CRC: 0x%02X", receivedCRC);

                    // Verify CRC
                    if (calculatedCRC == receivedCRC)
                    {
                        // Extract packet components
                        uint8_t packetType = buffer[1];
                        uint8_t *data = &buffer[7]; // Data starts after header

                        DEBUG_PRINTF("Processing packet type: 0x%02X", packetType);

                        // Process the packet
                        processPacket(packetType, data, dataLength);
                        packetProcessed = true;
                    }
                    else
                    {
                        DEBUG_PRINT("CRC mismatch, discarding packet");
                    }

                    // Reset buffer for next packet
                    bufferIndex = 0;
                }
            }
        }
    }

    return packetProcessed;
}

void SerialLinkDriver::processPacket(uint8_t packetType, uint8_t *data, size_t dataLength)
{
    switch (packetType)
    {
    case BATTERY_PACKET_TYPE:
        if (dataLength >= 40) // Full battery data is 39 bytes
        {
            DEBUG_PRINT("Received battery data:");
            String batteryHex = "";
            for (int i = 0; i < 40; i++)
            {
                char hex[4];
                sprintf(hex, "%02X ", data[i]);
                batteryHex += hex;
            }
            DEBUG_PRINT(batteryHex);

            ;

            parseBatteryData(data, &batteryData);

            DEBUG_PRINTF("Battery Info - Total Voltage: %d mV, RSOC: %d%%",
                         batteryData.totalVoltage, batteryData.rsoc);
            DEBUG_PRINTF("Cell 1 Voltage: %d mV", batteryData.cellVoltages[0]);
            DEBUG_PRINTF("Temperatures: %d°C, %d°C",
                         batteryData.temperature1, batteryData.temperature2);
            DEBUG_PRINTF("Temperatures: %d°C, %d°C",
                         batteryData.temperature1, batteryData.temperature2);
            DEBUG_PRINTF("Current: %d mAh",
                         batteryData.current);

            for (int i = 0; i < 16; i++)
            {
                DEBUG_PRINTF("Cell %d: %d mV", i + 1, batteryData.cellVoltages[i]);
            }
        }
        else
        {
            DEBUG_PRINT("Received incomplete battery data packet");
        }
        break;
    case TEMP_PACKET_TYPE:
        if (dataLength >= 26)
        {
            DEBUG_PRINT("Received temperature data");

            int offset = 0;

            tempData.battery1 = data[offset] | (static_cast<uint16_t>(data[offset + 1]) << 8);
            offset += 2;

            // Parse battery2 temperature (2 bytes) - little-endian
            tempData.battery2 = data[offset] | (static_cast<uint16_t>(data[offset + 1]) << 8);
            offset += 2;

            // Parse outside temperature (float - 4 bytes)
            memcpy(&tempData.outside, &data[offset], sizeof(float));
            offset += sizeof(float);

            // Parse BMS temperature (2 bytes) - little-endian
            tempData.bms = data[offset] | (static_cast<uint16_t>(data[offset + 1]) << 8);
            offset += 2;

            // Parse brake temperature (float - 4 bytes)
            memcpy(&tempData.brake, &data[offset], sizeof(float));
            offset += sizeof(float);

            // Parse motor controller temperature (float - 4 bytes)
            memcpy(&tempData.motorController, &data[offset], sizeof(float));
            offset += sizeof(float);

            // Parse control unit temperature (float - 4 bytes)
            memcpy(&tempData.controlUnit, &data[offset], sizeof(float));
            offset += sizeof(float);

            // Parse motor temperature (float - 4 bytes)
            memcpy(&tempData.motor, &data[offset], sizeof(float));
            offset += sizeof(float);

            // Print some debug info
            DEBUG_PRINTF("Battery temps: %d°C, %d°C", tempData.battery1, tempData.battery2);
            DEBUG_PRINTF("Outside: %.1f°C, BMS: %d°C", tempData.outside, tempData.bms);
            DEBUG_PRINTF("Brake: %.1f°C, Motor Controller: %.1f°C", tempData.brake, tempData.motorController);
            DEBUG_PRINTF("Control Unit: %.1f°C, Motor: %.1f°C", tempData.controlUnit, tempData.motor);
        }
        else
        {
            DEBUG_PRINT("Received incomplete temperature data packet");
        }
        break;

    case ACK_PACKET_TYPE:
        DEBUG_PRINT("Received ACK packet");
        break;

    case DASH_PACKET_TYPE:
        if (dataLength >= 7)
        {
            DEBUG_PRINT("Received dashboard data");

            // Parse dashboard data according to the protocol
            // RPM (2 bytes, big endian)
            dashboardInfo.rpm = static_cast<uint16_t>(data[0]) << 8 | data[1];

            // Temperature
            u16_t temp = static_cast<uint16_t>(data[2]) << 8 | data[3];
            dashboardInfo.temperature = float(temp) / 100.0;

            // Speed
            dashboardInfo.battery = data[4];

            // Status flags
            uint8_t statusByte = data[5];

            uint8_t breakValue = data[6];

            dashboardInfo.speedMode = (statusByte >> 4) & 0x0F;
            dashboardInfo.rideMode = statusByte & 0x0F;
            dashboardInfo.motorBreak = breakValue;

            DEBUG_PRINTF("Dashboard: RPM=%d, Temp=%.2f, Speed=%d, Mode=%d:%d, Break=%d",
                         dashboardInfo.rpm, dashboardInfo.temperature,
                         dashboardInfo.battery, dashboardInfo.speedMode, dashboardInfo.rideMode, breakValue);
        }
        else
        {
            DEBUG_PRINTF("Dashboard data too short: %d bytes", dataLength);
        }
        break;
        case RIDE_STATS_PACKET_TYPE:
        if (dataLength >= 36)
        { // Full ride stats data is 36 bytes
            DEBUG_PRINT("Received ride stats data");
    
             
            int offset = 0;
    
            // Unpack trip distance (4 bytes) and convert from meters to kilometers
            memcpy(&rideStatsData.trip_distance_km, &data[offset], sizeof(float));
            offset += sizeof(float);
    
            // Unpack total distance (4 bytes) and convert from meters to kilometers
            memcpy(&rideStatsData.total_distance_km, &data[offset], sizeof(float));
            offset += sizeof(float);
    
            // Unpack max RPM (4 bytes)
            rideStatsData.max_rpm = (data[offset] << 24) | (data[offset + 1] << 16) |
                                (data[offset + 2] << 8) | data[offset + 3];
            offset += 4;
    
            // Unpack max acceleration (4 bytes)
            memcpy(&rideStatsData.max_acceleration, &data[offset], sizeof(float));
            offset += sizeof(float);
    
            // Unpack best acceleration times (5 x 4 bytes)
            for (int i = 0; i < 5; i++)
            {
                memcpy(&rideStatsData.best_accel_times[i], &data[offset], sizeof(float));
                offset += sizeof(float);
            }
    
            // Print debug info with corrected units
            DEBUG_PRINTF("Trip Distance: %.2f km", rideStatsData.trip_distance_km);
            DEBUG_PRINTF("Total Distance: %.2f km", rideStatsData.total_distance_km);
            DEBUG_PRINTF("Max RPM: %d", rideStatsData.max_rpm);
            DEBUG_PRINTF("Max Acceleration: %.2f m/s²", rideStatsData.max_acceleration);
    
            for (int i = 0; i < 5; i++)
            {
                if (rideStatsData.best_accel_times[i] > 0)
                {
                    DEBUG_PRINTF("Accel Time #%d: %.2f s", i + 1, rideStatsData.best_accel_times[i]);
                }
            }
        }
        else
        {
            DEBUG_PRINT("Received incomplete ride stats data packet");
        }
        break;

    default:
        DEBUG_PRINTF("Unknown packet type: 0x%02X", packetType);
        break;
    }
}

void SerialLinkDriver::parseBatteryData(uint8_t *data, BatteryData *battery)
{
    int offset = 0;

    // Parse the 16 cell voltages
    for (int i = 0; i < 16; i++)
    {
        battery->cellVoltages[i] = data[offset] | (data[offset + 1] << 8);
        offset += 2;
    }

    // Parse total voltage
    battery->totalVoltage = data[offset] | (data[offset + 1] << 8);
    offset += 2;

    // Parse RSOC
    battery->rsoc = data[offset];
    offset += 1;

    // Parse temperatures
    battery->temperature1 = data[offset] | (data[offset + 1] << 8);
    offset += 2;
    battery->temperature2 = data[offset] | (data[offset + 1] << 8);
    offset += 2;
    battery->current = data[offset] | (data[offset + 1] << 8);
}

void SerialLinkDriver::changeSpeedMode(u8_t speedMode)
{
    uint8_t data[1];
    data[0] = speedMode;
    sendPacket(SPEED_PACKET_TYPE, data, 1);
}

void SerialLinkDriver::setLedMode(u8_t ledMode){
    uint8_t data[1];
    data[0] = ledMode;
    sendPacket(LED_SPOILER_PACKET_TYPE, data, 1);

}

void SerialLinkDriver::changeRideMode(u8_t rideMode)
{
    uint8_t data[1];
    data[0] = rideMode;
    sendPacket(MODE_PACKET_TYPE, data, 1);
}

void SerialLinkDriver::changeBreak(bool plus)
{
    u8_t data[1];
    if (plus)
    {
        data[0] = 10;
    }
    else
    {
        data[0] = 20;
    }
    sendPacket(BREAK_PACKET_TYPE, data, 1);
}

void SerialLinkDriver::sendPacket(uint8_t packetType, const uint8_t *data, size_t length)
{
    Serial1.flush();

    uint8_t packet[256];
    size_t packetIndex = 0;

    // Add start byte
    packet[packetIndex++] = START_BYTE;

    // Add packet type
    packet[packetIndex++] = packetType;

    // Add timestamp (4 bytes), big-endian
    uint32_t timestamp = millis();
    packet[packetIndex++] = (timestamp >> 24) & 0xFF;
    packet[packetIndex++] = (timestamp >> 16) & 0xFF;
    packet[packetIndex++] = (timestamp >> 8) & 0xFF;
    packet[packetIndex++] = timestamp & 0xFF;

    // Add data length
    packet[packetIndex++] = length;

    // Add data
    if (data != nullptr && length > 0)
    {
        memcpy(&packet[packetIndex], data, length);
        packetIndex += length;
    }

    // Calculate CRC
    uint8_t crc = calculateCRC(packet, packetIndex);
    packet[packetIndex++] = crc;

    // Debug output
    DEBUG_PRINT("Sending packet:");
    String packetHex = "";
    for (size_t i = 0; i < packetIndex; i++)
    {
        char hex[4];
        sprintf(hex, "%02X ", packet[i]);
        packetHex += hex;
    }
    DEBUG_PRINT(packetHex);

    // Send the packet
    size_t bytesWritten = Serial1.write(packet, packetIndex);
    DEBUG_PRINTF("Sent %d bytes", bytesWritten);
}

uint8_t SerialLinkDriver::calculateCRC(uint8_t *buffer, size_t length)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++)
    {
        crc ^= buffer[i];
    }
    return crc;
}

bool SerialLinkDriver::waitForAck()
{
    unsigned long startTime = millis();
    unsigned long timeout = 200; // Match steeringWheel's timeout

    DEBUG_PRINT("Waiting for ACK...");

    while (millis() - startTime < timeout)
    {
        static uint8_t buffer[256];
        static size_t bufferIndex = 0;

        if (Serial1.available() > 0)
        {
            uint8_t incomingByte = Serial1.read();

            // Debug the incoming byte
            DEBUG_PRINTF("Received byte: 0x%02X", incomingByte);

            // Start of a new packet
            if (bufferIndex == 0 && incomingByte == START_BYTE)
            {
                buffer[bufferIndex++] = incomingByte;
            }
            // Continue adding bytes if we've started a packet
            else if (bufferIndex > 0)
            {
                buffer[bufferIndex++] = incomingByte;

                // Minimum ACK packet is 8 bytes (START_BYTE + type + timestamp(4) + length(0) + CRC)
                if (bufferIndex >= 8)
                {
                    // Check if it's an ACK packet
                    if (buffer[1] == ACK_PACKET_TYPE)
                    {
                        DEBUG_PRINT("Found ACK packet");

                        // Validate CRC
                        uint8_t calculatedCRC = calculateCRC(buffer, bufferIndex - 1);
                        uint8_t receivedCRC = buffer[bufferIndex - 1];

                        DEBUG_PRINTF("ACK CRC check: calculated=0x%02X, received=0x%02X",
                                     calculatedCRC, receivedCRC);

                        if (calculatedCRC == receivedCRC)
                        {
                            DEBUG_PRINT("ACK validated successfully");
                            bufferIndex = 0;
                            return true;
                        }
                        else
                        {
                            DEBUG_PRINT("ACK CRC mismatch");
                        }
                    }

                    // Reset for next packet
                    bufferIndex = 0;
                }
            }
        }
    }

    DEBUG_PRINT("ACK timeout - no response received");
    return false;
}