/**********************************************************************************/
/* AuthEncryptor.java                                                             */
/* ------------------------------------------------------------------------------ */
/* DESCRIPTION: Performs authenticated encryption of data.                        */
/* ------------------------------------------------------------------------------ */
/* YOUR TASK: Implement authenticated encryption, ensuring:                       */
/*            (1) Confidentiality: the only way to recover encrypted data is to   */
/*                perform authenticated decryption with the same key and nonce    */
/*                used to encrypt the data.                                       */
/*            (2) Integrity: A party decrypting the data using the same key and   */
/*                nonce that were used to encrypt it can verify that the data has */
/*                not been modified since it was encrypted.                       */
/*                                                                                */
/**********************************************************************************/
import java.util.Arrays;
public class AuthEncryptor {
    // Class constants.
    public static final int   KEY_SIZE_BYTES = StreamCipher.KEY_SIZE_BYTES;
    public static final int NONCE_SIZE_BYTES = StreamCipher.NONCE_SIZE_BYTES;
    public static final int OUTPUT_SIZE_BYTES = PRF.OUTPUT_SIZE_BYTES; 
    // Instance variables.
    // IMPLEMENT THIS
    public final byte[] key; 
    public AuthEncryptor(byte[] key) {
        assert key.length == KEY_SIZE_BYTES;
        this.key = key; 
        // IMPLEMENT THIS
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
    // Encrypts the contents of <in> so that its confidentiality and integrity are
    // protected against those who do not know the key and nonce.
    // If <nonceIncluded> is true, then the nonce is included in plaintext with the
    // output.
    // Returns a newly allocated byte[] containing the authenticated encryption of
    // the input.
    public byte[] encrypt(byte[] in, byte[] nonce, boolean includeNonce) {
        assert nonce != null && nonce.length == NONCE_SIZE_BYTES;
        // encryptor 
        StreamCipher encryptor = new StreamCipher(key, nonce); 
        // MAC 
        PRF MAC = new PRF(key); 
        // encrypted [in]
        byte[] encrypted_in = new byte[in.length]; 
        encryptor.cryptBytes(in, 0, encrypted_in, 0, in.length); 
        // encrypted_in concat nonce value 
        byte[] encrypted_in_concat_nonce = concatByteArray(encrypted_in, nonce); 
        // MAC encrypted_in concat nonce value 
        byte[] MACd = MAC.eval(encrypted_in_concat_nonce); 
        byte[] output; 
        // concatenate [encrypted_in_concat_nonce] and [MACd], or [encrypted_in] and [MACd] 
        if (includeNonce) {
        // concatenate [encrypted_in_concat_nonce] and [MACd] 
            output = concatByteArray(encrypted_in_concat_nonce, MACd); 
        }
        else { 
        // concatenate [encrypted_in] and [MACd]
            output = concatByteArray(encrypted_in, MACd); 
        }
        return output; 
        // throw new RuntimeException("Unimplemented.");
        // IMPLEMENT THIS
    }
}