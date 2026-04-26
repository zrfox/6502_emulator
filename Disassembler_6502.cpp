#include <iostream>


// Produced mainly by Claude. Need to look over. Use for ROM testing. 
int disassemble6502(unsigned char* codeBuffer, int pc)
{
    unsigned char* opCode = &codeBuffer[pc];
    switch (*opCode) {
        // Flag string format: NV-BDIZC
    case 0x00:
        printf("%02X: BRK, ADRSM: Implied, Flags: --1B1---\n", *opCode);
        // Forces the generation of an interrupt request. PC and processor status pushed on stack,
        // IRQ interrupt vector at $FFFE/F loaded into PC, break flag set to 1.
        break;
    case 0x01:
        printf("%02X: ORA, ADRSM: (Indirect,X), Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x02:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x03:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x04:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x05:
        printf("%02X: ORA, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x06:
        printf("%02X: ASL, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Arithmetic Shift Left. Bit 0 set to 0, old bit 7 into carry.
        break;
    case 0x07:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x08:
        printf("%02X: PHP, ADRSM: Implied, Flags: --------\n", *opCode);
        // Push Processor Status on Stack.
        break;
    case 0x09:
        printf("%02X: ORA, ADRSM: Immediate, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x0A:
        printf("%02X: ASL, ADRSM: Accumulator, Flags: N-----ZC\n", *opCode);
        // Arithmetic Shift Left on Accumulator.
        break;
    case 0x0B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x0C:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x0D:
        printf("%02X: ORA, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x0E:
        printf("%02X: ASL, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Arithmetic Shift Left.
        break;
    case 0x0F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x10:
        printf("%02X: BPL, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Result Plus. Branch taken if negative flag is clear.
        break;
    case 0x11:
        printf("%02X: ORA, ADRSM: (Indirect),Y, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x12:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x13:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x14:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x15:
        printf("%02X: ORA, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x16:
        printf("%02X: ASL, ADRSM: Zero Page,X, Flags: N-----ZC\n", *opCode);
        // Arithmetic Shift Left.
        break;
    case 0x17:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x18:
        printf("%02X: CLC, ADRSM: Implied, Flags: -------C\n", *opCode);
        // Clear Carry Flag. C = 0
        break;
    case 0x19:
        printf("%02X: ORA, ADRSM: Absolute,Y, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x1A:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x1B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x1C:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x1D:
        printf("%02X: ORA, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // OR Memory with Accumulator. A = A | M
        break;
    case 0x1E:
        printf("%02X: ASL, ADRSM: Absolute,X, Flags: N-----ZC\n", *opCode);
        // Arithmetic Shift Left.
        break;
    case 0x1F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x20:
        printf("%02X: JSR, ADRSM: Absolute, Flags: --------\n", *opCode);
        // Jump to Subroutine. Pushes return address (PC-1) onto stack, then loads PC with target address.
        break;
    case 0x21:
        printf("%02X: AND, ADRSM: (Indirect,X), Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x22:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x23:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x24:
        printf("%02X: BIT, ADRSM: Zero Page, Flags: NV----Z-\n", *opCode);
        // Test Bits in Memory with Accumulator. Bits 7 and 6 of operand transferred to N and V flags.
        break;
    case 0x25:
        printf("%02X: AND, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x26:
        printf("%02X: ROL, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Rotate Left. Old bit 7 to carry, old carry to bit 0.
        break;
    case 0x27:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x28:
        printf("%02X: PLP, ADRSM: Implied, Flags: NV-BDIZC\n", *opCode);
        // Pull Processor Status from Stack. All flags restored from stack.
        break;
    case 0x29:
        printf("%02X: AND, ADRSM: Immediate, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x2A:
        printf("%02X: ROL, ADRSM: Accumulator, Flags: N-----ZC\n", *opCode);
        // Rotate Left on Accumulator.
        break;
    case 0x2B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x2C:
        printf("%02X: BIT, ADRSM: Absolute, Flags: NV----Z-\n", *opCode);
        // Test Bits in Memory with Accumulator.
        break;
    case 0x2D:
        printf("%02X: AND, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x2E:
        printf("%02X: ROL, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Rotate Left.
        break;
    case 0x2F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x30:
        printf("%02X: BMI, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Result Minus. Branch taken if negative flag is set.
        break;
    case 0x31:
        printf("%02X: AND, ADRSM: (Indirect),Y, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x32:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x33:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x34:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x35:
        printf("%02X: AND, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x36:
        printf("%02X: ROL, ADRSM: Zero Page,X, Flags: N-----ZC\n", *opCode);
        // Rotate Left.
        break;
    case 0x37:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x38:
        printf("%02X: SEC, ADRSM: Implied, Flags: -------C\n", *opCode);
        // Set Carry Flag. C = 1
        break;
    case 0x39:
        printf("%02X: AND, ADRSM: Absolute,Y, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x3A:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x3B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x3C:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x3D:
        printf("%02X: AND, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // AND Memory with Accumulator. A = A & M
        break;
    case 0x3E:
        printf("%02X: ROL, ADRSM: Absolute,X, Flags: N-----ZC\n", *opCode);
        // Rotate Left.
        break;
    case 0x3F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x40:
        printf("%02X: RTI, ADRSM: Implied, Flags: NV-BDIZC\n", *opCode);
        // Return from Interrupt. Pulls processor status and PC from stack.
        break;
    case 0x41:
        printf("%02X: EOR, ADRSM: (Indirect,X), Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x42:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x43:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x44:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x45:
        printf("%02X: EOR, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x46:
        printf("%02X: LSR, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Logical Shift Right. Bit 0 goes to carry, bit 7 set to 0.
        break;
    case 0x47:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x48:
        printf("%02X: PHA, ADRSM: Implied, Flags: --------\n", *opCode);
        // Push Accumulator on Stack.
        break;
    case 0x49:
        printf("%02X: EOR, ADRSM: Immediate, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x4A:
        printf("%02X: LSR, ADRSM: Accumulator, Flags: N-----ZC\n", *opCode);
        // Logical Shift Right on Accumulator.
        break;
    case 0x4B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x4C:
        printf("%02X: JMP, ADRSM: Absolute, Flags: --------\n", *opCode);
        // Jump to new location. PC = operand address.
        break;
    case 0x4D:
        printf("%02X: EOR, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x4E:
        printf("%02X: LSR, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Logical Shift Right.
        break;
    case 0x4F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x50:
        printf("%02X: BVC, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Overflow Clear. Branch taken if overflow flag is clear.
        break;
    case 0x51:
        printf("%02X: EOR, ADRSM: (Indirect),Y, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x52:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x53:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x54:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x55:
        printf("%02X: EOR, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x56:
        printf("%02X: LSR, ADRSM: Zero Page,X, Flags: N-----ZC\n", *opCode);
        // Logical Shift Right.
        break;
    case 0x57:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x58:
        printf("%02X: CLI, ADRSM: Implied, Flags: ---I----\n", *opCode);
        // Clear Interrupt Disable Bit. I = 0
        break;
    case 0x59:
        printf("%02X: EOR, ADRSM: Absolute,Y, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x5A:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x5B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x5C:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x5D:
        printf("%02X: EOR, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // Exclusive-OR Memory with Accumulator. A = A ^ M
        break;
    case 0x5E:
        printf("%02X: LSR, ADRSM: Absolute,X, Flags: N-----ZC\n", *opCode);
        // Logical Shift Right.
        break;
    case 0x5F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x60:
        printf("%02X: RTS, ADRSM: Implied, Flags: --------\n", *opCode);
        // Return from Subroutine. Pulls PC from stack and increments it.
        break;
    case 0x61:
        printf("%02X: ADC, ADRSM: (Indirect,X), Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x62:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x63:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x64:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x65:
        printf("%02X: ADC, ADRSM: Zero Page, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x66:
        printf("%02X: ROR, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Rotate Right. Old bit 0 to carry, old carry to bit 7.
        break;
    case 0x67:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x68:
        printf("%02X: PLA, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Pull Accumulator from Stack. Sets N and Z based on pulled value.
        break;
    case 0x69:
        printf("%02X: ADC, ADRSM: Immediate, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x6A:
        printf("%02X: ROR, ADRSM: Accumulator, Flags: N-----ZC\n", *opCode);
        // Rotate Right on Accumulator.
        break;
    case 0x6B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x6C:
        printf("%02X: JMP, ADRSM: Indirect, Flags: --------\n", *opCode);
        // Jump to new location via indirect address. PC = [operand].
        break;
    case 0x6D:
        printf("%02X: ADC, ADRSM: Absolute, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x6E:
        printf("%02X: ROR, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Rotate Right.
        break;
    case 0x6F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x70:
        printf("%02X: BVS, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Overflow Set. Branch taken if overflow flag is set.
        break;
    case 0x71:
        printf("%02X: ADC, ADRSM: (Indirect),Y, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x72:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x73:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x74:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x75:
        printf("%02X: ADC, ADRSM: Zero Page,X, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x76:
        printf("%02X: ROR, ADRSM: Zero Page,X, Flags: N-----ZC\n", *opCode);
        // Rotate Right.
        break;
    case 0x77:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x78:
        printf("%02X: SEI, ADRSM: Implied, Flags: ---I----\n", *opCode);
        // Set Interrupt Disable Status. I = 1
        break;
    case 0x79:
        printf("%02X: ADC, ADRSM: Absolute,Y, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x7A:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x7B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x7C:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x7D:
        printf("%02X: ADC, ADRSM: Absolute,X, Flags: NV----ZC\n", *opCode);
        // Add Memory to Accumulator with Carry. A = A + M + C
        break;
    case 0x7E:
        printf("%02X: ROR, ADRSM: Absolute,X, Flags: N-----ZC\n", *opCode);
        // Rotate Right.
        break;
    case 0x7F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x80:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x81:
        printf("%02X: STA, ADRSM: (Indirect,X), Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x82:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x83:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x84:
        printf("%02X: STY, ADRSM: Zero Page, Flags: --------\n", *opCode);
        // Store Index Y in Memory. M = Y
        break;
    case 0x85:
        printf("%02X: STA, ADRSM: Zero Page, Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x86:
        printf("%02X: STX, ADRSM: Zero Page, Flags: --------\n", *opCode);
        // Store Index X in Memory. M = X
        break;
    case 0x87:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x88:
        printf("%02X: DEY, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Decrement Index Y by One. Y = Y - 1
        break;
    case 0x89:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x8A:
        printf("%02X: TXA, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Transfer Index X to Accumulator. A = X
        break;
    case 0x8B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x8C:
        printf("%02X: STY, ADRSM: Absolute, Flags: --------\n", *opCode);
        // Store Index Y in Memory. M = Y
        break;
    case 0x8D:
        printf("%02X: STA, ADRSM: Absolute, Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x8E:
        printf("%02X: STX, ADRSM: Absolute, Flags: --------\n", *opCode);
        // Store Index X in Memory. M = X
        break;
    case 0x8F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x90:
        printf("%02X: BCC, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Carry Clear. Branch taken if carry flag is clear.
        break;
    case 0x91:
        printf("%02X: STA, ADRSM: (Indirect),Y, Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x92:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x93:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x94:
        printf("%02X: STY, ADRSM: Zero Page,X, Flags: --------\n", *opCode);
        // Store Index Y in Memory. M = Y
        break;
    case 0x95:
        printf("%02X: STA, ADRSM: Zero Page,X, Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x96:
        printf("%02X: STX, ADRSM: Zero Page,Y, Flags: --------\n", *opCode);
        // Store Index X in Memory. M = X
        break;
    case 0x97:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x98:
        printf("%02X: TYA, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Transfer Index Y to Accumulator. A = Y
        break;
    case 0x99:
        printf("%02X: STA, ADRSM: Absolute,Y, Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x9A:
        printf("%02X: TXS, ADRSM: Implied, Flags: --------\n", *opCode);
        // Transfer Index X to Stack Register. SP = X
        break;
    case 0x9B:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x9C:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x9D:
        printf("%02X: STA, ADRSM: Absolute,X, Flags: --------\n", *opCode);
        // Store Accumulator in Memory. M = A
        break;
    case 0x9E:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0x9F:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xA0:
        printf("%02X: LDY, ADRSM: Immediate, Flags: N-----Z-\n", *opCode);
        // Load Index Y with Memory. Y = M
        break;
    case 0xA1:
        printf("%02X: LDA, ADRSM: (Indirect,X), Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xA2:
        printf("%02X: LDX, ADRSM: Immediate, Flags: N-----Z-\n", *opCode);
        // Load Index X with Memory. X = M
        break;
    case 0xA3:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xA4:
        printf("%02X: LDY, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // Load Index Y with Memory. Y = M
        break;
    case 0xA5:
        printf("%02X: LDA, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xA6:
        printf("%02X: LDX, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // Load Index X with Memory. X = M
        break;
    case 0xA7:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xA8:
        printf("%02X: TAY, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Transfer Accumulator to Index Y. Y = A
        break;
    case 0xA9:
        printf("%02X: LDA, ADRSM: Immediate, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xAA:
        printf("%02X: TAX, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Transfer Accumulator to Index X. X = A
        break;
    case 0xAB:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xAC:
        printf("%02X: LDY, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // Load Index Y with Memory. Y = M
        break;
    case 0xAD:
        printf("%02X: LDA, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xAE:
        printf("%02X: LDX, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // Load Index X with Memory. X = M
        break;
    case 0xAF:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xB0:
        printf("%02X: BCS, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Carry Set. Branch taken if carry flag is set.
        break;
    case 0xB1:
        printf("%02X: LDA, ADRSM: (Indirect),Y, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xB2:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xB3:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xB4:
        printf("%02X: LDY, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // Load Index Y with Memory. Y = M
        break;
    case 0xB5:
        printf("%02X: LDA, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xB6:
        printf("%02X: LDX, ADRSM: Zero Page,Y, Flags: N-----Z-\n", *opCode);
        // Load Index X with Memory. X = M
        break;
    case 0xB7:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xB8:
        printf("%02X: CLV, ADRSM: Implied, Flags: -V------\n", *opCode);
        // Clear Overflow Flag. V = 0
        break;
    case 0xB9:
        printf("%02X: LDA, ADRSM: Absolute,Y, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xBA:
        printf("%02X: TSX, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Transfer Stack Pointer to Index X. X = SP
        break;
    case 0xBB:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xBC:
        printf("%02X: LDY, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // Load Index Y with Memory. Y = M
        break;
    case 0xBD:
        printf("%02X: LDA, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // Load Accumulator with Memory. A = M
        break;
    case 0xBE:
        printf("%02X: LDX, ADRSM: Absolute,Y, Flags: N-----Z-\n", *opCode);
        // Load Index X with Memory. X = M
        break;
    case 0xBF:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xC0:
        printf("%02X: CPY, ADRSM: Immediate, Flags: N-----ZC\n", *opCode);
        // Compare Memory and Index Y. Y - M, sets N, Z, C flags.
        break;
    case 0xC1:
        printf("%02X: CMP, ADRSM: (Indirect,X), Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xC2:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xC3:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xC4:
        printf("%02X: CPY, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Compare Memory and Index Y. Y - M, sets N, Z, C flags.
        break;
    case 0xC5:
        printf("%02X: CMP, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xC6:
        printf("%02X: DEC, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // Decrement Memory by One. M = M - 1
        break;
    case 0xC7:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xC8:
        printf("%02X: INY, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Increment Index Y by One. Y = Y + 1
        break;
    case 0xC9:
        printf("%02X: CMP, ADRSM: Immediate, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xCA:
        printf("%02X: DEX, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Decrement Index X by One. X = X - 1
        break;
    case 0xCB:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xCC:
        printf("%02X: CPY, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Compare Memory and Index Y. Y - M, sets N, Z, C flags.
        break;
    case 0xCD:
        printf("%02X: CMP, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xCE:
        printf("%02X: DEC, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // Decrement Memory by One. M = M - 1
        break;
    case 0xCF:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xD0:
        printf("%02X: BNE, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Result Not Zero. Branch taken if zero flag is clear.
        break;
    case 0xD1:
        printf("%02X: CMP, ADRSM: (Indirect),Y, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xD2:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xD3:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xD4:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xD5:
        printf("%02X: CMP, ADRSM: Zero Page,X, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xD6:
        printf("%02X: DEC, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // Decrement Memory by One. M = M - 1
        break;
    case 0xD7:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xD8:
        printf("%02X: CLD, ADRSM: Implied, Flags: ----D---\n", *opCode);
        // Clear Decimal Mode. D = 0
        break;
    case 0xD9:
        printf("%02X: CMP, ADRSM: Absolute,Y, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xDA:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xDB:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xDC:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xDD:
        printf("%02X: CMP, ADRSM: Absolute,X, Flags: N-----ZC\n", *opCode);
        // Compare Memory with Accumulator. A - M, sets N, Z, C flags.
        break;
    case 0xDE:
        printf("%02X: DEC, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // Decrement Memory by One. M = M - 1
        break;
    case 0xDF:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xE0:
        printf("%02X: CPX, ADRSM: Immediate, Flags: N-----ZC\n", *opCode);
        // Compare Memory and Index X. X - M, sets N, Z, C flags.
        break;
    case 0xE1:
        printf("%02X: SBC, ADRSM: (Indirect,X), Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xE2:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xE3:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xE4:
        printf("%02X: CPX, ADRSM: Zero Page, Flags: N-----ZC\n", *opCode);
        // Compare Memory and Index X. X - M, sets N, Z, C flags.
        break;
    case 0xE5:
        printf("%02X: SBC, ADRSM: Zero Page, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xE6:
        printf("%02X: INC, ADRSM: Zero Page, Flags: N-----Z-\n", *opCode);
        // Increment Memory by One. M = M + 1
        break;
    case 0xE7:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xE8:
        printf("%02X: INX, ADRSM: Implied, Flags: N-----Z-\n", *opCode);
        // Increment Index X by One. X = X + 1
        break;
    case 0xE9:
        printf("%02X: SBC, ADRSM: Immediate, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xEA:
        printf("%02X: NOP, ADRSM: Implied, Flags: --------\n", *opCode);
        // No Operation.
        break;
    case 0xEB:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xEC:
        printf("%02X: CPX, ADRSM: Absolute, Flags: N-----ZC\n", *opCode);
        // Compare Memory and Index X. X - M, sets N, Z, C flags.
        break;
    case 0xED:
        printf("%02X: SBC, ADRSM: Absolute, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xEE:
        printf("%02X: INC, ADRSM: Absolute, Flags: N-----Z-\n", *opCode);
        // Increment Memory by One. M = M + 1
        break;
    case 0xEF:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xF0:
        printf("%02X: BEQ, ADRSM: Relative, Flags: --------\n", *opCode);
        // Branch on Result Zero. Branch taken if zero flag is set.
        break;
    case 0xF1:
        printf("%02X: SBC, ADRSM: (Indirect),Y, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xF2:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xF3:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xF4:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xF5:
        printf("%02X: SBC, ADRSM: Zero Page,X, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xF6:
        printf("%02X: INC, ADRSM: Zero Page,X, Flags: N-----Z-\n", *opCode);
        // Increment Memory by One. M = M + 1
        break;
    case 0xF7:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xF8:
        printf("%02X: SED, ADRSM: Implied, Flags: ----D---\n", *opCode);
        // Set Decimal Flag. D = 1
        break;
    case 0xF9:
        printf("%02X: SBC, ADRSM: Absolute,Y, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xFA:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xFB:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xFC:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    case 0xFD:
        printf("%02X: SBC, ADRSM: Absolute,X, Flags: NV----ZC\n", *opCode);
        // Subtract Memory from Accumulator with Borrow. A = A - M - (1 - C)
        break;
    case 0xFE:
        printf("%02X: INC, ADRSM: Absolute,X, Flags: N-----Z-\n", *opCode);
        // Increment Memory by One. M = M + 1
        break;
    case 0xFF:
        printf("%02X: Future Expansion\n", *opCode);
        break;
    default:
        printf("%02X: Unknown Opcode\n", *opCode);
        break;
    }
    return 0;
}