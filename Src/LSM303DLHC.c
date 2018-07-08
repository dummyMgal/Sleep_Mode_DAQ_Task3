#include "LSM303DLHC.h"


const uint16_t GN_sensitivityXY[8] = {1,1100,855,670,450,400,330,230};
const uint16_t GN_sensitivityZ[8] = {1,980,760,600,400,355,295,205};
const uint8_t FS_sensitivity[4] = {1, 2, 4, 12};
const uint16_t FS_shift[4] = {0xFFFF>>1};
uint8_t temp;
//-------------ACCELEROMETER FUNCTIONS------------//
	//TODO: Test repeated start autoincrement by reading 2 bytes from a 1 byte location.
			// use the IRx MAGNETO registers

//Struct creators. Create structs with correct initial values reflecting the boot status of the LSM303DLHC
LSM303_A_InitStruct LSM303_A_CreateInitStruct(I2C_HandleTypeDef * hi2c){
	/**
	 * Description: Creates an initialisation struct/struct representation of for accelerometer portion of the
	 * 		LSM303DLHC, matching the startup values of the registers
	 * Author: Marko Galevski
	 * Inputs: I2C_HandleTypeDef *hi2c :: a handle pointing to the requisite I2C bus
	 * Return: The init struct
	 * Date: 2.07.2018
	 */
	LSM303_A_InitStruct IS = {0};
	//Initialise non-zero default values
	IS.hi2c = hi2c;
	IS.XEN = 0x01;
	IS.YEN = 0x01;
	IS.ZEN = 0x01;

	return IS;
}
LSM303_A_InterruptStruct LSM303_A_CreateInterruptStruct(INT_t INTx){
	/**
	 * Description: Creates an struct representing the register layout of the interrupt generators.
	 * Author: Marko Galevski
	 * Input: INT_t INTx :: An enumerated value which specifies whether this struct is for interrupt 1 or 2
	 * Return: LSm303_A_InterruptStruct
	 * Date: 2.07.2018
	 */
	LSM303_A_InterruptStruct IS = {0};
	IS.INTx = INTx;
	return IS;
}
LSM303_A_ClickStruct LSM303_A_CreateClickStruct(){
	/**
	 * Description: Creates a struct representing the click (i.e. hard tap, double tap) register layout.
	 * Author: Marko Galevski
	 * Return: LSM303_A_ClickStruct
	 * Date: 2.07.2018
	 */
	LSM303_A_ClickStruct CS = {0};
	return CS;
}


