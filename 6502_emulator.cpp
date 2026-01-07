#include <iostream>

int disassemble6502(unsigned char* codeBuffer, int pc);

void executeInstruction(uint8_t opcode);
uint8_t decodeBase64(const uint8_t charB64);

class CPU_6502 {
private:
    // Likely besjt to keep these private, but may need to consider them as public for interacting with rest of code? Idk yet. 
    unsigned char accumulator;

public:

};


int main()
{
    
}

// this will take the ASCII char used for base64 and make it the numerical value it represents
// do we need to add + 0 to make sure this evaluates to a numerical value? idk if it matters
uint8_t decodeBase64(const uint8_t charB64)
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

void executeInstruction(uint8_t opcode)
{
    // divide opcode by 6 to find which base64 character holds the bit to represent that opcode's flag
    uint8_t index = opcode / 6;
    // remainder determines which bit of the 6 is the exact bit in that base64 character
    uint8_t bit = opcode % 6;
    // shift constant bit times to get the proper bitmask
    uint8_t bitmask = 1 << bit;
#define t(pattern) if (decodeBase64(pattern[index]) & bitmask)

    t("");


}

int disassemble6502(unsigned char* codeBuffer, int pc) 
{
    unsigned char* opCode = &codeBuffer[pc];
    switch (*opCode) {
    case 0x00:
        printf("BRK, ADRSM: Implied, Flags: ------- ", opCode);
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
    

}