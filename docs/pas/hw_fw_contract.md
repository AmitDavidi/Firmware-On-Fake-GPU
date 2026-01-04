# Hw - Fw Contract

In this doc we'll specify the interfaces between hardware and FW, **including command submission semantics**.

---

## Register Map

| Offset | Name       | R/W | Written By | Description              |
| ------ | ---------- | --- | ---------- | ------------------------ |
| 0x00   | STATUS     | R   | HW         | IDLE / BUSY / ERROR      |
| 0x04   | CMD_HEAD   | R   | HW         | Command queue head       |
| 0x08   | CMD_TAIL   | W   | FW         | Command queue tail       |
| 0x0C   | IRQ_STATUS | R/W | HW/FW      | Interrupt reason         |
| 0x10   | CONTROL    | W   | FW         | START / RESET            |
| 0x14   | CMD_ID     | W   | FW         | Command ID to enqueue    |
| 0x18   | DOORBELL   | W   | FW         | Signal HW to process CMD |

---

### STATUS - Written by HW, Readable by FW

* IDLE: no command executing
* BUSY: command in progress
* ERROR: fatal error, requires RESET

---

### CMD_TAIL / CMD_ID / DOORBELL - Written by FW, Readable by HW

* **CMD_TAIL**: index in command queue; incremented by FW to indicate a new submission
* **CMD_ID**: the actual command value to enqueue in HW FIFO
* **DOORBELL**: pulse (write 1) to tell HW to snapshot CMD_TAIL and CMD_ID
* **Rule:** FW writes CMD_ID first, then increments CMD_TAIL, then pulses DOORBELL

> **Important:** HW maintains its **internal FIFO**. FW sees only registers. Writing CMD_TAIL alone does **not** put a command in FIFO – HW is responsible for copying CMD_ID to internal queue.

---

### IRQ_STATUS - Risen by HW, Cleared by FW

* bit0: command completed
* bit1: error
* FW must clear bits by writing 1 to the corresponding bit

---

### CONTROL

* **START**: begin processing commands – bit 0
* **RESET**: clears internal HW state, CMD_HEAD = CMD_TAIL

> Multiple writes allowed. RESET while BUSY aborts current command.

---

## Error Flows

| Scenario                                       | HW Action                                     | STATUS                 | CMD_HEAD / CMD_TAIL                        | IRQ_STATUS                           | Notes                                       |
| ---------------------------------------------- | --------------------------------------------- | ---------------------- | ------------------------------------------ | ------------------------------------ | ------------------------------------------- |
| FW writes START while BUSY                     | DROP request, start next command              | BUSY (current command) | CMD_HEAD unchanged, CMD_TAIL updated by FW | No change                            |                                             |
| FW writes START before ERROR cleared           | Keep ERROR, start new command                 | ERROR                  | CMD_HEAD unchanged, CMD_TAIL updated       | IRQ_STATUS set (from previous error) | FW must CLEAR + RESET to reset ERROR        |
| RESET during BUSY                              | DROP current command                          | IDLE                   | CMD_HEAD unchanged (or reset)              | IRQ_STATUS cleared                   | Command aborted                             |
| Command encounters Error mid-execution         | Stop command execution                        | ERROR                  | CMD_HEAD not updated                       | IRQ_STATUS set                       | FW must clear IRQ + write RESET to continue |
| FW overwrites CMD_TAIL while BUSY but no START | Command waits, HW continues current           | BUSY                   | CMD_TAIL overwritten by FW                 | No change                            | Pending command will execute on next START  |
| RESET during ERROR                             | Clear error, reset HW state                   | IDLE                   | CMD_HEAD, CMD_TAIL cleared                 | IRQ_STATUS cleared                   | Ready for next command                      |
| Multiple STARTs queued before processing       | Only first START triggers action, others DROP | BUSY (first command)   | CMD_HEAD/Tail managed as usual             | IRQ_STATUS only from errors          | Prevents unintended multiple execution      |
| FW writes CMD_ID without pulsing DOORBELL      | Command not enqueued                          | HW IDLE                | CMD_TAIL unchanged                         | No change                            | HW ignores CMD_ID until DOORBELL            |
| FW pulses DOORBELL                             | HW snapshots CMD_ID into internal FIFO        | BUSY or IDLE           | CMD_HEAD unchanged, CMD_TAIL advanced      | No change                            | Ensures HW FIFO receives correct command    |

---

### Notes

* **HW FIFO is the source of truth** for commands; FW only writes CMD_ID, CMD_TAIL, and DOORBELL
* **CMD_TAIL** is an index, **not a direct write into the FIFO**
* **DOORBELL** triggers HW to copy CMD_ID into FIFO at the proper index
* **STATUS** reflects HW state (IDLE / BUSY / ERROR)
* **CMD_HEAD** updated only by HW after successful command completion
* **IRQ_STATUS** set by HW on error or completion
* **CONTROL.START** triggers HW to check CMD_TAIL vs CMD_HEAD and execute queued commands
* **CONTROL.RESET** clears ERROR, optionally aborts BUSY command

---

אם אתה רוצה, אני יכול גם לשרטט **ASCII diagram של זרימת הפקודות FW → FIFO HW → STATUS** שיבהיר בדיוק מה FW רואה ומה HW מנהל פנימית.

רוצה שאעשה את זה?
