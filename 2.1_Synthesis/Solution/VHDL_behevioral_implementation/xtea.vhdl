LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.numeric_std.ALL;

PACKAGE x_pack IS
    CONSTANT CONST : INTEGER := 32;
    CONSTANT DELTA : UNSIGNED(CONST*2-1 DOWNTO 0) := "0000000000000000000000000000000010011110001101110111100110111001";


end x_pack;

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;
USE WORK.x_pack.ALL;

ENTITY xtea IS
    generic(SIZE  :  INTEGER := 32);
    port(
    clk                   :IN   bit;
    rst                   :IN   bit;
    input_rdy             :IN   bit;
    word1                 :IN   unsigned (SIZE-1 downto 0);
    word2                 :IN   unsigned (SIZE-1 downto 0);
    key0                  :IN   unsigned (SIZE-1 downto 0);
    key1                  :IN   unsigned (SIZE-1 downto 0);
    key2                  :IN   unsigned (SIZE-1 downto 0);
    key3                  :IN   unsigned (SIZE-1 downto 0);
    mode                  :IN   bit;
    result0               :OUT  unsigned(SIZE-1 downto 0);
    result1               :OUT  unsigned(SIZE-1 downto 0);
    output_rdy            :OUT  bit
    );
end xtea;

ARCHITECTURE x_arc of xtea IS

    TYPE STATES IS (Reset_ST, ST_0, ST_1, ST_2, ST_3, ST_4, ST_5, ST_6, ST_7,
                    ST_8, ST_9, ST_10, Final_ST);
    SIGNAL v0             :unsigned(SIZE-1 downto 0);
    SIGNAL v1             :unsigned(SIZE-1 downto 0);
    SIGNAL sum            :unsigned(SIZE*2-1 downto 0);
    SIGNAL counter        :unsigned(5 downto 0);
    SIGNAL delta          :unsigned(SIZE-1 downto 0);
    SIGNAL temp           :unsigned(SIZE-1 downto 0);
    SIGNAL STATUS         :STATES;
    SIGNAL NEXT_STATUS    :STATES;

