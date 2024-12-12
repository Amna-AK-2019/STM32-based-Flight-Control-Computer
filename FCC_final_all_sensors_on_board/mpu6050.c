#include <mpu6050.h>
#include <main.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

void mpu6050_init()
{
	  //HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials,uint32_t Timeout);
	  HAL_StatusTypeDef ret=  HAL_I2C_IsDeviceReady(&hi2c1, (DEVICE_ADDRESS<< 1)+0,
			  1,100);
	  if (ret == HAL_OK)
	  {
		  printf("The device is ready. \n");
	  }
	  else
	  {
		  printf("The device is not ready. \n");
	  }


	//  HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
	//                                      uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

	  uint8_t temp_data=FS_GYRO_500;
	  HAL_StatusTypeDef ret1= HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS<< 1)+0, REG_CONFIG_GYRO,
	                                      1, &temp_data, 1, 100);
	  if (ret1 == HAL_OK)
	    {
	  	  printf("Configuration of gyroscope. \n");
	    }
	    else
	    {
	  	  printf("Failed to configure gyroscope. \n");
	    }

	  uint8_t temp_data1=FS_ACC_4G;
	  HAL_StatusTypeDef ret2= HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS<< 1)+0, REG_CONFIG_ACC,
										  1, &temp_data1, 1, 100);
	  if (ret2 == HAL_OK)
		{
		  printf("Configuration of accelerometer. \n");
		}
		else
		{
		  printf("Failed to configure accelerometer. \n");
		}

	  uint8_t temp_data2=0;
	  HAL_StatusTypeDef ret3= HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS<< 1)+0, REG_USR_CTRL,
										  1, &temp_data2, 1, 100);
	  if (ret3 == HAL_OK)
		{
		  printf("Exiting from the sleep mode. \n");
		}
		else
		{
		  printf("Failed to exit from the sleep mode. \n");
		}
}

void mpu6050_read()
{
    // HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
    //                                     uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
	    uint8_t data_acc[6];
	    int16_t x_acc, y_acc, z_acc;
	    float Ax, Ay, Az;

	    // Read X-axis data
	    HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_X,1,  data_acc, 2, 100);
	    x_acc = ((int16_t)data_acc[0] << 8) | data_acc[1];

	    // Read Y-axis data
	    HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_Y,1, data_acc, 2, 100);
	    y_acc = ((int16_t)data_acc[0] << 8) | data_acc[1];

	    // Read Z-axis data
	    HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_Z,1, data_acc, 2, 100);
	    z_acc = ((int16_t)data_acc[0] << 8) | data_acc[1];

	    // Convert to floating point values (assuming 16g full scale range, adjust divisor as necessary)
	    Ax = (float)x_acc / 4096.0;
	    Ay = (float)y_acc / 4096.0;
	    Az = (float)z_acc / 4096.0;

	    // Print values
	    printf("x axis accelerometer: %d \n", x_acc);
	    printf("FLOAT VALUE of x axis accelerometer: %.2f \n", Ax);
	    printf("y axis accelerometer: %d \n", y_acc);
	    printf("FLOAT VALUE of y axis accelerometer: %.2f \n", Ay);
	    printf("z axis accelerometer: %d \n", z_acc);
	    printf("FLOAT VALUE of z axis accelerometer: %.2f \n", Az);
	    printf("-----------------------------------------\n");



	    uint8_t data_gyro[6];
		int16_t x_gyro, y_gyro, z_gyro;
		float Gx, Gy, Gz;

		// Read X-axis gyro data
		HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_GYRO_X, 1,data_gyro, 2, 100);
		x_gyro = ((int16_t)data_gyro[0] << 8) | data_gyro[1];
		Gx = (float)x_gyro / 65.5;

		// Read Y-axis gyro data
		HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_GYRO_Y, 1, data_gyro, 2, 100);
		y_gyro = ((int16_t)data_gyro[0] << 8) | data_gyro[1];
		Gy = (float)y_gyro / 65.5;

		// Read Z-axis gyro data
		HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_GYRO_Z,1,data_gyro, 2, 100);
		z_gyro = ((int16_t)data_gyro[0] << 8) | data_gyro[1];
		Gz = (float)z_gyro / 65.5;

		// Print values
		printf("x axis gyroscope: %d \n", x_gyro);
		printf("FLOAT VALUE x axis gyroscope: %.2f \n", Gx);
		printf("y axis gyroscope: %d \n", y_gyro);
		printf("FLOAT VALUE y axis gyroscope: %.2f \n", Gy);
		printf("z axis gyroscope: %d \n", z_gyro);
		printf("FLOAT VALUE z axis gyroscope: %.2f \n", Gz);

		printf("-----------------------------------------\n");

		uint8_t data[2];
		int16_t temperature_raw;
		float temperature_C;

		// Read temperature data
		HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1), REG_DATA_TEMP_H, 1, data, 2, 100);
		temperature_raw = ((int16_t)data[0] << 8) | data[1];

		// Convert raw temperature to degrees Celsius
		temperature_C = (float)temperature_raw / 340.0 + 36.53;

		// Print temperature
		printf("Temperature in Celsius: %.2f \n", temperature_C);
		printf("##########################################\n");


}










