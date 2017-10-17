#!/bin/bash

here=$(pwd)

echo "use: sh metricas.sh [directory] [list of simulations] [number of simulations]"

echo

for i in $1; do
	for k in $(seq 0 $2); do
		cp $here/$i/simul$k/rd* $here/$i/simul$k/sd* "$here"
		for y in $(seq 0 $3); do
			 $here/etmp4 -F -x sd_a01_$y rd_a01_$y "$here/st_container_cif_h264_300_20.st" "$here/container_cif_h264_300_20.mp4" Video$y
			 ffmpeg -i Video$y.mp4 Video$y.yuv
			 wine $here/msu_metric.exe -f container_cif_h264_300_20.yuv IYUV -yw 352 -yh 288 -f Video$y.yuv -sc 1 -cod . -metr ssim_precise -cc YYUV
			 wine $here/msu_metric.exe -f container_cif_h264_300_20.yuv IYUV -yw 352 -yh 288 -f Video$y.yuv -sc 1 -cod . -metr vqm -cc YYUV
			 mv *csv $i/simul$k/
		done
		rm  rd* sd* Video* loss* rate* delay*

	done
done