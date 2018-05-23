ls
add 11 a
add 21 b
add 31 c
ls
read_k 11
read_k 31
read_k 21
read_v c
read_v b
read_v a
update_kv 31 b
update_kv 21 c
read_k 11
read_k 21
read_k 31
read_v b
read_v c
read_v a
rm_k 21
read_k 21
rm_v b
read_k 31
update_kv 11 d
read_k 11
read_v d
rm_k 11
ls