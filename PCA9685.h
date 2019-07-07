#include <Arduino.h>
#include <Wire.h>
#include <stdbool.h>
#include <stdint.h>

#define DEFAULT_DEVICE_SIGNATURE 0x40 /**< Mode Register 1 */
#define DEVICE_ALL_CALL 0X70
#define DEFAULT_WIRE_SPEED 400000 

class pca9685
{
  public:
  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // Default constructor
  //
  pca9685( uint8_t sda_pin, uint8_t scl_pin, uint8_t OE )
  : sda_pin_( sda_pin )
  , scl_pin_( scl_pin )
  , OE_( OE )
  , default_device_signature_( DEFAULT_DEVICE_SIGNATURE )
  {
    #ifdef ESP8266
    Wire.begin( sda_pin_, scl_pin_ );
    #endif
    Wire.begin();
  }

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // Initializes a PCA9685 by using a user set device signature. The PCA 9685 
  // has a 6-bit address-set, and can therefore range between 0x41 to 0x7F
  // 
  void begin( );
 

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // This is a specific use case function, and will likely not need any use. But
  // this function will be here in the event the user decides to change register
  // values ( mode 1 & mode 2 ) bit by bit. 
  //
  // Param[ in ]
  // mode : is a integer range of 1 or 2, determining which mode register the user
  // intends to write to. 
  //
  // Param[ in ]
  // register_values : should be entered as a byte in binary according to the data
  // - sheet. In the event the parameters are exceeded for any reason, this 
  // function will return a " false " notifying the user of a failed write. 
  //
  bool set_mode( uint8_t mode, uint8_t register_values );

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // Resets ( all possible ) PCA9685's on the I2C bus. 
  // 
  void reset( void );

  
  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // Effectively, this function behaves like a "clear_all" function would. 
  // However, this function does not clear the output registers as one would 
  // expect from a "clear_all" function. Rather, a specific write cycle must be
  // met to actually write zeros to the output pins. 
  void clear( void ); 

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // The PCA9685 has an adjustable PWM frequency - up to 1.6khz which can be 
  // quite a bit faster than the Arduino's native frequency of 490hz-980hz.  
  // 
  // param[ in ]
  // frequency : is going to be a value that ranges up to 1600hz. Any value beyond
  // 1600hz will be rounded down to 1600hz. 
  // 
  // By default, this library assumes the user will want the maximum intended 
  // performance, and therefore the default frequency is set to the fastest speed
  // of 1600hz. Though, this can be changed in setup.  
  //
  void set_output_frequency( float frequency );

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // This function does a specific read command to get information from a single
  // chip at a time, one byte at a time. 
  //
  // param [ in ] 
  // device_addr : will not have a default address, and must be written to have a
  // successful read command. This parameter is the I2C signature of the device
  // the user intends to read from.
  //
  // param [ in ] 
  // addr : is the register the user intends to read from.
  // 
  uint8_t read( uint8_t device_addr, uint8_t addr );
  

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // An "analog" of Arduino's analogWrite to PCA9685's version. They will 
  // behave identically with the exception that a device signature is needed.
  // 
  // Param[ in ]
  // device_signature : is the I2C address needed to select which - if there are 
  // more than one - I2C device the user is referring to. The PCA9685 in 
  // particular can have an adjustable I2C address so this is especioially 
  // important to not leave as a default. 
  // 
  // Param[ in ]:
  // output_pin : is another I2C address that specifically refers to which PWM pin
  // the user is intending to write to. 
  // 
  // Param[ in ]:
  // pwm_val : is the actual analog value the user intends to output. 
  //
  void analogWrite( uint8_t output_pin, uint16_t analog_val, uint8_t device_signatuture = DEFAULT_DEVICE_SIGNATURE);

  ////////////////////////////////////////////////////////////////////////////// PRIVATE //////////////////////////////////////////////////////////////////////////////
  
  private:
  uint8_t sda_pin_;
  uint8_t scl_pin_;
  uint8_t OE_;
  uint8_t default_device_signature_;

  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // This function is used as an inbetween from analogWrite to the PCA9685. The 
  // PCA9685 has four registers that store high/low times. Rather than a range
  // of values like 0-4095, these registers have to be written to in a specific
  // way. This function takes care of this.   
  // 
  // Param[ in ]  
  // output_pin : is an array location ( 0 - 15 ) which contains the register location
  // of the output pin the user intends to write to. Any value above or below the 
  // range of 0-15 will be clipped to 0-15 respectively. 
  //
  // Param[ in ]  
  // on : 
  // Param[ in ]  
  // Param[ in ]  
  void PWM_parse(  uint8_t output_pin, uint16_t on, uint16_t off, uint8_t device_signature );
  
  //////////////////////////////////////////////////////////////////////////////
  // Purpose:
  // This function sends a single byte to a specifically selected device on the 
  // bus. The only difference between this function and the preceding write
  // function is this has the option to select which device address is being used.
  // Otherwise, device_addr will default to 0x40 - the default PCA9685 I2C address. 
  //
  // This function is private, as these are internal handlers.
  //
  // param [ in ] 
  // addr is the register the user intends to write to.
  // 
  // param [ in ] 
  // val is the byte the user intends to write inside the PCA9685's register. 
  //
  void write( uint8_t addr, uint8_t val, uint8_t device_signature = DEFAULT_DEVICE_SIGNATURE );

}

extern PCA9685;
