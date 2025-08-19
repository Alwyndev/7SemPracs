import random
import math

def is_prime(n):
    if n < 2:
        return False
    for i in range(2, int(math.sqrt(n)) + 1):
        if n % i == 0:
            return False
    return True

def generate_prime(min_val, max_val):
    while True:
        num = random.randint(min_val, max_val)
        if is_prime(num):
            return num

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def mod_inverse(e, phi):
    for d in range(1, phi):
        if (e * d) % phi == 1:
            return d
    return None

def rsa_keygen():
    # Generate two small primes
    p = generate_prime(10, 500)
    q = generate_prime(10, 500)
    while p == q:
        q = generate_prime(10, 500)
    
    n = p * q
    phi = (p - 1) * (q - 1)
    
    # Choose e
    e = 3
    while gcd(e, phi) != 1:
        e += 2
    
    # Calculate d
    d = mod_inverse(e, phi)
    
    return (e, n), (d, n), p, q

def encrypt(message, public_key):
    e, n = public_key
    return pow(message, e, n)

def decrypt(ciphertext, private_key):
    d, n = private_key
    return pow(ciphertext, d, n)

# ---- Added text helpers (byte-per-block) ----
def encrypt_text(text: str, public_key):
    e, n = public_key
    data = text.encode('utf-8')
    if n <= 255:
        raise ValueError("Modulus too small for direct byte encryption; choose larger primes.")
    return [pow(b, e, n) for b in data]

def decrypt_text(blocks, private_key):
    d, n = private_key
    bytes_out = bytearray()
    for c in blocks:
        m = pow(c, d, n)
        if m > 255:
            raise ValueError("Decrypted block out of byte range.")
        bytes_out.append(m)
    return bytes_out.decode('utf-8')

# Example usage
if __name__ == "__main__":
    # Generate keys
    public_key, private_key, p, q = rsa_keygen()
    print(f"Prime p: {p}, Prime q: {q}")
    print(f"Public key (e, n): {public_key}")
    print(f"Private key (d, n): {private_key}")
    e, n = public_key

    mode = input("Encrypt (i)nteger or (t)ext? [i/t]: ").strip().lower()
    if mode == 'i':
        message = int(input("Enter a message (integer < n) to encrypt: "))
        if not (0 <= message < n):
            print(f"Error: integer message must satisfy 0 <= m < n (n={n}). "
                  f"Your input would reduce modulo n to {message % n} and lose information.")
            exit(0)
        print(f"\nOriginal message: {message}")
        
        encrypted = encrypt(message, public_key)
        print(f"Encrypted: {encrypted}")
        
        decrypted = decrypt(encrypted, private_key)
        print(f"Decrypted: {decrypted}")
    else:
        text = input("Enter text to encrypt: ")
        try:
            blocks = encrypt_text(text, public_key)
        except ValueError as err:
            print("Encryption error:", err)
            exit(0)
        print("Encrypted blocks:", blocks)
        recovered = decrypt_text(blocks, private_key)
        print("Decrypted text:", recovered)