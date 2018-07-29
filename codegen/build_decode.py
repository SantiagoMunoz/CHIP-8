# This mini-program builds the pattern matching function for the chip8 instruction set from a plain text specification

def get_mask(p):
    mask = ""
    for i in range(len(p)):
        if p[i] >= "a" and p[i] <= "z":
            mask += "0"
        else:
            mask += "F"
    return mask

def get_pattern(p):
    pattern = ""
    for i in range(len(p)):
        if p[i] >= "a" and p[i] <= "z":
            pattern += "0"
        else:
            pattern += p[i]
    return pattern 

finstruction = open("instruction_list.txt","r")
decode = open("c8Decode.c","w")
decode.write("#include \"c8_decode.h\"\n\n")
decode.write("c8Func decode(uint16_t opcode)\n{\n")
for line in finstruction:
    m_list = line.split()
    pattern = get_pattern(m_list[0])
    mask = get_mask(m_list[0])
    key = m_list[2]
    decode.write("  if(match_opcode(opcode,0x{},0x{}))\n".format(pattern, mask));
    decode.write("       return c8_{};\n".format(key));

decode.write("}\n")
finstruction.close()
decode.close()

