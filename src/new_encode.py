import re

with open("output.txt", "r") as f:
    content = f.read()


pairs = re.findall(r'0x([0-9A-Fa-f]{2}),0x([0-9A-Fa-f]{2})', content)

if not pairs:
    print("Error: No encoded,key pairs found in output.txt")
    exit(1)


with open("encoded_payload.bin", "wb") as f:
    for encoded_hex, key_hex in pairs:
        f.write(bytes([int(encoded_hex, 16)]))
        f.write(bytes([int(key_hex, 16)]))

print(f"Created encoded payload file ({len(pairs) * 2} bytes)")


bad_chars = {0x00, 0x0A, 0x0D, 0xFF}
encoded_bytes = [int(e, 16) for e, k in pairs]
found_bad = [b for b in encoded_bytes if b in bad_chars]
if found_bad:
    print(f"WARNING: Found bad characters: {[hex(b) for b in found_bad]}")
else:
    print("[+] No bad characters found. File is valid.")