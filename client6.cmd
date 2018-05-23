ls
add 14 a
add 24 b
add 34 c
ls
read_k 14
read_k 34
read_k 24
read_v c
read_v b
read_v a
update_kv 34 b
update_kv 24 c
read_k 14
read_k 24
read_k 34
read_v b
read_v c
read_v a
rm_k 24
read_k 24
rm_v b
read_k 34
update_kv 14 d
read_k 14
read_v d
rm_k 14
ls