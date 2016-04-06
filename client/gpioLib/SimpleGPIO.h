/**
 * @file    SimpleGPIO.h
 * @author  DFLM (dflarios@dte.us.es)
 * @author  Mihaita Lupoiu (myhay.360@gmail.com)
 * @author  Jose Antonio Belloch (jobelrod@iteam.upv.es)
 * @date    February, 2016
 * @version 1.0b
 * @bug     Not known bugs in SimpleGPIO.h.
 * @brief   Definition of functions to manage GPIO.
 *
 * This function uses the generic sys class GPIO drivers.
 * These drivers are compatible with many ARM linux boards, as Nvidia Jetson.
 * More info in: http://elinux.org/Jetson/GPIO
 * Based on the work of Derek Molloy Work, School of Electronic Engineering, 
 * Dublin CityUniversity. www.derekmolloy.ie
 */
  
/**
 * @defgroup GPIO The GPIO library group.
 * @ingroup  UpperLevelGroup
 * @brief    functions to be used to manage GPIO in Linux boards.
 * @{
 */
#ifndef SIMPLEGPIO_H
#define SIMPLEGPIO_H


//******************************************************************************
//* INCLUDES                                                                   *
//******************************************************************************
#include <string>						// std::string.


//******************************************************************************
//* TYPEDEFS                                                                   *
//******************************************************************************
/**
 * @brief Define the GPIO modes.
 */
enum class PinDirection {
	INPUT,								///< GPIO as Input.
	OUTPUT,								///< GPIO as Output.
	NONE								///< GPIO not initialized.
};

/**
 * @brief Define the GPIO values, used in read / write operations.
 */
enum class PinValue {
	LOW,								///< GPIO pin in low state.
	HIGH								///< GPIO pin in high state.
};

/**
 * @brief Define the GPIO edges use to generate interruptions.
 *
 * These values are used in inputs' async mode.
 */
enum class PinEdge {
	RISSING,							///< Interrupt generated in 0->1 transition.
	FALLING,							///< Interrupt generated in 1->0 transition.
	BOTH								///< Interrupt generated in both transitions.
};


//******************************************************************************
//* CONSTANTS                                                                  *
//******************************************************************************
#define WITH_TYPE_TEST					///< If defined, type is tested in get/set.

namespace{
const std::string gpioPath="/sys/class/gpio";///< Path to the GPIO functions.
}

//******************************************************************************
//* GLOBAL VARIABLES                                                           *
//******************************************************************************


//******************************************************************************
//* FUNCTION PROPTOTIPES                                                       *
//******************************************************************************

 
 //******************************************************************************
//* CLASS PROPTOTIPES                                                          *
//******************************************************************************
/**
 * @brief The Gpio Class.
 *
 * This class define the main methods to read and write in Gpio pins.
 */
class Gpio {
public:
	/**
	 * @brief The Gpio constructor.
	 *
	 * Create a new GPIo device, and configures it as input or output.
	 * @param gpio Gpio pin number (see board datasheet).
	 * @param direction Defines the Gpio as input or output (default: input).
	 */
    Gpio(const unsigned int& gpio, const PinDirection& direction=PinDirection::INPUT);
	
	/**
	 * @brief The Gpio destructor.
	 *
	 * It closes the pin and free gpio resources.
	 */
	~Gpio();
	
	/**
	 * @brief The setDirection method.
	 *
	 * If change the gpio pin configuration.
	 * @param direction Can be input or output.
	 */	
	void setDirection(const PinDirection& direction);
	
	/**
	 * @brief The setDirection method.
	 *
	 * Return if the Gpio pin is an input or an output.
	 * @return The direction of the Gpio pin.
	 */	
    inline PinDirection getDifection() const { return pinDir; }
	
	/**
     * @brief The setAsync method.
	 *
	 * If change the gpio pin configuration.
	 * @param callback Function executed when appear the async method.
	 * @param edge The edge that generates the callback (Rissing, falling or both).
	 * @param initFunction Initialization function of async thread (if any).
	 */
    void setAsync(void (*callback)(PinValue), const PinEdge& edge=PinEdge::RISSING,
        void (*initFunction)(void)=NULL);
	
	/**
     * @brief The removeAsync method.
	 *
	 * Set input mode as synchronous
	 */	
    void removeAsync();
	
	/**
	 * @brief The getAsync method.
	 *
	 * Return if the input is in asynchronous or synchronous mode.
	 * @return The mode of the Gpio pin.
	 */
	inline bool getAsync() const { return async; }
	
	/**
	 * @brief The setValue method.
	 *
	 * Set the output value of a Gpio Pin.
	 * @param value It can be LOW or HIGH state.
	 */	
    void setValue(const PinValue& value);
	
	/**
	 * @brief The getValue method.
	 *
	 * Read de value of an input Pin.
	 * @return It can be LOW or HIGH state.
	 */
    PinValue getValue() const;

	/**
	 * @brief The getPinName method.
	 *
	 * Return Gpio the number of the.
	 * @return The pin Number, as a string.
	 */
	inline std::string getPinName() const { return pinName; }
	
private:

	/**
	 * @brief The threadFunction method.
	 *
	 * This function is executed in asynchronous mode, to test if the imput pin 
	 * change its state.
	 * @param callback Function executed when appear the async method.
	 * @param initFunction Initialization function of async thread (if any).	 
	 */
    void threadFunction(void (*callback)(PinValue), void(*initfunction)(void));

	std::string pinName;				///< Name of the pin (number as string).
	std::thread threadHandle;			///< 
	PinDirection pinDir;				///< The GPIO pi s is an imput or an output?
	int fd;								///< File pointer to manage the GPIO pin.
	bool async;							///< Is output in asynchronous mode?
};


#endif  								// SIMPLEGPIO_H
/** @} */ 								// end of group
