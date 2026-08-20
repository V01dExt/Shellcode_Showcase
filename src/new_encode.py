import re


with open("output.txt", "r") as f:
        content = f.read()


hex_values = re.findall(r'0x[0-9A-Fa-f]{2}', content)
byte_array = bytes([int(h, 16) for h in hex_values])


if byte_array[-1] != 0xAA:
        byte_array += b'\xAA'


with open("encoded_payload.bin", "wb") as f:
        f.write(byte_array)


print(f"Created encoded payload file ({len(byte_array)} bytes)")