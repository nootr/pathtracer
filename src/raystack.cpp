 /* The idea:
  *
  * * Stack-based VM + language
  * * All functions, datastructures, etc. in that language
  *
  * *Operands*
  *
  * Sixteen operands which are described using four bytes; this means three
  * operands fit into two printable ASCII-encoded characters.
  *
  * The stack elements are floats.
  *
  * General:
  * 0010 PSH(VALUE) - Push VALUE to stack
  * ???? POP() - Pop value from top of stack
  * ???? PON(N) - Pop Nth value from top of stack and push to top
  *
  * Functions/branching:
  * ???? CAL() - Relative jump to address on stack and push last address to
  *              stack
  * ???? RET() - Absolute jump to address on stack and reset frame pointer
  * ???? BRZ() - Relative jump to address on stack if value is zero
  * ???? BRN() - Relative jump to address on stack if value is negative
  *
  * Arithmetic:
  * ???? ADD() - Pop two values and push the sum
  * ???? MUL() - Pop two values and push the product
  * ???? DIV() - Pop two values and push the ratio
  * ???? SQR() - Pop a value and push the square root
  * 0100 INV() - Pop a value and push it multiplied by -1
  *
  * System:
  * 0011 RND() - Push a random value between 0.0 and 1.0
  * 0001 PNT() - Pop a value, convert it to an integer, then to a character and
  *              print it to STDOUT
  * 0000 HLT() - Halt the program
  *
  * If more characters are needed for the C description of an operand than it's
  * compiled version using other operands (like a NOT operand which is shorter
  * to write using a function with a JIZ operand), then the operand is removed
  * from the list.
  */

#include <stdio.h>  // printf()
#include <stdlib.h> // rand(), RAND_MAX

float stack[999];
/* Test program:
 * 0010 0000 1010 PSH(10) # Push newline to stack
 * 0010 0010 0001 PSH(33) # Push '!' to stack
 * 0001           PNT()   # Print "!\n"
 * 0001           PNT()
 * 0000           HLT()
 *
 * Ascii-printable:
 * 00101000 '('
 * 00101010 '*'
 * 00101000 '('
 * 01100001 'a'
 * 00100100 '$'
 * 00110000 '0'
 *
 * "(*(a$0"
 */
char instructions[] = "(*(a$0";
int sp=0,fp,op,ip=0;

int get_quad(int x=4)
{
  int y = 0;
  for(;x;)
    ++ip%8 && ip%8!=2 ? y=(y<<1)+((128>>(ip%8))&instructions[ip/8]?1:0),x--:x;
  return y;
}

int main() {
  printf("P6 200 100 255 ");
  while((op=get_quad()))
    --op?--op?--op?--op?--op?
              op
            :
              // 0101 ?
              op
          :
            // 0100 INV() - Pop, multiply by -1 and push
            stack[sp] *= -1
        :
          // 0011 RND() - Push random value between 0.0 and 1.0
          stack[++sp] = (float) rand() / RAND_MAX
      :
        // 0010 PSV(VALUE) - Push VALUE to stack
        stack[++sp] = get_quad(8)
    :
      // 0001 PNT() - Pop and print
      printf("%c", (int)stack[sp--]);
  return 0;
}
