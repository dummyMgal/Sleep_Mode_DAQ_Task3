#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#define SAD_ACCEL 0x19

//ACCELEROMETER REGISTERS
#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24
#define CTRL_REG6_A 0x25
#define REFERENCE_A 0x26
#define STATUS_REG_A 0x27
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
#define FIFO_CTRL_REG_A 0x2E
#define FIFO_SRC_REG_A 0x2F
#define INT1_CFG_A 0x30
#define INT1_SRC_A 0x31
#define INT1_THS_A 0x32
#define INT1_DURATION_A 0x33
#define INT2_CFG_A 0x34
#define INT2_SRC_A 0x35
#define INT2_THS_A 0x36
#define INT2_DURATION_A 0x37
#define CLICK_CFG_A 0x38
#define CLICK_SRC_A 0x39
#define CLICK_THS_A 0x3A
#define TIME_LIMIT_A 0x3B
#define TIME_LATENCY_A 0x3C
#define TIME_WINDOW_A 0x3D

//MAGNETOMETER REGISTERS
#define SAD_MAGNETO 0x1E
#define CRA_REG_M 0x00
#define CRB_REG_M 0x01
#define MR_REG_M 0x02
#define OUT_X_H_M 0x03
#define OUT_X_L_M 0x04
#define OUT_Z_H_M 0x05
#define OUT_Z_L_M 0x06
#define OUT_Y_H_M 0x07
#define OUT_Y_L_M 0x08
#define SR_REG_M 0x09

//Magnetometer identification registers containing constant values
#define IRA_REG_M 0x0A
#define IRB_REG_M 0x0B
#define IRC_REG_M 0x0C

//TEMPERATURE SENSOR REGISTERS
#define TEMP_OUT_H_M 0x31
#define TEMP_OUT_L_M 0x32



//-------------ACCELEROMETER TYPEDEFS-------------//

//Accelerometer Output Data Rate Enum
typedef enum {

	POWER_DOWN = 0x00,
	ODR_1HZ = 0x01,
	ODR_10HZ = 0x02,
	ODR_25HZ = 0x03,
	ODR_50HZ = 0x04,
	ODR_100HZ = 0x05,
	ODR_200HZ = 0x06,
	ODR_400HZ = 0x07,
	ODR_LP_1620HZ = 0x08,
	ODR_LP_5376HZ = 0x09
}ODR_t;

//Accelerometer HPF Mode Enum
typedef enum {

	NORMAL_MODE = 0x00,
	REFERENCE_SIGNAL = 0x01,
	AUTORESET_ON_INT = 0x03
}HPM_t;

//Accelerometer HPF Cutoff Frequency Enum
typedef enum {
	CUTOFF_1 = 0x00,
	CUTOFF_2 = 0x01,
	CUTOFF_3 = 0x02,
	CUTOFF_4 = 0x03
}HPCF_t;

//Accelerometer Full Scale Select Enum
typedef enum {
	FS_2G = 0x00,
	FS_4G = 0x01,
	FS_8G = 0x02,
	FS_16G = 0x03
}FS_t;

//Accelerometer Sensitivity Per FS setting
extern const uint8_t FS_sensitivity[4];

//axis memory offset typedef
typedef enum {
	x_a = 0,
	y_a = 2,
	z_a = 4
}axes_offset_a;

//Accelerometer Init Struct Tdef
typedef struct {
		I2C_HandleTypeDef * hi2c;
		//CR1
		ODR_t ODR;// = POWER_DOWN;
		uint8_t LP;// = 0x00;
		uint8_t XEN;// = 0x01;
		uint8_t YEN;// = 0x01;
		uint8_t ZEN;// = 0x01;
		//CR2
		HPM_t HPM;// = 0x00;
		HPCF_t HPCF;// = 0x00;
		uint8_t FDS_EN;// = 0x00;
		uint8_t HPCLICK_EN;//= 0x00;
		uint8_t HPIS1_EN;// = 0x00;
		uint8_t HPIS2_EN;// = 0x00;
		//CR3
		uint8_t I1_CLICK_EN;// = 0x00;
		uint8_t I1_AOI1_EN;// = 0x00;
		uint8_t I1_AOI2_EN;// = 0x00;
		uint8_t I1_DRDY1_EN;// = 0x00;
		uint8_t I1_DRDY2_EN;// = 0x00;
		uint8_t I1_WTM;// = 0x00;
		uint8_t I1_OVERRUN;// = 0x00;
		//CR4
		uint8_t BDU;// = 0x00;
		uint8_t BLE;// = 0x00;
		FS_t FS;// = FS_2G;
		uint8_t HR;// = 0x00;
		uint8_t SIM;// = 0x00;
		//CR5
		uint8_t BOOT;// = 0x00;
		uint8_t FIFO_EN;// = 0x00;
		uint8_t LIR_INT1;// = 0x00;
		uint8_t D4D_INT1;// = 0x00;
	    uint8_t LIR_INT2;// = 0x00;
	    uint8_t D4D_INT2;// = 0x00;
	    //CR6
	    uint8_t I2_CLICK_EN;// = 0x00;
	    uint8_t I2_INT1;// = 0x00;
	    uint8_t I2_INT2;// = 0x00;
	    uint8_t BOOT_I1;// = 0x00;
	    uint8_t P2_ACT;// = 0x00;
	    uint8_t H_LACTIVE;// = 0x00;

} volatile LSM303_A_InitStruct;

//Interrupt Select TypeDef
typedef enum {
	INT1 = 0,
	INT2 = 1
}INT_t;

