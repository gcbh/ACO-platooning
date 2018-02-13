import sys

if len(sys.argv) < 3:
	print len(sys.argv)
	raise Exception("Missing file input.")

file_name = sys.argv[1]
coords_file = sys.argv[2]
lines = list()
cities = dict()
num_cities = 0

try:
	f = open(file_name, 'r')
	for line in f:
		l = line.split(' ')
		if not cities.has_key(l[1]):
			cities.update({l[1]: l[0]})
		if not cities.has_key(l[3]):
			cities.update({l[3]: l[2]})
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

try:
	f = open(coords_file, 'r')
	for line in f:
		lines.append(line)
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

new_name = coords_file.split('.')
new_name = new_name[0] + "_update." + new_name[1]
try:
	f = open(new_name, 'w')
	for line in lines:
		l = line.split(' ')

		f.write(
			str(cities[l[1]]) + " " +
			str(l[1]).strip() + " " +
			str(l[2]).strip() + " " +
			str(l[3]).strip() + "\n"
			)
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()
