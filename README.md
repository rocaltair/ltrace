# ltrace

## Description 

```How To Debug With Lua, Lots of print?```

```We don't need print() any more!```

Instead of debug.traceback(), this lib gives you more 
infomations of every lua call stack frame.

In C++ or Java, you just use 

```
try {
	// statements
} catch(e) {
	// statements
}

```

while in Lua, you need xpcall like this:

```
function TryFunc()
	-- statement
end

function CatchFunc(err)
	-- statement
	print(err)
	print(debug.traceback())
end

xpcall(TryFunc, CatchFunc)
```

in CatchFunc, we just call debug.traceback() to get stackframe
to locate your mistake.

ltrace.getfulltrace() will show you every variable(name and value)
of each frame(with function name, filename and linenum) in lua call stack



## How To Use?

```
local ltrace = require "ltrace"

function main()
	-- do call
end

xpcall(main, function(err)
	print(err)
	print(ltrace.getfulltrace())
end)

```

## Example

[test.lua](https://github.com/rocaltair/ltrace/blob/master/test.lua)

this is a sample of err catching dumping

```
test.lua:17: attempt to perform arithmetic on global 'nondefined_var' (a nil value)
0 @test.lua:31 
        err:"test.lua:17: attempt to perform arithmetic on global 'nondefined_var' (a nil value)"
        (*temporary):<function: 0x7ff76bc05060>
1 @test.lua:17 in <a()>
        a000:false
        a111:111
        a222:"222"
        a333:"333"
        f:<function: 0x7ff76bc11390>
        t:{[1]=1, [2]=2, [3]=45, [4]=5, [5]=4, [6]=672, ff=<function: 0x7ff76bc114a0>, s="12", submap={[1]="aa", [2]="bb"}}
        (*temporary):"attempt to perform arithmetic on global 'nondefined_var' (a nil value)"
2 @test.lua:24 in <b()>
        bt:{[1]=1, [2]=5, [3]=256, [4]="234", a={b="v"}}
        bs:"this is a string"
        bn:4234239
3 @test.lua:28 
```

