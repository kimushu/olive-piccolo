	component olive_std_core is
		port (
			clk_100m_clk           : in    std_logic                     := 'X';             -- clk
			clk_25m_clk            : in    std_logic                     := 'X';             -- clk
			epcs_cso_n             : out   std_logic;                                        -- cso_n
			epcs_dclk              : out   std_logic;                                        -- dclk
			epcs_asdo              : out   std_logic;                                        -- asdo
			epcs_data0             : in    std_logic                     := 'X';             -- data0
			hostuart_rxd           : in    std_logic                     := 'X';             -- rxd
			hostuart_txd           : out   std_logic;                                        -- txd
			i2c_scl                : in    std_logic                     := 'X';             -- scl
			i2c_scl_oe             : out   std_logic;                                        -- scl_oe
			i2c_sda                : in    std_logic                     := 'X';             -- sda
			i2c_sda_oe             : out   std_logic;                                        -- sda_oe
			mreset_mreset_n        : in    std_logic                     := 'X';             -- mreset_n
			nios2_cpu_resetrequest : in    std_logic                     := 'X';             -- cpu_resetrequest
			nios2_cpu_resettaken   : out   std_logic;                                        -- cpu_resettaken
			pfcif_pfc_clk          : out   std_logic;                                        -- pfc_clk
			pfcif_pfc_reset        : out   std_logic;                                        -- pfc_reset
			pfcif_cmd              : out   std_logic_vector(36 downto 0);                    -- cmd
			pfcif_resp             : in    std_logic_vector(31 downto 0) := (others => 'X'); -- resp
			reset_reset_n          : in    std_logic                     := 'X';             -- reset_n
			sdr_addr               : out   std_logic_vector(11 downto 0);                    -- addr
			sdr_ba                 : out   std_logic_vector(1 downto 0);                     -- ba
			sdr_cas_n              : out   std_logic;                                        -- cas_n
			sdr_cke                : out   std_logic;                                        -- cke
			sdr_cs_n               : out   std_logic;                                        -- cs_n
			sdr_dq                 : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdr_dqm                : out   std_logic_vector(1 downto 0);                     -- dqm
			sdr_ras_n              : out   std_logic;                                        -- ras_n
			sdr_we_n               : out   std_logic;                                        -- we_n
			servo_pwm              : out   std_logic_vector(7 downto 0);                     -- pwm
			servo_dsm              : out   std_logic_vector(7 downto 0);                     -- dsm
			spi_ss_n               : out   std_logic;                                        -- ss_n
			spi_sclk               : out   std_logic;                                        -- sclk
			spi_mosi               : out   std_logic;                                        -- mosi
			spi_miso               : in    std_logic                     := 'X';             -- miso
			swi_cpu_resetrequest   : out   std_logic;                                        -- cpu_resetrequest
			swi_led                : out   std_logic_vector(3 downto 0);                     -- led
			uart0_rxd              : in    std_logic                     := 'X';             -- rxd
			uart0_txd              : out   std_logic;                                        -- txd
			uart0_cts_n            : in    std_logic                     := 'X';             -- cts_n
			uart0_rts_n            : out   std_logic;                                        -- rts_n
			uart1_rxd              : in    std_logic                     := 'X';             -- rxd
			uart1_txd              : out   std_logic;                                        -- txd
			uart1_cts_n            : in    std_logic                     := 'X';             -- cts_n
			uart1_rts_n            : out   std_logic                                         -- rts_n
		);
	end component olive_std_core;

	u0 : component olive_std_core
		port map (
			clk_100m_clk           => CONNECTED_TO_clk_100m_clk,           -- clk_100m.clk
			clk_25m_clk            => CONNECTED_TO_clk_25m_clk,            --  clk_25m.clk
			epcs_cso_n             => CONNECTED_TO_epcs_cso_n,             --     epcs.cso_n
			epcs_dclk              => CONNECTED_TO_epcs_dclk,              --         .dclk
			epcs_asdo              => CONNECTED_TO_epcs_asdo,              --         .asdo
			epcs_data0             => CONNECTED_TO_epcs_data0,             --         .data0
			hostuart_rxd           => CONNECTED_TO_hostuart_rxd,           -- hostuart.rxd
			hostuart_txd           => CONNECTED_TO_hostuart_txd,           --         .txd
			i2c_scl                => CONNECTED_TO_i2c_scl,                --      i2c.scl
			i2c_scl_oe             => CONNECTED_TO_i2c_scl_oe,             --         .scl_oe
			i2c_sda                => CONNECTED_TO_i2c_sda,                --         .sda
			i2c_sda_oe             => CONNECTED_TO_i2c_sda_oe,             --         .sda_oe
			mreset_mreset_n        => CONNECTED_TO_mreset_mreset_n,        --   mreset.mreset_n
			nios2_cpu_resetrequest => CONNECTED_TO_nios2_cpu_resetrequest, --    nios2.cpu_resetrequest
			nios2_cpu_resettaken   => CONNECTED_TO_nios2_cpu_resettaken,   --         .cpu_resettaken
			pfcif_pfc_clk          => CONNECTED_TO_pfcif_pfc_clk,          --    pfcif.pfc_clk
			pfcif_pfc_reset        => CONNECTED_TO_pfcif_pfc_reset,        --         .pfc_reset
			pfcif_cmd              => CONNECTED_TO_pfcif_cmd,              --         .cmd
			pfcif_resp             => CONNECTED_TO_pfcif_resp,             --         .resp
			reset_reset_n          => CONNECTED_TO_reset_reset_n,          --    reset.reset_n
			sdr_addr               => CONNECTED_TO_sdr_addr,               --      sdr.addr
			sdr_ba                 => CONNECTED_TO_sdr_ba,                 --         .ba
			sdr_cas_n              => CONNECTED_TO_sdr_cas_n,              --         .cas_n
			sdr_cke                => CONNECTED_TO_sdr_cke,                --         .cke
			sdr_cs_n               => CONNECTED_TO_sdr_cs_n,               --         .cs_n
			sdr_dq                 => CONNECTED_TO_sdr_dq,                 --         .dq
			sdr_dqm                => CONNECTED_TO_sdr_dqm,                --         .dqm
			sdr_ras_n              => CONNECTED_TO_sdr_ras_n,              --         .ras_n
			sdr_we_n               => CONNECTED_TO_sdr_we_n,               --         .we_n
			servo_pwm              => CONNECTED_TO_servo_pwm,              --    servo.pwm
			servo_dsm              => CONNECTED_TO_servo_dsm,              --         .dsm
			spi_ss_n               => CONNECTED_TO_spi_ss_n,               --      spi.ss_n
			spi_sclk               => CONNECTED_TO_spi_sclk,               --         .sclk
			spi_mosi               => CONNECTED_TO_spi_mosi,               --         .mosi
			spi_miso               => CONNECTED_TO_spi_miso,               --         .miso
			swi_cpu_resetrequest   => CONNECTED_TO_swi_cpu_resetrequest,   --      swi.cpu_resetrequest
			swi_led                => CONNECTED_TO_swi_led,                --         .led
			uart0_rxd              => CONNECTED_TO_uart0_rxd,              --    uart0.rxd
			uart0_txd              => CONNECTED_TO_uart0_txd,              --         .txd
			uart0_cts_n            => CONNECTED_TO_uart0_cts_n,            --         .cts_n
			uart0_rts_n            => CONNECTED_TO_uart0_rts_n,            --         .rts_n
			uart1_rxd              => CONNECTED_TO_uart1_rxd,              --    uart1.rxd
			uart1_txd              => CONNECTED_TO_uart1_txd,              --         .txd
			uart1_cts_n            => CONNECTED_TO_uart1_cts_n,            --         .cts_n
			uart1_rts_n            => CONNECTED_TO_uart1_rts_n             --         .rts_n
		);

