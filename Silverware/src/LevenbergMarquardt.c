#include "LevenbergMarquardt.h"


double lm_lambda;

static void UpdateMatrices(float dS[6], float JS[6][6], float beta[6], float data[3]);
static void ResetMatrices(float dS[6], float JS[6][6]);
static void GaussEliminateSolveDelta(float dS[6], float JS[6][6], float delta[6]);


float ConstrainFloat(float amt, float low, float high)
{
    if (isnan(amt))
    {
        return (low+high)*0.5f;
    }
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

void LevenbergMarquardt(Vector3f_t inputData[6], Vector3f_t* offset, Vector3f_t* scale, float initBeta[6], float length)
{
    uint32_t cnt    = 0;
    double   eps    = 1e-10;
    double   change = 100.0;
    double   changeTemp = 100.0;
    float    data[3];
    float    beta[6];      //???
    float    delta[6];     //????
    float    JtR[6];       //????
    float    JtJ[6][6];    //????


    for(uint8_t i=0; i<6; i++)
    {
        beta[i] = initBeta[i];
    }

    lm_lambda = 0.1;

    while(change > eps)
    {
        ResetMatrices(JtR, JtJ);

        for(uint8_t i=0; i<6; i++)
        {
            data[0] = inputData[i].x;
            data[1] = inputData[i].y;
            data[2] = inputData[i].z;
            UpdateMatrices(JtR, JtJ, beta, data);
        }

        GaussEliminateSolveDelta(JtR, JtJ, delta);

        changeTemp = delta[0]*delta[0] +
                     delta[0]*delta[0] +
                     delta[1]*delta[1] +
                     delta[2]*delta[2] +
                     delta[3]*delta[3] / (beta[3]*beta[3]) +
                     delta[4]*delta[4] / (beta[4]*beta[4]) +
                     delta[5]*delta[5] / (beta[5]*beta[5]);

        if(changeTemp < change)
        {
            lm_lambda /= 3;

            for(uint8_t i=0; i<6; i++)
            {
                beta[i] -= delta[i];
            }

            change = changeTemp;
        }
        else
        {
            lm_lambda *= 3;
            lm_lambda = ConstrainFloat(lm_lambda, 0, 1e10);
        }

        if(cnt++ > 1000)
            break;
    }

    scale->x  = beta[3] * length;
    scale->y  = beta[4] * length;
    scale->z  = beta[5] * length;
    offset->x = beta[0];
    offset->y = beta[1];
    offset->z = beta[2];
}

static void UpdateMatrices(float JtR[6], float JtJ[6][6], float beta[6], float data[3])
{
    int16_t j, k;
    float dx, b;
    float residual = 1.0;
    float jacobian[6];

    for(j=0; j<3; j++)
    {
        b = beta[3+j];
        dx = (float)data[j] - beta[j];
        residual -= b*b*dx*dx;

        jacobian[j] = 2.0f*b*b*dx;
        jacobian[3+j] = -2.0f*b*dx*dx;
    }

    for(j=0; j<6; j++)
    {
        JtR[j] += jacobian[j]*residual;

        for(k=0; k<6; k++)
        {
            JtJ[j][k] += jacobian[j]*jacobian[k];
        }
    }
}


static void ResetMatrices(float JtR[6], float JtJ[6][6])
{
    int16_t j,k;
    for(j=0; j<6; j++)
    {
        JtR[j] = 0.0f;
        for(k=0; k<6; k++)
        {
            JtJ[j][k] = 0.0f;
        }
    }
}

static void GaussEliminateSolveDelta(float JtR[6], float JtJ[6][6], float delta[6])
{
    int16_t i,j,k;
    float mu;

    for(i=0; i<6; i++)
    {
        JtJ[i][i] += lm_lambda;
    }

    for(i=0; i<6; i++)
    {
        for(j=i+1; j<6; j++)
        {
            mu = JtJ[i][j] / JtJ[i][i];
            if(mu != 0.0f)
            {
                JtR[j] -= mu * JtR[i];
                for(k=j; k<6; k++)
                {
                    JtJ[k][j] -= mu * JtJ[k][i];
                }
            }
        }
    }

    for(i=5; i>=0; i--)
    {
        JtR[i] /= JtJ[i][i];
        JtJ[i][i] = 1.0f;

        for(j=0; j<i; j++)
        {
            mu = JtJ[i][j];
            JtR[j] -= mu * JtR[i];
            JtJ[i][j] = 0.0f;
        }
    }

    for(i=0; i<6; i++)
    {
        delta[i] = JtR[i];
    }
}




