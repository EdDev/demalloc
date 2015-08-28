# demalloc

# What is it?
A minimalist memory hook library, aimed to allow specialized memory management control.

# Why?
Applications with high dynamic memory allocation rate are hard to debug and controlled in the memory space.
Existing tools are usually too "heavy" for running such applications.
Therefore, a light lib version, with extendability options has been created: demalloc.

# How?
Calls to memory allocators and deallocator are hijaked to replacers in the demalloc lib.
Per initial setup, hooks may be placed before and after original system allocator (or deallocator).

# Common Use Cases
- Detect memory overflows.
- Add debug metadata to allocated memory.
- Embed specialized logic to memory allocation.
- Replace system alloctors with private ones (like memory pools).
