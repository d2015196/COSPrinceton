import java.math.BigInteger;
import java.nio.ByteBuffer; 
import java.util.Arrays;
//import java.util.BitSet; 
public class RSAKey {
    private BigInteger exponent;
    private BigInteger modulus;
  //  private PRF hash_function; 
    public static final int BYTES_IN_INTEGER = 4; 
    public static final int BITS_IN_BYTE = 8; 
    public static final int KEY_SIZE_BYTES = PRGen.KEY_SIZE_BYTES; 
    // size of OAEP random bit array
    public static final int B1 = 128;
    // size of OAEP zero bit array 
    public static final int B0 = 128; 
    // B1 in bytes 
    public static int B1_in_bytes; 
    // B0 in bytes 
    public static int B0_in_bytes; 
    // size of modulus 
    private int size_modulus; 
    // B1 in bytes 
   //  public static final int B1_in_bytes = B1 / BITS_IN_BYTE; 
    // B0 in bytes 
  //   public static final int B0_in_bytes = B0 / BITS_IN_BYTES; 
 //   public static final int key_for_hash = 3; 
  //  public static final byte[] key_for_hash
    // hash function 
    public static final PRF hash_function = new PRF(new byte[KEY_SIZE_BYTES]); 
    // public static final int OUTPUT_SIZE_BYTES = PRF.OUTPUT_SIZE_BYTES; 
    public RSAKey(BigInteger theExponent, BigInteger theModulus) {
        exponent = theExponent;
        modulus = theModulus;
      //  hash_function = new PRF(key_for_hash);  
        // initialize static final variables that could not be initialized before.. 
        B0_in_bytes = B0 / BITS_IN_BYTE; 
        B1_in_bytes = B1 / BITS_IN_BYTE; 
        byte[] modulus_byte = modulus.toByteArray(); 
        size_modulus = modulus_byte.length; 
    }
    public BigInteger getExponent() {
        return exponent;
    }
    public BigInteger getModulus() {
        return modulus;
    }
    /* Message Padding and OAEP Encoding
     * 
     * The next four methods are public to help us grade the assignment.
     * Implement these methods independent of each other, you should NOT call
     * addPadding/removePadding from within encodeOaep/decodeOaep (or vice-versa).
     * 
     * Encode an input:
     * 
     *     byte[] plaintext = 'Hello World'.getBytes();
     *     byte[] paddedPlaintext = addPadding(plaintext)
     *     byte[] paddedPlaintextOAEP = encodeOaep(paddedPlaintext, prgen);
     * 
     * Recover plaintext:
     * 
     *    byte[] unOAEP = decodeOaep(paddedPlaintextOAEP);
     *    byte[] recoveredPlaintext = removePadding(unOAEP);
     * 
     * In practice, these would be private methods and not part of the public API.
     */ 
    // given the block, oaepBlock, of length, size_modulus, (message + B0 + B1), as built by encryptOaep(), 
    // this function returns the message block (message)
    private byte[] giveMessageBlock(byte[] oaepBlock) { 
        int oaepBlock_length = oaepBlock.length; 
       // assert (oaepBlock.length == size_modulus); 
       //  if ()
        return (Arrays.copyOfRange(oaepBlock, 0, size_modulus - B0_in_bytes - B1_in_bytes)); 
    }
    // returns new array, b, that has the [B1] first elements of array, a.  
    private static byte[] chunkOfA(byte[] a) { 
       // assert a.length => B1; 
        return(Arrays.copyOfRange(a, 0, B1_in_bytes)); 
    }
    // returns byte array, [a1] concat [a2]
    private static byte[] concatByteArray(byte[] a1, byte[] a2) { 
        byte[] concat = new byte[a1.length + a2.length]; 
        // copy over elements in a1 
        int i = 0; 
        for (; i < a1.length; i++) { 
            concat[i] = a1[i]; 
        }
        // copy over elments in a2 
        for (int j = 0; j < a2.length; j++) { 
            concat[j + i] = a2[j]; 
        }
        return concat; 
    }
    // returns array, [a1] padded with [pad_length] zeroes at the front 
    private static byte[] paddedFront(byte[] a1, int pad_length) { 
        byte[] zeroes = new byte[pad_length]; 
        return(concatByteArray(zeroes, a1)); 
    }
    // returns array, [a1] padded with [pad_length] zeroes at the back
     private static byte[] paddedBack(byte[] a1, int pad_length) { 
        byte[] zeroes = new byte[pad_length]; 
        return(concatByteArray(a1, zeroes)); 
    }
    // takes in arrays, [a1] and [a2] where a1.length = a2.length, and returns the xor of arrays, a1 and a2.
    private static byte[] xor_ByteArrays_sameSize(byte[] a1, byte[] a2) { 
         assert a1.length == a2.length;
         byte[] xor = new byte[a1.length]; 
         // source for xor ing byte https://stackoverflow.com/questions/24004579/xor-bytes-in-java
         for (int i = 0; i < a1.length; i++) { 
            xor[i] = (byte)(a1[i] ^ a2[i]);
         }
         return xor; 
    }
/*
    // takes in arrays, [a1] and [a2] and returns the xor of arrays, a1 and a2. 
    private static byte[] accomodate_and_xor(byte[] a1, byte[] a2) { 
        // accomodate array sizes to be equal 
        // find which array is the smaller one and add padding of zeroes to it 
        byte[] larger_array; 
        byte[] smaller_array; 
        if (a1.length > a2.length) { 
            larger_array = a1; 
            smaller_array = a2; 
        }
        else { 
            larger_array = a2; 
            smaller_array = a1; 
        }
        smaller_array = ByteBuffer.allocate(larger_array.length).put(smaller_array).array();
        // call function to compute the xor between these two arrays 
        return(xor_ByteArrays_sameSize(smaller_array, larger_array)); 
    } */ 
    public byte[] encodeOaep(byte[] input, PRGen prgen) {
       //  return null; // IMPLEMENT THIS
        // generate seed for PRGen, using random sequence of numbers generated by [prgen]. 
        // byte[] seed = new byte[KEY_SIZE_BYTES]; 
        // ---------------- CHECK INDEXING ---------------------------------------// 
        // assume that (B1 / BITS_IN_BYTES) is divisible by BYTES_IN_INT and that (B1 / BITS_IN_BYTES) / BYTES_IN_INT >= 1. 
        /*
        int number_next_for_seed = (B1 / BITS_IN_BYTE) / BYTES_IN_INTEGER; 
        for (int i = 0; i < number_next_for_seed; i++) { 
            int index = 4 * i; 
            for (int j = 0; j < BYTES_IN_INT; j++) { 
                int generator_output_int = prgen.next(BYTES_IN_INTEGER * BITS_IN_BYTES); 
                byte[] generator_output = ByteBuffer.allocate(BYTES_IN_INTEGER).putInt(generator_output_int).array(); 
                seed[index + j] = generator_output; 
            }
        } */ 
        // random array B1 bytes 
        byte[] random_seed_array = new byte[B1_in_bytes]; 
        prgen.nextBytes(random_seed_array); 
        // add padding if needed
                // ------------------ check indeces 
        // random_seed_array_padded has length, KEY_SIZE_BYTES
        byte[] random_seed_array_padded = paddedFront(random_seed_array, KEY_SIZE_BYTES - random_seed_array.length); 
        // generator with seed, [random_seed_array]
        PRGen generator = new PRGen(random_seed_array_padded); 
        // generator output 
        // byte[] generator_output = ByteBuffer.allocate(BYTES_IN_INTEGER).putInt(generator_output_int).array(); 
        // message, [input] padded with 0s 
        byte[] message_concat_padding = paddedBack(input, B0_in_bytes); 
        // generator output 
        byte[] generator_output = new byte[message_concat_padding.length]; 
        generator.nextBytes(generator_output); 
        // xor [message_concat_padding] with [generator_output] 
        byte[] first_xor = xor_ByteArrays_sameSize(message_concat_padding, generator_output); 
        // check no errors above, in the size before xor.... if not.... 
        // hash [first_xor]
        byte[] hash_first_xor = hash_function.eval(first_xor); 
        // xor once again with the random_seed_array 
        // byte[] second_xor = xor_ByteArrays_sameSize(hash_first_xor, random_seed_array_padded); 
        byte[] second_xor = xor_ByteArrays_sameSize(chunkOfA(hash_first_xor), random_seed_array); 
        // concatenate first xor with second xor 
        byte[] OAEP_evaluated = concatByteArray(first_xor, second_xor); 
        // assert OAEP_evaluated.length = size_modulus 
        return(OAEP_evaluated); 
        //------------------------------------------------------------------ YOU HAVE TO BE VERY CAREFUL -----------------// 
        // THE PADDING RIGHT NOW TAKES THE FRONT OF THE STRING AND ADDS PADDING, DONT KNOW IF IT MATTERS, BUT IT PADS THE ENNNDD 
        // message concat 0 pad: 
        // int capacity = (B0 / BITS_IN_BYTE) + INPUT.LENGTH; 
        // byte[] input_concatenate_zero_pad = ByteBuffer.allocate(capacity).put(message, 0).array(); 
        // one stream done 
        // run this xor through hash, xor with original random seed 
         
    }
    public byte[] decodeOaep(byte[] input) {
        //  return null; // IMPLEMENT THIS
        // partition off the [input] array into x and y 
        // assert (input.length == size_modulus)
        // [y] is the B1_in_bytes last number of bits, and [x] is the rest 
        int partition = input.length - B1_in_bytes; 
        byte[] x = Arrays.copyOfRange(input, 0, partition); 
        byte[] y = Arrays.copyOfRange(input, partition, input.length); 
        // hash 
        byte[] hash_of_x = hash_function.eval(x); 
        // xor [hash_of_x] with [y]
        byte[] first_xor = xor_ByteArrays_sameSize(chunkOfA(hash_of_x), y);
        // "run random generator" through [first_xor]
        // pad first_xor to make it size, KEY_SIZE_BYTES
        byte[] random_seed_array_padded = paddedFront(first_xor, KEY_SIZE_BYTES - first_xor.length); 
        PRGen generator = new PRGen(first_xor); 
        //generator output 
        byte[] generator_output = new byte[x.length]; 
        generator.nextBytes(generator_output); 
        // xor [x] with [generator_output]
        byte[] second_xor = xor_ByteArrays_sameSize(x, generator_output); 
        //concatenate [second_xor] with [first_xor]
        byte[] OAEP_evaluated = concatByteArray(second_xor, first_xor);
        // assert OAEP_evaluated.length == size_modulus 
        return OAEP_evaluated; 
    }
     
