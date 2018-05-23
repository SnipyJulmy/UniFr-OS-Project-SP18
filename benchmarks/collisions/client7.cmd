ls
add 15 a
add 25 b
add 35 c
ls
read_v 15
read_v 35
read_v 25
read_k c
read_k b
read_k a
update_kv 35 b
update_kv 25 c
read_v 15
read_v 25
read_v 35
read_k b
read_k c
read_k a
rm_k 25
read_v 25
rm_v b
read_v 35
update_kv 15 d
read_v 15
read_k d
rm_k 15
ls