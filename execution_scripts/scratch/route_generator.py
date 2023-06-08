import sys
from geopy.distance import geodesic

output_file = "airport_sim3.py"

output = open(output_file, "w")

class Airport:
    def __init__(self, name, symbol, location):
        self.name = name
        self.symbol = symbol
        self.location = location
        self.next_connection_point = 0

    def get_next_connector(self):
        val = self.next_connection_point
        self.next_connection_point = self.next_connection_point + 1
        return val

class Route:
    def __init__(self, source, dest):
        self.source = source
        self.dest = dest

hubs = [
    Airport("Albuquerque", "ABQ", (35.039333, -106.610778)),
    Airport("Altanta", "ATL", (33.636667, -84.428056)),
    Airport("Baltimore_Washington", "BWI", (39.175278, -76.668333)),
    Airport("Boston", "BOS", (42.363056, -71.006389)),
    Airport("Charlotte_Douglas", "CLT", (35.213889, -80.943056)),
    Airport("Chicago", "ORD", (41.978611, -87.904722)),
    Airport("Chicago", "MDW", (41.786111, -87.7525)),
    Airport("Cincinnati_Northern_Ketucky", "CVG", (39.048889, -84.667778)),
    Airport("Denver", "DEN", (42.2125, -83.353333)),
    Airport("Dallas", "DTW", (32.896944, -97.038056)),
    Airport("Dulles", "IAD", (38.944444, -77.455833)),
    Airport("Eppley", "OMA", (41.3, -95.895)),
    Airport("George_Bush", "IAH", (29.984444, -95.341389)),
    Airport("Harry_Reid", "LAS", (36.08, -115.152222)),
    Airport("Indianapolis", "IND", (39.717222, -86.294444)),
    Airport("John_Wayne", "SNA", (33.675556, -117.868333)),
    Airport("Minneapolis", "MSP", (44.881944, -93.221667)),
    Airport("Milwaukee", "MKE", (42.947222, -87.896667)),
    Airport("Newark_Liberty", "EWR", (40.6925, -74.168611)),
    Airport("Philadelphia", "PHL", (39.871944, -75.241111)),
    Airport("Phoenix", "PHX", (33.434167, -112.011667)),
    Airport("Portland", "PDX", (45.588611, -122.5975)),
    Airport("Los_Angeles", "LAX", (33.9425, -118.408056)),
    Airport("New_York", "LGA", (40.775, -73.875)),
    Airport("New_York", "JFK", (40.639722, -73.778889)),
    Airport("Salt_Lake_City", "SLC", (40.788333, -111.977778)),
    Airport("San_Diego", "SAN", (32.733611, -117.189722)),
    Airport("San_Francisco", "SFO", (37.618889, -122.375)),
    Airport("Seattle_Tacoma", "SEA", (47.448889, -122.309444)),
    Airport("Tampa", "TPA", (27.979722, -82.534722)),


]

routes = list()

#ny_jfk.addParams({
#	"name" : "New_York_JFK",
#    "num_connectors" : 1,
#    "num_planes": 3
#})

#atl_mke = sst.Link("atlanta_milwaukee")
#atl_mke.connect((atlanta, "airport_connector_0", "4s"), (milwaukee, "airport_connector_0", "4s"))

output.write("import sst\n")
output.write("import os\n")
output.write("script_path = os.path.dirname(os.path.abspath(__file__))\n")
output.write("sst.setProgramOption('timebase', '1s')\n\n\n")


for hub in hubs:
    fullname = hub.name + "_" + hub.symbol
    output.write(fullname + " = sst.Component('" + fullname + "', 'AirportSim.Airport')" + '\n')
    output.write(fullname + ".addParams({\n")
    output.write("\t'name' : '" + fullname + "',\n")
    output.write("\t'num_connectors' : " + str(len(hubs) - 1) + ",\n")
    output.write("\t'num_planes' : " + str((len(hubs) - 1) * 3)  + "\n")
    output.write("})\n\n")
    output.write('\n\n')            

for hub in hubs:
    fullname = hub.name + "_" + hub.symbol
    for dest in hubs:
        dest_fullname = dest.name + "_" + dest.symbol
        if fullname != dest_fullname:
            already_defined = False
            for route in routes:
                if (route.source == fullname and route.dest == dest_fullname) or (route.source == dest_fullname and route.dest == fullname):
                    already_defined = True

            if not already_defined:
                link_name = fullname + "_"+ dest_fullname
                distance =  int(geodesic(hub.location, dest.location).miles / 100)
                if distance == 0:
                    distance = 1 # damn you ny and chicago
                routes.append(Route(fullname, dest_fullname))
                output.write(link_name + " = sst.Link('" + link_name + "')\n")
                output.write(link_name + ".connect((" + fullname + ", 'airport_connector_" + str(hub.get_next_connector()) + "', '" + str(distance) + "s'), (" + dest_fullname + ", 'airport_connector_" + str(dest.get_next_connector()) + "', '" + str(distance) + "s'))\n")                

            
output.close()
