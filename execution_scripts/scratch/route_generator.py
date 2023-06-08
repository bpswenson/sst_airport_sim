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
    Airport("Altanta", "ATL", (33.636667, -84.428056)),
    Airport("Boston", "BOS", (42.363056, -71.006389)),
    Airport("Chicago", "ORD", (41.978611, -87.904722)),
    Airport("Chicago", "MDW", (41.786111, -87.7525)),
    Airport("Denver", "DEN", (42.2125, -83.353333)),
    Airport("Dallas", "DTW", (32.896944, -97.038056)),
    Airport("Minneapolis", "MSP", (44.881944, -93.221667)),
    Airport("Milwaukee", "MKE", (42.947222, -87.896667)),
    Airport("Los_Angeles", "LAX", (33.9425, -118.408056)),
    Airport("New_York", "LGA", (40.775, -73.875)),
    Airport("New_York", "JFK", (40.639722, -73.778889)),
    Airport("Salt_Lake_City", "SLC", (40.788333, -111.977778)),
    Airport("San_Francisco", "SFO", (37.618889, -122.375)),
    Airport("Seattle_Tacoma", "SEA", (47.448889, -122.309444)),

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
    output.write("\t'num_planes' : " + str((len(hubs) - 1) * 2)  + "\n")
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
