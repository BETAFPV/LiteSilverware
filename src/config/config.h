

#define BETA_FILTERING

#ifdef BETA_FILTERING  //*** ABOVE 100 ADJUST IN INCRIMENTS OF 20, BELOW 100 ADJUST IN INCRIMENTS OF 10, nothing coded beyond 500hz

//Select Gyro Filter Type *** Select Only One type
#define KALMAN_GYRO
//#define PT1_GYRO

//Select Gyro Filter Cut Frequency
#define GYRO_FILTER_PASS1 HZ_90
#define GYRO_FILTER_PASS2 HZ_90

//Select D Term Filter Cut Frequency *** Select Only one
#define  DTERM_LPF_2ND_HZ 100
//#define DTERM_LPF_1ST_HZ 70

#endif



#define USE_RX_BAYANG

//#define USE_RX_SBUS

//#define USE_RX_DSMX


