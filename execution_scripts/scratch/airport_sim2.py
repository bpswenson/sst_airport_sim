import sst
import os

script_path = os.path.dirname(os.path.abspath(__file__))

sst.setProgramOption("timebase", "1s")

atlanta = sst.Component("Atlanta_ATL", "AirportSim.Airport")
atlanta.addParams({
	"name" : "Atlanta_ATL",
    "num_connectors" : 2,
    "num_planes": 1
})

milwaukee = sst.Component("Milwaukee_MKE", "AirportSim.Airport")
milwaukee.addParams({
	"name" : "Milwaukee_MKE",
    "num_connectors" : 1,
    "num_planes": 2
})

lax = sst.Component("Los_Angeles_LAX", "AirportSim.Airport")
lax.addParams({
	"name" : "Los_Angeles_LAX",
    "num_connectors" : 1,
    "num_planes": 3
})

atl_mke = sst.Link("atlanta_milwaukee")
atl_mke.connect((atlanta, "airport_connector_0", "4s"), (milwaukee, "airport_connector_0", "4s"))

atl_lax = sst.Link("atlanta_losangeles")
atl_lax.connect((atlanta, "airport_connector_1", "16s"), (lax, "airport_connector_0", "16s"))
