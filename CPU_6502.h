#ifndef CPU_6502_H
#define CPU_6502_H

#include <cstdint>   // uint8_t, uint16_t, size_t
#include <memory>    // std::unique_ptr

class CPU_6502
{
private:
    // Likely best to keep these private, but may need to consider them as public for interacting with rest of code? Idk yet. 
    uint8_t m_accumulator = 0;
    uint8_t m_indexRegX = 0;
    uint8_t m_indexRegY = 0;
    // programCounter holds the address of the current instruction. 
    uint16_t m_programCounter = 0;
    uint16_t m_stackPtr = 0;

    // flags
    uint8_t m_carry = 0;
    uint8_t m_zero = 0;
    uint8_t m_interruptDisable = 0;
    uint8_t m_decimalMode = 0;
    uint8_t m_breakCommand = 0;
    uint8_t m_overflow = 0;
    uint8_t m_negative = 0;

    // memory map.
    std::unique_ptr<uint8_t[]> m_memoryMap;

    // instructions
    void addWithCarry(uint8_t valueFromMemory);
    void logicalAnd(uint8_t valueFromMemory);
    void arithmeticShiftLeft(uint8_t &valueToShift);


    void loadAccumulator(uint8_t byte);

    // address modes
    uint8_t getAddressValue(uint16_t address);
    uint8_t getZeroPageValue(uint8_t zeroPageLowByte);
    uint8_t getZeroPageOffsetValue(uint8_t address, uint8_t offset);
    uint8_t getAbsoluteOffsetValue(uint16_t absoluteAddress, uint8_t offset);
    uint16_t getIndirectValue(uint16_t addressOfLowByte);
    uint8_t getIndexedIndirectValue(uint8_t tableAddress, uint8_t offset);
    uint8_t getIndirectIndexedValue(uint8_t zeroPageLowByte, uint8_t offset);


    // flag functions
    
    // overloaded setCarry functions for when setting by single bit or checking arithmetic sum over 0xFF, respectively
    void setCarry(bool value);
    void setCarry(uint16_t sum);
    void setZero(uint8_t sum);
    void setOverflow(uint8_t val1, uint8_t val2, uint8_t sum);
    void setNegative(uint8_t sum);
    //void setDecimalMode(); <- Decimal mode is disabled for the NES. Might implement later for completeness. 





    // utilities
    uint16_t create16Bit(uint8_t lowByte, uint8_t highByte);
    uint16_t add8To16Bit(uint8_t eightBit, uint16_t sixteenBit);
    void assignMemory(uint8_t* rom, size_t size);


public:
    void executeInstruction();
    uint8_t decodeBase64(const uint8_t charB64);    

    CPU_6502();

    friend int main();

};

#endif