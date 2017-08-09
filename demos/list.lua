dofile("C:\\pres\\xenoscan\\demos\\helpers.lua")

-- attach to the process
local pid = executProcess("StdListDemo.exe")
local p = Process(pid)
wait()

-- scan for lists
local lists = p:findDataStructures("std::list")

-- locate the list
local listHead = 0
for _, data in ipairs(lists) do
	if (data.itemCount == 101) then
		listHead = data.identifier
	end
end

-- write memory
local firstEntry = p:readMemory(listHead, uint32)
p:writeMemory(firstEntry, 8, uint32(1))
print("HACKED LIST! Check window")


-- kill the process
wait()
killProcess(pid)
os.exit()
