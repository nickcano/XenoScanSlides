dofile("lib.lua")

PROC = Process("dolphin")

-- internal code to handle freezing
FROZEN_ADDRESSES = {}
FREEZE_TIMER = function()
	for address, val in pairs(FROZEN_ADDRESSES) do
		PROC:writeMemory(address, val)
	end
	settimeout(FREEZE_TIMER, 10)
end
FREEZE_TIMER()

-- functions which expose freezing functionality
function freeze(a, v)
	if (type(a) == 'table') then
		for _, address in ipairs(a) do
			freeze(address, v)
		end
	else
		FROZEN_ADDRESSES[a] = v
	end
end

function unfreeze(a)
	if (a) then
		freeze(a, nil)
	else
		FROZEN_ADDRESSES = {}
	end
end

-- addresses we're gonna freeze
HEALTH = { 0x804d782e, 0x81347024, 0x804d8490, 0x813468cc, 0x804d04e6, 0x81346722, 0x807057c6, 0x8134e60e, 0x80461b5a }


freeze100 = function() freeze(HEALTH, uint16(100)) end
freeze101 = function() freeze(HEALTH, uint16(101)) end
freeze1337 = function() freeze(HEALTH, uint16(1337)) end