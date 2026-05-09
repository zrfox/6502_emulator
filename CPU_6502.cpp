#include <iostream>

#include "CPU_6502.h"

// pc arg by value for disassembler? Maybe, we don't necessarily want it to affect the actual CPU? Could we run both CPU and disassembler at the same time for debugging? 
int disassemble6502(unsigned char* codeBuffer, int pc);

CPU_6502::CPU_6502() {
    this->m_memoryMap = std::make_unique<uint8_t[]>(0x10000);
}

// Flag Functions

// not going to implement this at this point
/*
void CPU_6502::setDecimalMode( )
{
}
*/

void CPU_6502::setCarry(bool value)
{
    this->m_carry = value;
}


// setCarry takes uint16 because it does num > 0xFF to check for carry. 
void CPU_6502::setCarry(uint16_t sum)
{
    this->m_carry = (sum > 0xFF) ? 1 : 0;
}

void CPU_6502::setOverflow(uint8_t valForBothXOR, uint8_t val2, uint8_t result)
{
    // overflow can only occur between two bytes if the operands are the same sign and the result is the opposite sign
    // & 0x80 makes sure we are only checking the 7th bit (the sign bit)
    // the first ^ will set or keep the 7th bit zero if both 7th bits are the same. The NOT ~ will inverse the byte that results so the 7th bit WILL be set if signs are the same.
    // the second ^ checks if valForBothXOR has a different sign than result. Truthy if different signs, falsy if same signs. If falsy, there was no overflow since the result shares a sign with one of the operands, which means both operands were not the same sign (which, as stated before, is required for overflow to occur.)
    this->m_overflow = (~(valForBothXOR ^ val2) & (valForBothXOR ^ result)) & 0x80 ? 1 : 0;
}

// for adc, set if accumulator is zero at the end (we only care if 8 bits of accumulator = 0. We don't care if the carry is set and the full value when considering accumulator and carry is really above 0xFF. We are checking if accumulator is zero, even in cases of 8bit wrap around.)
void CPU_6502::setZero(uint8_t sum)
{
    this->m_zero = (sum == 0) ? 1 : 0;
}

void CPU_6502::setNegative(uint8_t sum)
{
    this->m_negative = (sum & 0x80) ? 1 : 0;
}


// copy memory from file or array to set memoryMap data
void CPU_6502::assignMemory(uint8_t* rom, size_t size) 
{
    memcpy(this->m_memoryMap.get(), rom, size);
}

// ADDRESS MODE FUNCTIONS. !!! NEEDS HANDLING FOR CROSSING PAGES ON SOME OF THESE!

// IMPLICIT and ACCUMULATOR address modes will likely not have functions since the instructions with those modes are not uniform
// Immediate does not require a function since it simply grabs the value at the location indexed by the programCounter

// I'm calling it get...Value() for my own clarity to specify that this is getting/returning the value at the address and not the address itself. 
// this could be simple enough that I don't need it. 
uint8_t CPU_6502::getZeroPageValue(uint8_t zeroPageLowByte)
{
    uint16_t zeroPageAddress = static_cast<uint16_t>(zeroPageLowByte);
    return this->m_memoryMap[zeroPageAddress];
}

uint8_t CPU_6502::getZeroPageOffsetValue(uint8_t address, uint8_t offset)
{
    // the result is uint8_t because zero page addresses wrapAround instead of crossing pages. 
    uint8_t addressAfterOffset = address + offset;
    return this->m_memoryMap[static_cast<uint16_t>(addressAfterOffset)];
}


uint8_t CPU_6502::getAbsoluteOffsetValue(uint16_t absoluteAddress, uint8_t offset)
{
    uint16_t addressAfterOffset = absoluteAddress + offset;
    return this->m_memoryMap[addressAfterOffset];
}

// this is only for JMP. JMP opcode 0x6C, specifically. This (correctly) returns an address, not a value from an address. 
uint16_t CPU_6502::getIndirectValue(uint16_t addressOfLowByte)
{ // I NEED TO CHECK IF PAGES ARE CROSSED...maybe make a function for that? idk
    uint16_t fullIndirectTargetAddress = create16Bit(this->m_memoryMap[addressOfLowByte], this->m_memoryMap[addressOfLowByte + 1]);
    return fullIndirectTargetAddress;
}

// offset for this function is X register
uint8_t CPU_6502::getIndexedIndirectValue(uint8_t tableAddress, uint8_t offset)
{
    // address of target low byte. uint8_t this wraps around. 
    uint8_t addressAfterOffset = tableAddress + offset;
    uint16_t targetAddress = create16Bit(this->m_memoryMap[addressAfterOffset], this->m_memoryMap[static_cast<uint8_t>(addressAfterOffset + 1)]);
    return this->m_memoryMap[targetAddress];
}

// offset for this function is Y register
uint8_t CPU_6502::getIndirectIndexedValue(uint8_t zeroPageLowByte, uint8_t offset)
{
    uint16_t addressAfterOffset = (create16Bit(this->m_memoryMap[zeroPageLowByte], this->m_memoryMap[static_cast<uint8_t>(zeroPageLowByte + 1)])) + static_cast<uint16_t>(offset);
    return this->m_memoryMap[addressAfterOffset];
}

// Instruction functions...what's your functiiiiooon

