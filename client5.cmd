ls
add 13 a
add 23 b
add 33 c
ls
read_v 13
read_v 33
read_v 23
read_k c
read_k b
read_k a
update_kv 33 b
update_kv 23 c
read_v 13
read_v 23
read_v 33
read_k b
read_k c
read_k a
rm_k 23
read_v 23
rm_v b
read_v 33
update_kv 13 d
read_v 13
read_k d
rm_k 13
ls