void LSM303_A_QuickInit(LSM303_A_InitStruct * InitStruct){
	/**
	 * Description: A quick initialisation function which sets up the accel with basic settings such as ODR,
	 * 		full scale, enabled axes, high pass filter, and high resolution mode.
	 * Author: Marko Galevski
	 * Input: LSM303_A_InitStruct :: an init struct generated with the LSM303_A_CreateInitStruct();
	 * Date: 2.07.2018
	 */
	I2C_HandleTypeDef * hi2c = InitStruct->hi2c;
	//volatile HAL_StatusTypeDef status;
	//CR1 Setup
	temp = (InitStruct->ODR << 4) | (InitStruct->LP << 3) |(InitStruct->ZEN << 2)|
					(InitStruct->YEN << 1)| InitStruct->XEN;

	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG1_A, 1, &temp, sizeof(temp), 100);
	//CR2 Setup
	temp = (InitStruct->HPM << 6) | (InitStruct->HPCF << 4) | (InitStruct->FDS_EN << 3) |
				(InitStruct->HPCLICK_EN << 1) | (InitStruct->HPIS2_EN << 1) | (InitStruct->HPIS1_EN);

	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG2_A, 1, &temp, sizeof(temp), 100);

	//set FullScale and High Res Modes
	temp = (InitStruct->FS << 4) | (InitStruct->HR << 3);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG4_A, 1, &temp, sizeof(temp), 100);
	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, CTRL_REG4_A, 1, &temp, sizeof(temp), 100);

	//Debug LEDs to check if full scale is set correctly
	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, ((temp & (0x03 << 4)) >> 4 == InitStruct->FS));
	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, ((temp & (0x03 << 4)) >> 4 != InitStruct->FS));


}
void LSM303_A_FullInit(LSM303_A_InitStruct * InitStruct){
	/**
	 * Description: A full initialisation function, going over all 6 control register settings.
	 * Author: Marko Galevski
	 * Input: LSM303_A_InitStruct * InitStruct :: ""
	 * Date: 2.07.2018
	 */
	I2C_HandleTypeDef * hi2c = InitStruct->hi2c;

	//CR1 Setup
	temp = (InitStruct->ODR << 4) | (InitStruct->LP << 3) |(InitStruct->ZEN << 2)|
					(InitStruct->YEN << 1)| InitStruct->XEN;

	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL<< 1, CTRL_REG1_A, 1, &temp, sizeof(temp), 100);

	//CR2 Setup
	temp = (InitStruct->HPM << 6) | (InitStruct->HPCF << 4) | (InitStruct->FDS_EN << 3) |
				(InitStruct->HPCLICK_EN << 1) | (InitStruct->HPIS2_EN << 1) | (InitStruct->HPIS1_EN);

	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG2_A, 1, &temp, sizeof(temp), 100);

	//CR3 Setup
	temp = (InitStruct->I1_CLICK_EN << 7) | (InitStruct->I1_AOI1_EN << 6) | (InitStruct->I1_AOI2_EN << 5) |
			(InitStruct->I1_DRDY1_EN << 4) | (InitStruct->I1_DRDY2_EN << 3) | (InitStruct->I1_WTM << 2) |
				(InitStruct->I1_OVERRUN << 1);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG3_A, 1, &temp, sizeof(temp), 100);

	//CR4 Setup
	temp = (InitStruct->BDU << 7) | (InitStruct->BLE << 6) | (InitStruct->FS << 4) | (InitStruct->HR << 3) |
				(InitStruct->SIM);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG4_A, 1, &temp, sizeof(temp), 100);

	//CR5 Setup
	temp = (InitStruct->BOOT << 7) | (InitStruct->FIFO_EN << 6) | (InitStruct->LIR_INT1 << 3) |
			(InitStruct->D4D_INT1 << 2 ) | (InitStruct->LIR_INT2 << 1) | (InitStruct->D4D_INT2);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG5_A, 1, &temp, sizeof(temp), 100);

	//CR6 Setup
	temp = (InitStruct->I2_CLICK_EN << 7) | (InitStruct->I2_INT1 << 6) | (InitStruct->I2_INT2 << 5) |
			(InitStruct->BOOT_I1 << 4) | (InitStruct->P2_ACT << 3) | (InitStruct->H_LACTIVE << 1);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CTRL_REG6_A, 1, &temp, sizeof(temp), 100);

}
void LSM303_A_GetQuickConfig(LSM303_A_InitStruct * InitStruct){
	/**
	 * Description: A quick function which returns current states of settings used in quick init.
	 * 		Used mainly for debugging.
	 * Author: Marko Galevski
	 * Input: LSM303_A_InitStruct * InitStruct :: ptr to the desired struct container
	 * Date: 2.07.2018
	 */
	I2C_HandleTypeDef * hi2c = InitStruct->hi2c;

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, CTRL_REG1_A, 1, &temp, sizeof(temp), 100);
	InitStruct->ODR = (ODR_t) ((temp & (0x0F << 4)) >> 4);
	InitStruct->LP = (temp & (0x01 << 3)) >> 3;
	InitStruct->ZEN = (temp & (0x01 << 2)) >> 2;
	InitStruct->YEN = (temp & (0x01 << 1)) >> 1;
	InitStruct->XEN = (temp & 0x01);

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, CTRL_REG2_A, 1, &temp, sizeof(temp), 100);
	InitStruct->HPM = (HPM_t) (temp & (0x03 << 6)) >> 6;
	InitStruct->HPCF = (HPCF_t) (temp & (0x03 << 4)) >> 4;
	InitStruct->FDS_EN = (temp & (0x01 << 3)) >> 3;
	InitStruct->HPCLICK_EN = (temp & (0x01 << 2)) >> 2;
	InitStruct->HPIS2_EN = (temp & (0x01 << 1)) >> 1;
	InitStruct->HPIS1_EN = (temp & 0x01);

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1 ,CTRL_REG4_A, 1, &temp, sizeof(temp), 100);
	InitStruct->FS = (FS_t) (temp & (0x03 << 4)) >> 4;
	InitStruct->HR = (temp & (0x01 << 3)) >> 3;
}


