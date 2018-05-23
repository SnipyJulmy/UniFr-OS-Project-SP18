ls
add 1 a
add 2 b
add 3 c
ls
read_k 1
read_k 3
read_k 2
read_v c
read_v b
read_v a
update_kv 3 b
update_kv 2 c
read_k 1
read_k 2
read_k 3
read_v b
read_v c
read_v a
rm_k 2
read_k 2
rm_v b
read_k 3
update_kv 1 d
read_k 1
read_v d
rm_k 1
ls