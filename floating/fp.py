import struct

def float_to_binary(f):
    # Pack float into 4 bytes (IEEE 754 single-precision)
    packed = struct.pack('>f', f)  # Big-endian float
    # Convert to binary string
    bits = ''.join(f'{byte:08b}' for byte in packed)
    
    # Extract IEEE 754 components
    sign = int(bits[0], 2)
    exponent = int(bits[1:9], 2) - 127  # Unbiased exponent
    mantissa = bits[9:]  # Last 23 bits

    # Compute mantissa value (1 + sum of fractional parts)
    mantissa_value = 1 + sum(int(bit) * 2**(-i) for i, bit in enumerate(mantissa, start=1))

    # Compute final floating-point value
    computed_value = (-1)**sign * mantissa_value * 2**exponent

    # Display results
    print(f"Binary Representation: {bits}")
    print(f"Sign: {sign}, Exponent: {exponent + 127} (biased), Mantissa: {mantissa}")
    print(f"Computed Value: {computed_value}")

# Example: Convert 13.625 to IEEE 754 representation
float_to_binary(13.625)
