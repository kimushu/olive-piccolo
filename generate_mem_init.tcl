#================================================================================
# Memory initialization file generator for Intel Quartus Prime by @kimu_shu
#
# This is an alternative to Intel's generator (mem_init_generate) with
# workarounds for bugs of "quartus_cdb --update_mif"...
#
# **** How to use ****
# 1. Place this script to your Quartus project folder
# 2. Edit your Makefile like below:
#
#   $(ELF) : $(APP_OBJS) $(LINKER_SCRIPT) $(APP_LDDEPS)
#   	@$(ECHO) Info: Linking $@
#   	$(LD) $(APP_LDFLAGS) $(APP_CFLAGS) -o $@ ...
#   ifneq ($(DISABLE_ELFPATCH),1)
#   	$(ELFPATCH) $@ $(ELF_PATCH_FLAG)
# + 	tclsh $(dir $(SOPCINFO_FILE))/generate_mem_init.tcl $@ $(dir $(SOPCINFO_FILE))
#   endif
#
# 3. Every time you build your application, memory initialization files
#    will be updated with the latest content :-)
#================================================================================

proc usage {} {
	puts stderr "Usage: $argv0 \[<options>\] <elf_file> \[<output_directory>\]"
	puts stderr ""
	puts stderr "Options:"
	puts stderr "  -f, --fill <value> : Set filler byte (default: 0x00)"
	puts stderr "  -h, --help         : Print this help"
	puts stderr "  -v, --verbose      : Verbose mode"
}

proc ELF_read_file { path } {
	set fd [ open $path r ]
	fconfigure $fd -translation binary
	set bin [ read $fd ]
	close $fd
	ELF_read $bin
}

proc ELF_read { bin } {
	set EH_SIZE 0x34
	set ELFMAG 0x464c457f
	set EM_ALTERA_NIOS2 113
	set SHT_STRTAB 3

	set sopcinfo {}
	set cpu {}

	# Read file header
	binary scan [ string range $bin 0 [ expr $EH_SIZE - 1 ] ] i4ssiiiiissssss \
		idt typ mac ver ent pho sho flg ehs phs phn shs shn ssx
	if { [ lindex $idt 0 ] != $ELFMAG || $mac != $EM_ALTERA_NIOS2 } {
		puts stderr "Error: Not a valid ELF file for NiosII"
		exit 1
	}
	array set ehdr [ list \
		e_ident     $idt \
		e_type      $typ \
		e_machine   $mac \
		e_version   $ver \
		e_entry     $ent \
		e_phoff     $pho \
		e_shoff     $sho \
		e_flags     $flg \
		e_ehsize    $ehs \
		e_phentsize $phs \
		e_phnum     $phn \
		e_shentsize $shs \
		e_shnum     $shn \
		e_shstrndx  $ssx \
	]

	# Read section header string table
	set tmp [ expr $ehdr(e_shoff) + \
		$ehdr(e_shentsize) * $ehdr(e_shstrndx) ]
	binary scan \
		[ string range $bin $tmp [ expr $tmp + $ehdr(e_shentsize) - 1 ] ] \
		iiiiiiiiii \
		nam typ flg adr ofs siz lnk inf aln esz
	if { $typ == $SHT_STRTAB } {
		set shstrtab [ string range $bin $ofs [ expr $tmp + $siz - 1 ] ]
	} else {
		set shstrtab {}
	}

	# Read section headers
	set shlist [ list ]
	for { set i 0 } { $i < $ehdr(e_shnum) } { incr i } {
		set tmp [ expr $ehdr(e_shoff) + $ehdr(e_shentsize) * $i ]
		binary scan \
			[ string range $bin $tmp [ expr $tmp + $ehdr(e_shentsize) - 1 ] ] \
			iiiiiiiiii \
			nam typ flg adr ofs siz lnk inf aln esz

		set npos [ string first "\0" $shstrtab $nam ]
		if { $npos < 0 } { continue }
		set str [ string range $shstrtab $nam [ expr $npos - 1 ] ]
		lappend shlist [ list \
			name    $str \
			sh_addr $adr \
			sh_size $siz \
		]
		if { $str == ".sopcinfo" } {
			set sopcinfo [ string range $bin $ofs [ expr $ofs + $siz - 1 ] ]
		}
		if { $str == ".cpu" } {
			set cpu [ string range $bin $ofs [ expr $ofs + $siz - 1 ] ]
		}
	}

	# Read program headers
	set phlist [ list ]
	lappend phlist [ array get ehdr ]
	for { set i 0 } { $i < $ehdr(e_phnum) } { incr i } {
		set tmp [ expr $ehdr(e_phoff) + $ehdr(e_phentsize) * $i ]
		binary scan \
			[ string range $bin $tmp [ expr $tmp + $ehdr(e_phentsize) - 1 ] ] \
			iiiiiiii \
			typ off vad pad fsz msz flg aln

		# Search included sections
		set sec {}
		for { set j 0 } { $j < [ llength $shlist ] } { incr j } {
			set tmp [ lindex $shlist $j ]
			array set shdr $tmp
			if { ($vad <= $shdr(sh_addr)) \
				&& ($shdr(sh_addr) < ($vad + $msz)) } {
				lappend sec $tmp
			}
		}

		# Store data
		lappend phlist [ list \
			p_type   $typ \
			p_offset $off \
			p_vaddr  $vad \
			p_paddr  $pad \
			p_filesz $fsz \
			p_memsz  $msz \
			p_flags  $flg \
			p_align  $aln \
			content  [ string range $bin $off [ expr $off + $fsz - 1 ] ] \
			sections $sec \
			discard  0
		]
	}

	return [ list $phlist $sopcinfo $cpu ]
}

