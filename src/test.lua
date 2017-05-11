ltrace = require "ltrace"

function bar(f1, f2)
	print(ltrace.traceback())
end

function foo(a, b, c)
	bar(print, foo)
end

foo("s", {}, 234)
