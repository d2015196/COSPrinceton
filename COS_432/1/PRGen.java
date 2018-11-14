/**********************************************************************************/
/* PRGen.java                                                                     */
/* ------------------------------------------------------------------------------ */
/* DESCRIPTION: This class implements a backtracking-resistant pseudo-random      */
/*              generator.  It should produce a sequence of pseudo-random bits    */
/*              specified by a key of length <KEY_SIZE_BYTES>.                    */
/* ------------------------------------------------------------------------------ */
/* YOUR TASK: You must write a generator with the following properties:           */
/*            (1) It must be pseudo-random, meaning that there is no way to       */
/*                distinguish its output from that of a truly random generator    */
/*                unless you know the key.                                        */
/*            (2) It must be deterministic, meaning that, if two programs create  */
/*                generators with the same seed and make the same sequence of     */
/*                calls, they should receive the same sequence of bytes.          */
/*            (3) It must be backtracking-resistant, meaning that, even if an     */
/*                adversary observes the full state of the generator at time t,   */
/*                the adversary will not be able to determine the output of the   */
/*                generator at any time prior to t.                               */
/* ------------------------------------------------------------------------------ */
/* NOTE: This class extends java.util.Random, which means that, once implemented, */
/*       you have access to a number of useful utility methods for free.  We      */
/*       highly recommend that you look up the java.util.Random documentation to  */
/*       understand the full API of this class.  For example, you can write       */
/*           prg.nextBytes(outArray);                                             */
/*       instead of                                                               */
/*           for (int i = 0; i < outArray.length; i++) outArray[i] = prg.next();  */
/* ------------------------------------------------------------------------------ */
/* USAGE: Create a generator with a key k by calling the constructor:             */
/*            PRGen prg = new PRGen(k);                                           */
/*                                                                                */
/*        Retrieve pseudo-random bits from the sequence corresponding to key k by */
/*        calling next() (or any related method in the java.util.Random API):     */
/*            int r1 = prg.next(8);  // 8  pseudo-random bits                     */
/*            int r2 = prg.next(32); // 32 pseudo-random bits                     */
/*                                                                                */
/**********************************************************************************/
import java.nio.ByteBuffer; 
import java.util.BitSet; 
import java.util.Arrays; 
import java.lang.Math; 
public class PRGen extends java.util.Random {
    // Class constants.
    public static final int KEY_SIZE_BYTES = PRF.KEY_SIZE_BYTES;
    public static final int OUTPUT_SIZE_BYTES = PRF.OUTPUT_SIZE_BYTES; 
    public static final int BITS_IN_BYTE = 8; 
    public static final int BYTES_IN_INTEGER_TYPE = 4; 
    // source to use ByteArray class: https://stackoverflow.com/questions/6374915/java-convert-int-to-byte-array-of-4-bytes
    // Instance variables.
    // IMPLEMENT THIS
    public PRF prf; 
    public byte[] state; 
    public PRGen(byte[] key) {
        super(); // Calls the parent class's constructor.  Leave this here.
        assert key.length == KEY_SIZE_BYTES;
        state = key;
        prf = new PRF(state); 
        // IMPLEMENT THIS
    }
    // clears the left-most OUTPUT_SIZE_BITS * BITS_IN_BYTE - <bits> bits in to_be_fitted and returns value in integer from
    private int fitting(byte[] to_be_fitted, int bits) {
        BitSet to_be_fitted_bitset = BitSet.valueOf(to_be_fitted); 
        int total_to_clear = OUTPUT_SIZE_BYTES * BITS_IN_BYTE - bits;
        int byte_blocks_clear = (int) Math.floor(total_to_clear / ((1.0) * BITS_IN_BYTE)); 
        int single_bits_clear = total_to_clear - (byte_blocks_clear * BITS_IN_BYTE); 
        // clear byte blocks 
        to_be_fitted_bitset.clear(0, byte_blocks_clear * BITS_IN_BYTE); 
        // clear single bits 
        int end_index = (byte_blocks_clear + 1) * BITS_IN_BYTE - 1; 
        to_be_fitted_bitset.clear(end_index - single_bits_clear + 1, end_index + 1); 
        // bitset back to byte array 
        byte[] fitted = to_be_fitted_bitset.toByteArray(); 
        // read from index, <index> 
        int index = OUTPUT_SIZE_BYTES - BYTES_IN_INTEGER_TYPE; 
        // byte array to integer 
        int fitted_integer = ByteBuffer.allocate(OUTPUT_SIZE_BYTES).put(fitted).getInt(index); 
        return(fitted_integer); 
    }
    // Returns an integer whose low-order <bits> bits are set pseudo-randomly. The
    // higher-order bits should be set to 0.
    protected int next(int bits) {
        // assert <bits> is within the size of an integer type
        assert 0 < bits && bits <= 32;
        // declare PRF with key, state 
        prf = new PRF(state); 
        // output 
        int output_eval_at_integer = 1; 
        // integer convert to bytes 
       // byte[] output_eval_at = ByteBuffer.allocate(KEY_SIZE_BYTES).putInt(output_eval_at_integer).array();
        byte[] output_eval_at = new byte[BYTES_IN_INTEGER_TYPE]; 
        output_eval_at[BYTES_IN_INTEGER_TYPE - 1] = (byte) output_eval_at_integer; 
        // evaluate at PRF
        byte[] output = prf.eval(output_eval_at); 
        // accommodate size of output 
        int output_fitted_integer = fitting(output, bits); 
        // advance 
        int advance_eval_at_integer = 0; 
        // integer convert to bytes 
        // byte[] advance_eval_at = ByteBuffer.allocate(KEY_SIZE_BYTES).putInt(advance_eval_at_integer).array(); 
        byte[] advance_eval_at = new byte[BYTES_IN_INTEGER_TYPE]; 
        state = prf.eval(advance_eval_at); 
        return(output_fitted_integer); 
    }
    public static void main(String[] args) { 
    //   int key_int = 450; 
    //   byte[] key = ByteBuffer.
        int key_int = 450; 
        byte[] key = ByteBuffer.allocate(KEY_SIZE_BYTES).putInt(key_int).array();
        PRGen test = new PRGen(key); 
        int bits_max = 32; 
        for (int i = 0; i < bits_max; i++) { 
            int next = test.next(i); 
            System.out.println("bits = " + i + ", next = " + next); 
        }
    }
}