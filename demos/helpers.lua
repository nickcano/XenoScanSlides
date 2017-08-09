dofile("lib.lua")

PATH = "C:\\pres\\xenoscan\\demos\\"

function executProcess(name)
	os.execute("start cmd /c call " .. PATH .. name .. "")
	local pid = nil
	while (pid == nil) do
		f = io.popen("Wmic process where (Name like '%" .. name .. "%') get ProcessId")
		pid = tonumber(f:read("*a"):match("%d+"))
		f:close()
	end
	return pid
end

function killProcess(pid)
	os.execute("taskkill /F /PID " .. pid)
end

function wait()
	os.execute("pause")
end