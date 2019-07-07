#include "PCA9685.h"

static const int default_device_signature = 0x40;
static const int mode_1_register = 0x00;
static const int mode_2_register = 0x01;
static const int pre_scale = 0xFE; // frequency adjust register. 

// first of four bytes of the output registers. 
static const int output_pins[ 16 ] = { 0x06, 0x0A, 0x0E, 0x12 
                                     , 0x16, 0x1A, 0x1E, 0x22
                                     , 0x26, 0x2A, 0x2E, 0x32
                                     , 0x36, 0x3A, 0x3E, 0x42 
                                     };


// address is adjustable. 
uint8_t all_call_I2C_addr = 0x70; // 0x70 is an all call in the event there is more than one PCA9685.
// PCA9685 PCA9685( sda_pin, scl_pin, OE ); 

////////////////////////////////////////////////////////////////////////////// INITIALIZERS //////////////////////////////////////////////////////////////////////////////

void pca9685::begin( )
{
  Wire.setClock(400000); // default wire library to the fastest setting.
  PCA9685.set_mode( 1, 0b10100001 ); // 0b10100001
  PCA9685.set_mode( 2, 0b00000100  ); //0b00000100
  PCA9685.set_output_frequency( 1600 ); // ( globally ) sets  default PCA9685 to the fastest setting.  
}

bool pca9685::set_mode( uint8_t mode, uint8_t register_values )
{
  if ( mode == 1 )
  {
    PCA9685.write( mode_1_register, register_values ); 
    return true;
  }
  else if ( mode == 2 )
  {
    PCA9685.write( mode_2_register, register_values );
    return true;
  }
  else if ( mode > 2 )
  {
    return false;
  }
  else if ( mode < 1 )
  {
    return false;
  }
}

void pca9685::set_output_frequency ( float frequency )
{
  uint8_t write_frequency;
  if( frequency > 1526 )
  {
    frequency = 1526; // The maximum allowed frequency of the PCA9685.
  }
  else if ( ( frequency < 24 ) && ( frequency > 0 ) )
  {
    frequency = 24; // The minimum allowed frequency of the PCA9685.
  }
  else if ( frequency <= 0 )
  {
    PCA9685.clear();
  }
  write_frequency = 25000000 / ( 4096 * frequency );
  PCA9685.set_mode(1, 0b10110001 );
  delay( 1 );  
  PCA9685.write( pre_scale, write_frequency ); 
  PCA9685.set_mode(1, 0b10100001 );
  delay( 1 );
}

////////////////////////////////////////////////////////////////////////////////// CONTROL FUNCTIONS //////////////////////////////////////////////////////////////////////////////////

// Globally resets all PCA9685's in the I2C bus. 
void pca9685::reset()
{
  PCA9685.write( mode_1_register, 0b0000110 ); 
}

// writes zero's to the PWM output pins. 
void pca9685::clear( void ) 
{
  for( int a = 0; a < 16; a++ )
  {
    PCA9685.analogWrite( a, 0 );
  }
}

////////////////////////////////////////////////////////////////////////////// INPUT / OUTPUT FUNCTIONS //////////////////////////////////////////////////////////////////////////////

uint8_t pca9685::read( uint8_t device_addr, uint8_t addr )
{
  Wire.beginTransmission ( device_addr );
  Wire.write( addr );
  Wire.endTransmission();
  Wire.requestFrom( (uint8_t)device_addr, (uint8_t)1 ); // throws a warning if addr and number of bytes is not flagged with uint8_t. 
  return Wire.read(); 
}

// 12 bit input. 
// There are 2 12 bit registers per LED output - 24 bits. 12bits for
// on time and 12 bits for off time. 
// If device signature is not written to, it will default to the original 
// default signature. 
void pca9685::analogWrite(uint8_t output_pin, uint16_t analog_val, uint8_t device_signature )
{
  analog_val = min(analog_val, uint16_t(4095));
  output_pin = min(output_pin, 15);
  if ( analog_val == 4095 )
  {
    PWM_parse( output_pin, 4096, 0, device_signature );
  }
  else if ( analog_val == 0 )
  {
    PWM_parse( output_pin, 0, 4096, device_signature );
  }
  else
  {
    PWM_parse( output_pin, 0, analog_val, device_signature );
  }
}

void pca9685::PWM_parse( uint8_t output_pin, uint16_t on, uint16_t off, uint8_t device_signature )
{
  Wire.beginTransmission( device_signature );
  Wire.write( output_pins[ output_pin ] );
  Wire.write( on );
  Wire.write( on >> 8 );
  Wire.write( off );
  Wire.write( off >> 8 );
  Wire.endTransmission();
}

// device_signature is optional, as it will default to 0x40.
//
void pca9685::write( uint8_t addr, uint8_t val, uint8_t device_signature )
{
  Wire.beginTransmission( device_signature );
  Wire.write( addr );
  Wire.write( val );
  Wire.endTransmission();    
}
