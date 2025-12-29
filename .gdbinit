set disassembly-flavor intel

define pheap_list
	set $h = (struct heap *)$arg0
	set $i = 0
	while ($h != 0)
		printf "[%d] %p end=%p size=%lu bwd=%p fwd=%p\n", \
		       $i, $h, (char *)$h + ($h->size & 0xFFFFFFFFFFFFFFF8), $h->size, $h->bwd, $h->fwd
		set $h = $h->fwd
		set $i = $i + 1
	end
end


define pchunk
	set var $n = $arg0
	if $n->size & 0b001 != 0
		printf "address = %p | size = %lu | prealloc = %lu | prev_used = %lu\n", $n, $n->size & 0xFFFFFFFFFFFFFFF8, $n->size & 0b010, $n->size & 0b100
	else
		printf "bwd = %p | address = %p | fwd = %p | size = %lu | prealloc = %lu | prev_used = %lu\n", $n->bwd, $n, $n->fwd, $n->size & 0xFFFFFFFFFFFFFFF8, $n->size & 0b010, $n->size & 0b100
	end
end

define plist
	set var $n = $arg0
	while $n
		pchunk $n
		set var $n = $n->fwd
	end
end
