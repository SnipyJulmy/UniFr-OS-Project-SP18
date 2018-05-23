ls
add 13 a
add 23 b
add 33 c
ls
read_k 13
read_k 33
read_k 23
read_v c
read_v b
read_v a
update_kv 33 b
update_kv 23 c
read_k 13
read_k 23
read_k 33
read_v b
read_v c
read_v a
rm_k 23
read_k 23
rm_v b
read_k 33
update_kv 13 d
read_k 13
read_v d
rm_k 13
ls