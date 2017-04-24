# ltrace

## Two implemetations
[ltrace.lua](ltrace.lua) pure lua
[src/ltrace.c](src/ltrace.c) pure C, see [src/test.lua](src/test.lua)

## Description 

```How To Debug With Lua, Lots of print?```

```We don't need print() any more!```

Instead of debug.traceback(), this lib gives you more 
infomations of every lua call stack frame.

[lua](http://www.lua.org/) Support

[luajit](http://luajit.org/) Support(with param names for function output)

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


## Output Format

Variables show in each frame are sorted by declaration order.

If you are using luajit or lua 5.2 later, param names will be showed in brackets. See Example

```
frameidx @filepath.lua:linenum in <functionName()>
	local_variable_name1:variable_value
	local_variable_name2:variable_value
	...
```

## How To Use?

```
local ltrace = require "ltrace"

function errordump(errstr)
	io.stderr:write(errstr .. "\n")
end

function main()
	-- do call
end

xpcall(main, function(err)
	errordump(err)
	errordump(ltrace.getfulltrace())
end)

```

## Example

[test.lua](https://github.com/rocaltair/ltrace/blob/master/test.lua)

this is a sample of err catching dumping (use luajit or lua 5.2 or later)

```
test.lua:17: attempt to perform arithmetic on global 'nondefined_var' (a nil value)
0 @test.lua:31 in <__add(err)>
        err:"test.lua:17: attempt to perform arithmetic on global 'nondefined_var' (a nil value)"
        (*temporary):<file (0x7fff734894a0)>
1 @test.lua:17 in <a(arg1, arg2)>
        arg1:"xx"
        arg2:"yy"
        a000:false
        a111:111
        a222:"222"
        a333:"333"
        f:<function: 0x00050568>
        t:{[1]=1, [2]=2, [3]=45, [4]=5, [5]=4, [6]=672, ff=<function: 0x000506a0>, s="12", submap={[1]="aa", [2]="bb"}}
        (*temporary):"attempt to perform arithmetic on global 'nondefined_var' (a nil value)"
2 @test.lua:24 in <b()>
        bt:{[1]=1, [2]=5, [3]=256, [4]="234", a={b="v"}}
        bs:"this is a string"
        bn:4234239
3 @test.lua:28 

4[C] : in <xpcall>
5[C] : in <main>
6[C] : in <C>
```


