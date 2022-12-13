# This works as long as the flags used in the GNU module are identical with their Clang equivalents
include(HerdFlagsGNU)

# Sanitiser
# clang does not recognise the flag -static-libasan used by g++ for statically linking ASan
set(LINKER_CXX_SANITISER_FLAGS -fsanitize=address -fsanitize=undefined)

