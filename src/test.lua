ltrace = require "ltrace"

function bar(f)
	print(ltrace.traceback())
end

function foo(a, b, c)
	bar(print)
end

foo("s", {}, 234)
