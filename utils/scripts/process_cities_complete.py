#!/usr/bin/env python
#coding=utf-8

import sys
import os

class Cities(object):
	def getKey(self):
		return self.id;

	def getValue(self):
		return self

	def __init__(self, id, name, lat, long):
		self.id = id
		self.name = name
		self.lat = lat
		self.long = long

if len(sys.argv) < 3:
	print len(sys.argv)
	raise Exception("Missing file input.")

file_name = sys.argv[1]
coords_file_name = sys.argv[2]

lines = list()
coord_dict = {}
num_cities = 0

try:
	f = open(coords_file_name, 'r')
	for line in f:
		l = line.split(' ')
		city = Cities(l[0], l[1], l[2], l[3])
		coord_dict[city.getKey()] = city
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

try:
	f = open(file_name, 'r')
	for line in f:
		lines.append(line)
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()

new_name = file_name.rsplit('/', 1)[-1]
file_path = file_name.rsplit('/',1)
new_name = new_name.split('.')
new_name = file_path[0] + "/" +new_name[0] + "_complete." + new_name[1]
print new_name

try:
	f = open(new_name, 'w')
	for line in lines:
		l = line.split(' ')
		count = len(l)
		if (count > 3):
			f.write(
				l[0] + " " +
				str(l[1]).strip() + " " +
				str(coord_dict[l[0]].getValue().lat).strip() + " " + str(coord_dict[l[0]].getValue().long).strip() + " " +
				l[2] + " " +
				str(l[3]).strip() + " " +
				str(coord_dict[l[2]].getValue().lat).strip() + " " + str(coord_dict[l[2]].getValue().long).strip() + " " +
				l[4]
				)
		else:
			f.write(
				l[0] + " " +
				str(coord_dict[l[0]].getValue().name).strip() + " " +
				str(coord_dict[l[0]].getValue().lat).strip() + " " + str(coord_dict[l[0]].getValue().long).strip() + " " +
				l[1] + " " +
				str(coord_dict[l[1]].getValue().name).strip() + " " +
				str(coord_dict[l[1]].getValue().lat).strip() + " " + str(coord_dict[l[1]].getValue().long).strip() + " " +
				l[2]
				)
except IOError as e:
	print "I/O Error({0}): {1}".format(e.errno, e.strerror)
	raise
else:
	f.close()
