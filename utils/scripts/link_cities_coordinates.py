import sys
import json

if len(sys.argv) < 3:
    print len(sys.argv)
    raise Exception("Missing file input.")

cities_file_name = sys.argv[1]
coords_file_name = sys.argv[2]

city_lines = list()
city_coords = dict()
num_cities = 0

#Grab city names and IDs
try:
    f = open(cities_file_name, 'r')
    for line in f:
        city_lines.append(line)
except IOError as e:
    print "I/O Error({0}): {1}".format(e.errno, e.strerror)
    raise
else:
    f.close()

try:
    f = open(coords_file_name, 'r')
    data = json.load(f)
    for city_data in data:
        print "Storing: {0} -> ({1},{2})".format(city_data["city"].replace(" ", "_"), city_data["latitude"], city_data["longitude"])
        city_coords[city_data["city"].replace(" ", "_")] = (city_data["latitude"],city_data["longitude"])
except IOError as e:
    print "I/O Error({0}): {1}".format(e.errno, e.strerror)
    raise
else:
    f.close()

new_name = "cities_ids_coords.txt"
try:
    f = open(new_name, 'w')
    for line in city_lines:
        l = line.split(' ')
        #print "Checking: {0} {1}".format(l[0], l[1].rstrip())
        if city_coords.has_key(l[1].rstrip()):
            f.write(
            l[0] + " " +
            l[1].rstrip() + " " +
            str(city_coords[l[1].rstrip()][0]) + " " +
            str(city_coords[l[1].rstrip()][1]) + " " +
            "\n"
            )
        else:
            print l[0] + " " + l[1].rstrip() + ""
except IOError as e:
    print "I/O Error({0}): {1}".format(e.errno, e.strerror)
    raise
else:
    f.close()
