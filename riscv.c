#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *memory;

void init(registers_t *starting_registers)
{
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    memory = ht_init(1000);
}

void end()
{
    // TODO: Free everything from memory
    ht_free(memory);
    return;
}

// TODO: create any necessary helper functions

/**
 * Fetch the integer value stored in the register referred to by the input.
 * reg must be of the format "x[0-31]" in order to return a correct value.
 * Otherwise, an error will be thrown.
 */
int fetch_from_reg(char *reg)
{
    strsep(&reg,"x");
    int i = atoi(reg);
    return registers->r[i];
}

/**
 * Store the integer val in the register referred to by the input.
 * reg must be of the format "x[0-31]", otherwise an error will be thrown.
 */
void store_to_reg(char *reg, int val)
{
    strsep(&reg,"x");
    int i = atoi(reg);
    registers->r[i] = val;
}

int process_imm(char *imm)
{
    int base = strstr(imm, "x") != NULL ? 0 : 10;
    return strtol(imm, NULL, base);
}

int execute_R_type(char *op, int rs1, int rs2)
{
    if(strcmp(op,"add") == 0) {
        return rs1 + rs2;
    } else if(strcmp(op,"sub") == 0) {
        return rs1 - rs2;
    } else if(strcmp(op,"and") == 0) {
        return rs1 & rs2;
    } else if(strcmp(op,"or") == 0) {
        return rs1 | rs2;
    } else if(strcmp(op,"xor") == 0) {
        return rs1 ^ rs2;
    } else if(strcmp(op,"slt") == 0) {
        return rs1 < rs2 ? 1 : 0;
    } else if(strcmp(op,"sll") == 0) {
        return rs1 << rs2;
    } else if(strcmp(op,"sra") == 0) {
        return rs1 >> rs2;
    } else {
        return 0;
    }
}

int execute_I_type(char *op, int rs1, int imm)
{
    if(strcmp(op,"addi") == 0) {
        return rs1 + imm;
    } else if(strcmp(op,"andi") == 0) {
        return rs1 & imm;
    } else if(strcmp(op,"ori") == 0) {
        return rs1 | imm;
    } else if(strcmp(op,"xori") == 0) {
        return rs1 ^ imm;
    } else if(strcmp(op,"slti") == 0) {
        return rs1 < imm ? 1 : 0;
    } else {
        return 0;
    }
}

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    ;
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    // TODO: write logic for evaluating instruction on current interpreter state
    const char *delim = " ";
    const char *comma = ",";
    char *token;
    char args[400];

    // get rid of spaces    
    while(instruction != NULL) {
        token = strsep(&instruction, delim);
        strcat(args,token);
    }
 
    char *rd;
    char *rs1;
    char *rs2;
    char *imm;
    int rs1_val;
    int rs2_val;
    int imm_val;
    switch(op_type) {
        case 0: ; // [rd], [rs1], [rs2]
            // break args into register names
            rd = strtok(args, comma);
            rs1 = strtok(NULL, comma);
            rs2 = strtok(NULL, comma);

            rs1_val = fetch_from_reg(rs1);
            rs2_val = fetch_from_reg(rs2);

            // execute operation
            store_to_reg(rd, execute_R_type(op, rs1_val, rs2_val));
            break;
        case 1: ; // [rd], [rs1], [imm]
            rd = strtok(args, comma);
            rs1 = strtok(NULL, comma);
            imm = strtok(NULL, comma);

            rs1_val = fetch_from_reg(rs1);
            imm_val = process_imm(imm);
            store_to_reg(rd, execute_I_type(op, rs1_val, imm_val));
            break;
        case 2: ; // [rd], [imm]([rs1])
            rd = strtok(args, comma);
            imm = strtok(NULL, "(");
            rs1 = strtok(NULL, ")");

            rs1_val = fetch_from_reg(rs1);
            imm_val = process_imm(imm);
            int addr = rs1_val + imm_val; // mem address is immediate plus [rs1]

            int val;
            if(op[0] == 's') {
                val = fetch_from_reg(rd);
                if (op[1] == 'b') { // SB
                    val = val & 0xFF;
                    ht_add(memory, addr, val);
                } else { // SW
                    int lsbyte;
                    for(int i = 0; i <= 3; i++) {
                        lsbyte = (val >> (8*i)) & 0xFF;
                        ht_add(memory, addr+i, lsbyte);
                    }
                }
            } else if(op[0] == 'l') {
                val = ht_get(memory, addr);
                if(op[1] == 'w') { // LW
                    for(int i = 1; i <= 3; i++) {
                        val += (ht_get(memory, addr+i) << (8*i));
                    }
                }
                store_to_reg(rd, val); // LB (and LW)
            }
            break;
        case 3: ; // [rd], [imm]
            rd = strtok(args, comma);
            imm = strtok(NULL, comma);

            int lui;
            lui = process_imm(imm) << 12;
            store_to_reg(rd, lui);
            break;
    }
}
