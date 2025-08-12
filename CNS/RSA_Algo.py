def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return g, x, y

def mod_inv(a, b):
    g, x, y = extended_gcd(a, b)
    if g != 1:
        return None
    return x % b

def mod_pow(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = (result * base) % mod
        base = (base * base) % mod
        exp >>= 1
    return result

def isPrime(n):
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

# Single-block (kept) ---------------------------------
def encrypt(message, e, n):
    message = message.encode()
    message_int = int.from_bytes(message, byteorder='big')
    cipher_int = mod_pow(message_int, e, n)
    return cipher_int

def decrypt(cipher_int, d, n):
    plain_int = mod_pow(cipher_int, d, n)
    message = plain_int.to_bytes((plain_int.bit_length() + 7) // 8, byteorder='big')
    return message.decode()

# New multi-block helpers -----------------------------
def encrypt_message(message: str, e: int, n: int):
    data = message.encode('utf-8')
    # Maximum bytes per block so that block_int < n
    block_size = (n.bit_length() - 1) // 8
    if block_size <= 0:
        raise ValueError("Modulus too small for any data block.")
    # Warn if original whole message would overflow (explains old behavior)
    whole_int = int.from_bytes(data, 'big')
    if whole_int >= n:
        pass  # This is expected with tiny demo primes; we now split into blocks.
    cipher_blocks = []
    for i in range(len(data)):
        # Simpler: encrypt byte-by-byte (works even with very small n)
        block_int = data[i]
        if block_int >= n:
            raise ValueError("Byte value >= n; choose larger primes.")
        cipher_blocks.append(mod_pow(block_int, e, n))
    return cipher_blocks

def decrypt_message(cipher_blocks, d: int, n: int):
    plain_bytes = bytearray()
    for c in cipher_blocks:
        block_int = mod_pow(c, d, n)
        if block_int >= 256:
            raise ValueError("Decrypted block does not fit in one byte (invalid parameters).")
        plain_bytes.append(block_int)
    return plain_bytes.decode('utf-8')

def main():
    p = int(input("Enter a prime number (p): "))
    q = int(input("Enter another prime number (q): "))
    if not (isPrime(p) and isPrime(q)):
        print("Both numbers must be prime.")
        return
    n = p * q
    phi = (p - 1) * (q - 1)
    
    for i in range(2, phi):
        if gcd(i, phi) == 1:
            e = i
            break
    
    if not (1 < e < phi and gcd(e, phi) == 1):
        print("Invalid public exponent.")
        return
    d = mod_inv(e, phi)
    print("Public Key: (", e, ",", n, ")")
    print("Private Key: (", d, ",", n, ")")

    message = input("Enter the message to encrypt: ")
    print("Original Message:", message)

    # Use block/byte-wise encryption for tiny demo primes
    cipher_blocks = encrypt_message(message, e, n)
    print("Encrypted Blocks:", cipher_blocks)

    decrypted = decrypt_message(cipher_blocks, d, n)
    print("Decrypted Message:", decrypted)


if __name__ == "__main__":
    main()