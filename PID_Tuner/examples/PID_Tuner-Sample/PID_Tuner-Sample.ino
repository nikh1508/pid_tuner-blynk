#include <PID_Tuner.h>

float kp, ki, kd;
bool power;

PIDTuner tuner(&power, &kp, &ki, &kd, Serial3);

void setup()
{
  Serial.begin(115200);
  // Wait until the other device is ready to send data.
  while (!tuner.update())
    ;
}

void loop()
{
  if (tuner.update())
  {
    Serial.println(String(kp) + "\t" + String(ki) + "\t" + String(kd) + "\t" + String(power));
  }
}