void LSM303_A_INTxConfig(I2C_HandleTypeDef * hi2c, LSM303_A_InterruptStruct * IntStruct){
	/**
	 * Description: Writes the desired interrupt settings to the sensor. The code determines whether INT1
	 * 		or INT2 is being setup.
	 * Author: Marko Galevski
	 * Input: *hi2c :: i2c bus line, *IntStruct :: the requisite interrupt struct
	 * Date: 2.07.2018
	 */
	//Offset the memory addresses by 4 if we're using INT2.
	uint8_t offset = (uint8_t) IntStruct->INTx << 2;

	temp = (IntStruct->AOI << 7) | (IntStruct->sixD << 6) | (IntStruct->ZHIE << 5) | (IntStruct->ZLIE << 4) |
					(IntStruct->YHIE << 3) | (IntStruct->YLIE << 2) | (IntStruct->XHIE << 1) | (IntStruct->XLIE);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, INT1_CFG_A + offset, 1, &temp, sizeof(temp), 100);

	temp = IntStruct->THS;
	temp &= ~(0x01 << 7); //Clear the seventh bit
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL <<1, INT1_THS_A + offset, 1, &temp, sizeof(temp), 100);

	temp = IntStruct->DUR;
	temp &= ~(0x01 << 7);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, INT1_DURATION_A + offset, 1, &temp, sizeof(temp), 100);
}
void LSM303_A_GetINTxConfig(I2C_HandleTypeDef * hi2c, LSM303_A_InterruptStruct * IntStruct){
	/**
	 * Description: A get function to find out the current interrupt register settings. Useful for debugging
	 * Author: Marko Galevski
	 * Input: *hi2c :: i2c bus line, *IntStruct :: interrupt struct container
	 * Date: 2.07.2018
	 */
	//Offset the memory accesses by 4 if we're using INT2.
	uint8_t offset = (uint8_t) IntStruct->INTx << 2;
	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, INT1_CFG_A + offset, 1, &temp, sizeof(temp), 100);
	IntStruct->AOI = (temp & (0x01 << 7)) >> 7;
	IntStruct->sixD = (temp & (0x01 << 6)) >> 6;
	IntStruct->ZHIE = (temp & (0x01 << 5)) >> 5;
	IntStruct->ZLIE = (temp & (0x01 << 4)) >> 4;
	IntStruct->YHIE = (temp & (0x01 << 3)) >> 3;
	IntStruct->YLIE = (temp & (0x01 << 2)) >> 2;
	IntStruct->XHIE = (temp & (0x01 << 1)) >> 1;
	IntStruct->XLIE = (temp & 0x01);

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, INT1_THS_A + offset, 1, &temp, sizeof(temp), 100);
	IntStruct->THS = temp;

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, INT1_DURATION_A + offset, 1, &temp, sizeof(temp), 100);
	IntStruct->DUR = temp;
}
void LSM303_A_ClickConfig(I2C_HandleTypeDef * hi2c, LSM303_A_ClickStruct * ClickStruct){
	/**
	 * Description: Prepares the click register based on settings provided in clickstruct.
	 * Author: Marko Galevski
	 * Input: *hi2c :: i2c bus line, *ClickStruct :: Pointer to the click struct generated and setup earlier.
	 * Date: 2.07.2018
	 */
	temp = (ClickStruct->ZD << 5) | (ClickStruct->ZS << 4) | (ClickStruct->YD << 3) | (ClickStruct->YS << 2) |
				(ClickStruct->XD << 1) | (ClickStruct->XS);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CLICK_CFG_A, 1, &temp, sizeof(temp), 100);

	temp = ClickStruct->THS;
	temp &= ~(0x01 << 7); //clear 7th bit
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, CLICK_THS_A, 1, &temp, sizeof(temp), 100);

	temp = ClickStruct->TimeLimit;
	temp &= ~(0x01 << 7);
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, TIME_LIMIT_A, 1, &temp, sizeof(temp), 100);

	temp = ClickStruct->TimeLatency;
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, TIME_LATENCY_A, 1, &temp, sizeof(temp), 100);

	temp = ClickStruct->TimeWindow;
	HAL_I2C_Mem_Write(hi2c, SAD_ACCEL << 1, TIME_WINDOW_A, 1, &temp, sizeof(temp), 100);

}
void LSM303_A_GetClickConfig(I2C_HandleTypeDef * hi2c, LSM303_A_ClickStruct * ClickStruct){
	/**
	 * Description: A get function for the click register. Useful for debugging.
	 * Author: Marko Galevski
	 * Input: *hi2c, *ClickStruct
	 * Date: 2.07.2018
	 */
	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, CLICK_CFG_A, 1, &temp, sizeof(temp), 100);
	ClickStruct->ZD = (temp & (0x01 << 5)) >> 5;
	ClickStruct->ZS = (temp & (0x01 << 4)) >> 4;
	ClickStruct->YD = (temp & (0x01 << 3)) >> 3;
	ClickStruct->YS = (temp & (0x01 << 2)) >> 2;
	ClickStruct->XD = (temp & (0x01 << 1)) >> 1;
	ClickStruct->XS = (temp & 0x01);

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, CLICK_THS_A, 1, &temp, sizeof(temp), 100);
	ClickStruct->THS = temp;

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, TIME_LIMIT_A, 1, &temp, sizeof(temp), 100);
	ClickStruct->TimeLimit = temp;

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, TIME_LATENCY_A, 1, &temp, sizeof(temp), 100);
	ClickStruct->TimeLatency = temp;

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, TIME_WINDOW_A, 1, &temp, sizeof(temp), 100);
	ClickStruct->TimeWindow = temp;
}

