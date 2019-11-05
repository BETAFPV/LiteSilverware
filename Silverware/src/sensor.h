
#ifndef __VECTOR3_H__
#define __VECTOR3_H__


typedef struct {
    float x;
    float y;
    float z;
} Vector3f_t;

enum
{
    STATIC,
    MOTIONAL
};

enum
{
    GYRO,
    ACC,
    MAG,
    ANGLE,
    BARO,
    ESC
};


enum ORIENTATION_STATUS
{
    ORIENTATION_UP,
    ORIENTATION_DOWN,
    ORIENTATION_LEFT,
    ORIENTATION_RIGHT,
    ORIENTATION_FRONT,
    ORIENTATION_BACK,
};

void ImuOrientationDetect(void);
void AccCalibration(void);
void ImuLevelCalibration(void);
void VectorRotateToBodyFrame(float *vector, float *deltaAngle);

#endif



















