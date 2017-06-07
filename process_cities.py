import sys

if len(sys.argv) < 2:
	print len(sys.argv)
	raise Exception("Missing file input.")

file_name = sys.argv[1]
lines = list()
cities = dict()
num_cities = 0

try:
	f = open(file_name, 'r')
	for line in f:
		lines.append(line)
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

new_name = file_name.split('.')
new_name = new_name[0] + "_p." + new_name[1]
try:
	f = open(new_name, 'w')
	for line in lines:
		l = line.split(' ')
		if not cities.has_key(l[0]):
			cities.update({l[0]: num_cities})
			num_cities += 1
		if not cities.has_key(l[1]):
			cities.update({l[1]: num_cities})
			num_cities += 1
		cost = int(l[2]) * 1.60834 
		f.write(
			str(cities[l[0]]) + " " +
			l[0] + " " +
			str(cities[l[1]]) + " " +
			l[1] + " " +
			str(cost) + "\n"
			)
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()
