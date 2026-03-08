#include <iostream>

#include "CPU_6502.h"

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
    else if (byte & 128)
    {
        this->m_negative = 1;
    }
}

// pass pointer because we need multiple bytes? make it by reference so it isn't a copy of pointer and points to something? Don't think copy of pointer is an issue.
// use for most addressing: zero page, 
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
uint16_t CPU_6502::add8To16Bit(uint8_t eightBit, uint16_t sixteenBit) {
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
        : (charB64 == '+') ? 62 : 63;

        // if value is >= 'a' and <= 'z' then subtract 'a' and add 26

        // if value is >= '0' and <= '9' then subtract '0' and add 52

        // if + num is 62 else num is 63
}

// pass memory map to this indexed by program counter
// depending on the operation, can access next byte with this->m_memoryMap[programCounter]
void CPU_6502::executeInstruction(uint8_t pc)
{
    uint8_t opcode = this->m_memoryMap[pc];
    
    // increment counter to next instruction
    pc++;
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
    
    //get instruction address branch... for later processing CHANGE STRING
    t("=========================g=SgiI") {
        
        //done
        // get zero page address mode NEED TO CHANGE STRING
        t("=========================g=SgiI") {
            tmpAddress = static_cast<uint16_t>(this->m_memoryMap[pc]);
            cycles += 3;
        }

        // using uint8_t should naturally wrap-around. No need to check since cycles always 4 (wrap-around means stays on same page).
        // get ZERO PAGE, X mode NEED TO CHANGE STRING
        t("=========================g=SgiI") {
            int8_t lowByteAddress =  this->m_memoryMap[pc] + this->m_indexRegX;
            // checks for wrap-around ahead of time. int8_t will wrap-around on its own at overflow.
            int8_t highByteAddress = lowByteAddress + 1;
            // if the lowByte is the last address on zero page couldn't this accidnetally cross over? 
            tmpAddress = static_cast<uint16_t>(this->m_memoryMap[highByteAddress]) << 8 + this->m_memoryMap[lowByteAddress];
            cycles += 4;
        }

        //done
        // get ABSOLUTE address mode NEED TO CHANGE STRING
        t("=========================g=SgiI") {
            // pc is + 1 and 2 if pc is still on opcode. (I incremented pc after initing opcode
            tmpAddress = ((static_cast<uint16_t>(this->m_memoryMap[pc + 1]) << 8) | this->m_memoryMap[pc]);
            cycles += 4;

        }

        //done
        // do not need to check if instructionAddress low and high byte are on different pages since the 6502 has no issue moving over a byte in its structure.
        // the problem of adding a cycle only arises when a register adds to the address because the CPU needs to correct itself which takes a cycle. 
        // get ABSOLUTE, X NEED TO CHANGE STRING
        t("=========================g=SgiI") {
            // pc is + 1 and 2 if pc is still on opcode. 
            uint16_t instructionAddress = ((static_cast<uint16_t>(this->m_memoryMap[pc + 1]) << 8) | this->m_memoryMap[pc]);
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
        // get ABSOLUTE, Y NEED TO CHANGE STRING
        t("=========================g=SgiI") {
            // pc is + 1 and 2 if pc is still on opcode. 
            uint16_t instructionAddress = ((static_cast<uint16_t>(this->m_memoryMap[pc + 1]) << 8) | this->m_memoryMap[pc]);
            tmpAddress = instructionAddress + this->m_indexRegY;
            if ((instructionAddress / 0x100) != (tmpAddress / 0x100))
            {
                cycles += 5;
            }
            else {
                cycles += 4;
            }
        }

        //done
        // get INDIRECT, X. Uses Zero page. NEED TO CHANGE STRING
        // aka "Indexed indirect."
        // Wraps around zero page. Does not add additional cycles. Always 6. Don't need to check if wrap-around happened. So, uses &0xFF to wrap-around (knocks off the high byte).
        t("=========================g=SgiI") {
             uint8_t lowByteAddress = this->m_memoryMap[pc] + this->m_indexRegX;
             uint8_t highByteAddress = lowByteAddress + 1;
             tmpAddress = ((static_cast<uint16_t>(this->m_memoryMap[highByteAddress + 1]) << 8) +this->m_memoryMap[lowByteAddress]);
             cycles += 6;
             
        }

        // done
        // get Indirect, Y address mode. Uses Zero page. NEED TO CHANGE STRING
        // aka "Indirect Indexed." Get 16 bit using address in instruction as lower byte, add the next byte in memory as higher byte, then add Y. 
        // you check if page is crossed after adding Y. 
        t("=========================g=SgiI") {
            // could there be an issue with invalid index adding 1 to pc here? could these mess up and not wrap around? 
            uint16_t instructionAddress = (((static_cast<uint16_t>(this->m_memoryMap[pc + 1])) << 8) | this->m_memoryMap[pc]);
            tmpAddress = instructionAddress + this->m_indexRegY;

            if ((instructionAddress / 0x100) != (tmpAddress / 0x100))
            {
                cycles += 6;
            }
            else {
                cycles += 5;
            }
        }
    }
 //STRI <-- idk why this was here uncommented
    // add addresses (Zero page, X... etc.)  CHANGENG
    t("=========================g=SgiI") {

        // OPCodes that add X register CHANGE STRING
        t("=========================g===CI") {
            tmpAddress = this->m_memoryMap[pc + 1] + this->m_indexRegX;
        }
        //tmpAddress = this->m_memoryMap[pc + 1] + this->
    }

    t("=========================g=SgiI") {
        getAddressValue(this->m_memoryMap[pc]);
    }
    t("=========================gESgiI") {
    // uint8_t instruction = this->m_memoryMap[this->m_programCounter + 1] ||| I don't know yet if this is the value, will need to figure this out in other cases before loadAccumulator 
        loadAccumulator(instruction);
}


}

int disassemble6502(unsigned char* codeBuffer, int pc) 
{
    unsigned char* opCode = &codeBuffer[pc];
    switch (*opCode) {
    case 0x00:
        printf("BRK, ADRSM: Implied, Flags: ------- " + *opCode);
        //The BRK instruction forces the generation of an interrupt request. The program counter and processor status are pushed on the stack then the IRQ interrupt vector at $FFFE/F is loaded into the PC and the break flag in the status set to one.
        break;
    case 0x01:
        printf("");
        break;

    case 0x02:
        printf("");
        break;

    case 0x03:
        printf("");
        break;

    case 0x04:
        printf("");
        break;

    case 0x05:
        printf("");
        break;

    case 0x06:
        printf("");
        break;

    case 0x07:
        printf("");
        break;

    case 0x08:
        printf("");
        break;

    case 0x09:
        printf("");
        break;

    case 0x11:
        printf("");
        break;

    case 0x12:
        printf("");
        break;

    case 0x13:
        printf("");
        break;

    case 0x14:
        printf("");
        break;

    case 0x15:
        printf("");
        break;

    case 0x16:
        printf("");
        break;

    case 0x17:
        printf("");
        break;

    case 0x18:
        printf("");
        break;

    case 0x19:
        printf("");
        break;

    case 0x20:
        printf("");
        break;

    case 0x21:
        printf("");
        break;

    case 0x22:
        printf("");
        break;

    case 0x23:
        printf("");
        break;

    case 0x24:
        printf("");
        break;

    case 0x25:
        printf("");
        break;

    case 0x26:
        printf("");
        break;

    case 0x27:
        printf("");
        break;

    case 0x28:
        printf("");
        break;

    case 0x29:
        printf("");
        break;

    case 0x30:
        printf("");
        break;

    case 0x31:
        printf("");
        break;

    case 0x32:
        printf("");
        break;

    case 0x33:
        printf("");
        break;

    case 0x34:
        printf("");
        break;


    }
    
    return 0;
}