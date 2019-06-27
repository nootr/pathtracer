#!/usr/bin/env python3

## compiler.py
# A compiler to convert renderer function calls into a compact, ASCII-printable,
# bitstream.
#
# The instructionset which is used is as follows:
# * 1       box(down_x, down_y, down_z, up_x, up_y, up_z);
# * 000     sphere(center_x, center_y, center_z, radius);
# * 001     cilinder(bottom_x, bottom_y, bottom_z, height, width);
# * 01000   set_type(id);
# * 01001   if_less_than_one { BLOCK }
# * 01010   invert();
# * 010110  toggle_duplicate();
# * 010111  halt();
# * 011     min();
#
# Commands are semi-colon seperated, whitespace is ignored and all numbers are
# floats that consist of two custom-sized integers (a base and a 10-exponent).
#
# ASCII characters are printable between 0b00100000 and 0b01111111, so the
# first bit must be zero and the third bit must be 1. This means we only have
# 6 bits of data per character, with a bitmask of 0b01011111.

import logging
import re
import argparse

def numToBinary(num, base_length, exponent_length, invert=False):
    """Converts a float into a string of zeroes and ones."""
    logging.debug('Converting to binary: %s', num)
    if isinstance(num, int):
        base = num
    elif '.' in num:
        base = float(num.strip())
    else:
        base = int(num.strip())
    if invert:
        base = -base
    binary = ''
    try:
        sign_binary = '0'
        if base < 0:
            sign_binary = '1'
            base = -base
        logging.debug('Sign: %s', sign_binary)
        exponent = 0
        while abs((base % 1)) > 0.01 and abs(1-(base % 1)) > 0.01:
            base *= 10
            exponent += 1
            logging.debug('Scaling: %f * 1e-%d', base, exponent)
        base_binary = '{0:b}'.format(int(base))
        exponent_binary = ''
        if exponent:
            exponent_binary = '{0:b}'.format(int(exponent))
        assert len(base_binary) <= base_length
        assert len(exponent_binary) <= exponent_length
        binary = sign_binary
        binary += '0'*(base_length-len(base_binary)) + base_binary
        binary += '0'*(exponent_length-len(exponent_binary)) + exponent_binary
        logging.debug('Converted to binary: %s', binary)
    except Exception as e:
        logging.error('Could not convert number to binary: %s [base %d, exp %d]',
            num, base_length, exponent_length)
        raise
    return binary

class Lexer(object):
    """Lexer

    The Lexer turns the code into a list of tokens, which are pulled by the
    Parser one-by-one.
    """
    def __init__(self, code):
        logging.debug('Initializing Lexer with %d bytes of code.', len(code))
        self.code = code
        self._tokens = {
                '^box': 'BOX',
                '^sphere': 'SPHERE',
                '^cilinder': 'CILINDER',
                '^set_type': 'SET',
                '^if_less_than_one': 'IF',
                '^invert': 'INVERT',
                '^toggle_duplicate': 'TOGGLE',
                '^min': 'MIN',
                '^halt': 'HALT',
                '^\(': 'BRACKET_OPEN',
                '^\)': 'BRACKET_CLOSE',
                '^{': 'CURLY_OPEN',
                '^}': 'CURLY_CLOSE',
                '^-?[0-9]*.?[0-9]+': 'NUMBER',
                '^;': 'SEMICOLON',
                '^,': 'COMMA'
                }

    def pull(self):
        """Returns the next token or None on EOF, raises on syntax error."""
        self.code = self.code.strip()
        if not self.code:
            logging.debug('Lexer: pull -> EOF')
            return None
        for regex in self._tokens:
            match = re.search(regex, self.code)
            if match:
                value = match.group(0)
                token_type = self._tokens[regex]
                logging.debug('Lexer: pull -> %s %s',
                        token_type, value)
                self.code = self.code[len(value):]
                return {
                        'type': token_type,
                        'value': value
                        }
        raise SyntaxError(self.code)


class Parser(object):
    """Parser

    Fetches the tokens one-by-one and creates an Abstract Syntax Tree.
    """
    def __init__(self, code):
        logging.debug('Initializing Parser..')
        self._lexer = Lexer(code)
        self._AST = []

    @property
    def AST(self):
        """Creates and returns an Abstract Syntax Tree."""
        try:
            if not self._AST:
                logging.debug('Creating Abstract Syntax Tree..')
                token = self._lexer.pull()
                while token:
                    if token['type'] == 'IF':
                        curly_open = self._lexer.pull()
                        assert curly_open['type'] == 'CURLY_OPEN'
                        branch = [token, curly_open]
                        number_open = 1
                        while number_open:
                            new_token = self._lexer.pull()
                            assert new_token
                            if new_token['type'] == 'CURLY_OPEN':
                                number_open += 1
                            elif new_token['type'] == 'CURLY_CLOSE':
                                number_open -= 1
                            branch.append(new_token)
                        self._AST.append(branch)
                    else:
                        branch = [token]
                        while branch[-1]['type'] != 'SEMICOLON':
                            new_token = self._lexer.pull()
                            assert new_token
                            branch.append(new_token)
                        self._AST.append(branch)
                    token = self._lexer.pull()
            return self._AST
        except Exception as e:
            logging.error('Could not create AST: %s', e)
            raise SyntaxError()


