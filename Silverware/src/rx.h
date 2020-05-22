
void rx_init(void);
void checkrx( void);
#ifdef RX_SBUS_DSMX_BAYANG_SWITCH
void sbus_checkrx(void);
void sbus_rx_init(void);
void dsm_init(void);

void lite_2S_rx_spektrum_bind(void);
void rx_spektrum_bind(void);
void dsmx_rx_init(void);
void dsmx_checkrx(void);

#endif
#if defined(RX_DSMX_2048) || defined(RX_DSM2_1024)
void rx_spektrum_bind(void);
#endif








