#!/bin/bash

input_fn='/tmp/nsphere.dat'
num_records=1000000

duration=20.0
fps=60

prerender_res=3840x2160

outfile='/tmp/output_nsphere-ffmpeg.mp4'

src/nsphere-ffmpeg "$input_fn" "$num_records" "$prerender_res" "$duration" "$fps" | ffmpeg -video_size "$prerender_res" -framerate "$fps" -f rawvideo -pix_fmt rgb48le -i - -vf scale=1920:1080 -vcodec libx264 -pix_fmt yuv420p -profile:v baseline -level 3.0 "$outfile"
