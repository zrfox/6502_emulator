#ifndef CPU_6502_H
#define CPU_6502_H

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
    void loadAccumulator(uint8_t byte);

    // address modes
    uint8_t getAddressValue(uint16_t address);

    // utilities
    uint16_t create16Bit(uint8_t lowByte);
    uint16_t create16Bit(uint8_t lowByte, uint8_t highByte);

    uint16_t add8To16Bit(uint8_t eightBit, uint16_t sixteenBit);

    void assignMemory(uint8_t* rom, size_t size);


public:
    void executeInstruction(uint16_t opcode);
    uint8_t decodeBase64(const uint8_t charB64);    

    CPU_6502();

    friend int main();

};

#endif