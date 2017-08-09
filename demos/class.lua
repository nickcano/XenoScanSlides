dofile("C:\\pres\\xenoscan\\demos\\helpers.lua")

-- attach to the process
local pid = executProcess("ClassInstanceDemo.exe")
local p = Process(pid)
wait()

-- scan for classes
local classes = p:findDataStructures("Native Class Instance")

-- locate Nop and Win instances
local winVF = 0
local nopAddress = 0
for _, data in ipairs(classes) do
	val = p:readMemory(data.identifier, 4, uint32)
	if (val == 0x90) then
		nopAddress = data.identifier
	elseif (val == 0x1337) then
		winVF = data.VFTable
	end
end

-- write memory
p:writeMemory(nopAddress, uint32(winVF))
print("HACKED VF! Check window")


-- kill the process
wait()
killProcess(pid)
os.exit()
