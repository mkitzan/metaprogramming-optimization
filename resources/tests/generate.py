# SQL query generator for test queries
# Generates over 4 million unique queries

import itertools

tables = ["books", "stories", "authored", "collected"]
columns = {
	"books": ["title", "genre", "year", "pages"],
	"stories": ["title", "genre", "year"],
	"authored": ["title", "name"],
	"collected": ["title", "collection", "pages"]
}
joinable = {
	"books": ["authored"],
	"stories": ["authored", "collected"],
	"authored": [],
	"collected": []
}
joins = ["cross", "CROSS", "natural", "NATURAL"]
renames = {
	"genre": "type",
	"year": "published",
}
integral = ["year", "pages"]
all_comp = ["=", "!=", "<>"]
integral_comp = [">", "<", ">=", "<="]
bool_op = ["or", "OR", "and", "AND", "NOT", "not"]
where_data = {
	"name": ["Harlan Elison"],
	"year": [1970],
	"pages": [300],
	"genre": ["science fiction"]
}

query_output = open("all-queries.txt", "w")

def col_list(cs):
	cl = []
	re = False
	cols = ""
	for c in cs:
		cols += c
		if c in renames.keys():
			cols += " as " + renames[c]
			re = True
		cols += ", "
	cl += [cols[:-2]]
	if re:
		cols = ""
		for c in cs:
			cols += c
			cols += ", "	
		cl += [cols[:-2]]
	return cl

def froms(ts):
	f = []
	if len(ts) == 1:
		f = [ts[0]]
	else:
		for j in joins:
			f += [ts[0] + " " + j + " join " + ts[1]]
	return f

def compose(ts, cs, pred):
	if pred != "":
		pred = " where " + pred
	cols = col_list(cs)
	sel = froms(ts)
	for s in sel:
		for c in cols:
			query_output.write("select " + c + " from " + s + pred + "\n")

def next(cs, ci):
	if ci >= len(cs):
		return -1
	else:
		while not cs[ci] in where_data.keys():
			ci += 1
			if ci >= len(cs):
				return -1
		return ci

def predicate(ts, cs, ci, pred):
	ci = next(cs, ci)
	if ci == -1:
		compose(ts, cs, pred)
	else:
		for op in bool_op:
			p = pred + " " + op + " "
			operation(ts, cs, ci, p)

def operation(ts, cs, ci, pred):
	c = cs[ci]
	ops = []
	ops += all_comp
	if c in integral:
		ops += integral_comp
	for op in ops:
		for data in where_data[c]:
			p = pred
			p += c + " " + op + " "
			if type(data) is str:
				p += "\"" + data + "\""
			else:
				p += str(data)
			predicate(ts, cs, ci + 1, p)

def select(ts):
	cols = []
	for t in ts:
		cols += columns[t]
	for i in range(len(cols)):
		comb = list(itertools.combinations(cols, i + 1))
		for cs in comb:
			ci = next(cs, 0)
			if ci == -1:
				compose(ts, cs, "")
			else:
				pred = operation(ts, cs, ci, "")

def root_query(left):
	select([left])
	for right in joinable[left]:
		select([left, right])

def main():
	for table in tables:
		root_query(table)

if __name__ == "__main__":
	main()
	query_output.close()
