# Hw - Fw Contract
In this doc we'll specify the interfaces between harware and Fw.


## Register Map: 
| Offset | Name       | R/W | Written By | Description               |
| ------ | ---------- | --- | ------- | ------------------- |
| 0x00   | STATUS     | R   | HW      | IDLE / BUSY / ERROR |
| 0x04   | CMD_HEAD   | R   | HW      | Command queue head  |
| 0x08   | CMD_TAIL   | W   | FW      | Command queue tail  |
| 0x0C   | IRQ_STATUS | R/W | HW/FW   | Interrupt reason    |
| 0x10   | CONTROL    | W   | FW      | START / RESET       |


STATUS - Written by Hw - Readable by Fw
- IDLE: no command executing
- BUSY: command in progress
- ERROR: fatal error, requires RESET

CMD_TAIL - Written by FW, Readable by HW
- Written by FW to submit a new command
- Writing CMD_TAIL while HW is BUSY is allowed


IRQ_STATUS - Risen by HW - FW Picks it up and clears
- bit0: command completed
- bit1: error

- FW must clear bits by writing 1 to the corresponding bit.


CONTROL:
- START: begin processing commands - bit 0
- RESET: clears internal HW state, CMD_HEAD=CMD_TAIL 

Multiple writes allowed.
RESET while BUSY aborts current command.


## Error Flows

| Scenario | HW Action | STATUS | CMD_HEAD / CMD_TAIL | IRQ_STATUS | Notes |
|----------|-----------|--------|-------------------|------------|-------|
| FW writes START while BUSY | DROP request, start next command | BUSY (current command) | CMD_HEAD unchanged, CMD_TAIL updated by FW | No change |  |
| FW writes START before ERROR cleared | Keep ERROR, start new command | ERROR | CMD_HEAD unchanged, CMD_TAIL updated | IRQ_STATUS set (from previous error) | FW must CLEAR + RESET to reset ERROR |
| RESET during BUSY | DROP current command | IDLE | CMD_HEAD unchanged (or reset) | IRQ_STATUS cleared | Command aborted |
| Command encounters Error mid-execution | Stop command execution | ERROR | CMD_HEAD not updated | IRQ_STATUS set | FW must clear IRQ + write RESET to continue |
| FW overwrites CMD_TAIL while BUSY but no START | Command waits, HW continues current | BUSY | CMD_TAIL overwritten by FW | No change | Pending command will execute on next START |
| RESET during ERROR | Clear error, reset HW state | IDLE | CMD_HEAD, CMD_TAIL cleared | IRQ_STATUS cleared | Ready for next command |
| Multiple STARTs queued before processing | Only first START triggers action, others DROP | BUSY (first command) | CMD_HEAD/Tail managed as usual | IRQ_STATUS only from errors | Prevents unintended multiple execution |

### Notes

- **STATUS** reflects HW state (IDLE / BUSY / ERROR)  
- **CMD_HEAD** updated only by HW after successful command completion  
- **CMD_TAIL** written by FW to submit new commands  
- **IRQ_STATUS** set by HW when ERROR occurs or command completes (if desired)  
- **CONTROL.START** triggers HW to check CMD_TAIL vs CMD_HEAD and execute commands  
- **CONTROL.RESET** clears ERROR, optionally aborts BUSY command
