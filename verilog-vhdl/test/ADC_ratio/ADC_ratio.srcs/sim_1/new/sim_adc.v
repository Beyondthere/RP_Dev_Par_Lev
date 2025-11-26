module tb_adc_ratio;

    // Declare signals for the testbench
    reg clk;
    reg [13:0] adc;
    wire [14:0] dac;
    wire flag; // 'flag' must be added as an output to your main module to monitor it here

    // Instantiate the Device Under Test (DUT)
    adc_ratio DUT (
        .clk(clk),
        .adc(adc),
        .dac(dac)
        // .flag(flag) // Uncomment this line if you add 'output reg flag' to your adc_ratio module
    );

    // 2. Generate a Clock Signal
    parameter CLK_PERIOD = 10; // 10 ns period
    initial begin
        clk = 0;
        forever #(CLK_PERIOD/2) clk = ~clk;
    end

    // 3. Generate Input Stimuli
    initial begin
        // Initialize inputs
        adc = 14'h0000; 

        // Start simulation flow
        $display("------------------------------------------------------------------");
        $display("Time (ns) | ADC Input (Hex) | DAC Output (Hex)");
        $display("------------------------------------------------------------------");
        
        // Wait a bit for initialization
        # (CLK_PERIOD * 2); 

        // --- Test Normal Operation (Valid Range) ---
        // Input a valid mid-range value (0V equivalent in offset binary)
        adc = 14'h2000; 
        # CLK_PERIOD;

        // Input a valid high value
        adc = 14'h3000; 
        # CLK_PERIOD;

        // Input a valid low value
        adc = 14'h1000; 
        # CLK_PERIOD;

        // --- Test Max Clip Condition (0x3fff) ---
        // Note the interaction depends on the previous cycle's flag state
        adc = 14'h3fff; 
        # CLK_PERIOD; 
        # CLK_PERIOD; // Check behavior over multiple cycles

        // --- Test Min Clip Condition (0x0000) ---
        adc = 14'h0000; 
        # CLK_PERIOD;
        # CLK_PERIOD; // Check behavior over multiple cycles

        // --- Return to Normal Operation ---
        adc = 14'h2500; // A new valid value
        # CLK_PERIOD;

        // 4. Monitor and End Simulation
        $monitor("%0d          | %h              | %h               ", $time, adc, dac);
        
        # (CLK_PERIOD * 4); // Run for a few more cycles
        $display("------------------------------------------------------------------");
        $display("Simulation finished.");
        $finish; // System task to end the simulation
    end

    // Optional: Dump waveforms for visualization (e.g., using GTKWave)
    initial begin
        $dumpfile("adc_waveform.vcd");
        $dumpvars(0, tb_adc_ratio);
    end

endmodule