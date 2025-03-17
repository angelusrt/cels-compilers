# cels-compilers
This is a respository containing cels static-analyser-compiler.

# Proposal

The proposal consists on bringing utilities to 
C in a documentation-comment-block way - call it CDocs.

``` C
/**
 * @property(self, vector)
 * @assert(typeof(*self.data) in [int, long, string])
 * @assert(sizeof(*self.data) == type_size)
 * @attribute(thread-safe, reentrant, non-blocking)
 * @proven(thread-safe, reentrant, non-blocking)
 */
 error vectors_init(void *self, size_t type_size, size_t capacity, allocator *mem)
 ```

``` C
switch(name) {
    /** @comptime(strings_prehash("option 0")) */
    case 0: return 0;

    /** @comptime(strings_prehash("option 1")) */
    case 1: return 1;

    /** @comptime(strings_prehash("option 2")) */
    case 2: return 2;
}
 ```
