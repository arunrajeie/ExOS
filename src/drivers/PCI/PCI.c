/*
 * PCI.c
 *
 *  Created on: Jul 12, 2017
 *      Author: garrett
 */

#include "drivers/PCI/PCI.h"

//TODO clean up a lot

uint8_t MAX_PCI_FUNCTION = 8;
/**
 * bool array of valid PCI buses
 */
bool validPCIBuses[256];
/**
 * inits the PCI sub system. finds all PCI buses
 */
void PCIInit( ) {
	//make sure all buses are invalid
	for (uint16_t i = 0; i < 256; i++){
		validPCIBuses[i] = false;
	}
	//check if the first bus multifunction, if so check each function for buses.
	if (isPCIMultiFunctionDevice(0, 0, 0)) {
		for (uint8_t function = 0; function < MAX_PCI_FUNCTION; function++) {
			if ( !isValidPCIFunction(0, 0, function)) {
				break;
			}
			//check the valid function
			checkPCIBus(function);
		}
	} else {
		//check the base function if not multifunction
		checkPCIBus(0);
	}
}
/**
 * checks if the device is multifunction based on given function header type
 * @param bus device is on
 * @param device on bus
 * @param function in device
 * @return if device is multifunction
 */
bool isPCIMultiFunctionDevice(uint8_t bus, uint8_t device, uint8_t function) {
	return ! ( (getPCIHeaderType(bus, device, function) & 0x80) == 0);
}
/**
 * checks validity of function based off vender code
 * @param bus
 * @param device
 * @param function
 * @return if the function is a valid function
 */
bool isValidPCIFunction(uint8_t bus, uint8_t device, uint8_t function) {
	return getPCIVenderID(bus, device, function) != 0xFFFF;
}
/**
 * checks the vender of the pci device / function
 * @param bus
 * @param slot
 * @param function
 * @return vender ID
 */
uint16_t getPCIVenderID(uint8_t bus, uint8_t slot, uint8_t function) {
	return readPCIConfigWord(bus, slot, function, 0);
}
//TODO rewrite
/**
 * read 32 bits from PCI config space
 * @param bus
 * @param slot
 * @param func
 * @param offset into config space
 * @return uint32 of the config space starting at offset.
 */
uint32_t readPCIConfigWord(uint8_t bus, uint8_t slot, uint8_t func,
		uint8_t offset) {
	uint32_t address;
	uint32_t lbus = (uint32_t) bus;
	uint32_t lslot = (uint32_t) slot;
	uint32_t lfunc = (uint32_t) func;
	uint32_t tmp = 0;

	address = (uint32_t)(
			(lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc)
					| ((uint32_t) 0x80000000));

	outl(0xCF8, address);
	tmp = inl(0xCFC);
	return (tmp);
}
/**
 * check devices on bus for other buses
 * @param bus
 */
void checkPCIBus(uint8_t bus) {
	uint8_t device;
	//set that its a valid bus
	validPCIBuses[bus] = true;
	//check the devices on the bus
	for (device = 0; device < 32; device++) {
		checkPCIDevice(bus, device);
	}
}
/**
 * check device / function if it's a PCI bus
 * @param bus
 * @param device
 * @param function
 */
void checkPCIFunction(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t baseClass;
	uint8_t subClass;
	uint8_t secondaryBus;
	//get the class codes
	baseClass = getPCIBaseClass(bus, device, function);
	subClass = getPCISubClass(bus, device, function);
	//check the class codes to see if it's a bus
	if ( (baseClass == 0x06) && (subClass == 0x04)) {
		//check that bus
		secondaryBus = getPCISecondaryBus(bus, device, function);
		checkPCIBus(secondaryBus);
	}
}
/**
 * check device's functions to see if it holds a bus
 * @param bus
 * @param device
 */
void checkPCIDevice(uint8_t bus, uint8_t device) {
	uint8_t function = 0;

	uint16_t vendorID = getPCIVenderID(bus, device, function);
	if (vendorID == 0xFFFF)
		return;        // Device doesn't exist
	checkPCIFunction(bus, device, function);
	uint8_t headerType = getPCIHeaderType(bus, device, function);
	if ( (headerType & 0x80) != 0) {
		/* It is a multi-function device, so check remaining functions */
		for (function = 1; function < 8; function++) {
			if (getPCIVenderID(bus, device, function) != 0xFFFF) {
				checkPCIFunction(bus, device, function);
			}
		}
	}
}
/**
 * read the PCI base class from the config space
 * @param bus
 * @param device
 * @param function
 * @return base class
 */
uint8_t getPCIBaseClass(uint8_t bus, uint8_t device, uint8_t function){
	return readPCIConfigWord(bus,device,function,0xB);
}
/**
 * read the PCI sub class from the config space
 * @param bus
 * @param device
 * @param function
 * @return sub class
 */
uint8_t getPCISubClass(uint8_t bus, uint8_t device, uint8_t function){
	return readPCIConfigWord(bus,device,function,0xA);
}
/**
 * read the PCI header type from the config space
 * @param bus
 * @param device
 * @param function
 * @return header type
 */
uint8_t getPCIHeaderType(uint8_t bus, uint8_t device, uint8_t function){
	return readPCIConfigWord(bus,device,function,0xE);
}
/**
 * read the PCI second bus from the config space for pci bridge devices
 * @param bus
 * @param device
 * @param function
 * @return secondary bus
 */
uint8_t getPCISecondaryBus(uint8_t bus, uint8_t device, uint8_t function){
	return readPCIConfigWord(bus,device,function, 0x19);
}

