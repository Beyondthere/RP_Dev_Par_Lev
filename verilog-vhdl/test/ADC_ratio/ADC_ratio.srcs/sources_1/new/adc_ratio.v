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


module adc_ratio(
input clk,
input wire [13:0]adc, 
output reg [14:0] dac
    );
    wire [14:0] adc_val = (adc << 1);
    reg flag;
    always@(posedge clk) begin
        flag <= (adc_val[14] ~^ adc_val[12]);
        if (flag  == 1'b1 && adc == 14'h3fff) begin
            dac <= 15'h3fff;
        end
        else if( flag == 1'b1 && adc == 14'h0000)begin
            dac <= 15'h0000;
        end
        else begin
        dac <= adc_val;
        end
    end 
    
endmodule
