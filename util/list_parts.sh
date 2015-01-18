#!/bin/bash

program=$(basename $0)
input_path=
output_path="/tmp/am335x-boneblack.dts"

print_usage() 
{
	printf "usage: %s [arguements]\n" ${program}
}


decompile_dtb() 
{
	local linput_path=$1
	local loutput_path=$2

	if [ -z "${linput_path}" ]
	then
		uname_r=$(uname -r)
		linput_path="/boot/dtbs/${uname_r}/am335x-boneblack.dtb"
	fi

	dtc -I dtb -i ${linput_path} -O dts -o ${loutput_path}
}

list_parts()
{
	local ldts_path=$1
}

while getopts "hi:o:" opt; do
  case ${opt} in
    i)
			input_path=${OPTARG}
			;;
		o)
			output_path=${OPTARG}
			;;
		h)
			print_usage
			exit 0
			;;
    \?)
      echo "Invalid option: -${OPTARG}" >&2
			;;
    :)
      echo "-${OPTARG} requires an argument." >&2
      exit 1
			;;
  esac
done


decompile_dtb ${input_path} ${output_path}
list_parts ${output_path}
