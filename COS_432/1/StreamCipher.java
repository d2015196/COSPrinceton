/**********************************************************************************/
/* StreamCipher.java                                                              */
/* ------------------------------------------------------------------------------ */
/* DESCRIPTION: This class implements a stream cipher, which encrypts or decrypts */
/*              a stream of bytes (the two operations are identical).             */
/* ------------------------------------------------------------------------------ */
/* YOUR TASK: Implement a stream cipher.                                          */
/* ------------------------------------------------------------------------------ */
/* USAGE: Create a new StreamCipher with key k of length <KEY_SIZE_BYTES> and     */
/*        nonce n of length NONCE_SIZE_BYTES:                                     */
/*            StreamCipher enc = new StreamCipher(k, n);                          */
/*                                                                                */
/*        Encrypt two bytes b1 and b2:                                            */
/*            byte e1 = enc.cryptByte(b1);                                        */
/*            byte e2 = enc.cryptByte(b2);                                        */
/*                                                                                */
/*        Decrypt two bytes e1 and e2.  First, create a StreamCipher with the     */
/*        same key and nonce, and then call cryptByte() on the encrypted bytes in */
/*        the same order.                                                         */
/*            StreamCipher dec = new StreamCipher(k, n);                          */
/*            byte d1 = dec.cryptByte(e1);                                        */
/*            byte d2 = dec.cryptByte(e2);                                        */
/*            assert (d1 == b1 && d2 == b2);                                      */
/**********************************************************************************/
import java.util.Arrays; 
import java.nio.ByteBuffer; 
public class StreamCipher {
    // Class constants.
    public static final int KEY_SIZE_BYTES   = PRF.KEY_SIZE_BYTES;
    public static final int NONCE_SIZE_BYTES = 8;
    public static final int BITS_IN_BYTE = PRGen.BITS_IN_BYTE;
    public static final int BYTES_IN_INTEGER_TYPE  = PRGen.BYTES_IN_INTEGER_TYPE; 
    // Instance variables.
    // IMPLEMENT THIS
 //   public int nonceIndex; 
  //  public byte[] nonceArr; 
//    public byte[] key; 
    public PRGen generator; 
    // Creates a new StreamCipher with key <key> and nonce composed of
    // nonceArr[nonceOffset] through nonceArr[nonceOffset + NONCE_SIZE_BYTES - 1].
    public StreamCipher(byte[] key, byte[] nonceArr, int nonceOffset) {
        assert key.length == KEY_SIZE_BYTES;
        assert nonceArr.length >= NONCE_SIZE_BYTES + nonceOffset; 
    //    this.key = key; 
        // get nonce 
        byte[] nonce = Arrays.copyOfRange(nonceArr, nonceOffset, nonceOffset + NONCE_SIZE_BYTES); 
        // fk(N) where k is the key, and N is the nonce.  
        PRF for_seed = new PRF(key);
        //for_seed.update(nonce); 
        //byte[] seed =  for_seed.eval(); 
        byte[] seed = for_seed.eval(nonce); 
        // initialize generator 
        generator = new PRGen(seed); 
    //    nonceIndex = 0; 
        // IMPLEMENT THIS
    }
    public StreamCipher(byte[] key, byte[] nonce) {
        this(key, nonce, 0); // Call the other constructor.
    }
    // Encrypts or decrypts the next byte in the stream.
    public byte cryptByte(byte in) {
   //     throw new RuntimeException("Unimplemented.");
        int bits = BITS_IN_BYTE; 
        // generate random integer with <bits> random bits
        int random = generator.next(bits); 
        // random integer to byte array 
        //byte[] random_in_byte_array = ByteBuffer.allocate(BYTES_IN_INTEGER_TYPE).putInt(random).array(); 
        // really really annoyed by how ByteBuffer handles indexing..... would do conversion int to byte[] manually if more time.... 
        byte[] random_in_byte_array = ByteBuffer.allocate(BYTES_IN_INTEGER_TYPE).putInt(random).array(); 
        // pull out the last byte from byte array 
        byte random_byte = random_in_byte_array[BYTES_IN_INTEGER_TYPE - 1]; 
        // fixed casting to byte error by referring to this: https://coderanch.com/t/527437/java/Bitwise-Xor-bytes
        byte xor = (byte) (random_byte ^ in);
        return(xor); 
    
    }
    // Encrypts or decrypts multiple bytes.
    // Encrypts or decrypts inBuf[inOffset] through inBuf[inOffset + numBytes - 1],
    // storing the result in outBuf[outOffset] through outBuf[outOffset + numBytes - 1].
    public void cryptBytes(byte[]  inBuf, int  inOffset, 
                           byte[] outBuf, int outOffset, int numBytes) {
        //throw new RuntimeException("Unimplemented.");
        // IMPLEMENT THIS
        assert inBuf.length >= inOffset + numBytes; 
        assert outBuf.length >= outOffset + numBytes; 
        for (int j = 0; j < numBytes; j++) { 
            outBuf[j + outOffset] = cryptByte(inBuf[j + inOffset]); 
        }
    }
    public static void main(String[] args) { 
        // key      
        int position = KEY_SIZE_BYTES - BYTES_IN_INTEGER_TYPE; 
        int key_integer = 87; 
        byte[] key = ByteBuffer.allocate(KEY_SIZE_BYTES).putInt(key_integer).array(); 
        System.out.println("key: " + Arrays.toString(key)); 
        // nonce 
        int nonce_integer = 24; 
        byte[] nonce = ByteBuffer.allocate(NONCE_SIZE_BYTES).putInt(nonce_integer).array(); 
        // tester Stream Cipher object 
        StreamCipher tester = new StreamCipher(key, nonce); 
        // encrypt bytes
        int in = 2; 
        int capacity = 5; 
        byte[] inBuf = ByteBuffer.allocate(capacity).putInt(in).array(); 
        byte[] outBuf = new byte[capacity]; 
        tester.cryptBytes(inBuf, 0, outBuf, 0, inBuf.length); 
        //System.out.println("inBuf: " + Arrays.toString(inBuf)); 
        //System.out.println("outBuf: " + Arrays.toString(outBuf)); 
    }
}