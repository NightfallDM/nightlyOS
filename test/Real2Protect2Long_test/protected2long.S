# Now we are in the protected mode
# So, switch to the long mode right now

# The Order(protected mode 2 long mode)(with PAGE)

# Note that the "PAGE" is needed when you get into the long mode
# because the long mode use the "flat segment".
# It means that there is not segment protected ,So the "PAGE"
# protected mechanism is needed.

# 1. starting from protected mode, disable paging
# 2. enable "PAE"
# 3. load "PML4/5"(LEVEL 4/5 page map table) into CR3
# 4. enable IA-32e mode by setting IA32_EFER.LME = 1
# 5. enable paging

.code32
.global set_long_mode
set_long_mode:
	
