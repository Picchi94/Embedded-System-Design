add wave /xtea/rst
add wave /xtea/clk
add wave /xtea/mode
add wave /xtea/word1
add wave /xtea/word2
add wave /xtea/input_rdy
add wave /xtea/output_rdy
add wave /xtea/delta
add wave /xtea/counter
add wave /xtea/key0
add wave /xtea/key1
add wave /xtea/key2
add wave /xtea/key3
add wave /xtea/temp
add wave /xtea/v0
add wave /xtea/v1
add wave /xtea/sum
add wave /xtea/result0
add wave /xtea/result1
add wave /xtea/STATUS
add wave /xtea/NEXT_STATUS


force clk 0 0 ns, 1 1 ns -repeat 2

force rst 1 0 ns, 0 6 ns

force input_rdy 0 0 ns
run 6 ns

echo "First invocation: Encryption "

force word1 16#012345678 0 ns
force word2 16#09abcdeff 0 ns

force key0 2#01101010000111010111100011001000 0 ns
force key1 2#10001100100001101101011001111111 0 ns
force key2 2#00101010011001011011111110111110 0 ns
force key3 2#10110100101111010110111001000110 0 ns

force mode 0 0 ns
force input_rdy 1 0 ns

force rst 1 0 ns
run 4 ns
force rst 0 0 ns

run 300 ns

force rst 1 0 ns, 0 6 ns

force input_rdy 0 0 ns
run 6 ns

echo "Second invocation: Decryption "

force word1 16#99bbb92b 0 ns
force word2 16#3ebd1644 0 ns

force key0 2#01101010000111010111100011001000 0 ns
force key1 2#10001100100001101101011001111111 0 ns
force key2 2#00101010011001011011111110111110 0 ns
force key3 2#10110100101111010110111001000110 0 ns

force mode 1 0 ns
force input_rdy 1 0 ns

run 300 ns
