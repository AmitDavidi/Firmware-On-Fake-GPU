Why is using `bool` inside a hardware register struct dangerous?

Because FW often assumes each register field is a well-defined width (usually 32 bits), but the compiler is free to implement `bool` as 1 byte with padding or even 4 bytes.  
This can change the struct layout, add hidden padding, and make the FW view of offsets differ from the HW MMIO map.


Why does `volatile` not solve data races?

`volatile` only tells the compiler “do not optimize away loads/stores; always touch memory”.  
It does **not** give any atomicity, mutual exclusion, ordering, or visibility guarantees between threads, so read-modify-write races and coherence problems are still possible.


Why can padding break MMIO layouts?

If the compiler inserts padding between struct fields, the in-memory offsets of members no longer match the fixed offsets in the HW register map.  
FW may think a register is at offset X, while HW actually decodes it at offset Y, causing reads/writes to hit the wrong registers.

Why is `static_assert` a firmware best practice?
`static_assert` lets FW verify assumptions (like struct size, field alignment, constants, ABI expectations) **at compile time**.  
That way, if the compiler or platform changes something (e.g., padding or type sizes), the build fails instead of silently producing firmware that does not match the HW contract.