#ifndef CPU_6502_H
#define CPU_6502_H

class CPU_6502
{
private:
    // Likely best to keep these private, but may need to consider them as public for interacting with rest of code? Idk yet. 
    uint8_t m_accumulator;
    uint8_t m_indexRegX;
    uint8_t m_indexRegY;
    uint16_t m_programCounter;
    uint16_t m_stackPtr;

    // flags
    uint8_t m_carry = 0;
    uint8_t m_zero = 0;
    uint8_t m_interruptDisable = 0;
    uint8_t m_decimalMode = 0;
    uint8_t m_breakCommand = 0;
    uint8_t m_overflow = 0;
    uint8_t m_negative = 0;

    // memory map.
    uint8_t m_memoryMap[65536];

    // instructions
    uint8_t loadAccumulator(uint8_t byte);

    // address modes
    uint8_t getAddressValue(uint16_t address);

    // utilities
    uint16_t create16Bit(uint8_t lowByte);
    uint16_t create16Bit(uint8_t lowByte, uint8_t highByte);

    uint16_t add8To16Bit(uint8_t eightBit, uint16_t sixteenBit);




public:

};

#endif