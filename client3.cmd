ls
add 11 a
add 21 b
add 31 c
ls
read_v 11
read_v 31
read_v 21
read_k c
read_k b
read_k a
update_kv 31 b
update_kv 21 c
read_v 11
read_v 21
read_v 31
read_k b
read_k c
read_k a
rm_k 21
read_v 21
rm_v b
read_v 31
update_kv 11 d
read_v 11
read_k d
rm_k 11
ls