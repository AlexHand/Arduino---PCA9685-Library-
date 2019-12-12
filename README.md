# Arduino---PCA9685-Library-
A I2C Library for the PCA9685 12bit 16-channel PWM IC. 

There are many libraries available for this IC. Notably, there is even one by Adafruit. 
Their library works well too, their demo code proves it. Like many other libraries
onlines, though, I have some personal nit picks with it. The biggest issue I have with the 
Adafruit library is the IC is referred to as a servo library. If you are using this IC
for Servos, this is great! But, I wanted to use it solely as a PWM expansion modueles for 
the already ill-equipped Arduino Uno boards. I don't want a servo.write(val) command to 
control the output on an IN-9 nixie tube. So I wrote a VERY simple user library to simply
output a PWM to the pins of this IC. 

That being said, here are the functions available to the user to operate this library. 


PCA9685.begin() 
This loads default parameters, setting the I2C bus to 400khz, and register default modes. 

PCA9685.set_output_frequency( float )
This function accepts any number, but will be clamped to either 1526hz, or 24hz. Those 
numbers are hardeware limitations of the PCA9685 IC. 

PCA9685.set_mode( uint8_t mode, uint8_t register_values )
This function is for advanced users. The PCA9685 has quite a few built in functions that
this function directly manipulates. For instance, you may change if the register auto
incrememts I2C writes or not. If the user were to disable this for some reason, this library
will break. I have personal defaults set, and I believe this would be what most other 
people will want to use to. But, I left this function public in the event a power user wants
more control over their device. 

PCA9685.clear() 
This is a useful function, as the PCA9685 registers can behave like RAM and just output 
nonsense on powerup. This function just ensures the output will write all 0's. 

PCA9685.reset()
Softeare reset of the IC. This doesn't really work yet. 

PCA9685.read( uint8_t addr, uint8_t device_signature)
Returns a uint8_t register value from the IC. device_signature is the default I2C signature
of the PCA9685. addr is the register the user wishes to read from. 

PCA9685.analogWrite( uint8_t output_pin, uint16_t analog_val, uint8_t device_signature )
This function is the gold star of the whole library. Only two parameters are require for use. 
output_pin is which of the 16 PWM pins you intend to write to. analog_val is the 12bit PWM val
the user is going to write to which output pin. device_signature is optional, as it will default
to 0x40, which is the default I2C signature of the PCA9685 IC. 
