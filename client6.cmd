ls
add 14 a
add 24 b
add 34 c
ls
read_v 14
read_v 34
read_v 24
read_k c
read_k b
read_k a
update_kv 34 b
update_kv 24 c
read_v 14
read_v 24
read_v 34
read_k b
read_k c
read_k a
rm_k 24
read_v 24
rm_v b
read_v 34
update_kv 14 d
read_v 14
read_k d
rm_k 14
ls