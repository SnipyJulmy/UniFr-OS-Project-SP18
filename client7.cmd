ls
add 15 a
add 25 b
add 35 c
ls
read_k 15
read_k 35
read_k 25
read_v c
read_v b
read_v a
update_kv 35 b
update_kv 25 c
read_k 15
read_k 25
read_k 35
read_v b
read_v c
read_v a
rm_k 25
read_k 25
rm_v b
read_k 35
update_kv 15 d
read_k 15
read_v d
rm_k 15
ls