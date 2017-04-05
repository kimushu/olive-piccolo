##################################################
#
# MAX10 Dual configuration / rpd file generator
#
#	2017/01/30	s.osafune@j7system.jp
#
##################################################

set module [lindex $quartus(args) 0]

if [string match "quartus_asm" $module] {
    post_message "Running after assembler"

	# get project name path

	set rev_name [lindex $quartus(args) 2]
	set prj_path "output_files/${rev_name}"


	# make cof file

	set cof_xml "<?xml version=\"1.0\" encoding=\"US-ASCII\" standalone=\"yes\"?>\n<cof>\n\t<output_filename>${prj_path}.pof</output_filename>\n\t<n_pages>2</n_pages>\n\t<width>1</width>\n\t<mode>14</mode>\n\t<sof_data>\n\t\t<user_name>Page_0</user_name>\n\t\t<page_flags>1</page_flags>\n\t\t<bit0>\n\t\t\t<sof_filename>${prj_path}.sof<compress_bitstream>1</compress_bitstream></sof_filename>\n\t\t</bit0>\n\t</sof_data>\n\t<sof_data>\n\t\t<user_name>Page_1</user_name>\n\t\t<page_flags>2</page_flags>\n\t\t<bit0>\n\t\t\t<sof_filename>${prj_path}.sof<compress_bitstream>1</compress_bitstream></sof_filename>\n\t\t</bit0>\n\t</sof_data>\n\t<version>9</version>\n\t<create_cvp_file>0</create_cvp_file>\n\t<create_hps_iocsr>0</create_hps_iocsr>\n\t<auto_create_rpd>1</auto_create_rpd>\n\t<create_fif_file>0</create_fif_file>\n\t<options>\n\t</options>\n\t<MAX10_device_options>\n\t\t<por>0</por>\n\t\t<io_pullup>1</io_pullup>\n\t\t<config_from_cfm0_only>0</config_from_cfm0_only>\n\t\t<isp_source>0</isp_source>\n\t\t<verify_protect>0</verify_protect>\n\t\t<epof>0</epof>\n\t\t<ufm_source>0</ufm_source>\n\t</MAX10_device_options>\n\t<advanced_options>\n\t\t<ignore_epcs_id_check>2</ignore_epcs_id_check>\n\t\t<ignore_condone_check>2</ignore_condone_check>\n\t\t<plc_adjustment>0</plc_adjustment>\n\t\t<post_chain_bitstream_pad_bytes>-1</post_chain_bitstream_pad_bytes>\n\t\t<post_device_bitstream_pad_bytes>-1</post_device_bitstream_pad_bytes>\n\t\t<bitslice_pre_padding>1</bitslice_pre_padding>\n\t</advanced_options>\n</cof>\n"
	set cof_path "${prj_path}_auto.cof"

	set cof_file_id [open $cof_path w]
	puts $cof_file_id $cof_xml
	close $cof_file_id


	# convert programming file

#	set dellist [glob -nocomplain $prj_path{*}.rpd]
#	file delete -force {*}$dellist

	set cpf_cmd "quartus_cpf -c ${cof_path}"

	if { [catch {open "|$cpf_cmd"} errcode] } {
		return -code error $errcode
	}
}
