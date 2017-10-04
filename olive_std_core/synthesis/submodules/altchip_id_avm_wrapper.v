module altchip_id_avm_wrapper (
    clk, reset,
    chipid_address, chipid_read, chipid_readdata, chipid_waitrequest,
    valid_read, valid_readdata
);

input   clk;
input   reset;

input   chipid_address;
input   chipid_read;
output  [31:0] chipid_readdata;
output  chipid_waitrequest;

input   valid_read;
output  [31:0] valid_readdata;

wire    [63:0] chip_id_sig;
wire    data_valid_sig;

parameter DEVICE_FAMILY = "";
parameter VALIDITY_ASSERTION = "ZERO";

altchip_id u_chipid (
	.clkin(clk),
	.chip_id(chip_id_sig),
	.data_valid(data_valid_sig),
	.reset(reset)
);
defparam u_chipid.DEVICE_FAMILY = DEVICE_FAMILY;

generate
    if (VALIDITY_ASSERTION == "ZERO") begin
        assign chipid_readdata = (data_valid_sig == 1'b0) ? 32'b0 :
            ((chipid_address == 1'b1) ? chip_id_sig[63:32] : chip_id_sig[31:0]);
    end
    else begin
        assign chipid_readdata =
            ((chipid_address == 1'b1) ? chip_id_sig[63:32] : chip_id_sig[31:0]);
    end
    if (VALIDITY_ASSERTION == "WAIT") begin
        assign chipid_waitrequest = (data_valid_sig == 1'b0) ? 1'b1 : 1'b0;
    end
    if (VALIDITY_ASSERTION == "STATUS") begin
        assign valid_readdata[32:1] = 31'b0;
        assign valid_readdata[0] = data_valid_sig;
    end
endgenerate

endmodule
