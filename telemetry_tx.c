#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


float ax, ay, az;
float gx, gy, gz;
float altitude;
float temperature;


long long current_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)(ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL);
}

void generate_sensor_data(float t)
{
    ax = sin(t);
    ay = cos(t);
    az = 1.0f;

    gx = 0.1f * sin(t);
    gy = 0.1f * cos(t);
    gz = 0.05f;

    altitude = 100.0f + 0.5f * t;
    temperature = 25.0f + 0.2f * sin(0.1f * t);
}

unsigned char calculate_checksum(const char *data)
{
    unsigned char check = 0;

    while (*data)
    {
        check ^= (unsigned char)(*data);
        data++;
    }
    return check;
}

/*telemetry frame */
void send_telemetry_frame(long long timestamp)
{
    char payload[256];
    char frame[300];

    snprintf(payload, sizeof(payload),"L1,%lld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",timestamp, ax, ay, az, gx, gy, gz, altitude, temperature);

    unsigned char check = calculate_checksum(payload);

    snprintf(frame, sizeof(frame),
             "$%s*%02X\n", payload, check);

    printf("%s", frame);
}

int main(void)
{
    long long start_time = current_time_ms();
    int samples = 100;

    while (samples--)
    {
        long long now = current_time_ms();
        float t = (now - start_time) / 1000.0f;

        generate_sensor_data(t);
        send_telemetry_frame(now);

        usleep(50000);  // 20 Hz
    }

    return 0;
}
