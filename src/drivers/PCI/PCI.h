/*
 * PCI.h
 *
 *  Created on: Jul 12, 2017
 *      Author: garrett
 */

#ifndef SRC_DRIVERS_PCI_PCI_H_
#define SRC_DRIVERS_PCI_PCI_H_

#include "Global.h"

extern int8_t MAX_PCI_FUNCTIONS;
/**
 * bool array of valid PCI buses
 */
extern bool validPCIBuses[256];
/**
 * inits the PCI sub system. finds all PCI buses
 */
void PCIInit( );
/**
 * checks if the device is multifunction based on given function header type
 * @param bus device is on
 * @param device on bus
 * @param function in device
 * @return if device is multifunction
 */
bool isPCIMultiFunctionDevice(uint8_t bus, uint8_t device, uint8_t function);
/**
 * checks validity of function based off vender code
 * @param bus
 * @param device
 * @param function
 * @return if the function is a valid function
 */
bool isValidPCIFunction(uint8_t bus, uint8_t device, uint8_t function);
/**
 * checks the vender of the pci device / function
 * @param bus
 * @param slot
 * @param function
 * @return vender ID
 */
uint16_t getPCIVenderID(uint8_t bus, uint8_t slot, uint8_t function);
/**
 * read 32 bits from PCI config space
 * @param bus
 * @param slot
 * @param func
 * @param offset into config space
 * @return uint32 of the config space starting at offset.
 */
uint32_t readPCIConfigWord(uint8_t bus, uint8_t slot, uint8_t func,
		uint8_t offset);
/**
 * check devices on bus for other buses
 * @param bus
 */
void checkPCIBus(uint8_t bus);
/**
 * check device / function if it's a PCI bus
 * @param bus
 * @param device
 * @param function
 */
void checkPCIFunction(uint8_t bus, uint8_t device, uint8_t function);
/**
 * check device's functions to see if it holds a bus
 * @param bus
 * @param device
 */
void checkPCIDevice(uint8_t bus, uint8_t device);
/**
 * read the PCI base class from the config space
 * @param bus
 * @param device
 * @param function
 * @return base class
 */
uint8_t getPCIBaseClass(uint8_t bus, uint8_t device, uint8_t function);
/**
 * read the PCI sub class from the config space
 * @param bus
 * @param device
 * @param function
 * @return sub class
 */
uint8_t getPCISubClass(uint8_t bus, uint8_t device, uint8_t function);
/**
 * read the PCI header type from the config space
 * @param bus
 * @param device
 * @param function
 * @return header type
 */
uint8_t getPCIHeaderType(uint8_t bus, uint8_t device, uint8_t function);
/**
 * read the PCI second bus from the config space for pci bridge devices
 * @param bus
 * @param device
 * @param function
 * @return secondary bus
 */
uint8_t getPCISecondaryBus(uint8_t bus, uint8_t device, uint8_t function);

#endif /* SRC_DRIVERS_PCI_PCI_H_ */
