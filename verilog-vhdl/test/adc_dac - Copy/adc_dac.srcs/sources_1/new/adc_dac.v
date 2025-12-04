`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/26/2025 11:55:19 AM
// Design Name: 
// Module Name: adc_ratio
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module adc_ratio_safe(
    input  wire          clk,
    input  wire signed [13:0] adc,   // input from IN1
    output reg  signed [13:0] dac    // output to OUT1
);

    // Intermediate 15-bit value for safe multiplication
    wire signed [14:0] adc_mult;
    assign adc_mult = {adc[13], adc} << 1; // multiply by 2 with sign extension

    always @(posedge clk) begin
        // Saturate to 14-bit signed range
        if (adc_mult > 14'sh1FFF)
            dac <= 14'sh1FFF;
        else if (adc_mult < -14'sh2000)
            dac <= -14'sh2000;
        else
            dac <= adc_mult[13:0];
    end
endmodule