void CPU_6502::addWithCarry(uint8_t valueFromMemory)
{
    // use uint16_t to check if sum > 0xFF
    uint16_t sum = valueFromMemory + this->m_accumulator + this->m_carry;
    setCarry(sum);
    // checks if sign flipped (for when considering signed values, I think)
    // only want bottom 8 bits. Carry takes care of anything above 8 bits. 
    uint8_t result = sum & 0xFF;
    setOverflow(valueFromMemory, this->m_accumulator, result);
    this->m_accumulator = result;

    setZero(this->m_accumulator);
    // negative if bit 7 set from last operation..which operation? the sum assigning the accumulator the result? - if accumulator result is 0 (from the manual)
    setNegative(this->m_accumulator);

    // set carry if overflow for multiple byte addition...set overflow if sign is wrong...how do I know if sign is wrong? check signs of original values? adding two positive numbers will never be negative, if both are negative never positive, if negative bigger than positive result is negative, otherwise positive. But positive and a negative can never overflow. 

}

// does AND return anything? probably - it sets the accumulator (from the manual)
void CPU_6502::logicalAnd(uint8_t valueFromMemory)
{
    uint8_t result = valueFromMemory & this->m_accumulator;
    this->m_accumulator = result;
    // maybe directly setZero with accumulator since it's based on accumulator value. 
    setZero(this->m_accumulator);
    setNegative(this->m_accumulator);

}

// ASL - arithmetic shift left - occurs on accumulator or "the address memory location" (<- which means the value at the address)
// pass by ref because we want to affect the value itself we are shifting. 
// used to multiply memory contents by 2 (ignoring 2's complement considerations) - obelisk
void CPU_6502::arithmeticShiftLeft(uint8_t &valueToShift)
{
    // how do i get the 7th bit if it moves? 0th bit is always zero btw
    bool carryBit = valueToShift & 0x80;
    setCarry(carryBit);

    uint8_t result = valueToShift << 1;
    setNegative(result);
    setZero(result);
}

// BCC - $90
// idk of the offset is uint8 or 16. 
// only relative address mode. cycles change if new page. 
void CPU_6502::branchIfCarryClear(uint16_t& pc, uint8_t offset)
{
    if (!this->m_carry)
    {
        pc += static_cast<int8_t>(offset);
    }
}

// BCS - $B0
void CPU_6502::branchIfCarrySet(uint16_t& pc, uint8_t offset)
{
    if (this->m_carry)
    {
        pc += static_cast<int8_t>(offset);
    }
}

// BEQ - $F0
// "takes a conditional branch whenever the Z flag is on or the previous result is equal to 0" - manual
void CPU_6502::branchIfEqual(uint16_t& pc, uint8_t offset)
{
    if (this->m_zero)
    {
        pc += static_cast<int8_t>(offset);
    }
}

// BMI - $30
void CPU_6502::branchIfMinus(uint16_t& pc, uint8_t offset)
{
    if (this->m_negative)
    {
        pc += static_cast<int8_t>(offset);
    }
}

// BNE - $D0
void CPU_6502::branchIfNotEqual(uint16_t& pc, uint8_t offset)
{
    if (!this->m_zero)
    {
        pc += static_cast<int8_t>(offset);
    }
}

void CPU_6502::branchIfPositive(uint16_t& pc, uint8_t offset)
{
    if (!this->m_negative)
    {
        pc += static_cast<int8_t>(offset);
    }
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

                // immediate uses the value stored at programCounter's memory location as a literal, as opposed to the value stored being a low or high byte for a 16 bit address.
                uint8_t valueAtAddress = this->m_memoryMap[this->m_programCounter];
                uint16_t result16Bit = this->m_accumulator + valueAtAddress + this->m_carry;
                uint8_t result8Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;
                this->m_overflow = (result16Bit > 0xFF) ? 1 : 0;
                this->m_zero = (result16Bit == 0) ? 1 : 0;
                this->m_negative = (result8Bit & 0x80) ? 1 : 0;
                cycles += 2;
            }
            // NEED TO check for page cross..for other addres modes  

            // ADC Zero Page
            if (opcode == 0x65)
            {
                // get value that pc is pointing to and make 16 bit because it's zero page. then get the value at the address
                // (the pc is pointing to the low byte of a zero page address. The high byte is all zeros, so casting the low bit to uint16_t gets the full 16 bit address.)
                uint8_t valueAtAddress = this->m_memoryMap[(static_cast<uint16_t>(this->m_memoryMap[this->m_programCounter]))];
                uint16_t result16Bit = this->m_accumulator + valueAtAddress + this->m_carry;
                uint8_t result8Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;

                // function to check these flags instead? 
                this->m_overflow = (result16Bit > 0xFF) ? 1 : 0;
                this->m_zero = (result16Bit == 0) ? 1 : 0;
                this->m_negative = (result8Bit & 0x80) ? 1 : 0;
                cycles += 3;
            }

            // ADC Zero Page, X
            if (opcode == 0x65)
            {
                // get address of zeroPage byte and X address first as uint8_t. This is good because the address wrapsaround on the zero page. 
                uint8_t zeroPagePlusXAddress = this->m_memoryMap[this->m_programCounter] + this->m_indexRegX;
                uint8_t valueAtAddress = this->m_memoryMap[(static_cast<uint16_t>(zeroPagePlusXAddress))];
                uint16_t result16Bit = this->m_accumulator + valueAtAddress + this->m_carry;
                uint8_t result8Bit = this->m_accumulator + this->m_memoryMap[this->m_programCounter] + this->m_carry;
                this->m_overflow = (result16Bit > 0xFF) ? 1 : 0;
                this->m_zero = (result16Bit == 0) ? 1 : 0;
                this->m_negative = (result8Bit & 0x80) ? 1 : 0;
                cycles += 4;
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
            uint8_t lowByteAddress =  this->m_memoryMap[this->m_programCounter] + this->m_indexRegX;
            // checks for wrap-around ahead of time. uint8_t will wrap-around on its own at overflow.
            uint8_t highByteAddress = lowByteAddress + 1;
            // if the lowByte is the last address on zero page couldn't this accidnetally cross over? <- fixed by using low and High address with type uint8_t
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
