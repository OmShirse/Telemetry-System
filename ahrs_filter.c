#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


float ax, ay, az;
float gx, gy, gz;
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
float roll, pitch, yaw;
const float beta = 0.1f;  

void generate_sensordata(float t) {
    ax = sin(t);
    ay = cos(t);
    az = 1.0f;
    gx = 0.1f * sin(t);
    gy = 0.1f * cos(t);
    gz = 0.05f;
}


void MadgwickAHRSupdateIMU(float dt) {
    
    float norm = sqrt(ax*ax + ay*ay + az*az);
    if(norm == 0) return; 
    ax /= norm; ay /= norm; az /= norm;

    float _2q0 = 2.0f*q0;
    float _2q1 = 2.0f*q1;
    float _2q2 = 2.0f*q2;
    float _2q3 = 2.0f*q3;

    float f1 = _2q1*q3 - _2q0*q2 - ax;
    float f2 = _2q0*q1 + _2q2*q3 - ay;
    float f3 = 1.0f - 2.0f*(q1*q1 + q2*q2) - az;

    float J_11or24 = -_2q2;
    float J_12or23 = 2.0f*q3;
    float J_13or22 = -_2q0;
    float J_14or21 = 2.0f*q1;

    float s0 = J_14or21*f2 + J_11or24*f1;
    float s1 = J_12or23*f2 + J_13or22*f1;
    float s2 = J_12or23*f1 - J_13or22*f2;
    float s3 = J_14or21*f1 - J_11or24*f2;

    norm = sqrt(s0*s0 + s1*s1 + s2*s2 + s3*s3);
    s0 /= norm; s1 /= norm; s2 /= norm; s3 /= norm;

    float qDot0 = 0.5f * (-q1*gx - q2*gy - q3*gz) - beta*s0;
    float qDot1 = 0.5f * (q0*gx + q2*gz - q3*gy) - beta*s1;
    float qDot2 = 0.5f * (q0*gy - q1*gz + q3*gx) - beta*s2;
    float qDot3 = 0.5f * (q0*gz + q1*gy - q2*gx) - beta*s3;

    q0 += qDot0 * dt;
    q1 += qDot1 * dt;
    q2 += qDot2 * dt;
    q3 += qDot3 * dt;

    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 /= norm; q1 /= norm; q2 /= norm; q3 /= norm;

    roll  = atan2(2.0f*(q0*q1 + q2*q3), 1-2*(q1*q1 + q2*q2)) * 180.0f/M_PI;
    pitch = asin(2.0f*(q0*q2 - q3*q1)) * 180.0f/M_PI;
    yaw   = atan2(2.0f*(q0*q3 + q1*q2), 1-2*(q2*q2 + q3*q3)) * 180.0f/M_PI;
}

int main() {
    float t = 0.0f;
    float dt = 0.01f;

    printf("Time\tRoll\tPitch\tYaw\n");
    for(int i=0; i<500; i++) {
        generate_sensordata(t);
        MadgwickAHRSupdateIMU(dt);
        printf("%.2f\t%.2f\t%.2f\t%.2f\n", t, roll, pitch, yaw);
        t += dt;
    }
    return 0;
}
