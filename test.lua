local ltrace = require "ltrace"

function a(arg1, arg2)
	local a000 = false
	local a111 = 111
	local a222 = "222"
	local a333 = "333"
	local f = function() print("a") end
	local t =  {
		1, 2, 45, 5, 4,672,
		submap = {"aa", "bb"},
		ff = function() 
			print("123") 
		end,
		s = "12",
	}
	nondefined_var = nondefined_var + 1
end

function b()
	local bt = {1,5,256,"234", a = {b = "v"}}
	local bs = "this is a string"
	local bn = 4234239
	a()
end

xpcall(function()
	b()
end, function(err)
	io.stderr:write(err .. "\n")
	io.stderr:write(ltrace.getfulltrace() .. "\n")
	--[[
	local fullstack = ltrace.getstack()
	local list = ltrace.dumpstack(fullstack)
	table.foreach(list, function(i, v)
		print(v)
	end)
	--]]
end)

print('-----------------------------')
function xxx(a, b, cb)
	local c = a + b
	cb(c)
	print(ltrace.getfulltrace())
end
function yyy(cb)
	xxx(1, 2, cb)
end

yyy(print)
