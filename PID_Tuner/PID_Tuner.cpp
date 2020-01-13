#include <PID_Tuner.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#if ARDUINO > 22
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

void PIDTuner::serialFlush()
{
    while (port.available() > 0)
        char ch = port.read();
}

void PIDTuner::decodeData(byte toDecode[], byte totalRecvd, byte &decodedByteCount, byte decodedBytes[])
{
    for (int i = 1; i < (totalRecvd - 1); i++)
    {
        byte x = toDecode[i];
        if (x == specialByte)
            x += toDecode[++i];
        decodedBytes[decodedByteCount++] = x;
    }
}

bool PIDTuner::readBytes(byte data[], byte toRead, char toSend)
{
    static bool firstCall = true;
    static bool inProgress = false;
    static byte bytesRecvd = 0;
    static byte tempBuffer[20];
    static long long lastRead;
    if (firstCall)
    {
        firstCall = false;
        serialFlush();
        port.print(toSend);
        lastRead = millis();
    }

    while (port.available())
    {
        byte x = port.read();
        // debug("byte Received");
        lastRead = millis();
        if (x == startMarker)
        {
            // debug("start byte Received");
            inProgress = true;
            bytesRecvd = 0;
        }
        if (inProgress)
            tempBuffer[bytesRecvd++] = x;
        if (x == endMarker)
        {
            inProgress = false;
            firstCall = true;
            byte decodedByteCount = 0;
            decodeData(tempBuffer, bytesRecvd, decodedByteCount, data);
            if (decodedByteCount == toRead)
                return true;
            else
                debug("Received Incorrect Bytes in :: " + String(int(toSend)));
        }
    }
    if ((millis() - lastRead) > 50)
    {
        debug("SERIAL TIMEOUT");
        firstCall = true;
    }
    return false;
}

void PIDTuner::debug(String errorMessage)
{
#ifdef PID_TUNER_DEBUG
    Serial.println(errorMessage);
#endif
}

bool PIDTuner::update()
{
    byte data[13];                // 1 byte for Power & 4*3 bytes for PID constants
    if (readBytes(data, 13, 122)) // 'z'(122) is predecided as request character
    {
        *power = data[0];
        memcpy(Kp, data + 1, 4);
        memcpy(Ki, data + 5, 4);
        memcpy(Kd, data + 9, 4);
        return true;
    }
    return false;
}