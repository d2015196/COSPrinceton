import java.math.BigInteger;
import java.nio.ByteBuffer;
public class RSAKeyPair {
    private RSAKey publicKey;
    private RSAKey privateKey;
    private BigInteger p; 
    private BigInteger q; 
    public RSAKeyPair(PRGen rand, int numBits) {
        // Create an RSA key pair.  rand is a PRGen that this code can use to get pseudorandom
        //     bits.  numBits is the size in bits of each of the primes that will be used.
  		p = BigInteger.probablePrime(numBits, rand); 
  		q = BigInteger.probablePrime(numBits, rand); 
  		BigInteger N = p.multiply(q); 
  		BigInteger p_minus_1 = p.subtract(BigInteger.ONE); 
  		BigInteger q_minus_1 = q.subtract(BigInteger.ONE); 
  		BigInteger product_minus_1 = p_minus_1.multiply(q_minus_1); 
  		// pick e 
 // 		BigInteger bigInt_zero = new BigInteger(0);
 // 		BigInteger bigInt_one = new BigInteger(1); 
  		// e is random prime
  		BigInteger e = BigInteger.probablePrime(numBits, rand); 
  		// pick new e if it is not relatively prime to (p-1)(q-1). 
  		if (e.gcd(product_minus_1) != BigInteger.ONE) { 
  			e = BigInteger.probablePrime(numBits, rand); 
  		}
  		// find out d 
  		BigInteger d = e.modInverse(product_minus_1); 
      // ----------------------------------- ATTENTION NOT DONE ------------------------------
  		// find d 
  		// initialize publicKey and privateKey 
  		publicKey = new RSAKey(e, N); 
  		privateKey = new RSAKey(d, N); 
  		// "You have two options: you can increment through fixed choices for e, you can choose new values for p and q, or 
  		// you can randomly choose new e values" 
        // IMPLEMENT THIS
    }
    public RSAKey getPublicKey() {
        return publicKey;
    }
    public RSAKey getPrivateKey() {
        return privateKey;
    }
    public BigInteger[] getPrimes() {
        // Returns an array containing the two primes that were used in key generation.
        //   In real life we don't always keep the primes around.
        //   But including this helps us grade the assignment.
        BigInteger[] ret = new BigInteger[2];
        ret[0] = p; // IMPLEMENT THIS
        ret[1] = q;
        return ret;
    }
}