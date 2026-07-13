`timescale 1ns / 1ps


module my_timer(
    input wire clk,
    input wire [1:0] signal,
    output reg [63:0] count
    );
    
    always @(posedge clk) begin  
        if(signal == 2'd2) begin
            count <= 64'd0;
        end
        else if(signal == 2'd1) begin
            count <= count + 64'd1;
        end
    end
  
endmodule
