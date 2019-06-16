#!/usr/bin/env python3

def numToBinary(num, length):
    assert int(num) == num
    binary = '{0:b}'.format(int(num))
    assert len(binary) <= length
    return '0'*(length-len(binary))+binary

def convertBitstream(bitstream):
    ascii_possible = []
    character = ''
    count = 0
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
    if character:
        character += '0'*(8-len(character))
        ascii_possible.append(character)
    characters = ''
    for char in ascii_possible:
        print(char)
        characters += chr(int(char, 2))
    return characters


c = input('Sphere, Box or Cilinder? [SBC] ')
bitstream = ''
if c == 'S':
    raise NotImplementedError
elif c == 'B':
    bitstream += '1'
    a = float(input('First number? '))
    a += 1
    exponent = 0
    while int(a) != a:
        exponent += 1
        a *= 10
    bitstream += numToBinary(a, 10)
    bitstream += numToBinary(exponent, 2)

    b = float(input('Second number? '))
    exponent = 0
    while int(b) != b:
        exponent += 1
        b *= 10
    bitstream += numToBinary(b, 7)
    bitstream += numToBinary(exponent, 2)

    c = float(input('Third number? '))
    c += 9
    exponent = 0
    while int(c) != c and exponent < 1:
        exponent += 1
        c *= 10
    bitstream += numToBinary(int(c), 7)
    bitstream += numToBinary(exponent, 1)

    a = float(input('Fourth number? '))
    exponent = 0
    while int(a) != a and exponent < 2:
        exponent += 1
        a *= 10
    bitstream += numToBinary(int(a), 10)
    bitstream += numToBinary(exponent, 2)

    a = float(input('Fifth number? '))
    exponent = 0
    while int(a) != a and exponent < 2:
        exponent += 1
        a *= 10
    bitstream += numToBinary(int(a), 10)
    bitstream += numToBinary(exponent, 2)

    a = float(input('Last number? '))
    a += 8.6
    exponent = 0
    while int(a) != a and exponent < 2:
        exponent += 1
        a *= 10
    bitstream += numToBinary(int(a), 10)
    bitstream += numToBinary(exponent, 2)



elif c == 'C':
    raise NotImplementedError
else:
    print('Please enter S, B or C.')

print(bitstream)
print('"%s"' % convertBitstream(bitstream))