class Compiler(object):
    """Compiler

    Fetches an Abstract Syntax Tree, turns it into a bitstream and packs it in
    a string of ASCII-readable characters.
    """
    def __init__(self, AST):
        logging.debug('Initializing Compiler with AST: %s', AST)
        self.AST = AST
        self._bitstream = ''
        self._compile_functions = {
                'BOX': self._compile_box,
                'SPHERE': self._compile_sphere,
                'CILINDER': self._compile_cilinder,
                'SET': self._compile_set,
                'IF': self._compile_if,
                'INVERT': self._compile_invert,
                'TOGGLE': self._compile_toggle,
                'MIN': self._compile_min,
                'SEMICOLON': self._noop,
                'HALT': self._compile_halt
                }

    @property
    def bytecode(self):
        """Fetches a bitstream and converts it into ASCII-readable bytes."""
        ascii_possible = []
        character = ''
        count = 0
        logging.debug('ASCII-enabling bitstream: %s', self.bitstream)
        bitstream = self.bitstream + '0'*(len(self.bitstream) % 6)
        for c in bitstream:
            if count == 0:
                character += '0'
                count += 1
            elif count == 2:
                character += '1'
                count += 1
            character += c
            count += 1
            if count == 8:
                ascii_possible.append(character)
                character = ''
                count = 0
        logging.debug('ASCII-enabled bitstream: %s', ascii_possible)
        characters = ''
        for char in ascii_possible:
            characters += chr(int(char, 2))
        return characters

    @property
    def bitstream(self):
        """Converts an AST into a string of zeroes and ones."""
        if not self._bitstream:
            if isinstance(self.AST[0], list):
                for branch in self.AST:
                    compiler = Compiler(branch)
                    self._bitstream += compiler.bitstream
            else:
                token_type = self.AST[0]['type']
                compile_function = self._compile_functions.get(token_type,
                        None)
                if not compile_function:
                    raise SyntaxError(self.AST[0]['value'])
                self._bitstream = compile_function()
                logging.debug('Bitstream: %s', self._bitstream)
        return self._bitstream

    def _compile_box(self):
        """Compiles a box() function."""
        binary = '1'
        binary += numToBinary(self.AST[2]['value'], 10, 2, invert=True)
        binary += numToBinary(self.AST[4]['value'], 7, 2, invert=True)
        binary += numToBinary(self.AST[6]['value'], 7, 1, invert=True)
        binary += numToBinary(self.AST[8]['value'], 10, 2)
        binary += numToBinary(self.AST[10]['value'], 10, 2)
        binary += numToBinary(self.AST[12]['value'], 10, 2)
        return binary

    def _compile_sphere(self):
        """Compiles a sphere() function."""
        binary = '000'
        binary += numToBinary(self.AST[2]['value'], 7, 1)
        binary += numToBinary(self.AST[4]['value'], 4, 1)
        binary += numToBinary(self.AST[6]['value'], 7, 1)
        binary += numToBinary(self.AST[8]['value'], 4, 2)
        return binary

    def _compile_cilinder(self):
        """Compiles a cilinder() function."""
        binary = '001'
        binary += numToBinary(self.AST[2]['value'], 7, 1)
        binary += numToBinary(self.AST[4]['value'], 8, 2)
        binary += numToBinary(self.AST[6]['value'], 7, 1)
        binary += numToBinary(self.AST[8]['value'], 7, 2)
        binary += numToBinary(self.AST[10]['value'], 8, 2)
        return binary

    def _compile_set(self):
        """Compiles a set_type() function."""
        binary = '01000'
        binary += numToBinary(self.AST[2]['value'], 3, 0)
        return binary

    def _compile_if(self):
        """Compiles an if-statement."""
        compiler = Compiler(self.AST[2:-2])
        subbinary = compiler.bitstream
        binary = '01001'
        binary += numToBinary(len(subbinary), 7, 1)
        binary += subbinary
        return binary

    def _compile_invert(self):
        """Compiles an invert() function."""
        return '01010'

    def _compile_toggle(self):
        """Compiles a toggle_duplicate() function."""
        return '010110'

    def _compile_min(self):
        """Compiles a min() function."""
        return '011'

    def _compile_halt(self):
        """Compiles a halt() function."""
        return '010111'

    def _noop(self):
        """Do nothing."""
        return ''


def parse_args():
    """Parses arguments, the usual way."""
    parser = argparse.ArgumentParser(description='Compiler for my pathtracer.')
    parser.add_argument('filename', action='store',
            help='Source code')
    parser.add_argument('-d', '--debug',
            help='Enable debug', action='store_true')
    return parser.parse_args()

def read_file(filename):
    """Opens a file and returns its contents."""
    try:
        with open(filename, 'r') as file:
            return file.read()
    except Exception as e:
        logging.error('Could not open file: %s', e)

def run(filename):
    """Runs the program."""
    code = read_file(filename)
    parser = Parser(code)
    compiler = Compiler(parser.AST)
    print('Program: "{}"'.format(compiler.bytecode.replace('"', '\\"')))

if __name__ == "__main__":
    args = parse_args()
    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
    run(args.filename)
