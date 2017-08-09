dofile("C:\\pres\\xenoscan\\demos\\helpers.lua")

-- attach to the process
local pid = executProcess("ComposableDemo.exe")
local p = Process(pid)
wait()

-- define the structure we're scanning for
local attr = struct(
	uint8("stage"),
	uint8("level"),
	int16("x"),
	int16("y"),
	uint32("health"),
	uint32("maxHealth")
)

attr.stage = 1 -- 1, {}
attr.level = 5
attr.x = 0
attr.y = 10 -- 10, range(5, 15)
attr.health = 95
attr.maxHealth = 100 --100, range(95, 105)

-- run the scan
p:newScan()
p:scanFor(attr)

-- show some details
local r = p:getResults()
local addr = next(r, nil)
print(table.show(addr, "address"))

-- update the values we want to change
attr.x = 19
attr.y = 15

-- write memory
p:writeMemory(addr, attr)
print("HACKED MEMORY! Check game")

-- kill the process
wait()
killProcess(pid)
os.exit()