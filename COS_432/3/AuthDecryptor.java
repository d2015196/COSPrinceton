/**********************************************************************************/
/* AuthDecrytor.java                                                              */
/* ------------------------------------------------------------------------------ */
/* DESCRIPTION: Performs authenticated decryption of data encrypted using         */
/*              AuthEncryptor.java.                                               */
/* ------------------------------------------------------------------------------ */
/* YOUR TASK: Decrypt data encrypted by your implementation of AuthEncryptor.java */
/*            if provided with the appropriate key and nonce.  If the data has    */
/*            been tampered with, return null.                                    */
/*                                                                                */
/**********************************************************************************/
import java.util.Arrays;
public class AuthDecryptor {
    // Class constants.
    public static final int   KEY_SIZE_BYTES = AuthEncryptor.KEY_SIZE_BYTES;
    public static final int NONCE_SIZE_BYTES = AuthEncryptor.NONCE_SIZE_BYTES;
    public static final int OUTPUT_SIZE_BYTES = PRF.OUTPUT_SIZE_BYTES; 
    // Instance variables.
    // IMPLEMENT THIS
    //public final PRF MAC;
    public final byte[] key;  
    public AuthDecryptor(byte[] key) {
        assert key.length == KEY_SIZE_BYTES;
        // IMPLEMENT THIS
        this.key = key; 
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
    // Decrypts and authenticates the contents of <in>.  <in> should have been encrypted
    // using your implementation of AuthEncryptor.
    // The nonce has been included in <in>.
    // If the integrity of <in> cannot be verified, then returns null.  Otherwise,
    // returns a newly allocated byte[] containing the plaintext value that was
    // originally encrypted.
    public byte[] decrypt(byte[] in) {
        // throw new RuntimeException("Unimplemented.");
        // IMPLEMENT THIS
        // extract signature 
        byte[] signature = Arrays.copyOfRange(in, in.length - OUTPUT_SIZE_BYTES, in.length); 
        // extract message + nonce 
        byte[] message_and_nonce = Arrays.copyOfRange(in, 0, in.length - OUTPUT_SIZE_BYTES); 
        // extract nonce 
        byte[] nonce = Arrays.copyOfRange(message_and_nonce, message_and_nonce.length - NONCE_SIZE_BYTES, message_and_nonce.length); 
        // extract message  
        byte[] message = Arrays.copyOfRange(message_and_nonce, 0, message_and_nonce.length - NONCE_SIZE_BYTES); 
        // check nonce size + message size = message_and_nonce size 
        // check signature 
        PRF check_signature = new PRF(key); 
        byte[] computed_signature = check_signature.eval(message_and_nonce); 
        // decrypted text. Not yet sure if will return it as legitimate 
        StreamCipher decryptor = new StreamCipher(key, nonce); 
        byte[] decrypted = new byte[message.length]; 
        decryptor.cryptBytes(message, 0, decrypted, 0, message.length); 
        // if [in] was not encrypted properly with key [key] or the message/nonce were corrupted return null 
        if (!Arrays.equals(signature, computed_signature)) { 
            return (null); 
        }
        else { 
            return (decrypted); 
        }
    }
    // Decrypts and authenticates the contents of <in>.  <in> should have been encrypted
    // using your implementation of AuthEncryptor.
    // The nonce used to encrypt the data is provided in <nonce>.
    // If the integrity of <in> cannot be verified, then returns null.  Otherwise,
    // returns a newly allocated byte[] containing the plaintext value that was
    // originally encrypted.
    public byte[] decrypt(byte[] in, byte[] nonce) {
        assert nonce != null && nonce.length == NONCE_SIZE_BYTES;
      //  throw new RuntimeException("Unimplemented.");
        // IMPLEMENT THIS
        // extract signature 
        byte[] signature = Arrays.copyOfRange(in, in.length - OUTPUT_SIZE_BYTES, in.length); 
        // extract message 
        byte[] message = Arrays.copyOfRange(in, 0, in.length - OUTPUT_SIZE_BYTES); 
        // check signature.length + message.length = in.length 
        byte[] message_and_nonce = concatByteArray(message, nonce); 
        // compute signature 
        PRF check_signature = new PRF(key); 
        byte[] computed_signature = check_signature.eval(message_and_nonce); 
        // decrypted text. Not yet sure if will return it as legitimate 
        StreamCipher decryptor = new StreamCipher(key, nonce); 
        byte[] decrypted = new byte[message.length]; 
        decryptor.cryptBytes(message, 0, decrypted, 0, message.length); 
        // if [in] was not encrypted properly with key [key] or the message/nonce were corrupted return null 
        if (!Arrays.equals(signature, computed_signature)) { 
            return null; 
        }
        else { 
            return decrypted; 
        }
    }
}