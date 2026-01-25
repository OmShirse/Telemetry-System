#include <stdio.h>
#include <math.h>
#include <stdint.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float ax, ay, az;
float gx, gy, gz;
float altitude;
float temp;


float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
float roll, pitch, yaw;
const float beta = 0.1f;  

typedef struct {
    float ax, ay, az;       // Accelerometer
    float gx, gy, gz;       // Gyroscope
    float altitude;         // Altitude
    float temp;             // Temperature
    float roll, pitch, yaw; // AHRS orientation
    uint8_t checksum;       // Frame validation
} EnhancedTelemetryFrame;

void generate_sensordata(float t) {
    ax = sin(t);
    ay = cos(t);
    az = 1.0f;
    gx = 0.1f * sin(t);
    gy = 0.1f * cos(t);
    gz = 0.05f;

    altitude = 1000.0f + 0.5f * t;
    temp = 25.0f + 0.2f * sin(0.1f * t);
}

void MadgwickAHRSupdateIMU(float dt) {
    float norm = sqrt(ax*ax + ay*ay + az*az);
    if(norm == 0) return;
    ax /= norm; ay /= norm; az /= norm;

    float _2q0 = 2.0f*q0, _2q1 = 2.0f*q1, _2q2 = 2.0f*q2, _2q3 = 2.0f*q3;
    float f1 = _2q1*q3 - _2q0*q2 - ax;
    float f2 = _2q0*q1 + _2q2*q3 - ay;
    float f3 = 1.0f - 2.0f*(q1*q1 + q2*q2) - az;

    float s0 = -_2q2*f1 + _2q1*f2;
    float s1 =  _2q3*f1 + _2q0*f2;
    float s2 = -_2q0*f1 + _2q3*f2;
    float s3 =  _2q1*f1 + _2q2*f2;

    norm = sqrt(s0*s0 + s1*s1 + s2*s2 + s3*s3);
    s0 /= norm; s1 /= norm; s2 /= norm; s3 /= norm;

    float qDot0 = 0.5f * (-q1*gx - q2*gy - q3*gz) - beta*s0;
    float qDot1 = 0.5f * ( q0*gx + q2*gz - q3*gy) - beta*s1;
    float qDot2 = 0.5f * ( q0*gy - q1*gz + q3*gx) - beta*s2;
    float qDot3 = 0.5f * ( q0*gz + q1*gy - q2*gx) - beta*s3;

    q0 += qDot0 * dt;
    q1 += qDot1 * dt;
    q2 += qDot2 * dt;
    q3 += qDot3 * dt;

    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 /= norm; q1 /= norm; q2 /= norm; q3 /= norm;

    roll  = atan2(2*(q0*q1 + q2*q3), 1-2*(q1*q1 + q2*q2)) * 180.0f/M_PI;
    pitch = asin(2*(q0*q2 - q3*q1)) * 180.0f/M_PI;
    yaw   = atan2(2*(q0*q3 + q1*q2), 1-2*(q2*q2 + q3*q3)) * 180.0f/M_PI;
}

uint8_t calculate_checksum(EnhancedTelemetryFrame frame) {
    uint8_t *data = (uint8_t*)&frame;
    uint8_t sum = 0;
    // Exclude last byte (checksum itself)
    for(int i=0; i<sizeof(EnhancedTelemetryFrame)-1; i++) {
        sum += data[i];
    }
    return sum;
}

EnhancedTelemetryFrame generate_frame(float t, float dt) {
    generate_sensordata(t);
    MadgwickAHRSupdateIMU(dt);

    EnhancedTelemetryFrame frame;
    frame.ax = ax; frame.ay = ay; frame.az = az;
    frame.gx = gx; frame.gy = gy; frame.gz = gz;
    frame.altitude = altitude;
    frame.temp = temp;
    frame.roll = roll; frame.pitch = pitch; frame.yaw = yaw;

    frame.checksum = calculate_checksum(frame);
    return frame;
}


int main() {
    float dt = 0.01f;
    float t = 0.0f;

    printf("Time\tax\tay\taz\tgx\tgy\tgz\talt\ttemp\troll\tpitch\tyaw\tchecksum\n");

    for(int i=0; i<200; i++) {
        EnhancedTelemetryFrame frame = generate_frame(t, dt);
        printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\n",
               t, frame.ax, frame.ay, frame.az, frame.gx, frame.gy, frame.gz,
               frame.altitude, frame.temp, frame.roll, frame.pitch, frame.yaw, frame.checksum);
        t += dt;
    }

    return 0;
}
