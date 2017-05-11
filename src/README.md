# ltrace
# Description

a little like debug.traceback(), which gives you more info, such as every varname and value in each lua call stack frame.


## API

```
ltrace.traceback([co,]
		 is_show_var,
		 buff_sz,
		 top_frame_max_count,
		 bottom_frame_max_count)
```

### Args

is_show_var : default == true

buff_sz : default == (1024 * 1024) Bytes

top_frame_max_count : default == 15

bottom_frame_max_count : default == 10


