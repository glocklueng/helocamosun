unsigned char pwmCommand[] = { 'P', 0x01, 0x01, 0x01, 0x01 };
	
unsigned char GP_handshake[]= { 0xa5, 0x5a, 0x02, 0x43, 0x06, 0x00, 0x45, 0xCC, 0x33 };
	
unsigned char GSPI_AccReq[] = { 'A', 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 };
	
unsigned char GSPI_2GyroReq[] = { 'G', 0x01, 0x01, 0x01, 0x01, 0x00 };
	
unsigned char GSPI_CompReq[] = { 'C', 0x01, 0x01, 0x00 };
	
unsigned char GSPI_AcousticReq[] = { 'O', 0x01, 0x01, 0x00 };
	
unsigned char GSPI_VoltReq[] = { 'V', 0x01, 0x01, 0x01, 0x01, 0x00 };
	
unsigned char GSPI_TempReq[] = { 'T', 0x01, 0x01, 0x00 };
	
unsigned char GSPI_RpmReq[] = { 'R', 0x01, 0x01, 0x00 };
	
unsigned char GSPI_StatusReq[] = { 'S', 0x01, 0x01, 0x00 };
	

