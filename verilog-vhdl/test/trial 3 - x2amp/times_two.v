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
output reg [13:0] dac
    );
    wire adc_MSB = adc[13];
    wire [12:0] adc_val = (adc[12:0] << 1);
    
   wire flag = (adc_MSB != adc_val[12]); 
    
    
    always@(posedge clk) begin
        
        if(!flag && adc == 14'h2000) begin
            dac <= 14'h2000;
        end
        
        else if (flag && adc == 14'h3fff) begin
            dac <= 14'h1fff;
        end
        else if( flag && adc == 14'h0000)begin
            dac <= 14'h0000;
        end
        else begin
        dac <= {adc_MSB, adc_val};
        end
    end 
    
endmodule