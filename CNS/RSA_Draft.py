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

# Example usage
if __name__ == "__main__":
    # Generate keys
    public_key, private_key, p, q = rsa_keygen()
    print(f"Prime p: {p}, Prime q: {q}")
    print(f"Public key (e, n): {public_key}")
    print(f"Private key (d, n): {private_key}")
    
    # Encrypt and decrypt a message
    message = int(input("Enter a message (integer) to encrypt: "))
    print(f"\nOriginal message: {message}")
    
    encrypted = encrypt(message, public_key)
    print(f"Encrypted: {encrypted}")
    
    decrypted = decrypt(encrypted, private_key)
    print(f"Decrypted: {decrypted}")