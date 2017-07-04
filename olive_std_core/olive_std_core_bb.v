
module olive_std_core (
	clk_100m_clk,
	clk_25m_clk,
	epcs_cso_n,
	epcs_dclk,
	epcs_asdo,
	epcs_data0,
	hostuart_rxd,
	hostuart_txd,
	i2c_scl,
	i2c_scl_oe,
	i2c_sda,
	i2c_sda_oe,
	mreset_mreset_n,
	nios2_cpu_resetrequest,
	nios2_cpu_resettaken,
	pfcif_pfc_clk,
	pfcif_pfc_reset,
	pfcif_cmd,
	pfcif_resp,
	reset_reset_n,
	sdr_addr,
	sdr_ba,
	sdr_cas_n,
	sdr_cke,
	sdr_cs_n,
	sdr_dq,
	sdr_dqm,
	sdr_ras_n,
	sdr_we_n,
	servo_pwm,
	servo_dsm,
	spi_ss_n,
	spi_sclk,
	spi_mosi,
	spi_miso,
	swi_cpu_resetrequest,
	swi_led,
	uart0_rxd,
	uart0_txd,
	uart0_cts_n,
	uart0_rts_n,
	uart1_rxd,
	uart1_txd,
	uart1_cts_n,
	uart1_rts_n);	

	input		clk_100m_clk;
	input		clk_25m_clk;
	output		epcs_cso_n;
	output		epcs_dclk;
	output		epcs_asdo;
	input		epcs_data0;
	input		hostuart_rxd;
	output		hostuart_txd;
	input		i2c_scl;
	output		i2c_scl_oe;
	input		i2c_sda;
	output		i2c_sda_oe;
	input		mreset_mreset_n;
	input		nios2_cpu_resetrequest;
	output		nios2_cpu_resettaken;
	output		pfcif_pfc_clk;
	output		pfcif_pfc_reset;
	output	[36:0]	pfcif_cmd;
	input	[31:0]	pfcif_resp;
	input		reset_reset_n;
	output	[11:0]	sdr_addr;
	output	[1:0]	sdr_ba;
	output		sdr_cas_n;
	output		sdr_cke;
	output		sdr_cs_n;
	inout	[15:0]	sdr_dq;
	output	[1:0]	sdr_dqm;
	output		sdr_ras_n;
	output		sdr_we_n;
	output	[7:0]	servo_pwm;
	output	[7:0]	servo_dsm;
	output		spi_ss_n;
	output		spi_sclk;
	output		spi_mosi;
	input		spi_miso;
	output		swi_cpu_resetrequest;
	output	[3:0]	swi_led;
	input		uart0_rxd;
	output		uart0_txd;
	input		uart0_cts_n;
	output		uart0_rts_n;
	input		uart1_rxd;
	output		uart1_txd;
	input		uart1_cts_n;
	output		uart1_rts_n;
endmodule
