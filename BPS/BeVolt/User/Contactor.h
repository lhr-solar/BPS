// Contactor.h
/**
 * Driver for contactor (EV200) relay that disconnects the battery from car. Uses TI DRV102 driver
 * to deliver necessary current to energize the contactor
 * @authors Sijin Woo, Corey Hulse
 * @lastRevised 9/4/2018
 */

#ifndef CONTACTOR_H
#define CONTACTOR_H

/** Contactor
 * Contactor class that has functions related to the contactors.
 * Multiple instances of the class can be created in order to implement
 * multiple contactors for other systems.
 */
class Contactor{
public:

	/** Constructor
	 * Creates contactor instance for a single contactor relay. The
	 * input pin is used to signify a fault in the driver. The output
	 * pin is used to turn on the contactor through the TI DRV102 driver.
	 * @param Pin output
	 * @param Pin input
	 */
    //Contactor(PinName out, PinName in);

    /** openContactor
     * Sends signal to TI DRV102 to open the contactor. This disconnects the 
     * battery pack from the car.
     */
    void openContactor();

    /** closeContactor
     * Sends signal to TI DRV102 to close the contactor. This connects the battery
     * pack to the car.
     */
    void closeContactor();

    /** getFlagState
     * Gets the state of the flag. The flag is normally high but if a fault occurs,
     * the flag will be set to low
     * @return 1 is working properly, 0 if fault
     */
    int getFlagState();

    /** getSignalState
     * Gets the state of the output signal.
     * @return 1 if signal is high (contactor is closed), 0 if signal is low (contactor is open)
     */
    int getSignalState();
    
private:
    //DigitalOut signal;		// output signal to close contactor
    //DigitalIn flag;			// input pin for flag status
};



#endif