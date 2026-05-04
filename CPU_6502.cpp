#include <iostream>

#include "CPU_6502.h"

// pc arg by value for disassembler? Maybe, we don't necessarily want it to affect the actual CPU? Could we run both CPU and disassembler at the same time for debugging? 
int disassemble6502(unsigned char* codeBuffer, int pc);

CPU_6502::CPU_6502() {
    this->m_memoryMap = std::make_unique<uint8_t[]>(0x10000);
}

// copy memory from file or array to set memoryMap data
void CPU_6502::assignMemory(uint8_t* rom, size_t size) {
    memcpy(this->m_memoryMap.get(), rom, size);
}

// Loads passed value into accumulator. Final step for all LDA instructions. 
// works for immediate addressing, need to find value at address before running this. 
void CPU_6502::loadAccumulator(uint8_t byte)
{
    // loads a byte of emmory into the accumulator setting the zero and neg flags as appropriate.
    // you can load constant values and values at addresses I think? So, should I have a separate function for getting values from addresses to pass here?
    // maybe not, just dereference those values mayber? idk yet. 
    // actually, seems like the value doesn't matter whether it's a memory addres or constant, just add it. 
    this->m_accumulator = byte;
    if (byte == 0)
    {
        this->m_zero = 1;
    }
    // bitwise and byte with 1000 0000, which is non-zero if the 7th bit is set on byte. 
    else if (byte & 128)
    {
        this->m_negative = 1;
    }
}

// pass pointer because we need multiple bytes? make it by reference so it isn't a copy of pointer and points to something? Don't think copy of pointer is an issue.
// use for most addressing: zero page, 

// does this actually work? because the memory map in the actual cpu stores the 16 bit address 
// yes, I think so. Confused myself. It's an array of bytes (8-bits) but the size of the address space goes to the max value of 16 bits ~65k
uint8_t CPU_6502::getAddressValue(uint16_t address)
{
    return this->m_memoryMap[address];
}

// maybe this is overkill to have a function just to static_cast? idk, feels clean atm. 
// Think I'll change this to just static cast in the execute function branch
uint16_t CPU_6502::create16Bit(uint8_t lowByte)
{
    uint16_t result = static_cast<uint16_t>(lowByte);
    return  result;
}

// might make 2 parameters to keep program counter tracked outside of functions
uint16_t CPU_6502::create16Bit(uint8_t lowByte, uint8_t highByte)
{
    uint16_t result = (static_cast<uint16_t>(highByte) << 8) | lowByte;
    return  result;
}

// I need to add with carry too probably...so maybe I'm ahead of myself here? Actually, LDA with X and Y  . Maybe just make a function for adding the carry flag? seems messy and a lot but idk maybe it's better? 
uint16_t CPU_6502::add8To16Bit(uint8_t eightBit, uint16_t sixteenBit) 
{
    uint16_t result = (static_cast<uint16_t>(eightBit) + sixteenBit);
    return result;
}

// this will take the ASCII char used for base64 and make it the numerical value it represents
// do we need to add + 0 to make sure this evaluates to a numerical value? idk if it matters
uint8_t CPU_6502::decodeBase64(const uint8_t charB64)
{
    //if value is >= 'A' and <= 'Z' then subtract 'A'
    return (charB64 >= 'A' && charB64 <= 'Z') ? (charB64 - 'A' + 0)
        : (charB64 >= 'a' && charB64 <= 'z') ? (charB64 - 'a' + 26)
        : (charB64 >= '0' && charB64 <= '9') ? (charB64 - '0' + 52)
        : (charB64 == '+') ? 62
        : (charB64 == '/') ? 63
        : 0;

        // if value is >= 'a' and <= 'z' then subtract 'a' and add 26

        // if value is >= '0' and <= '9' then subtract '0' and add 52

        // if + num is 62 else num is 63
}

