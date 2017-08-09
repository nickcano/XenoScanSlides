dofile("C:\\pres\\xenoscan\\demos\\helpers.lua")

-- attach to the process
local pid = executProcess("StdMapDemo.exe")
p = Process(pid)
wait()

-- scan for maps
maps = p:findDataStructures("std::map")
map = {}
for _, v in ipairs(maps) do
	if v.itemCount == 5 then
		map = v
		break
	end
end

-- overwrite the map
local text = { "XenoScan", "was     ", "here    ", "biatch  ", "!       " }
function rewriteNodes(node, marker)
	node = ptrcast(node)
	if (node ~= marker) then
		local key = p:readMemory(node, 16, uint32)
		local val = text[key]
		if (val) then
			p:writeMemory(node, 20, ascii(val))
			p:writeMemory(node, 36, uint32(string.len(val)))
		end
		rewriteNodes(p:readMemory(node, 0, uint32), marker)
		rewriteNodes(p:readMemory(node, 8, uint32), marker)
	end
end
first = p:readMemory(map.identifier, 4, uint32)
rewriteNodes(first, map.identifier)

print("HACKED MAP! Check window")


-- kill the process
wait()
killProcess(pid)
os.exit()