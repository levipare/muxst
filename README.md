# Mutiplexed Status

**muxst** is a simple utility for combining the continuous output of several subprocesses into a single formatted outputted.
The subprocesses must output newline deliminated strings.

> Works well with [swab](https://github.com/levipare/swab)

## Building
```sh
git clone https://github.com/levipare/muxst
cd muxst
make
```

## Usage
```sh
# muxst <format> <cmd1> [cmd2...]
muxst "%s | %s" "while true; do free -g | awk '/^Mem:/ {print \$3 \"/\" \$2 \"GiB\"}'; sleep 10; done" "while true; do date; sleep 1; done"
# 5/27GiB | Sun Sep  7 12:37:52 PM PST 2025
```
