set disassembly-flavor intel

define plist
	set var $n = $arg0
	while $n
		printf "bwd = %p | address = %p | fwd = %p | size = %lu\n", $n->bwd, $n, $n->fwd, $n->size & 0xFFFFFFFFFFFFFFF8
		set var $n = $n->fwd
	end
end
