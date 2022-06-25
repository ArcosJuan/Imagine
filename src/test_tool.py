#!/usr/bin/env python3
import os
import sys
import time


def simple_load(filter, filter_args):
		IMAGE_SIZES=[200,400,600,800,1000,1200,1400,1600,1800, 2000]
		
		for image_size in IMAGE_SIZES:
			# Creates the  file if it doesn't exist.
			
			file_name=f"out_results/{filter}_{image_size}"
			try:
				file = open(file_name, "x")
				file.close()
			except: pass
			
			# Removes the file if has something written in it.
			with open(file_name, "r+") as file: file.truncate(0)
			
			# Reads the results file created by imagine and writes the new file.
			with open(file_name, "a") as out_file:
				for n_threads in range(1,16+1,1): 
					os.system(f"./imagine examples/sing_{image_size}x{image_size}.ppm out/result.ppm -f {filter} {n_threads} {filter_args} --test results 10")
					with open("results", "r") as file:
						next(file)
						for line in file: out_file.write(line)


def batch_load(filter, filter_args):
	IMAGE_FILES_INDEX=[4, 8, 16, 24, 32, 64]
	IMAGE_SIZES=[200,1000,2000]

	
	for image_size in IMAGE_SIZES:
		for image_file in IMAGE_FILES_INDEX:
			# Creates the  file if it doesn't exist.
			
			file_name=f"out_results/{filter}_{image_size}_{image_file}"
			try:
				file = open(file_name, "x")
				file.close()
			except: pass
			
			# Removes the file if has something written in it.
			with open(file_name, "r+") as file: file.truncate(0)
			
			# Reads the results file created by imagine and writes the new file.
			with open(file_name, "a") as out_file:
				for n_threads in range(1,16+1,1): 
					os.system(f"./imagine loader_examples/{image_size}-{image_file} out/ -s -b {n_threads} -f {filter} {filter_args} --test results 10")
					with open("results", "r") as file:
						next(file)
						for line in file: out_file.write(line)


if __name__=="__main__":
	if not os.path.exists('out_results'): os.mkdir("out_results")
	FILTER="edge"
	FILTER_ARGS=""
	simple_load(FILTER, FILTER_ARGS)
	# batch_load(FILTER, FILTER_ARGS)
