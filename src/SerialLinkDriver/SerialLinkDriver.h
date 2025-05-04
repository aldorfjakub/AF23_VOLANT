#ifndef SERIAL_LINK_DRIVER_H
#define SERIAL_LINK_DRIVER_H

#include <Arduino.h>

class SerialLinkDriver
{

private:
    bool isConnected = false;
    static const uint8_t START_BYTE = 0xAA;

public:
    // Dashboard info structure
    struct DashboardInfo {
        uint16_t rpm;
        uint8_t battery;
        float temperature;
        uint8_t speedMode;
        uint8_t rideMode =1;
        u8_t motorBreak=0;
    };
    struct BatteryData {
        uint16_t cellVoltages[16];
        uint16_t totalVoltage;
        uint8_t rsoc;
        int16_t current;
        uint16_t temperature1;
        uint16_t temperature2;
    };
    struct TemperatureData{
        u16_t battery1;
        u16_t battery2;
        float outside;
        u16_t  bms;
        float brake;
        float motorController;
        float controlUnit;
        float motor;
    };

     struct RideStatsData {
        float trip_distance_km;      // Trip distance in kilometers
        float total_distance_km;     // Total distance in kilometers
        uint16_t max_rpm;            // Maximum RPM recorded
        float max_acceleration;      // Maximum acceleration in m/s²
        float best_accel_times[5];    // Best 5 acceleration times (0-100 km/h)
    } ;
    
    DashboardInfo dashboardInfo;
    BatteryData batteryData;
    TemperatureData tempData;
    RideStatsData rideStatsData;

    SerialLinkDriver();
    void init();
    void update();
    void getBatteryData();
    void getDashboardData();
    void getTempData();
    void changeSpeedMode(u8_t speedMode);
    void changeRideMode(u8_t rideMode);
    bool readPacket();
    void processPacket(uint8_t packetType, uint8_t *data, size_t dataLength);
    void sendPacket(uint8_t packetType, const uint8_t *data, size_t length);
    void changeBreak(bool plus);
    uint8_t calculateCRC(uint8_t *buffer, size_t length);
    void setLedMode(u8_t ledMode);
    bool waitForAck();
    void parseBatteryData(uint8_t* data, BatteryData* battery);

};

#endif