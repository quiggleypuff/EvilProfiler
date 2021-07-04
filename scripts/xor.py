#!/usr/bin/env python3

import sys

if len(sys.argv) != 4:
    print(f'{sys.argv[0]} xor encrypts a file with a given key')
    print(f'useage: {sys.argv[0]} <input> <xor key> <output>')
    sys.exit()

input_bytes = None

# read input file
with open(sys.argv[1], 'rb') as input_file:
    input_bytes = bytearray(input_file.read())

input_size = len(input_bytes)
xor_key_size = len(sys.argv[2])

# xor input with xor_key
for byte in range(0, input_size):
    input_bytes[byte] ^= ord(sys.argv[2][byte % xor_key_size])


# write output to file
with open(sys.argv[3], 'wb') as output_file:
    output_file.write(input_bytes)
