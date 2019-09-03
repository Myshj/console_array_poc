# Standalone byte array

Static array of bytes with access through `standard input`.

Capacity is defined in `main.c` file (currently ~5GB) and in `CMakeLists.txt` (because everything is allocated on stack).

Has binary protocol and understands the next commands:

| #  | Command | Length (in bytes) | Reply length (in bytes) |
| ---|---------| ---------------|--------------|
| 1  | Stop  | 1 | 1|
| 2  | Get size  | 1| 8|
| 3  | Get element at position  | 2| 2|
| 4  | Set element at position  | 3| 1|

Each command has its structure and is followed by a reply.

# Commands

## Stop
Format: `0x03`.

Reply format: `0x03`.

Stops the service.

## Get size

Format: `0x02`.

Reply format: `<8 bytes of size>`

## Get element at position

Format: `0x00<8 bytes of position>`

Reply format (success): `0x04<1 byte of element>`

Reply format (error): `0x05<1 byte of error code>`

## Set element at position

Format: `0x01<8 bytes of position><1 byte of element>`

Reply format (success): `0x06`

Reply format (error): `0x07<1 byte of error code>`

# Error handling

This program receives commands from the `standard input`.

That means, you can redirect any file to it with so called `pipelining` (e.g. `program.exe < file.command`).

It works until `EOF` is reached, stops after that with the specified `exit code`.

If `EOF` is reached during command handling, the reply with the specified error will be sent and program will stop after that.

If the program is started without pipelined input, it will stop immediately, as the standard input is empty in such a case.

# Possible improvements

## Check endianness

Big endian in Windows environment (I think so in any case).

Should be the same for all envs.

## Check input/output modes

I'm not sure they are set to `binary` properly.

## Work with `sockets` instead of `standard input`

So that this program is more like a service.

Interesting but not simple, because you may want to store client connections somehow to avoid expensive TCP handshakes, address resolving and other stuff.

## Generate the program instead of writing it manually

So that you can store many datatypes.

## Work with errors better

So that there may be nothing unrecoverable from.

# Examples

In the `examples` folder.

`setAt.command` contains actually 3 commands:

1. Get the current value
2. Reassign it
3. Get it again to see if something changed