// pass memory map to this indexed by program counter
// depending on the operation, can access next byte with this->m_memoryMap[programCounter]
// does this even need to be a parameter? Can't I just access it with this->?
// I think this parameter should be uint16_t since the program counter is 16 bits...and the memory addresses in the memoryMap are 16 bits too. 
void CPU_6502::executeInstruction()
{
    uint8_t opcode = this->m_memoryMap[this->m_programCounter];
    
    // increment counter to next instruction
    this->m_programCounter++;
    // divide opcode by 6 to find which base64 character holds the bit to represent that opcode's flag
    uint8_t index = opcode / 6;
    // remainder determines which bit of the 6 is the exact bit in that base64 character
    uint8_t bit = opcode % 6;
    // shift constant bit times to get the proper bitmask
    uint8_t bitmask = 1 << bit;

    // instruction is a temporary variable for testing the macro
    uint8_t instruction = 66;
    // tmpAddress is being used as the final address currently
    uint16_t tmpAddress = 0;

    int cycles = 0;
#define t(pattern) if (decodeBase64(pattern[index]) & bitmask)
    
    // "address mode conditional branch(es)" for getting instruction address ... for later processing CHANGE STRING
    // any operation that requires getting an address will pass this condition. 
    // the following conditions are separated by address modes.
    // need to update this STRING!
    t("///////////////////////////////")
    {

        // Immediate address mode - Immediate addressing allows the programmer to directly specify an 8 bit constant within the instruction. 
        t("=I====g=====C====I========QBC===BI===Eg====")
        {
            /*
            if (LDA opcode)
            {
                // loads second byte of instruction, which is the immediate value. (The first byte is the opcode.)
               loadAccumulator(this->m_memoryMap[this->m_programCounter])

            }*/
            // ADC Immediate 
            if (opcode == 0x69)
            {
                // need to check for overflow. Set carry if overflow occurs - "this enables multiple byte addition to be performed.  
                uint16_t result16Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;
                uint8_t result8Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;
                this->m_overflow = (result16Bit > 0xFF) ? 1 : 0;
                this->m_zero = (result16Bit == 0) ? 1 : 0;
                this->m_negative = (~(result8Bit ^ 0x80) & 0x80) ? 1 : 0;
                cycles += 2;
            }
            // NEED TO check for page cross..for other addres modes  

            // ADC Zero Page
            if (opcode == 0x65)
            {
                // get value 
                uint16_t operandAddress = this->m_memoryMap[(static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter]))];
                uint16_t result16Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;
                uint8_t result8Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;
                this->m_overflow = (result16Bit > 0xFF) ? 1 : 0;
                this->m_zero = (result16Bit == 0) ? 1 : 0;
                this->m_negative = (~(result8Bit ^ 0x80) & 0x80) ? 1 : 0;
                cycles += 2;
            }

        }
         
        //done
        // get zero page address mode - string updated
        t("gB====H====Y====gB====H====c====wB====H====")
        {
            // ENTERED HERE FIRST
            tmpAddress = static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter]);
            cycles += 3;
        }

        // using uint8_t should naturally wrap-around. No need to check since cycles always 4 (wrap-around means stays on same page).
        // get ZERO PAGE, X mode - string updated
        t("===Y====gB====G====Y====w=====D====Y====gB=")
        {
            int8_t lowByteAddress =  this->m_memoryMap[this->m_programCounter] + this->m_indexRegX;
            // checks for wrap-around ahead of time. int8_t will wrap-around on its own at overflow.
            int8_t highByteAddress = lowByteAddress + 1;
            // if the lowByte is the last address on zero page couldn't this accidnetally cross over? <- fixed by using low and High address with type int8_t
            tmpAddress = (static_cast<uint16_t>(this->m_memoryMap[highByteAddress]) << 8) + this->m_memoryMap[lowByteAddress];
            cycles += 4;
        }

        // ZERO PAGE, Y mode
        t("=========================B====E============")
        {

        }


        //done
        // get ABSOLUTE address mode - string changed
        t("==G==E=c====wB====G====c====wB====H====c===")
        {
            // this->m_programCounter is + 1 and 2 if this->m_programCounter is still on opcode. (I incremented this->m_programCounter after initing opcode
            tmpAddress = ((static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter + 1]) << 8) | this->m_memoryMap[this->m_programCounter]);
            cycles += 4;
            std::cout << "Testing vars in executeInstructionscope:\n tmpAddress: " << tmpAddress << "\n cycles: " << cycles;

        }

        //done
        // do not need to check if instructionAddress low and high byte are on different pages since the 6502 has no issue moving over a byte in its structure.
        // the problem of adding a cycle only arises when a register adds to the address because the CPU needs to correct itself which takes a cycle. 
        // get ABSOLUTE, X - string updated
        t("====gB====G====Y====gB====C====M====gB====G")
        {
            // this->m_programCounter is + 1 and 2 if this->m_programCounter is still on opcode. 
            uint16_t instructionAddress = ((static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter + 1]) << 8) | this->m_memoryMap[this->m_programCounter]);
            tmpAddress = instructionAddress + +this->m_indexRegX;

            // adds 5 cycles if adding X crosses a memory page
            if((instructionAddress / 0x100) != (tmpAddress / 0x100))
            {
                cycles += 5;
            }
            else {
                cycles += 4;
            }
        }

        //done 
        // get ABSOLUTE, Y - string updated
        t("====C====I====g=====C====I====gQ====C====I=")
        {
            // this->m_programCounter is + 1 and 2 if this->m_programCounter is still on opcode. 
            uint16_t instructionAddress = ((static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter + 1]) << 8) | this->m_memoryMap[this->m_programCounter]);
            tmpAddress = instructionAddress + this->m_indexRegY;
            if ((instructionAddress / 0x100) != (tmpAddress / 0x100))
            {
                cycles += 5;
            }
            else {
                cycles += 4;
            }
        }

        // INDIRECT (without x or y), used by 0x6c only - string updated
        t("==================B========================")
        {

        }

        //done
        // get INDIRECT, X. Uses Zero page. - string updated
        // aka "Indexed indirect."
        // Wraps around zero page. Does not add additional cycles. Always 6. Don't need to check if wrap-around happened. So, uses &0xFF to wrap-around (knocks off the high byte).
        t("C====I====g=====C====I====g=====C====I=====")
        {
             uint8_t lowByteAddress = this->m_memoryMap[this->m_programCounter] + this->m_indexRegX;
             uint8_t highByteAddress = lowByteAddress + 1;
             tmpAddress = ((static_cast<uint16_t>(this->m_memoryMap[highByteAddress + 1]) << 8) +this->m_memoryMap[lowByteAddress]);
             cycles += 6;
             
        }

        // done
        // get Indirect, Y address mode. Uses Zero page.- string updated
        // aka "Indirect Indexed." Get 16 bit using address in instruction as lower byte, add the next byte in memory as higher byte, then add Y. 
        // you check if page is crossed after adding Y. 
        t("==g=====C====I====g=====C====I====g=====C==")
        {
            // could there be an issue with invalid index adding 1 to this->m_programCounter here? could these mess up and not wrap around? 
            uint16_t instructionAddress = (((static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter + 1])) << 8) | this->m_memoryMap[this->m_programCounter]);
            tmpAddress = instructionAddress + this->m_indexRegY;

            if ((instructionAddress / 0x100) != (tmpAddress / 0x100))
            {
                cycles += 6;
            }
            else {
                cycles += 5;
            }
        }

        // RELATIVE address mode. 
        t("==Q=====B====E====Q=====B====E====Q=====B==")
        {

        }

        // IMPLIED ADDRESS MODE - first character is A, which represents 0x00
        t("BE=BQ=B==U=BQ=F==U=BQ=B==EB=Q=B=QE==Q=B==EB")
        {

        }
    }
 //STRI <-- idk why this was here uncommented
    // add addresses (Zero page, X... etc.)  CHANGENG
    t("=========================g=SgiI")
    {

        // OPCodes that add X register CHANGE STRING
        t("=========================g===CI")
        {
            tmpAddress = this->m_memoryMap[this->m_programCounter + 1] + this->m_indexRegX;
        }
        //tmpAddress = this->m_memoryMap[this->m_programCounter + 1] + this->
    }

    t("=========================g=SgiI")
    {
        getAddressValue(this->m_memoryMap[this->m_programCounter]);
    }
    t("=========================gESgiI")
    {
    // uint8_t instruction = this->m_memoryMap[this->m_programCounter + 1] ||| I don't know yet if this is the value, will need to figure this out in other cases before loadAccumulator 
        loadAccumulator(instruction);
    }


}