begin

  fsm:  process(STATUS, input_rdy)

  begin NEXT_STATUS <= STATUS;

      case STATUS is
        when Reset_ST =>
        NEXT_STATUS<=ST_0;
        when ST_0 =>
        if input_rdy = '1' then
          NEXT_STATUS<= ST_1;
        else
          NEXT_STATUS<= ST_0;
        end if;
        when ST_1 =>
        if mode = '0' then
          NEXT_STATUS<= ST_2;
        elsif mode = '1' then
          NEXT_STATUS<= ST_3;
        end if;
        when ST_2 =>
        NEXT_STATUS<= ST_4;
        when ST_3 =>
        NEXT_STATUS<= ST_5;
        when ST_4 =>
        NEXT_STATUS<= ST_6;
        when ST_5 =>
        NEXT_STATUS<= ST_7;
        when ST_6 =>
        NEXT_STATUS<= ST_8;
        when ST_7 =>
        NEXT_STATUS<= ST_9;
        when ST_8 =>
        if counter < 31 then
          NEXT_STATUS <= ST_2;
        else
          NEXT_STATUS<= Final_ST;
        end if;
        when ST_9 =>
        NEXT_STATUS <= ST_10;
        when ST_10 =>
        if counter < 31 then
          NEXT_STATUS<= ST_5;
        else
          NEXT_STATUS<= Final_ST;
        end if;
        when Final_ST =>
        NEXT_STATUS<= ST_0;
      end case;
    end process;

  datapath:   process(clk, rst)

    begin
      if rst /= '0' then
        STATUS <= Reset_ST;
      elsif clk'event and clk = '1' then
        STATUS<=NEXT_STATUS;

        case STATUS is

          when Reset_ST =>
          result0 <= (others => '0');
          result1 <= (others => '0');
          output_rdy <= '0';
          sum <= (others => '0');

          when ST_0 =>
          result0 <= (others => '0');
          result1 <= (others => '0');
          output_rdy <= '0';
          v0 <= (others => '0');
          v1 <= (others => '0');
          sum <= (others => '0');
          temp <= (others => '0');
          counter <= (others => '0');
          delta <= "10011110001101110111100110111001";
        --  delta <= DELTA;
          when ST_1 =>
          v0 <= word1;
          v1 <= word2;

          when ST_2 =>
          if ((sum(SIZE-1 DOWNTO 0) AND to_unsigned(3, SIZE)) = 0) then
          temp <= key0;
          elsif ((sum(SIZE-1 DOWNTO 0) AND to_unsigned(3, SIZE)) = 1) then
          temp <= key1;
          elsif ((sum(SIZE-1 DOWNTO 0) AND to_unsigned(3, SIZE)) = 2) then
          temp <= key2;
          else temp <= key3;
          end if;

          when ST_3 =>
        --  sum <= shift_left(delta, 5);
        --  sum <= "11000110111011110011011100100000";
          sum <= delta *32;

          when ST_4 =>
        --  v0 <= (v0 + ((((shift_left(v1, 4) XOR shift_right(v1, 5)) + v1) XOR (sum + temp))));
          v0 <= v0 + (((shift_left(v1, 4) XOR shift_right(v1, 5)) + v1) XOR (sum(31 DOWNTO 0) + temp));
          sum <= sum + delta;

          when ST_5 =>
          if((shift_right(sum(SIZE-1 DOWNTO 0), 11) AND to_unsigned(3, SIZE)) = 0) then
          temp <= key0;
          elsif((shift_right(sum(SIZE-1 DOWNTO 0), 11) AND to_unsigned(3, SIZE)) = 1) then
          temp <= key1;
          elsif((shift_right(sum(SIZE-1 DOWNTO 0), 11) AND to_unsigned(3, SIZE)) = 2) then
          temp <= key2;
          else temp <= key3;
          end if;

          when ST_6 =>
          if((shift_right(sum(SIZE-1 DOWNTO 0), 11) AND to_unsigned(3, SIZE)) = 0) then
          temp <= key0;
          elsif((shift_right(sum(SIZE-1 DOWNTO 0), 11) AND to_unsigned(3, SIZE)) = 1) then
          temp <= key1;
          elsif((shift_right(sum(SIZE-1 DOWNTO 0), 11) AND to_unsigned(3, SIZE)) = 2) then
          temp <= key2;
          else temp <= key3;
          end if;

          when ST_7 =>
          --v1 <= (v1 - (((((shift_left(v0,4) XOR (shift_right(v0, 5))) + v0) XOR (sum + temp)))));
          v1 <= v1 - (((shift_left(v0, 4) XOR shift_right(v0, 5)) + v0) XOR (sum(31 DOWNTO 0) + temp));
          sum <= sum - delta;

          when ST_8 =>
          --v1 <= (v1 - (((((shift_left(v0,4) XOR (shift_right(v0, 5))) + v0) XOR (sum + temp)))));
          v1 <= v1 + (((shift_left(v0, 4) XOR shift_right(v0, 5)) + v0) XOR (sum(31 DOWNTO 0) + temp));
          if (counter < 31) then
          counter <= counter + 1;
          end if;

          when ST_9 =>
          if ((sum(SIZE-1 DOWNTO 0) AND to_unsigned(3, SIZE)) = 0) then
          temp <= key0;
          elsif ((sum(SIZE-1 DOWNTO 0) AND to_unsigned(3, SIZE)) = 1) then
          temp <= key1;
          elsif ((sum(SIZE-1 DOWNTO 0) AND to_unsigned(3, SIZE)) = 2) then
          temp <= key2;
          else temp <= key3;
          end if;

          when ST_10 =>
          v0 <= v0 - (((shift_left(v1, 4) XOR shift_right(v1, 5)) + v1) XOR (sum(31 DOWNTO 0) + temp));

          --v0 <= (v0 - ((((shift_left(v1, 4) XOR shift_right(v1, 5)) + v1) XOR (sum + temp))));
          if (counter < 31) then
          counter <= counter + 1;
          end if;

          when Final_ST =>
          result0 <= v0;
          result1 <= v1;
          output_rdy <= '1';

      end case;

    end if;

  end process;

end x_arc;