proc XML_load { path } {
	set fd [ open $path r ]
	fconfigure $fd -translation binary
	set bin [ read $fd ]
	close $fd
	return [ XML_parse $bin ]
}

proc XML_parse { bin } {
	set idx 0
	return [ XML_parse_inner bin idx {} ]
}

proc XML_parse_inner { binname idxname parent } {
	upvar $binname bin
	upvar $idxname idx
	set end [ string length $bin ]
	set closed 0
	set tags {}
	set data {}
	while { $idx < $end } {
		set tname {}
		set attrs {}
		if { [ regexp -start $idx {\A\s+} $bin match ] } {
			# Spaces => Skip
		} elseif { [ regexp -start $idx {\A<\?[^>]+\?>} $bin match ] } {
			# XML declaration => Skip
		} elseif { [ regexp -start $idx {\A<!--.*?-->} $bin match ] } {
			# Comments => Skip
		} elseif { [ regexp -start $idx {\A<!\[CDATA\[.*?\]\]>} $bin match ] } {
			# CDATA => Skip
		} elseif { [ regexp -start $idx {\A</([^\s>]+)>} $bin match tname ] } {
			# Close tag
			if { $tname != $parent } {
				puts stderr "Error: invalid close tag: $tname (expected $parent) "
				exit 1
			}
			set closed 1
		} elseif { [ regexp -start $idx {\A<([^\s/>]+)([^>]*)>} $bin match tname tattr ] } {
			# Open tag
			set aidx 0
			set aend [ string length $tattr ]
			while { $aidx < $aend } {
				if { [ regexp -start $aidx {\A\s+} $tattr amatch ] } {
					# Spaces => Skip
				} elseif { [ regexp -start $aidx {\A([^=\s]+)=\"([^\"]*)\"\s*} $tattr amatch aname avalue ] } {
					# Value with double-quote
					lappend attrs $aname [ regsub -all {\\\"} $avalue \" ]
				} elseif { [ regexp -start $aidx {\A([^=\s]+)=\'([^\']*)\'\s*} $tattr amatch aname avalue ] } {
					# Value with single-quote
					lappend attrs $aname [ regsub -all {\\\'} $avalue \' ]
				} elseif { [ regexp -start $aidx {\A/$} $tattr amatch ] } {
					set tname {}
					break
				} else {
					puts stderr "Error: invalid attribute: ${tattr}"
					exit 1
				}
				set aidx [ expr $aidx + [ string length $amatch ] ]
			}
		} elseif { [ regexp -start $idx {\A[^<]+} $bin match ] } {
			# Data
			set data $match
		} else {
			# Invalid syntax
			puts stderr "Error: invalid XML: [ string range $bin $idx [ expr $idx + 99 ] ]"
			exit 1
		}
		set idx [ expr $idx + [ string length $match ] ]
		if { $closed == 1 } {
			break;
		}
		if { $tname != "" } {
			set tag [ XML_parse_inner bin idx $tname ]
			lappend tag name $tname attrs $attrs
			lappend tags $tag
		}
	}
	if { $closed == 0 && $parent != "" } {
		puts stderr "Error: tag <$parent> not closed"
		exit 1
	}
	return [ list tags $tags data $data ]
}

proc XML_get_data { docVar } {
	upvar $docVar docList
	array set doc $docList
	return $doc(data)
}

proc XML_get_attr { docVar name } {
	upvar $docVar docList
	array set doc $docList
	array set attrs $doc(attrs)
	return [ array get attrs $name ]
}

proc XML_list_tags { docVar name args } {
	upvar $docVar docList
	array set doc $docList
	set argc [ llength $args ]
	set tags {}
	foreach tagList $doc(tags) {
		array set tag $tagList
		if { $tag(name) != $name } { continue }
		if { $argc >= 1 } {
			set attr [ XML_get_attr tagList [ lindex $args 0 ] ]
			if { $attr == "" } { continue }
			set value [ lindex $attr 1 ]
			if { $argc >= 2 && $value != [ lindex $args 1 ] } { continue }
		}
		lappend tags $tagList
	}
	return $tags
}

proc generate_hex { outfile dataVar base span width baddr } {
	upvar $dataVar data

	proc hex_line { fd addr type data } {
		binary scan $data c* data
		set bytes [ concat \
			[ llength $data ] \
			[ expr $addr >> 8 ] [ expr $addr & 0xff ] \
			[ expr $type ] \
			$data \
		]
		set sum 0
		foreach byte $bytes {
			incr sum $byte
		}
		lappend bytes [ expr (-$sum) & 0xff ]
		binary scan [ binary format c* $bytes ] H* line
		puts $fd ":[ string toupper $line ]"
	}

	set seg {}
	set step [ expr $width / 8 ]
	set records 32

	set fd [ open $outfile w ]
	for { set ofs 0 } { $ofs < $span } { incr ofs $records } {
		set chunk [ string range $data $ofs [ expr $ofs + $records - 1 ] ]
		if { $baddr == 0 } {
			set addr [ expr $ofs / $step ]
			switch $width {
				64 {
					binary scan $chunk w* words
					set chunk [ binary format W* $words ]
				}
				32 {
					binary scan $chunk i* words
					set chunk [ binary format I* $words ]
				}
				16 {
					binary scan $chunk s* words
					set chunk [ binary format S* $words ]
				}
			}
		} else {
			set addr $ofs
		}
		set nseg [ expr ($addr >> 4) & 0xf000 ]
		if { $nseg != $seg } {
			hex_line $fd 0 0x02 [ binary format S $nseg ]
			set seg $nseg
		}
		hex_line $fd [ expr $addr & 0xffff ] 0x00 $chunk
	}
	hex_line $fd 0 0x01 ""
	close $fd
}

proc generate_mif { outfile dataVar base span width baddr } {
	upvar $dataVar data
	set step [ expr $width / 8 ]
	set fd [ open $outfile w ]
	puts $fd "DEPTH = [ expr $span / $step ];"
	puts $fd "WIDTH = $width;"
	puts $fd "ADDRESS_RADIX = HEX;"
	puts $fd "DATA_RADIX = HEX;"
	puts $fd "CONTENT"
	puts $fd "BEGIN"
	for { set ofs 0 } { $ofs < $span } { incr ofs $step } {
		set addr [ expr $ofs / $step ]
		set chunk [ string range $data $ofs [ expr $ofs + $step - 1 ] ]
		switch $width {
			64 {
				binary scan $chunk w word
			}
			32 {
				binary scan $chunk i word
				set word [ expr $word & 0xffffffff ]
			}
			16 {
				binary scan $chunk s word
				set word [ expr $word & 0xffff ]
			}
			8 {
				binary scan $chunk c word
				set word [ expr $word & 0xff ]
			}
		}
		puts $fd [ format "%05X : %0[ expr $width / 4 ]X;" $addr $word ]
	}
	puts $fd "END;"
	close $fd
}

proc main { argv0 argv } {
	set verbose 0
	set elf {}
	set dir {}
	set filler "\x00"
	for { set argi 0 } { $argi < [ llength $argv ] } { incr argi } {
		set arg [ lindex $argv $argi ]
		switch -glob -- $arg {
			-f -
			--fill {
				incr argi
				set filler [ binary format c [ lindex $argv $argi ] ]
			}
			-h -
			--help {
				usage
				exit 1
			}
			-v -
			--verbose {
				set verbose [ expr $verbose + 1 ]
			}
			-* {
				puts stderr "Error: Unknown option: $arg"
				exit 1
			}
			default {
				if { $elf == "" } {
					set elf $arg
				} elseif { $dir == "" } {
					set dir $arg
				} else {
					puts stderr "Error: Too many arguments: $arg"
					exit 1
				}
			}
		}
	}
	if { $elf == "" } {
		puts stderr "Error: no input file"
		exit 1
	}
	if { $dir == "" } {
		set dir "./"
	}

	if { $verbose > 0 } {
		puts stderr "Info: Reading ELF file ($elf)"
	}
	set temp [ ELF_read_file $elf ]
	set phlist [ lindex $temp 0 ]
	set sopcinfo [ lindex $temp 1 ]
	set cpu_path [ lindex $temp 2 ]
	unset temp

	if { $verbose > 0 } {
		puts stderr "Info: Parsing sopcinfo XML ([ string length $sopcinfo ] bytes)"
	}
	set xml [ XML_parse $sopcinfo ]
	set root [ lindex [ XML_list_tags xml EnsembleReport ] 0 ]
	set modules [ XML_list_tags root module ]
	if { $verbose > 0 } {
		puts stderr "Info: Found [ llength $modules ] modules"
	}

	set cpu [ lindex [ XML_list_tags root module path $cpu_path ] 0 ]
	if { $verbose > 0 } {
		set kind [ lindex [ XML_get_attr cpu kind ] 1 ]
		puts stderr "Info: Found CPU ($kind @ $cpu_path)"
	}
	set masters [ XML_list_tags cpu interface kind avalon_master ]
	if { $verbose > 0 } {
		puts stderr "Info: Found [ llength $masters ] avalon masters from CPU"
	}

	foreach module $modules {
		set temp [ lindex [ XML_list_tags module parameter name initMemContent ] 0 ]
		if { $temp != "" } {
			set temp [ lindex [ XML_list_tags temp value ] 0 ]
			set temp [ XML_get_data temp ]
			if { $temp == "false" } { continue }
		}
		set temp [ lindex [ XML_list_tags module parameter name initFlashContent ] 0 ]
		if { $temp != "" } {
			set temp [ lindex [ XML_list_tags temp value ] 0 ]
			set temp [ XML_get_data temp ]
			if { $temp == "false" } { continue }
		}
		set temp [ lindex [ XML_list_tags module parameter name initializationFileName ] 0 ]
		if { $temp == "" } { continue }
		set mod_path [ lindex [ XML_get_attr module path ] 1 ]
		if { $verbose > 0 } {
			set kind [ lindex [ XML_get_attr module kind ] 1 ]
			puts stderr "Info: Found module with memory initialization ($kind @ $mod_path)"
		}
		set temp [ lindex [ XML_list_tags temp value ] 0 ]
		set outfile $dir[ XML_get_data temp ]
		set assignments [ XML_list_tags module assignment ]
		set width {}
		set baddr 0
		foreach assignment $assignments {
			set temp [ lindex [ XML_list_tags assignment name ] 0 ]
			set aname [ XML_get_data temp ]
			set temp [ lindex [ XML_list_tags assignment value ] 0 ]
			set avalue [ XML_get_data temp ]
			switch $aname {
				"embeddedsw.memoryInfo.MEM_INIT_DATA_WIDTH" {
					set width $avalue
				}
				"embeddedsw.memoryInfo.USE_BYTE_ADDRESSING_FOR_HEX" {
					set baddr $avalue
				}
			}
		}
		if { $verbose > 0 } {
			puts stderr "Info: Data width is $width"
			if { $baddr == 1 } {
				puts stderr "Info: Byte-addressing mode enabled"
			}
		}
		set slaves [ XML_list_tags module interface kind avalon_slave ]
		set slave_name {}
		foreach slave $slaves {
			set assignments [ XML_list_tags slave assignment ]
			foreach assignment $assignments {
				set temp [ lindex [ XML_list_tags assignment name ] 0 ]
				set aname [ XML_get_data temp ]
				set temp [ lindex [ XML_list_tags assignment value ] 0 ]
				set avalue [ XML_get_data temp ]
				switch $aname {
					"embeddedsw.configuration.isMemoryDevice" {
						if { $avalue == "1" } {
							set slave_name [ lindex [ XML_get_attr slave name ] 1 ]
							break
						}
					}
				}
			}
			if { $slave_name != "" } { break }
		}
		set slave_path "$mod_path.$slave_name"
		if { $verbose > 0 } {
			puts stderr "Info: Looking up avalon masters connected to $slave_path"
		}
		set base {}
		set span {}
		foreach master $masters {
			set master_path "$cpu_path.[ lindex [ XML_get_attr master name ] 1 ]"
			set blocks [ XML_list_tags master memoryBlock ]
			foreach block $blocks {
				set temp [ lindex [ XML_list_tags block name ] 0 ]
				set temp [ XML_get_data temp ]
				if { $temp == "$slave_path" } {
					set temp [ lindex [ XML_list_tags block baseAddress ] 0 ]
					set base [ XML_get_data temp ]
					set temp [ lindex [ XML_list_tags block span ] 0 ]
					set span [ XML_get_data temp ]
					break
				}
			}
			if { $base != "" } { break }
		}
		if { $base == "" } {
			puts stderr "Warning: No master connected to $slave_path"
			puts stderr "Info: Skipping $outfile"
			continue
		}
		if { $verbose > 0 } {
			set temp [ format "%d bytes @ 0x%08x" $span $base ]
			puts stderr "Info: Found connection between $slave_path and $master_path ($temp)"
		}
		set eaddr [ expr $base + $span ]

		set data [ string repeat $filler $span ]
		set init 0
		set used 0
		foreach ph [ lrange $phlist 1 end ] {
			array set phdr $ph
			set ofs [ expr $phdr(p_paddr) - $base ]
			if { $ofs < 0 } { continue }
			if { $ofs >= $span } { continue }
			if { $phdr(p_filesz) > ($span - $ofs) } {
				puts stderr "Error: Program exceeds memory size! ([
					format "0x%08x-0x%08x" $phdr(p_paddr) [ expr $phdr(p_paddr) + $phdr(p_filesz) ]
				])"
				exit 1
			}
			set last [ expr $ofs + $phdr(p_filesz) - 1 ]
			if { ($last + 1) > $used } {
				set used [ expr $last + 1 ]
			}
			set data [ string replace $data $ofs $last $phdr(content) ]
			set init 1
		}
		if { $init == 0 } {
			puts stderr "Warning: No initialization data for $slave_path"
			puts stderr "Info: Skipping $outfile"
			continue
		}
		set ratio [ expr $used * 100 / $span ]
		puts stderr "Info: Generating $outfile : $used bytes ($ratio%) used"
		switch -glob -nocase $outfile {
			*.hex {
				generate_hex $outfile data $base $span $width $baddr
			}
			*.mif {
				generate_mif $outfile data $base $span $width $baddr
			}
			default {
				puts stderr "Error: Unknown format ($outfile)"
				exit 1
			}
		}
	}
}

main $::argv0 $::argv
