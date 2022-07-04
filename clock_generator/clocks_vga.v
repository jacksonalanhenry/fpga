module vga_clk_gen (clk,
                    vga_h_sync, vga_v_sync, inDisplayArea, CounterX, CounterY) ;
   input clk;
   output vga_h_sync, vga_v_sync;
   output inDisplayArea;
   output [9:0] CounterX;
   output [8:0] CounterY;
////////////////////////////////////////////////////////////////////////////////


   reg [9:0] CounterX;
   reg [8:0] CounterY;
   wire      CounterXmaxed = (CounterX == 767);

   //CounterX clocking
   always @ ( posedge clk ) begin
      if(CounterXmaxed)
        CounterX <= 0;
      else
        CounterX <= CounterX + 1;
   end

   always @ (posedge clk) begin
      CounterY <= CounterY + 1;
   end

   always @ (posedge clk) begin
      if(CounterXmaxed)
        CounterY <= CounterY + 1;
   end

   reg vga_HS, vga_VS;
   always @ (posedge clk) begin
      vga_HS <= (CounterX[9:4]==6'h2D); // change this value to move the display horizontally
      vga_VS <= (CounterY==500);        // change this value to move the display vertically
   end

   reg inDisplayArea;
   always @(posedge clk) begin
     if(inDisplayArea==0)
       inDisplayArea <= (CounterXmaxed) && (CounterY<480);
     else
       inDisplayArea <= !(CounterX==639);
   end

   assign vga_h_sync = ~vga_HS;
   assign vga_v_sync = ~vga_VS;

endmodule // vga_clk_gen