     // adds padding to message, [input], returns padded message block of size, size_modulus - B0_in_bytes - B1_in_bytes.  
    public byte[] addPadding(byte[] input) {
        //return null; //IMPLEMENT THIS
        // condition A: B0_in_bytes + B1_in_bytes + 1 + 1 + padded_input.length = size_modulus
        byte[] padded_input = input; 
        // add the 1 front byte of value 1, to signify the start of the message itself
        byte[] byte_value_1 = new byte[1];
        byte_value_1[0] = 1; 
        padded_input = concatByteArray(byte_value_1, padded_input); 
        // add the 1 front byte of value 0, to avoid x > modulus, where x is the OAEP message 
        byte[] byte_value_0 = new byte[0]; 
        padded_input = concatByteArray(byte_value_0, padded_input); 
        // add bytes of value 0, needed to satisfy condition A. 
        int remaining_padding = size_modulus - B0_in_bytes - B1_in_bytes - input.length - 1 - 1;
        padded_input = paddedFront(padded_input, remaining_padding); 
        return(padded_input); 
    }
    // takes in padded message block [input] of length, size_modulus - B0_in_bytes - B1_in_bytes , and returns message without padding 
    public byte[] removePadding(byte[] input) {
        // return null; // IMPLEMENT THIS
       // assert (input.length = )
       // if (input.length != size_modulus - B0 - B1) { 
       //     return 
       // }
        assert (input.length == size_modulus - B0 - B1); 
        // 
        // find the byte, value 1 
        int i; 
        for (i = 0; i < input.length; i++) { 
            if (input[i] == (byte) 1) { 
                break; 
            }
        }
        // message starts after this byte, value 1 
        return(Arrays.copyOfRange(input, i + 1, input.length)); 
 
    }
    
