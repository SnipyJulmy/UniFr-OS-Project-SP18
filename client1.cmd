ls
add 1 a
add 2 b
add 3 c
ls
read_v 1
read_v 3
read_v 2
read_k c
read_k b
read_k a
update_kv 3 b
update_kv 2 c
read_v 1
read_v 2
read_v 3
read_k b
read_k c
read_k a
rm_k 2
read_v 2
rm_v b
read_v 3
update_kv 1 d
read_v 1
read_k d
rm_k 1
ls