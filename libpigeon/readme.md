## libpigeon

Compiler monitoring library.

Every `open` and `read` will be monitored by this library,
and the suspicious ones will be blocked.

### Usage

```bash
$ LD_PRELOAD=libpigeon/libpigeon.so g++ a.cpp
```

### Configuration

Text file `libpigeon/config.txt`:

Each line consists of one string starting with '+' or '-'.
'+' means allow and '-' means deny.

The other part of the string stands for the path.
There can be at most one '*' in the path.

The rules in config are executed from top to bottom.
If there's no rules matching, the read access will be blocked.