//TODO: change axis read to return in m/s^2 and not %fs
int16_t LSM303_A_Read_Axis(I2C_HandleTypeDef * hi2c, FS_t FS, axes_offset_a offset){
	/**
	 * Description: A single axis read function. Reading based on full scale setting. Will convert in the future.
	 * Author: Marko Galevski
	 * Inputs: *hi2c :: bus line, FS :: The full scale setting. Take it from the init struct,
	 * 		offset :: the axis to read. Check the enum.
	 * Return: The reading in int16_t.
	 * Date: 2.07.2018
	 */
	volatile uint16_t data;
	int16_t signed_data;
	uint8_t sensitivity = FS_sensitivity[(int) FS];

	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, OUT_X_H_A + offset, 1, &temp, sizeof(temp), 200);
	data = (temp << 8);
	HAL_I2C_Mem_Read(hi2c, SAD_ACCEL << 1, OUT_X_L_A + offset, 1, &temp, sizeof(temp), 200);
	data |= (temp);
	if (data & 0x8000)
		signed_data = -1*(~(data-1));
	else signed_data = data;

	signed_data /= sensitivity;
	return signed_data;
}
int16_t * LSM303_A_Read_All(I2C_HandleTypeDef * hi2c, FS_t FS){
	/**
	 * Description: Multi axis read which returns a 1x3 array of results
	 * Author: Marko Galevski
	 * Inputs: *hi2c :: i2c bus line, FS :: the full scale setting. Get it from the initstruct.
	 * Return: 1x3 array of readings
	 * Date: 2.07.2018
	 */
	static int16_t results[3]; //Static allocation ensures the results stay in the same location and
								//can be read sequentially reliably.
	results[0] = LSM303_A_Read_Axis(hi2c, FS, x_a);
	results[1] = LSM303_A_Read_Axis(hi2c, FS, y_a);
	results[2] = LSM303_A_Read_Axis(hi2c, FS, z_a);
	return results;
}

//-----------END ACCELEROMETER FUNCTIONS----------//

//-----------MAGNETOMETER FUNCTIONS---------------//
LSM303_M_InitStruct LSM303_M_CreateInitStruct(I2C_HandleTypeDef * hi2c){
	/**
	 * Description: Creates an initialisation structure for the magnetometer. It must read some default values
	 * 			due to lack of information in the datasheet
	 * Author: Marko Galevski
	 * Input: *hi2c :: the i2c bus line
	 * Return: LSM303_M_InitStruct Initialised to default values.
	 * Date: 2.07.2018
	 */
	LSM303_M_InitStruct IS = {0};
	IS.hi2c = hi2c;
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, CRB_REG_M, 1, &temp, sizeof(temp), 100);
	IS.GN = (GN_t) (temp & (0x03 << 6)) >> 6;
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, MR_REG_M, 1, &temp, sizeof(temp), 100);
	IS.MD = (MD_t) (temp & 0x03);
	return IS;
}

