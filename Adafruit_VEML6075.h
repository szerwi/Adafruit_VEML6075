/*!
 * @file Adafruit_VEML6075.h
 *
 * Designed specifically to work with the VEML6075 sensor from Adafruit
 * ----> https://www.adafruit.com/products/3964
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
 *
 * MIT license, all text here must be included in any redistribution.
 *
 */

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#define VEML6075_ADDR         (0x10) ///< I2C address (cannot be changed)
#define VEML6075_REG_CONF     (0x00) ///< Configuration register
#define VEML6075_REG_UVA      (0x07) ///< UVA band raw measurement
#define VEML6075_REG_DARK     (0x08) ///< Dark current (?) measurement
#define VEML6075_REG_UVB      (0x09) ///< UVB band raw measurement
#define VEML6075_REG_UVCOMP1  (0x0A) ///< UV1 compensation value
#define VEML6075_REG_UVCOMP2  (0x0B) ///< UV2 compensation value
#define VEML6075_REG_ID       (0x0C) ///< Manufacture ID

#define VEML6075_DEFAULT_UVA_A_COEFF 2.22
#define VEML6075_DEFAULT_UVA_B_COEFF 1.33
#define VEML6075_DEFAULT_UVB_C_COEFF 2.95
#define VEML6075_DEFAULT_UVB_D_COEFF 1.74
#define VEML6075_DEFAULT_UVA_RESPONSE 0.001461
#define VEML6075_DEFAULT_UVB_RESPONSE 0.002591

/**************************************************************************/
/*! 
    @brief  integration time definitions
*/
/**************************************************************************/
typedef enum veml6075_integrationtime {
  VEML6075_50MS,
  VEML6075_100MS,
  VEML6075_200MS,
  VEML6075_400MS,
  VEML6075_800MS,
} veml6075_integrationtime_t;



typedef union {
  struct {
    uint8_t SD:1;
    uint8_t UV_AF:1;
    uint8_t UV_TRIG:2;
    uint8_t UV_HD:1;
    uint8_t UV_IT:2;
    uint8_t reserved:1;
    uint8_t high_byte;
  } bit;
  uint16_t reg;
} veml6075_commandRegister;


/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with VEML6070 sensor IC
*/
/**************************************************************************/
class Adafruit_VEML6075 {
 public:
  Adafruit_VEML6075();

  boolean begin(veml6075_integrationtime_t itime = VEML6075_100MS, 
		bool highDynamic = false, bool forcedReads = false,
		TwoWire *twoWire = &Wire);

  void shutdown(bool sd);

  void setIntegrationTime(veml6075_integrationtime_t itime);
  veml6075_integrationtime_t getIntegrationTime(void);
  void setHighDynamic(bool hd);
  bool getHighDynamic(void);
  void setForcedMode(bool flag);
  bool getForcedMode(void);

  void setCoefficients(float UVA_A, float UVA_B, float UVA_C, float UVA_D,
		       float UVA_response, float UVB_response);

  float readUVA(void);
  float readUVB(void);

  float readUVI(void);

 private:
  void takeReading(void);

  void     writeRegister(uint8_t reg, uint16_t data);
  uint16_t readRegister(uint8_t reg);

  uint16_t _read_delay;

  // coefficients
  float _uva_a, _uva_b, _uvb_c, _uvb_d, _uva_resp, _uvb_resp;
  float _uva_calc, _uvb_calc;

  veml6075_commandRegister _commandRegister;
  TwoWire *_i2c;
};
