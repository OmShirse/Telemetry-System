#include<stdio.h>
#include<math.h>
#include<time.h>
#include<unistd.h>

float ax, ay, az ;/*Accelerometer Variables*/
float gx, gy, gz ; /*Gyroscope Varible*/ 
float altitude, temp;

long long current_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return (long long)(ts.tv_sec * 1000LL + ts.tv_nsec /1000000LL);
}

void generate_sensordata(float t)
{
  ax = sin(t);
  ay = cos(t);
  az = 1.0f;
  gx = 1.0f * sin(t);
  gy = 1.0f* cos(t);
  gz = 0.5f;
  altitude = 1000.f + 0.5f * t;
  temp = 25.0f + 0.2f * sin(0.1f*t);
}

int main(void)
{
  long long start_time = current_time_ms();
  while (1)
  {
    long long now  = current_time_ms();
    float t = (now -start_time)/1000.0f;
    generate_sensordata(t);
    printf("ACC: %.2f %.2f %.2f | GYRO: %.2f %.2f %.2f | ALT: %.2f | TEMP: %.2f\n",ax, ay, az, gx, gy, gz, altitude, temp);
    usleep(50000);
  }
  return 0;
}