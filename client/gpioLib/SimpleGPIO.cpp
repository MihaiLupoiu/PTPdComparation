/**
 * @file    SimpleGPIO.cpp
 * @author  DFLM (dflarios@dte.us.es)
 * @author  Mihaita Lupoiu (myhay.360@gmail.com)
 * @author  Jose Antonio Belloch (jobelrod@iteam.upv.es)
 * @date    February, 2016
 * @version 1.0b
 * @bug     Not known bugs in SimpleGPIO.cpp.
 * @brief   Implementation of functions to manage GPIO.
 */
 
 // / * * @mainpage GPIO Library
 //*
 //* Library to manage GPIO in Linux boards.
 //* /
 
/**
 * @defgroup GPIO The GPIO library group.
 * @ingroup  UpperLevelGroup
 * @brief    functions to be used to manage GPIO in Linux boards.
 * @{
 */
 
//******************************************************************************
//* INCLUDES                                                                   *
//******************************************************************************
 #include <thread>						// used in async trhead.
 #include <iostream>						// std::cout


//#include <stdio.h>
#include <unistd.h>						// read, write.
#include <fcntl.h>						// open.
#include <poll.h>						// poll in async function.
//#include <errno.h>

 #include "SimpleGPIO.h"
 

//******************************************************************************
//* TYPEDEFS                                                                   *
//******************************************************************************


//******************************************************************************
//* CONSTANTS                                                                  *
//******************************************************************************


//******************************************************************************
//* GLOBAL VARIABLES                                                           *
//******************************************************************************


//******************************************************************************
//* PRIVATE LOCAL FUNCTION PROPTOTIPES                                         *
//******************************************************************************
/** @} */ 								// end of group


//******************************************************************************
//* FUNCTION IMPLEMENTATIONS                                                   *
//******************************************************************************


//******************************************************************************
//* CLASS IMPLEMENTATIONS                                                      *
//******************************************************************************
Gpio::Gpio(const unsigned int& gpio, const PinDirection& direction): 
        pinName(std::to_string(gpio)), pinDir(PinDirection::NONE), async(false) {

    fd= open((gpioPath+ "/export").c_str(), O_WRONLY);// Export GPIO pin.
	if (fd< 0)
		throw 22;
	write(fd, pinName.c_str(), pinName.length()+1);
	close(fd);
	setDirection(direction);			// Configure it as input or output.
}


Gpio::~Gpio() {
	if(async)							// Input in asynchronous mode?
        removeAsync();					// Remove it.
	close(fd);
    fd = open((gpioPath+"/unexport").c_str(), O_WRONLY);// Unexport the GPIO pin.
	if (fd < 0)
		throw 22;
	write(fd, pinName.c_str(), pinName.length()+1);
	close(fd);
}


void Gpio::setDirection(const PinDirection& direction){
    if(direction==PinDirection::NONE)	// None is not a valid direction.
		throw 22;
    if(pinDir!=PinDirection::NONE) {	//If pin was an input or output.
		if(async)						// If async mode
            removeAsync();				// remove it.
		close(fd);						// Close file pointer handler.
	}	
    int lfd= open((gpioPath+ "/gpio"+ pinName+ "/direction").c_str(), O_WRONLY);
	if (lfd < 0)
		throw 22;
	if (direction==PinDirection::OUTPUT) {	// Configure as input or output.
		write(lfd, "out", 4);
		fd= open((gpioPath+ "/gpio"+ pinName+ "/value").c_str(), O_WRONLY);
	}
	else {
		write(lfd, "in", 3);
		fd= open((gpioPath+ "/gpio"+ pinName+ "/value").c_str(), O_RDONLY);
	}
	if (fd < 0)
		throw 22;
	pinDir=direction;
	close(lfd);
}


void Gpio::setAsync(void(*callback)(PinValue), const PinEdge& edge,
        void (*initFunction)(void)) {
	#ifdef WITH_TYPE_TEST
	if((async==false) && (pinDir==PinDirection::INPUT)) {
	#endif
		std::string edgeValue[]={"rising", "falling", "both"};// Possible edges.
        std::string tmpText= edgeValue[static_cast<int>(edge)];//Selected edge.
		
        int lfd = open((gpioPath+ "/gpio"+ pinName+ "/edge").c_str(), O_WRONLY);
		if (lfd < 0)
			throw 22;
		write(lfd, tmpText.c_str(), tmpText.length()+1);
		close(lfd);
		async=true;
		threadHandle= std::thread(&Gpio::threadFunction, this,
			callback, initFunction);
	#ifdef WITH_TYPE_TEST
	}
	else								// An output cannot be in async mode.
		throw 22;
	#endif
}


void Gpio::removeAsync() {
	#ifdef WITH_TYPE_TEST
	if(async==true){
	#endif			
		async=false;					// Set synchronous flag.
        int lfd = open((gpioPath+ "/gpio"+ pinName+ "/edge").c_str(), O_WRONLY);
		write(lfd, "none", 5);			// Remove edges settings.
		close(lfd);
		threadHandle.join();			// Close thread that controls GPIO.
	#ifdef WITH_TYPE_TEST
	}
	else
		throw 22;
	#endif
}


void Gpio::setValue(const PinValue& value) {
	#ifdef WITH_TYPE_TEST
	if (pinDir==PinDirection::OUTPUT) {
	#endif
		if (value==PinValue::HIGH)
			write(fd, "1", 2);			// 1 -> High state.
		else 
			write(fd, "0", 2);			// 0 -> High state.
	#ifdef WITH_TYPE_TEST
	}
	else
		throw 22;
	#endif
}


PinValue Gpio::getValue() const {
	char pinRead;
	
	#ifdef WITH_TYPE_TEST
	if (pinDir==PinDirection::INPUT) {
	#endif
		lseek(fd, 0, 0);				// Go to the start of the "file".
		read(fd, &pinRead, 1);			// Read the value.
		if(pinRead=='0')				// '0' represents low state.
			return PinValue::LOW;
		else							// '1' represents high state.
			return PinValue::HIGH;
	#ifdef WITH_TYPE_TEST
	}
	else {								// An output cannot be readed.
		throw 22;
		return PinValue::LOW;
	}
	#endif
}


void Gpio::threadFunction(void (*callback)(PinValue),
        void (*initfunction)(void)) {
	pollfd pfd;

	if (initfunction!=NULL)				// Call init function (if any).
		initfunction();					
	pfd.fd=fd;							// configure poll structure
	pfd.events = POLLPRI;
	while (async) {
		poll(&pfd, 1, 1000);			// Wait changesor time  (in uS, -1 =inf).
		if(pfd.revents & POLLPRI)		// If edge event
            callback(getValue());			// Call callback, sending ping value.
	}
}
