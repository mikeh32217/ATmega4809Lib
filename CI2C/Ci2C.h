/*
 * Ci2C.h
 *
 * Created: 1/26/2022 9:54:09 AM
 *  Author: Mike
 */ 
#ifndef CI2C_H_
#define CI2C_H_

#define CLK_PER                       20000000ul     // 4MHz default clock
#define TWI0_BAUD(F_SCL, T_RISE)      ((((((float)CLK_PER / (float)F_SCL)) - 10 - ((float)CLK_PER * T_RISE))) / 2)
#define I2C_SCL_FREQ                  100000
#define I2C_SLAVE_ADDRESS             0xA0

#define I2C_DIRECTION_BIT_WRITE       0
#define I2C_DIRECTION_BIT_READ        1

#define I2C_STATUS_BUSERR_bm		0b00000100
#define I2C_STATUS_ARBLOST_bm		0b00001000
#define I2C_STATUS_MASK_gc			(I2C_STATUS_BUSERR_bm | I2C_STATUS_ARBLOST_bm)
 
#define CREATE_16BIT_VARIABLE(HIGH_BYTE, LOW_BYTE)      ((HIGH_BYTE << 8) | (LOW_BYTE & 0xFF))

class CI2C
{
	private:
	
	public:
		CI2C();
		
		uint8_t	Start(uint8_t addr, uint8_t dir);
		void	SendCommand(uint8_t cmd);
		
		uint8_t ReceiveData();
		void SendData(uint8_t addr, uint8_t dir);
};


#endif /* CI2C_H_ */