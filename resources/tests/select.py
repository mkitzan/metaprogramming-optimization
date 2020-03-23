# Randomly selects ~500 queries to test from the 1.4mil query set

import random

def main():
	outfile = open("test-queries.txt", "w")
	h = 100 / 23000
	outfile.write("JOINLESS\n")
	with open("joinless-queries.txt", "r") as infile:
		for line in infile:
			if random.random() < h:
				outfile.write(line)				
	h = 500 / 700000
	outfile.write("NATURAL\n")
	with open("natural-queries.txt", "r") as infile:
		for line in infile:
			if random.random() < h:
				outfile.write(line)				
	outfile.write("CROSS\n")
	with open("cross-queries.txt", "r") as infile:
		for line in infile:
			if random.random() < h:
				outfile.write(line)
	outfile.close()

if __name__ == "__main__":
	main()
