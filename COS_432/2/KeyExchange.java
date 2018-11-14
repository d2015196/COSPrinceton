import java.math.BigInteger;
public class KeyExchange {
    public static final int OUTPUT_SIZE_BYTES = PRF.OUTPUT_SIZE_BYTES;
    public static final int OUTPUT_SIZE_BITS = 8 * OUTPUT_SIZE_BYTES;
    public static final int KEY_SIZE_BYTES = PRF.KEY_SIZE_BYTES; 
    public static final int BITS_IN_BYTE = 8; 
    public static final BigInteger p = DHConstants.p; 
    public static final BigInteger g = DHConstants.g; 
    public static int size_p; 
    public static final PRF hash_function = new PRF(new byte[KEY_SIZE_BYTES]); 
    private BigInteger a; 
    private BigInteger b; 
    public KeyExchange(PRGen rand, boolean iAmServer) {
        // Prepares to do a key exchange.   <rand> is a secure pseudorandom generator
        //    that can be used by the implementation.   <iAmServer> is true iff we are
        //    playing the server role in this exchange.   Each exchange has two 
        //    participants; one of them plays the client role and the other plays the
        //    server role.
        //
        // Once the KeyExchange object is created, two things have to happen for the 
        //    key exchange process to be complete.
        // 1.  Call prepareOutMessage on this object, and send the result to the other
        //     participant.
        // 2.  Receive the result of the other participant's prepareOutMessage, and pass it in
        //     as the argument to a call on this object's processInMessage.  
        // These two things can happen in either order, or even concurrently (e.g., in 
        //     different threads).  This code must work correctly regardless of the order.
        //
        // The call to processInMessage should behave as follows:
        //     If passed a null value, then throw a NullPointerException.
        //     Otherwise, if passed a value that could not possibly have been generated
        //        by prepareOutMessage, then return null.
        //     Otherwise, return a "digest" (hash) with the property described below.
        //
        // This code must provide the following security guarantee: If the two 
        //    participants end up with the same non-null digest value, then this digest value
        //    is not known to anyone else.   This must be true even if third parties
        //    can observe and modify the messages sent between the participants.
        // This code is NOT required to check whether the two participants end up with
        //    the same digest value; the code calling this must verify that property.
        // IMPLEMENT THIS
        byte[] p_byte_array = p.toByteArray(); 
        size_p = p_byte_array.length; 
        // generate 'a'
        a = new BigInteger(size_p * BITS_IN_BYTE, rand);
        BigInteger p_minus_1 = p.subtract(BigInteger.ONE); 
        // ensure 'a' less than 'p_minus_1' and greater than 1
        if ((a.compareTo(p_minus_1) != -1) || (a.equals(BigInteger.ONE))){
            a = new BigInteger(size_p * BITS_IN_BYTE, rand); 
        }
        // generate 'b' 
        b = new BigInteger(size_p * BITS_IN_BYTE, rand);
        // ensure 'b' less than 'p_minus_1' and greater than 1
        if ((b.compareTo(p_minus_1) != -1) || (b.equals(BigInteger.ONE))) { 
            b = new BigInteger(size_p * BITS_IN_BYTE, rand); 
        }
    }
    public byte[] prepareOutMessage() {
    //    return null; // IMPLEMENT THIS
        BigInteger outMessage_bigInt = g.modPow(a, p); 
        byte[] outMessage_bigInt_byte_array = HW2Util.bigIntegerToBytes(outMessage_bigInt, size_p); 
        return (outMessage_bigInt.toByteArray()); 
    }
    public byte[] processInMessage(byte[] inMessage) {
        // Output: Digest of size OUTPUT_SIZE_BYTES
        if (inMessage == null)    throw new NullPointerException();
        BigInteger p_minus_1 = p.subtract(BigInteger.ONE); 
        // [inMessage] to [inMessage_big_integer]
        int inMessage_length = inMessage.length;  
        BigInteger inMessage_big_integer = HW2Util.bytesToBigInteger(inMessage); 
        // return null on 1, p-1 
        if ((inMessage_big_integer.equals(BigInteger.ONE)) || (inMessage_big_integer.equals(p_minus_1))) { 
            return null; 
        }
        // [inMessage_big_integer] ^ b mod m 
        BigInteger inMessage_big_integer_own_key = inMessage_big_integer.modPow(b, p);  
        // [inMessage_big_integer_own_key] to [inMessage_byte_array_own_key]
        byte[] inMessage_byte_array_own_key = HW2Util.bigIntegerToBytes(inMessage_big_integer_own_key, size_p); 
        // hash 
        byte[] hashed_inMessage_byte_array_own_key = hash_function.eval(inMessage_byte_array_own_key); 
        
        // return hash 
        return(hashed_inMessage_byte_array_own_key); 
       // BigInteger inMessage_own_key = inMessage.pow(b); 
    //    return(hash)
       // return null; // IMPLEMENT THIS
    }
}