void LSM303_M_Init(LSM303_M_InitStruct * InitStruct){
	/**
	 * Description: Writes the contents of the initstruct to the magnetometer
	 * Author: Marko Galevski
	 * Input: *InitStruct :: generated earlier.
	 * Date: 2.07.2018
	 */
	I2C_HandleTypeDef * hi2c = InitStruct->hi2c;

	//CRA_M Setup
	temp = (InitStruct->DO << 2);
	HAL_I2C_Mem_Write(hi2c, SAD_MAGNETO << 1, CRA_REG_M, 1, &temp, sizeof(temp), 100);

	//CRB_M Setup
	temp = (InitStruct->GN << 5);
	HAL_I2C_Mem_Write(hi2c, SAD_MAGNETO << 1, CRB_REG_M, 1, &temp, sizeof(temp), 100);

	//MR_REG Setup
	temp = (InitStruct->MD);
	HAL_I2C_Mem_Write(hi2c, SAD_MAGNETO << 1, MR_REG_M, 1, &temp, sizeof(temp), 100);
}

//TODO: Test output data from the Magnet read
float LSM303_M_Read_Axis(I2C_HandleTypeDef * hi2c, GN_t GN, axes_offset_m offset){
	/**
	 * Description: Single axis read of the magnetometer. Output not yet tested
	 * Author: Marko Galevski
	 * Input: *hi2c :: i2c bus line, GN :: the magnetmeters gain, offset :: axis
	 * Return: reading in float format. Must check.
	 * Date: 2.07.2018
	 */
	uint16_t data;
	int16_t signed_data;
	float result;
	//extern uint16_t GN_sensitivityXY[8];
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, OUT_X_L_M + offset, 1, &temp, sizeof(temp), 100);
	//extern uint16_t GN_sensitivityZ[8];
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, OUT_X_H_M + offset, 1, &temp, sizeof(temp), 100);
	data = (temp << 8);
	data |= (temp) ;

	if (data & 0x8000)
		signed_data = -1*(~(data-1));
	else signed_data = data;

	result = (float) signed_data;
	if (offset == z_m)
		result/= GN_sensitivityZ[(int) GN];
	else
		result /= GN_sensitivityXY[(int) GN];

	return result;
}
float * LSM303_M_Read_All(I2C_HandleTypeDef * hi2c, GN_t GN){
	/**
	 * Description: Multi-axis read of the magnetometer. Returns an array of readings
	 * Author: Marko Galevski
	 * Input: *hi2c, GN
	 * Output: 1x3 array of readings
	 * Date: 2.07.2018
	 */
	static float results[3];
	results[0] = LSM303_M_Read_Axis(hi2c, GN, x_m);
	results[1] = LSM303_M_Read_Axis(hi2c, GN, y_m);
	results[2] = LSM303_M_Read_Axis(hi2c, GN, z_m);
	return results;
}

//------------END MAGNETOMETER FUNCTIONS----------//

//------------THERMOMETER FUNCTIONS---------------//
void LSM303_T_Init(I2C_HandleTypeDef * hi2c){
	/**
	 * Description: The thermometer is a submodule of the magnetometer. Its initialisation process is
	 * 		incredibly simple, so no dedicated struct is required
	 * Author: Marko Galevski
	 * Input: *hi2c :: i2c bus line
	 * Date: 2.07.2018
	 */
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, CRA_REG_M, 1, &temp, sizeof(temp), 100);
	temp |= 0x01 << 7;
	HAL_I2C_Mem_Write(hi2c, SAD_MAGNETO << 1, CRA_REG_M, 1, &temp, sizeof(temp), 100);
}

//TODO: Test outpt of temp sensor
float LSM303_T_ReadTemp(I2C_HandleTypeDef * hi2c){
	/**
	 * Description: Read of the thermometer. Not tested, should work. Returns temp in degrees celsius.
	 * Author: Marko Galevski
	 * Input: *hi2c
	 * Output: Reading in float format (-40*C - 85*C)
	 * Date: 2.07.2018
	 *
	 */
	int16_t data;
	float result;
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, TEMP_OUT_H_M, 1, &temp, sizeof(temp), 100);
	data = temp << 8;
	HAL_I2C_Mem_Read(hi2c, SAD_MAGNETO << 1, TEMP_OUT_L_M, 1, &temp, sizeof(temp), 100);
	data |= temp;
	data = data >> 4; //Shift 4 bits to the right because temp is 12 bits
	if (data & 0x0800)  //test if the MSB is 1 (i.e. negative)
		data = ~(data - 1) & ~(0xF000); //get twos complement and don't forget to clear the upper nibble
	result = (float) data;
	result /= 8;  //from datasheet
	return result;

}

//-------------END THERMOMETER FUNCTIONS----------//
