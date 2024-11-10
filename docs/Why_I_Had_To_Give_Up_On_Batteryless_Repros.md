# Why I had to give up on batteryless Reproduction Cartridges

## Introduction

One of the things I was looking forward to support in PokeMe64 is reproduction cartridges. Even though I personally own original cartridges, I realize that these have become fairly expensive. I therefore did not want to leave out users who chose the financially more reasonable route of buying a reproduction cartridge.

Alas, I've hit a bump in the road I fear I'm unable to overcome. (more on that later)

This is a bit of a bummer to me. Because instead of making repro carts slightly more "feature complete" by allowing you to get these event pokémon with PokeMe64 on those as well, I have ended up validating the cost of buying an original cartridge instead.

Well, I suppose that is not entirely true. Reproduction carts which actually HAVE battery-backed SRAM and which therefore don't use any of these batteryless save patches probably already work out of the box with PokeMe64 already, because they'd be indistinguishable from the real thing from the Nintendo 64 Transfer Pak's perspective. Those likely also work with the stadium games.

In fact, I've read posts that people have managed to convert a batteryless reproduction/bootleg to a battery-backed one by soldering a battery and re-flashing the repro cart with a cartridge flasher such as GBxCart RW. If this can be done, those converted reproduction carts should theoretically work with PokeMe64 (AND the stadium games).

## How reproduction cartridges work

As you may or may not have read already, reproduction cartridges usually don't have a battery embedded on the cartridge board. Instead, they have specific patches applied to use unused sections of the rom data to store the save data instead. That means that these cartridges actually re-flash part of their rom to store the save data when you try to save in-game. And when the game is starting, it copies the save data from flash to SRAM. (since there's no battery, SRAM loses its content every time the gameboy powers off). 

The patches applied to these repros are so-called "batteryless save patches". In fact, there's a whole community out there (check the [BennVenn discord](https://discord.gg/KYxpSRgB)) that is actually backing up such patches and writing new ones, because they like having cartridges that don't require battery replacements.

## Flashing reproduction cartridges

Now, while you might think writing to a flashable cartridge would be a simple matter of trying to write to gameboy rom space, that's just not the case.

Because the memory address ranges for reading gameboy rom data are not supposed to be writable, they are actually reused for different things when writing to the same addresses.

For instance, the memory range 0x0000-0x4000 would map to ROM bank 0 when you'd try to read it. However, on MBC3 cartridges, if you try to write to 0x0000-0x1FFF, you would change the RAM/RTC Enable register and writing to 0x2000-0x4000 would swap the current rom bank.

Instead, reproduction/bootleg cartridge types have specific commands to trigger erase/flash operations on their flash chip. So far, I've seen that those commands are triggered by writing specific values to specific memory addresses. And these memory addresses are actually overlapping with existing MBC3 registers.

People have managed to reflash entire bootleg/reproduction cartridges with completely different games (once again, check the [BennVenn discord](https://discord.gg/KYxpSRgB)) using some of the cartridge flashers (such as GbxCart RW). 

I'm not entirely sure how these cartridge flashers work. (I didn't research that in depth.) However, while they probably use the aforementioned commands, I have also seen that there are cartridges out there that may use Pin 31 (AUDIO IN) to put the cartridge in flash mode. Then again, the latter does not seem to be relevant, because the gameboy can't change the signal of PIN 31, so it would still need separate commands to trigger the flash/erase operations while running the cartridge in the gameboy.

## The problem(s)

### Non-showstoppers
During my research, I stumbled onto a couple of problems that could've made adding reproduction support fairly hard:

- There are multiple types of reproduction carts out there. All of these have different commands to trigger the flash operation to support the batteryless saves
- There may be multiple batteryless save patches out there, even for a single type of reproduction cartridge. That also means the save data is potentially stored at different locations in the rom data.

The problems above would make it hard, but not impossible (and these are therefore not showstoppers). 

In fact, I found [this](https://github.com/tyllmoritz/game-boy-batteryless-patcher/blob/0fbe8fecc8dc4b7ff0cdc9d7f7a3c19156798eab/src/batteryless.asm) which seems to deal with most common reproduction cart type (WR/AAA/AA, WR/AAA/A9, WR/555/AA, WR/555/A9). In fact, when I compared my own dump of a reproduction cartridge, which I borrowed from my brother, to this repository code, I noticed the commands in this repro were included in this repository. So if I'd just write my code based on this repository, I thought that would give me support for the most common reproduction cartridges.

And for figuring out the exact save data location, I could've made PokeMe64 ask for the trainer name and trainer id to search the rom for the right location.

So I already had some ideas on how to overcome the aforementioned challenges.

### The showstopper

There is 1 big limitation for the Nintendo 64 Transfer Pak: It can only write blocks of 32 bytes starting at 32-byte aligned memory addresses. (= memory addresses which are mathematically divisible by 32).

As I mentioned earlier, the flash/erase commands for these reproduction cartridges require you to write to a (single-byte) memory address which overlaps with an existing MBC3 register. So a 32-byte block write would unintentionally write to a different register (thus trigger a different command) as well.

But wait... There's more!

During my research I also stumbled onto [this blogpost](https://dhole.github.io/post/gameboy_cartridge_rw_1/). What is of particular interest there is his screenshot of the datasheet of the MX29LV320AT/B cartridge. I'm not a hardware engineer, but my interpretation of this sheet is that - in order to trigger these commands -, you don't just need to write the right values at the right addresses in possibly the right sequence. But since it specifically mentions "bus cycles", it means these writes also need to be done with a specific timing.

But since the Nintendo 64 Transfer pak can only do 32-byte block writes, I can forget about being able to finish any of these writes in a single "bus cycle". (not to mention that the accidental write to neighbouring memory addresses may just break the sequence).

This is not just theoretical. I have tried writing code to simulate the batteryless patch that I dumped from my brothers' reproduction cart. But I also tried coding the wram\_bootleg\_read\_identifier method of [https://github.com/tyllmoritz/game-boy-batteryless-patcher/blob/0fbe8fecc8dc4b7ff0cdc9d7f7a3c19156798eab/src/batteryless.asm](https://github.com/tyllmoritz/game-boy-batteryless-patcher/blob/0fbe8fecc8dc4b7ff0cdc9d7f7a3c19156798eab/src/batteryless.asm). Neither of those attempts did what they were supposed to do.

## Conclusion

Because of the aforementioned 32-block write limitation of the Nintendo 64 Transfer Pak, I'm afraid I won't be able to add support for batteryless reproduction cartridges.

