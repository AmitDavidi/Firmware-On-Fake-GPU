# GPU Full State Machine


IDLE --> CONTROL.START --> HW compares CMD_TAIL != CMD_HEAD --> Hw Exectutes Command --> **BUSY**

BUSY --> Command Done -> IDLE

BUSY --> Command encountered Error mid execution --> IRQ_STATUS Set


