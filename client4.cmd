ls
add 12 a
add 22 b
add 32 c
ls
read_v 12
read_v 32
read_v 22
read_k c
read_k b
read_k a
update_kv 32 b
update_kv 22 c
read_v 12
read_v 22
read_v 32
read_k b
read_k c
read_k a
rm_k 22
read_v 22
rm_v b
read_v 32
update_kv 12 d
read_v 12
read_k d
rm_k 12
ls