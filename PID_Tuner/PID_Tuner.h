#ifndef PID_TUNER_H
#define PID_TUNER_H
#define PID_TUNER_DEBUG

#if ARDUINO > 22
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <HardwareSerial.h>

constexpr byte startMarker = 254;
constexpr byte endMarker = 255;
constexpr byte specialByte = 253;

class PIDTuner
{
    HardwareSerial &port;
    bool *power;
    float *Kp;
    float *Ki;
    float *Kd;

    void serialFlush();
    bool readBytes(byte[], byte, char);
    void decodeData(byte[], byte, byte &, byte[]);
    void debug(String);

public:
    // PIDTuner(byte *, float *, float *, float *, HardwareSerial &);
    PIDTuner(bool *_power, float *_Kp, float *_Ki, float *_Kd, HardwareSerial &newPort = Serial1) : port(newPort)
    {
        port.begin(115200);
        power = _power;
        Kp = _Kp;
        Ki = _Ki;
        Kd = _Kd;
    }
    bool update();
};
#endif