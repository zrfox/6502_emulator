#include <iostream>

#include "CPU_6502.h"

int main()
{
	uint8_t testByte1 = 8;
	uint8_t testByte2 = -8;
	uint8_t testByte3 = 0;

	CPU_6502 obj;

	////obj.loadAccumulator(testByte2);
	////std::cout << static_cast<signed int>(obj.m_accumulator) << std::endl;

	//// base64 byte that has flag turned on for indirect X.
	//uint8_t testByteExecInstructOpCode1 = 173;
	//uint8_t memTest1[] = { 0xAD, 0xDF };
	////uint8_t* memPtr = &memTest1[0];
	//size_t memSize = sizeof(memTest1);

	//obj.assignMemory(memTest1, memSize);
	//// need to initialize memory with a made up value in the second position that the var instruction will become
	//obj.executeInstruction(obj.getAddressValue(0));
	//std::cout << static_cast<signed int>(obj.m_accumulator) << std::endl;


	//pc is equal to zero right now, so it will index the value of 0xAE, absolute address mode. 
	uint8_t memoryMap[] = { 0xAE, 0xFF, 0x01 };
	obj.assignMemory(memoryMap, size_t(3));
	obj.executeInstruction();
	/*

	// testing wrap around for indirect, x
	uint8_t memoryMap[] = { 0x0F, 0xFF };
	uint8_t indexRegX = 0xFF;

	uint16_t tmpAddress = static_cast<uint16_t>(memoryMap[0]) + static_cast<uint16_t>(indexRegX);

	if (tmpAddress > 0xFF) {
		tmpAddress -= 0xFF;
	}
	std::cout << tmpAddress << std::endl;
	*/



	return 0;
}