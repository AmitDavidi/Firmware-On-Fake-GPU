          +----------------+
          |      IDLE      |
          | STATUS = IDLE  |
          +----------------+
                   |
    CONTROL.START && CMD_TAIL != CMD_HEAD
                   v
          +----------------+
          |      BUSY      |
          | STATUS = BUSY  |
          +----------------+
           |            |
    Command Done      Command Error
           |             |
           v             v
+-----------------+  +----------------+
| IDLE (Success)  |  | ERROR           |
| STATUS=IDLE     |  | STATUS=ERROR    |
| CMD_HEAD updated|  | IRQ_STATUS set |
+-----------------+  +----------------+
                            | 
                 FW clears IRQ + writes RESET
                            |
                            v
                    +----------------+
                    |      IDLE      |
                    | STATUS=IDLE    |
                    +----------------+
