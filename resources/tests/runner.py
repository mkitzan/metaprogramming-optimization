import os

def main():
	os.system("python3 select.py")
	num = 1
	token = ""
	with open("test-queries.txt", "r") as queries:
		for query in queries:
			query = query.strip()
			if query[0] != "s":
				token = query
				continue
			q = open("query.txt", "w")
			q.write(query + "\n")
			q.close()
			os.system("python3 compose.py")
			os.system("g++ -std=c++2a " + "-D" + token + " -O3 -I../../include -o test test.cpp")
			os.system("./test | sort > cpp-results.txt")
			os.system("sqlite3 data/library.db \"" + query + ";\" | sort > sql-results.txt")
			stream = os.popen("diff cpp-results.txt sql-results.txt")
			res = "Passed"
			for line in stream:
				res = "Failed"
			print("Test " + str(num) + ":\t" + res + "\n\t" + query)
			if res == "Failed":
				exit()
			num += 1

if __name__ == "__main__":
	main()
