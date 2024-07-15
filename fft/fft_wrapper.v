////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	thruwire.v
//
// Project:	Verilog Tutorial Example file
//
// Purpose:	An exceptionally simple Verilog file, just connecting one
//		input port (i_sw) to one output port (o_led)
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Written and distributed by Gisselquist Technology, LLC
//
// This program is hereby granted to the public domain.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
// FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////
//
//
`default_nettype none
//
//
//
module fft_wrapper(i_val, o_val);
	input	wire [15:0]	i_val;
	output	wire [15:0] o_val;

	assign	o_val = i_val;
	//assign	o_val = 16'd4;

endmodule