//Accelerometer Interrupt Struct Tdef
typedef struct{
	INT_t INTx;
	//INTx Config Reg
	uint8_t AOI;// = 0x00;
	uint8_t sixD;// = 0x00;
	uint8_t ZHIE;// = 0x00;
	uint8_t ZLIE;// = 0x00;
	uint8_t YHIE;// = 0x00;
	uint8_t YLIE;// = 0x00;
	uint8_t XHIE;// = 0x00;
	uint8_t XLIE;// = 0x00;

	//INTx THS
	uint8_t THS;// = 0x00;

	//INTx DUR
	uint8_t DUR;// = 0x00;

}LSM303_A_InterruptStruct;

//Accelerometer Click Struct Tdef
typedef struct{

	//CLICK CFG
	uint8_t ZD;// = 0x00;
	uint8_t ZS;// = 0x00;
	uint8_t YD;// = 0x00;
	uint8_t YS;// = 0x00;
	uint8_t XD;// = 0x00;
	uint8_t XS;// = 0x00;

	uint8_t THS;// = 0x00;

	uint8_t TimeLimit;// = 0x00;

	uint8_t TimeLatency;// = 0x00;

	uint8_t TimeWindow;// = 0x00;

}LSM303_A_ClickStruct;

//-------------END ACCELEROMETER TYPEDEFS---------//

/***************************************************************/

//-------------MAGNETOMETER TYPEDEFS--------------//

//Magnetometer Data Output Enum
typedef enum{
	DO_0_75HZ = 0,
	DO_1_5HZ = 1,
	DO_3_0HZ = 2,
	DO_7_5HZ = 3,
	DO_15HZ = 4,
	DO_30HZ = 5,
	DO_75HZ = 6,
	DO_220HZ = 7
}DO_t;

//Magnetometer Gain Mode Enum
typedef enum{

	//REFER TO TABLE 75 in LSM303DHLC Datasheet
	GN_1g3 = 1,
	GN_1g9 = 2,
	GN_2g5 = 3,
	GN_4g0= 4,
	GN_4g7 = 5,
	GN_5g6 = 6,
	GN_8g1 = 7
}GN_t;

//Sensitivities per gain setting, as described in table 75 of the datasheet
extern const uint16_t GN_sensitivityXY[8];
extern const uint16_t GN_sensitivityZ[8];

//Magentometer Operating Mode Enum
typedef enum{
		MD_CC = 0, //Continuous Conversion Mode
		MD_SC = 1, //Single Conversion Mode
		MD_SP = 2  //Sleep Mode
}MD_t;

//Magnetometer Init Struct
typedef struct {
		I2C_HandleTypeDef * hi2c;

		//CRA_REG_M
		uint8_t TEMP_EN;// = 0x00;
		DO_t DO;// = DO_15HZ;
		//CRB_REG_M
		GN_t GN;// = GN_1g3;
		//MR_REG_M
		MD_t MD;// = MD_CC;

}LSM303_M_InitStruct;

//Magnetometer axis memory offset
typedef enum {
	x_m = 0,
	z_m = 2,
	y_m = 4
}axes_offset_m;

//-------------END MAGNETOMETER TYPEDEFS----------//

//--------------ACCELEROMETER FN PROTOTYPES-------//

LSM303_A_InitStruct LSM303_A_CreateInitStruct(I2C_HandleTypeDef * hi2c);
LSM303_A_InterruptStruct LSM303_A_CreateInterruptStruct(INT_t INTx);
LSM303_A_ClickStruct LSM303_A_CreateClickStruct();

void LSM303_A_QuickInit(LSM303_A_InitStruct * InitStruct);
void LSM303_A_FullInit(LSM303_A_InitStruct * InitStruct);
void LSM303_A_GetQuickConfig(LSM303_A_InitStruct * InitStruct);

void LSM303_A_INTxConfig(I2C_HandleTypeDef * hi2c,LSM303_A_InterruptStruct * IntStruct);
void LSM303_A_GetINTxConfig(I2C_HandleTypeDef * hi2c, LSM303_A_InterruptStruct * IntStruct);
void LSM303_A_ClickConfig(I2C_HandleTypeDef * hi2c, LSM303_A_ClickStruct * ClickStruct);
void LSM303_A_GetClickConfig(I2C_HandleTypeDef * hi2c, LSM303_A_ClickStruct * ClickStruct);

int16_t LSM303_A_Read_Axis(I2C_HandleTypeDef * hi2c, FS_t FS, axes_offset_a offset);
int16_t * LSM303_A_Read_All(I2C_HandleTypeDef * hi2c, FS_t FS);

//------------END ACCELEROMETER FN PROTOTYPES-----//

//--------------MAGNETOMETER FN PROTOTYPES--------//

LSM303_M_InitStruct LSM303_M_CreateInitStruct(I2C_HandleTypeDef * hi2c);

void LSM303_M_Init(LSM303_M_InitStruct * InitStruct);

float LSM303_M_Read_Axis(I2C_HandleTypeDef * hi2c, GN_t GN, axes_offset_m offset);
float * LSM303_M_Read_All(I2C_HandleTypeDef * hi2c, GN_t GN);


//------------END MAGNETOMETER FN PROTOTYPES------//

//------------TEMPERATURE SENSOR FN PROTOTYPES----//
void LSM303_T_Init(I2C_HandleTypeDef * hi2c);
float LSM303_T_ReadTemp(I2C_HandleTypeDef * hi2c);
#endif