    public int maxPlaintextLength() {
        // Return the largest N such that any plaintext of size N bytes
        //      can be encrypted with this key and padding/encoding.
        // B0_in_bytes + B1_in_bytes bytes used by OAEP 
        // 1 front byte of value 1, to signify the start of the message itself 
        // 1 front front byte of value 0, to avoid x > modulus, where x is the OAEP message 
        int max_num_bytes_message = size_modulus - B0_in_bytes - B1_in_bytes - 1 - 1; 
        return (max_num_bytes_message); // IMPLEMENT THIS
    }
    
    /*
     * RSA Operations
     */
    
    public byte[] encrypt(byte[] plaintext, PRGen prgen) {
       if (plaintext == null)    throw new NullPointerException();
  //     return null; // IMPLEMENT THIS
       // pad 
       byte[] padded_plain_text = addPadding(plaintext); 
       // encodeOaep 
       byte[] encodeOaep_output = encodeOaep(padded_plain_text, prgen); 
       //int encodeOaep_output_length = encodeOaep_output.length; 
       // encrypt 
       // byte array to big int 
       BigInteger encodeOaep_output_big_integer = HW2Util.bytesToBigInteger(encodeOaep_output); 
       // big int ^ exponent mod modulus
       BigInteger encrypted_encodeOaep_output_big_integer = encodeOaep_output_big_integer.modPow(exponent, modulus); 
       // big int to bytes 
       byte[] encrypted_encodeOaep_output_byte_array = HW2Util.bigIntegerToBytes(encrypted_encodeOaep_output_big_integer, size_modulus); 
       return encrypted_encodeOaep_output_byte_array;  
        
    }
    public byte[] decrypt(byte[] ciphertext) {
        if (ciphertext == null)    throw new NullPointerException();
        // decrypt 
        // assert ciphertext.length == size_modulus 
        // byte[] to big integer 
        BigInteger ciphertext_big_integer = HW2Util.bytesToBigInteger(ciphertext); 
        // int ciphertext_length = ciphertext.length; 
        BigInteger decrypted_ciphertext_big_integer = ciphertext_big_integer.modPow(exponent, modulus); 
        // big integer to byte[] 
        byte[] decrypted_ciphertext_byte_array = HW2Util.bigIntegerToBytes(decrypted_ciphertext_big_integer, size_modulus); 
        // decode
        byte[] decode_decrypted_ciphertext_byte_array = decodeOaep(decrypted_ciphertext_byte_array); 
        // access message block 
        byte[] messageBlock = giveMessageBlock(decode_decrypted_ciphertext_byte_array); 
        // access message without padding 
        byte[] message = removePadding(messageBlock); 
        return message; 
        
      //  return null; // IMPLEMENT THIS
    }
    public byte[] sign(byte[] message, PRGen prgen) {
        // Create a digital signature on <message>. The signature need
        //     not contain the contents of <message>--we will assume
        //     that a party who wants to verify the signature will already
        //     know which message this is (supposed to be) a signature on.
         if (message == null)    throw new NullPointerException();
         // hash message
        byte[] hashed_message = hash_function.eval(message); 
        // encrypt [hashed_message]
        byte[] encrypted_hash = encrypt(hashed_message, prgen); 
        return encrypted_hash; 
       //  return null; // IMPLEMENT THIS
    }
    public boolean verifySignature(byte[] message, byte[] signature) {
        // Verify a digital signature. Returns true if  <signature> is
        //     a valid signature on <message>; returns false otherwise.
        //     A "valid" signature is one that was created by calling
        //     <sign> with the same message, using the other RSAKey that
        //     belongs to the same RSAKeyPair as this object.
        if ((message == null) || (signature == null))    throw new NullPointerException();
        // hash message
        byte[] hashed_message = hash_function.eval(message); 
        // decrypt signature 
        byte[] decrypted_signature = decrypt(signature); 
        // hash of decrypted signature 
        //byte[] hashed_signature = hash_function.eval()
        return (Arrays.equals(decrypted_signature, hashed_message)); 
     //   return false; // IMPLEMENT THIS
    }
    public static void main(String[] args) { 
        // addPadding, removePadding 
        //RSAKey = new RSAKey()
        String sampleArray_string = "Hello World"; 
        byte[] sampleArray = sampleArray_string.getBytes();
        int numBits = 100000; 
        RSAKeyPair tester = new RSAKeyPair(new PRGen(new byte[KEY_SIZE_BYTES]), numBits); 
        RSAKey tester_key = tester.getPublicKey(); 
        byte[] padded_tester_key = tester_key.addPadding(sampleArray); 
        byte[] unpadded_padded = tester_key.removePadding(padded_tester_key); 
        if (sampleArray.equals(unpadded_padded)) { 
            System.out.println("Unpadding unpadded"); 
        }
        else { 
            System.out.println("Unpadding did not unpad correctly, or pad did not pad correctly"); 
        }
       // RSAKey.addPadding()
    }
}