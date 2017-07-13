import sys

if len(sys.argv) < 2:
	print len(sys.argv)
	raise Exception("Missing file input.")

file_name = sys.argv[1]
cities = dict()

try:
	f = open(file_name, 'r')
	for line in f:
		data = line.split(' ')
		cities.update({data[0] : data[1]})
		cities.update({data[2] : data[3]})
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

print file_name
new_name = file_name.split('.')
new_name = new_name[0] + "_list." + new_name[1]
print new_name
try:
	f = open(new_name, 'w')
	for k, v in cities.iteritems():
		f.write(k + " " + v + "\n")
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

