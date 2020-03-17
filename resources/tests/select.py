# Randomly selects ~500 queries to test from the 4mil query set

import random

def main():
	h = 500 / 4000000
	outfile = open("test-queries.txt", "w")
	with open("all-queries.txt", "r") as infile:
		for line in infile:
			if random.random() < h:
				outfile.write(line)
	outfile.close()

if __name__ == "__main__":
	main()
